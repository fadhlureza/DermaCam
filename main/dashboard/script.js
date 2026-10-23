document.addEventListener('DOMContentLoaded', () => {
    // UI Elements
    const toggleLightBtn = document.getElementById('toggle-light-btn');
    const btnText = document.getElementById('btn-text');
    const aiStatus = document.getElementById('ai-status');
    const severityBadge = document.getElementById('severity-badge');
    const confidenceVal = document.getElementById('confidence-val');
    const statusVal = document.getElementById('status-val');
    const streamImg = document.getElementById('camera-stream');
    const recommendation = document.getElementById('recommendation');
    
    // Set date
    document.getElementById('date-display').textContent = new Date().toLocaleDateString('en-US', { weekday: 'long', year: 'numeric', month: 'long', day: 'numeric' });

    const captureBtn = document.getElementById('capture-btn');
    const captureBtnText = document.getElementById('capture-btn-text');
    const streamBadge = document.getElementById('stream-badge');

    // State Variables
    let model = null;
    let lightOn = true; // Firmware defaults LED to ON
    let isFrozen = false;

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

        try {
            model = await tf.loadLayersModel('/model.json');
            
            // Update UI status
            aiStatus.className = 'status-item ready';
            aiStatus.querySelector('span:nth-child(2)').textContent = 'AI Engine Ready';
            
            statusVal.textContent = "Ready to Scan";
            captureBtn.disabled = false;
            captureBtnText.textContent = "Capture & Diagnose";
            console.log("TensorFlow.js model loaded successfully.");
        } catch (error) {
            console.error("Failed to load model:", error);
            aiStatus.className = 'status-item error';
            aiStatus.querySelector('span:nth-child(2)').textContent = 'AI Model Error';
            statusVal.textContent = "Model Error";
            captureBtnText.textContent = "Model Load Failed";
        }
    }

    // --- CAPTURE & DIAGNOSE FUNCTION ---
    async function triggerCapture() {
        if (!model) {
            console.warn("AI model not yet loaded.");
            return;
        }

        // If currently showing a frozen capture, clicking Retake restores live stream
        if (isFrozen) {
            isFrozen = false;
            streamImg.src = "http://192.168.4.1:81/stream";
            streamBadge.className = "badge live";
            streamBadge.textContent = "LIVE";
            captureBtn.classList.remove('btn-retake');
            captureBtnText.textContent = "Capture & Diagnose";
            statusVal.textContent = "Live Stream Active";
            return;
        }

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
        captureBtnText.textContent = "Retake / Live Feed";

        statusVal.textContent = "Analyzing...";
        severityBadge.textContent = "Diagnosing...";
        severityBadge.style.background = '#e2e8f0';
        severityBadge.style.color = '#475569';
        recommendation.textContent = "Processing captured frame with neural network...";

        try {
            const imgData = ctx.getImageData(0, 0, w, h);
            const tensor = tf.browser.fromPixels(imgData)
                .resizeBilinear([224, 224])
                .toFloat()
                .div(tf.scalar(255.0))
                .expandDims();

            const prediction = await model.predict(tensor).data();
            tensor.dispose();

            const severityMap = ["1st Degree", "2nd Degree", "3rd Degree"];
            const maxIdx = prediction.indexOf(Math.max(...prediction));
            const severity = severityMap[maxIdx];
            const confidence = (prediction[maxIdx] * 100).toFixed(1);

            // Update UI
            confidenceVal.textContent = `${confidence}%`;
            severityBadge.textContent = severity;
            statusVal.textContent = "Report Sent";

            // Style based on severity
            if (severity === "1st Degree") {
                severityBadge.style.background = '#dcfce7';
                severityBadge.style.color = '#166534';
                recommendation.textContent = "First-degree burn detected. Usually heals without scarring. Apply cool water and aloe vera.";
            } else if (severity === "2nd Degree") {
                severityBadge.style.background = '#fef3c7';
                severityBadge.style.color = '#92400e';
                recommendation.textContent = "Second-degree burn detected. Blistering present. Keep clean, do not pop blisters, and consult a doctor.";
            } else {
                severityBadge.style.background = '#fee2e2';
                severityBadge.style.color = '#991b1b';
                recommendation.textContent = "WARNING: Third-degree burn detected! Requires IMMEDIATE emergency medical attention.";
            }

            // Send report to ESP32 -> triggers OLED & LoRa TX!
            sendReport(severity, confidence);
        } catch (error) {
            console.error("Diagnosis error:", error);
            statusVal.textContent = "Diagnosis Failed";
            recommendation.textContent = "Unable to process frame. Please press Retake and try again.";
        }
    }

    captureBtn.addEventListener('click', triggerCapture);

    // --- HARDWARE BUTTON POLLING ---
    // Polls /api/trigger every 300ms so physical button presses on the ESP32 trigger the AI scan
    setInterval(async () => {
        if (!model) return;
        try {
            const res = await fetch('/api/trigger');
            if (res.ok) {
                const data = await res.json();
                if (data.trigger) {
                    console.log("Hardware trigger received from ESP32 button!");
                    triggerCapture();
                }
            }
        } catch (e) {
            // Ignore temporary fetch errors
        }
    }, 300);

    async function sendReport(severity, confidence) {
        try {
            await fetch('/api/report', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ severity, confidence: parseFloat(confidence) })
            });
            console.log("Report sent successfully to ESP32 & LoRa TX");
        } catch (err) {
            console.error("Report transmission failed:", err);
        }
    }

    // Initialize
    initModel();
});
