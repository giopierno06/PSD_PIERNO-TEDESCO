#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "gestione_Abbonamenti.h"
#include "util.h"
#include "list.h"
#include "abbonamento.h"
#include "data.h"


// Struttura lista (opaca, non visibile all'esterno)
struct list {
    abbonamento* value;    // Valore contenuto nel nodo, di tipo 'abbonamento'
    struct list *next;    // Puntatore al nodo successivo (NULL se è l'ultimo)
};  // Fine definizione struttura list


#define VIOLA    "\033[1;35m" 
#define ROSSO    "\033[1;31m"
#define VERDE    "\033[1;32m"
#define GIALLO   "\033[1;33m"
#define BLU   "\033[34m"
#define RESET    "\033[0m"     //comandi per stampare a video defini come define per rendere il tutto più ordinato




// Funzione getter per il campo 'next' del nodo
// Parametri:
//   - nodo: il nodo corrente della lista
// Restituisce:
//   - Il prossimo nodo nella lista
list* getNext(list* nodo) {
    return nodo->next;  // Restituisce il prossimo nodo collegato
}


// Funzione setter per il campo 'next' del nodo
// Parametri:
//   - nodo: il nodo corrente della lista
//   - next: il prossimo nodo che deve essere collegato
void setNext(list* nodo, list* next) {
    nodo->next = next;  // Imposta il prossimo nodo collegato
}


// Funzione per creare una nuova lista vuota
list* newList(void) {
    return NULL;  // Restituisce NULL, che rappresenta una lista vuota
}

// Funzione per verificare se la lista è vuota
int emptyList(list* l) {
    return l == NULL;  // La lista è vuota se è NULL
}

// Funzione per aggiungere un nuovo elemento in testa alla lista
list* consList(abbonamento* val, list* l) {
    list* nuovo = malloc(sizeof(list));  // Allocazione della memoria per il nuovo nodo
    if (nuovo != NULL) {
        nuovo->value = val;  // Assegna l'abbonamento al nuovo nodo
        nuovo->next = l;     // Collega il nuovo nodo alla lista esistente
    }
    return nuovo;  // Restituisce la lista con il nuovo nodo aggiunto
}



// Funzione per ottenere la coda della lista (tutto tranne il primo elemento)
list* tailList(list* l) {
    if (l != NULL) {
        return l->next;  // Restituisce il successivo nodo (la coda)
    }
    return NULL;  // Se la lista è vuota, restituisce NULL
}

abbonamento* getFirst(list* l) {
    if (l != NULL) 
        return l->value;  // Restituisce un puntatore al valore del primo nodo (l'abbonamento)
    
    else 
        return NULLITEM;  // Se la lista è vuota, restituisce un abbonamento "vuoto" (un puntatore)
}


//Funcione per stampare la lista
void printList(list* l) {
    // Controlla se la lista è vuota
    if (l == NULL) {
        printf("%sLa lista degli abbonamenti è vuota.%s\n",ROSSO,RESET);
        return;  // Esce dalla funzione se la lista è vuota
    }

    // Altrimenti, stampa tutti gli abbonamenti
    while (l != NULL) {
        // Stampa dei dettagli dell'abbonamento
        // Funzione per stampare tutti i dettagli di un abbonamento
         stampaDettagliAbbonamento(l->value); 
        l = l->next;  // Passa al nodo successivo}
    printf("\n");
}}
// Funzione per liberare la memoria della lista
void freeList(list* l) {
    list* temp;
    while (l != NULL) {
        temp = l;
        l = l->next;
        free(temp);  // Libera la memoria del nodo
    }
}


// Getter e setter per 'value' (abbonamento)
abbonamento* getValue(list* l) {
    return l->value;  // Restituisce un puntatore a 'value'
}

void setValue(list* l,  abbonamento* a) {
    l->value = a;  // Imposta il valore dell'abbonamento
}

