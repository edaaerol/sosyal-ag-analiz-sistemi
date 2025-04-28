#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KULLANICI 1000 // Maksimum kullan�c� say�s�
#define MAX_ARCADAS 100    // Bir kullan�c�n�n maksimum arkada� say�s�

#define KIRMIZI 0          // Red-Black Tree'de k�rm�z� renk de�eri
#define SIYAH 1            // Red-Black Tree'de siyah renk de�eri

// Kullan�c�y� temsil eden yap�
struct Kullanici {
    int id;                // Kullan�c� ID'si
    int arkadasSayisi;     // Kullan�c�n�n arkada� say�s�
    int arkadaslar[MAX_ARCADAS]; // Kullan�c�n�n arkada�lar�n�n ID'leri
};

// Red-Black Tree d���m yap�s�
struct RBTDugum {
    struct Kullanici* kullanici;    // D���mdeki kullan�c�
    int renk;                       // D���m�n rengi (KIRMIZI veya SIYAH)
    struct RBTDugum* sol;           // Sol alt d���m
    struct RBTDugum* sag;           // Sa� alt d���m
    struct RBTDugum* ebeveyn;       // �st d���m (ebeveyn)
};

RBTDugum* kok = NULL;  // Red-Black Tree'nin k�k d���m�

// Kullan�c�lar�n bilgilerini tutan dizi
struct Kullanici kullanicilar[MAX_KULLANICI];
int toplamKullanici = 0;  // Toplam kullan�c� say�s�

// Fonksiyon Prototipleri
struct Kullanici* kullaniciGetir(int id);
void arkadasEkle(int id1, int id2);
void veriDosyasiniOku(const char* dosyaAdi);
void dfsAra(int baslangicID, int hedefDerinlik, int mevcutDerinlik, int ziyaretEdildi[], int sonuc[], int* sayac);
void arkadaslariBul(int kullaniciID);
void topluluklariBul();
void ortakArkadaslariBul(int id1, int id2);

// Red-Black Tree Fonksiyonlar�
RBTDugum* rbtEkle(RBTDugum* kok, struct Kullanici* kullanici);
RBTDugum* yeniRBTDugumu(struct Kullanici* kullanici);
RBTDugum* solaDondur(RBTDugum* kok, RBTDugum* x);
RBTDugum* sagaDondur(RBTDugum* kok, RBTDugum* y);
RBTDugum* duzeltEkleme(RBTDugum* kok, RBTDugum* z);
void siraliListele(RBTDugum* kok);

// Kullan�c�y� ID'ye g�re getirir, yoksa yeni olusturur
struct Kullanici* kullaniciGetir(int id) {
    for (int i = 0; i < toplamKullanici; i++) {
        if (kullanicilar[i].id == id) return &kullanicilar[i];  // Kullan�c� bulundu
    }
    // Kullan�c� bulunamad�ysa yeni kullan�c� ekle
    kullanicilar[toplamKullanici].id = id;
    kullanicilar[toplamKullanici].arkadasSayisi = 0;
    return &kullanicilar[toplamKullanici++];
}

// Arkada�l�k ili�kisi ekler
void arkadasEkle(int id1, int id2) {
    struct Kullanici* k1 = kullaniciGetir(id1);  // id1 i�in kullan�c�y� getir
    struct Kullanici* k2 = kullaniciGetir(id2);  // id2 i�in kullan�c�y� getir
    k1->arkadaslar[k1->arkadasSayisi++] = id2;   // id2'yi id1'in arkada�lar� listesine ekle
    k2->arkadaslar[k2->arkadasSayisi++] = id1;   // id1'i id2'nin arkada�lar� listesine ekle
}

