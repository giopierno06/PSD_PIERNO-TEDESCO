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


/*
 * Funzione: gestione_prenotazioni
 * -------------------------------
 * Questa funzione gestisce l'intero flusso di interazione per la gestione delle prenotazioni.
 *
 * - Mostra un menu testuale all'utente, con diverse opzioni per aggiungere, cancellare o visualizzare prenotazioni,
 *   oltre a visualizzare report mensili.
 * 
 * 
 * La funzione usa un ciclo infinito per mantenere attiva l'interfaccia finché l'utente non decide di uscire.
 * Le liste (prenotazioni, abbonamenti, lezioni) sono presumibilmente variabili globali o esterne a questa funzione.
 */
void gestione_prenotazioni() {
    // Carica la lista delle prenotazioni da file se non è già stata caricata
    if (lista_prenotazioni == NULL)
        lista_prenotazioni = caricaPrenotazioniDaFile(lista_prenotazioni, "prenotazioni.txt");

    // Se il file era vuoto o inesistente, crea una nuova lista vuota
    if (lista_prenotazioni == NULL)
        lista_prenotazioni = prenotazione_newList();

    // Carica la lista degli abbonati da file se non è ancora caricata
    if (abbonati == NULL)
        abbonati = caricaAbbonamentiDaFile(abbonati, "abbonamenti.txt");

    // Se il file era vuoto o inesistente, crea una nuova lista vuota per gli abbonati
    if (abbonati == NULL)
        abbonati = newList();

    // Carica la lista delle lezioni da file se non è ancora caricata
    if (lezioni == NULL)
        lezioni = caricaLezioniDaFile(lezioni, "lezioni.txt");

    // Se il file era vuoto o inesistente, crea una nuova lista vuota per le lezioni
    if (lezioni == NULL)
        lezioni = lezione_newList();

    char input[10];  // Buffer per leggere l'input dell'utente come stringa
    int scelta;      // Variabile per memorizzare la scelta convertita in intero

    // Ciclo infinito per mostrare il menu finché l'utente non sceglie di uscire (0)
    while (1) {
        pulisciSchermo(); // Pulisce lo schermo per una visualizzazione pulita

        // Stampa il menu gestionale per la gestione delle prenotazioni
        printf("\n%s=============================================%s\n", VIOLA, RESET);
        printf("%s   BENVENUTO NELLA GESTIONE PRENOTAZIONI   %s\n", GIALLO, RESET);
        printf("%s=============================================%s\n", VIOLA, RESET);
        printf("%s 1.%s PRENOTA UNA LEZIONE PER UN CLIENTE\n", VERDE, RESET);
        printf("%s 2.%s DISDICI UNA PRENOTAZIONE\n", VERDE, RESET);
        printf("%s 3.%s VISUALIZZA LISTA PRENOTAZIONI\n", VERDE, RESET);
        printf("%s 4.%s VISUALIZZA REPORT MENSILE\n", VERDE, RESET);
        printf("%s 0.%s TORNA AL MENU GESTORE\n", ROSSO, RESET);
        printf("%s------------------------------%s\n", VIOLA, RESET);
        printf("%s  Inserisci la tua scelta: %s", GIALLO, RESET);

        // Legge una linea di input dall'utente (stringa)
        if (!fgets(input, sizeof(input), stdin)) {
            // Se la lettura fallisce, mostra messaggio di errore e ricomincia il ciclo
            messaggio_errore();
            continue;
        }

        // Rimuove il carattere newline '\n' dalla stringa input, se presente
        input[strcspn(input, "\n")] = 0;

        // Converte la stringa input in un numero intero, se possibile
        if (sscanf(input, "%d", &scelta) != 1) {
            // Se la conversione fallisce, mostra messaggio di errore e ricomincia
            messaggio_errore();
            continue;
        }

        // Gestisce la scelta dell'utente usando uno switch
        switch (scelta) {
            case 0:
                // Scelta 0: esce dal menu gestione prenotazioni e torna al menu gestore
                pulisciSchermo();
                return;

            case 1:
                // Scelta 1: aggiungi una nuova prenotazione
                pulisciSchermo();
                aggiungi_prenotazione(-1,-1);
                break;

            case 2:
                // Scelta 2: disdici una prenotazione esistente
                pulisciSchermo();
                disdici_prenotazione();
                break;

            case 3:
                // Scelta 3: visualizza la lista di tutte le prenotazioni
                pulisciSchermo();
                visualizzaPrenotazioni(lista_prenotazioni, abbonati);
                // Attende che l'utente prema invio prima di tornare al menu
                printf("\nPremi invio per tornare al menu...");
                fgets(input, sizeof(input), stdin);
                break;

            case 4:
                // Scelta 4: visualizza il report mensile delle prenotazioni
                pulisciSchermo();
                reportGestore(lista_prenotazioni, lezioni);
                // Attende che l'utente prema invio prima di tornare al menu
                printf("\nPremi invio per tornare al menu...");
                fgets(input, sizeof(input), stdin);
                break;

            default:
                // Se la scelta non è valida, mostra un messaggio di errore
                messaggio_errore();
                break;
        }
    }
}