//controllare inf hiding

//funzione per stampare a schermo la lista secondo l'oridine desiderato dal cliente
void visualizzaListaOrdinata(list* abbonati) {
    int scelta;
    list* copia = NULL;      // Puntatore per la copia della lista
    list* temp = abbonati;   // Puntatore temporaneo per scorrere la lista originale

    // Crea una copia della lista originale
    while (temp != NULL) {
        // Allocazione di un nuovo nodo per la copia della lista
        list* nuovo = malloc(sizeof(list));  // Alloca un nodo per la copia
        if (!nuovo) {
            // Se l'allocazione fallisce, termina la funzione
            printf("Errore di allocazione.\n");
            return;
        }
        setValue(nuovo, getValue(temp));  // Copia il valore (abbonamento) dal nodo 'temp' al nodo 'nuovo'
        setNext(nuovo, NULL);  // Imposta il prossimo nodo di 'nuovo' come NULL (fine della lista)

        // Inserimento in fondo alla nuova lista (copia)
        if (copia == NULL) {
            copia = nuovo;  // Se la lista di copia è vuota, il nuovo nodo è il primo
        } else {
            list* p = copia;
            // Trova l'ultimo nodo della lista copia
            while (p->next != NULL) p = p->next;
            // Collega il nuovo nodo all'ultimo nodo della lista copia
            p->next = nuovo;
        }

        // Passa al nodo successivo della lista originale
        temp = temp->next;
    }
    while(1){
    // Mostra il menu di scelta per l'ordinamento
    printf("%s=============================================================\n",GIALLO);
    printf("SCEGLI L'ORDINAMENTO PER LA VISUALIZZAZIONE DELLA LISTA:\n");
    printf("=============================================================%s\n",RESET);
    printf("%s1. Alfabetico (nome)\n",VERDE);
    printf("2. Codice abbonamento crescente\n");
    printf("3. Codice abbonamento decrescente\n");
    printf("4. Data di inizio abbonamento\n");
    printf("5. Data di scadenza abbonamento\n");
    printf("6. Ricerca l'abbonamento%s\n",RESET); 
    printf("%s0. Torna indietro\n%s",ROSSO,RESET);
    printf("%s=============================================================%s\n",GIALLO,RESET);
    printf("Scelta: ");
    scanf("%d", &scelta);
    getchar();  // Consuma il carattere di invio lasciato dal scanf

    // Ordinamento della lista in base alla scelta dell'utente
    switch (scelta) {
        case 0:
        pulisciSchermo();
        return;
        break;
        case 1:
        pulisciSchermo(); 
        ordinaPerNome(&copia);  // Ordina alfabeticamente per nome e per cognome
        printList(copia);
        printf("Premi invio per tornare indietro......\n");
        getchar();
        pulisciSchermo(); 
        break;
        case 2:
        pulisciSchermo(); 
        ordinaPerCodiceCrescente(&copia);  // Ordina per codice crescente
        printList(copia);
        printf("Premi invio per tornare indietro......\n");
        getchar();
        pulisciSchermo(); 
        break;
        case 3:
        pulisciSchermo(); 
        ordinaPerCodiceDecrescente(&copia);  // Ordina per codice crescente
        printList(copia);
        printf("Premi invio per tornare indietro......\n");
        getchar();
        pulisciSchermo(); 
        break;
        case 4:
        pulisciSchermo(); 
        ordinaPerDataInizio(&copia);  // Ordina per data di inizio
        printList(copia);
        printf("Premi invio per tornare indietro......\n");
        getchar();
        pulisciSchermo(); 
        break;
        case 5:
        pulisciSchermo(); 
        ordinaPerDataScadenza(&copia);  // Ordina per data di scadenza
        printList(copia);
        printf("Premi invio per tornare indietro......\n");
        getchar();
        pulisciSchermo();     
        break;
        case 6:
        pulisciSchermo(); 
        list* temp=ricercaAbbonamento(abbonati);   // Funzione di ricerca abbonamento
        if(temp==NULL){
        pulisciSchermo();
        break;}
        printf("%sAbbonamento trovato!%s\n", VERDE, RESET);
        printf("------------------------------------------\n");
        stampaDettagliAbbonamento(temp->value);
        printf("Premere INVIO per continuare...\n");
        getchar();  // Attende che l'utente premi INVIO
        pulisciSchermo(); 
        break;
        default:
            // Se l'utente inserisce una scelta non valida, mostra messaggio errore
            pulisciSchermo();
            printf("%sSCELTA NON VALIDA\n%s",ROSSO,RESET);
            break;
    }


}
       // Libera la memoria della lista 
       
       freeList(copia);  // Libera tutta la memoria allocata per la lista copia
    }

