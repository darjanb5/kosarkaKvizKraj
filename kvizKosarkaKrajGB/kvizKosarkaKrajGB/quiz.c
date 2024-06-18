#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "quiz.h"

static RezultatIgraca* rezultatIgraca;
int brojRezultata;

static Pitanje pitanja[BROJ_PITANJA] = {
	{"\nTko je poznat kao 'Kralj kosarke'?", {"Michael Jordan", "LeBron James", "Kobe Bryant", "Shaquille O'Neal"}, 2},
	{"\nKoliko igraca ima kosarkaska momcad na terenu?", {"5", "6", "7", "8"}, 1},
	{"\nKoje godine je osnovana NBA?", {"1946.", "1956.", "1966.", "1976."}, 1},
	{"\nKoja momcad ima najvise NBA prvenstava?", {"Los Angeles Lakers", "Chicago Bulls", "Boston Celtics", "Golden State Warriors"}, 3},
	{"\nKoliki je promjer kosarkaskog obruca u incima?", {"16", "18", "20", "22"}, 2},
	{"\nZa koju momcad trenutno igra Luka Doncic?", {"New York Knicks", "Brooklyn Nets", "Dallas Mavericks", "Atlanta Hawks"}, 3},
	{"\nCiji je nadimak 'Mozart kosarke'?", {"Julius Erving", "Paul Pierce", "Pedja Stojakovic", "Drazen Petrovic"}, 4},
	{"\nKoja momcad nije iz Istocne Konferencije?", {"Boston Celtics", "Indiana Pacers", "Los Angeles Lakers", "Toronto Raptors"}, 3},
	{"\nZa koji tim Toni Kukoc nikad nije nastupio?", {"Dallas Mavericks", "Chicago Bulls", "Philadelphia 76ers", "Milwaukee Bucks"}, 1},
	{"\nIme trenutnog komesara NBA?", {"Mark Cuban", "Adam Silver", "Karl Malone", "Jerry West"}, 2}
};

void prikaziIzbornik() {
	printf("--- IZBORNIK ---\n");
	printf("1. Pokreni kviz\n");
	printf("2. Pregledaj rezultate\n");
	printf("3. Pretrazi rezultate\n");
	printf("4. Izmijeni rezultate\n");
	printf("5. Dodaj igraca\n");
	printf("6. Obrisi igraca\n");
	printf("7. Kopiraj datoteku\n");
	printf("8. Obrisi datoteku\n");
	printf("9. Izlaz\n");
}

void ocisti() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}

void randomPitanja() {
	srand((unsigned)time(NULL));
	for (int i = 0; i < BROJ_PITANJA; ++i) {
		int j = rand() % BROJ_PITANJA;
		Pitanje temp = pitanja[i];
		pitanja[i] = pitanja[j];
		pitanja[j] = temp;
	}
}

void pokreniKviz() {
	RezultatIgraca rezultatIgraca = { 0 };
	printf("\nUnesite vase ime: ");
	(void)scanf("%s", rezultatIgraca.imeIgraca);
	ocisti();
	rezultatIgraca.rezultat = 0;

	randomPitanja();
	postaviPitanja(&rezultatIgraca);
	spremiRezultat(rezultatIgraca.imeIgraca, rezultatIgraca.rezultat);
}

static void postaviPitanja(RezultatIgraca* rezultatIgraca) {
	int odgovor;
	char input[10];

	for (int i = 0; i < BROJ_PITANJA; ++i) {
		while (1) {
			printf("%s\n", pitanja[i].pitanje);
			for (int j = 0; j < 4; ++j) {
				printf("%d. %s\n", j + 1, pitanja[i].opcije[j]);
			}
			printf("Vas odgovor: ");
			fgets(input, sizeof(input), stdin);

			if (sscanf(input, "%d", &odgovor) == 1 && provjeriOdgovor(odgovor)) {
				if (odgovor == pitanja[i].tocanOdgovor) {
					rezultatIgraca->rezultat += 10;
				}
				break;
			}
			else {
				printf("Neispravan unos. Molimo unesite broj izmedu 1 i 4.\n");
			}
		}
	}
	printf("\nKviz zavrsen! Vas rezultat: %d/100\n", rezultatIgraca->rezultat);
}