/*
 * aggiungi_prenotazione:
 * Crea e aggiunge una nuova prenotazione, 
 * controllando validità abbonamento, data e disponibilità posti.
 */
void aggiungi_prenotazione(int codice_abbonamento_param, int codice_lezione_param) {
    extern listP* lista_prenotazioni;
    extern list* abbonati;
    extern listL* lezioni;
    
    

    prenotazione* nuova_prenotazione = creaPrenotazione();
    if (!nuova_prenotazione) {
        printf("Errore allocazione memoria per prenotazione\n");
        return;
    }

    int max_id = getMaxCodicePrenotazione(lista_prenotazioni);
    prenotazione_setCodicePrenotazione(nuova_prenotazione, max_id + 1);

    abbonamento* abbonamento_prenotazione = NULL;

    if (codice_abbonamento_param == -1) {
        // Comportamento standard: cerca abbonamento tramite input
        list* nodo_abbonamento = ricercaAbbonamento(abbonati);
        if (nodo_abbonamento == NULL) {
            printf("Abbonamento non trovato.\n");
            prenotazione_distruggi(nuova_prenotazione);
            return;
        }
        abbonamento_prenotazione = (abbonamento*) getValue(nodo_abbonamento);
    } else {
        // Cerca abbonamento per codice passato
        abbonamento_prenotazione = getAbbonamentoByID(abbonati, codice_abbonamento_param);
        if (abbonamento_prenotazione == NULL) {
            printf("Abbonamento con codice %d non trovato.\n", codice_abbonamento_param);
            prenotazione_distruggi(nuova_prenotazione);
            return;
        }
    }

    if (abbonamento_isScaduto(abbonamento_prenotazione)) {
        printf("%sErrore: L'abbonamento è scaduto.%s\n", ROSSO, RESET);
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    prenotazione_setCodiceAbbonamento(nuova_prenotazione, getCodiceAbbonamento(abbonamento_prenotazione));

    Data* data_prenotazione = NULL;

    if (codice_lezione_param == -1) {
        // Legge data da input
        printf("Data della lezione che vuoi prenotare: ");
        data_prenotazione = leggiData();

        if (confrontaDate(data_prenotazione, oggi()) == -1) {
            printf("%sErrore: Data passata.%s\n", ROSSO, RESET);
            prenotazione_distruggi(nuova_prenotazione);
            return;
        }
    } else {
        // Se codice lezione passato, recupera la data della lezione
        lezione* l = getLezioneByID(lezioni, codice_lezione_param);
        if (!l) {
            printf("Lezione con codice %d non trovata.\n", codice_lezione_param);
            prenotazione_distruggi(nuova_prenotazione);
            return;
        }
        data_prenotazione = getDataLezione(l);

        if (confrontaDate(data_prenotazione, oggi()) == -1) {
            printf("%sErrore: La data della lezione è passata.%s\n", ROSSO, RESET);
            prenotazione_distruggi(nuova_prenotazione);
            return;
        }
    }

    Data* inizio_abbonamento = getDataInizio(abbonamento_prenotazione);
    int durata_mesi = getDurata(abbonamento_prenotazione);
    Data* scadenza_abbonamento = aggiungiMesi(inizio_abbonamento, durata_mesi);

    if (confrontaDate(data_prenotazione, scadenza_abbonamento) > 0) {
        printf("%sErrore: Abbonamento scaduto alla data scelta.%s\n", ROSSO, RESET);
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    if (codice_lezione_param == -1) {
        lezione_printByDate(lezioni, data_prenotazione);
        printf("Inserisci il codice della lezione (0 per annullare): ");
        scanf("%d", &codice_lezione_param);

        if (codice_lezione_param == 0) {
            prenotazione_distruggi(nuova_prenotazione);
            return;
        }
    }

    if (prenotazione_esistePerAbbonamentoELezione(lista_prenotazioni, lezioni,
        getCodiceAbbonamento(abbonamento_prenotazione), codice_lezione_param, data_prenotazione)) {
        printf("%sErrore: Prenotazione duplicata.%s\n", ROSSO, RESET);
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    int esito = lezione_checkByID(lezioni, codice_lezione_param, data_prenotazione);
    if (esito != 1) {
        printf("Errore: Lezione non trovata o non disponibile.\n");
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    lezione* l = getLezioneByID(lezioni, codice_lezione_param);
    if (!l) {
        printf("Errore interno: lezione non accessibile.\n");
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    if (getPostiOccupati(l) >= getPostiMax(l)) {
        printf("Errore: Lezione piena (%d/%d).\n", getPostiOccupati(l), getPostiMax(l));
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    addPostiOccupati(l);
    prenotazione_setCodiceLezione(nuova_prenotazione, codice_lezione_param);

    // Stampa conferma prenotazione (puoi adattare come preferisci)
    printf("\n%s=== PRENOTAZIONE CONFERMATA ===%s\n", VERDE, RESET);
    printf("Codice: %d\n", getCodicePrenotazione(nuova_prenotazione));
    printf("Cliente: %s %s\n", getNome(abbonamento_prenotazione), getCognome(abbonamento_prenotazione));
    printf("Codice Abbonamento: %d\n", getCodiceAbbonamento(abbonamento_prenotazione));
    printf("Lezione: %s (codice %d) in data ", getNomeLezione(l), codice_lezione_param);
    stampaData(data_prenotazione);
    printf(" alle ore ");
    stampaOrario(getOrarioLezione(l));
    printf("\n==============================\n");
    getchar();
    printf("Premi INVIO per continuare.....\n");
    getchar();

    

    // Aggiunge alla lista e salva
    lista_prenotazioni = prenotazione_consList(nuova_prenotazione, lista_prenotazioni);
    salvaPrenotazioniSuFile(lista_prenotazioni, "prenotazioni.txt");
    salvaLezioniSuFile(lezioni, "lezioni.txt");
}

/*
 * disdici_prenotazione:
 * Permette di annullare una prenotazione associata a un abbonamento,
 * aggiornando i posti disponibili nella lezione.
 */
void disdici_prenotazione() {
    extern listP* lista_prenotazioni;
    extern list* abbonati;
    extern listL* lezioni;

    pulisciSchermo();

    // Controlla se ci sono prenotazioni
    if (lista_prenotazioni == NULL) {
        printf("%s[INFO]%s Nessuna prenotazione registrata.\n", GIALLO, RESET);
        printf("Premi INVIO per tornare indietro...");
        getchar();
        return;
    }

    // Ricerca abbonamento utente
    list* nodo_abbonamento = ricercaAbbonamento(abbonati);
    if (nodo_abbonamento == NULL) {
        printf("%s[ERRORE]%s Abbonamento non trovato.\n", ROSSO, RESET);
        printf("Premi INVIO per tornare indietro...");
        getchar();
        return;
    }

    abbonamento* ab = (abbonamento*) getValue(nodo_abbonamento);
    int codice_abbonamento = getCodiceAbbonamento(ab);

    // Mostra tutte le prenotazioni di questo abbonamento
    printf("\n%sPrenotazioni per %s %s:%s\n", BLU, getNome(ab), getCognome(ab), RESET);
    int trovato = 0;
    listP* corrente = lista_prenotazioni;

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);
        if (prenotazione_getCodiceAbbonamento(p) == codice_abbonamento) {
            lezione* l = getLezioneByID(lezioni, prenotazione_getCodiceLezione(p));
            if (l != NULL) {
                printf("  → Codice Prenotazione: %d | Lezione: %-20s | Data: ",
                       getCodicePrenotazione(p), getNomeLezione(l));
                stampaData(getDataLezione(l));
                printf("\n");
                trovato = 1;
            }
        }
        corrente = prenotazione_getNext(corrente);
    }

    // Se non trova prenotazioni da disdire, esce
    if (!trovato) {
        printf("%s[INFO]%s Nessuna prenotazione trovata.\n", GIALLO, RESET);
        printf("Premi INVIO per tornare indietro...");
        getchar();
        return;
    }

    // Chiede codice prenotazione da annullare
    printf("\nInserisci il codice della prenotazione da disdire (0 per annullare): ");
    int codice_da_disdire;
    scanf("%d", &codice_da_disdire);
    getchar();

    if (codice_da_disdire == 0) return;

    // Cerca la prenotazione da annullare
    listP* precedente = NULL;
    corrente = lista_prenotazioni;

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);

        if (getCodicePrenotazione(p) == codice_da_disdire &&
            prenotazione_getCodiceAbbonamento(p) == codice_abbonamento) {

            // Aggiorna posti occupati della lezione
            lezione* l = getLezioneByID(lezioni, prenotazione_getCodiceLezione(p));
            if (l != NULL) {
                removePostiOccupati(l);
            }

            // Rimuove la prenotazione dalla lista
            if (precedente == NULL) {
                lista_prenotazioni = prenotazione_getNext(corrente);
            } else {
                prenotazione_setNext(precedente, prenotazione_getNext(corrente));
            }

            prenotazione_distruggi(p);
            free(corrente);

            printf("\n%s[SUCCESSO]%s Prenotazione annullata correttamente.\n", VERDE, RESET);

            // Salva modifiche su file
            salvaPrenotazioniSuFile(lista_prenotazioni, "prenotazioni.txt");
            salvaLezioniSuFile(lezioni, "lezioni.txt");

            printf("Premi INVIO per tornare indietro...");
            getchar();
            pulisciSchermo();
            return;
        }

        precedente = corrente;
        corrente = prenotazione_getNext(corrente);
    }

    // Se non trova la prenotazione da disdire
    printf("\n%s[ERRORE]%s Prenotazione non trovata o non appartenente al suo abbonamento.\n", ROSSO, RESET);
    printf("Premi INVIO per tornare indietro...");
    getchar();
}


/*
 * visualizzaPrenotazioni:
 * Mostra un menu per visualizzare le prenotazioni in modi diversi:
 * 1) Tutte le prenotazioni
 * 2) Prenotazioni filtrate per codice lezione
 * 3) Prenotazioni filtrate per codice abbonamento (cliente)
 */
void visualizzaPrenotazioni(listP* lista_prenotazioni, list* lista_abbonati) {
    int scelta = -1;
    extern listL* lezioni;  // lista lezioni esterna

    pulisciSchermo();

    // Stampa menu
    printf("\n%s--- Visualizzazione Prenotazioni --- %s\n", BLU, RESET);
    printf("%s1.%s Visualizza %sTUTTE%s le prenotazioni\n", GIALLO, RESET, BLU, RESET);
    printf("%s2.%s Visualizza prenotazioni %sper lezione%s\n", GIALLO, RESET, BLU, RESET);
    printf("%s3.%s Visualizza prenotazioni %sper cliente%s\n", GIALLO, RESET, BLU, RESET);
    printf("%s0.%s Torna indietro\n", GIALLO, RESET);

    while (1) {
        printf("%sScelta:%s ", BLU, RESET);

        // Legge input, se non è un intero riprova
        if (scanf("%d", &scelta) != 1) {
            while(getchar() != '\n');  // Pulisce buffer input non valido
            printf("%sInput non valido. Riprova.%s\n", ROSSO, RESET);
            continue;
        }
        // NON pulire qui il newline perché alcune funzioni lo richiedono

        switch (scelta) {
            case 1:
                pulisciSchermo();
                // Mostra tutte le prenotazioni
                prenotazione_printList(lista_prenotazioni);
                printf("Premi INVIO per tornare indietro...");
                getchar();  // attende invio
                return;     // esce dalla funzione

            case 2: {
                int codice_lezione;
                printf("Inserisci il codice della lezione (0 per tornare indietro): ");
                if (scanf("%d", &codice_lezione) != 1) {
                    while(getchar() != '\n'); // Pulisce input non valido
                    printf("Input non valido.\n");
                    return;
                }
                while(getchar() != '\n'); // Pulisce newline rimasto

                if (codice_lezione == 0) return; // torna indietro se 0

                pulisciSchermo();
                // Mostra prenotazioni filtrate per lezione
                visualizzaPrenotazioniLezione(codice_lezione, lista_prenotazioni, lista_abbonati);
                return;
            }

            case 3: {
                int codice_abbonamento;
                printf("Inserisci il codice dell'abbonamento (0 per tornare indietro): ");
                if (scanf("%d", &codice_abbonamento) != 1) {
                    while(getchar() != '\n'); // Pulisce input non valido
                    printf("Input non valido.\n");
                    return;
                }
                while(getchar() != '\n'); // Pulisce newline rimasto

                if (codice_abbonamento == 0) return; // torna indietro se 0

                pulisciSchermo();
                // Mostra prenotazioni filtrate per cliente
                visualizzaPrenotazioniCliente(codice_abbonamento, lista_prenotazioni, lezioni);
                return;
            }

            case 0:
                // Torna indietro
                return;

            default:
                // Input non valido, riprova
                printf("%sInput non valido. Riprova.%s\n", ROSSO, RESET);
                break;
        }
    }
}

// Visualizza tutte le prenotazioni associate a una specifica lezione
void visualizzaPrenotazioniLezione(int codice_lezione, listP* lista_prenotazioni, list* lista_abbonati) {
    listP* corrente = lista_prenotazioni;
    int trovate = 0;

    // Cerca la lezione con quel codice
    lezione* l = getLezioneByID(lezioni, codice_lezione);
    if (l == NULL) {
        printf("Lezione con codice %d non trovata.\n", codice_lezione);
        printf("Premi INVIO per tornare indietro...");
        return;
    }

    // Stampa dettagli della lezione
    stampaDettagliLezione(l);

    // Intestazione tabella
    printf("%-20s %-10s %-15s %-15s\n", "Codice Prenotazione", "ID Utente", "Nome", "Cognome");
    printf("--------------------------------------------------------%s\n", RESET);

    // Scorre tutte le prenotazioni
    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);

        // Se la prenotazione è per la lezione richiesta
        if (p != NULL && prenotazione_getCodiceLezione(p) == codice_lezione) {
            // Cerca l'abbonamento dell'utente
            abbonamento* a = getAbbonamentoByID(lista_abbonati, prenotazione_getCodiceAbbonamento(p));
            if (a != NULL) {
                // Stampa dati prenotazione e utente
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

// Visualizza le prenotazioni di un cliente specifico, mostrando solo quelle con data futura o odierna
void visualizzaPrenotazioniCliente(int codice_abbonamento, listP* lista_prenotazioni, listL* lista_lezioni) {
    listP* corrente = lista_prenotazioni;
    int trovate = 0;

    printf("%sLE TUE PRENOTAZIONI\n", GIALLO);
    printf("%-20s %-20s %-15s %-12s %-10s\n", "Codice Prenotazione", "Nome Lezione", "Disciplina", "Data", "Orario");
    printf("-------------------------------------------------------------------------------%s\n", RESET);

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);

        // Se la prenotazione appartiene al cliente
        if (p != NULL && prenotazione_getCodiceAbbonamento(p) == codice_abbonamento) {
            int codice_lezione = prenotazione_getCodiceLezione(p);
            lezione* l = getLezioneByID(lista_lezioni, codice_lezione);

            if (l != NULL) {
                Data* data_lezione = getDataLezione(l);

                // Mostra solo lezioni odierne o future
                if (confrontaDate(data_lezione, oggi()) >= 0) {
                    printf("%-20d %-20s %-15s ", 
                           getCodicePrenotazione(p),
                           getNomeLezione(l),
                           getDisciplinaLezione(l));

                    stampaData(data_lezione);
                    printf("   ");
                    stampaOrario(getOrarioLezione(l));
                    printf("\n");

                    trovate++;
                }
            }
        }
        corrente = prenotazione_getNext(corrente);
    }

    if (trovate == 0) {
        printf("Nessuna lezione prenotata trovata per questo cliente.\n");
    }

    printf("-------------------------------------------------------------------------------\n");
    printf("Premi INVIO per tornare indietro.....");
    getchar();
}

// Report delle lezioni svolte da un cliente negli ultimi 30 giorni
void reportLezioniUltimoMeseCliente(int codice_abbonamento, listP* lista_prenotazioni, listL* lista_lezioni) {
    listP* corrente = lista_prenotazioni;
    int trovate = 0;
    Data* inizioPeriodo = sottraiGiorni(oggi(), 30);  // Data 30 giorni fa

    printf("%sREPORT LEZIONI - ULTIMO MESE\n", GIALLO);
    printf("%-20s %-20s %-15s %-12s %-10s\n", "Codice Prenotazione", "Nome Lezione", "Disciplina", "Data", "Orario");
    printf("-------------------------------------------------------------------------------%s\n", RESET);

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);

        // Solo prenotazioni del cliente
        if (p != NULL && prenotazione_getCodiceAbbonamento(p) == codice_abbonamento) {
            int codice_lezione = prenotazione_getCodiceLezione(p);
            lezione* l = getLezioneByID(lista_lezioni, codice_lezione);

            if (l != NULL) {
                Data* data_lezione = getDataLezione(l);

                // Solo lezioni tra 30 giorni fa e oggi inclusi
                if (confrontaDate(data_lezione, inizioPeriodo) >= 0 && confrontaDate(data_lezione, oggi()) <= 0) {
                    printf("%-20d %-20s %-15s ", 
                           getCodicePrenotazione(p),
                           getNomeLezione(l),
                           getDisciplinaLezione(l));
                    
                    stampaData(data_lezione);
                    printf("   ");
                    stampaOrario(getOrarioLezione(l));
                    printf("\n");

                    trovate++;
                }
            }
        }
        corrente = prenotazione_getNext(corrente);
    }

    if (trovate == 0) {
        printf("Nessuna lezione svolta nell'ultimo mese trovata per questo cliente.\n");
    }

    printf("-------------------------------------------------------------------------------\n");
    printf("Premi INVIO per tornare indietro.....");
    getchar();
}