// Funzione per ordinare la lista degli abbonamenti in base al nome, cognome e data di iscrizione
void ordinaPerNome(list** head) {
    // Verifica se la lista è vuota o nulla
    if (!head || !*head) return;  // Se la lista è vuota o nulla, non fare nulla.

    int scambiato;               // Variabile per verificare se è stato effettuato uno scambio
    list* p;                     // Puntatore temporaneo per attraversare la lista

    // Ciclo per ordinare la lista, algoritmo di ordinamento a bolle (Bubble Sort)
    do {
        scambiato = 0;  // Impostiamo a 0 che non ci sono stati scambi nella prima iterazione
        p = *head;      // Inizializza il puntatore p all'inizio della lista

        // Ciclo interno per confrontare gli abbonamenti a coppie
        while (getNext(p) != NULL) {
            // Confronta i nomi degli abbonamenti
            int confrontoNome = strcmp(getNome(getValue(p)), getNome(getValue(getNext(p))));
            if (confrontoNome > 0) {
                // Se il nome di p è maggiore di quello di p->next, scambia i valori
                abbonamento* temp = getValue(p);  // Usa l'operatore . per accedere ai membri
                setValue(p, getValue(getNext(p))); // Imposta il valore di p con il valore del nodo successivo
                setValue(getNext(p), temp); // Imposta il valore del nodo successivo con il valore temporaneo
                scambiato = 1;  // Segna che è stato effettuato uno scambio
            } 
            // Se i nomi sono uguali, ordina per cognome
            else if (confrontoNome == 0) {
                // Confronta i cognomi degli abbonamenti
                int confrontoCognome = strcmp(getCognome(getValue(p)), getCognome(getValue(getNext(p))));
                if (confrontoCognome > 0) {
                    // Se il cognome di p è maggiore di quello di p->next, scambia i valori
                    abbonamento* temp = getValue(p);
                    setValue(p, getValue(getNext(p))); // Imposta il valore di p con il valore del nodo successivo
                    setValue(getNext(p), temp); // Imposta il valore del nodo successivo con il valore temporaneo
                    scambiato = 1;  // Segna che è stato effettuato uno scambio
                }
                // Se anche i cognomi sono uguali, ordina per data di iscrizione
                else if (confrontoCognome == 0) {
                    // Confronta le date di iscrizione (data_inizio) degli abbonamenti
                    if (confrontaDate(getDataInizio(getValue(p)), getDataInizio(getValue(getNext(p)))) > 0) {
                        // Se la data di iscrizione di p è dopo quella di p->next, scambia i valori
                        abbonamento* temp = getValue(p);
                        setValue(p, getValue(getNext(p))); // Imposta il valore di p con il valore del nodo successivo
                        setValue(getNext(p), temp); // Imposta il valore del nodo successivo con il valore temporaneo
                        scambiato = 1;  // Segna che è stato effettuato uno scambio
                    }
                }
            }

            p = getNext(p);  // Passa al prossimo elemento della lista
        }
    } while (scambiato);  // Continua a ripetere il processo finché ci sono scambi
}


