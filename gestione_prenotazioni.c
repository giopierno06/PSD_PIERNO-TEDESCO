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

extern listP* lista_prenotazioni;
extern list* abbonati;
extern listL* lezioni;


void gestione_prenotazioni() {
    
    if (lista_prenotazioni == NULL)
    lista_prenotazioni = caricaPrenotazioniDaFile(lista_prenotazioni, "prenotazioni.txt");
    
    if (lista_prenotazioni == NULL)
        lista_prenotazioni = prenotazione_newList();
    
    
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
                aggiungi_prenotazione();
                break;

            case 2:
             break;
             case 3:
             break;

            case 4:
                visualizzaPrenotazioni( lista_prenotazioni,abbonati);
                getchar();
                break;

            default:
                // Scelta non valida
                pulisciSchermo();
                messaggio_errore();  // Messaggio di errore generico
        }
    }
}

void aggiungi_prenotazione() {
    extern listP* lista_prenotazioni;
    extern list* abbonati;
    extern listL* lezioni;

    // Creazione di una nuova prenotazione
    prenotazione* nuova_prenotazione = creaPrenotazione();
    if (!nuova_prenotazione) {
        printf("Errore allocazione memoria per prenotazione\n");
        return;
    }

    // Assegna un nuovo codice prenotazione
    int max_id = getMaxCodicePrenotazione(lista_prenotazioni);
    prenotazione_setCodicePrenotazione(nuova_prenotazione, max_id + 1);

    // Selezione abbonamento
    list* nodo_abbonamento = ricercaAbbonamento(abbonati);
    if (nodo_abbonamento == NULL) {
        printf("Abbonamento non trovato.\n");
        free(nuova_prenotazione);
        return;
    }

    abbonamento* abbonamento_prenotazione = (abbonamento*) getValue(nodo_abbonamento);

    // Controllo scadenza abbonamento
    if (abbonamento_isScaduto(abbonamento_prenotazione)) {
        printf("%sErrore: L'abbonamento è scaduto. Non puoi prenotare.%s\n",ROSSO,RESET);
        printf("Premi INVIO per tornare indietro..........");
        getchar();
        free(nuova_prenotazione);
        return;
    }

    prenotazione_setCodiceAbbonamento(nuova_prenotazione, getCodiceAbbonamento(abbonamento_prenotazione));

    // Inserisci data della lezione
    printf("Data della lezione che vuoi prenotare: ");
    Data* data_prenotazione = leggiData();

    // Mostra lezioni disponibili
    lezione_printByDate(lezioni, data_prenotazione);

    printf("Inserisci il codice della lezione che vuoi prenotare (0 per annullare): ");
    int codice_lezione;
    scanf("%d", &codice_lezione);

    if (codice_lezione == 0) {
        free(nuova_prenotazione);
        return;
    }

    // Controllo prenotazione duplicata (modificato: aggiunto parametro lezioni)
    if (prenotazione_esistePerAbbonamentoELezione(lista_prenotazioni, lezioni,
            getCodiceAbbonamento(abbonamento_prenotazione), codice_lezione, data_prenotazione)) {
        printf("%sErrore: Hai già una prenotazione per questa lezione in questa data.%s\n",ROSSO,RESET);
        getchar();
        printf("Premi INVIO per tornare indietro.......");
        getchar();
        free(nuova_prenotazione);
        return;
    }

    // Controlla esistenza lezione e posti disponibili
    int esito = lezione_checkByID(lezioni, codice_lezione, data_prenotazione);
    if (esito != 1) {
        printf("Errore: Lezione non trovata.\n");
        free(nuova_prenotazione);
        return;
    }

    // Ottieni la lezione e controlla disponibilità
    lezione* l = getLezioneByID(lezioni, codice_lezione);
    if (l == NULL) {
        printf("Errore interno: lezione non accessibile.\n");
        free(nuova_prenotazione);
        return;
    }

    if (getPostiOccupati(l) >= getPostiMax(l)) {
        printf("Errore: la lezione è già piena (%d/%d).\n", getPostiOccupati(l), getPostiMax(l));
        free(nuova_prenotazione);
        return;
    }

    // Incrementa i posti occupati
    addPostiOccupati(l);

    // Imposta la lezione sulla prenotazione
    prenotazione_setCodiceLezione(nuova_prenotazione, codice_lezione);

    // Conferma prenotazione
    printf("\n%s==================PRENOTAZIONE====================", VERDE);
    printf("\nConfermiamo la tua prenotazione con codice: %d\n", getCodicePrenotazione(nuova_prenotazione));
    abbonamento* temp = getAbbonamentoByID(abbonati, prenotazione_getCodiceAbbonamento(nuova_prenotazione));
    printf("A nome di %s %s ", getNome(temp), getCognome(temp));
    printf("Codice Abbonamento: %d\n", prenotazione_getCodiceAbbonamento(nuova_prenotazione));
    lezione* templ = getLezioneByID(lezioni, prenotazione_getCodiceLezione(nuova_prenotazione));
    printf("Alla lezione con codice: %d di %s in data : \n", prenotazione_getCodiceLezione(nuova_prenotazione), getNomeLezione(templ));
    stampaData(getDataLezione(templ));
    printf(" Alle ore: ");
    stampaOrario(getOrarioLezione(templ));
    printf("\n==================================================%s\n", RESET);

    getchar();
    printf("Premi INVIO per tornare indietro.....");
    getchar();
    pulisciSchermo();

    // Inserisci la prenotazione nella lista
    lista_prenotazioni = prenotazione_consList(nuova_prenotazione, lista_prenotazioni);

     salvaPrenotazioniSuFile(lista_prenotazioni, "prenotazioni.txt");
}

