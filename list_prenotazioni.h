#ifndef LIST_PRENOTAZIONI_H
#define LIST_PRENOTAZIONI_H

typedef struct listP listP;
#include "prenotazioni.h"

// ===========================
// PROTOTIPI DELLE FUNZIONI
// ===========================


// Funzione getter per il campo 'next' del nodo
listP* prenotazione_getNext(listP* );

// Funzione setter per il campo 'next' del nodo
void prenotazione_setNext(listP* , listP* );

// Crea una nuova lista vuota (ritorna NULL)
listP* prenotazione_newList(void);

// Restituisce 1 se la lista è vuota, 0 altrimenti
int prenotazione_emptyList(listP* );

// Aggiunge un nuovo nodo con valore 'val' in testa alla lista 'l'
listP* prenotazione_consList(prenotazione*, listP* );

// Restituisce la coda della lista, cioè tutti i nodi tranne il primo
// Restituisce il valore contenuto nel primo nodo della lista
// Precondizione: la lista non deve essere vuota
prenotazione* prenotazione_getFirst(listP*);

// Stampa tutti gli abbonamenti nella lista su standard output
// Richiede che ci sia una funzione di stampa per 'prenotazione'
void prenotazione_printList(listP* );

// Funzione per ottenere la coda della lista (tutto tranne il primo elemento)
listP* prenotazione_tailList(listP* ); 

// Libera la memoria occupata da tutti i nodi della lista (previene memory leak)
void prenotazione_freeList(listP* );

// Getter e setter per 'value' (prenotazione)
prenotazione* prenotazione_getValue( listP* );

void prenotazione_setValue(listP* l,  prenotazione* a);

#endif