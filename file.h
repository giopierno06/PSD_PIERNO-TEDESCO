#ifndef FILE_H
#define FILE_H

#include "list.h"
#include "list_Lezioni.h"
#include "list_prenotazioni.h"


// Funzione per caricare gli abbonamenti da un file
list* caricaAbbonamentiDaFile(list* l, const char* nome_file);

// Funzione per salvare gli abbonamenti su un file
void salvaAbbonamentiSuFile(list* l, const char* nome_file);


// Funzione per caricare le lezioni da un file
listL* caricaLezioniDaFile(listL* l, const char* nome_file);

// Funzione per salvare le lezioni su un file
void salvaLezioniSuFile(listL* l, const char* nome_file);


listP* caricaPrenotazioniDaFile(listP* lista, const char* nome_file);

void salvaPrenotazioniSuFile(listP* lista, const char* nome_file);

#endif