#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KULLANICI 1000 // Maksimum kullanýcý sayýsý
#define MAX_ARCADAS 100    // Bir kullanýcýnýn maksimum arkadaþ sayýsý

#define KIRMIZI 0          // Red-Black Tree'de kýrmýzý renk deðeri
#define SIYAH 1            // Red-Black Tree'de siyah renk deðeri

// Kullanýcýyý temsil eden yapý
struct Kullanici {
    int id;                // Kullanýcý ID'si
    int arkadasSayisi;     // Kullanýcýnýn arkadaþ sayýsý
    int arkadaslar[MAX_ARCADAS]; // Kullanýcýnýn arkadaþlarýnýn ID'leri
};

// Red-Black Tree düðüm yapýsý
struct RBTDugum {
    struct Kullanici* kullanici;    // Düðümdeki kullanýcý
    int renk;                       // Düðümün rengi (KIRMIZI veya SIYAH)
    struct RBTDugum* sol;           // Sol alt düðüm
    struct RBTDugum* sag;           // Sað alt düðüm
    struct RBTDugum* ebeveyn;       // Üst düðüm (ebeveyn)
};

RBTDugum* kok = NULL;  // Red-Black Tree'nin kök düðümü

// Kullanýcýlarýn bilgilerini tutan dizi
struct Kullanici kullanicilar[MAX_KULLANICI];
int toplamKullanici = 0;  // Toplam kullanýcý sayýsý

// Fonksiyon Prototipleri
struct Kullanici* kullaniciGetir(int id);
void arkadasEkle(int id1, int id2);
void veriDosyasiniOku(const char* dosyaAdi);
void dfsAra(int baslangicID, int hedefDerinlik, int mevcutDerinlik, int ziyaretEdildi[], int sonuc[], int* sayac);
void arkadaslariBul(int kullaniciID);
void topluluklariBul();
void ortakArkadaslariBul(int id1, int id2);

// Red-Black Tree Fonksiyonlarý
RBTDugum* rbtEkle(RBTDugum* kok, struct Kullanici* kullanici);
RBTDugum* yeniRBTDugumu(struct Kullanici* kullanici);
RBTDugum* solaDondur(RBTDugum* kok, RBTDugum* x);
RBTDugum* sagaDondur(RBTDugum* kok, RBTDugum* y);
RBTDugum* duzeltEkleme(RBTDugum* kok, RBTDugum* z);
void siraliListele(RBTDugum* kok);

// Kullanýcýyý ID'ye göre getirir, yoksa yeni olusturur
struct Kullanici* kullaniciGetir(int id) {
    for (int i = 0; i < toplamKullanici; i++) {
        if (kullanicilar[i].id == id) return &kullanicilar[i];  // Kullanýcý bulundu
    }
    // Kullanýcý bulunamadýysa yeni kullanýcý ekle
    kullanicilar[toplamKullanici].id = id;
    kullanicilar[toplamKullanici].arkadasSayisi = 0;
    return &kullanicilar[toplamKullanici++];
}

// Arkadaþlýk iliþkisi ekler
void arkadasEkle(int id1, int id2) {
    struct Kullanici* k1 = kullaniciGetir(id1);  // id1 için kullanýcýyý getir
    struct Kullanici* k2 = kullaniciGetir(id2);  // id2 için kullanýcýyý getir
    k1->arkadaslar[k1->arkadasSayisi++] = id2;   // id2'yi id1'in arkadaþlarý listesine ekle
    k2->arkadaslar[k2->arkadasSayisi++] = id1;   // id1'i id2'nin arkadaþlarý listesine ekle
}

