#include "gestione_lezioni.h"
#include "lezione.h"
#include "list_Lezioni.h"
#include "data.h"
#include "file.h"
#include "orario.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define VIOLA    "\033[1;35m" 
#define ROSSO    "\033[1;31m"
#define VERDE    "\033[1;32m"
#define GIALLO   "\033[1;33m"
#define RESET    "\033[0m"     //comandi per stampare a video defini come define per rendere il tutto più ordinato

extern listL* lezioni;  // Modifica: usa un singolo puntatore per la lista delle lezioni

/*---------------------------------------------------------------
 * Funzione: gestione_Lezioni
 * Scopo:    Gestisce un menu interattivo per la gestione delle
 *           lezioni, permettendo all'utente di aggiungere,
 *           modificare, eliminare e visualizzare lezioni.
 * Parametri: Nessuno
 * Ritorna:   Nessun valore (void)
 * Note:      Utilizza una lista di lezioni che viene inizializzata
 *            e caricata da file se non già presente.
 *---------------------------------------------------------------*/
void gestione_Lezioni() {
    pulisciSchermo(); // Pulisce lo schermo all'avvio della funzione

    // Se la lista delle lezioni non è stata inizializzata
    if (lezioni == NULL) {
        lezioni = lezione_newList();  // Inizializza una nuova lista di lezioni
        lezioni = caricaLezioniDaFile(lezioni, "lezioni.txt"); // Carica le lezioni da file
    }

    int scelta = -1; // Variabile per memorizzare la scelta dell'utente

    // Ciclo principale che mostra il menu finché l'utente non sceglie di tornare indietro
    while (1) {
        // Stampa del menu con colori ANSI per una migliore leggibilità
        printf("\n%s=============================================%s\n", VIOLA, RESET);
        printf("%s   BENVENUTO NELLA GESTIONE LEZIONE  %s\n", GIALLO, RESET);
        printf("%s=============================================%s\n", VIOLA, RESET);
        printf("%s 1.%s AGGIUNGI UNA NUOVA LEZIONE\n", VERDE, RESET);
        printf("%s 2.%s MODIFICA UNA LEZIONE\n", VERDE, RESET);
        printf("%s 3.%s ELIMINA UNA LEZIONE\n", VERDE, RESET); 
        printf("%s 4.%s VISUALIZZA LISTA LEZIONI\n", VERDE, RESET);
        printf("%s 0. TORNA AL MENU GESTORE %s\n", ROSSO, RESET);
        printf("%s------------------------------%s\n", VIOLA, RESET);
        printf("%s  Inserisci la tua scelta: %s", GIALLO, RESET);

        // Controllo che l'input sia valido (cioè un intero)
        char extra;
        if (scanf("%d%c", &scelta, &extra) != 2 || extra != '\n') {
            pulisciSchermo();  // Pulisce lo schermo
            printf("%sATTENZIONE!! DEVI IMMETTERE 0, 1, 2, 3 O 4 A SECONDA DELLA TUA SCELTA. RIPROVA:%s\n", ROSSO, RESET);
            while (getchar() != '\n');  // Pulisce il buffer
            continue;  // Ritorna all'inizio del ciclo senza eseguire il resto
        }

        // Gestione della scelta tramite switch
        switch (scelta) {
            case 0:
                pulisciSchermo();
                return; // Esce dal menu e torna al menu gestore

            case 1:
                // Funzione per aggiungere una nuova lezione
                pulisciSchermo();
                int c = aggiungi_lezione(&lezioni); // Passa il riferimento alla lista (doppio puntatore)
                if (c == 1) {
                    printf("%sLa lezione è stata aggiunta con successo.%s\n", VERDE, RESET);
                } else {
                    printf("%sLezione non aggiunta.%s\n", ROSSO, RESET);
                }
                break;

            case 2:
                // Funzione per modificare una lezione esistente
                modificaLezione(lezioni);
                break;

            case 3:
                // Funzione per eliminare una lezione esistente
                eliminaLezione(lezioni);
                break;

            case 4:
                // Funzione per visualizzare tutte le lezioni
                pulisciSchermo();
                visualizzaLezioni(lezioni);  // Passa direttamente il puntatore alla lista
                getchar(); // Attende un input per continuare
                pulisciSchermo();
                break;

            default:
                pulisciSchermo(); // In caso di scelta non valida
                messaggio_errore(); // Funzione che stampa un messaggio d’errore generico
        }
    }
}

