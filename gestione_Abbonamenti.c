#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "gestione_Abbonamenti.h"
#include "list.h"
#include "abbonamento.h"
#include "file.h"
#include "data.h"
#include "util.h"


extern list* abbonati;


// Definizioni di colori ANSI per formattare il testo in output. 
// Questi vengono usati per stampare messaggi colorati in console.
#define VIOLA    "\033[1;35m"   // Colore Viola
#define ROSSO    "\033[1;31m"   // Colore Rosso
#define VERDE    "\033[1;32m"   // Colore Verde
#define GIALLO   "\033[1;33m"   // Colore Giallo
#define BLU      "\033[34m"     // Colore Blu
#define RESET    "\033[0m"      // Resetta il colore alla configurazione predefinita della console




/**
 * Funzione: controllo_abbonamento
 * -------------------------------
 * Visualizza lo stato dell'abbonamento di un cliente.
 * 
 * - Se l'abbonamento è ancora valido, mostra quanto tempo resta (anni, mesi, giorni).
 * - Se è scaduto, mostra da quanto tempo è scaduto.
 * 
 * Utilizza colori (VERDE per valido, ROSSO per scaduto) per una migliore leggibilità.
 * Richiede conferma (INVIO) per tornare indietro.
 * 
 * Parametri:
 *   - abbonamento *a: puntatore alla struttura dell'abbonamento del cliente.
 */
void controllo_abbonamento(abbonamento *a) {
    // Pulisce lo schermo e mostra nome/cognome del cliente come intestazione
    pulisciSchermoCliente(getNome(a), getCognome(a));

    // Stampa intestazione e dettagli dell’abbonamento
    printf("\nECCO IL TUO ABBONAMENTO\n");
    printf("----------------------------------------\n");
    stampaDettagliAbbonamento(a);

    // Ottiene la data corrente dal sistema
    Data* oggi_data = oggi();

    // Calcola la data di scadenza dell’abbonamento
    Data* scadenza = aggiungiMesi(getDataInizio(a), getDurata(a));

    // Verifica se l'abbonamento è ancora valido (scadenza >= oggi)
    if (confrontaDate(oggi_data, scadenza) <= 0) {
        // Calcolo semplificato della differenza di tempo rimanente
        int anni = getAnno(scadenza) - getAnno(oggi_data);
        int giorni_totali = (getMese(scadenza) - getMese(oggi_data)) * 30 + (getGiorno(scadenza) - getGiorno(oggi_data));

        // Se il totale è negativo, corregge riducendo di un anno
        if (giorni_totali < 0) {
            anni--;
            giorni_totali += 365;
        }

        // Converte i giorni totali in mesi e giorni (approssimazione: 30 giorni = 1 mese)
        int mesi = giorni_totali / 30;
        int giorni = giorni_totali % 30;

        // Messaggio in verde: abbonamento ancora valido
        printf("%s\nIl tuo abbonamento è ancora valido: potrai frequentare la nostra palestra per ", VERDE);

        if(anni==0&&mesi==0&&giorni==0)
            printf("l'ultimo giorno");
        else{
        if (anni > 0)
            printf("%d anno%s ", anni, anni > 1 ? "i" : "");

        if (mesi > 0)
            printf("%d mese%s ", mesi, mesi > 1 ? "i" : "");

        if (giorni > 0 || (anni == 0 && mesi == 0))
            printf("%d giorno%s", giorni, giorni != 1 ? "i" : "");
        }
        printf(".\n%s", RESET);
    } else {
        // Abbonamento scaduto → calcolo del tempo trascorso dalla scadenza
        int anni = getAnno(oggi_data) - getAnno(scadenza);
        int mesi = getMese(oggi_data) - getMese(scadenza);
        int giorni = getGiorno(oggi_data) - getGiorno(scadenza);

        // Aggiusta i valori negativi per ottenere una differenza positiva coerente
        if (giorni < 0) {
            giorni += 30;
            mesi--;
        }
        if (mesi < 0) {
            mesi += 12;
            anni--;
        }

        // Messaggio in rosso: abbonamento scaduto
        printf("\n%sL'abbonamento è scaduto da ", ROSSO);

        if (anni > 0)
            printf("%d anno%s ", anni, anni > 1 ? "i" : "");

        if (mesi > 0)
            printf("%d mese%s ", mesi, mesi > 1 ? "i" : "");

        if (giorni > 0 || (anni == 0 && mesi == 0))
            printf("%d giorno%s", giorni, giorni != 1 ? "i" : "");

        printf(".\n%s", RESET);
        printf("RICORDA: PUOI EFFETTUARE IL RINNOVO ESCLUSIVAMENTE IN LOCO.\n");
    }

    // Libera la memoria allocata per la data odierna
    distruggiData(oggi_data);

    // Attende che l'utente prema INVIO per tornare indietro
    printf("\nPremi INVIO per tornare indietro.....\n");
    getchar();
}