// Dosyadaki verileri okur ve kullanýcýlarý ekler
void veriDosyasiniOku(const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "r");  // Dosyayý aç
    if (!dosya) {
        printf("Dosya acilamadi.\n");
        exit(1);  // Dosya açýlamazsa programdan çýk
    }
    char satir[100];
    while (fgets(satir, sizeof(satir), dosya)) {
        if (strncmp(satir, "USER", 4) == 0) {
            int id;
            sscanf(satir, "USER %d", &id);  // USER satýrýndaki ID'yi oku
            struct Kullanici* yeniKullanici = kullaniciGetir(id);
            kok = rbtEkle(kok, yeniKullanici);  // Yeni kullanýcýyý Red-Black Tree'ye ekle
        }
        else if (strncmp(satir, "FRIEND", 6) == 0) {
            int id1, id2;
            sscanf(satir, "FRIEND %d %d", &id1, &id2);  // FRIEND satýrýndaki iki ID'yi oku
            arkadasEkle(id1, id2);  // Ýki kullanýcýyý arkadaþ yap
        }
    }
    fclose(dosya);  // Dosyayý kapat
}

// Derinlik öncelikli arama (DFS) algoritmasý
void dfsAra(int baslangicID, int hedefDerinlik, int mevcutDerinlik, int ziyaretEdildi[], int sonuc[], int* sayac) {
    ziyaretEdildi[baslangicID] = 1;  // Mevcut kullanýcýyý ziyaret ettik
    if (mevcutDerinlik == hedefDerinlik) {
        sonuc[(*sayac)++] = baslangicID;  // Ýlgili derinlikteki kullanýcýyý sonuçlara ekle
        return;
    }
    struct Kullanici* kullanici = kullaniciGetir(baslangicID);  // Kullanýcýyý getir
    for (int i = 0; i < kullanici->arkadasSayisi; i++) {
        int arkadasID = kullanici->arkadaslar[i];  // Arkadaþ ID'sini al
        if (!ziyaretEdildi[arkadasID]) {
            dfsAra(arkadasID, hedefDerinlik, mevcutDerinlik + 1, ziyaretEdildi, sonuc, sayac);  // Rekürsif olarak arkadaþlarý ara
        }
    }
}

// Kullanýcýnýn 1. ve 2. derece arkadaþlarýný bulur
void arkadaslariBul(int kullaniciID) {
    int ziyaretEdildi[MAX_KULLANICI] = { 0 };  // Ziyaret edilmeyen kullanýcýlarý iþaretle
    int birinciSeviye[MAX_KULLANICI];  // Birinci derece arkadaþlar
    int ikinciSeviye[MAX_KULLANICI];  // Ýkinci derece arkadaþlar
    int sayac1 = 0, sayac2 = 0;  // Arkadaþ sayacý

    // 1. derece arkadaþlarý bul
    dfsAra(kullaniciID, 1, 0, ziyaretEdildi, birinciSeviye, &sayac1);

    // 2. derece arkadaþlarý bul
    memset(ziyaretEdildi, 0, sizeof(ziyaretEdildi));  // Ziyaret edilmeyenleri sýfýrla
    dfsAra(kullaniciID, 2, 0, ziyaretEdildi, ikinciSeviye, &sayac2);

    // 1. derece arkadaþlarý yazdýr
    printf("Birinci seviye arkadaslar: ");
    for (int i = 0; i < sayac1; i++)
        if (birinciSeviye[i] != kullaniciID) printf("%d ", birinciSeviye[i]);
    printf("\n");

    // 2. derece arkadaþlarý yazdýr
    printf("Ikinci seviye arkadaslar: ");
    for (int i = 0; i < sayac2; i++) {
        int id = ikinciSeviye[i];
        int atla = 0;
        // Ortak arkadaþlarý atlama
        for (int j = 0; j < sayac1; j++)
            if (birinciSeviye[j] == id) atla = 1;
        if (!atla && id != kullaniciID)
            printf("%d ", id);
    }
    printf("\nEtki Alani: %d\n", sayac1 + sayac2);  // Etki alaný: 1. ve 2. derece arkadaþlarýn toplamý
}

// Ýki kullanýcýnýn ortak arkadaþlarýný bulur
void ortakArkadaslariBul(int id1, int id2) {
    struct Kullanici* k1 = kullaniciGetir(id1);
    struct Kullanici* k2 = kullaniciGetir(id2);
    printf("%d ve %d numarali kullanicilarin ortak arkadaslari: ", id1, id2);
    for (int i = 0; i < k1->arkadasSayisi; i++) {
        for (int j = 0; j < k2->arkadasSayisi; j++) {
            if (k1->arkadaslar[i] == k2->arkadaslar[j]) {  // Ortak arkadaþ bulunduysa
                printf("%d ", k1->arkadaslar[i]);
            }
        }
    }
    printf("\n");
}

