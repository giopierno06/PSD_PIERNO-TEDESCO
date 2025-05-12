#include <stdio.h>
#include <time.h>
#include "orario.h"
#include <stdlib.h>


#define VIOLA    "\033[1;35m" 
#define ROSSO    "\033[1;31m"
#define VERDE    "\033[1;32m"
#define GIALLO   "\033[1;33m"
#define RESET    "\033[0m"     //comandi per stampare a video defini come define per rendere il tutto più ordinato








// Funzione per creare una nuova istanza di Orario
Orario* newOrario() {
    Orario* orario = (Orario*)malloc(sizeof(Orario));
    if (orario != NULL) {
        // Imposta valori di default per ora e minuto (ad esempio 0)
        orario->ora = 0;
        orario->minuti = 0;
    } else {
        printf("Errore nell'allocazione della memoria per Orario.\n");
    }
    return orario;
}

int getOra(const Orario* orario) {
    return orario->ora;
}

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


int getMinuto(const Orario* orario) {
    return orario->minuti;
}


// Funzione setter per l'ora
void setOra(Orario* orario, int ora) {
    if (orario != NULL) {
        orario->ora = ora;
    } else {
        printf("Errore: Orario non valido.\n");
    }
}


// Funzione setter per il minuto
void setMinuto(Orario* orario, int minuto) {
    if (orario != NULL) {
        orario->minuti = minuto;
    } else {
        printf("Errore: Orario non valido.\n");
    }
}




void stampaOrarioAttuale() {
    time_t tempo_attuale;
    struct tm *orario_locale;

    // Ottieni il tempo corrente
    time(&tempo_attuale);

    // Convertilo in orario locale
    orario_locale = localtime(&tempo_attuale);

    printf(" %02d:%02d\n", 
           orario_locale->tm_hour, 
           orario_locale->tm_min
);
}


Orario* leggiOrario() {
    Orario* o = malloc(sizeof(Orario));
    if (o == NULL) {
        printf("Errore di allocazione memoria per Orario.\n");
        exit(1);  // o gestisci l’errore in altro modo
    }

    while (1) {
        printf("Inserisci ora di inizio (formato HH MM): ");
        if (scanf("%hd %hd", &o->ora, &o->minuti) != 2 || o->ora < 0 || o->ora > 23 || o->minuti < 0 || o->minuti > 59) {
            printf("%sOrario non valido, riprova.%s\n", ROSSO, RESET);
            while (getchar() != '\n'); // pulisci buffer
        } else {
            break;
        }
    }

    return o;
}

// Funzione per confrontare due orari
// Ritorna: -1 se o1 < o2, 0 se o1 == o2, 1 se o1 > o2
int confrontaOrari(Orario* o1, Orario* o2) {
    if (o1->ora < o2->ora || (o1->ora == o2->ora && o1->minuti < o2->minuti)) {
        return -1;
    } else if (o1->ora == o2->ora && o1->minuti == o2->minuti) {
        return 0;
    } else {
        return 1;
    }
}

// Funzione per aggiungere minuti a un orario
Orario* aggiungiMinuti(Orario*o, int minuti) {
    int totaleMinuti = o->ora * 60 + o->minuti + minuti;

    // Gestisce overflow di minuti
    if (totaleMinuti < 0) totaleMinuti = 0;  // Evita orari negativi

    o->ora = (totaleMinuti / 60) % 24;  // Modulo 24 per gestire passaggi di giorno
    o->minuti = totaleMinuti % 60;

    return o;  // Restituisce una nuova copia dell'orario modificato
}

// Funzione per stampare un orario (opzionale)
void stampaOrario(Orario* o) {
    printf("%02d:%02d\n", o->ora, o->minuti);
}