// Report generale delle discipline più frequentate nell'ultimo mese (tutte le lezioni)
void reportDisciplineUltimoMese(listP* lista_prenotazioni, listL* lista_lezioni) {
    // Array per tenere traccia di discipline e conteggi
    char* discipline[100];
    int conteggi[100];
    int num_discipline = 0;

    Data* data_limite = sottraiGiorni(oggi(), 30);  // Data 30 giorni fa
    Data* data_oggi = oggi();

    listP* corrente = lista_prenotazioni;

    // Scorre tutte le prenotazioni
    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);
        if (p != NULL) {
            int codice_lezione = prenotazione_getCodiceLezione(p);
            lezione* l = getLezioneByID(lista_lezioni, codice_lezione);

            if (l != NULL) {
                Data* data_lezione = getDataLezione(l);

                // Considera solo lezioni nell'ultimo mese
                if (confrontaDate(data_lezione, data_limite) >= 0 && confrontaDate(data_lezione, data_oggi) <= 0) {
                    const char* disciplina = getDisciplinaLezione(l);
                    int trovata = 0;

                    // Cerca se la disciplina è già in lista
                    for (int i = 0; i < num_discipline; i++) {
                        if (strcmp(discipline[i], disciplina) == 0) {
                            conteggi[i]++;
                            trovata = 1;
                            break;
                        }
                    }

                    // Se nuova disciplina, la aggiunge
                    if (!trovata && num_discipline < 100) {
                        discipline[num_discipline] = strdup(disciplina);
                        conteggi[num_discipline] = 1;
                        num_discipline++;
                    }
                }
            }
        }
        corrente = prenotazione_getNext(corrente);
    }

    // Ordina discipline per numero di prenotazioni in ordine decrescente
    for (int i = 0; i < num_discipline - 1; i++) {
        for (int j = i + 1; j < num_discipline; j++) {
            if (conteggi[j] > conteggi[i]) {
                // Scambia conteggi
                int temp_count = conteggi[i];
                conteggi[i] = conteggi[j];
                conteggi[j] = temp_count;

                // Scambia discipline
                char* temp_str = discipline[i];
                discipline[i] = discipline[j];
                discipline[j] = temp_str;
            }
        }
    }

    // Stampa il report delle discipline
    printf("%sREPORT MENSILE GENERALE PER DISCIPLINA\n", GIALLO);
    printf("%-30s %-10s\n", "Disciplina", "Prenotazioni");
    printf("--------------------------------------------%s\n", RESET);

    if (num_discipline == 0) {
        printf("Nessuna lezione svolta nell'ultimo mese.\n");
    } else {
        for (int i = 0; i < num_discipline; i++) {
            printf("%-30s %-10d\n", discipline[i], conteggi[i]);
        }
    }

    printf("--------------------------------------------\n");
    printf("Premi INVIO per tornare indietro.....");
    getchar();

    // Libera la memoria allocata con strdup
    for (int i = 0; i < num_discipline; i++) {
        free(discipline[i]);
    }
}