// Kullanýcýlarý topluluklar halinde bulur
void topluluklariBul() {
    int ziyaretEdildi[MAX_KULLANICI] = { 0 };
    printf("Topluluklar:\n");
    for (int i = 0; i < toplamKullanici; i++) {
        int id = kullanicilar[i].id;
        if (!ziyaretEdildi[id]) {  // Ziyaret edilmemiþse
            int grup[MAX_KULLANICI];
            int grupSayisi = 0;
            dfsAra(id, MAX_KULLANICI, 0, ziyaretEdildi, grup, &grupSayisi);  // DFS baþlat
            printf("Topluluk: ");
            for (int j = 0; j < grupSayisi; j++) {
                printf("%d ", grup[j]);  // Topluluk üyelerini yazdýr
            }
            printf("\n");
        }
    }
}

// Red-Black Tree'de yeni düðüm oluþturur
RBTDugum* yeniRBTDugumu(struct Kullanici* kullanici) {
    RBTDugum* yeni = (RBTDugum*)malloc(sizeof(RBTDugum));  // Yeni düðüm için bellek ayýr
    yeni->kullanici = kullanici;
    yeni->renk = KIRMIZI;  // Yeni düðüm baþlangýçta kýrmýzý
    yeni->sol = yeni->sag = yeni->ebeveyn = NULL;  // Düðümün baðlantýlarýný sýfýrla
    return yeni;
}

// Red-Black Tree'de sola dönüþ iþlemi yapar
RBTDugum* solaDondur(RBTDugum* kok, RBTDugum* x) {
    RBTDugum* y = x->sag;  // x'in sað çocuðunu y'ye ata
    x->sag = y->sol;  // y'nin sol çocuðunu x'in sað çocuðuna ata
    if (y->sol) y->sol->ebeveyn = x;  // y'nin sol çocuðunun ebeveynini güncelle
    y->ebeveyn = x->ebeveyn;  // x'in ebeveynini y'ye ata
    if (!x->ebeveyn) kok = y;  // Eðer x kökse, kökü y olarak güncelle
    else if (x == x->ebeveyn->sol) x->ebeveyn->sol = y;  // x'in ebeveyni x'i sol çocuk olarak tutuyorsa
    else x->ebeveyn->sag = y;  // x sað çocuksa, ebeveynin sað çocuðunu güncelle
    y->sol = x;  // x'i y'nin sol çocuðu yap
    x->ebeveyn = y;  // x'in ebeveynini y olarak ata
    return kok;
}

// Red-Black Tree'de saða dönüþ iþlemi yapar
RBTDugum* sagaDondur(RBTDugum* kok, RBTDugum* y) {
    RBTDugum* x = y->sol;  // y'nin sol çocuðunu x'e ata
    y->sol = x->sag;  // x'in sað çocuðunu y'nin sol çocuðuna ata
    if (x->sag) x->sag->ebeveyn = y;  // x'in sað çocuðunun ebeveynini y olarak güncelle
    x->ebeveyn = y->ebeveyn;  // y'nin ebeveynini x'e ata
    if (!y->ebeveyn) kok = x;  // Eðer y kökse, kökü x olarak güncelle
    else if (y == y->ebeveyn->sol) y->ebeveyn->sol = x;  // y sol çocuksa
    else y->ebeveyn->sag = x;  // y sað çocuksa
    x->sag = y;  // y'yi x'in sað çocuðu yap
    y->ebeveyn = x;  // y'nin ebeveynini x olarak ata
    return kok;
}