static int provjeriOdgovor(int odgovor) {
	return odgovor >= 1 && odgovor <= 4;
}

void pregledajRezultate() {
	FILE* datoteka = fopen("rezultati.txt", "rb");
	if (datoteka == NULL) {
		perror("Neuspjelo otvaranje datoteke za citanje rezultata\n");
		return;
	}

	fseek(datoteka, 0, SEEK_END);
	long velicinaDatoteke = ftell(datoteka);
	rewind(datoteka);

	brojRezultata = velicinaDatoteke / sizeof(RezultatIgraca);
	rezultatIgraca = (RezultatIgraca*)malloc(brojRezultata * sizeof(RezultatIgraca));
	if (rezultatIgraca == NULL) {
		perror("Neuspjelo zauzimanje memorije za rezultate\n");
		fclose(datoteka);
		return;
	}

	fread(rezultatIgraca, sizeof(RezultatIgraca), brojRezultata, datoteka);
	qsort(rezultatIgraca, brojRezultata, sizeof(RezultatIgraca), usporediRezultate);

	printf("Rezultati:\n");
	for (int i = 0; i < brojRezultata; ++i) {
		printf("%s: %d\n", rezultatIgraca[i].imeIgraca, rezultatIgraca[i].rezultat);
	}

	free(rezultatIgraca);
	rezultatIgraca = NULL;

	fclose(datoteka);
}

void spremiRezultat(char* imeIgraca, int rezultat) {
	if (imeIgraca == NULL) {
		fprintf(stderr, "Nevazeci pokazivac na ime igraca.\n");
		return;
	}

	FILE* datoteka = fopen("rezultati.txt", "ab");
	if (datoteka == NULL) {
		perror("Neuspjelo otvaranje datoteke za spremanje rezultata\n");
		return;
	}

	RezultatIgraca noviRezultat = { 0 };
	strncpy(noviRezultat.imeIgraca, imeIgraca, sizeof(noviRezultat.imeIgraca) - 1);
	noviRezultat.imeIgraca[sizeof(noviRezultat.imeIgraca) - 1] = '\0';
	noviRezultat.rezultat = rezultat;

	fwrite(&noviRezultat, sizeof(RezultatIgraca), 1, datoteka);
	fclose(datoteka);
}

int usporediRezultate(const void* a, const void* b) {
	return ((RezultatIgraca*)b)->rezultat - ((RezultatIgraca*)a)->rezultat;
}

static int usporediPoImenu(const void* a, const void* b) {
	return strcmp(((RezultatIgraca*)a)->imeIgraca, ((RezultatIgraca*)b)->imeIgraca);
}

int pretraziRezultat(const char* imeIgraca, RezultatIgraca* rezultatIgraca, int brojRezultata) {
	if (imeIgraca == NULL) {
		fprintf(stderr, "Nevazeci pokazivac na ime igraca.\n");
		return 0;
	}

	if (rezultatIgraca == NULL || brojRezultata == 0) {
		fprintf(stderr, "Rezultati nisu ucitani.\n");
		return 0;
	}

	RezultatIgraca kljuc = { 0 };
	strncpy(kljuc.imeIgraca, imeIgraca, sizeof(kljuc.imeIgraca) - 1);
	kljuc.imeIgraca[sizeof(kljuc.imeIgraca) - 1] = '\0';

	RezultatIgraca* pronadjen = (RezultatIgraca*)bsearch(&kljuc, rezultatIgraca, brojRezultata, sizeof(RezultatIgraca), usporediPoImenu);
	if (pronadjen != NULL) {
		printf("Pronaden rezultat za %s: %d\n", pronadjen->imeIgraca, pronadjen->rezultat);
	}
	else {
		printf("Rezultat za %s nije pronaden.\n", imeIgraca);
	}
	return 0;
}