/**
 * Funzione che gestisce il menu dei report per il gestore.
 * Permette di scegliere tra:
 *  1. Visualizzare il report delle lezioni svolte da un singolo cliente nell'ultimo mese.
 *  2. Visualizzare il report generale delle discipline con il numero di prenotazioni nell'ultimo mese.
 * L'utente può anche tornare indietro al menu principale.
 */
void reportGestore(listP* lista_prenotazioni, listL* lista_lezioni) {
    int scelta;
    int codice_abbonamento;

    do {
        system("clear"); // o system("cls") su Windows
        printf("%s======== MENU REPORT GESTORE ========%s\n", GIALLO, RESET);
        printf("1. Visualizza report di un cliente\n");
        printf("2. Visualizza report delle discipline (ultimo mese)\n");
        printf("%s0. Torna indietro%s\n",ROSSO,RESET);
        printf("%s===================================================%s\n",GIALLO,RESET);
        printf("Scelta: ");
        scanf("%d", &scelta);
        getchar(); // Consuma l'INVIO

        switch (scelta) {
            case 1:
                printf("Inserisci il codice abbonamento del cliente: ");
                scanf("%d", &codice_abbonamento);
                getchar(); // Consuma l'INVIO
                reportLezioniUltimoMeseCliente(codice_abbonamento, lista_prenotazioni, lista_lezioni);
                break;

            case 2:
                reportDisciplineUltimoMese(lista_prenotazioni, lezioni);
                break;

            case 0:
                printf("Torno al menu principale...\n");
                break;

            default:
                printf("%sScelta non valida. Riprova.%s\n",ROSSO,RESET);
                break;
        }

    } while (scelta != 0);
}

