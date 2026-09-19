document.addEventListener('DOMContentLoaded', () => {
    const scanBtn = document.getElementById('capture-btn');
    const severityBadge = document.getElementById('severity-badge');
    const statusVal = document.getElementById('status-val');
    const confidenceVal = document.getElementById('confidence-val');
    const recommendation = document.getElementById('recommendation');

    const RECOM_RINGAN = "Luka ringan. Bersihkan area luka dengan air mengalir dan observasi.";
    const RECOM_SEDANG = "Luka derajat sedang (Partial-thickness). Risiko blister. Butuh penanganan steril, hindari pecah lepuh.";
    const RECOM_BERAT = "Luka derajat berat (Full-thickness). Jaringan rusak dalam. Segera prioritaskan evakuasi ke fasilitas medis!";

    function updateUI(severity, status, confidence) {
        // Reset classes
        severityBadge.className = 'severity-badge';
        
        // Update values
        statusVal.textContent = status;
        confidenceVal.textContent = confidence ? `${confidence}%` : '-';
        
        if (severity) {
            severityBadge.textContent = severity.toUpperCase();
            severityBadge.classList.add(severity.toLowerCase());
            
            // Update recommendation based on severity
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
                default:
                    recommendation.textContent = "Data tidak valid.";
            }
        } else {
            severityBadge.textContent = 'Menunggu...';
            recommendation.textContent = "Silakan lakukan pemindaian luka untuk melihat hasil evaluasi.";
            recommendation.style.borderLeftColor = 'var(--primary-color)';
        }
    }

    async function fetchStatus() {
        try {
            // Using Fetch API to get status from ESP32 API
            const response = await fetch('/api/status');
            
            if (!response.ok) {
                throw new Error(`HTTP error! status: ${response.status}`);
            }
            
            const data = await response.json();
            console.log("Data received:", data);
            
            if (data && data.status === 'success') {
                updateUI(data.burn_severity, "Berhasil dianalisis", data.confidence);
            }
            
        } catch (error) {
            console.error("Fetch error:", error);
            statusVal.textContent = "Gagal memuat data";
        } finally {
            scanBtn.textContent = "Scan Area";
            scanBtn.disabled = false;
        }
    }

    scanBtn.addEventListener('click', () => {
        scanBtn.textContent = "Menganalisis...";
        scanBtn.disabled = true;
        
        severityBadge.className = 'severity-badge';
        severityBadge.textContent = 'Proses...';
        statusVal.textContent = 'Mengambil gambar...';
        
        // Simulate a slight delay for realism before fetching
        setTimeout(() => {
            fetchStatus();
        }, 1500);
    });
});
