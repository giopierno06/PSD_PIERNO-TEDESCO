#ifndef GESTIONE_LEZIONI_H
#define GESTIONE_LEZIONI_H

#include "list_Lezioni.h"



void gestione_Lezioni();
int aggiungi_lezione(listL**);//nella funzione verranno inseirti da tastiera i dati della lezione e verranno aggiunti alla lista passata come parametro.Tornera 0 se verrà tutto ben eseguito 1 sen trova anomalie

// Funzione per visualizzare le lezioni di un giorno specifico (data)
void visualizzaLezioni(listL* lezioni);

// Funzione per stampare le lezioni di un giorno specifico (data)
void lezione_printByDate(listL*, Data* );



#endif