/*---------------------------------------------------------------
 * Funzione: aggiungi_lezione
 * Scopo:    Crea e aggiunge una nuova lezione alla lista delle lezioni.
 * Parametri:
 *    - listL** lezioni: puntatore al puntatore della lista delle lezioni
 * Ritorna:
 *    - 1 se la lezione è stata aggiunta correttamente
 *    - 0 in caso di errore o annullamento da parte dell'utente
 * Note:
 *    - Chiede all’utente di inserire i dati relativi alla nuova lezione
 *    - Controlla la validità di ogni input
 *    - Salva le modifiche su file
 *---------------------------------------------------------------*/
int aggiungi_lezione(listL** lezioni) {
    lezione* nuova_lezione = creaLezione(); // Crea una nuova struttura lezione
    
    if (nuova_lezione == NULL) {
        printf("Errore nella creazione della lezione!\n");
        return 0;
    }

    // Imposta un codice univoco per la nuova lezione
    int max_id = getMaxCodiceLezione(*lezioni);
    setCodiceLezione(nuova_lezione, max_id + 1);

    /*----------------------------
     * SCELTA DELLA DISCIPLINA
     *----------------------------*/
    while (1) {
        printf("\n========================================================================================================\n");
        printf("                                     Aggiungi Nuova Lezione\n");
        printf("========================================================================================================\n"); 
        printf("Se vuoi annullare e tornare al menu gestore digitare 'exit' nel campo 'nome'\n");
        printf("========================================================================================================\n");

        int scelta;
        char buffer_nome[50];

        // Menu per selezionare una disciplina predefinita o personalizzata
        printf("%sScegli una disciplina:\n", VERDE);
        printf("1: Yoga\n2: Pilates\n3: Zumba\n4: Spinning\n5: Aerobica\n6: Altro\n0: Torna al menu%s\n> ", RESET);
        if (scanf("%d", &scelta) != 1) {
            while (getchar() != '\n');
            pulisciSchermo();
            printf("%sERRORE%s Inserisci un numero valido.\n", ROSSO, RESET);
            continue;
        }
        getchar(); // consuma newline

        switch (scelta) {
            case 1: setDisciplinaLezione(nuova_lezione, "Yoga"); break;
            case 2: setDisciplinaLezione(nuova_lezione, "Pilates"); break;
            case 3: setDisciplinaLezione(nuova_lezione, "Zumba"); break;
            case 4: setDisciplinaLezione(nuova_lezione, "Spinning"); break;
            case 5: setDisciplinaLezione(nuova_lezione, "Aerobica"); break;
            case 6:
                // Inserimento manuale del nome della disciplina
                printf("%sInserisci manualmente la disciplina: %s", VERDE, RESET);
                if (fgets(buffer_nome, sizeof(buffer_nome), stdin) != NULL) {
                    buffer_nome[strcspn(buffer_nome, "\n")] = '\0'; // Rimuove newline
                    if (strlen(buffer_nome) == 0) {
                        printf("%sERRORE%s Il nome non può essere vuoto.\n", ROSSO, RESET);
                        continue;
                    }
                    setDisciplinaLezione(nuova_lezione, buffer_nome);
                } else {
                    printf("%sERRORE%s Input non valido.\n", ROSSO, RESET);
                    continue;
                }
                break;
            case 0:
                // Annulla l’operazione e libera la memoria
                pulisciSchermo();
                distruggiLezione(nuova_lezione);
                return 0;
            default:
                pulisciSchermo();
                printf("%sERRORE%s Scelta non valida.\n", ROSSO, RESET);
                continue;
        }
        break;
    }

    /*----------------------------
     * INSERIMENTO NOME LEZIONE
     *----------------------------*/
    while (1) {
        char buffer_nome[50];
        printf("%sInserisci nome lezione: %s", VERDE, RESET);
        if (fgets(buffer_nome, sizeof(buffer_nome), stdin) != NULL) {
            buffer_nome[strcspn(buffer_nome, "\n")] = '\0'; // Rimuove newline
            if (strcmp(buffer_nome, "exit") == 0) {
                pulisciSchermo();
                distruggiLezione(nuova_lezione);
                return 0;
            }
            if (strlen(buffer_nome) == 0) {
                printf("%sERRORE%s Il nome non può essere vuoto.\n", ROSSO, RESET);
            } else {
                setNomeLezione(nuova_lezione, buffer_nome);
                break;
            }
        }
    }

    /*----------------------------
     * INSERIMENTO NUMERO POSTI
     *----------------------------*/
    short int posti_max;
    while (1) {
        printf("%sInserisci numero massimo di posti: %s", VERDE, RESET);
        if (scanf("%hd", &posti_max) != 1 || posti_max < 1 || posti_max > 999) {
            printf("%sNumero di posti non valido.%s\n", ROSSO, RESET);
            while (getchar() != '\n');
        } else {
            setPostiMax(nuova_lezione, posti_max);
            break;
        }
    }

    /*----------------------------
     * INSERIMENTO DATA LEZIONE
     *----------------------------*/
    while (1) {
        printf("%sData della lezione (deve essere almeno domani):%s\n", VERDE, RESET);
        Data* data_input = leggiData();
        if (confrontaDate(data_input, oggi()) < 1) {
            printf("%sLa data deve essere almeno il giorno dopo oggi.%s\n", ROSSO, RESET);
        } else {
            setDataLezione(nuova_lezione, data_input);
            break;
        }
    }

    /*----------------------------
     * INSERIMENTO ORARIO LEZIONE
     *----------------------------*/
    printf("%sOrario della lezione: %s\n", VERDE, RESET);
    Orario* orario_inizio = leggiOrario();
    setOrarioInizio(nuova_lezione, orario_inizio);

    /*----------------------------
     * INSERIMENTO DURATA LEZIONE
     *----------------------------*/
    short int durata;
    while (1) {
        printf("%sInserisci durata in minuti (max 360): %s", VERDE, RESET);
        if (scanf("%hd", &durata) != 1 || durata <= 0 || durata > 360) {
            printf("%sDurata non valida.%s\n", ROSSO, RESET);
            while (getchar() != '\n');
        } else {
            setDurataLezione(nuova_lezione, durata);
            break;
        }
    }

    // Calcola l'orario di fine lezione
    pulisciSchermo();
    Orario* orario_fine = aggiungiMinuti(getOrarioLezione(nuova_lezione), getDurataLezione(nuova_lezione));

    /*----------------------------
     * RIEPILOGO LEZIONE INSERITA
     *----------------------------*/
    printf("\n%s================ LEZIONE INSERITA =================%s\n", VERDE, RESET);
    printf("Lezione '%s' (%s), codice %d, data ",
           getNomeLezione(nuova_lezione),
           getDisciplinaLezione(nuova_lezione),
           getCodiceLezione(nuova_lezione));
    stampaData(getDataLezione(nuova_lezione));
    printf(" alle ");
    stampaOrario(getOrarioLezione(nuova_lezione));
    printf(", termina alle ");
    stampaOrario(orario_fine);
    printf("\n===================================================\n");

    /*----------------------------
     * AGGIUNTA IN LISTA E SALVATAGGIO
     *----------------------------*/
    *lezioni = lezione_consList(nuova_lezione, *lezioni); // Inserisce in testa alla lista
    salvaLezioniSuFile(*lezioni, "lezioni.txt"); // Salva su file

    getchar(); // attende invio
    printf("\nPremi INVIO per continuare...\n");
    getchar();
    pulisciSchermo();

    return 1;
}
/*---------------------------------------------------------------
 * Funzione: lezione_printByDate
 * Scopo:    Visualizza tutte le lezioni presenti in una data specifica.
 * Parametri:
 *    - listL* lezioni: lista delle lezioni
 *    - Data* data_input: data da confrontare per filtrare le lezioni
 * Note:
 *    - Se non ci sono lezioni o la data non è valida, viene mostrato un messaggio.
 *    - Per ogni lezione trovata, stampa codice, nome, disciplina, orario e posti rimanenti.
 *---------------------------------------------------------------*/
