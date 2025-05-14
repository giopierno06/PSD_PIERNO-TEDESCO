#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prenotazioni.h"
#include "lezione.h"
#include "data.h"
#include "orario.h"
#include "util.h"
#include "list_Lezioni.h"
#include "list_prenotazioni.h"



// Definizione di colori per output formattato nel terminale
#define VIOLA    "\033[1;35m" 
#define ROSSO    "\033[1;31m"
#define VERDE    "\033[1;32m"
#define GIALLO   "\033[1;33m"
#define RESET    "\033[0m"     // Reset dello stile del terminale

//==============================//
//     STRUTTURA DATI BASE     //
//==============================//

struct prenotazione {
    int codice_prenotazione;  // Codice identificativo della prenotazione
    int codice_abbonamento;   // Codice identificativo univoco del cliente
    int codice_lezione;       // Codice identificativo della lezione
};

listL* prenotazioni;


/**
 * Alloca dinamicamente una nuova struttura prenotazione
 * e inizializza tutti i suoi campi a zero.
 *
 * ritorna Puntatore alla nuova struttura prenotazione,
 *         oppure NULL se l'allocazione fallisce.
 */
prenotazione* creaPrenotazione(void) {
    // Allocazione dinamica della struttura prenotazione
    prenotazione* p = malloc(sizeof(prenotazione));

    // Verifica che l'allocazione sia andata a buon fine
    if (p != NULL) {
        // Inizializzazione dei campi a valori predefiniti
        p->codice_prenotazione = 0;  // Codice identificativo della prenotazione
        p->codice_abbonamento = 0;   // Codice identificativo del cliente
        p->codice_lezione = 0;       // Codice identificativo della lezione
    }

    // Ritorna il puntatore alla struttura allocata (o NULL se errore)
    return p;
}

//==============================//
//         FUNZIONI GET         //
//==============================//

// Restituisce il codice della prenotazione
int getCodicePrenotazione(prenotazione* p) {
    if (p != NULL) {
        return p->codice_prenotazione;
    }
    return -1; // valore di errore
}

// Restituisce il codice dell'abbonamento
int prenotazione_getCodiceAbbonamento(prenotazione* p) {
    if (p != NULL) {
        return p->codice_abbonamento;
    }
    return -1; // valore di errore
}

// Restituisce il codice della lezione
int prenotazione_getCodiceLezione(prenotazione* p) {
    if (p != NULL) {
        return p->codice_lezione;
    }
    return -1; // valore di errore
}

//==============================//
//         FUNZIONI SET         //
//==============================//

// Imposta il codice della prenotazione
void prenotazione_setCodicePrenotazione(prenotazione* p, int codice) {
    if (p != NULL) {
        p->codice_prenotazione = codice;
    }
}

// Imposta il codice dell'abbonamento
void prenotazione_setCodiceAbbonamento(prenotazione* p, int codice) {
    if (p != NULL) {
        p->codice_abbonamento = codice;
    }
}

// Imposta il codice della lezione
void prenotazione_setCodiceLezione(prenotazione* p, int codice) {
    if (p != NULL) {
        p->codice_lezione = codice;
    }
}


/**
 * Libera la memoria occupata da una struttura prenotazione.
 *
 * parametri p Puntatore alla prenotazione da distruggere
 */
void prenotazione_distruggi(prenotazione* p) {
    if (p != NULL) {
        free(p);
    }
}


// Funzione che restituisce il codice lezione massimo presente nella lista
// Parametri:
//   - prenotazioni: puntatore alla lista di prenotazioni
// Restituisce:
//   - Il valore massimo del campo 'codice_prenotazione' trovato nella lista
int getMaxCodicePrenotazione(listP* prenotazioni) {
    int max = 0;  // Inizializza il massimo a 0 (presuppone codici positivi)

    // Itera finché la lista non è vuota
    while (!prenotazione_emptyList(prenotazioni)) {
        // Ottiene il primo elemento della lista (senza modificarla)
        prenotazione *p = prenotazione_getFirst(prenotazioni);

        // Usa il getter per accedere al codice prenotazione
        int codice = getCodicePrenotazione(p);

        // Se il codice corrente è maggiore del massimo, lo aggiorna
        if (codice > max) {
            max = codice;
        }

        // Passa al resto della lista (esclude il primo elemento)
        prenotazioni = prenotazione_tailList(prenotazioni);
    }

    // Ritorna il massimo codice trovato
    return max;
}


void stampaDettagliPrenotazione(prenotazione *p) {
    if (p == NULL) {
        printf("Prenotazione non disponibile.\n");
        return;
    }
   // printf("========PRENOTAZIONE====================")
    printf("Codice Prenotazione: %d\n", p->codice_prenotazione);

    printf("Codice Abbonamento: %d\n", p->codice_abbonamento);
    printf("Codice Lezione: %d\n", p->codice_lezione);
}