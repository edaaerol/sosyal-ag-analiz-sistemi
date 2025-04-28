# sosyal-ag-analiz-sistemi
# Sosyal Ağ Analiz Sistemi

## Proje Açıklaması
Bu proje, sosyal ağlardaki kullanıcı ilişkilerini modellemek ve analiz etmek amacıyla geliştirilmiştir. Kullanıcılar arasındaki ilişkiler, **ağaç veri yapıları** kullanılarak modellenmiştir. Bu sistem, derinlik öncelikli arama (DFS), ortak arkadaş analizi, topluluk tespiti ve etki alanı hesaplama gibi analizler yapabilmektedir. Proje, ayrıca **kırmızı-siyah ağaç** veri yapısını içeren bir yapıyı kullanarak kullanıcıların ilişkilerini verimli bir şekilde depolar.

## Özellikler

- **Ağaç Veri Yapısı Kullanımı:** Kullanıcılar ve ilişkileri, ağaç veri yapısı kullanılarak modellenir.
- **Derinlik Öncelikli Arama (DFS):** Kullanıcılar arasındaki ilişkileri keşfetmek için DFS algoritması uygulanır.
- **Ortak Arkadaş Analizi:** Kullanıcılar arasındaki ortak arkadaşlar belirlenebilir.
- **Topluluk Tespiti:** Sosyal ağdaki benzer özelliklere sahip kullanıcılar gruplandırılır.
- **Etki Alanı Hesaplama:** Bir kullanıcının ağdaki etkisi ve bağlı olduğu kişiler hesaplanır.
- **Kırmızı-Siyah Ağaç:** Kullanıcıların ilişkileri, verimli arama ve güncelleme işlemleri için kırmızı-siyah ağaç kullanılarak saklanır.

## Kullanım

### Derleme ve Çalıştırma

Bu kodu çalıştırmak için bir C derleyicisine ihtiyacınız vardır. GCC kullanarak şu şekilde derleyebilirsiniz:

```sh
gcc social_network_analysis.c -o social_network_analysis
./social_network_analysis
```
Örnek Çıktı
Kod çalıştırıldığında aşağıdaki gibi bir çıktı üretilecektir:


Topluluklar:
------------------------
Topluluk 1: [Kullanıcı A, Kullanıcı B, Kullanıcı C]

Topluluk 2: [Kullanıcı D, Kullanıcı E]

Ortak Arkadaşlar:
------------------------
Kullanıcı A ve Kullanıcı B'nin ortak arkadaşları: [Kullanıcı C]

Kullanıcı A'nın Etki Alanı: [Kullanıcı B, Kullanıcı C]

Fonksiyonlar
------------------------
```sh
void initSocialNetwork(SocialNetwork* network): Sosyal ağ sistemini başlatır.

void addUser(SocialNetwork* network, const char* userName): Yeni bir kullanıcıyı sosyal ağa ekler.

void addFriendship(SocialNetwork* network, const char* user1, const char* user2): İki kullanıcı arasında arkadaşlık ilişkisi ekler.

void dfs(SocialNetwork* network, const char* startUser): Derinlik öncelikli arama (DFS) algoritmasını başlatır.

void commonFriends(SocialNetwork* network, const char* user1, const char* user2): İki kullanıcı arasındaki ortak arkadaşları listeler.

void detectCommunities(SocialNetwork* network): Sosyal ağdaki toplulukları tespit eder.

void calculateInfluence(SocialNetwork* network, const char* user): Bir kullanıcının sosyal ağdaki etkisini hesaplar.

void printNetwork(SocialNetwork* network): Tüm sosyal ağı ve kullanıcı ilişkilerini yazdırır.
```

Lisans
Bu proje MIT Lisansı ile lisanslanmıştır. Daha fazla bilgi için LICENSE dosyasına bakabilirsiniz.