// Funzione che gestisce il sottomenu degli abbonamenti
// Parametri:
//   - Nessuno (usa la variabile globale 'abbonati')
// Restituisce:
//   - Nessuno (funzione void)
// Descrizione:
//   - Mostra un menu che permette di aggiungere, modificare, eliminare o visualizzare abbonamenti.
//   - Carica i dati dal file se non ancora caricati.
//   - Salva le modifiche su file dopo operazioni rilevanti.
//   - Torna al menu principale quando l’utente seleziona 0.
void gestione_abbonamenti() {
    // Se gli abbonati non sono stati ancora caricati, li carica dal file
    if (abbonati == NULL)
        abbonati = caricaAbbonamentiDaFile(abbonati, "abbonamenti.txt");

    // Se la lista è ancora vuota (file vuoto o non esistente), inizializza una lista nuova
    if (abbonati == NULL)
        abbonati = newList();

    int scelta = -1;  // Variabile per memorizzare la scelta dell’utente nel menu

    // Ciclo principale: continua a mostrare il menu fino a che l'utente non sceglie di uscire (0)
    while (1) {
        // Menu con codici ANSI per i colori (estetica)
        printf("\n%s=============================================%s\n", VIOLA, RESET);
        printf("%s   BENVENUTO NELLA GESTIONE ABBONAMENTI   %s\n", GIALLO, RESET);
        printf("%s=============================================%s\n", VIOLA, RESET);
        printf("%s 1.%s AGGIUNGI UN NUOVO ABBONAMENTO\n", VERDE, RESET);
        printf("%s 2.%s MODIFICA/RINNOVA UN ABBONAMENTO\n", VERDE, RESET);
        printf("%s 3.%s ELIMINA UN ABBONAMENTO\n", VERDE, RESET); 
        printf("%s 4.%s VISUALIZZA LISTA ABBONAMENTI\n", VERDE, RESET);
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
                // Aggiunge un nuovo abbonamento
                pulisciSchermo();
                if (aggiungi_abbonamento(&abbonati)) {
                    printf("%sL'abbonamento è stato aggiunto con successo.%s\n", VERDE, RESET);
                } else {
                    printf("%sAbbonamento non aggiunto.%s\n", ROSSO, RESET);
                }
                break;

            case 2:
                // Modifica un abbonamento esistente
                pulisciSchermo();
                list* modificata = modificaAbbonamento(abbonati);  // restituisce nuova lista (o NULL)
                if (modificata != NULL) {
                    abbonati = modificata;  // aggiorna la lista originale
                    salvaAbbonamentiSuFile(abbonati, "abbonamenti.txt");  // salva su file
                }
                pulisciSchermo();
                break;

            case 3:
                // Elimina un abbonamento
                pulisciSchermo();
                abbonati = eliminaAbbonamento(abbonati);  // restituisce lista aggiornata
                break;

            case 4:
                // Visualizza tutti gli abbonamenti ordinati
                pulisciSchermo();
                visualizzaListaOrdinata(abbonati);  // usa funzione che stampa abbonamenti (astratta)
                break;

            default:
                // Scelta non valida
                pulisciSchermo();
                messaggio_errore();  // Messaggio di errore generico
        }
    }
}