RezultatIgraca* ucitajRezultate(int* brojRezultata) {
	FILE* datoteka = fopen("rezultati.txt", "rb");
	if (datoteka == NULL) {
		perror("Neuspjelo otvaranje datoteke za citanje rezultata\n");
		return NULL;
	}

	fseek(datoteka, 0, SEEK_END);
	long velicinaDatoteke = ftell(datoteka);
	rewind(datoteka);

	*brojRezultata = velicinaDatoteke / sizeof(RezultatIgraca);
	RezultatIgraca* rezultati = (RezultatIgraca*)malloc(*brojRezultata * sizeof(RezultatIgraca));
	if (rezultati == NULL) {
		perror("Neuspjelo zauzimanje memorije za rezultate\n");
		fclose(datoteka);
		return NULL;
	}

	fread(rezultati, sizeof(RezultatIgraca), *brojRezultata, datoteka);
	fclose(datoteka);

	return rezultati;
}

void azurirajRezultat(const char* imeIgraca, int noviRezultat) {
	if (imeIgraca == NULL) {
		fprintf(stderr, "Nevazeci pokazivac na ime igraca.\n");
		return;
	}

	RezultatIgraca* rezultati = ucitajRezultate(&brojRezultata);
	if (rezultati == NULL) {
		fprintf(stderr, "Neuspjelo ucitavanje rezultata.\n");
		return;
	}

	int pronadjen = 0;
	for (int i = 0; i < brojRezultata; ++i) {
		if (strcmp(rezultati[i].imeIgraca, imeIgraca) == 0) {
			rezultati[i].rezultat = noviRezultat;
			pronadjen = 1;
			break;
		}
	}

	if (pronadjen) {
		FILE* datoteka = fopen("rezultati.txt", "wb");
		if (datoteka == NULL) {
			perror("Neuspjelo otvaranje datoteke za azuriranje rezultata\n");
			free(rezultati);
			return;
		}

		fwrite(rezultati, sizeof(RezultatIgraca), brojRezultata, datoteka);
		fclose(datoteka);
		printf("Rezultat uspjesno azuriran.\n");
	}
	else {
		printf("Rezultat za igraca %s nije pronaden.\n", imeIgraca);
	}

	free(rezultati);
}

int provjeraSlova() {
	char input[DS];
	int option;

	while (1) {
		fgets(input, sizeof(input), stdin);
		if (sscanf(input, "%d", &option) == 1) {
			break;
		}
		printf("Neispravan unos. Molimo unesite broj: ");
	}

	return option;
}
void dodajIgraca(const char* imeIgraca, int rezultat) {
	if (imeIgraca == NULL) {
		fprintf(stderr, "Nevazeci pokazivac na ime igraca.\n");
		return;
	}

	FILE* datoteka = fopen("rezultati.txt", "ab");
	if (datoteka == NULL) {
		perror("Neuspjelo otvaranje datoteke za dodavanje igraca\n");
		return;
	}

	RezultatIgraca noviRezultat = {0};
	strncpy(noviRezultat.imeIgraca, imeIgraca, IME);
	noviRezultat.rezultat = rezultat;

	fwrite(&noviRezultat, sizeof(RezultatIgraca), 1, datoteka);
	fclose(datoteka);
}

void obrisiIgraca(const char* imeIgraca) {
	FILE* datoteka = fopen("rezultati.txt", "rb");
	if (datoteka == NULL) {
		perror("Neuspjelo otvaranje datoteke za citanje rezultata\n");
		return;
	}

	FILE* temp = fopen("temp.txt", "wb");
	if (temp == NULL) {
		perror("Neuspjelo otvaranje privremene datoteke\n");
		fclose(datoteka);
		return;
	}

	RezultatIgraca rezultat;
	int pronaden = 0;
	while (fread(&rezultat, sizeof(RezultatIgraca), 1, datoteka) == 1) {
		if (strcmp(rezultat.imeIgraca, imeIgraca) == 0) {
			pronaden = 1;
		}
		else {
			fwrite(&rezultat, sizeof(RezultatIgraca), 1, temp);
		}
	}

	fclose(datoteka);
	fclose(temp);

	if (pronaden) {
		remove("rezultati.txt");
		(void)rename("temp.txt", "rezultati.txt");
		printf("Igrac %s je obrisan.\n", imeIgraca);
	}
	else {
		remove("temp.txt");
		printf("Igrac s imenom %s nije pronaden.\n", imeIgraca);
	}
}