// Dosyadaki verileri okur ve kullan�c�lar� ekler
void veriDosyasiniOku(const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "r");  // Dosyay� a�
    if (!dosya) {
        printf("Dosya acilamadi.\n");
        exit(1);  // Dosya a��lamazsa programdan ��k
    }
    char satir[100];
    while (fgets(satir, sizeof(satir), dosya)) {
        if (strncmp(satir, "USER", 4) == 0) {
            int id;
            sscanf(satir, "USER %d", &id);  // USER sat�r�ndaki ID'yi oku
            struct Kullanici* yeniKullanici = kullaniciGetir(id);
            kok = rbtEkle(kok, yeniKullanici);  // Yeni kullan�c�y� Red-Black Tree'ye ekle
        }
        else if (strncmp(satir, "FRIEND", 6) == 0) {
            int id1, id2;
            sscanf(satir, "FRIEND %d %d", &id1, &id2);  // FRIEND sat�r�ndaki iki ID'yi oku
            arkadasEkle(id1, id2);  // �ki kullan�c�y� arkada� yap
        }
    }
    fclose(dosya);  // Dosyay� kapat
}

// Derinlik �ncelikli arama (DFS) algoritmas�
void dfsAra(int baslangicID, int hedefDerinlik, int mevcutDerinlik, int ziyaretEdildi[], int sonuc[], int* sayac) {
    ziyaretEdildi[baslangicID] = 1;  // Mevcut kullan�c�y� ziyaret ettik
    if (mevcutDerinlik == hedefDerinlik) {
        sonuc[(*sayac)++] = baslangicID;  // �lgili derinlikteki kullan�c�y� sonu�lara ekle
        return;
    }
    struct Kullanici* kullanici = kullaniciGetir(baslangicID);  // Kullan�c�y� getir
    for (int i = 0; i < kullanici->arkadasSayisi; i++) {
        int arkadasID = kullanici->arkadaslar[i];  // Arkada� ID'sini al
        if (!ziyaretEdildi[arkadasID]) {
            dfsAra(arkadasID, hedefDerinlik, mevcutDerinlik + 1, ziyaretEdildi, sonuc, sayac);  // Rek�rsif olarak arkada�lar� ara
        }
    }
}

// Kullan�c�n�n 1. ve 2. derece arkada�lar�n� bulur
void arkadaslariBul(int kullaniciID) {
    int ziyaretEdildi[MAX_KULLANICI] = { 0 };  // Ziyaret edilmeyen kullan�c�lar� i�aretle
    int birinciSeviye[MAX_KULLANICI];  // Birinci derece arkada�lar
    int ikinciSeviye[MAX_KULLANICI];  // �kinci derece arkada�lar
    int sayac1 = 0, sayac2 = 0;  // Arkada� sayac�

    // 1. derece arkada�lar� bul
    dfsAra(kullaniciID, 1, 0, ziyaretEdildi, birinciSeviye, &sayac1);

    // 2. derece arkada�lar� bul
    memset(ziyaretEdildi, 0, sizeof(ziyaretEdildi));  // Ziyaret edilmeyenleri s�f�rla
    dfsAra(kullaniciID, 2, 0, ziyaretEdildi, ikinciSeviye, &sayac2);

    // 1. derece arkada�lar� yazd�r
    printf("Birinci seviye arkadaslar: ");
    for (int i = 0; i < sayac1; i++)
        if (birinciSeviye[i] != kullaniciID) printf("%d ", birinciSeviye[i]);
    printf("\n");

    // 2. derece arkada�lar� yazd�r
    printf("Ikinci seviye arkadaslar: ");
    for (int i = 0; i < sayac2; i++) {
        int id = ikinciSeviye[i];
        int atla = 0;
        // Ortak arkada�lar� atlama
        for (int j = 0; j < sayac1; j++)
            if (birinciSeviye[j] == id) atla = 1;
        if (!atla && id != kullaniciID)
            printf("%d ", id);
    }
    printf("\nEtki Alani: %d\n", sayac1 + sayac2);  // Etki alan�: 1. ve 2. derece arkada�lar�n toplam�
}

// �ki kullan�c�n�n ortak arkada�lar�n� bulur
void ortakArkadaslariBul(int id1, int id2) {
    struct Kullanici* k1 = kullaniciGetir(id1);
    struct Kullanici* k2 = kullaniciGetir(id2);
    printf("%d ve %d numarali kullanicilarin ortak arkadaslari: ", id1, id2);
    for (int i = 0; i < k1->arkadasSayisi; i++) {
        for (int j = 0; j < k2->arkadasSayisi; j++) {
            if (k1->arkadaslar[i] == k2->arkadaslar[j]) {  // Ortak arkada� bulunduysa
                printf("%d ", k1->arkadaslar[i]);
            }
        }
    }
    printf("\n");
}