void visualizzaPrenotazioni(listP* lista_prenotazioni, list* lista_abbonati) {
    int scelta = -1;
    pulisciSchermo();
    printf("\n%s--- Visualizzazione Prenotazioni --- %s\n", BLU, RESET);
    printf("%s1.%s Visualizza %sTUTTE%s le prenotazioni\n", GIALLO, RESET, BLU, RESET);
    printf("%s2.%s Visualizza prenotazioni %sper lezione%s\n", GIALLO, RESET, BLU, RESET);
    printf("%s0.%s Torna indietro\n", GIALLO, RESET);

    while (1) {
        printf("%sScelta:%s ", BLU, RESET);
        if (scanf("%d", &scelta) != 1) {
            while(getchar() != '\n');  // pulizia buffer input non valido
            printf("%sInput non valido. Riprova.%s\n", ROSSO, RESET);
            continue;
        }
        // NON pulire qui il newline!

        switch (scelta) {
            case 1:
                pulisciSchermo();
                prenotazione_printList(lista_prenotazioni);
                printf("Premi INVIO per tornare indietro...");
                getchar();
                return;    // esce dalla funzione

            case 2: {
                int codice_lezione;
                printf("Inserisci il codice della lezione (0 per tornare indietro): ");
                if (scanf("%d", &codice_lezione) != 1) {
                    while(getchar() != '\n'); // pulizia buffer input non valido
                    printf("Input non valido.\n");
                    return;
                }
                while(getchar() != '\n'); // pulizia newline rimasto da scanf

                if (codice_lezione == 0) return;
                pulisciSchermo();
                visualizzaPrenotazioniLezione(codice_lezione, lista_prenotazioni, lista_abbonati);
                return;
            }

            case 0:
                return;

            default:
                printf("%sInput non valido. Riprova.%s\n", ROSSO, RESET);
                break;
        }
    }
}

void visualizzaPrenotazioniLezione(int codice_lezione, listP* lista_prenotazioni, list* lista_abbonati) {
    listP* corrente = lista_prenotazioni;
    int trovate = 0;

    // Recupera la lezione
    lezione* l = getLezioneByID(lezioni,codice_lezione);
    if (l == NULL) {
        printf("Lezione con codice %d non trovata.\n", codice_lezione);
        printf("Premi INVIO per tornare indietro...");
        return;
    }

    // Stampa dati della lezione
    stampaDettagliLezione(l);

    printf("%-20s %-10s %-15s %-15s\n", "Codice Prenotazione", "ID Utente", "Nome", "Cognome");
    printf("--------------------------------------------------------%s\n", RESET);

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);
        if (p != NULL && prenotazione_getCodiceLezione(p) == codice_lezione) {
            abbonamento* a = getAbbonamentoByID(lista_abbonati, prenotazione_getCodiceAbbonamento(p));
            if (a != NULL) {
                printf("%-20d %-10d %-15s %-15s\n",
                       getCodicePrenotazione(p),
                       prenotazione_getCodiceAbbonamento(p),
                       getNome(a),
                       getCognome(a));
                trovate++;
            }
        }
        corrente = prenotazione_getNext(corrente);
    }

    if (trovate == 0) {
        printf("Nessuna prenotazione trovata per questa lezione.\n");
    }

    printf("--------------------------------------------------------\n");
    printf("Premi INVIO per tornare indietro.....");
}