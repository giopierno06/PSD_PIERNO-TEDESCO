#ifndef LEZIONE_H
#define LEZIONE_H
#include "list_Lezioni.h"
#include "data.h"
#include "orario.h"

// Tipo opaco per nascondere l'implementazione
typedef struct lezione lezione;

typedef struct listL listL;


//========================
// Costruttore / Distruttore
//========================

/*
 * Alloca e inizializza una nuova lezione.
 * return Puntatore a una nuova istanza di lezione.
 */
lezione* creaLezione();

/*
 * Libera la memoria associata a una lezione.
 * param Puntatore alla lezione da distruggere.
 */
void distruggiLezione(lezione*);

//========================
// Getter
//========================

/*
 * Restituisce il codice identificativo della lezione.
 */
int getCodiceLezione(const lezione*);

/*
 * Restituisce la disciplina della lezione (stringa interna).
 */
const char* getDisciplinaLezione(const lezione*);

/*
 * Restituisce il nome della lezione (stringa interna).
 */
const char* getNomeLezione(const lezione*);

/*
 * Restituisce il numero massimo di posti disponibili.
 */
short int getPostiMax(const lezione*);

/*
 * Restituisce il numero di posti attualmente occupati.
 */
int getPostiOccupati(const lezione*);

/*
 * Restituisce un puntatore alla data della lezione.
 */
Data* getDataLezione(const lezione*);

/*
 * Restituisce l’orario di inizio della lezione.
 */
Orario* getOrarioLezione(const lezione*);

/*
 * Restituisce la durata della lezione in minuti.
 */
short int getDurataLezione(const lezione*);

//========================
// Setter
//========================

/*
 * Imposta il codice identificativo della lezione.
 */
void setCodiceLezione(lezione*, int);

/*
 * Imposta la disciplina della lezione.
 */
void setDisciplinaLezione(lezione*, const char*);

/*
 * Imposta il nome della lezione (massimo 49 caratteri).
 */
void setNomeLezione(lezione*, const char*);

/*
 * Imposta il numero massimo di posti disponibili.
 */
void setPostiMax(lezione*, short int);

/*
 * Incrementa di 1 il numero di posti occupati.
 */
void addPostiOccupati(lezione*);

/*
 * Imposta la data della lezione.
 */
void setDataLezione(lezione*, Data*);

/*
 * Imposta l’orario di inizio della lezione.
 */
void setOrarioInizio(lezione*, Orario*);

/*
 * Imposta la durata della lezione (in minuti).
 */
void setDurataLezione(lezione*, short int);


/*
 * Stmpa i dettagli della lezione
*/
void stampaDettagliLezione(lezione*);

/*
* Funzione che restituisce un puntatore alla lezione dato il suo codice identificativo
*/
lezione* getLezioneByID(listL* , int);

/*
 * Restituisce il codice abbonamento più alto
 */
int getMaxCodiceLezione(listL*);

#endif