// Kullan�c�lar� topluluklar halinde bulur
void topluluklariBul() {
    int ziyaretEdildi[MAX_KULLANICI] = { 0 };
    printf("Topluluklar:\n");
    for (int i = 0; i < toplamKullanici; i++) {
        int id = kullanicilar[i].id;
        if (!ziyaretEdildi[id]) {  // Ziyaret edilmemi�se
            int grup[MAX_KULLANICI];
            int grupSayisi = 0;
            dfsAra(id, MAX_KULLANICI, 0, ziyaretEdildi, grup, &grupSayisi);  // DFS ba�lat
            printf("Topluluk: ");
            for (int j = 0; j < grupSayisi; j++) {
                printf("%d ", grup[j]);  // Topluluk �yelerini yazd�r
            }
            printf("\n");
        }
    }
}

// Red-Black Tree'de yeni d���m olu�turur
RBTDugum* yeniRBTDugumu(struct Kullanici* kullanici) {
    RBTDugum* yeni = (RBTDugum*)malloc(sizeof(RBTDugum));  // Yeni d���m i�in bellek ay�r
    yeni->kullanici = kullanici;
    yeni->renk = KIRMIZI;  // Yeni d���m ba�lang��ta k�rm�z�
    yeni->sol = yeni->sag = yeni->ebeveyn = NULL;  // D���m�n ba�lant�lar�n� s�f�rla
    return yeni;
}

// Red-Black Tree'de sola d�n�� i�lemi yapar
RBTDugum* solaDondur(RBTDugum* kok, RBTDugum* x) {
    RBTDugum* y = x->sag;  // x'in sa� �ocu�unu y'ye ata
    x->sag = y->sol;  // y'nin sol �ocu�unu x'in sa� �ocu�una ata
    if (y->sol) y->sol->ebeveyn = x;  // y'nin sol �ocu�unun ebeveynini g�ncelle
    y->ebeveyn = x->ebeveyn;  // x'in ebeveynini y'ye ata
    if (!x->ebeveyn) kok = y;  // E�er x k�kse, k�k� y olarak g�ncelle
    else if (x == x->ebeveyn->sol) x->ebeveyn->sol = y;  // x'in ebeveyni x'i sol �ocuk olarak tutuyorsa
    else x->ebeveyn->sag = y;  // x sa� �ocuksa, ebeveynin sa� �ocu�unu g�ncelle
    y->sol = x;  // x'i y'nin sol �ocu�u yap
    x->ebeveyn = y;  // x'in ebeveynini y olarak ata
    return kok;
}

// Red-Black Tree'de sa�a d�n�� i�lemi yapar
RBTDugum* sagaDondur(RBTDugum* kok, RBTDugum* y) {
    RBTDugum* x = y->sol;  // y'nin sol �ocu�unu x'e ata
    y->sol = x->sag;  // x'in sa� �ocu�unu y'nin sol �ocu�una ata
    if (x->sag) x->sag->ebeveyn = y;  // x'in sa� �ocu�unun ebeveynini y olarak g�ncelle
    x->ebeveyn = y->ebeveyn;  // y'nin ebeveynini x'e ata
    if (!y->ebeveyn) kok = x;  // E�er y k�kse, k�k� x olarak g�ncelle
    else if (y == y->ebeveyn->sol) y->ebeveyn->sol = x;  // y sol �ocuksa
    else y->ebeveyn->sag = x;  // y sa� �ocuksa
    x->sag = y;  // y'yi x'in sa� �ocu�u yap
    y->ebeveyn = x;  // y'nin ebeveynini x olarak ata
    return kok;
}

