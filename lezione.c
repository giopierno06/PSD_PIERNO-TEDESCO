#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lezione.h"
#include "data.h"
#include "orario.h"
#include "util.h"
#include "list_Lezioni.h"


#define VIOLA    "\033[1;35m" 
#define ROSSO    "\033[1;31m"
#define VERDE    "\033[1;32m"
#define GIALLO   "\033[1;33m"
#define RESET    "\033[0m"     //comandi per stampare a video defini come define per rendere il tutto più ordinato

listL* lezioni;


 struct lezione {
    int codice_lezione;         //Codidce lezione
    char disciplina[50];        //Disciplina della lezione 
    char nome[50];              //nome lezione
    short int postimax;         //numero di posti massimi
    int posti_occupato;         //numero di posti prenotati
    Data* data;                  // Data della lezione
    Orario* ora_di_inizio;       //orario di inizio della lezione 
    short int durata;           //durata della lezione in munuti
};


lezione* creaLezione() {
    lezione* l = malloc(sizeof(lezione));
    if (l!=NULL){
    l->codice_lezione = 0;
    strcpy(l->nome, "");
    strcpy(l->disciplina, "");
    l->data= creaData(1,1,2000);
    l->ora_di_inizio = creaOrario(0,0);
    l->durata = 0;
    l->postimax = 0;
}
    return l;
}

// Distruttore: libera la memoria
void distruggiLezione(lezione* l) {
    if (l != NULL) {
        // Se necessario, deallocare anche l->data
        free(l);
    }
}

// Getter (restituiscono copia o puntatore costante)

// Restituisce il codice della lezione
int getCodiceLezione(const lezione* l) {
    return l->codice_lezione;
}

// Restituisce la disciplina della lezione della lezione
const char* getDisciplinaLezione(const lezione* l) {
    return l->disciplina;
}

// Restituisce il nome della lezione della lezione
const char* getNomeLezione(const lezione* l) {
    return l->nome;
}

// Restituisce il numero massimo di posti disponibili
short int getPostiMax(const lezione* l) {
    return l->postimax;
}

// Restituisce il numero di posti occupati
int getPostiOccupati(const lezione* l) {
    return l->posti_occupato;
}

// Restituisce un puntatore alla data della lezione (sola lettura)
 Data* getDataLezione(const lezione* l) {
    return l->data;
}

// Restituisce l’orario di inizio della lezione
Orario* getOrarioLezione(const lezione* l) {
    return l->ora_di_inizio;
}

// Restituisce la durata della lezione
short int getDurataLezione(const lezione* l) {
    return l->durata;
}

// Setter (modificano i campi in modo sicuro)

// Funzione per impostare il codice della lezione
void setCodiceLezione(lezione* l, int codice) {
    if (l != NULL) {
        l->codice_lezione = codice;  // Assegna il codice alla lezione
    } else {
        printf("Errore: la lezione è NULL.\n");
    }
}


// Imposta la disciplina della lezione (copiato con sicurezza)
void setDisciplinaLezione(lezione* l, const char* disciplina) {
    if (l != NULL && disciplina != NULL) {
        strncpy(l->disciplina, disciplina, sizeof(l->disciplina) - 1);
        l->disciplina[sizeof(l->disciplina) - 1] = '\0'; // assicurati del terminatore
    }
}

// Imposta il nome della lezione (copiato con sicurezza)
void setNomeLezione(lezione* l, const char* nome) {
    strncpy(l->nome, nome, sizeof(l->nome));
    l->nome[sizeof(l->nome) - 1] = '\0'; // sicurezza
}

// Imposta il numero massimo di posti disponibili
void setPostiMax(lezione* l, short int max) {
    l->postimax = max;
}

// incrementa il numero di posti occupati
void addPostiOccupati(lezione* l) {
    l->posti_occupato++;
}

// Imposta la data della lezione (assegna il puntatore)
void setDataLezione(lezione* l, Data* data) {
    l->data =data; 
}

void setOrarioInizio(lezione* l, Orario* o) {
    if (l != NULL && o != NULL) {
        if (l->ora_di_inizio == NULL)
            l->ora_di_inizio = malloc(sizeof(Orario));
        *(l->ora_di_inizio) = *o;
    }
}

// Imposta la durata della lezione
void setDurataLezione(lezione* l, short int durata) {
    l->durata = durata;
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