// Funzione che aggiunge un nuovo abbonamento alla lista, rispettando l'information hiding
// Parametri:
//   - abbonati: doppio puntatore alla lista degli abbonamenti
// Restituisce:
//   1 se l’abbonamento è stato aggiunto correttamente
//   0 se l’operazione è stata annullata
int aggiungi_abbonamento(list** abbonati) {
    
    abbonamento* nuovo_abbonamento = creaAbbonamento();  

    if (nuovo_abbonamento == NULL) {
    fprintf(stderr, "Errore allocazione memoria per nuovo abbonamento.\n");
    return 0;
                                   }


    char buffer[100];                  // Buffer temporaneo per l’input utente

    // Ottieni il codice abbonamento massimo presente nella lista
    int max_id = getMaxCodiceAbbonamento(*abbonati);

    // Imposta il codice del nuovo abbonamento come max + 1
    setCodiceAbbonamento(nuovo_abbonamento, max_id + 1);

    // Intestazione grafica
    printf("\n========================================================================================================\n");
    printf("                                     Aggiungi Nuovo Abbonamento\n");
    printf("========================================================================================================\n");
    printf("Se vuoi annullare e tornare al menu gestore digitare 'exit' nei campi 'nome' o 'cognome'.\n");
    printf("========================================================================================================\n");

    // ===========================
    // Inserimento del nome cliente
    // ===========================
    while (1) {
        printf("%sInserisci nome cliente: %s", VERDE, RESET);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;  // Rimuove il carattere newline

            if (strcmp(buffer, "exit") == 0) {
                pulisciSchermo();  // Torna al menu
                return 0;
            }

            if (strlen(buffer) > 0 && soloLettere(buffer)) {
                setNome(nuovo_abbonamento, buffer);  // Usa setter
                break;
            } else {
                printf("%sERRORE%s Il nome deve contenere solo lettere e non può essere vuoto.\n", ROSSO, RESET);
            }
        }
    }

    // ==============================
    // Inserimento del cognome cliente
    // ==============================
    while (1) {
        printf("%sInserisci cognome cliente: %s", VERDE, RESET);
        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            buffer[strcspn(buffer, "\n")] = 0;

            if (strcmp(buffer, "exit") == 0) {
                pulisciSchermo();
                return 0;
            }

            if (strlen(buffer) > 0 && soloLettere(buffer)) {
                setCognome(nuovo_abbonamento, buffer);
                break;
            } else {
                printf("%sERRORE%s Il cognome deve contenere solo lettere e non può essere vuoto.\n", ROSSO, RESET);
            }
        }
    }

    // ==============================
    // Inserimento della data di inizio
    // ==============================
    while (1) {
        printf("Data dell'inizio dell'abbonamento di %s %s: \n",
               getNome(nuovo_abbonamento), getCognome(nuovo_abbonamento));

        Data* data_inizio = leggiData();  // Lettura e validazione
        Data* controllo = aggiungiMesi(data_inizio, 1);  // Data di controllo (un mese dopo)

        if (confrontaDate(controllo, oggi()) == -1) {
            printf("%sLa data non può essere precedente di più di un mese rispetto alla data odierna.%s\n", ROSSO, RESET);
        } else {
            setDataInizio(nuovo_abbonamento, data_inizio);
            break;
        }
    }

    // ==============================
    // Inserimento durata abbonamento
    // ==============================
    while (1) {
        printf("%sInserisci durata abbonamento (mesi) MAX 48: %s", VERDE, RESET);
        int durata;
        if (scanf("%d", &durata) != 1 || durata <= 0) {
            while (getchar() != '\n');  // Pulisce il buffer
            printf("%sERRORE%s Inserisci un numero valido di mesi positivo.\n", ROSSO, RESET);
        } else {
            if (durata > 48) {
                printf("%sERRORE%s L'abbonamento può durare massimo 48 mesi.\n", ROSSO, RESET);
            } else {
                setDurata(nuovo_abbonamento, durata);
                break;
            }
        }
    }

    // ==============================
    // Calcolo e stampa della scadenza
    // ==============================
    pulisciSchermo();
    Data* data_scadenza = aggiungiMesi(getDataInizio(nuovo_abbonamento), getDurata(nuovo_abbonamento));

    printf("\n%s=====================================================%s\n", VERDE, RESET);
    printf("%s           ABBONAMENTO INSERITO CON SUCCESSO!!!%s\n", VERDE, RESET);
    printf("%s=====================================================%s\n", VERDE, RESET);

    printf("\n%sL'abbonamento con codice %d di %s %s registrato il: ",
           VERDE,
           getCodiceAbbonamento(nuovo_abbonamento),
           getNome(nuovo_abbonamento),
           getCognome(nuovo_abbonamento));
    stampaData(getDataInizio(nuovo_abbonamento));

    printf("\nScadrà il giorno: ");
    stampaData(data_scadenza);

    printf("\n%s=====================================================%s\n", VERDE, RESET);
    printf("Tutti i dettagli sono stati correttamente registrati.\n");
    printf("%s=====================================================%s\n\n", VERDE, RESET);

    // ==============================
    // Aggiunta alla lista e salvataggio
    // ==============================
    *abbonati = consList(nuovo_abbonamento, *abbonati);  // Inserisce in testa
    salvaAbbonamentiSuFile(*abbonati, "abbonamenti.txt");  // Salva su file

    // ==============================
    // Pausa prima di tornare al menu
    // ==============================
    getchar();  // Per catturare l’invio rimasto da scanf
    printf("\nPremi il tasto invio per continuare...\n");
    getchar();
    pulisciSchermo();  // Pulizia finale dello schermo

    return 1;  // Successo
}

