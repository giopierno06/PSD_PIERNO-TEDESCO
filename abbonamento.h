#ifndef ABBONAMENTO_H
#define ABBONAMENTO_H

#include "data.h"     // Include la struttura 'Data' usata nella definizione dell'abbonamento

// =========================
// DICHIARAZIONE ANTICIPATA
// =========================

// Dichiarazione anticipata della struct list per evitare dipendenze circolari
struct list;
typedef struct list list;  // Alias per struct list

// =========================
// DEFINIZIONE STRUTTURA ABBONAMENTO
// =========================

// Definizione opaca di abbonamento
typedef struct abbonamento abbonamento;

// Dichiarazione del puntatore vuoto
extern abbonamento* vuoto;

// Macro per restituire NULLITEM
#define NULLITEM (vuoto)


// =========================
// GET E SET
// =========================

// Restituisce il codice identificativo dell'abbonamento
int getCodiceAbbonamento(const abbonamento* a);

// Restituisce il nome del cliente nell'abbonamento
const char* getNome(const abbonamento* a);

// Restituisce il cognome del cliente nell'abbonamento
const char* getCognome(const abbonamento* a);

// Restituisce la data di inizio dell'abbonamento
Data * getDataInizio(const abbonamento* a);

// Restituisce la durata dell'abbonamento in mesi
int getDurata(const abbonamento* a);


// Funzioni setter per modificare i campi della struct abbonamento

// Imposta il codice identificativo dell'abbonamento
void setCodiceAbbonamento(abbonamento* a, int codice);

// Imposta il nome del cliente nell'abbonamento
void setNome(abbonamento* a, const char* nome);

// Imposta il cognome del cliente nell'abbonamento
void setCognome(abbonamento* a, const char* cognome);

// Imposta la data di inizio dell'abbonamento
void setDataInizio(abbonamento* a, Data* data);

// Imposta la durata in mesi dell'abbonamento
void setDurata(abbonamento* a, int durata);




// =========================
// PROTOTIPI DI FUNZIONE
// =========================

abbonamento* creaAbbonamento();

// Funzione che restituisce il codice abbonamento massimo presente nella lista
int getMaxCodiceAbbonamento(list*);

// Funzione per stampare tutti i dettagli di un abbonamento
void stampaDettagliAbbonamento(abbonamento* );

void distruggiAbbonamento(abbonamento*);



// =========================
// COSTANTE DI ABBONAMENTO NULLO
// =========================


#endif  // Fine della guardia di inclusione