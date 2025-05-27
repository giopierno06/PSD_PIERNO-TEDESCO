#ifndef GESTIONE_LEZIONI_H
#define GESTIONE_LEZIONI_H

#include "list_Lezioni.h"

// Funzione principale per gestire il menu e le operazioni sulle lezioni
void gestione_Lezioni();

// Aggiunge una nuova lezione alla lista puntata da listL**
// Ritorna 0 se l'inserimento è andato a buon fine, 1 in caso di anomalie o errori
int aggiungi_lezione(listL**);

// Funzione per visualizzare le lezioni, menu interattivo che permette di scegliere
// se visualizzare per data, tutte, per ID o per disciplina
void visualizzaLezioni(listL*);

// Stampa tutte le lezioni di una data disciplina inserita dall'utente
void lezione_printByDisciplina(listL*);

// Stampa la lezione che corrisponde all'ID passato come parametro
void lezione_printByID(listL*, int);

// Stampa le lezioni per una data specifica passata come parametro
void lezione_printByDate(listL*, Data*);

// Verifica se nella lista esiste una lezione con un certo ID e data
// Ritorna 1 se trovata, 0 altrimenti
int lezione_checkByID(listL*, int, Data*);

// Cerca e ritorna un puntatore alla lezione con l'ID specificato nella lista
// Ritorna NULL se non trovata
lezione* trovaLezionePerID(listL*, int);

// Modifica una lezione trovata nella lista, richiede interazione con l'utente
listL* modificaLezione(listL*);

// Elimina una lezione dalla lista (probabilmente interattivo o su base criteri)
listL* eliminaLezione(listL*);

// Elimina una lezione dalla lista tramite ID (interattivo)
listL* eliminaLezionePerID(listL*);

#endif
