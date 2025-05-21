#include <stdio.h>
#include <time.h>
#include "orario.h"
#include <stdlib.h>

// Definizioni di colori per output su terminale
#define VIOLA    "\033[1;35m" 
#define ROSSO    "\033[1;31m"
#define VERDE    "\033[1;32m"
#define GIALLO   "\033[1;33m"
#define RESET    "\033[0m" // Reset colore

// ===============================
// DEFINIZIONE DELLA STRUTTURA
// ===============================
struct Orario {
    short int ora;     // Ora (0–23)
    short int minuti;  // Minuti (0–59)
};

// ===============================
// CREAZIONE DI UN NUOVO ORARIO (inizializzato a 00:00)
// ===============================
Orario* newOrario() {
    Orario* orario = (Orario*)malloc(sizeof(Orario));
    if (orario != NULL) {
        orario->ora = 0;
        orario->minuti = 0;
    } else {
        printf("Errore nell'allocazione della memoria per Orario.\n");
    }
    return orario;
}

// ===============================
// GETTER ORA
// ===============================
int getOra(const Orario* orario) {
    return orario->ora;
}

// ===============================
// CREA UN ORARIO SPECIFICO (con validazione)
// ===============================
Orario* creaOrario(int ora, int minuti) {
    if (ora < 0 || ora > 23 || minuti < 0 || minuti > 59) {
        printf("Orario non valido.\n");
        return NULL;
    }

    Orario* nuovo_orario = malloc(sizeof(Orario));
    if (nuovo_orario == NULL) {
        printf("Errore allocazione memoria per Orario.\n");
        return NULL;
    }

    nuovo_orario->ora = ora;
    nuovo_orario->minuti = minuti;
    return nuovo_orario;
}

// ===============================
// GETTER MINUTI
// ===============================
int getMinuto(const Orario* orario) {
    return orario->minuti;
}

// ===============================
// SETTER ORA
// ===============================
void setOra(Orario* orario, int ora) {
    if (orario != NULL) {
        orario->ora = ora;
    } else {
        printf("Errore: Orario non valido.\n");
    }
}

// ===============================
// SETTER MINUTI
// ===============================
void setMinuto(Orario* orario, int minuto) {
    if (orario != NULL) {
        orario->minuti = minuto;
    } else {
        printf("Errore: Orario non valido.\n");
    }
}


// ===============================
// LETTURA DI UN ORARIO DA INPUT UTENTE
// ===============================
Orario* leggiOrario() {
    Orario* o = malloc(sizeof(Orario));
    if (o == NULL) {
        printf("Errore di allocazione memoria per Orario.\n");
        exit(1);
    }

    while (1) {
        printf("Inserisci ora di inizio (formato HH MM): ");
        if (scanf("%hd %hd", &o->ora, &o->minuti) != 2 || o->ora < 0 || o->ora > 23 || o->minuti < 0 || o->minuti > 59) {
            printf("%sOrario non valido, riprova.%s\n", ROSSO, RESET);
            while (getchar() != '\n'); // Pulisce il buffer dell'input
        } else {
            break;
        }
    }

    return o;
}

// ===============================
// CONFRONTO TRA DUE ORARI
// - Ritorna -1 se o1 < o2
// - Ritorna 0 se o1 == o2
// - Ritorna 1 se o1 > o2
// ===============================
int confrontaOrari(Orario* o1, Orario* o2) {
    if (o1->ora < o2->ora || (o1->ora == o2->ora && o1->minuti < o2->minuti)) {
        return -1;
    } else if (o1->ora == o2->ora && o1->minuti == o2->minuti) {
        return 0;
    } else {
        return 1;
    }
}

// ===============================
// AGGIUNGE MINUTI A UN ORARIO
// - Non modifica l'orario originale
// - Restituisce un nuovo oggetto Orario
// ===============================
Orario* aggiungiMinuti(Orario* o, int minuti) {
    Orario* nuovo_orario = malloc(sizeof(Orario));
    if (nuovo_orario == NULL) {
        printf("Errore di allocazione memoria per nuovo orario.\n");
        return NULL;
    }

    int totaleMinuti = o->ora * 60 + o->minuti + minuti;
    if (totaleMinuti < 0) totaleMinuti = 0; // Evita valori negativi

    // Calcolo nuovo orario
    nuovo_orario->ora = (totaleMinuti / 60) % 24; // modulo 24 per evitare overflow giorni
    nuovo_orario->minuti = totaleMinuti % 60;

    return nuovo_orario;
}

// ===============================
// STAMPA UN ORARIO FORMATTATO (HH:MM)
// ===============================
void stampaOrario(Orario* o) {
    printf("%02d:%02d\n", o->ora, o->minuti);
}