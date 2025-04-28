# Sosyal Ağ Analiz Sistemi

C dilinde geliştirilmiş, **Red-Black Tree** veri yapısını kullanan bir sosyal ağ analiz sistemi. Bu program, kullanıcılar arasındaki arkadaşlık ilişkilerini takip eder ve çeşitli sosyal ağ analizleri yapmanıza olanak tanır.

## Özellikler

- **Kullanıcı Yönetimi**: Kullanıcıları Red-Black Tree veri yapısında saklar ve yönetir
- **Arkadaşlık İlişkileri**: Kullanıcılar arasında arkadaşlık bağlantıları oluşturur
- **Derinlik Analizi**: Birinci ve ikinci seviye arkadaşları tespit eder
- **Sosyal Çevre Etki Alanı**: Bir kullanıcının etki alanını hesaplar
- **Ortak Arkadaş Tespiti**: İki kullanıcı arasındaki ortak arkadaşları bulur
- **Topluluk Tespiti**: Ağdaki bağlantılı toplulukları tespit eder
- **Veri Dosyası Desteği**: Metin dosyalarından veri okuyabilme kabiliyeti

## Teknik Altyapı

- **Red-Black Tree** veri yapısı ile verimli kullanıcı aramaları
- **DFS** (Derinlik Öncelikli Arama) algoritması ile ağ analizi
- C dilinin dinamik bellek yönetim yetenekleri

## Kullanım

### Derleme

```bash
gcc -o sosyal_ag_analiz sosyal_ag_analiz.c
```

### Çalıştırma

```bash
./sosyal_ag_analiz
```

### Menü Seçenekleri

Programı çalıştırdığınızda aşağıdaki menü seçenekleri görüntülenir:

1. **Veri Dosyasını Oku**: Dışarıdan bir dosyadan kullanıcı ve arkadaşlık verilerini okur
2. **Arkadaşlık İlişkisi Ekle**: İki kullanıcı arasında yeni bir arkadaşlık bağlantısı oluşturur
3. **Kullanıcı Arkadaşlarını Göster**: Bir kullanıcının birinci ve ikinci seviye arkadaşlarını listeler
4. **Ortak Arkadaşları Bul**: İki kullanıcı arasındaki ortak arkadaşları bulur
5. **Toplulukları Tespit Et**: Ağdaki bağlantılı toplulukları tespit eder
6. **Tüm Kullanıcıları Göster**: Sistemdeki tüm kullanıcıları sıralı olarak gösterir
0. **Çıkış**: Programdan çıkar

## Veri Dosya Formatı

Program, aşağıdaki formatta veri dosyaları kabul eder:

```
USER [id]
FRIEND [id1] [id2]
```

**Örnek:**
```
USER 101
USER 102
USER 103
FRIEND 101 102
FRIEND 102 103
```

## Kısıtlamalar

- Maksimum kullanıcı sayısı: **1000**
- Bir kullanıcının maksimum arkadaş sayısı: **100**

## Algoritma Detayları

### Red-Black Tree

Program, kullanıcıları verimli şekilde saklamak ve aramak için bir **Red-Black Tree** veri yapısı kullanır. Bu yapı, **O(log n)** zaman karmaşıklığında arama, ekleme ve silme işlemlerine olanak tanır.

### DFS (Derinlik Öncelikli Arama)

Ağdaki bağlantıları analiz etmek için **DFS algoritması** kullanılmaktadır. Bu algoritma, belirli derinlikteki arkadaşları bulmak ve toplulukları tespit etmek için kullanılır.

## Lisans

Bu proje açık kaynak olarak **MIT lisansı** altında dağıtılmaktadır. Daha fazla bilgi için `LICENSE` dosyasını inceleyebilirsiniz.
