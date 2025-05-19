#ifndef DATA_H
#define DATA_H

/// ===========================
/// Struttura dati
/// ===========================


// Dichiarazione di tipo opaco 
typedef struct Data Data;

// Interfaccia pubblica


/// ===========================
/// Costruttori e accessori
/// ===========================

//alloca una nuova data
Data* newData(void);

/**
 * Crea una data a partire da giorno, mese e anno.
 */
Data* creaData(int giorno, int mese, int anno);

/**
 * Restituisce il giorno della data.
 */
int getGiorno(Data* d);

/**
 * Restituisce il mese della data.
 */
int getMese(Data* d);

/**
 * Restituisce l'anno della data.
 */
int getAnno(Data* d);

/*
 * Imposta il giorno della data.
 */
void setGiorno(Data* d, int giorno);

/**
 * Imposta il mese della data.
 */
void setMese(Data* d, int mese);

/**
 * Imposta l'anno della data.
 */
void setAnno(Data* d, int anno);


void distruggiData(Data* d);


/// ===========================
/// Input/Output
/// ===========================

/**
 * Legge una data da input standard finché non è valida.
 * Formato richiesto: gg mm aaaa
 */
Data* leggiData();

/**
 * Stampa la data nel formato GG/MM/AAAA.
 */
void stampaData(Data* d);


/// ===========================
/// Funzionalità principali
/// ===========================

/**
 * Verifica se una data è valida.
 * Restituisce 1 se la data è valida, 0 altrimenti.
 */
int dataValida(Data d);

/**
 * Confronta due date:
 *   - ritorna -1 se d1 < d2
 *   - ritorna  0 se d1 == d2
 *   - ritorna  1 se d1 > d2
 */
int confrontaDate(Data* d1, Data*d2);

/**
 * Aggiunge un numero di mesi a una data.
 * Se il giorno non è valido nel nuovo mese, lo adatta al massimo possibile.
 */
Data* aggiungiMesi(Data*d, int mesi);

/**
 * Restituisce la data odierna secondo l'orologio del sistema.
 */
Data* oggi();

//Sottrae un numero di giorni da una data e restituisce la nuova data.
Data* sottraiGiorni(Data*, int);



#endif // DATA_H