void lezione_printByDate(listL* lezioni, Data* data_input) {
    // Controllo parametri nulli
    if (lezioni == NULL || data_input == NULL) {
        printf("%sNESSUNA LEZIONE IN QUESTA DATA.%s\n", ROSSO, RESET);
        getchar();
        printf("Premi INVIO per tornare indietro..........\n");
        getchar();
        pulisciSchermo();
        return;
    }

    listL* temp = lezioni;
    int trovato = 0;

    // Stampa intestazione con data selezionata
    printf("\n%sVisualizzazione lezioni per la data: ", GIALLO);
    stampaData(data_input);  // Funzione per stampare la data
    printf("\n%s", RESET);

    // Stampa intestazione tabella
    printf("%-15s | %-20s | %-20s | %-15s | %-17s\n",
           "Codice", "Nome", "Disciplina", "Orario", "Posti Rimanenti");
    printf("-----------------------------------------------------------------------------------------------\n");

    // Scorri la lista delle lezioni
    while (temp != NULL) {
        lezione* lezione_corrente = lezione_getValue(temp);

        if (lezione_corrente == NULL) {
            temp = lezione_getNext(temp);
            continue;
        }

        // Ottieni e controlla la data della lezione
        Data* data_lezione = getDataLezione(lezione_corrente);
        if (data_lezione == NULL) {
            temp = lezione_getNext(temp);
            continue;
        }

        // Se la data coincide, stampa le informazioni della lezione
        if (confrontaDate(data_input, data_lezione) == 0) {
            int posti_max = getPostiMax(lezione_corrente);
            int prenotati = getPostiOccupati(lezione_corrente);
            int posti_rimanenti = posti_max - prenotati;

            // Orario inizio e calcolo orario fine
            Orario* orario_inizio = getOrarioLezione(lezione_corrente);
            int durata = getDurataLezione(lezione_corrente);
            Orario* orario_fine = aggiungiMinuti(orario_inizio, durata);

            // Formattazione stringa orario
            char orario_str[20];
            snprintf(orario_str, sizeof(orario_str), "%02d:%02d - %02d:%02d",
                     getOra(orario_inizio), getMinuto(orario_inizio),
                     getOra(orario_fine), getMinuto(orario_fine));

            // Stampa una riga della tabella
            printf("%-15d | %-20s | %-20s | %-15s | %-17d\n",
                   getCodiceLezione(lezione_corrente),
                   getNomeLezione(lezione_corrente),
                   getDisciplinaLezione(lezione_corrente),
                   orario_str,
                   posti_rimanenti);

            trovato = 1;
        }

        // Passa alla lezione successiva
        temp = lezione_getNext(temp);
    }

    // Se nessuna lezione è stata trovata per quella data
    if (!trovato) {
        printf("\n%sNessuna lezione in questa data.%s\n", ROSSO, RESET);
    }

    // Attende input per tornare indietro
    getchar();
    printf("\nPremi INVIO per tornare continuare.....");
    getchar();
}





