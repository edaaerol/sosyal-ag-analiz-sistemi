#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_KULLANICI 1000
#define MAX_ARCADAS 100
#define KIRMIZI 0
#define SIYAH 1

// Kullanıcıyı temsil eden yapı
struct Kullanici {
    int id;
    int arkadasSayisi;
    int arkadaslar[MAX_ARCADAS];
};

// Red-Black Tree düğüm yapısı
struct RBTDugum {
    struct Kullanici* kullanici;
    int renk;
    struct RBTDugum* sol;
    struct RBTDugum* sag;
    struct RBTDugum* ebeveyn;
};

struct RBTDugum* kok = NULL;
struct Kullanici kullanicilar[MAX_KULLANICI];
int toplamKullanici = 0;

// Kullanıcıyı ID'ye göre getirir, yoksa yeni oluşturur
struct Kullanici* kullaniciGetir(int id) {
    for (int i = 0; i < toplamKullanici; i++) {
        if (kullanicilar[i].id == id) return &kullanicilar[i];
    }
    kullanicilar[toplamKullanici].id = id;
    kullanicilar[toplamKullanici].arkadasSayisi = 0;
    return &kullanicilar[toplamKullanici++];
}

// Arkadaşlık ilişkisi ekler
void arkadasEkle(int id1, int id2) {
    struct Kullanici* k1 = kullaniciGetir(id1);
    struct Kullanici* k2 = kullaniciGetir(id2);
    k1->arkadaslar[k1->arkadasSayisi++] = id2;
    k2->arkadaslar[k2->arkadasSayisi++] = id1;
}

// Red-Black Tree'de yeni düğüm oluşturur
struct RBTDugum* yeniRBTDugumu(struct Kullanici* kullanici) {
    struct RBTDugum* yeni = (struct RBTDugum*)malloc(sizeof(struct RBTDugum));
    yeni->kullanici = kullanici;
    yeni->renk = KIRMIZI;
    yeni->sol = yeni->sag = yeni->ebeveyn = NULL;
    return yeni;
}

// Red-Black Tree'de sola dönüş işlemi yapar
struct RBTDugum* solaDondur(struct RBTDugum* kok, struct RBTDugum* x) {
    struct RBTDugum* y = x->sag;
    x->sag = y->sol;
    if (y->sol) y->sol->ebeveyn = x;
    y->ebeveyn = x->ebeveyn;
    if (!x->ebeveyn) kok = y;
    else if (x == x->ebeveyn->sol) x->ebeveyn->sol = y;
    else x->ebeveyn->sag = y;
    y->sol = x;
    x->ebeveyn = y;
    return kok;
}

// Red-Black Tree'de sağa dönüş işlemi yapar
struct RBTDugum* sagaDondur(struct RBTDugum* kok, struct RBTDugum* y) {
    struct RBTDugum* x = y->sol;
    y->sol = x->sag;
    if (x->sag) x->sag->ebeveyn = y;
    x->ebeveyn = y->ebeveyn;
    if (!y->ebeveyn) kok = x;
    else if (y == y->ebeveyn->sol) y->ebeveyn->sol = x;
    else y->ebeveyn->sag = x;
    x->sag = y;
    y->ebeveyn = x;
    return kok;
}

// Red-Black Tree'de ekleme sonrası dengeleme işlemi
struct RBTDugum* duzeltEkleme(struct RBTDugum* kok, struct RBTDugum* z) {
    while (z != kok && z->ebeveyn && z->ebeveyn->renk == KIRMIZI) {
        if (z->ebeveyn->ebeveyn && z->ebeveyn == z->ebeveyn->ebeveyn->sol) {
            struct RBTDugum* y = z->ebeveyn->ebeveyn->sag;
            if (y && y->renk == KIRMIZI) {
                z->ebeveyn->renk = SIYAH;
                y->renk = SIYAH;
                z->ebeveyn->ebeveyn->renk = KIRMIZI;
                z = z->ebeveyn->ebeveyn;
            }
            else {
                if (z == z->ebeveyn->sag) {
                    z = z->ebeveyn;
                    kok = solaDondur(kok, z);
                }
                if (z->ebeveyn) {
                    z->ebeveyn->renk = SIYAH;
                    if (z->ebeveyn->ebeveyn) {
                        z->ebeveyn->ebeveyn->renk = KIRMIZI;
                        kok = sagaDondur(kok, z->ebeveyn->ebeveyn);
                    }
                }
            }
        }
        else if (z->ebeveyn->ebeveyn) {
            struct RBTDugum* y = z->ebeveyn->ebeveyn->sol;
            if (y && y->renk == KIRMIZI) {
                z->ebeveyn->renk = SIYAH;
                y->renk = SIYAH;
                z->ebeveyn->ebeveyn->renk = KIRMIZI;
                z = z->ebeveyn->ebeveyn;
            }
            else {
                if (z == z->ebeveyn->sol) {
                    z = z->ebeveyn;
                    kok = sagaDondur(kok, z);
                }
                if (z->ebeveyn) {
                    z->ebeveyn->renk = SIYAH;
                    if (z->ebeveyn->ebeveyn) {
                        z->ebeveyn->ebeveyn->renk = KIRMIZI;
                        kok = solaDondur(kok, z->ebeveyn->ebeveyn);
                    }
                }
            }
        }
    }
    kok->renk = SIYAH;
    return kok;
}

