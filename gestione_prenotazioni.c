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
        printf("%s 2.%s DISDICI UNA PRENOTAZIONE\n", VERDE, RESET); 
        printf("%s 3.%s VISUALIZZA LISTA PRENOTAZIONI \n", VERDE, RESET);
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
                  disdici_prenotazione();
                  break;
             
             case 3:
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
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    abbonamento* abbonamento_prenotazione = (abbonamento*) getValue(nodo_abbonamento);

    // Controllo scadenza abbonamento alla data odierna
    if (abbonamento_isScaduto(abbonamento_prenotazione)) {
        printf("%sErrore: L'abbonamento è scaduto. Non puoi prenotare.%s\n", ROSSO, RESET);
        printf("Premi INVIO per tornare indietro..........");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    prenotazione_setCodiceAbbonamento(nuova_prenotazione, getCodiceAbbonamento(abbonamento_prenotazione));

    // Inserisci data della lezione con controllo se la data e maggiore o uguale all'odierna
    printf("Data della lezione che vuoi prenotare: ");
    Data* data_prenotazione = leggiData();
    if(confrontaDate(data_prenotazione,oggi())==-1){
        printf("%sErrore:Questa data è gia passata. Non puoi prenotare.%s\n", ROSSO, RESET);
        getchar();
        printf("Premi INVIO per tornare indietro..........");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }


    // Calcola la data di scadenza dell'abbonamento (inizio + durata)
    Data* inizio_abbonamento = getDataInizio(abbonamento_prenotazione);
    int durata_mesi = getDurata(abbonamento_prenotazione);
    Data* scadenza_abbonamento = aggiungiMesi(inizio_abbonamento, durata_mesi);

    // Controllo che la data della lezione non sia dopo la scadenza
    if (confrontaDate(data_prenotazione, scadenza_abbonamento) > 0) {
        printf("%sErrore: L'abbonamento sarà scaduto alla data della lezione scelta.%s\n", ROSSO, RESET);
        printf("Non puoi prenotare una lezione oltre la scadenza dell'abbonamento.\n");
        getchar();
        printf("Premi INVIO per tornare indietro......");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Mostra lezioni disponibili
    lezione_printByDate(lezioni, data_prenotazione);

    printf("Inserisci il codice della lezione che vuoi prenotare (0 per annullare): ");
    int codice_lezione;
    scanf("%d", &codice_lezione);

    if (codice_lezione == 0) {
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Controllo prenotazione duplicata
    if (prenotazione_esistePerAbbonamentoELezione(lista_prenotazioni, lezioni,
            getCodiceAbbonamento(abbonamento_prenotazione), codice_lezione, data_prenotazione)) {
        printf("%sErrore: Hai già una prenotazione per questa lezione in questa data.%s\n", ROSSO, RESET);
        getchar();
        printf("Premi INVIO per tornare indietro.......");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Controlla esistenza lezione e posti disponibili
    int esito = lezione_checkByID(lezioni, codice_lezione, data_prenotazione);
    if (esito != 1) {
        printf("Errore: Lezione non trovata.\n");
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
    salvaLezioniSuFile(lezioni, "lezioni.txt");
}


void disdici_prenotazione() {
    extern listP* lista_prenotazioni;
    extern list* abbonati;
    extern listL* lezioni;

    pulisciSchermo();

    if (lista_prenotazioni == NULL) {
        printf("%s[INFO]%s Nessuna prenotazione registrata.\n", GIALLO, RESET);
        printf("Premi INVIO per tornare indietro.....");
        getchar();
        return;
    }

    // Ricerca dell'abbonamento
    list* nodo_abbonamento = ricercaAbbonamento(abbonati);
    if (nodo_abbonamento == NULL) {
        printf("%s[ERRORE]%s Abbonamento non trovato.\n", ROSSO, RESET);
        printf("Premi INVIO per tornare indietro.....");
        getchar();
        return;
    }

    abbonamento* ab = (abbonamento*) getValue(nodo_abbonamento);
    int codice_abbonamento = getCodiceAbbonamento(ab);

    // Stampa prenotazioni trovate
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

    if (!trovato) {
        printf("%s[INFO]%s Nessuna prenotazione trovata.\n", GIALLO, RESET);
        printf("Premi INVIO per tornare indietro.....");
        getchar();
        return;
    }

    printf("\nInserisci il codice della prenotazione da disdire (0 per annullare): ");
    int codice_da_disdire;
    scanf("%d", &codice_da_disdire);
    getchar();

    if (codice_da_disdire == 0) return;

    // Cerca e rimuove la prenotazione
    listP* precedente = NULL;
    corrente = lista_prenotazioni;

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);
        if (getCodicePrenotazione(p) == codice_da_disdire &&
            prenotazione_getCodiceAbbonamento(p) == codice_abbonamento) {

            // Riduci i posti occupati della lezione
            lezione* l = getLezioneByID(lezioni, prenotazione_getCodiceLezione(p));
            if (l != NULL) {
                removePostiOccupati(l);
            }

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

    printf("\n%s[ERRORE]%s Prenotazione non trovata o non appartenente al suo abbonamento.\n", ROSSO, RESET);
    printf("Premi INVIO per tornare indietro.....");
    getchar();
}





void visualizzaPrenotazioni(listP* lista_prenotazioni, list* lista_abbonati) {
    int scelta = -1;
    extern listL* lezioni;
    pulisciSchermo();
    printf("\n%s--- Visualizzazione Prenotazioni --- %s\n", BLU, RESET);
    printf("%s1.%s Visualizza %sTUTTE%s le prenotazioni\n", GIALLO, RESET, BLU, RESET);
    printf("%s2.%s Visualizza prenotazioni %sper lezione%s\n", GIALLO, RESET, BLU, RESET);
    printf("%s3.%s Visualizza prenotazioni %sper cliente%s\n", GIALLO, RESET, BLU, RESET);
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
                case 3: {
                int codice_abbonamento;
                printf("Inserisci il codice dell'abbonamneto (0 per tornare indietro): ");
                if (scanf("%d", & codice_abbonamento) != 1) {
                    while(getchar() != '\n'); // pulizia buffer input non valido
                    printf("Input non valido.\n");
                    return;
                }
                while(getchar() != '\n'); // pulizia newline rimasto da scanf

                if ( codice_abbonamento == 0) return;
                pulisciSchermo();
                visualizzaPrenotazioniCliente(codice_abbonamento, lista_prenotazioni, lezioni);
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

void visualizzaPrenotazioniCliente(int codice_abbonamento, listP* lista_prenotazioni, listL* lista_lezioni) {
    listP* corrente = lista_prenotazioni;
    int trovate = 0;

    printf("%sLE TUE PRENOTAZIONI\n",GIALLO);
    printf("%-20s %-20s %-15s %-12s %-10s\n", "Codice Prenotazione", "Nome Lezione", "Disciplina", "Data", "Orario");
    printf("-------------------------------------------------------------------------------%s\n",RESET);

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);
        if (p != NULL && prenotazione_getCodiceAbbonamento(p) == codice_abbonamento) {
            int codice_lezione = prenotazione_getCodiceLezione(p);
            lezione* l = getLezioneByID(lista_lezioni, codice_lezione);

            if (l != NULL) {
                Data* data_lezione = getDataLezione(l);
                if (confrontaDate(data_lezione,oggi()) >= 0) {
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


void reportLezioniUltimoMeseCliente(int codice_abbonamento, listP* lista_prenotazioni, listL* lista_lezioni) {
    listP* corrente = lista_prenotazioni;
    int trovate = 0;
    Data* inizioPeriodo = sottraiGiorni(oggi(), 30);  // Calcola la data di 30 giorni fa

    printf("%sREPORT LEZIONI - ULTIMO MESE\n", GIALLO);
    printf("%-20s %-20s %-15s %-12s %-10s\n", "Codice Prenotazione", "Nome Lezione", "Disciplina", "Data", "Orario");
    printf("-------------------------------------------------------------------------------%s\n", RESET);

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);
        if (p != NULL && prenotazione_getCodiceAbbonamento(p) == codice_abbonamento) {
            int codice_lezione = prenotazione_getCodiceLezione(p);
            lezione* l = getLezioneByID(lista_lezioni, codice_lezione);

            if (l != NULL) {
                Data* data_lezione = getDataLezione(l);
                // Controlla se la lezione è avvenuta tra 30 giorni fa e oggi (inclusi)
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

void reportDisciplineUltimoMese(listP* lista_prenotazioni, listL* lista_lezioni) {
    // Array per memorizzare le discipline e i relativi conteggi
    char* discipline[100];
    int conteggi[100];
    int num_discipline = 0;

    // Calcola la data limite: oggi meno 30 giorni
    Data* data_limite = sottraiGiorni(oggi(), 30);
    Data* data_oggi = oggi();

    listP* corrente = lista_prenotazioni;

    while (corrente != NULL) {
        prenotazione* p = prenotazione_getValue(corrente);
        if (p != NULL) {
            int codice_lezione = prenotazione_getCodiceLezione(p);
            lezione* l = getLezioneByID(lista_lezioni, codice_lezione);

            if (l != NULL) {
                Data* data_lezione = getDataLezione(l);

                // Controlla che la data della lezione sia nell'intervallo [data_limite, oggi]
                if (confrontaDate(data_lezione, data_limite) >= 0 && 
                    confrontaDate(data_lezione, data_oggi) <= 0) {

                    const char* disciplina = getDisciplinaLezione(l);
                    int trovata = 0;

                    // Cerca se la disciplina è già registrata nell'array
                    for (int i = 0; i < num_discipline; i++) {
                        if (strcmp(discipline[i], disciplina) == 0) {
                            conteggi[i]++;
                            trovata = 1;
                            break;
                        }
                    }

                    // Se non è stata trovata, aggiungila all'array
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

    // Ordina le discipline in ordine decrescente per numero di prenotazioni
    for (int i = 0; i < num_discipline - 1; i++) {
        for (int j = i + 1; j < num_discipline; j++) {
            if (conteggi[j] > conteggi[i]) {
                int temp_count = conteggi[i];
                conteggi[i] = conteggi[j];
                conteggi[j] = temp_count;

                char* temp_str = discipline[i];
                discipline[i] = discipline[j];
                discipline[j] = temp_str;
            }
        }
    }

    // Stampa il report
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

    // Libera la memoria allocata da strdup
    for (int i = 0; i < num_discipline; i++) {
        free(discipline[i]);
    }
}

void reportGestore(listP* lista_prenotazioni, listL* lista_lezioni) {
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

void aggiungi_prenotazioneCliente(abbonamento* abbonamento_prenotazione) {
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

    // Controllo scadenza abbonamento alla data odierna
    if (abbonamento_isScaduto(abbonamento_prenotazione)) {
        printf("%sErrore: L'abbonamento è scaduto. Non puoi prenotare.%s\n", ROSSO, RESET);
        printf("Premi INVIO per tornare indietro..........");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    prenotazione_setCodiceAbbonamento(nuova_prenotazione, getCodiceAbbonamento(abbonamento_prenotazione));

  
     // Inserisci data della lezione più controllo se la data e maggiore o uguale all'odierna
    printf("Data della lezione che vuoi prenotare: ");
    Data* data_prenotazione = leggiData();
    if(confrontaDate(data_prenotazione,oggi())==-1){
        printf("%sErrore:Questa data è gia passata. Non puoi prenotare.%s\n", ROSSO, RESET);
        getchar();
        printf("Premi INVIO per tornare indietro..........");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }




    // Calcola la data di scadenza dell'abbonamento (inizio + durata)
    Data* inizio_abbonamento = getDataInizio(abbonamento_prenotazione);
    int durata_mesi = getDurata(abbonamento_prenotazione);
    Data* scadenza_abbonamento = aggiungiMesi(inizio_abbonamento, durata_mesi);

    // Controllo che la data della lezione non sia dopo la scadenza
    if (confrontaDate(data_prenotazione, scadenza_abbonamento) > 0) {
        printf("%sErrore: L'abbonamento sarà scaduto alla data della lezione scelta.%s\n", ROSSO, RESET);
        printf("Non puoi prenotare una lezione oltre la scadenza dell'abbonamento.\n");
        getchar();
        printf("Premi INVIO per tornare indietro......");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Mostra lezioni disponibili
    lezione_printByDate(lezioni, data_prenotazione);

    printf("Inserisci il codice della lezione che vuoi prenotare (0 per annullare): ");
    int codice_lezione;
    scanf("%d", &codice_lezione);

    if (codice_lezione == 0) {
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Controllo prenotazione duplicata
    if (prenotazione_esistePerAbbonamentoELezione(lista_prenotazioni, lezioni,
            getCodiceAbbonamento(abbonamento_prenotazione), codice_lezione, data_prenotazione)) {
        printf("%sErrore: Hai già una prenotazione per questa lezione in questa data.%s\n", ROSSO, RESET);
        getchar();
        printf("Premi INVIO per tornare indietro.......");
        getchar();
        prenotazione_distruggi(nuova_prenotazione);
        return;
    }

    // Controlla esistenza lezione e posti disponibili
    int esito = lezione_checkByID(lezioni, codice_lezione, data_prenotazione);
    if (esito != 1) {
        printf("Errore: Lezione non trovata.\n");
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
    salvaLezioniSuFile(lezioni, "lezioni.txt");
}