// Funzione per eliminare un abbonamento dalla lista
// Parametri:
//   - abbonati: puntatore alla lista di abbonamenti
// Restituisce:
//   - La lista aggiornata con l'abbonamento eliminato
list* eliminaAbbonamento(list* abbonati) {
    int scelta;

    // Verifica se la lista è vuota
    if (emptyList(abbonati)) {
        // Se la lista è vuota, informa l'utente e torna al menu
        pulisciSchermo();
        printf("%s=============================================================================%s\n", ROSSO, RESET);
        printf("%sLA LISTA DEGLI ABBONATI E' VUOTA QUINDI NON E' POSSIBILE ELIMINARE ELEMENTI%s\n", ROSSO, RESET);
        printf("%s=============================================================================%s\n", ROSSO, RESET);
        printf("Premi invio per continuare......\n");
        getchar();
        pulisciSchermo();
        return abbonati;  // Torna alla lista senza modifiche
    }

    // Ciclo principale per la gestione delle scelte dell'utente
    while (1) {
        // Presenta il menu di selezione per l'eliminazione
        printf("%s==================================================%s\n", GIALLO, RESET);
        printf("%sCome vuoi eliminare l'abbonamento?%s\n", GIALLO, RESET);
        printf("%s1. Per ID%s\n", GIALLO, RESET);
        printf("%s2. Per nome e cognome%s\n", GIALLO, RESET);
        printf("%s0. Torna alla gestione abbonamenti%s\n", ROSSO, RESET);
        printf("%s==================================================%s\n", GIALLO, RESET);
        printf("Per tornare indietro digitare 'exit' nel campo nome o cognome\n");
        printf("\n");
        printf("Scelta: ");
        scanf("%d", &scelta);
        getchar();  // Per consumare il newline rimanente

        // Gestione della scelta dell'utente
        switch (scelta) {
            case 0:
                // Se l'utente sceglie di tornare indietro
                pulisciSchermo();
                return abbonati;  // Torna al menu principale

            case 1:
                // Eliminazione per ID
                abbonati = eliminaPerID(abbonati);
                return abbonati;  // Torna al menu principale

            case 2:
                // Eliminazione per nome e cognome
                abbonati = eliminaPerNomeCognome(abbonati);
                return abbonati;  // Torna al menu principale

            default:
                // Se la scelta non è valida, mostra un messaggio di errore
                printf("%sScelta non valida.%s\n", ROSSO, RESET);
                break;
        }

        // Se l'abbonamento non viene trovato, avvisa l'utente
        pulisciSchermo();
        printf("%sAbbonamento non trovato.%s\n", ROSSO, RESET);
        printf("Premi invio per continuare......\n");
        getchar();
        pulisciSchermo();
    }
}

