#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "list.h"
#include "abbonamento.h"
#include "file.h"
#include "data.h"
#include "util.h"

// =========================
// DEFINIZIONE STRUTTURA ABBONAMENTO
// =========================

struct abbonamento {
    int codice_abbonamento;   // Codice identificativo univoco del cliente
    char nome[50];            // Nome del cliente
    char cognome[50];         // Cognome del cliente
    Data* data_inizio;         // Data di inizio dell'abbonamento
    int durata;               // Durata in mesi dell'abbonamento
};

// Definizione del puntatore vuoto
abbonamento* vuoto = NULL;

// Funzione per inizializzare un abbonamento "vuoto"
void init_vuoto() {
    vuoto = creaAbbonamento();  // Allocazione memoria per il puntatore
    if (vuoto != NULL) {
    vuoto->codice_abbonamento = 0;  // Corretto, a è un puntatore
    strcpy(vuoto->nome, "");  // Corretto, a è un puntatore
    strcpy(vuoto->cognome, "");  // Corretto, a è un puntatore
    vuoto->data_inizio = creaData(1, 1, 2000);  // Corretto, a è un puntatore
    }
}

// Definizioni di colori ANSI per formattare il testo in output. 
// Questi vengono usati per stampare messaggi colorati in console.
#define VIOLA    "\033[1;35m"   // Colore Viola
#define ROSSO    "\033[1;31m"   // Colore Rosso
#define VERDE    "\033[1;32m"   // Colore Verde
#define GIALLO   "\033[1;33m"   // Colore Giallo
#define BLU      "\033[34m"     // Colore Blu
#define RESET    "\033[0m"      // Resetta il colore alla configurazione predefinita della console


list* abbonati;


//Creazione di un abbonamneto e allocazione della memoria
abbonamento* creaAbbonamento() {
    abbonamento* a = malloc(sizeof(abbonamento));
    if (a == NULL) return NULL;

    a->codice_abbonamento = 0;
    strcpy(a->nome, "");
    strcpy(a->cognome, "");
    a->data_inizio = NULL;
    a->durata = 0;

    return a;
}



// Restituisce il codice identificativo dell'abbonamento.
// Parametri:
//   - a: puntatore alla struct abbonamento da cui ottenere il codice
// Restituisce:
//   - il valore del campo codice_abbonamento
int getCodiceAbbonamento(const abbonamento* a) {
    return a->codice_abbonamento;  // Restituisce il valore del codice
}

// Restituisce il nome del cliente nell'abbonamento.
// Parametri:
//   - a: puntatore alla struct abbonamento da cui ottenere il nome
// Restituisce:
//   - una copia del campo nome (stringa)
const char* getNome(const abbonamento* a) {
    return a->nome;  // Restituisce direttamente il campo nome
}

// Restituisce il cognome del cliente nell'abbonamento.
// Parametri:
//   - a: puntatore alla struct abbonamento da cui ottenere il cognome
// Restituisce:
//   - una copia del campo cognome (stringa)
const  char* getCognome(const abbonamento* a) {
    return a->cognome;  // Restituisce direttamente il campo cognome
}

// Restituisce la data di inizio dell'abbonamento.
// Parametri:
//   - a: puntatore alla struct abbonamento da cui ottenere la data
// Restituisce:
//   - il valore del campo data_inizio (di tipo Data)
Data* getDataInizio(const abbonamento* a) {
    return a->data_inizio;  // Restituisce il valore del campo data_inizio
}

// Restituisce la durata dell'abbonamento in mesi.
// Parametri:
//   - a: puntatore alla struct abbonamento da cui ottenere la durata
// Restituisce:
//   - il valore del campo durata
int getDurata(const abbonamento* a) {
    return a->durata;  // Restituisce il valore del campo durata
}


// Imposta il codice identificativo dell'abbonamento.
// Parametri:
//   - a: puntatore alla struct abbonamento da modificare
//   - codice: nuovo valore intero da assegnare al campo codice_abbonamento
void setCodiceAbbonamento(abbonamento* a, int codice) {
    a->codice_abbonamento = codice;  // Assegna direttamente il codice
}

// Imposta il nome del cliente nell'abbonamento, copiando in modo sicuro.
// Parametri:
//   - a: puntatore alla struct abbonamento da modificare
//   - nome: stringa da copiare nel campo nome (massimo 49 caratteri + terminatore)
void setNome(abbonamento* a, const char* nome) {
    strncpy(a->nome, nome, sizeof(a->nome) - 1);     // Copia al massimo 49 caratteri
    a->nome[sizeof(a->nome) - 1] = '\0';              // Garantisce che la stringa sia terminata correttamente
}

// Imposta il cognome del cliente nell'abbonamento, copiando in modo sicuro.
// Parametri:
//   - a: puntatore alla struct abbonamento da modificare
//   - cognome: stringa da copiare nel campo cognome (massimo 49 caratteri + terminatore)
void setCognome(abbonamento* a, const char* cognome) {
    strncpy(a->cognome, cognome, sizeof(a->cognome) - 1); // Copia sicura del cognome
    a->cognome[sizeof(a->cognome) - 1] = '\0';             // Terminazione sicura
}