/*---------------------------------------------------------------
 * Funzione: modificaLezione
 * Scopo:    Permette di modificare i dettagli di una lezione esistente
 *           cercandola tramite il suo codice identificativo (ID).
 * Parametri:
 *    - listL* lezioni: lista delle lezioni esistenti
 * Ritorna:
 *    - listL*: lista aggiornata con la lezione modificata
 *---------------------------------------------------------------*/
listL* modificaLezione(listL* lezioni) {
    // Richiede l'ID della lezione da modificare
    int id;
    printf("Inserisci l'ID della lezione da modificare: ");
    scanf("%d", &id);
    getchar();

    listL* temp = lezioni;

    // Scorre la lista per cercare la lezione con l'ID inserito
    while (temp != NULL) {
        pulisciSchermo();
        lezione* l = lezione_getValue(temp);
        if (l != NULL && getCodiceLezione(l) == id) {
            printf("%sLezione trovata!%s\n", VERDE, RESET);
            stampaDettagliLezione(l);  // Mostra i dettagli attuali della lezione

            int scelta = -1;
            char extra;
            
            // Menu delle modifiche disponibili
            printf("%s========================================================\n", GIALLO);
            printf("            DIGITA COSA VUOI MODIFICARE\n");
            printf("========================================================%s\n", RESET);
            printf("1. NOME\n");
            printf("2. DISCIPLINA\n");
            printf("3. DATA\n");
            printf("4. ORARIO INIZIO\n");
            printf("5. DURATA\n");
            printf("6. POSTI MASSIMI\n");
            printf("%s0. TORNA INDIETRO%s\n", ROSSO, RESET);
            printf("%s========================================================%s\n", GIALLO, RESET);

            // Gestione modifiche fino a scelta 0
            while (scelta != 0) {
                printf("Scelta: ");

                // Controlla che l'input sia un intero seguito da newline
                if (scanf("%d%c", &scelta, &extra) != 2 || extra != '\n') {
                    pulisciSchermo();
                    printf("%sScelta non valida. Inserisci un numero tra 0 e 6.%s\n", ROSSO, RESET);
                    while (getchar() != '\n');
                    continue;
                }

                switch (scelta) {
                    case 1: {
                        // Modifica nome
                        char nuovoNome[50];
                        printf("Inserisci il nuovo nome della lezione: ");
                        scanf("%s", nuovoNome);
                        setNomeLezione(l, nuovoNome);
                        printf("%sNome modificato con successo!%s\n", VERDE, RESET);
                        break;
                    }

                    case 2: {
                        // Modifica disciplina
                        char nuovaDisciplina[50];
                        printf("Inserisci la nuova disciplina: ");
                        scanf("%s", nuovaDisciplina);
                        setDisciplinaLezione(l, nuovaDisciplina);
                        printf("%sDisciplina modificata con successo!%s\n", VERDE, RESET);
                        break;
                    }

                    case 3: {
                        // Modifica data
                        Data* nuovaData = leggiData();
                        setDataLezione(l, nuovaData);
                        printf("%sData modificata con successo!%s\n", VERDE, RESET);
                        break;
                    }

                    case 4: {
                        // Modifica orario di inizio
                        Orario* nuovoOrario = leggiOrario();
                        setOrarioInizio(l, nuovoOrario);
                        printf("%sOrario modificato con successo!%s\n", VERDE, RESET);
                        break;
                    }

                    case 5: {
                        // Modifica durata
                        int nuovaDurata;
                        printf("Inserisci la nuova durata (in minuti): ");
                        scanf("%d", &nuovaDurata);
                        setDurataLezione(l, nuovaDurata);
                        printf("%sDurata modificata con successo!%s\n", VERDE, RESET);
                        break;
                    }

                    case 6: {
                        // Modifica posti massimi
                        int nuoviPosti;
                        printf("Inserisci il nuovo numero massimo di posti: ");
                        scanf("%d", &nuoviPosti);
                        setPostiMax(l, nuoviPosti);
                        printf("%sPosti massimi modificati con successo!%s\n", VERDE, RESET);
                        break;
                    }

                    case 0:
                        // Salva e torna indietro
                        salvaLezioniSuFile(lezioni, "lezioni.txt");
                        pulisciSchermo();
                        return lezioni;

                    default:
                        // Scelta non valida
                        printf("%sScelta non valida.%s\n", ROSSO, RESET);
                        break;
                }

                // Salva su file dopo ogni modifica
                salvaLezioniSuFile(lezioni, "lezioni.txt");
            }
        }

        // Continua con la lezione successiva
        temp = lezione_getNext(temp);
    }

    // Se la lezione non è stata trovata
    printf("%sNessuna lezione trovata con ID %d.%s\n", ROSSO, id, RESET);
    printf("Premi INVIO per tornare...\n");
    getchar();
    pulisciSchermo();
    return lezioni;
}