void ordinaPerDataScadenza(list** abbonati) {
    // Controlla se la lista è vuota o ha un solo elemento
    if (*abbonati == NULL || getNext(*abbonati)== NULL) {
        return; // In tal caso non è necessario ordinare
    }

    list *i, *j;
    abbonamento* temp;

    // Algoritmo di ordinamento a bolle (Bubble Sort)
    for (i = *abbonati; i != NULL; i = getNext(i)) {
        for (j = getNext(i); j != NULL; j = getNext(j)) {
            // Calcola la data di scadenza per entrambi gli abbonamenti usando la funzione aggiungiMesi
            Data* data_scadenza_i = aggiungiMesi(getDataInizio(getValue(i)), getDurata(getValue(i)));
            Data* data_scadenza_j = aggiungiMesi(getDataInizio(getValue(j)), getDurata(getValue(j)));

            // Confronta le date di scadenza
            if (confrontaDate(data_scadenza_i, data_scadenza_j) > 0) {
                // Se la data di scadenza di i è maggiore (più recente) di quella di j,
                // scambia i contenuti dei due nodi
                temp = getValue(i);
                setValue(i, getValue(j));  // Imposta il valore di i a quello di j
                setValue(j, temp);         // Imposta il valore di j a quello temporaneo (che è il valore di i)
            }
            // Liberare la memoria allocata dinamicamente per le date di scadenza
            free(data_scadenza_i);
            free(data_scadenza_j);
        }
    }
}


// Funzione per ordinare la lista degli abbonati in base alla data di inizio (in ordine crescente)
void ordinaPerDataInizio(list** abbonati) {
    // Controlla se la lista è vuota o ha un solo elemento
    if (*abbonati == NULL ||getNext(*abbonati) == NULL) {
        return; // In tal caso non è necessario ordinare
    }

    list *i, *j;            // Puntatori per scorrere la lista
    abbonamento* temp;       // Variabile temporanea per lo scambio dei valori

    // Algoritmo di ordinamento a bolle (Bubble Sort)
    for (i = *abbonati; i != NULL; i = getNext(i)) {
        for (j =  getNext(i); j != NULL; j =  getNext(j)) {
            // Confronta le date di inizio di due nodi
            // confrontaDate restituisce > 0 se la prima data è successiva alla seconda
            if (confrontaDate(getDataInizio(getValue(i)), getDataInizio(getValue(j))) > 0) {
                // Scambia i valori se la data di i è dopo quella di j
                temp = getValue(i);
                setValue(i, getValue(j)); 
                setValue(j,temp); 
            }
        }
    }
}

// Funzione per ordinare la lista di abbonamenti in ordine decrescente
void ordinaPerCodiceDecrescente(list** head) {
    if (!head || !*head) return;

    int scambiato;
    list* p;

    do {
        scambiato = 0;
        p = *head;

        while (getNext(p) != NULL) {
            abbonamento* a1 = getValue(p);
            abbonamento* a2 = getValue(getNext(p));

            if (getCodiceAbbonamento(a1) < getCodiceAbbonamento(a2)) {
                abbonamento* temp = a1;
                setValue(p, a2);
                setValue(getNext(p), temp);
                scambiato = 1;
            }

            p = getNext(p);
        }
    } while (scambiato);
}


void ordinaPerCodiceCrescente(list** head) {
    if (!head || !*head) return;

    int scambiato;
    list* p;

    do {
        scambiato = 0;
        p = *head;

        while (getNext(p) != NULL) {
            abbonamento* a1 = getValue(p);
            abbonamento* a2 = getValue(getNext(p));

            if (getCodiceAbbonamento(a1) > getCodiceAbbonamento(a2)) {
                abbonamento* temp = a1;
                setValue(p, a2);
                setValue(getNext(p), temp);
                scambiato = 1;
            }

            p = getNext(p);
        }
    } while (scambiato);
}