// Funzione per eliminare un abbonamento per ID
// Parametri:
//   - abbonati: puntatore alla lista di abbonamenti
// Restituisce:
//   - La lista aggiornata con l'abbonamento eliminato
list* eliminaPerID(list* abbonati) {
    int id;
    printf("Inserisci l'ID da eliminare: ");
    scanf("%d", &id);

    list *curr = abbonati, *prev = NULL;

    // Scorri la lista per trovare l'ID da eliminare
    while (curr != NULL) {
        if (getCodiceAbbonamento(getValue(curr)) == id) {  // Usa il getter per ottenere l'ID
            // Se l'ID corrisponde, elimina il nodo
            if (prev == NULL) {
                abbonati = getNext(curr);  // Nodo da eliminare è la testa
            } else {
                setNext(prev, getNext(curr));  // Nodo nel mezzo o in fondo
            }

            distruggiAbbonamento(getValue(curr));  // Libera abbonamento
            free(curr);  // Libera il nodo della lista

            pulisciSchermo();
            salvaAbbonamentiSuFile(abbonati, "abbonamenti.txt");
            printf("%sAbbonamento eliminato con successo.%s\n", VERDE, RESET);
            getchar();
            printf("Premi invio per continuare......\n");
            getchar();
            pulisciSchermo();
            return abbonati;  // Torna al menu principale
        }
        prev = curr;
        curr = getNext(curr);  // Usa il getter per ottenere il prossimo nodo
    }

    // Se l'abbonamento non è stato trovato
    pulisciSchermo();
    printf("%sErrore: nessun abbonamento trovato con ID %d.%s\n", ROSSO, id, RESET);
    getchar();
    printf("Premi invio per continuare......\n");
    getchar();
    pulisciSchermo();

    return abbonati;
}

// Funzione per eliminare un abbonamento per nome e cognome
// Parametri:
//   - abbonati: puntatore alla lista di abbonamenti
// Restituisce:
//   - La lista aggiornata con l'abbonamento eliminato
list* eliminaPerNomeCognome(list* abbonati) {
    char nome[50], cognome[50];

    // Ciclo per il nome
    while (1) {
        printf("%sInserisci nome cliente: %s", VERDE, RESET);
        if (fgets(nome, sizeof(nome), stdin) != NULL) {
            nome[strcspn(nome, "\n")] = 0;  // Rimuove '\n' dalla fine
            if (strcmp(nome, "exit") == 0) {
                pulisciSchermo();
                return abbonati;  // Torna al menu principale
            }
        }
        if (strlen(nome) > 0 && soloLettere(nome)) {
            break;  // Nome valido
        } else {
            printf("%sERRORE%s Il nome deve contenere solo lettere e non può essere vuoto.\n", ROSSO, RESET);
        }
    }

    // Ciclo per il cognome
    while (1) {
        printf("%sInserisci cognome cliente: %s", VERDE, RESET);
        if (fgets(cognome, sizeof(cognome), stdin) != NULL) {
            cognome[strcspn(cognome, "\n")] = 0;  // Rimuove '\n' dalla fine
            if (strcmp(cognome, "exit") == 0) {
                pulisciSchermo();
                return abbonati;  // Torna al menu principale
            }
        }
        if (strlen(cognome) > 0 && soloLettere(cognome)) {
            break;  // Cognome valido
        } else {
            printf("%sERRORE%s Il cognome deve contenere solo lettere e non può essere vuoto.\n", ROSSO, RESET);
        }
    }

    list *curr = abbonati, *prev = NULL;

    // Scorri la lista per trovare nome e cognome da eliminare
    while (curr != NULL) {
        if (strcmp(getNome(getValue(curr)), nome) == 0 && strcmp(getCognome(getValue(curr)), cognome) == 0) {
            // Se nome e cognome corrispondono, elimina il nodo
            if (prev == NULL) {
                abbonati = getNext(curr);  // Nodo da eliminare è la testa
            } else {
                setNext(prev, getNext(curr));  // Nodo nel mezzo o in fondo
            }

            
            free(curr);
            pulisciSchermo();
            salvaAbbonamentiSuFile(abbonati, "abbonamenti.txt");
            printf("%sAbbonamento eliminato con successo.%s\n", VERDE, RESET);
            printf("Premi invio per continuare......\n");
            getchar();
            pulisciSchermo();
            return abbonati;  // Torna al menu principale
        }
        prev = curr;
        curr = getNext(curr);  // Usa il getter per ottenere il prossimo nodo
    }

    // Se l'abbonamento non è stato trovato
    pulisciSchermo();
    printf("%sErrore: nessun abbonamento trovato con con nome: %s e cognome:%s .%s\n", ROSSO, nome,cognome, RESET);
    printf("Premi invio per continuare......\n");
    getchar();
    pulisciSchermo();
    return abbonati;  // Se non trovato, ritorna la lista invariata
}