/**
 * Funzione che permette a un cliente di aggiungere una nuova prenotazione.
 * Viene verificata la validità dell'abbonamento (non scaduto),
 * la validità della data scelta (non passata e entro la scadenza dell'abbonamento),
 * la disponibilità della lezione e posti disponibili.
 * Se tutti i controlli sono superati, la prenotazione viene aggiunta alla lista e salvata su file.
 */
void aggiungi_prenotazioneCliente(abbonamento* abbonamento_prenotazione) {
    extern listP* lista_prenotazioni;
    extern list* abbonati;
    extern listL* lezioni;

    // Creazione nuova prenotazione
    prenotazione* nuova_prenotazione = creaPrenotazione();
    if (!nuova_prenotazione) {
        printf("Errore allocazione memoria per prenotazione\n");
        return;
    }

    // Assegna codice prenotazione incrementale
    int max_id = getMaxCodicePrenotazione(lista_prenotazioni);
    prenotazione_setCodicePrenotazione(nuova_prenotazione, max_id + 1);

    // Controlla scadenza abbonamento
    if (abbonamento_isScaduto(abbonamento_prenotazione)) {
        printf("%sErrore: L'abbonamento è scaduto. Non puoi prenotare.%s\n", ROSSO, RESET);
        getchar();
        printf("Premi INVIO per tornare indietro..........");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    prenotazione_setCodiceAbbonamento(nuova_prenotazione, getCodiceAbbonamento(abbonamento_prenotazione));

    // Leggi data prenotazione e controlla che non sia passata
    printf("Data della lezione che vuoi prenotare: ");
    Data* data_prenotazione = leggiData();

    if (confrontaDate(data_prenotazione, oggi()) == -1) {
        printf("%sErrore: Questa data è già passata. Non puoi prenotare.%s\n", ROSSO, RESET);
        getchar();
        printf("Premi INVIO per tornare indietro..........");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Calcola data scadenza abbonamento
    Data* inizio_abbonamento = getDataInizio(abbonamento_prenotazione);
    int durata_mesi = getDurata(abbonamento_prenotazione);
    Data* scadenza_abbonamento = aggiungiMesi(inizio_abbonamento, durata_mesi);

    // Controlla che la data prenotazione non superi la scadenza abbonamento
    if (confrontaDate(data_prenotazione, scadenza_abbonamento) > 0) {
        printf("%sErrore: L'abbonamento sarà scaduto alla data della lezione scelta.%s\n", ROSSO, RESET);
        printf("Non puoi prenotare una lezione oltre la scadenza dell'abbonamento.\n");
        getchar();
        printf("Premi INVIO per tornare indietro......");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Mostra lezioni disponibili in quella data
    lezione_printByDate(lezioni, data_prenotazione);

    printf("Inserisci il codice della lezione che vuoi prenotare (0 per annullare): ");
    int codice_lezione;
    scanf("%d", &codice_lezione);
    getchar();

    if (codice_lezione == 0) {
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Controlla prenotazione duplicata
    if (prenotazione_esistePerAbbonamentoELezione(lista_prenotazioni, lezioni,
            getCodiceAbbonamento(abbonamento_prenotazione), codice_lezione, data_prenotazione)) {
        printf("%sErrore: Hai già una prenotazione per questa lezione in questa data.%s\n", ROSSO, RESET);
        printf("Premi INVIO per tornare indietro.......\n");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Controlla esistenza lezione e posti disponibili
    int esito = lezione_checkByID(lezioni, codice_lezione, data_prenotazione);
    if (esito != 1) {
        printf("Errore: Lezione non trovata o non disponibile in questa data.\n");
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    lezione* l = getLezioneByID(lezioni, codice_lezione);
    if (l == NULL) {
        printf("Errore interno: lezione non accessibile.\n");
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    if (getPostiOccupati(l) >= getPostiMax(l)) {
        printf("Errore: la lezione è già piena (%d/%d).\n", getPostiOccupati(l), getPostiMax(l));
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Incrementa posti occupati nella lezione selezionata
    addPostiOccupati(l);

    // Imposta codice lezione sulla prenotazione
    prenotazione_setCodiceLezione(nuova_prenotazione, codice_lezione);

    // Conferma prenotazione stampando i dettagli all'utente
    printf("\n%s==================PRENOTAZIONE====================%s\n", VERDE, RESET);
    printf("Confermiamo la tua prenotazione con codice: %d\n", getCodicePrenotazione(nuova_prenotazione));

    abbonamento* temp = getAbbonamentoByID(abbonati, prenotazione_getCodiceAbbonamento(nuova_prenotazione));
    printf("A nome di %s %s\n", getNome(temp), getCognome(temp));
    printf("Codice Abbonamento: %d\n", prenotazione_getCodiceAbbonamento(nuova_prenotazione));

    lezione* templ = getLezioneByID(lezioni, prenotazione_getCodiceLezione(nuova_prenotazione));
    printf("Alla lezione con codice: %d di %s in data: ", prenotazione_getCodiceLezione(nuova_prenotazione), getNomeLezione(templ));
    stampaData(getDataLezione(templ));
    printf(" Alle ore: ");
    stampaOrario(getOrarioLezione(templ));
    printf("\n==================================================\n");

    printf("Premi INVIO per tornare indietro.....");
    getchar();
    pulisciSchermo();

    // Inserisci la prenotazione creata nella lista globale delle prenotazioni
    lista_prenotazioni = prenotazione_consList(nuova_prenotazione, lista_prenotazioni);

    // Salva i dati aggiornati su file
    salvaPrenotazioniSuFile(lista_prenotazioni, "prenotazioni.txt");
    salvaLezioniSuFile(lezioni, "lezioni.txt");
}

/**
 * Funzione che permette al cliente di disdire una prenotazione esistente.
 * La funzione andrà implementata per:
 *  - Visualizzare le prenotazioni correnti del cliente
 *  - Permettere la selezione di una prenotazione da annullare
 *  - Aggiornare la lista prenotazioni e posti occupati nella lezione
 *  - Salvare i dati aggiornati su file
 */
void disdici_prenotazioneCliente(abbonamento* abbonamento_prenotazione) {
    extern listP* lista_prenotazioni;
    extern listL* lezioni;

    pulisciSchermo();

    if (lista_prenotazioni == NULL) {
        printf("%s[INFO]%s Nessuna prenotazione registrata.\n", GIALLO, RESET);
        printf("Premi INVIO per tornare indietro.....");
        getchar();
        return;
    }

    int codice_abbonamento = getCodiceAbbonamento(abbonamento_prenotazione);

    // Mostra solo prenotazioni future o odierne
    printf("\n%sPrenotazioni attive (future) per %s %s:%s\n", BLU, getNome(abbonamento_prenotazione), getCognome(abbonamento_prenotazione), RESET);
    int trovato = 0;
    listP* corrente = lista_prenotazioni;

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);
        if (prenotazione_getCodiceAbbonamento(p) == codice_abbonamento) {
            lezione* l = getLezioneByID(lezioni, prenotazione_getCodiceLezione(p));
            if (l != NULL && confrontaDate(getDataLezione(l), oggi()) >= 0) {
                printf("  → Codice Prenotazione: %d | Lezione: %-20s | Data: ",
                       getCodicePrenotazione(p), getNomeLezione(l));
                stampaData(getDataLezione(l));
                printf("\n");
                trovato = 1;
            }
        }
        corrente = prenotazione_getNext(corrente);
    }

    if (!trovato) {
        printf("%s[INFO]%s Nessuna prenotazione futura trovata per questo abbonamento.\n", GIALLO, RESET);
        printf("Premi INVIO per tornare indietro.....");
        getchar();
        return;
    }

    printf("\nInserisci il codice della prenotazione da disdire (0 per annullare): ");
    int codice_da_disdire;
    scanf("%d", &codice_da_disdire);
    getchar();

    if (codice_da_disdire == 0) return;

    // Cerca e rimuove la prenotazione futura
    listP* precedente = NULL;
    corrente = lista_prenotazioni;

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);
        lezione* l = getLezioneByID(lezioni, prenotazione_getCodiceLezione(p));

        if (getCodicePrenotazione(p) == codice_da_disdire &&
            prenotazione_getCodiceAbbonamento(p) == codice_abbonamento &&
            l != NULL &&
            confrontaDate(getDataLezione(l), oggi()) >= 0) {

            // Riduci i posti occupati della lezione
            removePostiOccupati(l);

            // Rimuovi il nodo dalla lista
            if (precedente == NULL) {
                lista_prenotazioni = prenotazione_getNext(corrente);
            } else {
                prenotazione_setNext(precedente, prenotazione_getNext(corrente));
            }

            prenotazione_distruggi(p);
            free(corrente);

            printf("\n%s[SUCCESSO]%s Prenotazione annullata correttamente.\n", VERDE, RESET);

            salvaPrenotazioniSuFile(lista_prenotazioni, "prenotazioni.txt");
            salvaLezioniSuFile(lezioni, "lezioni.txt");

            printf("Premi INVIO per tornare indietro.....");
            getchar();
            pulisciSchermo();
            return;
        }

        precedente = corrente;
        corrente = prenotazione_getNext(corrente);
    }

    printf("\n%s[ERRORE]%s Prenotazione non trovata, scaduta o non appartiene al tuo abbonamento.\n", ROSSO, RESET);
    printf("Premi INVIO per tornare indietro.....");
    getchar();
}