/*
 * Funzione: lezione_printByDisciplina
 * Scopo:    Visualizza tutte le lezioni appartenenti a una determinata disciplina.
 * Parametri:
 *   - lezioni: puntatore alla lista delle lezioni.
 * Note:
 *   - Stampa una tabella con le informazioni delle lezioni trovate.
 *   - Richiede all'utente il nome della disciplina.
 */
void lezione_printByDisciplina(listL* lezioni) {
    // Controlla se la lista delle lezioni è vuota
    if (lezioni == NULL) {
        printf("%sLa lista delle lezioni è vuota.%s\n", ROSSO, RESET);
        printf("Premi INVIO per tornare indietro...\n");
        getchar();
        return;
    }

    // Acquisisce la disciplina da cercare
    char disciplina[50];
    printf("Inserisci il nome della disciplina da cercare: ");
    scanf(" %[^\n]", disciplina);  // Legge stringhe con spazi inclusi

    pulisciSchermo();

    // Stampa intestazione della tabella
    printf("\n%sLezioni per la disciplina: %s%s\n", GIALLO, disciplina, RESET);
    printf("%-15s | %-20s | %-20s | %-15s | %-20s | %-17s\n",
           "Codice", "Nome", "Disciplina", "Orario", "Data", "Posti Rimanenti");
    printf("-----------------------------------------------------------------------------------------------\n");

    listL* temp = lezioni;
    int trovato = 0;

    // Scorre tutta la lista delle lezioni
    while (temp != NULL) {
        lezione* l = lezione_getValue(temp);

        if (l != NULL) {
            // Ottiene la disciplina della lezione
            const char* disciplina_lezione = getDisciplinaLezione(l);

            // Confronta con la disciplina cercata
            if (strcmp(disciplina_lezione, disciplina) == 0) {
                // Calcolo orario inizio e fine
                Orario* orario_inizio = getOrarioLezione(l);
                int durata = getDurataLezione(l);
                Orario* orario_fine = aggiungiMinuti(orario_inizio, durata);  // Alloca nuova struttura orario

                // Costruisce stringa dell'orario
                char orario_str[20];
                snprintf(orario_str, sizeof(orario_str), "%02d:%02d - %02d:%02d",
                         getOra(orario_inizio), getMinuto(orario_inizio),
                         getOra(orario_fine), getMinuto(orario_fine));

                // Calcolo posti rimanenti
                int posti_max = getPostiMax(l);
                int posti_occupati = getPostiOccupati(l);
                int posti_rimanenti = posti_max - posti_occupati;

                // Ottiene e formatta la data della lezione
                Data* data_lezione = getDataLezione(l);
                char data_str[20];
                snprintf(data_str, sizeof(data_str), "%02d/%02d/%04d",
                         getGiorno(data_lezione), getMese(data_lezione), getAnno(data_lezione));

                // Stampa le informazioni in formato tabellare
                printf("%-15d | %-20s | %-20s | %-15s | %-20s | %-17d\n",
                       getCodiceLezione(l),
                       getNomeLezione(l),
                       disciplina_lezione,
                       orario_str,
                       data_str,
                       posti_rimanenti);

                // Libera la memoria dell'orario di fine (se allocato dinamicamente)
                free(orario_fine);

                trovato = 1;
            }
        }

        // Passa alla lezione successiva nella lista
        temp = lezione_getNext(temp);
    }

    // Se nessuna lezione è stata trovata con quella disciplina
    if (!trovato) {
        printf("%sNessuna lezione trovata per la disciplina '%s'.%s\n", ROSSO, disciplina, RESET);
    }

    // Pausa prima di tornare al menu precedente
    printf("\nPremi INVIO per tornare indietro...");
    getchar();
    getchar();  // Consuma newline rimasta nel buffer
    pulisciSchermo();
}
/*
 * Funzione: lezione_printByID
 * Scopo:    Visualizza le informazioni dettagliate di una lezione dato il suo ID (codice).
 * Parametri:
 *   - lezioni: puntatore alla lista delle lezioni
 *   - id_input: ID della lezione da cercare
 */
