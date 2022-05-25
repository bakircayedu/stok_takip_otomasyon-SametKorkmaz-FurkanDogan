#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
FILE *pDosya;
char kategoriler[7][50] = {"Atistirmalik", "Icecek", "Temel Gida", "Et Urunleri", "Temizlik & Ev Gerecleri", "Kisisel Bakim", "Sut & Kahvaltılık"};
char sehirler[6][50] = {"Izmir", "Istanbul", "Bursa", "Ankara", "Antalya", "Adana"};
enum dosyaDegisim
{
    silme = 0,
    guncelleme = 1,
    za
};
enum tedarikciIslemleri
{
    tedarikci_girisi = 1,
    tedarikciGuncelleSil,
    tedarikciGoruntule
};
enum urunIslemleri
{
    urungiris = 1,
    urunGuncelleSil,
    urunGoruntule,
    geriDonus
};
enum genelislemler
{
    urunIslem = 1,
    tedarikciIslem,
    cikis
};
struct urun
{
    char kategori[30], urunAdi[40];
    int urunKodu, urunSatisFiyati, stokMiktari;
} urunListesi, urunOkumaListe, *guncellenenListe;
struct tedarikci
{
    int no;
    char ad[50], adres[100], sehir[50];
} tedarikciListe;
void dosyaGuncelle(int urunKodu, struct urun guncellenenUrun, int islem)
{
    int structMiktari, i = 0, k;
    struct urun okunanUrun;
    pDosya = fopen("urunListesi.txt", "r");
    structMiktari = filelength(fileno(pDosya)) / sizeof(struct urun);
    if (islem == silme)
    {
        guncellenenListe = malloc(sizeof(struct urun) * (structMiktari - 1));
    }
    else
    {
        guncellenenListe = malloc(sizeof(struct urun) * structMiktari);
    }

    for (k = 0; k < structMiktari; k++)
    {

        fread(&okunanUrun, sizeof(struct urun), 1, pDosya);
        if (urunKodu != okunanUrun.urunKodu)
        {
            guncellenenListe[i] = okunanUrun;
            i++;
        }
        else
        {
            if (islem == guncelleme)
            {
                guncellenenListe[i] = guncellenenUrun;
                i++;
            }
        }
    }
    fclose(pDosya);
    pDosya = fopen("urunListesi.txt", "w");
    i = 0;
    if (islem == silme)
    {
        structMiktari--;
    }
    for (k = 0; k < structMiktari; k++)
    {
        fwrite(&guncellenenListe[i], sizeof(struct urun), 1, pDosya);
        i++;
    }

    free(guncellenenListe);
    fclose(pDosya);
}

void ekranUrunYaz(struct urun yazilanUrun)
{
    printf("Urun kategorisi: %s\n", yazilanUrun.kategori);
    printf("Urun ismi: %s\n", yazilanUrun.urunAdi);
    printf("Urun kodu: %d\n", yazilanUrun.urunKodu);
    printf("Urun satis fiyati: %d\n", yazilanUrun.urunSatisFiyati);
    printf("Urun stok miktari: %d\n", yazilanUrun.stokMiktari);
    printf("\n");
}

int urunBul(int urunKod, struct urun *okunanUrun)
{
    int structSayisi, i;
    pDosya = fopen("urunListesi.txt", "r");
    structSayisi = filelength(fileno(pDosya)) / sizeof(struct urun);
    for (i = 0; i < structSayisi; i++)
    {
        fread(okunanUrun, sizeof(struct urun), 1, pDosya);
        if (urunKod == (*okunanUrun).urunKodu)
        {
            fclose(pDosya);
            return 1;
        }
    }

    fclose(pDosya);
    return 0;
}

