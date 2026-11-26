document.addEventListener('DOMContentLoaded', () => {
    // UI Elements
    const toggleLightBtn = document.getElementById('toggle-light-btn');
    const btnText = document.getElementById('btn-text');
    const aiStatus = document.getElementById('ai-status');
    const aiStatusText = document.getElementById('ai-status-text');
    const aiProgressBar = document.getElementById('ai-progress-bar');
    const aiProgressContainer = document.getElementById('ai-progress-container');
    const severityBadge = document.getElementById('severity-badge');
    const confidenceVal = document.getElementById('confidence-val');
    const statusVal = document.getElementById('status-val');
    const streamImg = document.getElementById('camera-stream');
    const recommendation = document.getElementById('recommendation');
    const deviceStatus = document.getElementById('device-status');
    const deviceStatusText = document.getElementById('device-status-text');
    
    // Set date
    document.getElementById('date-display').textContent = new Date().toLocaleDateString('en-US', { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric' });

    const captureBtn = document.getElementById('capture-btn');
    const captureBtnText = document.getElementById('capture-btn-text');
    const streamBadge = document.getElementById('stream-badge');

    // State Variables
    let model = null;
    let lightOn = true; // Firmware defaults LED to ON
    let isFrozen = false;
    let isProcessing = false;
    let isPollingTrigger = false;
    let pendingTrigger = false;

    // --- TOGGLE LIGHT ---
    toggleLightBtn.addEventListener('click', async () => {
        lightOn = !lightOn;
        
        // Update UI immediately for responsiveness
        if (lightOn) {
            toggleLightBtn.classList.add('active');
            btnText.textContent = "Turn Off Ring Light";
        } else {
            toggleLightBtn.classList.remove('active');
            btnText.textContent = "Turn On Ring Light";
        }

        try {
            await fetch('/api/light', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ state: lightOn ? "on" : "off" })
            });
        } catch (err) {
            console.error("Failed to toggle light", err);
        }
    });

    // --- AI MODEL INITIALIZATION ---
    async function initModel() {
        captureBtn.disabled = true;
        captureBtnText.textContent = "Loading AI Engine...";
        if (aiStatusText) aiStatusText.textContent = "Checking Cache...";
        if (aiProgressBar) aiProgressBar.style.width = "0%";

        const MODEL_CACHE_KEY = 'indexeddb://dermacam-burn-model-v2';

        try {
            // Remove legacy 3-class model cache if present
            try {
                await tf.io.removeModel('indexeddb://dermacam-burn-model');
            } catch (_) {}

            // 1. Try loading directly from client's IndexedDB storage (instant, zero Wi-Fi traffic)
            try {
                console.log("Checking client IndexedDB for cached model...");
                model = await tf.loadLayersModel(MODEL_CACHE_KEY);
                const outShape = model.outputs && model.outputs[0] ? model.outputs[0].shape : null;
                if (!outShape || outShape[outShape.length - 1] !== 4) {
                    throw new Error(`Cached model output shape mismatch: ${JSON.stringify(outShape)}`);
                }
                console.log("Model successfully loaded from IndexedDB cache!");
                if (aiProgressBar) aiProgressBar.style.width = "100%";
            } catch (cacheErr) {
                console.log("Model not found or outdated in IndexedDB. Downloading from ESP32 /model.json...", cacheErr);

                // Verify that shard binary files exist in ESP32 SPIFFS partition
                try {
                    const testShard = await fetch('/group1-shard1of3.bin');
                    if (!testShard.ok) {
                        throw new Error(`AI model file not found (HTTP ${testShard.status}). Please run 'idf.py storage-flash' in your ESP-IDF terminal.`);
                    }
                } catch (probeErr) {
                    throw probeErr;
                }

                // 2. Fallback to downloading from ESP32 if not cached yet
                model = await tf.loadLayersModel('/model.json', {
                    onProgress: (fraction) => {
                        const pct = Math.round(fraction * 100);
                        console.log(`AI Model Download: ${pct}%`);
                        captureBtnText.textContent = `Loading Model (${pct}%)...`;
                        if (aiStatusText) aiStatusText.textContent = `Loading AI (${pct}%)...`;
                        if (aiProgressBar) aiProgressBar.style.width = `${pct}%`;
                        statusVal.textContent = `Loading ${pct}%`;
                    }
                });

                // 3. Save to client IndexedDB for future instant loads
                try {
                    await model.save(MODEL_CACHE_KEY);
                    console.log("Model successfully saved to IndexedDB for offline use.");
                } catch (saveErr) {
                    console.warn("Could not cache model to IndexedDB:", saveErr);
                }
            }
            
            // Update UI status
            aiStatus.className = 'status-item ready';
            if (aiStatusText) aiStatusText.textContent = 'AI Engine Ready';
            if (aiProgressContainer) aiProgressContainer.style.display = 'none';
            
            statusVal.textContent = "Ready to Scan";
            captureBtn.disabled = false;
            captureBtnText.textContent = "Capture & Diagnose";
            console.log("TensorFlow.js model loaded successfully and ready.");

            // If physical button was pressed while model was loading, execute once stream has loaded a frame
            if (pendingTrigger) {
                pendingTrigger = false;
                setTimeout(() => triggerCapture(), 600);
            }
        } catch (error) {
            console.error("Failed to load model:", error);
            aiStatus.className = 'status-item error';
            if (aiStatusText) aiStatusText.textContent = 'AI Model Error';
            statusVal.textContent = "Model Error";
            captureBtnText.textContent = "Model Load Failed";
            alert("Error loading AI model: " + error.message + "\nPlease check browser console (F12).");
        } finally {
            // Start video stream after model check/download completes to prevent Wi-Fi bandwidth contention
            if (!streamImg.getAttribute('src')) {
                const streamHost = window.location.hostname || '192.168.4.1';
                streamImg.src = `http://${streamHost}:81/stream?t=${Date.now()}`;
            }
        }
    }

    // --- CAPTURE & DIAGNOSE FUNCTION ---
    async function triggerCapture() {
        // Protection 1: Block new scan if AI model is not ready or currently processing an image
        if (!model) {
            console.warn("AI model not yet loaded.");
            return;
        }

        if (isProcessing) {
            console.warn("AI is currently processing an image. Request ignored.");
            return;
        }

        // If currently showing a frozen capture, clicking Retake restores live stream
        if (isFrozen) {
            isFrozen = false;
            const streamHost = window.location.hostname || '192.168.4.1';
            streamImg.src = `http://${streamHost}:81/stream?t=${Date.now()}`;
            streamBadge.className = "badge live";
            streamBadge.textContent = "LIVE";
            captureBtn.classList.remove('btn-retake');
            captureBtnText.textContent = "Capture & Diagnose";
            statusVal.textContent = "Live Stream Active";
            return;
        }

        // Lock processing state immediately
        isProcessing = true;
        captureBtn.disabled = true;
        captureBtnText.textContent = "Processing... Please wait";
        statusVal.textContent = "Analyzing Image...";
        severityBadge.textContent = "Diagnosing...";
        severityBadge.style.background = '#e2e8f0';
        severityBadge.style.color = '#475569';
        recommendation.textContent = "Neural network is evaluating skin/burn pattern. Please hold steady...";

        try {
            // Freeze current frame onto canvas
            const canvas = document.getElementById('hidden-canvas');
            const ctx = canvas.getContext('2d');
            const w = streamImg.naturalWidth || streamImg.videoWidth || streamImg.width || 320;
            const h = streamImg.naturalHeight || streamImg.videoHeight || streamImg.height || 240;
            canvas.width = w;
            canvas.height = h;

            try {
                ctx.drawImage(streamImg, 0, 0, w, h);
                const snapshotUrl = canvas.toDataURL('image/jpeg');
                streamImg.src = snapshotUrl; // Freeze frame preview
            } catch (e) {
                console.warn("Direct snapshot draw warning (continuing analysis):", e);
            }

            isFrozen = true;
            streamBadge.className = "badge frozen";
            streamBadge.textContent = "FROZEN";
            captureBtn.classList.add('btn-retake');

            const imgData = ctx.getImageData(0, 0, w, h);
            // Model includes Rescaling(1/127.5, -1.0), so pass raw [0, 255] float32 pixels
            const tensor = tf.tidy(() =>
                tf.browser.fromPixels(imgData)
                    .resizeBilinear([224, 224])
                    .toFloat()
                    .expandDims()
            );

            const predictionTensor = model.predict(tensor);
            const prediction = await predictionTensor.data();
            tensor.dispose();
            predictionTensor.dispose();

            const severityMap = ["1st Degree", "2nd Degree", "3rd Degree", "Non-Burn"];
            const maxIdx = prediction.indexOf(Math.max(...prediction));
            const severity = severityMap[maxIdx] || "Non-Burn";
            const confidence = (prediction[maxIdx] * 100).toFixed(1);

            // Update UI with results
            confidenceVal.textContent = `${confidence}%`;
            severityBadge.textContent = severity;

            // Style based on severity
            if (severity === "1st Degree") {
                severityBadge.style.background = '#dcfce7';
                severityBadge.style.color = '#166534';
                recommendation.textContent = "First-degree burn detected. Usually heals without scarring. Apply cool water and aloe vera.";
            } else if (severity === "2nd Degree") {
                severityBadge.style.background = '#fef3c7';
                severityBadge.style.color = '#92400e';
                recommendation.textContent = "Second-degree burn detected. Blistering present. Keep clean, do not pop blisters, and consult a doctor.";
            } else if (severity === "3rd Degree") {
                severityBadge.style.background = '#fee2e2';
                severityBadge.style.color = '#991b1b';
                recommendation.textContent = "WARNING: Third-degree burn detected! Requires IMMEDIATE emergency medical attention.";
            } else {
                severityBadge.style.background = '#e0f2fe';
                severityBadge.style.color = '#075985';
                recommendation.textContent = "No burn injury detected. If redness, pain, or irritation persists, clean the area gently or consult a healthcare professional.";
            }

            // Send report to ESP32 -> triggers OLED & LoRa TX!
            await sendReport(severity, confidence);
        } catch (error) {
            console.error("Diagnosis error:", error);
            statusVal.textContent = "Diagnosis Failed";
            recommendation.textContent = "Unable to process frame. Please press Retake and try again.";
        } finally {
            // Unlock processing state and re-enable button as "Retake"
            isProcessing = false;
            captureBtn.disabled = false;
            captureBtnText.textContent = "Retake / Live Feed";
        }
    }

    captureBtn.addEventListener('click', triggerCapture);

    // --- FAST HTTP POLLING TRIGGER CHANNEL (250ms with in-flight guard) ---
    // Stateless, non-overlapping, cache-busted polling
    setInterval(async () => {
        if (isPollingTrigger) return;
        isPollingTrigger = true;

        const controller = new AbortController();
        const timeoutId = setTimeout(() => controller.abort(), 1500);

        try {
            const res = await fetch(`/api/trigger?t=${Date.now()}`, {
                cache: 'no-store',
                signal: controller.signal
            });
            clearTimeout(timeoutId);

            if (res.ok) {
                if (deviceStatus) deviceStatus.className = 'status-item ready';
                if (deviceStatusText) deviceStatusText.textContent = 'ESP32 Online';

                const data = await res.json();
                if (data.trigger) {
                    console.log("[Trigger] Physical button trigger received via /api/trigger!");
                    if (isFrozen) {
                        // If viewing a previous capture, button acts as instant retake
                        triggerCapture();
                    } else if (isProcessing) {
                        console.log("[Trigger] AI is currently processing an image. Ignored.");
                    } else if (!model) {
                        console.warn("[Trigger] Button pressed while AI model is loading. Queued for capture.");
                        pendingTrigger = true;
                        statusVal.textContent = "Button Pressed! Queued after model load...";
                    } else {
                        triggerCapture();
                    }
                }
            } else {
                if (deviceStatus) deviceStatus.className = 'status-item error';
                if (deviceStatusText) deviceStatusText.textContent = 'ESP32 Offline';
            }
        } catch (e) {
            clearTimeout(timeoutId);
            // Temporary fetch error during Wi-Fi reconnect or network switch
            if (deviceStatus) deviceStatus.className = 'status-item error';
            if (deviceStatusText) deviceStatusText.textContent = 'ESP32 Offline (Retrying...)';
        } finally {
            isPollingTrigger = false;
        }
    }, 250);

    async function sendReport(severity, confidence) {
        try {
            const res = await fetch('/api/report', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ severity, confidence: parseFloat(confidence) })
            });

            if (res.ok) {
                const data = await res.json();
                if (data.status === "queued") {
                    console.log(`[LORA] Cooldown active. Report queued at #${data.position} (auto-TX in ~${data.remaining}s)`);
                    statusVal.textContent = `LoRa Queued #${data.position} (~${data.remaining}s)`;
                } else if (data.status === "cooldown") {
                    console.warn(`[LORA] Queue full. Remaining: ${data.remaining}s`);
                    statusVal.textContent = `LoRa Queue Full (${data.remaining}s)`;
                } else {
                    console.log("Report sent immediately to ESP32 & LoRa TX");
                    statusVal.textContent = "Report Sent (LoRa OK)";
                }
            }
        } catch (err) {
            console.error("Report transmission failed:", err);
            statusVal.textContent = "Report Transmission Error";
        }
    }

    // Initialize
    initModel();
});
