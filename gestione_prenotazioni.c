#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "prenotazioni.h"
#include "lezione.h"
#include "data.h"
#include "orario.h"
#include "list.h"
#include "util.h"
#include "abbonamento.h"
#include "gestione_Abbonamenti.h"
#include "gestione_lezioni.h"
#include "list_Lezioni.h"
#include "gestione_prenotazioni.h"
#include "list_prenotazioni.h"
#include "file.h"

// Definizione dei colori ANSI per stampare testo colorato nel terminale
#define VIOLA    "\033[1;35m"
#define ROSSO    "\033[1;31m"
#define VERDE    "\033[1;32m"
#define GIALLO   "\033[1;33m"
#define RESET    "\033[0m"
#define BLU      "\033[1;34m"   

extern listP* prenotazioni;
extern list* abbonati;
extern listL* lezioni;


void gestione_prenotazioni() {
    if (prenotazioni == NULL)
        prenotazioni = prenotazione_newList();
    
    
    // Se gli abbonati non sono stati ancora caricati, li carica dal file
    if (abbonati == NULL)
        abbonati = caricaAbbonamentiDaFile(abbonati, "abbonamenti.txt");

    // Se la lista è ancora vuota (file vuoto o non esistente), inizializza una lista nuova
    if (abbonati == NULL)
        abbonati = newList();

    
    // Se gli lezioni non sono stati ancora caricati, li carica dal file
    if (lezioni == NULL)
        lezioni = caricaLezioniDaFile(lezioni, "lezioni.txt");

      // Se la lista è ancora vuota (file vuoto o non esistente), inizializza una lista nuova
    if (lezioni == NULL)
        lezioni = lezione_newList();

    int scelta = -1;  // Variabile per memorizzare la scelta dell’utente nel menu

    // Ciclo principale: continua a mostrare il menu fino a che l'utente non sceglie di uscire (0)
    while (1) {
         pulisciSchermo(); 
        // Menu con codici ANSI per i colori (estetica)
        printf("\n%s=============================================%s\n", VIOLA, RESET);
        printf("%s   BENVENUTO NELLA GESTIONE PRENOTAZIONI   %s\n", GIALLO, RESET);
        printf("%s=============================================%s\n", VIOLA, RESET);
        printf("%s 1.%s PRENOTA UNA LEZIONE PER UN CLIENTE\n", VERDE, RESET);
        printf("%s 2.%s MODIFICA PRENOTAZIONI DI UNA LEZIONE\n", VERDE, RESET);
        printf("%s 3.%s ELIMINA UNA PRENOTAZIONE\n", VERDE, RESET); 
        printf("%s 4.%s VISUALIZZA LISTA PRENOTAZIONI \n", VERDE, RESET);
        printf("%s 0. TORNA AL MENU GESTORE  %s\n", ROSSO, RESET); 
        printf("%s------------------------------%s\n", VIOLA, RESET);
        printf("%s  Inserisci la tua scelta: %s", GIALLO, RESET);

        // Verifica dell’input: deve essere un intero seguito da '\n'
        char extra;
        if (scanf("%d%c", &scelta, &extra) != 2 || extra != '\n') {
            pulisciSchermo();
            printf("%sATTENZIONE!! DEVI IMMETTERE 0, 1, 2, 3 O 4 A SECONDA DELLA TUA SCELTA. RIPROVA:%s\n", ROSSO, RESET);
            while (getchar() != '\n'); // Svuota il buffer in caso di input errato
            continue;
        }

        // Gestione della scelta tramite switch-case
        switch (scelta) {
            case 0:
                // Uscita dal menu
                pulisciSchermo();
                return;

            case 1:
                pulisciSchermo();
                aggiungi_prenotazione(&prenotazioni,abbonati,lezioni);
                break;

            case 2:
             break;
             case 3:
             break;

            case 4:
                prenotazione_printList(prenotazioni);
                getchar();
                break;

            default:
                // Scelta non valida
                pulisciSchermo();
                messaggio_errore();  // Messaggio di errore generico
        }
    }
}

void aggiungi_prenotazione(listP** lista, list* abbonati, listL* lezioni) {
    // Creazione di una nuova prenotazione
    prenotazione *nuova_prenotazione = creaPrenotazione();
    if (!nuova_prenotazione) {
        printf("Errore allocazione memoria per prenotazione\n");
        return;
    }

    // Recupera il massimo codice di prenotazione esistente e incrementalo
    int max_id = getMaxCodicePrenotazione(*lista);
    prenotazione_setCodicePrenotazione(nuova_prenotazione, max_id + 1);

    // Cerca l'abbonamento (utilizzando la funzione che restituisce un nodo)
    list* nodo_abbonamento = ricercaAbbonamento(abbonati);  // Restituisce list*
    if (nodo_abbonamento == NULL) {
        printf("Abbonamento non trovato.\n");
         free(nuova_prenotazione);
        return;
    }

    abbonamento* abbonamento_prenotazione = (abbonamento*) getValue(nodo_abbonamento);  // Estrae il valore

    // Imposta il codice abbonamento nella prenotazione
    prenotazione_setCodiceAbbonamento(nuova_prenotazione, getCodiceAbbonamento(abbonamento_prenotazione));

    // Chiedi all'utente di inserire la data della lezione
    printf("Data della lezione che vuoi prenotare: ");
    Data* data_prenotazione = leggiData();

    // Mostra le lezioni disponibili per quella data
    lezione_printByDate(lezioni, data_prenotazione);
    
    // Chiedi all'utente di inserire l'ID della lezione da prenotare
    printf("Inserisci il codice della lezione che vuoi prenotare (0 per annullare): ");
    int codice_lezione;
    scanf("%d", &codice_lezione);

    // Verifica se la lezione esiste per quel codice e data
    int esito = lezione_checkByID(lezioni,codice_lezione,data_prenotazione);
    if (esito == 1) {
        prenotazione_setCodiceLezione(nuova_prenotazione, codice_lezione);
    } else {
        printf("Errore: Lezione non trovata.\n");
        free(nuova_prenotazione);
        return;
        
    }

    // Inserisci la prenotazione nella lista
    *lista = prenotazione_consList(nuova_prenotazione, *lista);
}