// Red-Black Tree'ye eleman ekler
struct RBTDugum* rbtEkle(struct RBTDugum* kok, struct Kullanici* kullanici) {
    struct RBTDugum* yeni = yeniRBTDugumu(kullanici);
    if (!kok) {
        yeni->renk = SIYAH;
        return yeni;
    }

    struct RBTDugum* mevcut = kok;
    struct RBTDugum* ebeveyn = NULL;
    while (mevcut) {
        ebeveyn = mevcut;
        if (kullanici->id < mevcut->kullanici->id) mevcut = mevcut->sol;
        else mevcut = mevcut->sag;
    }

    yeni->ebeveyn = ebeveyn;
    if (kullanici->id < ebeveyn->kullanici->id) ebeveyn->sol = yeni;
    else ebeveyn->sag = yeni;

    return duzeltEkleme(kok, yeni);
}

// Red-Black Tree'yi sıralı şekilde yazdırır
void siraliListele(struct RBTDugum* kok) {
    if (kok != NULL) {
        siraliListele(kok->sol);
        printf("%d ", kok->kullanici->id);
        siraliListele(kok->sag);
    }
}

// Dosyadaki verileri okur ve kullanıcıları ekler
void veriDosyasiniOku(const char* dosyaAdi) {
    FILE* dosya = fopen(dosyaAdi, "r");
    if (!dosya) {
        printf("Dosya acilamadi.\n");
        return;
    }

    char satir[100];
    while (fgets(satir, sizeof(satir), dosya)) {
        if (strncmp(satir, "USER", 4) == 0) {
            int id;
            sscanf(satir, "USER %d", &id);
            struct Kullanici* yeniKullanici = kullaniciGetir(id);
            kok = rbtEkle(kok, yeniKullanici);
        }
        else if (strncmp(satir, "FRIEND", 6) == 0) {
            int id1, id2;
            sscanf(satir, "FRIEND %d %d", &id1, &id2);
            arkadasEkle(id1, id2);
        }
    }
    printf("Dosya basariyla okundu.\n");
    fclose(dosya);
}

// Derinlik öncelikli arama (DFS) algoritması
void dfsAra(int baslangicID, int hedefDerinlik, int mevcutDerinlik, int ziyaretEdildi[], int sonuc[], int* sayac) {
    ziyaretEdildi[baslangicID] = 1;

    if (mevcutDerinlik == hedefDerinlik) {
        sonuc[(*sayac)++] = baslangicID;
        return;
    }

    struct Kullanici* kullanici = kullaniciGetir(baslangicID);
    for (int i = 0; i < kullanici->arkadasSayisi; i++) {
        int arkadasID = kullanici->arkadaslar[i];
        if (!ziyaretEdildi[arkadasID]) {
            dfsAra(arkadasID, hedefDerinlik, mevcutDerinlik + 1, ziyaretEdildi, sonuc, sayac);
        }
    }
}

