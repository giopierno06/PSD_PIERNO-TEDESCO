#ifndef GESTIONE_PRENOTAZIONI_H
#define GESTIONE_PRENOTAZIONI_H

#include "prenotazioni.h"
#include "list_prenotazioni.h"
#include "list.h"
#include "list_Lezioni.h"


void gestione_prenotazioni();
void aggiungi_prenotazione();

void visualizzaPrenotazioni(listP*, list*);

void visualizzaPrenotazioniLezione(int, listP* , list*);

#endif