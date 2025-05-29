#ifndef MENU_H
#define MENU_H

#include "list.h"

// Mostra il menu iniziale dell'applicazione, da cui si può scegliere tra menu gestore o cliente
void menu_iniziale();

// Mostra il menu destinato all'utente gestore per la gestione abbonamenti, lezioni e prenotazioni
void menu_gestore();

// Mostra il menu destinato all'utente cliente, che consente di gestire il proprio abbonamento e prenotazioni
void menu_cliente();

// Funzione che controlla l'identità del cliente richiedendo nome, cognome e codice abbonamento,
// e restituisce un puntatore al nodo della lista clienti se riconosciuto, altrimenti NULL.
list* controlloCliente();

#endif // MENU_H
