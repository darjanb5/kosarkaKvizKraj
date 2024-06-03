#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quiz.h"
#include "file_utils.h"


int main() {
	int opcija;
	char izvornaDatoteka[DBS];
	char odredisnaDatoteka[DBS];

	while (1) {
		prikaziIzbornik();
		printf("Odaberite opciju: ");
		(void)scanf("%d", &opcija);
		char ime[IME];

		IzbornikOpcija izbor = opcija;
		RezultatIgraca rezultatIgraca = {0};
		RezultatIgraca* rezultati = {0};
		int brojRezultata = 0;

		switch (izbor) {
		case POKRENI_KVIZ:
			pokreniKviz();
			break;
		case PREGLEDAJ_REZULTATE:
			pregledajRezultate();
			break;
		case PRETRAZI_REZULTATE:
			printf("Unesite ime igraca za pretragu: ");
			(void) scanf("%s", ime); 
			rezultati = ucitajRezultate(&brojRezultata);
			if (rezultati != NULL) {
				pretraziRezultat(ime, rezultati, brojRezultata);
				free(rezultati);
				rezultati = NULL;
			}
			break;
		case KOPIRAJ_DATOTEKU:
			printf("Unesite naziv izvorne datoteke: ");
			(void) scanf("%s", izvornaDatoteka);
			printf("Unesite naziv odredisne datoteke: ");
			(void) scanf("%s", odredisnaDatoteka);
			kopirajDatoteku(izvornaDatoteka, odredisnaDatoteka);
			break;
		case OBRISI_DATOTEKU:
			printf("Unesite naziv datoteke za brisanje: ");
			(void) scanf("%s", izvornaDatoteka);
			obrisiDatoteku(izvornaDatoteka);
			break;
		case IZLAZ:
			printf("Izlaz iz programa.\n");
			exit(0);
		default:
			printf("Neispravan odabir. Molim pokusajte ponovo.\n");
		}
	}

	return 0;
}