// Funzione che permette di cercare un abbonamento nella lista
// Parametri:
//   - abbonati: puntatore alla lista degli abbonamenti
// Restituisce:
//   - Puntatore al nodo contenente l’abbonamento trovato
//   - NULL se l’utente torna al menu o se non viene trovato alcun abbonamento
// Descrizione:
//   - Mostra un menu di ricerca (per ID o per nome e cognome)
//   - Valida gli input inseriti dall’utente
//   - Scorre la lista per trovare l’abbonamento corrispondente
list* ricercaAbbonamento(list* abbonati) {
    int scelta;  // Variabile per memorizzare la scelta dell'utente nel menu

    while (1) {  // Ciclo infinito che continua fino a che l'utente non esce
        int trovato = 0;  // Flag per sapere se un abbonamento è stato trovato

        // Menu di ricerca
        printf("%s========================================\n", BLU);
        printf("Come vuoi cercare l'abbonamento?\n");
        printf("========================================%s\n", RESET);
        printf("1. Per ID\n");
        printf("2. Per nome e cognome\n");
        printf("%s0. Torna al menu principale%s\n", ROSSO, RESET);
        printf("%s========================================%s\n", BLU, RESET);

        char scelta_input[10];  // Buffer per l'input della scelta
        char *endptr;

        // Leggi l'input dell'utente
        printf("Scelta: ");
        fgets(scelta_input, sizeof(scelta_input), stdin);
        scelta_input[strcspn(scelta_input, "\n")] = 0;  // Rimuove il newline

        // Converte l'input in un intero
        scelta = strtol(scelta_input, &endptr, 10);  

        // Se la conversione non è valida, mostra errore e ritorna al menu
        if (endptr == scelta_input || *endptr != '\0') {
            pulisciSchermo();
            printf("%sInserisci una scelta valida (solo numeri)!%s\n", ROSSO, RESET);
            continue;  // Riprende il ciclo
        }

        switch (scelta) {
            case 0:  // Caso per tornare al menu principale
                return NULL;  // Uscita dalla funzione

            case 1: {  // Ricerca per ID
                char buffer[100];
                char *endptr;
                int id;

                printf("Inserisci l'ID dell'abbonamento da cercare: ");
                fgets(buffer, sizeof(buffer), stdin);  // Legge l'input dell'ID

                // Rimuove il newline se presente
                buffer[strcspn(buffer, "\n")] = 0;

                id = strtol(buffer, &endptr, 10);  // Converte la stringa a intero

                // Controlla che l'input sia un numero valido
                if (endptr == buffer || *endptr != '\0') {
                    pulisciSchermo();
                    printf("%sInserisci solo numeri!%s\n", ROSSO, RESET);
                    break;  // Esce dalla ricerca
                }

                // Scorre la lista alla ricerca dell'abbonamento con l'ID specificato
                list* curr = abbonati;
                while (curr != NULL) {
                    if (getCodiceAbbonamento(getValue(curr)) == id) {  // Usa il getter per l'ID
                        pulisciSchermo();
                        trovato = 1;  // Segna che l'abbonamento è stato trovato
                        return curr;  // Esce dalla lista e restituisce l'abbonamento trovato
                    }
                    curr =getNext(curr);  // Passa al prossimo abbonamento
                }

                // Se non è stato trovato l'abbonamento
                if (!trovato) {
                    printf("%sAbbonamento NON trovato!%s\n", ROSSO, RESET);
                    printf("Premere INVIO per continuare...\n");
                    getchar();
                    pulisciSchermo();
                    break;
                }

                break;  // Esce dal case 1
            }

            case 2: {  // Ricerca per nome e cognome
                char nome[50], cognome[50];

                printf("Inserisci nome: ");
                fgets(nome, sizeof(nome), stdin);  // Legge il nome
                nome[strcspn(nome, "\n")] = 0;  // Rimuove il newline finale

                printf("Inserisci cognome: ");
                fgets(cognome, sizeof(cognome), stdin);  // Legge il cognome
                cognome[strcspn(cognome, "\n")] = 0;  // Rimuove il newline finale

                // Scorre la lista alla ricerca del nome e cognome
                list* curr = abbonati;
                while (curr != NULL) {
                    if (strcmp(getNome(getValue(curr)), nome) == 0 &&  // Usa il getter per nome
                        strcmp(getCognome(getValue(curr)), cognome) == 0) {  // Usa il getter per cognome
                        pulisciSchermo();
                        trovato = 1;  // Segna che l'abbonamento è stato trovato
                        return curr;  // Esce dalla lista e restituisce l'abbonamento trovato
                    }
                    curr = getNext(curr); // Passa al prossimo abbonamento
                }

                // Se non è stato trovato l'abbonamento
                if (!trovato) {
                    printf("%sAbbonamento NON trovato!%s\n", ROSSO, RESET);
                    printf("Premere INVIO per continuare...\n");
                    getchar();
                    pulisciSchermo();
                    break;
                }

                break;  // Esce dal case 2
            }

            default:  // Caso per scelta non valida
                pulisciSchermo();
                printf("%sScelta non valida, riprova.%s\n", ROSSO, RESET);
                break;  // Ritorna al menu di ricerca
        }
    }
}


