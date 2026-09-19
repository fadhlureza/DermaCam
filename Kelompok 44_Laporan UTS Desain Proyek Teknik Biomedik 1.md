&nbsp;

![][image1]

**DESAIN PROYEK 1 TEKNIK BIOMEDIK**

***“Derma-Cam” AI-Based Scanner and Health Database Telecommunication device***

&nbsp;

| Client: | BASARNAS (Badan Nasional Pencarian dan Pertolongan) |
| :---- | :---- |
| **Advisor:** | Mohammad Ikhsan, Ph.D&nbsp;&nbsp; |

**Kelompok 44**

| Aisya Rivelia Azzahra | Teknik Komputer | 2306161864 |
| :---- | :---- | ----: |
| Alana Maritza Tridi | Teknik Biomedik | 2306251525 |
| Annisaa Sheryl Tabina | Teknik Elektro | 2306266716 |
| Fadhlureza Sebastian | Teknik Komputer | 2306161971 |
| Nathania Isabella Ulibasa Sihite | Teknik Elektro | 2306247484 |

**FAKULTAS TEKNIK**

**PROGRAM STUDI TEKNIK BIOMEDIK**

**UNIVERSITAS INDONESIA**

**2026**&nbsp;

# **ABSTRACT** {#abstract}

Bencana alam seringkali memicu korban massal dengan berbagai tingkat cedera, termasuk luka bakar. Di lapangan, proses identifikasi tingkat keparahan luka oleh tim SAR atau evakuator masih mengandalkan observasi visual manual, sehingga rentan subjektivitas dan memakan waktu. Untuk mengatasi hal ini, kami mengembangkan Derma-Cam yang merupakan perangkat pemindai luka portabel berbasis *Artificial Intelligence* atau AI. Alat ini membantu evakuator mengklasifikasikan keparahan luka secara cepat sebagai skrining awal. Selain itu, Derma-Cam dilengkapi modul telekomunikasi LoRa untuk mengirimkan data korban ke fasilitas kesehatan terdekat, bahkan di area yang tidak memiliki jaringan seluler.

Pengembangan sistem ini mengintegrasikan mikrokontroler ESP32-CAM dengan model *Convolutional Neural Network* (CNN) yang telah dioptimalkan melalui pendekatan *TinyML* untuk klasifikasi luka secara *on-device*. Prototipe Derma-Cam dirancang dengan bentuk ergonomis agar mudah dioperasikan di medan bencana yang ekstrem. Melalui integrasi teknologi visi komputer dan transmisi data nirkabel jarak jauh, perangkat ini diharapkan dapat meminimalisir kesalahan prosedur triase dan mempercepat waktu tanggap darurat medis. Dengan demikian, Derma-Cam hadir sebagai solusi inovatif dalam menjembatani kebutuhan diagnosis awal yang akurat dengan keterbatasan infrastruktur komunikasi di area pascabencana.

&nbsp;

&nbsp;

&nbsp;

&nbsp;

**DAFTAR ISI**

&nbsp;