void lezione_printByID(listL* lezioni, int id_input) {
    // Se la lista è vuota, avvisa l'utente e ritorna
    if (lezioni == NULL) {
        printf("%sNESSUNA LEZIONE PRESENTE.%s\n", ROSSO, RESET);
        getchar();
        printf("Premi INVIO per tornare indietro..........\n");
        getchar();
        pulisciSchermo();
        return;
    }

    listL* temp = lezioni;
    int trovato = 0;

    pulisciSchermo();

    // Intestazione della ricerca
    printf("\n%sRicerca lezione con ID: %d%s\n", GIALLO, id_input, RESET);
    printf("%-15s | %-20s | %-20s | %-15s | %-17s\n",
           "Codice", "Nome", "Disciplina", "Orario", "Posti Rimanenti");
    printf("-----------------------------------------------------------------------------------------------\n");

    // Scorre la lista alla ricerca dell'ID
    while (temp != NULL) {
        lezione* lezione_corrente = lezione_getValue(temp);

        // Controlla se la lezione è valida e ha l'ID cercato
        if (lezione_corrente != NULL && getCodiceLezione(lezione_corrente) == id_input) {
            // Ottiene dati rilevanti
            Orario* orario_inizio = getOrarioLezione(lezione_corrente);
            int durata = getDurataLezione(lezione_corrente);
            int posti_max = getPostiMax(lezione_corrente);
            int prenotati = getPostiOccupati(lezione_corrente);
            int posti_rimanenti = posti_max - prenotati;

            // Calcola orario di fine
            Orario* orario_fine = aggiungiMinuti(orario_inizio, durata);

            // Costruisce stringa orario "inizio - fine"
            char orario_str[20];
            snprintf(orario_str, sizeof(orario_str), "%02d:%02d - %02d:%02d",
                     getOra(orario_inizio), getMinuto(orario_inizio),
                     getOra(orario_fine), getMinuto(orario_fine));

            // Stampa lezione trovata
            printf("%-15d | %-20s | %-20s | %-15s | %-17d\n",
                   getCodiceLezione(lezione_corrente),
                   getNomeLezione(lezione_corrente),
                   getDisciplinaLezione(lezione_corrente),
                   orario_str,
                   posti_rimanenti);

            trovato = 1;
            break;  // Interrompe la ricerca una volta trovata
        }

        // Passa alla prossima lezione
        temp = lezione_getNext(temp);
    }

    // Se non è stata trovata alcuna lezione con quell'ID
    if (!trovato) {
        printf("\n%sNessuna lezione trovata con ID %d.%s\n", ROSSO, id_input, RESET);
    }

    // Pausa finale
    getchar();
    printf("\nPremi INVIO per tornare indietro.....");
    getchar();
    pulisciSchermo();
}
/*
 * Funzione: lezione_checkByID
 * Scopo:    Verifica se esiste una lezione con uno specifico ID e in una data specifica.
 * Parametri:
 *   - lezioni: puntatore alla lista delle lezioni
 *   - id_input: ID della lezione da cercare
 *   - data_input: puntatore alla data in cui cercare la lezione
 * Ritorna:
 *   - 1 se la lezione è stata trovata nella data specificata
 *   - 0 altrimenti
 */
