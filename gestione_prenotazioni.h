#ifndef GESTIONE_PRENOTAZIONI_H
#define GESTIONE_PRENOTAZIONI_H

#include "prenotazioni.h"
#include "list_prenotazioni.h"
#include "list.h"
#include "list_Lezioni.h"


void gestione_prenotazioni();

void aggiungi_prenotazione();

void disdici_prenotazione();

void visualizzaPrenotazioni(listP*, list*);

void visualizzaPrenotazioniLezione(int, listP* , list*);

void aggiungi_prenotazioneCliente(abbonamento*); 

void visualizzaPrenotazioniCliente(int, listP*, listL*);

void reportLezioniUltimoMeseCliente(int ,listP* , listL*);

void reportDisciplineUltimoMese(listP* , listL* );

void reportGestore(listP*, listL*);

#endif