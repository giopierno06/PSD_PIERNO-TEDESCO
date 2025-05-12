#ifndef LISTA_H
#define LISTA_H

#include "abbonamento.h"  // Include la definizione della struttura abbonamento

typedef struct list list;  // Tipo opaco


// ===========================
// PROTOTIPI DELLE FUNZIONI
// ===========================


// Funzione getter per il campo 'next' del nodo
list* getNext(list* );

// Funzione setter per il campo 'next' del nodo
void setNext(list* , list* );

// Crea una nuova lista vuota (ritorna NULL)
list* newList(void);

// Restituisce 1 se la lista è vuota, 0 altrimenti
int emptyList(list* l);

// Aggiunge un nuovo nodo con valore 'val' in testa alla lista 'l'
list* consList(abbonamento* val, list* l);

// Restituisce la coda della lista, cioè tutti i nodi tranne il primo
list* tailList(list* l);

// Restituisce il valore contenuto nel primo nodo della lista
// Precondizione: la lista non deve essere vuota
abbonamento* getFirst(list* l);

// Stampa tutti gli abbonamenti nella lista su standard output
// Richiede che ci sia una funzione di stampa per 'abbonamento'
void printList(list* l);

// Libera la memoria occupata da tutti i nodi della lista (previene memory leak)
void freeList(list* l);

// Getter e setter per 'value' (abbonamento)
abbonamento* getValue( list* );

void setValue(list* l,  abbonamento* a);

//funzione per stampare a schermo la lista secondo l'oridine desiderato dal cliente
void visualizzaListaOrdinata(list*);

// Funzione per ordinare la lista degli abbonamenti in base al nome, cognome e data di iscrizione
void ordinaPerNome(list**);

//funzione per stampare a schermo la lista secondo l'oridine desiderato dal cliente
void visualizzaListaOrdinata(list*);

// Funzione per ordinare la lista degli abbonamenti in base alla data di scadenza (in ordine crescente)
void ordinaPerDataScadenza(list**);

// Funzione per ordinare la lista degli abbonati in base alla data di inizio (in ordine crescente)
void ordinaPerDataInizio(list**);

// Funzione per ordinare la lista di abbonamenti in ordine decrescente
void ordinaPerCodiceDecrescente(list**);

// Funzione per ordinare una lista collegata in ordine crescente
// in base al campo 'codice_abbonamento' presente nella struttura 'abbonamento'
void ordinaPerCodiceCrescente(list**);


#endif  // Fine della guardia di inclusione LISTA_H