// Red-Black Tree'ye eleman ekler
RBTDugum* rbtEkle(RBTDugum* kok, struct Kullanici* kullanici) {
    RBTDugum* yeni = yeniRBTDugumu(kullanici);  // Yeni d���m olu�tur
    if (!kok) return yeni;  // E�er a�ac�n k�k� bo�sa, yeni d���m� k�k yap
    RBTDugum* mevcut = kok;
    RBTDugum* ebeveyn = NULL;
    while (mevcut) {
        ebeveyn = mevcut;
        if (kullanici->id < mevcut->kullanici->id) mevcut = mevcut->sol;  // ID k���kse sol alt d���me git
        else mevcut = mevcut->sag;  // ID b�y�kse sa� alt d���me git
    }
    yeni->ebeveyn = ebeveyn;
    if (kullanici->id < ebeveyn->kullanici->id) ebeveyn->sol = yeni;
    else ebeveyn->sag = yeni;
    kok = duzeltEkleme(kok, yeni);  // Red-Black Tree dengeleme i�lemi
    return kok;
}

// Red-Black Tree'de ekleme sonras� dengeleme i�lemi
RBTDugum* duzeltEkleme(RBTDugum* kok, RBTDugum* z) {
    while (z != kok && z->ebeveyn->renk == KIRMIZI) {  // E�er ebeveyn k�rm�z�ysa d�zeltme yap
        if (z->ebeveyn == z->ebeveyn->ebeveyn->sol) {
            RBTDugum* y = z->ebeveyn->ebeveyn->sag;
            if (y && y->renk == KIRMIZI) {  // E�er amca k�rm�z�ysa
                z->ebeveyn->renk = SIYAH;  // Ebeveyn ve amca siyah olur
                y->renk = SIYAH;
                z->ebeveyn->ebeveyn->renk = KIRMIZI;  // B�y�k ebeveyn k�rm�z� olur
                z = z->ebeveyn->ebeveyn;  // B�y�k ebeveyn �zerinde i�lem yap
            }
            else {
                if (z == z->ebeveyn->sag) {
                    z = z->ebeveyn;  // z sol alt a�a�ta ise ebeveyn �zerinde i�lem yap
                    kok = solaDondur(kok, z);  // Ebeveyn �zerinde sola d�n�� yap
                }
                z->ebeveyn->renk = SIYAH;  // Ebeveyn siyah olur
                z->ebeveyn->ebeveyn->renk = KIRMIZI;  // B�y�k ebeveyn k�rm�z� olur
                kok = sagaDondur(kok, z->ebeveyn->ebeveyn);  // B�y�k ebeveyn �zerinde sa�a d�n�� yap
            }
        }
        else {
            RBTDugum* y = z->ebeveyn->ebeveyn->sol;
            if (y && y->renk == KIRMIZI) {  // E�er amca k�rm�z�ysa
                z->ebeveyn->renk = SIYAH;  // Ebeveyn ve amca siyah olur
                y->renk = SIYAH;
                z->ebeveyn->ebeveyn->renk = KIRMIZI;  // B�y�k ebeveyn k�rm�z� olur
                z = z->ebeveyn->ebeveyn;  // B�y�k ebeveyn �zerinde i�lem yap
            }
            else {
                if (z == z->ebeveyn->sol) {
                    z = z->ebeveyn;  // z sa� alt a�a�ta ise ebeveyn �zerinde i�lem yap
                    kok = sagaDondur(kok, z);  // Ebeveyn �zerinde sa�a d�n�� yap
                }
                z->ebeveyn->renk = SIYAH;  // Ebeveyn siyah olur
                z->ebeveyn->ebeveyn->renk = KIRMIZI;  // B�y�k ebeveyn k�rm�z� olur
                kok = solaDondur(kok, z->ebeveyn->ebeveyn);  // B�y�k ebeveyn �zerinde sola d�n�� yap
            }
        }
    }
    kok->renk = SIYAH;  // K�k her zaman siyah olur
    return kok;
}

// Red-Black Tree'yi s�ral� �ekilde yazd�r�r
void siraliListele(RBTDugum* kok) {
    if (kok != NULL) {
        siraliListele(kok->sol);  // �nce sol alt a�ac� yazd�r
        printf("%d ", kok->kullanici->id);  // Kullan�c� ID'sini yazd�r
        siraliListele(kok->sag);  // Sonra sa� alt a�ac� yazd�r
    }
}
