#ifndef ORARIO_H 
#define ORARIO_H

// Definizione della struttura Orario
typedef struct {
    short int ora;     // Ora (da 0 a 23)
    short int minuti;  // Minuti (da 0 a 59)
} Orario;


// Funzione per creare una nuova istanza di Orario
Orario* newOrario();

int getOra(const Orario* orario);

int getMinuto(const Orario* orario) ;

// Funzione setter per l'ora
void setOra(Orario*, int);

// Funzione setter per il minuto
void setMinuto(Orario*, int);


// Funzione che stampa l'orario attuale del sistema in formato HH:MM:SS
void stampaOrarioAttuale();

// Funzione che legge un orario da input utente (richiede ora e minuti)
// Restituisce una variabile di tipo Orario
Orario* leggiOrario();

// Funzione che confronta due orari
// Ritorna: -1 se il primo orario è precedente, 0 se sono uguali, 1 se il primo è successivo
int confrontaOrari(Orario*, Orario*);

// Funzione che aggiunge un certo numero di minuti a un orario
// Modifica direttamente l'orario passato tramite puntatore
Orario* aggiungiMinuti(Orario*, int); 

// Funzione che stampa un orario (in formato HH:MM)
void stampaOrario(Orario*);

#endif // ORARIO_H