// Funzione che gestisce la modifica di un abbonamento esistente
// Accetta come parametro un puntatore alla lista di abbonati (list* abbonati)
// Funzione che gestisce la modifica di un abbonamento esistente
list* modificaAbbonamento(list* abbonati) {
    // Ricerca un abbonamento nella lista
    list* temp = ricercaAbbonamento(abbonati);
    
    // Se l'abbonamento non viene trovato (temp è NULL)
    if (temp == NULL) {
        // Stampa un messaggio di errore
        printf("%sAbbonamento NON trovato!%s\n", ROSSO, RESET);
        printf("Premi INVIO per tornare.........\n");
        getchar();  // Aspetta che l'utente premi INVIO
        pulisciSchermo();  // Pulisce lo schermo
        return NULL;  // Ritorna NULL per indicare che non è stato trovato l'abbonamento
    }

    // Ciclo infinito per la gestione delle modifiche
    while (1) {
        // Se l'abbonamento è stato trovato
        printf("%sAbbonamento trovato!%s\n", VERDE, RESET);
        printf("------------------------------------------\n");
        // Stampa i dettagli dell'abbonamento trovato
        stampaDettagliAbbonamento(getValue(temp));
        
        // Stampa il menu con le opzioni per modificare l'abbonamento
        printf("%s========================================================\n", GIALLO);
        printf("            DIGITA COSA VUOI MODIFICARE\n");
        printf("==========================================================%s\n", RESET);
        printf("1. NOME\n");
        printf("2. COGNOME\n");
        printf("3. DATA INIZIO (anche in caso di rinnovo abbonamento)\n");
        printf("4. DURATA (anche in caso di rinnovo abbonamento)\n");
        printf("%s0. TORNA INDIETRO%s\n", ROSSO, RESET);  // Opzione per tornare indietro
        printf("%s==========================================================%s\n", GIALLO, RESET);
        
        // Variabile per memorizzare la scelta dell'utente
        int scelta = -1;
        char extra;

        // Ciclo per gestire la scelta dell'utente
        while (scelta != 0) {
            // Legge la scelta dell'utente
            printf("Inserisci la tua scelta: ");
            if (scanf("%d%c", &scelta, &extra) != 2 || extra != '\n') {
                pulisciSchermo();  // Pulisce lo schermo
                printf("%sATTENZIONE!! DEVI IMMETTERE 0, 1, 2, 3 O 4 A SECONDA DELLA TUA SCELTA. RIPROVA:%s\n", ROSSO, RESET);
                while (getchar() != '\n');  // Pulisce il buffer
                continue;  // Ritorna all'inizio del ciclo
            }

            // Gestisce la scelta dell'utente
            switch (scelta) {
                // Modifica nome
                case 1:  
                    {
                        // Nascondi il nome durante l'inserimento
                        char nuovoNome[50];
                        printf("Inserisci il nuovo nome: ");
                        scanf("%s", nuovoNome);
                        // Modifica il nome dell'abbonamento tramite il setter
                        setNome(getValue(temp), nuovoNome);
                        printf("%sNome modificato con successo!%s\n", VERDE, RESET);
                        // Salva la lista aggiornata nel file
                        salvaAbbonamentiSuFile(abbonati, "abbonamenti.txt");
                    }
                    break;

                // Modifica cognome
                case 2:  
                    {
                        // Nascondi il cognome durante l'inserimento
                        char nuovoCognome[50];
                        printf("Inserisci il nuovo cognome: ");
                        scanf("%s", nuovoCognome);
                        // Modifica il cognome dell'abbonamento tramite il setter
                        setCognome(getValue(temp), nuovoCognome);
                        printf("%sCognome modificato con successo!%s\n", VERDE, RESET);
                        // Salva la lista aggiornata nel file
                        salvaAbbonamentiSuFile(abbonati, "abbonamenti.txt");
                    }
                    break;

                // Modifica data di inizio
                case 3:
                    {
                        // Richiede e legge la nuova data di inizio
                        Data* nuovaDataInizio = leggiData();
                        // Modifica la data di inizio dell'abbonamento tramite il setter
                        setDataInizio(getValue(temp), nuovaDataInizio);
                        printf("%sData di inizio modificata con successo!%s\n", VERDE, RESET);
                        // Salva la lista aggiornata nel file
                        salvaAbbonamentiSuFile(abbonati, "abbonamenti.txt");
                    }
                    break;

                // Modifica durata
                case 4:  
                    {
                        // Nascondi la durata durante l'inserimento
                        int nuovaDurata;
                        printf("Inserisci la nuova durata (in mesi): ");
                        scanf("%d", &nuovaDurata);
                        // Modifica la durata dell'abbonamento tramite il setter
                        setDurata(getValue(temp), nuovaDurata);
                        printf("%sDurata modificata con successo!%s\n", VERDE, RESET);
                        // Salva la lista aggiornata nel file
                        salvaAbbonamentiSuFile(abbonati, "abbonamenti.txt");
                    }
                    break;

                // Torna indietro
                case 0:
                    return abbonati;

                // Caso in cui l'utente inserisce una scelta non valida
                default:
                    printf("%sScelta non valida, riprova!%s\n", ROSSO, RESET);
                    break;
            }
        }
    }

    // Ritorna la lista aggiornata
    pulisciSchermo();
    return abbonati;
}