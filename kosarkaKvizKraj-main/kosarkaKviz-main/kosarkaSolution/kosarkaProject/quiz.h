#ifndef QUIZ_H
#define QUIZ_H


#define BROJ_PITANJA 10
#define DBS 256
#define DG 4
#define GG 128
#define IME 50

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
	KOPIRAJ_DATOTEKU,
	OBRISI_DATOTEKU,
	IZLAZ
} IzbornikOpcija;

void prikaziIzbornik();
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

extern Pitanje pitanja[];
extern int brojPitanja;

#endif