int urunleriOku()
{
    int i, structSayisi;

    pDosya = fopen("urunListesi.txt", "r");
    structSayisi = filelength(fileno(pDosya)) / sizeof(struct urun);
    if (structSayisi == 0)
    {
        printf("Dosya bos!\n\n");
        fclose(pDosya);
        return 0;
    }

    for (i = 0; i < structSayisi; i++)
    {
        fread(&urunOkumaListe, sizeof(struct urun), 1, pDosya);
        ekranUrunYaz(urunOkumaListe);
        printf("\n");
    }
    fclose(pDosya);
    return 1;
}
int urunSilme()
{
    int urunKod, k, komut;
    struct urun silinicekUrun;
    urunleriOku();
    printf("Urun silme sectiniz.\nGeri donmek icin 0 yaziniz\nSilmek istediginiz urun kodunu yaziniz: ");
    scanf("%d", &urunKod);
    if (urunKod == 0)
    {
        system("cls");
        return 0;
    }
    k = urunBul(urunKod, &silinicekUrun);
    if (k == 0)
    {
        printf("Aradıgınız urun kodu bulunamadı!\nDevam etmek icin herhangi bir tusa basiniz.\n");
        getch();
        urunSilme();
    }
    system("cls");
    ekranUrunYaz(silinicekUrun);
    printf("Silmek isteginiz urun bu mu? Evet(1) Hayır(2) ");
    scanf("%d", &komut);
    if (komut == 1)
    {
        dosyaGuncelle(urunKod, silinicekUrun, silme);
    }
    else
    {
        urunSilme();
    }
    system("cls");
    printf("Urun silindi!\n\n");
}
int urunGuncelleme()
{
    int urunkod, sayi, i, kategori, komut, t;

    struct urun guncellenenUrun;
    urunleriOku();
    printf("Urun guncelleme sectiniz.\nGeri donmek icin 0 yaziniz\nGuncellemek istediginiz urunun kodunu yaziniz: ");
    scanf("%d", &urunkod);
    if (urunkod == 0)
    {
        system("cls");
        return 0;
    }
    system("cls");
    t = urunBul(urunkod, &guncellenenUrun);
    if (t == 1)
    {
        ekranUrunYaz(guncellenenUrun);
        printf("Guncellemek istediginiz urun bu mu? Evet(1) Hayır(2)\n");
        scanf(" %d", &komut);
        if (komut == 1)
        {
            printf("Guncellemek istediginiz kismi seciniz.\nKategori (1)\nUrun ismi (2)\nUrun kodu (3)\nUrun satis fiyatı (4)\nUrun stok miktari (5)\nGeri donus (6)\n");
            scanf("%d", &sayi);
            switch (sayi)
            {
            case 1:
                system("cls");
                for (i = 0; i < 7; i++)
                {
                    printf("%s (%d)\n", kategoriler[i], i + 1);
                }
                printf("Yeni kategoriyi seciniz: ");
                scanf("%d", &kategori);
                strcpy(guncellenenUrun.kategori, kategoriler[kategori - 1]);
                break;
            case 2:
                printf("Yeni urun ismini giriniz: ");
                scanf(" %[^\n]s", guncellenenUrun.urunAdi);
                break;

            case 3:
                printf("Yeni urun kodunu giriniz: ");
                scanf("%d", &guncellenenUrun.urunKodu);
                break;

            case 4:
                printf("Yeni urun satis fiyatini giriniz: ");
                scanf("%d", &guncellenenUrun.urunSatisFiyati);
                break;

            case 5:
                printf("Yeni urun stok miktarini giriniz: ");
                scanf("%d", &guncellenenUrun.stokMiktari);
                break;
            case 6:
                system("cls");
                urunGuncelleme();

            default:
                break;
            }
            system("cls");
            dosyaGuncelle(urunkod, guncellenenUrun, guncelleme);
            printf("Urun guncellendi!\n\n");
        }
    }
    else
    {

        printf("Aradıgınız urun kodu bulunamadı!\nDevam etmek icin herhangi bir tusa basiniz.\n");
        getch();
        system("cls");
        urunGuncelleme();
    }
}
void DosyaUrunYaz()
{
    pDosya = fopen("urunListesi.txt", "a");

    fwrite(&urunListesi, sizeof(struct urun), 1, pDosya);
    fclose(pDosya);
}
void ekranaTedarikciYaz(struct tedarikci yazilanTedarikci)
{
    printf("Tedarikci sehri: %s\n", yazilanTedarikci.sehir);
    printf("Tedarikci ismi: %s\n", yazilanTedarikci.ad);
    printf("Tedarikci no: %d\n", yazilanTedarikci.no);
    printf("Tedarikci adresi: %s\n", yazilanTedarikci.adres);
    printf("\n");
}

