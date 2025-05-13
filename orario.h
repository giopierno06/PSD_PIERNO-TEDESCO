#ifndef ORARIO_H 
#define ORARIO_H

// Forward declaration della struttura Orario (struttura definita altrove)
typedef struct Orario Orario;

// === CREAZIONE / INIZIALIZZAZIONE ===

// Crea un orario con ora e minuti specificati (es. 14, 30 -> 14:30)
Orario* creaOrario(int, int);

// Crea un nuovo orario (probabilmente inizializzato a 00:00)
Orario* newOrario();

// === GETTER ===

// Restituisce l'ora (campo ora) da una struttura Orario
int getOra(const Orario*);

// Restituisce i minuti (campo minuti) da una struttura Orario
int getMinuto(const Orario*);

// === SETTER ===

// Imposta il campo "ora" in una struttura Orario
void setOra(Orario*, int);

// Imposta il campo "minuto" in una struttura Orario
void setMinuto(Orario*, int);

// === FUNZIONALITÀ AVANZATE ===

// Stampa l'orario attuale del sistema in formato HH:MM:SS
void stampaOrarioAttuale();

// Legge un orario da input utente (ora e minuti) e restituisce un puntatore a Orario
Orario* leggiOrario();

// Confronta due orari:
// - Ritorna -1 se il primo è prima del secondo
// - Ritorna  0 se sono uguali
// - Ritorna  1 se il primo è dopo il secondo
int confrontaOrari(Orario*, Orario*);

// Aggiunge un certo numero di minuti a un orario dato
// Restituisce un nuovo puntatore a Orario (gestione memoria esterna necessaria)
Orario* aggiungiMinuti(Orario*, int); 

// Stampa un orario in formato HH:MM
void stampaOrario(Orario*);

#endif // ORARIO_H