// Kullanıcının 1. ve 2. derece arkadaşlarını bulur
void arkadaslariBul(int kullaniciID) {
    int ziyaretEdildi[MAX_KULLANICI] = { 0 };
    int birinciSeviye[MAX_KULLANICI];
    int ikinciSeviye[MAX_KULLANICI];
    int sayac1 = 0, sayac2 = 0;

    // 1. derece arkadaşları bul
    dfsAra(kullaniciID, 1, 0, ziyaretEdildi, birinciSeviye, &sayac1);

    // 2. derece arkadaşları bul
    memset(ziyaretEdildi, 0, sizeof(ziyaretEdildi));
    dfsAra(kullaniciID, 2, 0, ziyaretEdildi, ikinciSeviye, &sayac2);

    // 1. derece arkadaşları yazdır
    printf("Birinci seviye arkadaslar: ");
    for (int i = 0; i < sayac1; i++)
        if (birinciSeviye[i] != kullaniciID) printf("%d ", birinciSeviye[i]);
    printf("\n");

    // 2. derece arkadaşları yazdır
    printf("Ikinci seviye arkadaslar: ");
    for (int i = 0; i < sayac2; i++) {
        int id = ikinciSeviye[i];
        int atla = 0;
        // Ortak arkadaşları atlama
        for (int j = 0; j < sayac1; j++)
            if (birinciSeviye[j] == id) atla = 1;
        if (!atla && id != kullaniciID)
            printf("%d ", id);
    }
    printf("\nEtki Alani: %d\n", sayac1 + sayac2);
}

// İki kullanıcının ortak arkadaşlarını bulur
void ortakArkadaslariBul(int id1, int id2) {
    struct Kullanici* k1 = kullaniciGetir(id1);
    struct Kullanici* k2 = kullaniciGetir(id2);
    printf("%d ve %d numarali kullanicilarin ortak arkadaslari: ", id1, id2);
    for (int i = 0; i < k1->arkadasSayisi; i++) {
        for (int j = 0; j < k2->arkadasSayisi; j++) {
            if (k1->arkadaslar[i] == k2->arkadaslar[j]) {
                printf("%d ", k1->arkadaslar[i]);
            }
        }
    }
    printf("\n");
}

// Kullanıcıları topluluklar halinde bulur
void topluluklariBul() {
    int ziyaretEdildi[MAX_KULLANICI] = { 0 };
    printf("Topluluklar:\n");
    for (int i = 0; i < toplamKullanici; i++) {
        int id = kullanicilar[i].id;
        if (!ziyaretEdildi[id]) {
            int grup[MAX_KULLANICI];
            int grupSayisi = 0;
            dfsAra(id, MAX_KULLANICI, 0, ziyaretEdildi, grup, &grupSayisi);
            printf("Topluluk: ");
            for (int j = 0; j < grupSayisi; j++) {
                printf("%d ", grup[j]);
            }
            printf("\n");
        }
    }
}

// Ana menü
void menuGoster() {
    printf("\n--- SOSYAL AG ANALIZ SISTEMI ---\n");
    printf("1. Veri Dosyasini Oku\n");
    printf("2. Arkadaslik Iliskisi Ekle\n");
    printf("3. Kullanici Arkadaslarini Goster\n");
    printf("4. Ortak Arkadaslari Bul\n");
    printf("5. Topluluklari Tespit Et\n");
    printf("6. Tum Kullanicilari Goster\n");
    printf("0. Cikis\n");
    printf("Seciminiz: ");
}

int main() {
    int secim, id1, id2;
    char dosyaAdi[100];

    do {
        menuGoster();
        scanf("%d", &secim);

        switch (secim) {
        case 0:
            printf("Programdan cikiliyor...\n");
            break;
        case 1:
            printf("Veri dosyasi adini girin: ");
            scanf("%s", dosyaAdi);
            veriDosyasiniOku(dosyaAdi);
            break;
        case 2:
            printf("Iki kullanici ID'sini girin: ");
            scanf("%d %d", &id1, &id2);
            arkadasEkle(id1, id2);
            printf("Arkadaslik iliskisi eklendi.\n");
            break;
        case 3:
            printf("Kullanici ID'sini girin: ");
            scanf("%d", &id1);
            arkadaslariBul(id1);
            break;
        case 4:
            printf("Iki kullanici ID'sini girin: ");
            scanf("%d %d", &id1, &id2);
            ortakArkadaslariBul(id1, id2);
            break;
        case 5:
            topluluklariBul();
            break;
        case 6:
            printf("Kullanicilar: ");
            siraliListele(kok);
            printf("\n");
            break;
        default:
            printf("Gecersiz secim!\n");
        }
    } while (secim != 0);

    return 0;
}