void dosyaTedarikciYaz()
{
    pDosya = fopen("tedarikciListesi.txt", "a");
    fwrite(&tedarikciListe, sizeof(struct tedarikci), 1, pDosya);
    fclose(pDosya);
}
int tedarikcileriOku()
{
    int i, structSayisi;
    pDosya = fopen("tedarikciListesi.txt", "r");
    structSayisi = filelength(fileno(pDosya)) / sizeof(struct tedarikci);
    if (structSayisi == 0 || pDosya == NULL)
    {
        printf("Dosya bos!\n\n");
        fclose(pDosya);

        return 0;
    }
    while (structSayisi != 0)
    {
        fread(&tedarikciListe, sizeof(struct tedarikci), 1, pDosya);
        ekranaTedarikciYaz(tedarikciListe);
        structSayisi--;
    }
    fclose(pDosya);
}
void tedarikciGiris()
{
    int i, sehir;
    system("cls");
    printf("Tedarikci girisi.\n-------------\n");
    for (i = 0; i < 6; i++)
    {
        printf("%s (%d)", sehirler[i], i + 1);
        printf("\n");
    }
    printf("Tedarikci sehiri seciniz: ");
    scanf("%d", &sehir);
    strcpy(tedarikciListe.sehir, sehirler[sehir - 1]);
    printf("Tedarikci Ad: ");
    scanf(" %[^\n]s", tedarikciListe.ad);
    printf("Tedarikci no: ");
    scanf("%d", &tedarikciListe.no);
    printf("Tedarikci adresi: ");
    scanf(" %[^\n]s", tedarikciListe.adres);
    system("cls");
    dosyaTedarikciYaz();
    printf("Tedarikci girisi basarili!\n\n");
}

void urunGiris()
{

    system("cls");
    int i, kategori;

    printf("Urun girisi\n----------\n");
    for (i = 0; i < 7; i++)
    {
        printf("%s (%d)\n", kategoriler[i], i + 1);
    }
    printf("Urun kategorisi seciniz: ");
    scanf("%d", &kategori);
    strcpy(urunListesi.kategori, kategoriler[kategori - 1]);
    printf("Urun ismi: ");
    scanf(" %[^\n]s", urunListesi.urunAdi);
    printf("Urun kodu: ");
    scanf("%d", &urunListesi.urunKodu);
    printf("Urun satis fiyati: ");
    scanf("%d", &urunListesi.urunSatisFiyati);
    printf("Urun stok miktari: ");
    scanf("%d", &urunListesi.stokMiktari);
    system("cls");
    DosyaUrunYaz();
    system("cls");
    printf("Urun girisi basarılı!\n");
}

int main()
{

    int komut, urunKomut, tedarikciKomut;
    printf("Merhaba!\nYapmak istediginiz islemi belirtiniz.\n");
    while (1)
    {

        printf("Urun islemleri (1)\nTedarikci islemleri (2)\nProgramdan cıkıs (3)\n");
        scanf("%d", &komut);
        if (komut == urunIslem)
        {
            system("cls");
            printf("Urun girisi (1)\nUrun guncelleme/silme (2)\nUrunleri goruntuleme (3)\nGeri donus (4)\n");
            scanf("%d", &urunKomut);
            if (urunKomut == urungiris)
            {
                urunGiris();
            }
            else if (urunKomut == urunGuncelleSil)
            {
                system("cls");
                if (urunleriOku() == 0)
                {
                    continue;
                }
                printf("Urun guncellemek icin (1)\nUrun silmek icin (2)\nGeri donus (3)\n");
                scanf("%d", &urunKomut);
                if (urunKomut == 1)
                {
                    system("cls");
                    urunGuncelleme();
                }
                else if (urunKomut == 2)
                {
                    system("cls");
                    urunSilme();
                }
                else
                {
                    system("cls");
                    continue;
                }
            }

            else if (urunKomut == urunGoruntule)
            {
                system("cls");
                urunleriOku();
            }
            else
            {
                system("cls");
                continue;
            }
        }
        else if (komut == tedarikciIslem)
        {
            system("cls");
            printf("Tedarikci girisi (1)\nTedarikci guncelleme/silme (2)\nTedarikcileri goruntuleme (3)\nGeri donus (4)\n");
            scanf("%d", &tedarikciKomut);
            if (tedarikciKomut == tedarikci_girisi)
            {
                tedarikciGiris();
                system("cls");
            }
            else if (tedarikciKomut == tedarikciGuncelleSil)
            {
            }
            else if (tedarikciKomut == tedarikciGoruntule)
            {
                system("cls");
                tedarikcileriOku();
            }
        }
        else if (komut == cikis)
        {
            break;
        }
    }
    return 0;
}
