#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>



// Struttura privata visibile solo in questo file
struct Data {
    int giorno;
    int mese;
    int anno;
};

/// ==========================
/// Funzioni di utilità interne
/// ==========================

/**
 * Restituisce il numero di giorni nel mese specificato.
 * Considera l'anno bisestile per il mese di febbraio.
 */
static int giorniNelMese(int mese, int anno) {
    int giorni[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Febbraio in anno bisestile
    if (mese == 2 && ((anno % 4 == 0 && anno % 100 != 0) || (anno % 400 == 0))) {
        return 29;
    }

    return giorni[mese - 1];
}

/// ==========================
/// Funzioni di validazione
/// ==========================

/**
 * Controlla se una data è valida.
 * Verifica l'intervallo di giorno, mese e anno.
 */
int dataValida(Data d) {
    if (d.anno < 1900 || d.anno > 2100) return 0;
    if (d.mese < 1 || d.mese > 12) return 0;
    if (d.giorno < 1 || d.giorno > giorniNelMese(d.mese, d.anno)) return 0;
    return 1;
}

/// ==========================
/// Costruttori e accessori
/// ==========================

//alloca dinamicamente una nuova data
Data* newData(void) {
    return malloc(sizeof(Data));
}


/**
 * Crea una struttura Data a partire da giorno, mese e anno.
 */
Data* creaData(int giorno, int mese, int anno) {
    // Alloca dinamicamente la memoria per la struttura Data
    Data* d = malloc(sizeof(Data));

    // Verifica che l'allocazione sia andata a buon fine
    if (d == NULL) {
        return NULL;  // Restituisci NULL in caso di errore di allocazione
    }

    // Assegna i valori ai membri della struttura
    d->giorno = giorno;
    d->mese = mese;
    d->anno = anno;

    // Restituisci il puntatore alla struttura Data
    return d;
}


/*
 * Imposta il giorno della data.
 */
void setGiorno(Data* d, int giorno) {
    if (d != NULL) {
        d->giorno = giorno;
    }
}

/**
 * Imposta il mese della data.
 */
void setMese(Data* d, int mese) {
    if (d != NULL) {
        d->mese = mese;
    }
}

/**
 * Imposta l'anno della data.
 */
void setAnno(Data* d, int anno) {
    if (d != NULL) {
        d->anno = anno;
    }
}




/**
 * Restituisce il giorno della data.
 */
int getGiorno(Data* d) {
    return d->giorno;
}

/**
 * Restituisce il mese della data.
 */
int getMese(Data* d) {
    return d->mese;
}

/**
 * Restituisce l'anno della data.
 */
int getAnno(Data* d) {
    return d->anno;
}


// Funzione per liberare la memoria di una data
void distruggiData(Data* d) {
    free(d);
}
/// ==========================
/// Input/Output
/// ==========================


/**
 * Legge una data da input standard finché non è valida.
 */
Data* leggiData() {
    Data* d = malloc(sizeof(Data));  // Alloca dinamicamente la memoria per Data
    if (d == NULL) {
        printf("Errore nell'allocazione della memoria.\n");
        return NULL;
    }

    while (1) {
        printf("Inserisci data (gg mm aaaa): ");
        
        if (scanf("%d %d %d", &d->giorno, &d->mese, &d->anno) != 3) {
            // Pulisce il buffer in caso di input non valido
            while (getchar() != '\n');
            printf("Input non valido. Riprova.\n");
            continue;
        }

        if (!dataValida(*d)) {
            printf("Data non valida. Riprova.\n");
        } else {
            break;
        }
    }
    return d;
}

/**
 * Stampa una data nel formato gg/mm/aaaa.
 */
void stampaData(Data*d) {
    printf("%02d/%02d/%04d", d->giorno, d->mese, d->anno);
}

/// ==========================
/// Confronto tra date
/// ==========================

/**
 * Confronta due date.
 * Restituisce -1 se d1 < d2, 1 se d1 > d2, 0 se uguali.
 */
int confrontaDate(Data* d1, Data* d2) {
    if (d1->anno != d2->anno) return d1->anno < d2->anno ? -1 : 1;
    if (d1->mese != d2->mese) return d1->mese < d2->mese ? -1 : 1;
    if (d1->giorno != d2->giorno) return d1->giorno < d2->giorno ? -1 : 1;
    return 0;
}

/// ==========================
/// Manipolazione di date
/// ==========================

/**
 * Aggiunge un certo numero di mesi a una data.
 * Se il giorno risultante non è valido nel nuovo mese, viene corretto.
 * La funzione restituisce una nuova Data con la data modificata.
 */
Data* aggiungiMesi(Data* d, int mesi) {
    // Alloca memoria per la nuova data
    Data* nuovaData = malloc(sizeof(Data));
    if (nuovaData == NULL) {
        return NULL;  // Errore di allocazione memoria
    }

    // Copia la data originale nella nuova data
    *nuovaData = *d;

    // Aggiungi mesi alla data
    nuovaData->mese += mesi;

    // Gestisci l'overflow (mesi > 12) o l'underflow (mesi < 1)
    while (nuovaData->mese > 12) {
        nuovaData->mese -= 12;
        nuovaData->anno++;
    }
    while (nuovaData->mese < 1) {
        nuovaData->mese += 12;
        nuovaData->anno--;
    }

    // Ottieni il numero massimo di giorni nel mese risultante
    int gMax = giorniNelMese(nuovaData->mese, nuovaData->anno);
    
    // Se il giorno della data supera il numero massimo di giorni, lo correggi
    if (nuovaData->giorno > gMax) {
        nuovaData->giorno = gMax;  // Imposta il giorno massimo possibile nel mese
    }

    // Ritorna la nuova data modificata
    return nuovaData;
}

//calcola la data odierna
Data* oggi() {
    Data* d = (Data*)malloc(sizeof(Data));  // Alloca memoria dinamicamente
    if (d == NULL) {
        return NULL;  // Gestione errore se la memoria non può essere allocata
    }

    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    d->giorno = tm_info->tm_mday;
    d->mese = tm_info->tm_mon + 1;
    d->anno = tm_info->tm_year + 1900;

    return d;
}

/**
 * Sottrae un numero di giorni da una data e restituisce la nuova data.
 *
 *  data La data di partenza.
 *  giorni Il numero di giorni da sottrarre.
 * return Una nuova struttura Data con la data risultante.
 */
Data* sottraiGiorni(Data*data, int giorni) {
    Data* risultato = data;

    while (giorni > 0) {
        risultato->giorno--;

        // Se il giorno scende sotto 1, si passa al mese precedente
        if (risultato->giorno < 1) {
            risultato->mese--;

            // Se il mese scende sotto 1, si passa all'anno precedente
            if (risultato->mese < 1) {
                risultato->mese = 12;
                risultato->anno--;
            }

            // Aggiorna il giorno all'ultimo del mese precedente
            risultato->giorno = giorniNelMese(risultato->mese, risultato->anno);
        }

        giorni--;
    }

    return risultato;
}
