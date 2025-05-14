#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lezione.h"
#include "data.h"
#include "orario.h"
#include "util.h"
#include "prenotazioni.h"  
#include "list_Lezioni.h"

// Definizione di colori per output formattato nel terminale
#define VIOLA    "\033[1;35m" 
#define ROSSO    "\033[1;31m"
#define VERDE    "\033[1;32m"
#define GIALLO   "\033[1;33m"
#define RESET    "\033[0m"     // Reset dello stile del terminale

listL* lezioni; // Puntatore alla lista delle lezioni (presumibilmente usato altrove)


// Definizione della struttura lezione
struct lezione {
    int codice_lezione;         // Codice identificativo della lezione
    char disciplina[50];        // Disciplina della lezione
    char nome[50];              // Nome della lezione
    short int postimax;         // Numero massimo di posti disponibili
    int posti_occupato;         // Numero di posti già prenotati
    Data* data;                 // Puntatore alla data della lezione
    Orario* ora_di_inizio;      // Puntatore all'orario di inizio
    short int durata;           // Durata in minuti
};

// Funzione per creare una nuova lezione con valori di default
lezione* creaLezione() {
    lezione* l = malloc(sizeof(lezione)); // Allocazione dinamica
    if (l != NULL) {
        l->codice_lezione = 0;
        strcpy(l->nome, "");
        strcpy(l->disciplina, "");
        l->data = creaData(1, 1, 2000);              // Inizializza a una data fittizia
        l->ora_di_inizio = creaOrario(0, 0);         // Inizializza a mezzanotte
        l->posti_occupato = 0;
        l->durata = 0;
        l->postimax = 0;
    }
    return l;
}

// Funzione per deallocare la memoria di una lezione
void distruggiLezione(lezione* l) {
    if (l != NULL) {
        // NOTA: libera solo la struttura, non le sottostrutture (data, orario)
        free(l);
    }
}

// === GETTER ===

// Restituisce il codice della lezione
int getCodiceLezione(const lezione* l) {
    return l->codice_lezione;
}

// Restituisce la disciplina
const char* getDisciplinaLezione(const lezione* l) {
    return l->disciplina;
}

// Restituisce il nome
const char* getNomeLezione(const lezione* l) {
    return l->nome;
}

// Numero massimo di posti
short int getPostiMax(const lezione* l) {
    return l->postimax;
}

// Numero di posti prenotati
int getPostiOccupati(const lezione* l) {
    return l->posti_occupato;
}

// Puntatore alla data della lezione
Data* getDataLezione(const lezione* l) {
    return l->data;
}

// Puntatore all'orario di inizio
Orario* getOrarioLezione(const lezione* l) {
    return l->ora_di_inizio;
}

// Durata della lezione
short int getDurataLezione(const lezione* l) {
    return l->durata;
}

// === SETTER ===

// Imposta il codice della lezione
void setCodiceLezione(lezione* l, int codice) {
    if (l != NULL) {
        l->codice_lezione = codice;
    } else {
        printf("Errore: la lezione è NULL.\n");
    }
}

// Imposta la disciplina (con protezione da overflow)
void setDisciplinaLezione(lezione* l, const char* disciplina) {
    if (l != NULL && disciplina != NULL) {
        strncpy(l->disciplina, disciplina, sizeof(l->disciplina) - 1);
        l->disciplina[sizeof(l->disciplina) - 1] = '\0';
    }
}

// Imposta il nome della lezione
void setNomeLezione(lezione* l, const char* nome) {
    strncpy(l->nome, nome, sizeof(l->nome));
    l->nome[sizeof(l->nome) - 1] = '\0'; // Assicura il terminatore
}

// Imposta il numero massimo di posti
void setPostiMax(lezione* l, short int max) {
    l->postimax = max;
}

// Incrementa di 1 i posti occupati
void addPostiOccupati(lezione* l) {
    l->posti_occupato++;
}

// Imposta la data della lezione
void setDataLezione(lezione* l, Data* data) {
    l->data = data;
}

// Imposta l'orario di inizio (con protezione da NULL)
void setOrarioInizio(lezione* l, Orario* o) {
    if (l != NULL && o != NULL) {
        if (l->ora_di_inizio == NULL)
            l->ora_di_inizio = newOrario(); // Alloca se non esiste
        l->ora_di_inizio = o;               // Sovrascrive comunque
    }
}

// Imposta la durata della lezione
void setDurataLezione(lezione* l, short int durata) {
    l->durata = durata;
}

// Stampa dettagliata di una lezione
void stampaDettagliLezione(lezione* l) {
    if (l == NULL) {
        printf("%sLezione non valida.%s\n", ROSSO, RESET);
        return;
    }

    int posti_rimanenti = l->postimax - l->posti_occupato;
    Orario* orario_fine = aggiungiMinuti(l->ora_di_inizio, l->durata); // Calcola orario di fine

    // Stampa formattata
    printf("%s================== DETTAGLI LEZIONE ==================%s\n", GIALLO, RESET);
    printf("Codice Lezione     : %d\n", l->codice_lezione);
    printf("Nome               : %s\n", l->nome);
    printf("Disciplina         : %s\n", l->disciplina);
    printf("Data               : ");
    stampaData(l->data); // Funzione esterna
    printf("\nOrario             : %02d:%02d - %02d:%02d\n",
           getOra(l->ora_di_inizio), getMinuto(l->ora_di_inizio),
           getOra(orario_fine), getMinuto(orario_fine));
    printf("Durata             : %d minuti\n", l->durata);
    printf("Posti Massimi      : %d\n", l->postimax);
    printf("Posti Occupati     : %d\n", l->posti_occupato);
    printf("Posti Rimanenti    : %d\n", posti_rimanenti);
    printf("%s=====================================================%s\n", GIALLO, RESET);

    free(orario_fine); // Dealloca orario creato dinamicamente
}

// Funzione: getMaxCodiceLezione
// ------------------------------
// Parametri:
// - lezioni: un puntatore a una lista di lezioni (di tipo listL), 
//   dove ogni nodo contiene una struttura 'lezione'.
//   Ogni lezione ha un codice numerico univoco.
//
// Restituisce:
// - Il codice massimo di lezione trovato nella lista.
// - Se la lista è vuota, restituisce 0 (si presume che i codici siano positivi o nulli).
int getMaxCodiceLezione(listL* lezioni) {
    int max = 0;
    listL* corrente = lezioni;

    while (corrente != NULL) {
        lezione* a = lezione_getValue(corrente);
        if (a == NULL) {
            corrente = lezione_getNext(corrente);
            continue;
        }
        int codice = getCodiceLezione(a);
        if (codice > max) {
            max = codice;
        }
        corrente = lezione_getNext(corrente);
    }

    return max;
}