#ifndef GESTIONE_ABBONAMENTI_H
#define GESTIONE_ABBONAMENTI_H

#include "list.h"

// =========================
// PROTOTIPI DI FUNZIONE
// =========================


// Funzione principale per la gestione degli abbonamenti (mostra un menu con operazioni come aggiunta, eliminazione, ecc.)
void gestione_abbonamenti(void);


// Aggiunge un nuovo abbonamento alla lista (inserimento da tastiera)
// Ritorna 0 se l'inserimento ha successo, 1 in caso di errore o anomalia
int aggiungi_abbonamento(list**);

// Elimina un abbonamento dalla lista
// Ritorna la nuova lista con l'abbonamento eventualmente rimosso
list* eliminaAbbonamento(list*);

// Funzione per eliminare un abbonamento per ID
list* eliminaPerID(list* );

// Funzione per eliminare un abbonamento per nome e cognome
list* eliminaPerNomeCognome(list* );

// Funzione di ricerca abbonamento
list* ricercaAbbonamento(list* );

// Funzione di ricerca abbonamento per modificarlo
list* ricercaEModificaAbbonamento(list* );

// Funzione per modificare abbonamento
list* modificaAbbonamento(list*,list*);

//interfaccia in cui il clente potrà vedere i propri dati
void controllo_abbonamento(abbonamento *);


#endif