int lezione_checkByID(listL* lezioni, int id_input, Data* data_input) {
    // Controllo se la lista delle lezioni è vuota
    if (lezioni == NULL) {
        printf("%sNESSUNA LEZIONE PRESENTE.%s\n", ROSSO, RESET);
        getchar();
        printf("Premi INVIO per tornare indietro..........\n");
        getchar();
        pulisciSchermo();
        return 0;
    }

    listL* temp = lezioni;
    int trovato = 0;

    // Scorre tutta la lista alla ricerca della lezione con ID e data corrispondenti
    while (temp != NULL) {
        lezione* lezione_corrente = lezione_getValue(temp);

        // Verifica che la lezione esista, che l'ID corrisponda e che la data sia uguale
        if (lezione_corrente != NULL &&
            getCodiceLezione(lezione_corrente) == id_input &&
            confrontaDate(getDataLezione(lezione_corrente), data_input) == 0) {

            trovato = 1; // Lezione trovata
            break;       // Esce dal ciclo
        }

        temp = lezione_getNext(temp); // Passa alla lezione successiva
    }

    // Se nessuna lezione è stata trovata, avvisa l’utente
    if (!trovato) {
        printf("\n%sNessuna lezione trovata con ID %d nella data specificata.%s\n", ROSSO, id_input, RESET);
    }

    return trovato; // Ritorna 1 se trovata, altrimenti 0
}

// Funzione per visualizzare tutte le lezioni
void visualizzaLezioni(listL* lezioni) {
    int scelta;
    Data* data_input = NULL;
    int id=0;

    while (1) {
        pulisciSchermo();
        // Mostra il menu per la scelta dell'utente
        printf("\n%s===================================\n", GIALLO);
        printf("          Scegli un'opzione:\n");
        printf("===================================%s\n", RESET);
        printf("1. Visualizza lezioni per un giorno specifico\n");
        printf("2. Visualizza tutte le lezioni\n");
        printf("3. Cerca una lezione con l'id\n");
        printf("4. Cerca le lezione di una disciplina\n");
        printf("%s0. Torna indietro%s\n", ROSSO, RESET);
        printf("%s===================================%s\n", GIALLO, RESET);
        printf("Inserisci la tua scelta: ");
        
        // Acquisisci la scelta dell'utente
        if (scanf("%d", &scelta) != 1) {
            while (getchar() != '\n');  // Pulisce il buffer
            pulisciSchermo();
             printf("%sScelta non valida. Riprova.%s\n",ROSSO,RESET);
            continue;  // Torna all'inizio del ciclo
        }
        
        switch (scelta) {
            case 1:
                // Visualizza lezioni per un giorno specifico
                data_input = leggiData();  // Funzione per leggere la data dall'utente
                if (data_input != NULL) {
                    printf("\n%sVisualizzazione lezioni per la data: ",GIALLO);
                    stampaData(data_input);  // Funzione per stampare la data
                    printf("\n%s",RESET);
                    lezione_printByDate(lezioni, data_input);  // Funzione che stampa le lezioni per la data specificata
                } else {
                    printf("Errore nell'inserimento della data.\n");
                }
                break;
                
            case 2:
                // Visualizza tutte le lezioni
                if (lezioni != NULL) {
                    lezione_printList(lezioni);  // Funzione che stampa tutte le lezioni
                } else {
                    
                    printf("Non ci sono lezioni da visualizzare.\n");
                }
                getchar();  // Pulisce il buffer
                printf("Premi INVIO per tornare indietro...\n");
                getchar();
                pulisciSchermo();
                break;

            case 3:
                 printf("Quale id vuoi cercare?: ");
                 scanf("%d",&id);
                 lezione_printByID(lezioni,id);
                 break;
            case 4:
                 lezione_printByDisciplina(lezioni);
                 break;

                
            case 0:
                // Torna indietro
                return;
                
            default:
                pulisciSchermo();
                printf("%sScelta non valida. Riprova.%s\n",ROSSO,RESET);
                break;
        }
    }
}

