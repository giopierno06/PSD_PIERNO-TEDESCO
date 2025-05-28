#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prenotazioni.h"
#include "abbonamento.h"
#include "lezione.h"
#include "data.h"
#include "orario.h"
#include "util.h"
#include "file.h"
#include "list.h"
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

listP* lista_prenotazioni=NULL;
extern list* abbonati;
extern listL* lezioni;


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
int getMaxCodicePrenotazione(listP* prenotazionii) {
    int max = 0;  // Inizializza il massimo a 0 (presuppone codici positivi)

    // Itera finché la lista non è vuota
    while (!prenotazione_emptyList(prenotazionii)) {
        // Ottiene il primo elemento della lista (senza modificarla)
        prenotazione *p = prenotazione_getFirst(prenotazionii);

        // Usa il getter per accedere al codice prenotazione
        int codice = getCodicePrenotazione(p);

        // Se il codice corrente è maggiore del massimo, lo aggiorna
        if (codice > max) {
            max = codice;
        }

        // Passa al resto della lista (esclude il primo elemento)
        prenotazionii = prenotazione_tailList(prenotazionii);
    }

    // Ritorna il massimo codice trovato
    return max;
}


/**
 * brief Stampa i dettagli di una prenotazione, inclusi dati dell'abbonato e della lezione.
 *
 * param p Puntatore alla struttura prenotazione da stampare.
 *
 * pre
 * - Il puntatore `p` può essere NULL. In tal caso, la funzione stampa un messaggio e termina.
 * - Le strutture globali `abbonati` e `lezioni` devono essere inizializzate e contenere gli elementi corrispondenti agli ID presenti in `p`.
 *
 * post
 * - Se `p` è valido e le strutture `abbonati` e `lezioni` contengono gli elementi richiesti, stampa una riga con i dettagli della prenotazione.
 * - Se `p` è NULL o i dati associati non sono trovati, stampa un messaggio di errore.
 *
 * note
 * - La funzione non modifica alcuna struttura dati.
 * - Utilizza le funzioni ausiliarie `getAbbonamentoByID`, `getLezioneByID`, `getNome`, `getCognome`, `getDisciplinaLezione`, `getDataLezione`, `getOrarioLezione`, `stampaData` e `stampaOrario`.
 */
void stampaDettagliPrenotazione(prenotazione* p) {
    if (p == NULL) {
        printf("Prenotazione non disponibile.\n");
        return;
    }

    abbonamento* a = getAbbonamentoByID(abbonati, p->codice_abbonamento);
    lezione* l = getLezioneByID(lezioni, p->codice_lezione);

    if (a == NULL || l == NULL) {
        printf("Errore: dati abbonamento o lezione mancanti.\n");
        return;
    }

    // Stampa una sola riga con tutti i dati
    printf("%-20d %-10d %-15s %-15s %-10d %-15s ",
           p->codice_prenotazione,
           p->codice_abbonamento,
           getNome(a),
           getCognome(a),
           p->codice_lezione,
           getDisciplinaLezione(l));

    // Stampa la data in formato gg/mm/aaaa
    stampaData(getDataLezione(l));
    printf(" ");

    // Stampa l’orario in formato hh:mm
    stampaOrario(getOrarioLezione(l));

    printf("\n-------------------------------------------------------------------------------------------------------------\n");
}

/**
 * Verifica se esiste già una prenotazione per uno specifico abbonamento, lezione e data.
 *
 * param lista Lista delle prenotazioni.
 * param lezioni Lista delle lezioni.
 * param codice_abbonamento Codice dell’abbonamento da cercare.
 * param codice_lezione Codice della lezione da cercare.
 * param data Data da confrontare con quella della lezione.
 *
 * pre lista, lezioni e data devono essere validi (non NULL).
 * post Restituisce 1 se esiste una prenotazione corrispondente, altrimenti 0.
 */
int prenotazione_esistePerAbbonamentoELezione(listP* lista, listL* lezioni, int codice_abbonamento, int codice_lezione, Data* data) {
    if (!lista || !data || !lezioni) return 0;

    listP* nodo = lista;

    while (nodo != NULL) {
        prenotazione* p = (prenotazione*) prenotazione_getValue(nodo); 

        if (p != NULL) {
            int codice_ab = p->codice_abbonamento;
            int codice_lez = p->codice_lezione;

            if (codice_ab == codice_abbonamento && codice_lez == codice_lezione) {
                lezione* l = getLezioneByID(lezioni, codice_lez);
                if (l != NULL) {
                    Data* data_lezione = getDataLezione(l);
                    if (confrontaDate(data_lezione, data) == 0) {
                        return 1; // Prenotazione già esistente
                    }
                }
            }
        }

        nodo = prenotazione_getNext(nodo);
    }

    return 0; // Nessuna prenotazione trovata
}