// Imposta la data di inizio dell'abbonamento.
// Parametri:
//   - a: puntatore alla struct abbonamento da modificare
//   - data: oggetto di tipo Data da assegnare
void setDataInizio(abbonamento* a, Data* data) {
    a->data_inizio = data;  // Assegnazione diretta del campo struct
}

// Imposta la durata in mesi dell'abbonamento.
// Parametri:
//   - a: puntatore alla struct abbonamento da modificare
//   - durata: numero di mesi di durata dell'abbonamento
void setDurata(abbonamento* a, int durata) {
    a->durata = durata;  // Assegna direttamente la durata
}

// Funzione che restituisce il codice abbonamento massimo presente nella lista
// Parametri:
//   - abbonati: puntatore alla lista di abbonamenti
// Restituisce:
//   - Il valore massimo del campo 'codice_abbonamento' trovato nella lista
int getMaxCodiceAbbonamento(list* abbonati) {
    int max = 0;  // Inizializza il massimo a 0 (presuppone codici positivi)

    // Itera finché la lista non è vuota
    while (!emptyList(abbonati)) {
        // Ottiene il primo elemento della lista (senza modificarla)
        abbonamento* a = getFirst(abbonati);

        // Usa il getter per accedere al codice abbonamento
        int codice = getCodiceAbbonamento(a);

        // Se il codice corrente è maggiore del massimo, lo aggiorna
        if (codice > max) {
            max = codice;
        }

        // Passa al resto della lista (esclude il primo elemento)
        abbonati = tailList(abbonati);
    }

    // Ritorna il massimo codice trovato
    return max;
}

// Funzione per stampare tutti i dettagli di un abbonamento
// Parametro:
//   - a: Abbonamento da stampare
// La funzione usa i getter per accedere alle informazioni dell'abbonamento
// e le stampa in modo formattato, senza esporre i dettagli interni.
void stampaDettagliAbbonamento(abbonamento* a) {
    // Stampa l'ID dell'abbonamento usando il getter per accedere in modo incapsulato al campo 'codice_abbonamento'
    printf("ID: %d\n", getCodiceAbbonamento(a));
    
    // Stampa il nome del cliente utilizzando il getter per il nome, mantenendo il dato incapsulato
    printf("Nome: %s\n", getNome(a));
    
    // Stampa il cognome del cliente usando il getter per accedere al cognome in modo sicuro
    printf("Cognome: %s\n", getCognome(a));
    
    // Stampa la data di inizio dell'abbonamento tramite il getter della data di inizio
    printf("Data di inizio: ");
    stampaData(getDataInizio(a));  // Funzione che stampa la data nel formato desiderato
    
    // Stampa la durata dell'abbonamento, ottenuta tramite il getter della durata
    printf("\nDurata: %d mesi\n", getDurata(a));
    
    // Calcola la data di scadenza aggiungendo la durata in mesi alla data di inizio
    Data* data_scadenza = aggiungiMesi(getDataInizio(a), getDurata(a));
    
    // Stampa la data di scadenza, utilizzando la funzione di stampa della data
    printf("Data Scadenza: ");
    stampaData(data_scadenza);  // Mostra la data di scadenza
    
    // Separatore visivo per migliorare la leggibilità della stampa
    printf("\n--------------------------------------------\n");
}

// Funzione che restituisce un puntatore a un abbonamento dato il suo codice identificativo
// Parametri:
//   - abbonati: lista degli abbonamenti (list*)
//   - codice: codice identificativo dell'abbonamento da cercare
// Ritorna:
//   - Puntatore all'abbonamento trovato (abbonamento*), oppure NULL se non esiste
abbonamento* getAbbonamentoByID(list* abbonati, int codice) {
    list* curr = abbonati;  // Inizializza il puntatore al primo nodo della lista

    // Cicla attraverso la lista fino alla fine
    while (curr != NULL) {
        abbonamento* abb = (abbonamento*) getValue(curr);  // Estrae l'abbonamento dal nodo corrente

        // Controlla se il codice dell'abbonamento corrisponde a quello cercato
        if (getCodiceAbbonamento(abb) == codice) {
            return abb;  // Ritorna il puntatore all'abbonamento trovato
        }

        curr = getNext(curr);  // Passa al nodo successivo
    }

    return NULL;  // Nessun abbonamento con il codice specificato trovato
}


int abbonamento_isScaduto(abbonamento* ab) {
    if (!ab) return 1; // consideriamo scaduto se abbonamento nullo

    Data* data_scadenza = aggiungiMesi(ab->data_inizio,ab->durata); // funzione che ti restituisce la data di scadenza

    Data* data_oggi = oggi();
    if (!data_oggi) {
        // se non riesci a ottenere la data odierna, per sicurezza consideriamo scaduto
        return 1;
    }

    int confronto = confrontaDate(data_oggi, data_scadenza);

    free(data_oggi);

    // Se data odierna > data scadenza, l'abbonamento è scaduto
    if (confronto == 1) {
        return 1;
    }

    return 0;
}





// Funzione per distruggere un abbonamento
void distruggiAbbonamento(abbonamento* a) {
    if (a != NULL) {
        distruggiData(a->data_inizio);  // Libera la memoria della data associata
        free(a);  // Libera l'abbonamento stesso
    }
}