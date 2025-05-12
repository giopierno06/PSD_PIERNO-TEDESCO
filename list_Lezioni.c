#include "list_Lezioni.h"
#include "lezione.h"
#include "data.h"
#include "util.h"
#include <stdio.h>
#include <stdlib.h>


#define VIOLA    "\033[1;35m" 
#define ROSSO    "\033[1;31m"
#define VERDE    "\033[1;32m"
#define GIALLO   "\033[1;33m"
#define RESET    "\033[0m"     //comandi per stampare a video defini come define per rendere il tutto più ordinato


// Definizione del nodo della lista (tipo opaco)
struct listL {
    lezione* value;  // Puntatore al valore (lezione)
    listL* next;     // Puntatore al prossimo nodo della lista
};

// Funzione getter per il campo 'next' del nodo
listL* lezione_getNext(listL* l) {
    return l->next;
}

// Funzione setter per il campo 'next' del nodo
void lezione_setNext(listL* l, listL* next) {
    l->next = next;
}

listL* lezione_newList() {
    listL* nuova_lista = malloc(sizeof(listL));
    if (nuova_lista == NULL) return NULL;

    nuova_lista->value = NULL;  // Nessuna lezione ancora
    nuova_lista->next = NULL;    // Fine lista
    return nuova_lista;
}

// Restituisce 1 se la lista è vuota, 0 altrimenti
int lezione_emptyList(listL* l) {
    return (l == NULL);
}

// Funzione per aggiungere una nuova lezione alla lista
listL* lezione_consList(lezione* val, listL* next) {
    listL* nuovo_nodo = malloc(sizeof(listL));
    if (!nuovo_nodo) return NULL;
    nuovo_nodo->value = val;
    nuovo_nodo->next = next;
    return nuovo_nodo;
}

// Restituisce la coda della lista, cioè tutti i nodi tranne il primo
listL* lezione_tailList(listL* l) {
    if (l == NULL) return NULL;
    return l->next;
}

// Restituisce il valore contenuto nel primo nodo della lista
// Precondizione: la lista non deve essere vuota
lezione* lezione_getFirst(listL* l) {
    if (l == NULL) {
        fprintf(stderr, "Errore: lista vuota!\n");
        exit(1);
    }
    return l->value;
}

// Stampa tutte le lezioni nella lista su standard output
void lezione_printList(listL* l) {
    if(l==NULL)
    printf("%sLISTA VUOTA%s \n",ROSSO,RESET);
    
    while (l != NULL) {
        printf("Codice Lezione: %d\n", getCodiceLezione(l->value));
        printf("Disciplina: %s\n", getDisciplinaLezione(l->value));
        printf("Nome: %s\n", getNomeLezione(l->value));
        printf("Data: ");
        stampaData(getDataLezione(l->value));
        printf("\nOrario di inizio: ");
        stampaOrario(getOrarioLezione(l->value));
        printf("Durata: %d minuti\n", getDurataLezione(l->value));
        printf("Posti Occupati: %d\n", getPostiOccupati(l->value));
        printf("Posti Max: %d\n\n", getPostiMax(l->value));
        l = l->next;
    }
}

// Libera la memoria occupata da tutti i nodi della lista
void lezione_freeList(listL* l) {
    while (l != NULL) {
        listL* temp = l;
        l = l->next;
        free(temp);
    }
}

// Getter e setter per 'value' (lezione)
lezione* lezione_getValue(listL* l) {
    return l->value;
}

void lezione_setValue(listL* l, lezione* a) {
    l->value = a;
}