// Funzione per eliminare una lezione dalla lista
// Parametri:
//   - lezioni: puntatore alla lista di lezioni
// Restituisce:
//   - La lista aggiornata con la lezione eliminata
listL* eliminaLezione(listL* lezioni) {
    int scelta;
    pulisciSchermo();
    if (lezioni == NULL) {
        pulisciSchermo();
        printf("%s=============================================================================%s\n", ROSSO, RESET);
        printf("%sLA LISTA DELLE LEZIONI È VUOTA QUINDI NON È POSSIBILE ELIMINARE ELEMENTI%s\n", ROSSO, RESET);
        printf("%s=============================================================================%s\n", ROSSO, RESET);
        printf("Premi invio per continuare......\n");
        getchar();
        pulisciSchermo();
        return lezioni;
    }

    while (1) {
        printf("%s=========================================%s\n", GIALLO, RESET);
        printf("%sEliminazione Lezione:%s\n", GIALLO, RESET);
        printf("%s1. Elimina per ID%s\n", GIALLO, RESET);
        printf("%s0. Torna al menu precedente%s\n", ROSSO, RESET);
        printf("%s=========================================%s\n", GIALLO, RESET);
        printf("Scelta: ");
        scanf("%d", &scelta);
        getchar();

        switch (scelta) {
            case 0:
                pulisciSchermo();
                return lezioni;

            case 1:
                lezioni = eliminaLezionePerID(lezioni);
                return lezioni;

            default:
                printf("%sScelta non valida.%s\n", ROSSO, RESET);
                break;
        }

        pulisciSchermo();
        printf("%sOperazione non eseguita.%s\n", ROSSO, RESET);
        printf("Premi invio per continuare......\n");
        getchar();
        pulisciSchermo();
    }
}


/**
 * Cerca nella lista delle lezioni una lezione con l'ID specificato.
 * 
 * param lezioni  Lista delle lezioni (listL*).
 * param id       ID della lezione da cercare.
 * return         Puntatore alla lezione trovata (lezione*), oppure NULL se non trovata.
 */
lezione* trovaLezionePerID(listL* lezioni, int id) {
    listL* curr = lezioni;
    while (curr != NULL) {
        lezione* l = lezione_getValue(curr);
        if (getCodiceLezione(l) == id) {
            return l;  // trovato, ritorna la lezione
        }
        curr = lezione_getNext(curr);
    }
    return NULL;  // non trovato
}


// Funzione per eliminare una lezione per ID
// Parametri:
//   - lezioni: puntatore alla lista di lezioni
// Restituisce:
//   - La lista aggiornata con la lezione eliminata
listL* eliminaLezionePerID(listL* lezioni) {
    int id;
    printf("Inserisci l'ID della lezione da eliminare: ");
    scanf("%d", &id);
    getchar();

    listL *curr = lezioni, *prev = NULL;

    while (curr != NULL) {
        if (getCodiceLezione(lezione_getValue(curr)) == id) {
            if (prev == NULL) {
                lezioni = lezione_getNext(curr);
            } else {
                lezione_setNext(prev, lezione_getNext(curr));
            }

            distruggiLezione(lezione_getValue(curr));
            free(curr);

            pulisciSchermo();
            salvaLezioniSuFile(lezioni, "lezioni.txt");
            printf("%sLezione eliminata con successo.%s\n", VERDE, RESET);
            printf("Premi invio per continuare......\n");
            getchar();
            pulisciSchermo();
            return lezioni;
        }

        prev = curr;
        curr = lezione_getNext(curr);
    }

    pulisciSchermo();
    printf("%sErrore: nessuna lezione trovata con ID %d.%s\n", ROSSO, id, RESET);
    printf("Premi invio per continuare......\n");
    getchar();
    pulisciSchermo();
    return lezioni;
}