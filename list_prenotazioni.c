#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prenotazioni.h"
#include "lezione.h"
#include "data.h"
#include "orario.h"
#include "util.h"
#include "list_Lezioni.h"
#include "gestione_prenotazioni.h"
#include "list_prenotazioni.h"

// Struttura lista (opaca, non visibile all'esterno)
struct listP {
    prenotazione* value;    // Valore contenuto nel nodo, di tipo 'prenotazione'
    struct listP *next;    // Puntatore al nodo successivo (NULL se è l'ultimo)
};  // Fine definizione struttura listP


#define VIOLA    "\033[1;35m" 
#define ROSSO    "\033[1;31m"
#define VERDE    "\033[1;32m"
#define GIALLO   "\033[1;33m"
#define BLU   "\033[34m"
#define RESET    "\033[0m"     //comandi per stampare a video defini come define per rendere il tutto più ordinato


// Funzione getter per il campo 'next' del nodo
// Parametri:
//   - nodo: il nodo corrente della lista
// Restituisce:
//   - Il prossimo nodo nella lista
listP* prenotazione_getNext(listP* nodo) {
    return nodo->next;  // Restituisce il prossimo nodo collegato
}


// Funzione setter per il campo 'next' del nodo
// Parametri:
//   - nodo: il nodo corrente della lista
//   - next: il prossimo nodo che deve essere collegato
void prenotazione_setNext(listP* nodo, listP* next) {
    nodo->next = next;  // Imposta il prossimo nodo collegato
}


// Funzione per creare una nuova lista vuota
listP* prenotazione_newList(void) {
    return NULL;  // Restituisce NULL, che rappresenta una lista vuota
}

// Funzione per verificare se la lista è vuota
int prenotazione_emptyList(listP* l) {
    return l == NULL;  // La lista è vuota se è NULL
}

// Funzione per aggiungere un nuovo elemento in testa alla lista
listP* prenotazione_consList(prenotazione* val, listP* l) {
    listP* nuovo = malloc(sizeof(listP));  // Allocazione della memoria per il nuovo nodo
    if (nuovo != NULL) {
        nuovo->value = val;  // Assegna l'prenotazione al nuovo nodo
        nuovo->next = l;     // Collega il nuovo nodo alla lista esistente
    }
    return nuovo;  // Restituisce la lista con il nuovo nodo aggiunto
}


// Funzione per ottenere la coda della lista (tutto tranne il primo elemento)
listP* prenotazione_tailList(listP* l) {
    if (l != NULL) {
        return l->next;  // Restituisce il successivo nodo (la coda)
    }
    return NULL;  // Se la lista è vuota, restituisce NULL
}

//restiutisce l'elemnto in cima
prenotazione* prenotazione_getFirst(listP* l) {
    if (l != NULL) 
        return l->value;  // Restituisce un puntatore al valore del primo nodo (l'abbonamento)
    
    else 
        return NULL;  // Se la lista è vuota, restituisce un abbonamento "vuoto" (un puntatore)
}


//Funcione per stampare la lista
void prenotazione_printList(listP* l) {

    // Controlla se la lista è vuota
    if (l == NULL) {
        printf("%sLa lista delle prenotazioni è vuota.%s\n",ROSSO,RESET);
        return;  // Esce dalla funzione se la lista è vuota
    }
     printf("\n%sElenco completo delle prenotazioni:\n",BLU);
    printf("-------------------------------------------------------------------------------------------------------------\n");
    printf("%-20s %-10s %-15s %-15s %-10s %-15s %-10s %-10s\n",
           "Codice Prenotazione", "ID Utente", "Nome", "Cognome", "Lezione", "Disciplina", "Data", "Orario");
    printf("-------------------------------------------------------------------------------------------------------------%s\n",RESET);
    // Altrimenti, stampa tutti gli abbonamenti
    while (l != NULL) {
        // Stampa dei dettagli della prenotazione
        // Funzione per stampare tutti i dettagli di una prenotazione
         stampaDettagliPrenotazione(l->value); 
        l = l->next;  // Passa al nodo successivo}
    printf("\n");
}}

// Funzione per liberare la memoria della lista
void prenotazione_freeList(listP* l) {
    listP* temp;
    while (l != NULL) {
        temp = l;
        l = l->next;
        free(temp);  // Libera la memoria del nodo
    }
}


// Getter e setter per 'value' (prenotazione)
prenotazione* prenotazione_getValue(listP* l) {
    return l->value;  // Restituisce un puntatore a 'value'
}

void prenotazione_setValue(listP* l,  prenotazione* a) {
    l->value = a;  // Imposta il valore della prenotazione
}