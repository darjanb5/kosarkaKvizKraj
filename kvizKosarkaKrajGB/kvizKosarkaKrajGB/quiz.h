#ifndef QUIZ_H
#define QUIZ_H


#define BROJ_PITANJA 10
#define DBS 256
#define DG 4
#define GG 128
#define IME 50
#define DS 10

typedef struct {
	char pitanje[DBS];
	char opcije[DG][GG];
	int tocanOdgovor;
} Pitanje;

typedef struct {
	char imeIgraca[IME];
	int rezultat;
} RezultatIgraca;

typedef enum {
	POKRENI_KVIZ = 1,
	PREGLEDAJ_REZULTATE,
	PRETRAZI_REZULTATE,
	IZMIJENI_REZULTAT,
	DODAJ_IGRACA,
	OBRISI_IGRACA,
	KOPIRAJ_DATOTEKU,
	OBRISI_DATOTEKU,
	IZLAZ
} IzbornikOpcija;

void prikaziIzbornik();
void ocisti();
void randomPitanja();
void pokreniKviz();
void postaviPitanja(RezultatIgraca* rezultatIgraca);
int provjeriOdgovor(int odgovor);
void pregledajRezultate();
void spremiRezultat(char* imeIgraca, int rezultat);
int usporediRezultate(const void* a, const void* b);
int usporediPoImenu(const void* a, const void* b);
int pretraziRezultat(const char* imeIgraca, RezultatIgraca* rezultatIgraca, int brojRezultata);
RezultatIgraca* ucitajRezultate(int* brojRezultata);
void azurirajRezultat(const char* imeIgraca, int noviRezultat);
int provjeraSlova();
void dodajIgraca(const char* imeIgraca, int rezultat);
void obrisiIgraca(const char* imeIgraca);

extern Pitanje pitanja[];
extern int brojPitanja;

#endif