// Red-Black Tree'ye eleman ekler
RBTDugum* rbtEkle(RBTDugum* kok, struct Kullanici* kullanici) {
    RBTDugum* yeni = yeniRBTDugumu(kullanici);  // Yeni düðüm oluþtur
    if (!kok) return yeni;  // Eðer aðacýn kökü boþsa, yeni düðümü kök yap
    RBTDugum* mevcut = kok;
    RBTDugum* ebeveyn = NULL;
    while (mevcut) {
        ebeveyn = mevcut;
        if (kullanici->id < mevcut->kullanici->id) mevcut = mevcut->sol;  // ID küçükse sol alt düðüme git
        else mevcut = mevcut->sag;  // ID büyükse sað alt düðüme git
    }
    yeni->ebeveyn = ebeveyn;
    if (kullanici->id < ebeveyn->kullanici->id) ebeveyn->sol = yeni;
    else ebeveyn->sag = yeni;
    kok = duzeltEkleme(kok, yeni);  // Red-Black Tree dengeleme iþlemi
    return kok;
}

// Red-Black Tree'de ekleme sonrasý dengeleme iþlemi
RBTDugum* duzeltEkleme(RBTDugum* kok, RBTDugum* z) {
    while (z != kok && z->ebeveyn->renk == KIRMIZI) {  // Eðer ebeveyn kýrmýzýysa düzeltme yap
        if (z->ebeveyn == z->ebeveyn->ebeveyn->sol) {
            RBTDugum* y = z->ebeveyn->ebeveyn->sag;
            if (y && y->renk == KIRMIZI) {  // Eðer amca kýrmýzýysa
                z->ebeveyn->renk = SIYAH;  // Ebeveyn ve amca siyah olur
                y->renk = SIYAH;
                z->ebeveyn->ebeveyn->renk = KIRMIZI;  // Büyük ebeveyn kýrmýzý olur
                z = z->ebeveyn->ebeveyn;  // Büyük ebeveyn üzerinde iþlem yap
            }
            else {
                if (z == z->ebeveyn->sag) {
                    z = z->ebeveyn;  // z sol alt aðaçta ise ebeveyn üzerinde iþlem yap
                    kok = solaDondur(kok, z);  // Ebeveyn üzerinde sola dönüþ yap
                }
                z->ebeveyn->renk = SIYAH;  // Ebeveyn siyah olur
                z->ebeveyn->ebeveyn->renk = KIRMIZI;  // Büyük ebeveyn kýrmýzý olur
                kok = sagaDondur(kok, z->ebeveyn->ebeveyn);  // Büyük ebeveyn üzerinde saða dönüþ yap
            }
        }
        else {
            RBTDugum* y = z->ebeveyn->ebeveyn->sol;
            if (y && y->renk == KIRMIZI) {  // Eðer amca kýrmýzýysa
                z->ebeveyn->renk = SIYAH;  // Ebeveyn ve amca siyah olur
                y->renk = SIYAH;
                z->ebeveyn->ebeveyn->renk = KIRMIZI;  // Büyük ebeveyn kýrmýzý olur
                z = z->ebeveyn->ebeveyn;  // Büyük ebeveyn üzerinde iþlem yap
            }
            else {
                if (z == z->ebeveyn->sol) {
                    z = z->ebeveyn;  // z sað alt aðaçta ise ebeveyn üzerinde iþlem yap
                    kok = sagaDondur(kok, z);  // Ebeveyn üzerinde saða dönüþ yap
                }
                z->ebeveyn->renk = SIYAH;  // Ebeveyn siyah olur
                z->ebeveyn->ebeveyn->renk = KIRMIZI;  // Büyük ebeveyn kýrmýzý olur
                kok = solaDondur(kok, z->ebeveyn->ebeveyn);  // Büyük ebeveyn üzerinde sola dönüþ yap
            }
        }
    }
    kok->renk = SIYAH;  // Kök her zaman siyah olur
    return kok;
}

// Red-Black Tree'yi sýralý þekilde yazdýrýr
void siraliListele(RBTDugum* kok) {
    if (kok != NULL) {
        siraliListele(kok->sol);  // Önce sol alt aðacý yazdýr
        printf("%d ", kok->kullanici->id);  // Kullanýcý ID'sini yazdýr
        siraliListele(kok->sag);  // Sonra sað alt aðacý yazdýr
    }
}
