#ifndef PRENOTAZIONI_H
#define PRENOTAZIONI_H

typedef struct prenotazione prenotazione;
#include "list_prenotazioni.h"

//==============================//
//     PROTOTIPI FUNZIONI GET   //
//==============================//

prenotazione* creaPrenotazione(void);

/**
 * Restituisce il codice della prenotazione.
 *   p Puntatore alla prenotazione
 *  Il codice della prenotazione, oppure -1 in caso di errore
 */
int prenotazione_getCodicePrenotazione(prenotazione*);

/**
 * Restituisce il codice dell'abbonamento.
 *   p Puntatore alla prenotazione
 *  Il codice dell'abbonamento, oppure -1 in caso di errore
 */
int prenotazione_getCodiceAbbonamento(prenotazione*);

/**
 * Restituisce il codice della lezione.
 *   p Puntatore alla prenotazione
 *  Il codice della lezione, oppure -1 in caso di errore
 */
int prenotazione_getCodiceLezione(prenotazione*);

//==============================//
//     PROTOTIPI FUNZIONI SET   //
//==============================//

/**
 * Imposta il codice della prenotazione.
 *   p Puntatore alla prenotazione
 *   codice Il codice della prenotazione da impostare
 */
void prenotazione_setCodicePrenotazione(prenotazione*, int);

/**
 * Imposta il codice dell'abbonamento.
 *   p Puntatore alla prenotazione
 *   codice Il codice dell'abbonamento da impostare
 */
void prenotazione_setCodiceAbbonamento(prenotazione*, int);

/**
 * Imposta il codice della lezione.
    p Puntatore alla prenotazione
 *   codice Il codice della lezione da impostare
 */
void prenotazione_setCodiceLezione(prenotazione*, int);

//==============================//
//     FUNZIONE DI DISTRUZIONE   //
//==============================//

/**
 * Libera la memoria occupata da una struttura prenotazione.
 *   p Puntatore alla prenotazione da distruggere
 */
void prenotazione_distruggi(prenotazione*);

// Funzione che restituisce il codice lezione massimo presente nella lista
int getMaxCodicePrenotazione(listP* );

void stampaDettagliPrenotazione(prenotazione *);


#endif // PRENOTAZIONE_H