[**ABSTRACT	1**](#abstract)

[**I. PENDAHULUAN	4**](#pendahuluan)

[1.1 Motivasi dan Dampak Global	4](#1.1-motivasi-dan-dampak-global)

[1.2 Perangkat yang Ada dan Metode Saat ini	5](#1.2-perangkat-yang-ada-dan-metode-saat-ini)

[1.3 Pernyataan Masalah	6](#1.3-pernyataan-masalah)

[**II. LATAR BELAKANG DAN TINJAUAN PUSTAKA	8**](#latar-belakang-dan-tinjauan-pustaka)

[2.1 Latar Belakang	8](#2.1-latar-belakang)

[2.2 Tinjauan Pustaka	10](#2.2-tinjauan-pustaka)

[2.2.1 Fisiologi dan Biologi Luka dan Luka Bakar	11](#2.2.1-fisiologi-dan-biologi-luka-dan-luka-bakar)

[2.2.2 Kebutuhan Literatur untuk Merancang dan Membangun Prototipe	14](#2.2.2-kebutuhan-literatur-untuk-merancang-dan-membangun-prototipe)

[2.2.3 Pembahasan Solusi Masalah pada Literatur 1 : AI untuk Karakterisasi Luka Bakar	16](#2.2.3-pembahasan-solusi-masalah-pada-literatur-1-:-ai-untuk-karakterisasi-luka-bakar)

[2.2.4 Pembahasan Solusi Masalah pada Literatur 2 : Mobile Tool untuk Penilaian Luka Bakar	17](#2.2.4-pembahasan-solusi-masalah-pada-literatur-2-:-mobile-tool-untuk-penilaian-luka-bakar)

[2.2.5 Pembahasan Solusi Masalah pada Literatur 3 : TinyML dan ESP32-CAM	17](#2.2.5-pembahasan-solusi-masalah-pada-literatur-3-:-tinyml-dan-esp32-cam)

[2.2.6 Sintesis Tinjauan Pustaka	18](#2.2.6-sintesis-tinjauan-pustaka)

[2.3 Teori Dasar dan Mata Kuliah BME Terkait	19](#2.3-teori-dasar-dan-mata-kuliah-bme-terkait)

[2.4 Standar dan Batasan Realistis	20](#2.4-standar-dan-batasan-realistis)

[2.4.1 Standar Target Solusi	20](#2.4.1-standar-target-solusi)

[2.4.2 Standar Rumusan Solusi	20](#2.4.2-standar-rumusan-solusi)

[2.4.3 Technical Constraints	21](#2.4.3-technical-constraints)

[2.4.4 Non-Technical Constraints	22](#2.4.4-non-technical-constraints)

[**III. DESAIN AWAL	24**](#desain-awal)

[3.1 Desain Awal	24](#3.1-desain-awal)

[3.1.1 Konsep yang Dipertimbangkan	24](#3.1.1-konsep-yang-dipertimbangkan)

[3.1.2 Spesifikasi Produk Desain Akhir \- Komponen dan Material	25](#3.1.2-spesifikasi-produk-desain-akhir---komponen-dan-material)

[3.1.3 Sketsa Fisik dan Dimensi Produk	25](#3.1.3-sketsa-fisik-dan-dimensi-produk)

[3.2 Kompleksitas Desain Sistem	27](#3.2-kompleksitas-desain-sistem)

[3.2.1 Perangkat Keras dan Lingkungan Fisik	27](#3.2.1-perangkat-keras-dan-lingkungan-fisik)

[3.2.2 Kecerdasan Buatan dan Batasan Komputasi	27](#3.2.2-kecerdasan-buatan-dan-batasan-komputasi)

[3.2.3 Fungsi Utama dan Validasi Klinis	28](#3.2.3-fungsi-utama-dan-validasi-klinis)

[**IV. EVALUASI DESAIN AWAL	29**](#evaluasi-desain-awal)

[4.1 Evaluasi Desain Awal	29](#4.1-evaluasi-desain-awal)

[4.1.1 Ringkasan Matriks Desain	29](#4.1.1-ringkasan-matriks-desain)

[4.1.2 Desain Akhir	29](#4.1.2-desain-akhir)

[4.2 Studi Kelayakan Desain Akhir yang Diusulkan	30](#4.2-studi-kelayakan-desain-akhir-yang-diusulkan)

[4.2.2 Potensi Manfaat bagi Pengguna	30](#4.2.2-potensi-manfaat-bagi-pengguna)

[4.2.3 Potensi Skalabilitas dari Prototyping ke Produksi Massal	30](#4.2.3-potensi-skalabilitas-dari-prototyping-ke-produksi-massal)

[4.2.4 Analisis Ekonomi Teknik	31](#4.2.4-analisis-ekonomi-teknik)

[**V. PERENCANAAN PROYEK DAN TANGGUNG JAWAB	32**](#perencanaan-proyek-dan-tanggung-jawab)

[6.1 Timeline dan Rincian Pekerjaan	32](#5.1-timeline-dan-rincian-pekerjaan)

[6.2 Pemetaan Pengetahuan dan Keterampilan Tim	32](#5.2-pemetaan-pengetahuan-dan-keterampilan-tim)

[Kemampuan Awal yang Sudah Dimiliki Kelompok	32](#kemampuan-awal-yang-sudah-dimiliki-kelompok)

[Tim Interdisiplin sudah memiliki campuran dari ketiga jurusan dalam Departemen Teknik Elektro dengan ilmu masing- masing. Hal tersebut membantu kombinasi ilmu untuk memulai pengembangan proyek Derma-Cam, baik dari sisi teknis maupun pemahaman konsep.	32](#tim-interdisiplin-sudah-memiliki-campuran-dari-ketiga-jurusan-dalam-departemen-teknik-elektro-dengan-ilmu-masing--masing.-hal-tersebut-membantu-kombinasi-ilmu-untuk-memulai-pengembangan-proyek-derma-cam,-baik-dari-sisi-teknis-maupun-pemahaman-konsep.)

[Keterampilan yang Perlu Dikembangkan Selama Proyek	33](#keterampilan-yang-perlu-dikembangkan-selama-proyek)

[Selain kemampuan awal yang sudah dimiliki, terdapat beberapa hal yang masih perlu dipelajari agar pengembangan sistem dapat berjalan secara optimal.	33](#selain-kemampuan-awal-yang-sudah-dimiliki,-terdapat-beberapa-hal-yang-masih-perlu-dipelajari-agar-pengembangan-sistem-dapat-berjalan-secara-optimal.)

[6.3 Rencana Anggaran Belanja	34](#5.3-rencana-anggaran-belanja)

[6.3.1 Aset yang Menggunakan Pinjaman Lab / Tahun Sebelumnya	34](#5.3.1-aset-yang-menggunakan-pinjaman-lab-/-tahun-sebelumnya)

[6.3.2 Bahan Habis Pakai (Perlu Dibeli Baru)	34](#5.3.2-bahan-habis-pakai-\(perlu-dibeli-baru\))

[6.3.3 Perangkat dan Komponen Elektronik (Perlu Dibeli Baru)	35](#5.3.3-perangkat-dan-komponen-elektronik-\(perlu-dibeli-baru\))

[**REFERENSI	36**](#referensi)

&nbsp;

1. # **PENDAHULUAN** {#pendahuluan}

   ## **1.1	Motivasi dan Dampak Global** {#1.1-motivasi-dan-dampak-global}

   Cedera luka dan luka bakar merupakan salah satu bentuk trauma yang sering muncul pada situasi darurat, termasuk bencana alam, kebakaran, ledakan, kecelakaan industri, dan bencana alam. Pada kondisi tersebut, penanganan awal dilakukan dengan cepat karena keterlambatan asesmen dapat memperburuk keadaan korban. *World Health Organization* menyatakan bahwa luka bakar masih menjadi masalah kesehatan global menyebabkan sekitar 180.000 kematian setiap tahun, dengan mayoritas terjadi di negara berpendapatan rendah dan menengah. Selain itu, luka bakar juga dapat menimbulkan kecacatan jangka panjang, gangguan fungsi, serta kebutuhan rehabilitasi yang kompleks.  
   Penanganan korban luka bakar sangat membebani industri medis dan sistem kesehatan dalam skala besar. World Health Organization (WHO) menegaskan bahwa tatalaksana luka bakar dalam *Mass Casualty* incident memerlukan koordinasi, triase, dan alokasi sumber daya yang baik karena kebutuhan perawatan dapat dengan cepat melampaui kapasitas layanan setempat. Dalam konteks seperti ini, asesmen awal yang cepat dan cukup akurat menjadi sangat penting, terutama pada fase pra-rumah sakit ketika keputusan prioritas evakuasi dan pertolongan pertama harus dibuat dalam waktu singkat.  
   Kebutuhan tersebut relevan bagi Indonesia sebagai negara dengan tingkat kerawanan bencana yang tinggi. Data BNPB menunjukkan bahwa dampak bencana di Indonesia pada tahun 2024 sangat besar terhadap masyarakat dan infrastruktur, termasuk korban terdampak, rumah rusak, dan fasilitas umum yang terganggu. Pada situasi lapangan yang padat, kacau, dan terbatas sumber daya, pihak pertama yang berinteraksi dengan korban tidak selalu tenaga medis, melainkan relawan, petugas evakuasi, atau personel lapangan non-medis. Akibatnya, kemampuan melakukan identifikasi awal terhadap tingkat keparahan luka menjadi kebutuhan praktis yang belum sepenuhnya terpenuhi.  
   Permasalahan ini semakin penting karena pengetahuan pertolongan pertama luka bakar pada masyarakat tidak selalu memadai. Sebuah studi observasional di Indonesia bagian timur melaporkan bahwa 76,2% responden tidak familiar dengan praktik pertolongan pertama luka bakar yang direkomendasikan, sehingga terdapat kesenjangan antara kebutuhan lapangan dan kesiapan pengguna awal dalam merespons cedera secara tepat. Dengan demikian, dibutuhkan suatu alat bantu yang dapat mendukung proses identifikasi awal luka luar dan luka bakar secara cepat, mudah, dan cukup objektif, khususnya bagi pengguna non-spesialis di lokasi kejadian.

   ## **1.2	Perangkat yang Ada dan Metode Saat ini** {#1.2-perangkat-yang-ada-dan-metode-saat-ini}

   Metode penilaian luka dan luka bakar yang paling umum saat ini masih mengandalkan observasi visual langsung. Dalam pendekatan konvensional, petugas menilai warna luka, kelembaban jaringan, adanya lepuh, luas area terdampak, dan dugaan kedalaman kerusakan untuk memperkirakan tingkat keparahan serta kebutuhan tatalaksana. Akan tetapi, akurasi pendekatan ini sangat dipengaruhi oleh pengalaman pemeriksa, kondisi pencahayaan, dan situasi lapangan saat pemeriksaan dilakukan.  
   Dalam praktik klinis, penentuan keparahan luka bakar umumnya dilakukan berdasarkan kedalaman kerusakan jaringan. Secara umum, luka bakar terbagi menjadi beberapa derajat, mulai dari yang ringan sampai kerusakan penuh lapisan kulit dan jaringan dalam. Perbedaan klasifikasi ini penting karena berkaitan dengan pertolongan pertama, kebutuhan perawatan lanjutan, serta kemungkinan rujukan ke fasilitas yang lebih memadai.  
   Selain kedalaman luka, penilaian awal pada kasus luka bakar juga mempertimbangkan luas permukaan tubuh yang terdampak. Salah satu metode yang umum digunakan adalah *Rule of Nines,* yaitu pendekatan estimasi cepat untuk menghitung persentase *Total Body Surface Area (TBSA)* yang mengalami luka bakar. Metode ini membagi tubuh ke dalam beberapa area dengan persentase tertentu sehingga memudahkan penilaian awal pada situasi darurat. Informasi mengenai luas luka bakar penting karena berhubungan dengan penentuan tingkat keparahan kasus, kebutuhan resusitasi cairan, dan keputusan rujukan lebih lanjut. Namun, *Rule of Nines* digunakan untuk memperkirakan luas luka bakar, bukan untuk menentukan kedalaman atau derajat luka itu sendiri.  
   Di samping karakteristik luka, asesmen awal korban dalam situasi gawat darurat juga sering mempertimbangkan kondisi umum pasien, termasuk tingkat kesadaran atau respons terhadap rangsangan. Salah satu metode cepat yang digunakan adalah AVPU, yang mengelompokkan pasien ke dalam kondisi *Alert*, *Verbal*, *Pain*, dan *Unresponsive*. Skala ini membantu petugas menilai respons pasien secara cepat pada kondisi darurat, terutama ketika waktu penanganan terbatas dan jumlah korban banyak. Namun, AVPU tidak digunakan untuk menentukan derajat luka, melainkan sebagai alat bantu untuk menilai kondisi neurologis umum pasien dalam proses triase awal.  
   Seiring perkembangan teknologi, berbagai sistem berbasis kecerdasan buatan mulai dikembangkan untuk mendukung asesmen luka. Contohnya *Skinopathy Burns* dikembangkan untuk membantu *triage* dan pelacakan luka bakar, termasuk estimasi total body surface area serta pemantauan proses penyembuhan. Selain itu, *DeepView System* dari *Spectral AI* memanfaatkan multispectral imaging dan algoritma AI untuk memprediksi potensi penyembuhan luka bakar secara lebih objektif. Pada tingkat riset, pengembangan juga dilakukan melalui sistem multimodal AI yang menggabungkan citra digital dan ultrasound untuk klasifikasi luka bakar, serta berbagai perangkat lunak asesmen luka berbasis AI untuk segmentasi jaringan, pemantauan luka, dan prediksi penyembuhan. Meskipun demikian, sebagian besar teknologi tersebut masih berorientasi pada penggunaan klinis, perangkat khusus, atau pemantauan luka secara komprehensif, sehingga belum sepenuhnya sesuai dengan kebutuhan lapangan yang menuntut solusi *portable*, berbiaya rendah, mudah digunakan, dan dapat dioperasikan oleh pengguna non-medis.&nbsp;  
   &nbsp;  
   &nbsp;  
   **Alternatif Solusi dari Literatur**  
   Berdasarkan tinjauan literatur, terdapat tiga pendekatan solusi yang dipertimbangkan untuk membantu identifikasi awal luka luar dan luka bakar pada situasi darurat :  
* **Pendekatan Asesmen Klinis Manual Berbantuan Panduan Triase**  
  Mengandalkan observasi visual petugas terhadap karakteristik luka, dikombinasikan dengan panduan seperti klasifikasi derajat luka bakar, *Rule of Nines*, dan AVPU. Kelebihan \> sederhana, murah, dan tidak membutuhkan perangkat khusus. Kekurangan \> sangat bergantung pada pengalaman pengguna dan cenderung subjektif.  
* **Pendekatan Analisis Citra RGB Berbasis AI**  
  Menggunakan kamera standar untuk mengambil citra luka, kemudian memprosesnya dengan model kecerdasan buatan untuk membantu klasifikasi tingkat keparahan. Kelebihan \> lebih objektif, dapat dibuat *portable*, dan relatif ekonomis. Kekurangan \> akurasi sangat bergantung pada kualitas citra dan tidak dapat menangkap informasi kedalaman jaringan secara langsung.  
* **Pendekatan Pencitraan Lanjutan/Multimodal**  
  Menggunakan teknologi seperti multispectral imaging, ultrasound, atau terahertz imaging untuk menilai karakteristik luka secara lebih mendalam. Kelebihan \> berpotensi memberikan akurasi yang lebih tinggi dan informasi jaringan yang lebih kaya. Kekurangan \> biaya lebih tinggi, perangkat lebih kompleks, dan kurang sesuai untuk kondisi lapangan.

  ## **1.3	Pernyataan Masalah** {#1.3-pernyataan-masalah}

Masalah utama yang diselesaikan oleh proyek ini adalah :

* **Identifikasi awal luka** luar dan luka bakar pada situasi bencana masih sulit dilakukan secara cepat, konsisten, dan cukup objektif, terutama ketika jumlah korban banyak dan waktu penanganan sangat terbatas.  
* **Penilaian tingkat keparahan luka** di lapangan masih sangat bergantung pada observasi visual manual, sehingga hasil asesmen dipengaruhi oleh pengalaman, pelatihan, dan subjektivitas petugas yang melakukan pemeriksaan.  
* **Kondisi lapangan** bencana yang tidak ideal, seperti pencahayaan yang buruk, tidak ada sinyal, mobilitas korban yang tinggi, dan keterbatasan sarana medis, menyebabkan proses penilaian luka menjadi semakin sulit dilakukan secara akurat.  
* **Tampilan visual luka** tidak selalu cukup untuk merepresentasikan tingkat urgensi penanganan korban secara menyeluruh, sehingga terdapat risiko bahwa korban dengan kondisi yang lebih serius tidak segera diprioritaskan.  
* **Keterbatasan kemampuan pengguna** non-medis dalam membedakan luka ringan, luka dalam, dan luka bakar dengan tingkat keparahan yang berbeda dapat menyebabkan ketidaktepatan pertolongan pertama dan keputusan triase awal.

  &nbsp;

&nbsp;**Rumusan Permasalahan Solusi**

Dalam proses pengembangan Derma-Cam, tim mengidentifikasi beberapa permasalahan teknis yang akan dihadapi selama tahap perancangan, integrasi, dan pengujian sistem :

* Kesulitan memperoleh citra luka yang konsisten pada kondisi lapangan yang tidak terkontrol, terutama akibat variasi pencahayaan, bayangan, sudut pengambilan gambar, jarak kamera, dan pergerakan operator saat proses akuisisi citra berlangsung.  
* Tantangan melakukan klasifikasi tingkat keparahan luka hanya berdasarkan informasi visual dua dimensi, mengingat penilaian klinis pada praktik nyata juga dipengaruhi oleh kedalaman jaringan, luas area luka, karakteristik permukaan, dan kondisi umum pasien.  
* Kompleksitas dalam menyusun dataset citra luka dan luka bakar yang representatif serta tervalidasi secara medis, karena data sekunder sering memiliki kualitas gambar, sudut pengambilan, dan anotasi yang tidak seragam.  
* Kesulitan mengembangkan model kecerdasan buatan yang cukup ringan untuk dijalankan secara offline pada perangkat dengan sumber daya terbatas, namun tetap memiliki akurasi yang memadai untuk mendukung asesmen awal.  
* Kesulitan menerjemahkan hasil klasifikasi sistem ke dalam keluaran yang sederhana, relevan, dan aman bagi pengguna non-medis, sehingga dapat membantu pertolongan pertama tanpa menimbulkan salah interpretasi.

2. # **LATAR BELAKANG DAN TINJAUAN PUSTAKA** {#latar-belakang-dan-tinjauan-pustaka}

## **2.1 Latar Belakang** {#2.1-latar-belakang}

Indonesia merupakan salah satu negara yang memiliki tingkat kerentanan bencana yang tinggi. Posisi geografis Indonesia yang berada pada pertemuan beberapa lempeng tektonik, ditambah kondisi iklim dan topografi yang beragam, menyebabkan kejadian seperti gempa bumi, banjir, tanah longsor, dan kebakaran masih sering terjadi di berbagai wilayah. Dalam kondisi seperti ini, penanganan korban pada fase awal menjadi sangat penting karena keterlambatan asesmen dan tindakan awal dapat memperburuk kondisi korban, terutama pada kejadian dengan jumlah korban yang banyak.&nbsp;

Pada situasi bencana, tim penyelamat, relawan, maupun penolong pertama sering menjadi pihak yang paling awal berinteraksi dengan korban sebelum korban mendapat penanganan medis yang lebih lengkap. Masalahnya, proses penilaian awal di lapangan sering dilakukan dalam kondisi yang tidak ideal, misalnya pencahayaan kurang baik, lingkungan tidak kondusif, jumlah korban banyak, serta keterbatasan komunikasi dengan fasilitas kesehatan. Salah satu kondisi yang cukup menantang untuk diidentifikasi secara cepat di lapangan adalah luka luar dan luka bakar. Secara umum, luka bakar tidak hanya dinilai dari tampilan permukaannya, tetapi juga dari kedalaman kerusakan jaringan dan luas area tubuh yang terkena. Artinya, dua luka yang sekilas tampak mirip belum tentu memiliki tingkat keparahan yang sama. Kesalahan dalam mengenali tingkat keparahan luka dapat menyebabkan prioritas penanganan menjadi kurang tepat, sedangkan keterlambatan penanganan luka bakar dapat meningkatkan risiko komplikasi lanjutan.

Selain kondisi luka, tingkat respons korban juga menjadi bagian penting dalam asesmen awal. Salah satu metode yang umum digunakan adalah AVPU, yaitu *Alert, Verbal, Painful, dan Unresponsive.* Skala ini digunakan untuk menilai tingkat kesadaran atau respons korban secara cepat. Korban yang masih sadar penuh masuk kategori *alert*, korban yang masih merespons saat diajak bicara masuk kategori *verbal*, korban yang hanya merespons terhadap nyeri masuk kategori *painful*, sedangkan korban yang tidak memberi respons masuk kategori *unresponsive*. Penggunaan skala ini relevan pada situasi darurat karena sederhana, cepat, dan membantu petugas menentukan prioritas penanganan awal.&nbsp;

Berdasarkan wawancara awal dengan pihak **BASARNAS** atau tim pencarian dan pertolongan, diperoleh gambaran bahwa penilaian keparahan korban di lapangan pada dasarnya sangat bergantung pada pengalaman dan pelatihan petugas. Petugas yang sudah terbiasa menangani kondisi darurat umumnya mampu melakukan asesmen secara cepat. Namun, dalam situasi bencana yang melibatkan banyak korban, kebutuhan akan alat bantu tetap relevan, terutama untuk membantu relawan, petugas non-medis, atau pengguna lain yang belum memiliki pengalaman klinis yang memadai. Dari wawancara tersebut juga terlihat bahwa penilaian luka membutuhkan lebih dari sekadar melihat adanya cedera, tetapi juga mempertimbangkan jenis luka, perkiraan kedalaman, serta luas area cedera, khususnya pada luka bakar.&nbsp;

Masukan dari pihak industri, yaitu kak **Fadinda Shafira, [S.Si](http://S.Si)** dari PT Kalbe menunjukkan bahwa pengembangan solusi seperti ini tidak bisa hanya dilihat dari sisi medis, tetapi juga harus realistis dari sisi rekayasa. Pemilihan kamera, kualitas pencahayaan, ukuran perangkat, kebutuhan daya, serta biaya pembuatan menjadi faktor penting dalam perancangan prototipe. Dengan kata lain, alat yang dirancang harus cukup sederhana untuk diwujudkan, tetapi tetap mampu menghasilkan citra yang layak untuk diproses. Pertimbangan seperti ini penting karena kualitas gambar sangat mempengaruhi hasil klasifikasi pada sistem berbasis citra.&nbsp;

Berdasarkan kondisi tersebut, diperlukan sebuah alat bantu skrining awal yang dapat membantu pengguna mengenali kondisi luka secara lebih cepat dan lebih konsisten. Pada proyek ini diusulkan Derma-Cam, yaitu alat pemindai luka berbasis kamera dan kecerdasan buatan yang dirancang untuk membantu proses identifikasi awal luka luar dan luka bakar pada situasi bencana. Sistem ini diarahkan untuk berfungsi sebagai alat bantu pengambilan keputusan awal, bukan sebagai alat diagnosis medis final. Dengan pendekatan ini, hasil dari perangkat dapat digunakan untuk membantu penolong di lapangan dalam memperkirakan tingkat keparahan luka dan menyampaikan informasi awal kepada tenaga medis atau fasilitas kesehatan terdekat. Pendekatan seperti ini sejalan dengan pemanfaatan AI dalam asesmen luka walaupun implementasinya tetap perlu dibatasi secara realistis.

&nbsp;

Secara keseluruhan, pengembangan Derma-Cam dilatarbelakangi oleh kebutuhan akan alat yang portabel, cepat, dan mudah digunakan untuk membantu asesmen awal korban luka di area bencana. Dengan menggabungkan kamera, pemrosesan citra, dan kecerdasan buatan ringan, solusi ini diharapkan dapat menjadi jembatan antara proses identifikasi awal di lapangan dan penanganan medis lanjutan, terutama pada kondisi ketika jumlah korban banyak dan sumber daya yang tersedia terbatas.&nbsp;

&nbsp;

## **2.2 Tinjauan Pustaka** {#2.2-tinjauan-pustaka}

Evaluasi luka secara konvensional masih sangat bergantung pada observasi visual manual, yang memakan waktu dan rentan terhadap subjektivitas. Dalam konteks luka bakar, kedalaman luka dan persentase luas area terdampak (*Total Body Surface Area*/TBSA) adalah dua metrik krusial untuk menentukan tindakan klinis awal.

Potensi kecerdasan buatan dalam memitigasi subjektivitas penilaian medis telah divalidasi oleh berbagai penelitian terkini. Salah satunya, pengembangan alat berbasis AI oleh peneliti di *University of Waterloo* menunjukkan bahwa integrasi citra digital dapat membantu tenaga medis menentukan kedalaman luka bakar secara objektif, melampaui metode observasi visual tradisional yang seringkali tidak konsisten. Sejalan dengan itu, efektivitas arsitektur *Deep Learning* terus berkembang; penggunaan model hibrida seperti *CNN-Transformer* terbukti mampu mengekstraksi fitur tekstur luka dengan presisi tinggi, memberikan hasil klasifikasi yang lebih reliabel bahkan pada dataset yang kompleks.&nbsp;

Mengadaptasi hal tersebut, Derma-Cam menerapkan prinsip serupa namun dengan optimalisasi pada aspek portabilitas. Melalui pendekatan *TinyML*, sistem ini mentransfer kapabilitas inferensi AI ke perangkat genggam berbasis ESP32-CAM. Meskipun memiliki keterbatasan sumber daya komputasi dibandingkan perangkat laboratorium, Derma-Cam menyiasatinya dengan struktur data yang efisien dan konektivitas LoRa, sehingga mampu menjadi jembatan antara kebutuhan pemindaian luka instan di lapangan dengan akurasi diagnosis yang lebih terukur.

&nbsp;

Hasil literatur berikut dapat diimplementasikan ke dalam sistem *embedded* berbiaya rendah (pendekatan *TinyML*), penggunaan mikrokontroler seperti ESP32-CAM menawarkan solusi yang portabel. Meskipun mikrokontroler memiliki memori terbatas , Derma-Cam menyiasatinya dengan mendelegasikan beban komputasi AI (inferensi model CNN) ke *smartphone* pengguna secara *offline*. Sintesis dari berbagai literatur ini mengukuhkan posisi Derma-Cam sebagai alat bantu skrining objektif yang menjembatani kebutuhan lapangan darurat dengan efisiensi perangkat keras.

&nbsp;

### **2.2.1 Fisiologi dan Biologi Luka dan Luka Bakar** {#2.2.1-fisiologi-dan-biologi-luka-dan-luka-bakar}

Kulit merupakan organ terbesar pada tubuh manusia dan berfungsi sebagai pelindung utama terhadap lingkungan luar. Secara umum, kulit tersusun atas epidermis, dermis, dan jaringan subkutan. Susunan ini penting untuk dipahami karena tingkat kerusakan pada masing-masing lapisan akan menentukan keparahan luka, respons nyeri, proses penyembuhan, hingga risiko komplikasi. Dalam konteks alat yang dirancang, pemahaman tentang struktur kulit menjadi dasar karena sistem Derma-Cam bekerja dengan membaca ciri visual pada permukaan luka, seperti warna, kelembapan, tekstur, adanya bula, dan perubahan jaringan, lalu menghubungkannya dengan kemungkinan tingkat keparahan luka. Informasi ini sejalan dengan penjelasan MSD Manual yang menyebut bahwa burn injuries dapat mengenai kulit maupun jaringan yang lebih dalam tergantung pada jenis dan intensitas paparan.&nbsp;

&nbsp;&nbsp;**Gambar  2.1 Struktur lapisan kulit manusia : Epidermis, Dermis, dan Subkutan**

&nbsp;

Pada luka bakar, dua parameter yang paling penting dalam evaluasi klinis adalah kedalaman luka dan luas area tubuh yang terdampak. Artikel di *Military Medicine* menegaskan bahwa *“Burn depth and the percentage of total body surface area (%TBSA) affected by the wound are the two key factors in evaluating burn injuries.”* Artinya, penilaian luka bakar tidak cukup hanya melihat apakah kulit tampak merah atau melepuh, tetapi juga perlu mempertimbangkan seberapa dalam jaringan yang rusak dan seberapa luas area tubuh yang terkena.&nbsp;

Secara umum, luka bakar dapat dibedakan menjadi tiga kelompok utama. Luka bakar superfisial hanya mengenai epidermis, biasanya tampak merah, kering, terasa nyeri, dan tidak menimbulkan bula. Luka bakar *partial-thickness* sudah mengenai epidermis dan sebagian dermis, sehingga sering tampak merah, lembap, bengkak, dan disertai bula. Sementara itu, luka bakar *full-thickness* mengenai seluruh ketebalan kulit hingga jaringan yang lebih dalam, dengan tampilan yang dapat berwarna putih, cokelat, atau kehitaman, serta sensasi nyerinya justru dapat berkurang karena kerusakan ujung saraf. Uraian ini juga dijelaskan pada artikel *Military Medicine* saat membedakan superficial, partial-thickness, dan full-thickness burns.&nbsp;

&nbsp;

| Jenis Luka Bakar | Lapisan Kulit yang Terkena | Ciri Visual | Tingkat Nyeri | Implikasi Awal |
| :---- | :---- | :---- | :---- | :---- |
| Superficial | Epidermis | Merah, kering, tanpa bula | Nyeri | Biasanya ringan, observasi dan pendinginan |
| Partial-thickness | Epidermis \+ sebagian dermis | Merah, lembap, bula, bengkak | Sangat nyeri | Perlu penanganan steril dan evaluasi medis |
| Full-thickness | Seluruh lapisan kulit | Putih/coklat/kehitaman, kering | Bisa menurun | Prioritas penanganan medis lebih lanjut |

**Tabel 2.1 Klasifikasi luka bakar berdasarkan kedalaman**

&nbsp;

Selain kedalaman, luas luka bakar juga sangat menentukan tingkat keparahan. Literatur menjelaskan bahwa luas luka umumnya dinyatakan dalam persentase *Total Body Surface Area (TBSA)*. Pada luka bakar yang luas, kebutuhan cairan dan risiko komplikasi menjadi jauh lebih tinggi. MSD Manual menyebut bahwa pasien dengan luka bakar besar, khususnya di atas 20% TBSA, memerlukan resusitasi cairan. Kalimat yang tertulis adalah *“Patients with large burns (\> 20% TBSA) require fluid resuscitation.”* Hal ini menunjukkan bahwa kesalahan estimasi luas luka dapat berdampak langsung pada keputusan klinis.&nbsp;

Di lapangan, luas luka biasanya diperkirakan dengan *Rule of Nines* atau diagram Lund and Browder. Metode ini memang cepat, tetapi tetap membutuhkan kemampuan observasi yang baik. Artikel *Military Medicine* bahkan menyebut bahwa metode Wallace Rule of Nines dan Lund and Browder chart cenderung dapat melebihi estimasi TBSA pada kondisi tertentu, sehingga pengukuran luas luka secara manual masih menjadi tantangan. Karena itu, pendekatan berbasis citra menjadi relevan sebagai alat bantu yang lebih konsisten.&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

### **2.2.2 Kebutuhan Literatur untuk Merancang dan Membangun Prototipe** {#2.2.2-kebutuhan-literatur-untuk-merancang-dan-membangun-prototipe}

Dalam proyek ini, tinjauan pustaka tidak hanya dibutuhkan untuk memahami luka bakar dari sisi medis, tetapi juga untuk mendukung perancangan prototipe dari sisi teknis. Artinya, literatur yang dipakai harus mencakup dua hal sekaligus : pertama, dasar ilmiah tentang bagaimana luka dinilai; kedua, dasar rekayasa tentang bagaimana citra luka dapat diambil, diproses, dan ditampilkan dalam bentuk hasil klasifikasi yang berguna bagi pengguna. Dengan kata lain, produk Derma-Cam tidak cukup dibangun hanya berdasarkan teori AI atau hanya berdasarkan teori luka, tetapi harus berdiri di antara keduanya.

Dari sisi teknis, kualitas citra menjadi faktor yang sangat menentukan. Model klasifikasi berbasis citra sangat bergantung pada kualitas gambar yang masuk. Bila gambar terlalu gelap, buram, terlalu dekat, atau tidak fokus, maka fitur visual yang penting bisa hilang. Ini menjadi tantangan besar karena alat dirancang untuk digunakan pada situasi bencana, di mana pencahayaan dan kondisi pengambilan gambar sering kali tidak ideal. Artikel *Military Medicine* juga menyoroti bahwa banyak pendekatan burn assessment berbasis RGB image cukup sensitif terhadap perbedaan pigmentasi kulit, cahaya lingkungan, dan posisi kamera. Ini berarti pemilihan kamera, pencahayaan tambahan, dan prosedur penggunaan alat harus dipikirkan sejak awal.&nbsp;

Dari sisi komputasi, salah satu pertanyaan utama adalah apakah proses klasifikasi akan dilakukan langsung pada modul kamera seperti ESP32-CAM, atau gambar hanya diambil oleh kamera lalu diproses di perangkat lain seperti ponsel. Literatur TinyML menunjukkan bahwa machine learning pada perangkat kecil memang memungkinkan, tetapi selalu ada kompromi antara ukuran model, resolusi gambar, kecepatan inferensi, dan penggunaan memori. Studi MDPI tentang klasifikasi citra dengan ESP32 menjelaskan bahwa peningkatan resolusi gambar dapat membantu akurasi, tetapi sekaligus meningkatkan kebutuhan komputasi dan memori perangkat. Ini sangat relevan untuk Derma-Cam, karena luka bakar memiliki detail warna dan tekstur yang cukup penting untuk dibaca model.&nbsp;

&nbsp;

&nbsp;

Dengan mempertimbangkan hal tersebut, arsitektur sistem yang paling realistis untuk prototipe mahasiswa kemungkinan bukan memaksa seluruh model AI berjalan di ESP32-CAM, melainkan menjadikan ESP32-CAM sebagai modul akuisisi citra, sedangkan inferensi model dilakukan pada perangkat lain yang masih *portable*, misalnya smartphone. Pendekatan ini lebih masuk akal karena tetap menjaga biaya perangkat rendah, tetapi tidak membebani mikrokontroler dengan model yang terlalu besar. Literatur tentang TinyML mendukung bahwa sistem embedded harus dirancang sesuai keterbatasan sumber daya, bukan sekadar meniru sistem komputasi besar dalam versi kecil.&nbsp;

Agar hasil alat dapat benar-benar membantu di lapangan, tampilan keluarannya juga perlu dibuat sederhana. Sistem tidak perlu menghasilkan analisis medis yang terlalu kompleks, tetapi cukup memberikan klasifikasi awal yang mudah dipahami, misalnya kategori luka ringan, sedang, atau berat, disertai informasi tindak lanjut awal. Pendekatan seperti ini lebih realistis karena alat diarahkan sebagai **Decision Support Tool**, bukan alat diagnosis final. Prinsip tersebut juga sejalan dengan pengembangan AI di bidang burn assessment yang saat ini lebih banyak diposisikan sebagai alat bantu objektif bagi tenaga kesehatan atau first responder.&nbsp;

&nbsp;

| Aspek Perancangan | Informasi yang Dibutuhkan | Alasan Penting | Dampak ke Desain |
| :---- | :---- | :---- | :---- |
| Medis | Klasifikasi luka bakar, AVPU, TBSA | Menentukan output sistem | Menentukan kategori hasil alat |
| Kamera | Resolusi, fokus, pencahayaan | Mempengaruhi kualitas citra | Menentukan jenis kamera dan LED |
| AI/ML | Metode klasifikasi citra, ukuran model | Menentukan feasibility inferensi | Menentukan model ringan atau tidak |
| Embedded system | Kapasitas memori, daya, komunikasi data | Menentukan batas hardware | Menentukan arsitektur sistem |
| Antarmuka | Tampilan hasil yang sederhana | Memudahkan relawan/petugas | Menentukan UI aplikasi atau layar |

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Tabel  2.2.2 Kebutuhan Literatur untuk Merancang dan Membangun Prototipe.**

&nbsp;

### **2.2.3 Pembahasan Solusi Masalah pada Literatur 1 : AI untuk Karakterisasi Luka Bakar** {#2.2.3-pembahasan-solusi-masalah-pada-literatur-1-:-ai-untuk-karakterisasi-luka-bakar}

Salah satu literatur utama yang relevan untuk proyek ini adalah artikel Jacobson dkk. pada *Military Medicine* tahun 2023 yang membahas karakterisasi luka bakar otomatis menggunakan artificial intelligence. Penelitian ini penting karena secara langsung membahas dua masalah utama pada *Burn Assessment*, yaitu penentuan kedalaman luka dan penentuan luas luka. Pada artikel tersebut dijelaskan bahwa pemeriksaan klinis biasa masih menjadi metode yang paling umum dipakai, tetapi akurasinya terbatas. Bahkan disebutkan bahwa penilaian visual dan taktil manual dapat menghasilkan variabilitas antar-penilai, terutama pada luka partial-thickness. Artikel itu juga menyebut bahwa akurasi klinis pada penilai berpengalaman berada di kisaran 67–76%, dan bisa turun menjadi sekitar 50% pada penilai yang kurang berpengalaman.&nbsp;

Temuan ini sangat penting untuk Derma-Cam karena menunjukkan bahwa masalah penilaian luka bakar memang nyata, bukan sekadar asumsi. Jadi, pengembangan alat bantu berbasis AI punya dasar kebutuhan yang kuat. Dalam studi tersebut, sistem AI dikembangkan dengan pendekatan multimodal, yaitu memanfaatkan *ultrasound B-mode* untuk klasifikasi kedalaman luka dan citra RGB overhead untuk segmentasi area luka. Hasilnya menunjukkan bahwa sistem mampu mengklasifikasikan burn depth dengan accuracy dan F1 *average* di atas 80%, sementara modul segmentasi mencapai *mean global accuracy* lebih dari 84% dan *mean intersection-over-union* di atas 0,74. Ini menunjukkan bahwa AI memang punya potensi untuk meningkatkan objektivitas burn assessment.&nbsp;

&nbsp;

&nbsp;

### **2.2.4 Pembahasan Solusi Masalah pada Literatur 2 : Mobile Tool untuk Penilaian Luka Bakar** {#2.2.4-pembahasan-solusi-masalah-pada-literatur-2-:-mobile-tool-untuk-penilaian-luka-bakar}

Literatur kedua yang paling dekat dengan ide Derma-Cam adalah artikel yang terindeks di PMC berjudul *Artificial Intelligence-powered Mobile Tool for Burn Injury Assessment* dimana penelitian membahas implementasi dalam bentuk aplikasi mobile untuk *first responder.* Dari ringkasan yang tersedia, aplikasi tersebut dirancang untuk mengintegrasikan praktik *Advanced Burn Life Support (ABLS)* dengan AI agar proses penilaian luka bakar menjadi lebih terstandar dan lebih mudah dibawa ke lapangan. Dalam abstraknya juga disebutkan bahwa aplikasi ini membantu pengumpulan informasi penting, misalnya berat badan pasien untuk menghitung kebutuhan *fluid resuscitation.*&nbsp;

Literatur ini relevan karena arah pengembangannya hampir sama dengan Derma-Cam, yaitu membawa proses *burn assessment* lebih dekat ke titik kejadian, bukan hanya dilakukan di rumah sakit. Dalam kondisi bencana, penilaian awal justru paling dibutuhkan saat korban pertama kali ditemukan atau dievakuasi. Dengan adanya sistem mobile berbasis AI, *first responder* atau relawan bisa memperoleh gambaran awal yang lebih objektif sebelum pasien mendapatkan evaluasi klinis lanjutan. Hal ini memperkuat bahwa konsep Derma-Cam bukan ide yang berdiri sendiri, tetapi sejalan dengan perkembangan solusi yang memang sedang diteliti.&nbsp;

Dari sisi performa, literatur tersebut menyebut model prototipe mereka mencapai F1-score sekitar 78% untuk membedakan tingkat keparahan luka. Nilai ini menunjukkan bahwa sistem AI untuk burn assessment sudah cukup menjanjikan, tetapi belum cukup untuk dijadikan penentu tunggal keputusan klinis. Karena itu, posisi yang paling aman untuk Derma-Cam adalah sebagai alat bantu skrining awal, bukan alat diagnosis.&nbsp;

&nbsp;

### **2.2.5 Pembahasan Solusi Masalah pada Literatur 3 : TinyML dan ESP32-CAM** {#2.2.5-pembahasan-solusi-masalah-pada-literatur-3-:-tinyml-dan-esp32-cam}

Dari sisi implementasi perangkat, konsep TinyML menjadi penting karena Derma-Cam dirancang dengan target biaya rendah dan perangkat yang portabel. TinyML pada dasarnya adalah penerapan model *machine learning* pada perangkat dengan sumber daya terbatas seperti mikrokontroler. Konsep ini relevan karena pada kondisi bencana, sistem yang terlalu bergantung pada internet atau cloud akan lebih sulit diandalkan. Dengan inferensi lokal atau semi-lokal, alat dapat tetap bekerja walaupun konektivitas terbatas. Literatur TinyML menjelaskan bahwa proses pelatihan model umumnya tetap dilakukan di luar perangkat, lalu model yang sudah diperkecil dan dioptimasi dipindahkan ke perangkat untuk inferensi.&nbsp;

Studi MDPI tentang klasifikasi citra menggunakan ESP32 menunjukkan bahwa perangkat seperti ini memang bisa menjalankan *image classification,* tetapi ada batasan pada memori dan komputasi. Semakin tinggi resolusi gambar, semakin besar pula kebutuhan resource. Sebaliknya, bila resolusi terlalu rendah, informasi penting pada gambar bisa hilang. Hal ini penting arena karakteristik luka bakar sering bergantung pada perbedaan warna, tekstur, kelembapan, dan batas area luka.&nbsp;

Karena itu, desain sistem yang paling realistis kemungkinan adalah menggunakan ESP32-CAM sebagai modul pengambil gambar, lalu hasil citra dikirim ke aplikasi pada smartphone untuk diproses. Pendekatan ini membuat sistem tetap murah dan portabel, tetapi tidak memaksakan model berat berjalan langsung di mikrokontroler. Di sisi lain, bila nantinya model yang dipakai cukup ringan, bukan tidak mungkin sebagian inferensi bisa dilakukan langsung di perangkat embedded.

&nbsp;

### **2.2.6 Sintesis Tinjauan Pustaka** {#2.2.6-sintesis-tinjauan-pustaka}

Dari seluruh literatur yang dibahas, terlihat bahwa masalah *burn assessment* memang berada di pertemuan antara tantangan medis dan tantangan teknis. Dari sisi medis, kedalaman luka dan luas luka merupakan dua parameter utama, tetapi penilaian manual masih memiliki keterbatasan akurasi dan sangat dipengaruhi pengalaman penilai. Dari sisi teknologi, AI telah menunjukkan potensi untuk membantu klasifikasi dan segmentasi luka bakar, baik melalui sistem multimodal yang lebih kompleks maupun aplikasi mobile yang lebih praktis. Sementara itu, dari sisi implementasi perangkat, penggunaan modul seperti ESP32-CAM dan pendekatan TinyML membuka peluang untuk membuat purwarupa berbiaya rendah, walaupun tetap ada keterbatasan pada kualitas citra, memori, dan komputasi.&nbsp;

Dengan demikian, posisi Derma-Cam dalam tinjauan pustaka ini dapat dirumuskan sebagai alat bantu skrining awal luka berbasis citra yang mencoba menjembatani kebutuhan asesmen cepat di lapangan dengan keterbatasan perangkat murah dan portabel. Nilai utamanya bukan menggantikan dokter atau petugas medis, melainkan membantu penolong non-spesialis memperoleh gambaran awal yang lebih objektif ketika harus bertindak cepat dalam situasi bencana.&nbsp;

&nbsp;

## **2.3 Teori Dasar dan Mata Kuliah BME Terkait** {#2.3-teori-dasar-dan-mata-kuliah-bme-terkait}

Pengembangan sistem Derma-Cam didasarkan pada beberapa konsep dasar dalam bidang Teknik Biomedik, yaitu :

* **Biomedical Signal & Image Processing**  
  Digunakan dalam pengolahan citra luka untuk mengekstraksi fitur penting yang akan digunakan dalam proses klasifikasi oleh model AI.  
* **Artificial Intelligence in Healthcare**  
  Pemanfaatan model CNN untuk mengenali pola visual luka dan mengklasifikasikan tingkat keparahannya secara otomatis.  
* **Medical Instrumentation**  
  Perancangan perangkat keras seperti kamera (ESP32-CAM) dan sistem pencahayaan LED untuk mendukung akuisisi data yang optimal.  
* **Telemedicine & Healthcare Communication Systems**  
  Penggunaan teknologi LoRa untuk mendukung pengiriman data medis dari lokasi kejadian ke fasilitas kesehatan.  
* **Human Factors in Biomedical Engineering**  
  Desain alat yang mempertimbangkan kemudahan penggunaan oleh non-tenaga medis dalam kondisi darurat.  
  &nbsp;  
  &nbsp;  
  &nbsp;  
  &nbsp;  
  &nbsp;  
  &nbsp;

&nbsp;

## **2.4 Standar dan Batasan Realistis** {#2.4-standar-dan-batasan-realistis}

### **2.4.1 Standar Target Solusi** {#2.4.1-standar-target-solusi}

Derma-Cam dirancang sebagai alat bantu skrining awal untuk membantu penilaian luka luar dan luka bakar pada situasi bencana. Karena digunakan pada tahap awal penanganan korban, target utama alat ini bukan memberikan diagnosis medis definitif, melainkan membantu pengguna memperoleh gambaran cepat mengenai tingkat keparahan luka berdasarkan citra yang diambil. Penempatan fungsi ini penting agar ruang lingkup alat tetap realistis dan tidak melampaui kewenangan tenaga medis.&nbsp;

Berdasarkan tujuan, standar target solusi Derma-Cam dapat disimpulkan : **Pertama**, alat mampu menghasilkan hasil sederhana dan mudah dipahami, misalnya kategori luka ringan, sedang, atau berat, atau klasifikasi awal luka bakar berdasarkan tingkat keparahan. **Kedua**, alat dapat digunakan oleh pengguna non-spesialis, seperti relawan atau petugas lapangan, sehingga penggunaan harus sederhana. **Ketiga**, alat mendukung proses penanganan awal secara cepat, sehingga waktu penggunaan per korban idealnya singkat dan tidak menghambat alur evakuasi. **Keempat**, hasil alat diposisikan sebagai pendukung keputusan awal, bukan pengganti evaluasi klinis tenaga medis.&nbsp;

Dalam konteks luka bakar, target solusi juga harus mengikuti parameter klinis yang paling relevan, yaitu kedalaman luka dan luas area tubuh yang terdampak. Artikel *Military Medicine* menyebut bahwa *burn depth* dan persentase *total body surface area* merupakan dua faktor kunci dalam evaluasi luka bakar. Artinya, Derma-Cam hasilnya diarahkan pada dua aspek tersebut, atau setidaknya pada kategori keparahan yang diturunkan dari keduanya.&nbsp;

&nbsp;

### **2.4.2 Standar Rumusan Solusi** {#2.4.2-standar-rumusan-solusi}

Selain standar target, perlu juga dirumuskan standar pada tingkat implementasi atau bentuk solusi. Derma-Cam harus memenuhi prinsip portabel, sederhana, aman digunakan, dan dapat direalisasikan dengan komponen yang terjangkau. Perangkat sebaiknya berbentuk handheld atau mudah digenggam, sehingga pengguna dapat mengoperasikannya dengan cepat di lapangan. Kamera harus ditempatkan pada posisi yang stabil, dan bila perlu ditambah pencahayaan buatan seperti LED agar hasil gambar tidak selalu bergantung pada kondisi cahaya lingkungan. Hal ini penting karena akurasi *burn assessment* sangat sensitif terhadap kualitas gambar, termasuk pencahayaan, sudut pengambilan, dan detail permukaan luka.&nbsp;

Standar rumusan solusi juga perlu mengatur bentuk keluaran sistem. Derma-Cam sebaiknya tidak menghasilkan narasi medis yang panjang, tetapi memberikan tampilan singkat yang langsung dapat dipahami, misalnya luka ringan (1), curiga luka bakar derajat sedang (2), atau prioritas evaluasi medis (2). Kalau ditambah panduan, maka panduan bersifat umum dan aman, seperti anjuran merujuk atau melakukan penanganan awal dasar, bukan instruksi terapi definitif. Dengan demikian, desain alat tetap sesuai fungsi sebagai alat skrining awal dan tidak menimbulkan klaim berlebihan.

Dari sisi sistem elektronik, solusi juga perlu dirumuskan secara realistis. Bila ESP32-CAM digunakan, maka modul tersebut lebih aman diposisikan sebagai modul akuisisi gambar, sedangkan pemrosesan model dapat dipindahkan ke perangkat lain. Literatur TinyML menunjukkan bahwa inferensi pada perangkat kecil memang memungkinkan, tetapi sangat dibatasi oleh memori, kapasitas komputasi, dan kebutuhan optimasi model. Karena itu, rumusan solusi harus mempertimbangkan pembagian fungsi sistem yang efisien, bukan memaksakan ke satu perangkat kecil.&nbsp;

&nbsp;

### **2.4.3 Technical Constraints** {#2.4.3-technical-constraints}

Batasan teknis utama pada Derma-Cam berasal dari kamera, kondisi pencahayaan, kualitas gambar, serta kemampuan komputasi device. Pada sistem berbasis *scanning*, kualitas gambar merupakan faktor yang sangat menentukan dimana gambaran  yang terlalu gelap, tidak fokus, terlalu dekat, atau terlalu jauh dapat menyebabkan fitur visual luka tidak terbaca dengan baik oleh model. Ini menjadi tantangan karena perangkat dirancang untuk digunakan pada situasi bencana, di mana kondisi lingkungan sangat mungkin tidak stabil. Selain itu, variasi warna kulit, latar belakang gambar, dan sudut pengambilan juga dapat mempengaruhi hasil klasifikasi. Penelitian pada *burn assessment* berbasis AI menegaskan bahwa perbedaan kondisi masih menjadi salah satu sumber variasi performa sistem.&nbsp;

Batasan teknis berikutnya adalah kapasitas perangkat embedded. Bila ESP32-CAM digunakan sebagai inti perangkat, maka keterbatasannya meliputi RAM kecil, daya komputasi terbatas, serta keterbatasan ukuran model yang dapat dijalankan secara langsung. Literatur TinyML menekankan bahwa bukan hanya ukuran model yang menjadi masalah, tetapi juga activation memory dan kebutuhan proses inferensi. Karena itu, model klasifikasi citra untuk Derma-Cam harus dibuat seringan mungkin, atau sebagian proses harus dipindahkan ke perangkat lain. Ada kemungkinan hasil bukan sepenuhnya on-device AI, tetapi *semi-edge system,* yaitu kamera mengambil gambar dan perangkat lain melakukan inferensi.&nbsp;

Keterbatasan teknis lain yang perlu dipertimbangkan adalah ketergantungan dataset. Akurasi hasil AI sangat bergantung pada variasi dataset yang digunakan. Jika dataset terlalu terbatas dan tidak ada variasi kondisi nyata di lapangan, maka model berisiko gagal saat diuji pada kondisi berbeda. Dalam pengembangan prototipe mahasiswa, pengumpulan dataset yang benar-benar representatif juga menjadi tantangan tersendiri, terutama karena isu etika dan akses terhadap data medis. Oleh sebab itu, sejak awal perlu disadari bahwa hasil klasifikasi yang diperoleh prototipe kemungkinan besar masih berada pada tingkat pembuktian konsep, belum pada tingkat akurasi klinis.

Selain itu, Derma-Cam juga akan dibatasi oleh daya tahan baterai, kestabilan komunikasi data, dan ketahanan fisik alat. Jika gambar harus dikirim ke smartphone, maka koneksi lokal seperti Wi-Fi direct atau Bluetooth harus cukup stabil. Bila baterai terlalu kecil, alat tidak akan cocok untuk penggunaan lapangan. Bila *hardware* tidak kuat, alat juga rentan rusak dalam kondisi darurat. Jadi, aspek teknis tidak berhenti pada AI, tetapi mencakup keseluruhan integrasi hardware dan usability.

&nbsp;

### **2.4.4 Non-Technical Constraints** {#2.4.4-non-technical-constraints}

Di luar aspek teknis, Derma-Cam juga memiliki beberapa batasan nonteknis yang perlu dinyatakan secara jelas. Salah satunya adalah batasan ekonomi. Karena alat ini dirancang sebagai prototipe mahasiswa, maka biaya pembuatan harus dijaga tetap rendah agar realistis untuk diwujudkan. Ini berarti pemilihan komponen harus mempertimbangkan keseimbangan antara performa dan harga. Kamera dengan kualitas terlalu tinggi mungkin menghasilkan citra yang lebih baik, tetapi akan menaikkan biaya dan membuat prototipe kurang sesuai dengan tujuan awal sebagai solusi murah dan portabel. Karena itu, desain akhir perlu mempertimbangkan kompromi antara kualitas teknis dan keterjangkauan.

Batasan nonteknis lain adalah etika penggunaan data luka. Sistem berbasis kamera untuk penilaian luka tentu berkaitan dengan data visual tubuh manusia yang sensitif. Bila nantinya dilakukan pengumpulan atau pengujian pada manusia, maka harus ada perhatian terhadap izin penggunaan data, privasi, persetujuan subjek, serta keamanan penyimpanan gambar. Bahkan bila prototipe hanya diuji menggunakan gambar sekunder atau simulasi, penjelasan mengenai keterbatasan etis tetap perlu dicantumkan agar pengembangan alat tidak terkesan mengabaikan aspek keselamatan dan martabat pasien. Literatur terbaru tentang AI dalam burn assessment juga menyoroti bahwa validasi klinis, generalisasi model, dan tata kelola data merupakan isu penting sebelum teknologi seperti ini dapat diadopsi secara luas.&nbsp;

Selain itu, ada batasan dari sisi keselamatan penggunaan. Karena alat ini ditujukan untuk situasi darurat, perangkat tidak boleh menimbulkan risiko tambahan bagi korban maupun pengguna. Permukaan alat harus aman disentuh, tidak memiliki bagian tajam, dan tidak menghasilkan panas berlebih dari komponen elektronik atau lampu LED. Kalau ada tambahan pencahayaan, intensitasnya juga harus cukup untuk membantu pengambilan gambar tanpa mengganggu pengguna atau memperparah ketidaknyamanan korban.

3. # **DESAIN AWAL** {#desain-awal}

   ## **3.1	Desain Awal** {#3.1-desain-awal}

   ### **3.1.1	Konsep yang Dipertimbangkan** {#3.1.1-konsep-yang-dipertimbangkan}

   Berdasarkan *Research brainstorming* dan analisis kebutuhan, terdapat tiga pendekatan konsep solusi yang dipertimbangkan untuk membantu identifikasi awal luka luar dan luka bakar pada situasi darurat :  
   &nbsp;  
1. Sistem Monitoring Wearable Smart Vest berbasis Sensor Lingkungan

   Konsep ini menggunakan vest khusus yang dilengkapi sensor GPS, gas berbahaya, suhu, dan tombol darurat untuk memantau keselamatan personel tim SAR secara *real-time*.

   **Kelebihan :** Memberikan perlindungan preventif bagi petugas di area berbahaya dan mempermudah koordinasi lokasi antar anggota tim. **Kekurangan :** Terlalu berfokus pada keselamatan petugas (K3) dan kurang memberikan solusi klinis langsung bagi penanganan medis korban bencana.

&nbsp;

2. Pendekatan Analisis Citra RGB Berbasis AI (Derma-Cam)

   Konsep ini menggunakan perangkat kamera *portable* untuk mengambil citra luka, yang kemudian diproses menggunakan model kecerdasan buatan *(Artificial Intelligence)* untuk mengklasifikasikan tingkat keparahan luka secara otomatis.

   **Kelebihan** **:** Penilaian menjadi lebih objektif, desain dapat dibuat portable, dan biaya pembuatannya relatif ekonomis.&nbsp;  
   **Kekurangan :** Akurasi klasifikasi sangat bergantung pada kualitas pencahayaan dan kejelasan citra yang ditangkap, serta tidak dapat mengukur kedalaman jaringan secara langsung.

   &nbsp;

3. Perangkat Diagnosa Syok Hipovolemik Genggam (Shock-Grip)

   Konsep ini berupa alat *handheld* untuk mendeteksi dehidrasi ekstrem dan syok pada korban melalui pengukuran *Galvanic Skin Response* (GSR) pada telapak tangan dan kekuatan kontraksi otot (FSR).

   **Kelebihan** : Biaya produksi sangat rendah, komponen mudah dirakit, dan memberikan indikator fisiologis (keringat/otot) yang tidak terlihat secara visual. **Kekurangan** : Akurasi sensor GSR rentan terhadap gangguan lingkungan dan hasil pengukuran otot bisa sangat bervariasi tergantung usia serta kondisi fisik korban sebelum cedera.

   &nbsp;

   ### **3.1.2	Spesifikasi Produk Desain Akhir \- Komponen dan Material** {#3.1.2-spesifikasi-produk-desain-akhir---komponen-dan-material}

   Perangkat keras Derma-Cam dirancang secara ergonomis dan efisien. Berikut merupakan spesifikasi lengkap komponen dan material untuk purwarupa Derma-Cam :

| Komponen | Spesifikasi Teknis | Fungsi | Subsistem |
| ----- | ----- | ----- | :---: |
| Mikrokontroler Kamera | Modul ESP32-CAM dengan Wi-Fi/BT terintegrasi. Terdapat pin GPIO 2617, 364 (GND), dan 40-pin. | Sensor utama penangkap citra luka. Sebagai otak pemrosesan dasar pada perangkat keras. | Elektronik / Processing |
| Sensor Kamera | Modul lensa OV2640 Ultra-Compact terhubung melalui kabel pita CSI (warna biru). | Menangkap citra luka secara real-time. | Optik / Sensing |
| Pencahayaan Tambahan | LED Ring Light (Front Mount). | Memastikan objek luka terlihat jelas pada kondisi minim cahaya di lokasi darurat. | Optik / Iluminasi |
| Layar Penampil | 0.96 Inch I2C OLED Display. Kabel I2C SDA (kuning) dan I2C SCL (hijau). | Menampilkan status perangkat dan informasi dasar bagi pengguna. | UI/UX |
| Tombol Kontrol | Orange Trigger Button yang terhubung ke pin GPIO. | Memulai proses pemindaian atau pengambilan gambar citra luka. | Mekanik / Input |
| Sumber Daya | Baterai Li-Ion 18650 (3.7V \- 2600mAh). Jalur daya menggunakan kabel merah (+VCC) dan hitam (GND). | Sumber daya utama yang memungkinkan operasional tanpa kabel di lapangan. | Daya |
| Sistem Pengisian Daya | Modul Charging Circuit TP4056 dengan USB-C Charging Port. | Mengisi ulang daya baterai perangkat dengan aman. | Daya |
| Casing Perangkat | Desain Clamshell. Disatukan dengan sekrup perakitan 4x M3. | Melindungi komponen elektronik dan menjaga ergonomi perangkat genggam. | ABS Plastic / Mekanik |

   &nbsp;

   ### **3.1.3	Sketsa Fisik dan Dimensi Produk** {#3.1.3-sketsa-fisik-dan-dimensi-produk}

   &nbsp;Desain Hardware Derma-Cam mengambil bentuk menyerupa*i Handheld Thermogun/Scanner* yang ergonomis. Desain mekanik utama dicetak menggunakan plastik ABS dengan metode clamshell (dua sisi yang disatukan). Pada bagian depan, terdapat sensor kamera OV2640 yang dikelilingi oleh LED *Ring Light* untuk memberikan pencahayaan langsung pada target luka.

   &nbsp;

   &nbsp;Pada bagian belakang perangkat yang menghadap ke pengguna, diletakkan layar OLED 0.96 inci yang dimiringkan agar memudahkan pembacaan hasil pemindaian saat alat diarahkan ke bawah. *Grip Hardware* dirancang untuk memuat baterai Li-Ion 18650 di dalamnya guna menjaga keseimbangan beban, dengan trigger button berwarna oranye diletakkan pada posisi jari telunjuk. Modul pengisian daya USB-C disematkan pada bagian bawah gagang alat.

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;**Gambar 3.1.3 Derma-Cam Hardware Mockup dalam 2D dan 3D**

**3.1.4	Ilustrasi Perancangan Mekanisme Sistem**

Alur kerja dan mekanisme sistem Derma-Cam dirancang agar dapat memfasilitasi pengguna lapangan secara efektif melalui tahapan berikut :

> > > 1. Scan Luka: Pengguna mengarahkan perangkat ke area cedera. Sistem akan menangkap gambar menggunakan kamera ESP32-CAM dengan bantuan pencahayaan LED.  
> > > 2. Identifikasi Luka : Sistem kecerdasan buatan berbasis *Computer Vision* (dengan CNN) menganalisis citra untuk mengklasifikasikan jenis dan tingkat keparahan luka secara offline melalui aplikasi seluler.  
> > > 3. Penanggulangan P3K : Hasil analisis ditampilkan dalam bentuk skrining awal beserta panduan tindakan pertolongan pertama yang mudah dipahami oleh petugas non-medis.  
> > > 4. Pencatatan Data : Data hasil skrining awal, foto luka, dan waktu pemeriksaan disimpan dalam sistem database untuk kebutuhan observasi dan dokumentasi.  
> > > 5. Pengiriman ke Fasilitas : Data akan dikirimkan ke fasilitas kesehatan atau puskesmas terdekat menggunakan integrasi telekomunikasi jarak jauh (LoRa) di area minim sinyal.

   ## **3.2	Kompleksitas Desain Sistem** {#3.2-kompleksitas-desain-sistem}

   ### **3.2.1	Perangkat Keras dan Lingkungan Fisik** {#3.2.1-perangkat-keras-dan-lingkungan-fisik}

   Kompleksitas utama pada perangkat keras terletak pada penggunaan modul kamera yang masih mengandalkan fitur *fixed focus*. Keterbatasan ini memunculkan tantangan kerentanan gambar yang menjadi *blur* (kabur) apabila evakuator tidak presisi dalam mengatur jarak pemotretan. Selain itu, perancangan Hardware masih menghadapi limitasi dalam mencapai sertifikasi kedap air dan debu (IP Rating) yang memadai, sehingga diperlukan kehati-hatian ekstra saat mengoperasikan alat ini di medan bencana yang kotor dan ekstrem.

   ### **3.2.2	Kecerdasan Buatan dan Batasan Komputasi** {#3.2.2-kecerdasan-buatan-dan-batasan-komputasi}

   &nbsp;Pengembangan model kecerdasan buatan Derma-Cam memiliki tantangan komputasi akibat memori ESP32 yang terbatas, yang menyebabkan perangkat ini belum mampu menjalankan kecerdasan buatan komputasi mandiri (Edge AI). Pemrosesan citra sepenuhnya dialihkan dan sangat bergantung pada daya komputasi serta baterai dari smartphone milik relawan melalui proses operasional offline. Kompleksitas lainnya adalah memitigasi potensi bias akurasi AI pada variasi warna kulit tertentu. Dataset citra yang digunakan sebagai referensi pelatihan awal bersumber dari data terbuka (open source) yang mungkin kurang bervariasi secara demografis, sehingga diperlukan proses anotasi yang hati-hati agar pengenalan luka tetap adaptif terhadap data pasien yang nyata.

   ### **3.2.3	Fungsi Utama dan Validasi Klinis** {#3.2.3-fungsi-utama-dan-validasi-klinis}

   Batasan kompleksitas juga terlihat pada tujuan penggunaan produk. Meskipun menggunakan pendekatan berbasis CNN yang efisien, desain algoritma dikondisikan murni sebagai pendukung sistem triase visual darurat. Perangkat diatur agar memberikan informasi yang disederhanakan sebagai bentuk skrining awal—bukan sebuah diagnosis klinis final layaknya perangkat kesehatan bersertifikasi resmi. Batasan ini diimplementasikan guna menghindari kesalahan persepsi pengguna non-medis saat memberikan tindakan penanggulangan P3K di lapangan.

4. # **EVALUASI DESAIN AWAL** {#evaluasi-desain-awal}

   ## **4.1	Evaluasi Desain Awal**&nbsp; {#4.1-evaluasi-desain-awal}

   ### **4.1.1	Ringkasan Matriks Desain** {#4.1.1-ringkasan-matriks-desain}

   Evaluasi desain dilakukan dengan membandingkan tiga konsep yang telah dipertimbangkan pada tahap sebelumnya, yaitu pendekatan asesmen manual, analisis citra berbasis AI (Derma-Cam), dan pencitraan lanjutan. Parameter yang digunakan dalam matriks desain meliputi aspek biaya, portabilitas, kemudahan penggunaan, akurasi, serta kesiapan implementasi di kondisi darurat.  
   Hasil evaluasi menunjukkan bahwa pendekatan asesmen manual memiliki keunggulan dari sisi biaya dan kemudahan implementasi, namun memiliki kelemahan signifikan pada tingkat subjektivitas dan ketergantungan terhadap pengalaman pengguna. Sementara itu, pendekatan pencitraan lanjutan unggul dalam hal akurasi diagnostik, tetapi memiliki keterbatasan dari segi biaya yang sangat tinggi, kompleksitas sistem, serta rendahnya mobilitas untuk kondisi lapangan.  
   Pendekatan analisis citra berbasis AI (Derma-Cam) berada pada posisi yang paling seimbang di antara ketiga konsep tersebut. Meskipun memiliki keterbatasan pada akurasi yang bergantung pada kualitas gambar dan dataset, konsep ini dinilai paling memenuhi kebutuhan sistem yang portabel, ekonomis, dan tetap memberikan peningkatan objektivitas dibanding metode manual.

   ### **4.1.2	Desain Akhir**&nbsp; {#4.1.2-desain-akhir}

   Berdasarkan hasil matriks desain, konsep Derma-Cam dipilih sebagai desain akhir yang diusulkan. Perangkat ini mengintegrasikan kamera berbasis ESP32-CAM dengan sistem analisis citra berbasis kecerdasan buatan yang dijalankan melalui smartphone.  
   Desain ini dipilih karena mampu menjawab kebutuhan utama dalam kondisi darurat, yaitu kecepatan, kemudahan penggunaan oleh non-tenaga medis, serta fleksibilitas penggunaan di lapangan. Selain itu, penggunaan komponen yang relatif terjangkau menjadikan solusi ini lebih realistis untuk dikembangkan sebagai purwarupa hingga tahap produksi awal.  
   Namun demikian, desain ini tetap memiliki sejumlah keterbatasan, seperti penggunaan kamera dengan *fixed focus* yang berpotensi menghasilkan gambar blur apabila jarak tidak optimal, serta ketergantungan pada perangkat eksternal untuk proses komputasi.

   ## **4.2	Studi Kelayakan Desain Akhir yang Diusulkan** {#4.2-studi-kelayakan-desain-akhir-yang-diusulkan}

   **4.2.1	Potensi Calon Pengguna**

   Perangkat Derma-Cam dirancang untuk digunakan oleh berbagai kalangan, khususnya pada situasi darurat atau bencana. Calon pengguna utama meliputi relawan kemanusiaan, petugas lapangan non-medis, tim SAR atau BASARNAS, serta tenaga kesehatan di fasilitas dengan keterbatasan alat. Selain itu, perangkat ini juga berpotensi digunakan di daerah terpencil yang memiliki keterbatasan akses terhadap tenaga medis profesional. Dengan antarmuka yang sederhana dan panduan berbasis visual, Derma-Cam diharapkan dapat membantu pengguna awam dalam melakukan penilaian awal terhadap kondisi luka.

   ### **4.2.2	Potensi Manfaat bagi Pengguna** {#4.2.2-potensi-manfaat-bagi-pengguna}

   Manfaat utama dari penggunaan Derma-Cam adalah peningkatan objektivitas dalam proses identifikasi luka dibandingkan metode manual. Dengan bantuan analisis citra berbasis AI, pengguna dapat memperoleh hasil skrining awal secara lebih konsisten. Selain itu, perangkat ini juga memberikan panduan tindakan pertolongan pertama (P3K) yang dapat langsung diterapkan di lapangan. Hal ini sangat penting dalam kondisi darurat, di mana keterlambatan penanganan dapat memperburuk kondisi pasien.  
   Namun, perlu ditekankan bahwa perangkat ini tidak dimaksudkan sebagai alat diagnosis klinis. Sistem hanya memberikan rekomendasi berbasis skrining awal untuk membantu proses triase, sehingga keputusan medis tetap harus dilakukan oleh tenaga profesional.

   ### **4.2.3	Potensi Skalabilitas dari Prototyping ke Produksi Massal** {#4.2.3-potensi-skalabilitas-dari-prototyping-ke-produksi-massal}

   Dari sisi skalabilitas, desain Derma-Cam memiliki potensi yang cukup baik untuk dikembangkan ke tahap produksi massal. Hal ini didukung oleh penggunaan komponen yang relatif umum dan mudah diperoleh, seperti ESP32-CAM, modul baterai Li-Ion, serta casing berbasis plastik ABS.  
   Meskipun demikian, terdapat beberapa aspek yang perlu ditingkatkan sebelum memasuki tahap produksi massal. Salah satunya adalah desain *hardware* yang saat ini belum memenuhi standar ketahanan lingkungan, sehingga rentan terhadap debu dan air. Selain itu, peningkatan kualitas sensor kamera, seperti penggunaan auto-focus atau sensor dengan resolusi lebih tinggi, juga perlu dipertimbangkan untuk meningkatkan keandalan sistem.

   ### **4.2.4	Analisis Ekonomi Teknik** {#4.2.4-analisis-ekonomi-teknik}

   Secara ekonomi, pengembangan Derma-Cam tergolong dalam kategori biaya rendah hingga menengah dibandingkan dengan solusi pencitraan medis lainnya. Penggunaan AI *open-source* dan perangkat keras yang terjangkau menjadi keunggulan utama dalam menekan biaya produksi. Namun, terdapat beberapa *trade-off* yang harus diperhatikan. Keterbatasan memori pada ESP32 menyebabkan sistem belum mampu menjalankan komputasi AI secara mandiri (Edge AI), sehingga bergantung pada smartphone sebagai media pemrosesan. Hal ini berimplikasi pada kebutuhan tambahan perangkat serta konsumsi daya yang lebih besar dari sisi pengguna.  
   Selain itu, aspek akurasi model AI juga perlu ditingkatkan melalui pengembangan dataset yang lebih beragam. Investasi pada pengumpulan dan anotasi data menjadi faktor penting dalam meningkatkan performa sistem secara keseluruhan. Secara keseluruhan, desain Derma-Cam dapat dikatakan layak secara ekonomi untuk tahap pengembangan awal dan prototyping, dengan potensi peningkatan nilai guna apabila dilakukan optimalisasi lebih lanjut pada aspek teknis dan validasi sistem.  
   &nbsp;

5. # **PERENCANAAN PROYEK DAN TANGGUNG JAWAB** {#perencanaan-proyek-dan-tanggung-jawab}

   ## **5.1	Timeline dan Rincian Pekerjaan** {#5.1-timeline-dan-rincian-pekerjaan}

   ## 

   ## **5.2	Pemetaan Pengetahuan dan Keterampilan Tim** {#5.2-pemetaan-pengetahuan-dan-keterampilan-tim}

   ### **Kemampuan Awal yang Sudah Dimiliki Kelompok** {#kemampuan-awal-yang-sudah-dimiliki-kelompok}

   ### Tim Interdisiplin sudah memiliki campuran dari ketiga jurusan dalam Departemen Teknik Elektro dengan ilmu masing- masing. Hal tersebut membantu kombinasi ilmu untuk memulai pengembangan proyek Derma-Cam, baik dari sisi teknis maupun pemahaman konsep. {#tim-interdisiplin-sudah-memiliki-campuran-dari-ketiga-jurusan-dalam-departemen-teknik-elektro-dengan-ilmu-masing--masing.-hal-tersebut-membantu-kombinasi-ilmu-untuk-memulai-pengembangan-proyek-derma-cam,-baik-dari-sisi-teknis-maupun-pemahaman-konsep.}

* **Teknik Elektro**  
  Anggota dari Teknik Elektro telah memahami dasar sistem embedded serta penggunaan mikrokontroler seperti ESP32. Selain itu, sudah memiliki pengalaman dalam merakit rangkaian elektronik sederhana dan mengintegrasikan komponen seperti kamera dan LED. Kemampuan dalam proses perakitan dan pengujian hardware juga sudah cukup mendukung tahap awal pengembangan alat.  
* **Teknik Komputer**  
  Anggota dari Teknik Komputer telah menguasai dasar pemrograman dan memiliki pemahaman awal mengenai machine learning serta pengolahan citra. Mereka juga sudah familiar dengan penggunaan framework seperti TensorFlow atau PyTorch untuk klasifikasi gambar, serta memiliki pengalaman dalam pengembangan aplikasi sederhana.  
* **Teknik Biomedik**  
  Anggota dari Teknik Biomedik telah memahami konsep dasar terkait jenis luka, klasifikasi luka bakar, serta metode penilaian kondisi korban seperti AVPU dan Rule of Nines. Selain itu, sudah memiliki kemampuan dalam membaca dan menganalisis literatur medis sebagai dasar validasi sistem.  
* **Seluruh Tim**  
  Secara umum, seluruh anggota tim sudah terbiasa menggunakan tools pendukung seperti Microsoft Word dan PowerPoint untuk dokumentasi dan presentasi. Tim juga telah memiliki pengalaman bekerja secara kolaboratif dalam menyelesaikan proyek berbasis tim.

  ### **Keterampilan yang Perlu Dikembangkan Selama Proyek** {#keterampilan-yang-perlu-dikembangkan-selama-proyek}

  ### Selain kemampuan awal yang sudah dimiliki, terdapat beberapa hal yang masih perlu dipelajari agar pengembangan sistem dapat berjalan secara optimal. {#selain-kemampuan-awal-yang-sudah-dimiliki,-terdapat-beberapa-hal-yang-masih-perlu-dipelajari-agar-pengembangan-sistem-dapat-berjalan-secara-optimal.}

* **Teknik Elektro**  
  Perlu memperdalam pemahaman terkait penggunaan modul ESP32-CAM, terutama dalam menjaga kualitas citra yang stabil di berbagai kondisi pencahayaan. Selain itu, diperlukan pembelajaran lebih lanjut mengenai implementasi komunikasi LoRa untuk pengiriman data serta pengelolaan konsumsi daya pada perangkat portabel.  
* **Teknik Komputer**  
  Perlu mengembangkan kemampuan dalam membangun model AI yang ringan agar dapat dijalankan pada perangkat dengan keterbatasan resource. Selain itu, perlu memahami teknik optimasi model serta integrasi sistem AI dengan aplikasi mobile untuk pemrosesan data secara offline.  
* **Teknik Biomedik**  
  Perlu memperdalam pemahaman terhadap variasi kondisi luka berdasarkan citra nyata serta melakukan validasi terhadap hasil klasifikasi AI agar tetap sesuai dengan prinsip medis. Selain itu, penting untuk memahami batasan penggunaan alat sebagai sistem skrining awal.  
* **Seluruh Tim**  
  Tim perlu mempelajari aspek desain produk yang sesuai untuk kondisi darurat, seperti kemudahan penggunaan, ketahanan alat, serta efisiensi penggunaan di lapangan. Selain itu, pemahaman mengenai regulasi alat kesehatan juga diperlukan sebagai dasar pengembangan lebih lanjut.  
  &nbsp;

  ## **5.3	Rencana Anggaran Belanja** {#5.3-rencana-anggaran-belanja}

  ### **5.3.1	Aset yang Menggunakan Pinjaman Lab / Tahun Sebelumnya** {#5.3.1-aset-yang-menggunakan-pinjaman-lab-/-tahun-sebelumnya}

  &nbsp;

| No | Item | Spesifikasi | Jumlah | Keterangan |
| :---- | :---- | :---- | :---- | :---- |
| 1 | ESP32 Dev Board | Mikrokontroler utama | 1 | Pinjaman lab |
| 2 | Kabel jumper | Male-male & male-female | 1 set | Pinjaman lab |
| 3 | Resistor | 220Ω, 1kΩ, dll | 1 set | Pinjaman lab |
| 4 | Kapasitor | Elektrolit & keramik | 1 set | Pinjaman lab |
| 5 | Power supply module | 5V/3.3V regulator | 1 | Pinjaman lab |
| 6 | Multimeter | Pengukuran rangkaian | 1 | Pinjaman lab |

  &nbsp;

  ### **5.3.2	Bahan Habis Pakai (Perlu Dibeli Baru)** {#5.3.2-bahan-habis-pakai-(perlu-dibeli-baru)}

  &nbsp;

| No | Item | Spesifikasi | Jumlah | Harga Satuan (Rp) | Total (Rp) |
| ----- | ----- | ----- | ----- | ----- | ----- |
| 1 | Breadboard | 830 tie points | 1 | 30.000 | 30.000 |
| 2 | Timah solder | 0.8 mm | 1 | 20.000 | 20.000 |
| 3 | Flux solder | Mempermudah penyolderan | 1 | 15.000 | 15.000 |
| 4 | Kabel kecil | Untuk wiring internal | 1 pack | 20.000 | 20.000 |
| 5 | Heat shrink | Pelindung kabel | 1 pack | 15.000 | 15.000 |
| 6 | Isolasi listrik | Finishing kabel | 1 | 10.000 | 10.000 |
| 7 | Baut & mur kecil | Perakitan casing | 1 set | 15.000 | 15.000 |

  &nbsp;

  ### **5.3.3	Perangkat dan Komponen Elektronik (Perlu Dibeli Baru)** {#5.3.3-perangkat-dan-komponen-elektronik-(perlu-dibeli-baru)}

  &nbsp;

| No | Item | Spesifikasi | Jumlah | Harga Satuan (Rp) | Total (Rp) |
| ----- | ----- | ----- | ----- | ----- | ----- |
| 1 | ESP32-CAM | Modul kamera AI | 1 | 70.000 | 70.000 |
| 2 | Modul LoRa (SX1278) | Komunikasi jarak jauh | 1 | 120.000 | 120.000 |
| 3 | Antena LoRa | Long range antenna | 1 | 30.000 | 30.000 |
| 4 | LED Ring | Pencahayaan kamera | 1 | 25.000 | 25.000 |
| 5 | Baterai Li-ion 18650 | 3.7V rechargeable | 1 | 40.000 | 40.000 |
| 6 | Modul charger TP4056 | Charging baterai | 1 | 10.000 | 10.000 |
| 7 | Step-up converter (MT3608) | Boost voltage | 1 | 15.000 | 15.000 |
| 8 | Push button | Power switch | 1 | 5.000 | 5.000 |
| 9 | OLED Display (opsional) | Tampilan data | 1 | 40.000 | 40.000 |
| 10 | Buzzer kecil | Feedback suara | 1 | 5.000 | 5.000 |
| 11 | Casing (3D print / akrilik) | Body alat | 1 | 50.000 | 50.000 |

&nbsp;

# **REFERENSI**&nbsp; {#referensi}

\[1\] C. Berry et al., "Prehospital hemorrhage control and treatment by clinicians: A joint position statement," Prehospital Emergency Care, vol. 27, no. 5, pp. 544-551, 2023\.

\[2\] R. K. Latif et al., "Traumatic hemorrhage and chain of survival," Scandinavian Journal of Trauma, Resuscitation and Emergency Medicine, vol. 31, no. 1, p. 25, May 2023\.

\[3\] H. Herbst and D. Willis, "Under Pressure: AutoTQ is Designed for Intuitive, Rapid Hemorrhage Control," IEEE Pulse, pp. 97-99, Nov./Dec. 2025\. doi: 10.1109/MPULS.2025.3640853.

\[4\] J. A. McEwen and V. Casey, "Measurement of hazardous pressure levels and gradients produced on human limbs by non-pneumatic tourniquets," in Proc. 28th Annu. Int. Conf. IEEE Eng. Med. Biol. Soc., May 2006, pp. 510-513.

\[5\] S. J. Vega, S. I. Hernandez-Torres, D. Berard, E. N. Boice, and E. J. Snider, "Development and Characterization of a Self-Tightening Tourniquet System," Sensors, vol. 22, no. 3, p. 1122, Feb. 2022\. doi: 10.3390/s22031122.

\[6\] E. J. Snider, S. J. Vega, E. Ross, D. Berard, S. I. Hernandez-Torres, J. Salinas, and E. N. Boice, "Supervisory Algorithm for Autonomous Hemodynamic Management Systems," Sensors, vol. 22, no. 2, p. 529, Jan. 2022\. doi: 10.3390/s22020529.

\[7\] J. A. McEwen, B. A. Masri, B. Day, and A. S. Younger, "Development of Personalized Tourniquet Systems Using a New Technique for Measuring Limb Occlusion Pressure," in Proc. World Congress on Medical Physics and Biomedical Engineering, Toronto, ON, Canada, 2015, pp. 1325-1328.

\[8\] B. A. Masri, A. Eisen, C. P. Duncan, and J. A. McEwen, "Tourniquet-Induced Nerve Compression Injuries Are Caused by High Pressure Levels and Gradients \- A Review of the Evidence to Guide Safe Surgical, Pre-Hospital and Blood Flow Restriction Usage," BMC Biomedical Engineering, vol. 2, no. 1, p. 7, 2020\.

\[9\] B. Graham, M. J. Breault, J. A. McEwen, and R. W. McGraw, "Occlusion of Arterial Flow in the Extremities at Subsystolic Pressures through the Use of Wide Tourniquet Cuffs," Clinical Orthopaedics and Related Research, vol. 286, pp. 257-261, 1993\.

\[10\] M. J. Khodaei, N. Candelino, A. Mehrvarz, and N. Jalili, "Physiological Closed-Loop Control (PCLC) Systems: Review of a Modern Frontier in Automation," IEEE Access, vol. 8, pp. 23965-24005, 2020\.

\[11\] K. Nyberger, L. Strommer, and C.-M. Wahlgren, "A systematic review of hemorrhage and vascular injuries in civilian public mass shootings," Scandinavian Journal of Trauma, Resuscitation and Emergency Medicine, vol. 31, no. 1, p. 30, Jun. 2023\.

\[12\] K. Mistry et al., "Limb Occlusion Pressure and Its Determinants in Healthy Adults: A Systematic Review," Journal of Clinical Monitoring and Computing, 2023\.

\[13\] E. M. Ross et al., "The tourniquet gap: A pilot study of the intuitive placement of three tourniquet ty

\[14\] M. J. Jacobson, M. El Masry, D. C. Arrubla, M. R. Tricas, S. C. Gnyawali, X. Zhang, G. Gordillo, Y. Xue, C. K. Sen, and J. Wachs, “Autonomous Multi-modality Burn Wound Characterization using Artificial Intelligence,” *Military Medicine*, vol. 188, no. Supplement\_6, pp. 674–681, Nov./Dec. 2023, doi: 10.1093/milmed/usad301.

\[15\] A. Perry, S. Dodd, H. O. Chan, R. Joshi, J. N. Wong, and C. Hong, “606 Artificial Intelligence-powered Mobile Tool for Burn Injury Evaluation for First Responders,” *Journal of Burn Care & Research*, vol. 45, suppl. 1, pp. S188–S189, 2024, doi: 10.1093/jbcr/irae036.240.

\[16\] S. Heydari and Q. H. Mahmoud, “Tiny Machine Learning and On-Device Inference: A Survey of Applications, Challenges, and Future Directions,” *Sensors*, vol. 25, no. 10, p. 3191, 2025, doi: 10.3390/s25103191.

\[17\] D. W. Carter and D. A. Spain, “Burns,” *MSD Manual Professional Edition*, Sept. 2024\. \[Online\]. Available: [https://www.msdmanuals.com/professional/injuries-poisoning/burns/burns](https://www.msdmanuals.com/professional/injuries-poisoning/burns/burns?utm_source=chatgpt.com). \[Accessed: Apr. 1, 2026\].

\[18\] World Health Organization, “Burns,” *WHO Fact Sheets*, Oct. 13, 2023\. \[Online\]. Available: [https://www.who.int/news-room/fact-sheets/detail/burns](https://www.who.int/news-room/fact-sheets/detail/burns?utm_source=chatgpt.com). \[Accessed: Apr. 1, 2026\].

\[19\] World Health Organization, *Facility-Based Mass Casualty Triage: Guidance Note*. \[Online\]. Available: https://cdn.who.int/media/docs/default-source/integrated-health-services-%28ihs%29/csy/mcm-triage-guidance-note.pdf. \[Accessed: Apr. 1, 2026\].

\[20\] D. Romanelli and M. W. Farrell, “AVPU Scale,” in *StatPearls* \[Internet\]. Treasure Island, FL, USA: StatPearls Publishing, 2026\. \[Online\]. Available: [https://www.ncbi.nlm.nih.gov/books/NBK538431/](https://www.ncbi.nlm.nih.gov/books/NBK538431/?utm_source=chatgpt.com). \[Accessed: Apr. 1, 2026\].

\[21\] Spectral AI, “Spectral AI Announces Successful Results from its Burn Validation Study of the DeepView System,” Mar. 17, 2025\. \[Online\]. Available: [https://investors.spectral-ai.com/news-releases/news-release-details/spectral-ai-announces-successful-results-its-burn-validation](https://investors.spectral-ai.com/news-releases/news-release-details/spectral-ai-announces-successful-results-its-burn-validation?utm_source=chatgpt.com). \[Accessed: Apr. 1, 2026\].

&nbsp;

[image1]: <data:image/png;base64,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>