#ifndef GESTIONE_PRENOTAZIONI_H
#define GESTIONE_PRENOTAZIONI_H

#include "prenotazioni.h"      // Definizione della struttura e funzioni relative alla singola prenotazione
#include "list_prenotazioni.h" // Gestione della lista di prenotazioni
#include "list.h"              // Lista generica, probabilmente usata per altri tipi di dati
#include "list_Lezioni.h"      // Gestione della lista delle lezioni

// Funzione principale per gestire il flusso delle prenotazioni (aggiunta, cancellazione, visualizzazione)
void gestione_prenotazioni();

// Aggiunge una nuova prenotazione, interagendo con l'utente o dati di input
void aggiungi_prenotazione(int,int);

// Disdice (cancella) una prenotazione esistente
void disdici_prenotazione();

// Visualizza tutte le prenotazioni presenti, riceve una lista di prenotazioni e una lista generica (probabilmente di clienti o lezioni)
void visualizzaPrenotazioni(listP*, list*);

// Visualizza tutte le prenotazioni associate a una specifica lezione identificata da un int (id), con le liste di prenotazioni e generica
void visualizzaPrenotazioniLezione(int, listP*, list*);

// Aggiunge una prenotazione per un cliente in possesso di un abbonamento
void aggiungi_prenotazioneCliente(abbonamento*); 

// Visualizza tutte le prenotazioni di un cliente identificato da un int (id), utilizzando le liste di prenotazioni e lezioni
void visualizzaPrenotazioniCliente(int, listP*, listL*);

// Genera un report delle lezioni frequentate da un cliente nell’ultimo mese
void reportLezioniUltimoMeseCliente(int, listP*, listL*);

// Genera un report delle discipline frequentate nell’ultimo mese, usando le liste di prenotazioni e lezioni
void reportDisciplineUltimoMese(listP*, listL*);

// Genera un report generale per il gestore (amministratore) con dati aggregati di prenotazioni e lezioni
void reportGestore(listP*, listL*);

// Cancella una prenotazione per un cliente con abbonamento
void disdici_prenotazioneCliente(abbonamento*);

#endif