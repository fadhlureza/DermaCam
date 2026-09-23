document.addEventListener('DOMContentLoaded', async () => {
    const streamImg = document.getElementById('camera-stream');
    const severityBadge = document.getElementById('severity-badge');
    const statusVal = document.getElementById('status-val');
    const confidenceVal = document.getElementById('confidence-val');
    const recommendation = document.getElementById('recommendation');
    const modelStatus = document.getElementById('model-status');

    let model = null;
    let isPredicting = false;
    let lastReportedSeverity = "";

    const RECOM_RINGAN = "Luka ringan. Bersihkan area luka dengan air mengalir dan observasi.";
    const RECOM_SEDANG = "Luka derajat sedang (Partial-thickness). Risiko blister. Butuh penanganan steril, hindari pecah lepuh.";
    const RECOM_BERAT = "Luka derajat berat (Full-thickness). Jaringan rusak dalam. Segera prioritaskan evakuasi ke fasilitas medis!";
    const CLASSES = ['Ringan', 'Sedang', 'Berat']; // Sesuaikan dengan kelas model Anda

    function updateUI(severity, confidence) {
        severityBadge.className = 'severity-badge';
        statusVal.textContent = "Berhasil dianalisis";
        confidenceVal.textContent = confidence ? `${(confidence * 100).toFixed(1)}%` : '-';
        
        severityBadge.textContent = severity.toUpperCase();
        severityBadge.classList.add(severity.toLowerCase());
        
        switch(severity.toLowerCase()) {
            case 'ringan':
                recommendation.textContent = RECOM_RINGAN;
                recommendation.style.borderLeftColor = 'var(--success-color)';
                break;
            case 'sedang':
                recommendation.textContent = RECOM_SEDANG;
                recommendation.style.borderLeftColor = 'var(--warning-color)';
                break;
            case 'berat':
                recommendation.textContent = RECOM_BERAT;
                recommendation.style.borderLeftColor = 'var(--danger-color)';
                break;
        }
    }

    async function sendReportToESP(severity, confidence) {
        // Hindari spamming, hanya kirim jika severity berubah (atau bisa diset tiap x detik)
        if (severity === lastReportedSeverity) return;
        lastReportedSeverity = severity;

        try {
            const payload = {
                severity: severity,
                confidence: confidence,
                timestamp: new Date().toISOString()
            };

            await fetch('/api/report', {
                method: 'POST',
                headers: {
                    'Content-Type': 'application/json',
                },
                body: JSON.stringify(payload)
            });
            console.log("Report sent to ESP32 for LoRa transmission.");
        } catch (err) {
            console.error("Failed to send report to ESP32", err);
        }
    }

    async function predictLoop() {
        if (isPredicting && model && streamImg.complete && streamImg.naturalHeight !== 0) {
            try {
                // Konversi gambar img element ke Tensor
                // Note: tf.browser.fromPixels akan error jika streamImg belum load.
                const imgTensor = tf.browser.fromPixels(streamImg)
                    .resizeBilinear([224, 224]) // Sesuaikan resolusi input model Anda (misal 224x224)
                    .expandDims(0)
                    .toFloat()
                    .div(tf.scalar(255)); // Normalisasi 0-1 (sesuaikan dengan training Anda)
                
                const predictions = await model.predict(imgTensor).data();
                
                // Cari index dengan probabilitas tertinggi
                const maxIndex = predictions.indexOf(Math.max(...predictions));
                const confidence = predictions[maxIndex];
                const predictedClass = CLASSES[maxIndex];
                
                updateUI(predictedClass, confidence);
                
                // Kirim ke ESP32 (hanya jika ada perubahan kelas)
                sendReportToESP(predictedClass, confidence);
                
                imgTensor.dispose(); // Wajib untuk mencegah memory leak
            } catch (err) {
                // Kemungkinan error membaca tensor saat gambar refresh
            }
        }
        
        // Loop setiap detik untuk menghemat baterai HP (tidak perlu 60fps)
        setTimeout(() => requestAnimationFrame(predictLoop), 1000); 
    }

    // Inisialisasi Model TF.js
    async function initModel() {
        try {
            // Note: Jika tf.loadLayersModel gagal, pastikan Anda menggunakan loadGraphModel jika exportnya berupa graph.
            model = await tf.loadLayersModel('/model.json');
            modelStatus.textContent = "Model AI Siap - Pemindaian Aktif";
            modelStatus.style.color = 'var(--success-color)';
            isPredicting = true;
            predictLoop();
        } catch (error) {
            console.error("Gagal memuat model:", error);
            modelStatus.textContent = "Error: File Model tidak ditemukan atau rusak. Cek file placeholder.";
            modelStatus.style.color = 'var(--danger-color)';
        }
    }

    initModel();
});
