#define _CRT_SECURE_NO_WARNINGS
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
void anaMenuyeDonme();
void addNumber(int,int);
void viewNumbers(int);
void anaMenu();
int kontrol();
void numaraSilme(int);
#define MAX 100
#define DOSYA_ADI "Telefon Rehberi.txt"
typedef struct {
	char number[50];
	char name[50];
}rehber;
int sayac = 0; //Art arda hatalı girişleri ve sonsuz donguyu kirmak icin
rehber kayit[MAX];
FILE* fptr;
int main(void)
{
	anaMenu();
	return(0);
}
void addNumber(int son_deger) {
	if (sayac >= 3)
		anaMenu();
	else if (sayac == 2)
		printf("\nSonraki hatali giriste Ana menuye yonlendirileceksiniz.\n");
	fptr = fopen(DOSYA_ADI, "a");
	if (fptr == NULL) {
		printf("Dosya açılamadı. Lütfen dosyanın mevcut olduğunu kontrol edin.");
		return;
	}
	else {
		if (son_deger == MAX) {
			printf("Kayit sinirina ulastiniz.\nYeni kayit eklemek icin yer acmaniz gerekiyor.\n(Kayit siniri = 100)");
			fclose(fptr);
			anaMenu();
		}
		else {
			int i;
			char secim;
			do {
					printf("Numara giriniz: ");
					scanf(" %49s", kayit[son_deger].number); // Overflow riskini ortadan kaldırmak için %49s ile sınırlandı
					if (strlen(kayit[son_deger].number) != 11) {
						printf("Telefon numarasi 11 haneli olmalidir.\n");
						sayac++;
						continue;
					}
					int flag = 1;
					for (int i = 0; i < 11; i++) {
						if (kayit[son_deger].number[i] < '0' || kayit[son_deger].number[i] > '9') {
							flag = 0;
							if (flag == 0)	
								break;
						}
					}
					if (!flag) {
						printf("Telefon numarasi sadece rakamlardan olusmalidir.\n");
						continue;
					}
					break; // Hata yoksa döngüden çıkmak için
				} while (1);
				printf("Isim giriniz :");
				getchar(); // Tampon temizleme
				fgets(kayit[son_deger].name, sizeof(kayit[son_deger].name), stdin);
				kayit[son_deger].name[strcspn(kayit[son_deger].name, '\n')] = '\0';
				fprintf(fptr, "%s\t%s\n", kayit[son_deger].name, kayit[son_deger].number);
				fclose(fptr);
				printf("%s kisisi %s numarasiyla kayit edilmistir.\n", kayit[son_deger].name, kayit[son_deger].number);
				anaMenuyeDonme();
		}
	}
}
void viewNumbers(int son_deger) {
	int siralama = 1;
	char string[MAX];
	fptr = fopen(DOSYA_ADI, "r");
	if (fptr == NULL) {
		printf("Dosya açılamadı");
		return;
	}
	while (fgets(string, MAX, fptr) != NULL) {
		printf("%d) %s",siralama , string);
		siralama += 1;
	}
	fclose(fptr);
	anaMenuyeDonme();
}
int kontrol() {
	char kontrol[MAX];
	int son_deger = 0;
	fptr = fopen(DOSYA_ADI, "r");
	if (fptr == NULL) {
		printf("Telefon Rehberi uygulamasına erişilemedi.");
		exit(1); // Beklenmedik hata durumu.
	}
	while (fgets(kontrol, MAX, fptr) != NULL) {
		son_deger += 1;
	}
	fclose(fptr);
	return(son_deger);
}
void anaMenu(){
	sayac = 0; // Ana menuye donuldugunde sayac sifirlanmasi
	printf("\n");
	int choice;
	int son_deger = kontrol();
	printf("\t\t\033[4;1mTelefon Rehberi\n\n");
	printf("\t1-Numara ekle\n");
	printf("\t2-Kayitli numaralari goruntule\n");
	printf("\t3-Numara silme\n");
	printf("\t4-Cikis yap \033[0m\n");
	scanf(" %d", &choice);
	getchar();
	if (choice == 1) addNumber(son_deger);
	else if (choice == 2) viewNumbers(son_deger);
	else if (choice == 3) numaraSilme(son_deger);
	else if (choice == 4) return;
	else { printf("Yanlis deger girisi yaptiniz."); }
}
void anaMenuyeDonme() {
	char cevap;
	printf("\nAna menuye donmek ister misiniz ?(E/H)\n");
	scanf(" %c", &cevap);
	getchar();
	switch (cevap) {
	case 'e':
	case'E':
		anaMenu();
		break;
	case'h':
	case'H':
		printf("Program sonlandiriliyor.\n");
		break;
	default:
		printf("Evet icin : (E/e)\nHayir icin : (H/h)	Girmelisiniz.\n");
		anaMenuyeDonme();
	}
}
void numaraSilme(int son_deger) {
	if (sayac >= 3)
		anaMenu();
	else if (sayac == 2)
		printf("\nSonraki hatali giriste Ana menuye yonlendirileceksiniz.\n");
	char string[MAX];
	int secim, guncelSatir=0;
	FILE* clone;
	if (son_deger == 0) {
		printf("Dosyada silinecek kayit bulunamadi.");
		anaMenuyeDonme();
	}
	fptr = fopen(DOSYA_ADI, "r");
	if (fptr == NULL) {
		printf("Dosya açılamadı.Cikis Yapiliyor.");
		exit(0); // Hata durumu .
		
	}
	printf("Kayitlar :\n");
	while (fgets(string, MAX, fptr) != NULL) {
		printf("%d) %s", ++guncelSatir, string);
	}
	printf("Silmek istediginiz kisiyi seciniz (1-%d):", son_deger);
	scanf(" %d", &secim);
	getchar();
	if (secim < 1 || secim > son_deger) {
		if(sayac < 2) //Son hata mesajının görülmesine gerek yok çünkü zaten ana menüye dönülecek
			printf("1 ile %d arasinda secim yapmalisiniz.",son_deger);
		fclose(fptr);
		sayac++;
		numaraSilme(son_deger);
	}
	rewind(fptr);
	clone = fopen("Clone.txt", "w");
	if (clone == NULL) {
		printf("Dosya açılamadı.Cikis Yapiliyor.");
		fclose(fptr);
		exit(0); // Hata durumu 
	}
	guncelSatir = 0;
	while (fgets(string, MAX, fptr) != NULL)
	{
		guncelSatir++;
		if (guncelSatir != secim)
		{
			fputs(string, clone);
		}
	}
	fclose(clone);
	fclose(fptr);
	remove(DOSYA_ADI);
	rename("Clone.txt", DOSYA_ADI);
	anaMenuyeDonme();
}