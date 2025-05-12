#ifndef LIST_LEZIONI_H
#define LIST_LEZIONI_H

#include "lezione.h"  // Include la definizione della struttura lezione

typedef struct lezione lezione;
typedef struct listL listL;  // Tipo opaco


// ===========================
// PROTOTIPI DELLE FUNZIONI
// ===========================


// Funzione getter per il campo 'next' del nodo
listL* lezione_getNext(listL* );

// Funzione setter per il campo 'next' del nodo
void lezione_setNext(listL* , listL* );

// Crea una nuova lista vuota (ritorna NULL)
listL* lezione_newList(void);

// Restituisce 1 se la lista è vuota, 0 altrimenti
int lezione_emptyList(listL* );

// Aggiunge un nuovo nodo con valore 'val' in testa alla lista 'l'
listL* lezione_consList(lezione* val, listL* );

// Restituisce la coda della lista, cioè tutti i nodi tranne il primo
listL* lezione_tailList(listL* );

// Restituisce il valore contenuto nel primo nodo della lista
// Precondizione: la lista non deve essere vuota
lezione* lezione_getFirst(listL* );

// Stampa tutte le lezioni nella lista su standard output
// Richiede che ci sia una funzione di stampa per 'lezione'
void lezione_printList(listL* );

// Libera la memoria occupata da tutti i nodi della lista (previene memory leak)
void lezione_freeList(listL* );

// Getter e setter per 'value' (lezione)
lezione* lezione_getValue(listL* );

void lezione_setValue(listL* l,  lezione*);

#endif  // Fine della guardia di inclusione LIST_LEZIONI_H