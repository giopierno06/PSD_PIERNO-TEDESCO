#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "gestione_Abbonamenti.h"
#include "util.h"
#include "data.h"
#include "list.h"
#include "gestione_prenotazioni.h"
#include "lezione.h"
#include "gestione_lezioni.h"
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
 * Funzione: menu_iniziale
 * -----------------------
 * Stampa il menu principale dell'applicazione palestra, mostrando all'utente 
 * le opzioni disponibili: accedere al menu gestore, al menu cliente oppure uscire.
 * La funzione utilizza colori ANSI (definiti tramite macro come GIALLO, BLU, VERDE, ROSSO, RESET)
 * per rendere il testo più leggibile e gradevole visivamente.
 *
 * Non riceve parametri e non restituisce valori, si limita a mostrare il menu.
 */
void menu_iniziale() {
    // Stampa una linea orizzontale colorata in giallo per delimitare il menu
    printf("\n%s========================================================================%s\n", GIALLO, RESET);

    // Stampa il titolo del menu principale in giallo, con un messaggio di benvenuto personalizzato
    printf("  %s BENVENUTO NEL MENU' PRINCIPALE DELLA TUA PLAESTRA PREFERITA%s\n", GIALLO, RESET);

    // Stampa una seconda linea orizzontale in giallo per chiudere la sezione del titolo
    printf("%s========================================================================%s\n", GIALLO, RESET);

    // Stampa una riga vuota e un messaggio in blu che chiede all'utente quale menu desidera aprire
    printf("\n%s A quale menu' vuoi accedere?:  %s\n\n", BLU, RESET);

    // Opzione 1: Menu Gestore, mostrata in verde per evidenziare l'opzione selezionabile
    printf("  %s1. Menu Gestore %s\n", VERDE, RESET);

    // Opzione 2: Menu Cliente, anch'essa in verde
    printf("  %s2. Menu Cliente %s\n", VERDE, RESET);

    // Opzione 0: Esci dal programma, evidenziata in rosso come scelta di terminazione
    printf("  %s0. Esci %s (Ci vediamo alla prossima!)\n", ROSSO, RESET);

    // Stampa una linea orizzontale in giallo per chiudere il menu
    printf("\n%s========================================================================%s\n", GIALLO, RESET);

    // Infine, stampa la richiesta di input per selezionare una delle opzioni, in blu
    printf("    %sSeleziona un'opzione (1, 2, 0): %s", BLU, RESET);
}
/*
 * Funzione: menu_gestore
 * ----------------------
 * Mostra il menu gestionale per l'utente "gestore" della palestra. 
 * Consente di scegliere tra varie opzioni per gestire abbonamenti, prenotazioni, lezioni 
 * o tornare al menu principale. 
 * La funzione legge l'input da tastiera, verifica la validità della scelta 
 * ed esegue l'azione corrispondente.
 * Usa colori ANSI per rendere il menu più leggibile e piacevole.
 *
 * Non riceve parametri e non restituisce valori, ma ritorna al chiamante se l'utente sceglie di uscire.
 */
void menu_gestore() {
    char input[10]; // buffer per leggere la scelta dell'utente come stringa
    int scelta;     // variabile per memorizzare la scelta convertita in intero

    while (1) { // ciclo infinito finché l'utente non sceglie di uscire (caso 0)
        // Stampa il bordo superiore del menu colorato in viola
        printf("\n%s=================================%s\n", VIOLA, RESET);
        
        // Titolo del menu in giallo
        printf("%s   BENVENUTO NEL MENU GESTIONALE   %s\n", GIALLO, RESET);
        
        // Stampa il bordo sotto il titolo in viola
        printf("%s===================================%s\n", VIOLA, RESET);

        // Opzioni del menu, tutte in verde tranne l'uscita in rosso
        printf("%s 1.%s Gestisci gli Abbonamenti\n", VERDE, RESET);
        printf("%s 2.%s Gestisci le Prenotazioni e Report\n", VERDE, RESET);
        printf("%s 3.%s Gestisci le Lezioni\n", VERDE, RESET);
        printf("%s 0.%s TORNA AL MENU PRINCIPALE\n", ROSSO, RESET);

        // Linea divisoria in viola
        printf("%s------------------------------%s\n", VIOLA, RESET);

        // Prompt per l'inserimento della scelta, in giallo
        printf("%s  Inserisci la tua scelta: %s", GIALLO, RESET);

        // Legge l'intera riga di input da stdin, salva in input[]
        if (!fgets(input, sizeof(input), stdin)) {
            // Se la lettura fallisce, mostra un messaggio di errore e ricomincia il ciclo
            messaggio_errore();
            continue;
        }

        // Rimuove il carattere newline finale dalla stringa input, se presente
        input[strcspn(input, "\n")] = 0;

        // Converte la stringa input in intero (scelta)
        // Se non è un numero valido, mostra errore e ricomincia il ciclo
        if (sscanf(input, "%d", &scelta) != 1) {
            pulisciSchermo();
            messaggio_errore();
            continue;
        }

        // Gestisce le scelte con switch-case
        switch (scelta) {
            case 0:  // Uscita dal menu gestionale: pulisce schermo e ritorna alla funzione chiamante
                pulisciSchermo();
                return;

            case 1:  // Gestione abbonamenti
                pulisciSchermo();
                gestione_abbonamenti();
                break;

            case 2:  // Gestione prenotazioni e report
                pulisciSchermo();
                gestione_prenotazioni();
                break;

            case 3:  // Gestione lezioni
                pulisciSchermo();
                gestione_Lezioni();
                break;

            default: // Qualsiasi altro input non valido
                pulisciSchermo();
                messaggio_errore();
        }
    }
}

/*
 * Funzione: controlloCliente
 * --------------------------
 * Permette all'utente di accedere all'area personale inserendo nome, cognome e codice abbonamento.
 * Se gli abbonamenti non sono già caricati, li carica dal file "programma/abbonamenti.txt".
 * Legge input da tastiera con controlli di validità e permette di uscire inserendo 'exit' o '0'.
 * Cerca nella lista degli abbonati un match con i dati inseriti.
 * Se trovato, restituisce il nodo della lista relativo all'abbonamento.
 * Altrimenti ripete la richiesta.
 *
 * Restituisce:
 *  - puntatore al nodo list contenente i dati dell'abbonato riconosciuto,
 *  - NULL se l'utente decide di uscire.
 */
list* controlloCliente() {

    // Se la lista di abbonati non è stata caricata, la carica dal file
    if(abbonati == NULL)    
        abbonati = caricaAbbonamentiDaFile(abbonati, "programma/abbonamenti.txt");  
    
    char buffer[100];     // buffer per input generico (id abbonamento)
    char nome[50], cognome[50];  // buffer per nome e cognome
    int id;               // variabile per codice abbonamento
    char *endptr;         // puntatore usato da strtol per controllare input

    while (1) { // ciclo infinito finché l'utente non esce o inserisce dati validi
        pulisciSchermo();

        // Stampa intestazione con istruzioni
        printf("%s=====================================================================================\n", GIALLO);
        printf("        PER ACCEDERE ALL'AREA PERSONALE INSERIRE NOME, COGNOME E CODICE ABBONAMENTO\n");
        printf("=======================================================================================%s\n", RESET);
        printf("Per annullare e tornare indietro inserire 'exit' nel campo nome o cognome oppure '0' nel campo codice abbonamento.\n\n");

        // --- Input codice abbonamento ---
        printf("Inserisci l'ID dell'abbonamento da cercare: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // rimuove newline

        // Controllo se l'utente vuole uscire
        if (strcmp(buffer, "0") == 0)  {
            pulisciSchermo();
            return NULL;  // esce dalla funzione
        }

        // Converte stringa in intero e verifica che sia valido
        id = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\0') { // se input non è un numero valido
            printf("%sInserisci solo numeri!%s\n", ROSSO, RESET);
            printf("Premi INVIO per continuare...\n");
            getchar(); // attende invio
            continue;  // ripete il ciclo
        }

        // --- Input nome ---
        printf("Inserisci nome: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = 0;  // rimuove newline

        if (strcmp(nome, "exit") == 0) {  // controllo uscita
            pulisciSchermo();
            return NULL;
        }

        // --- Input cognome ---
        printf("Inserisci cognome: ");
        fgets(cognome, sizeof(cognome), stdin);
        cognome[strcspn(cognome, "\n")] = 0;  // rimuove newline

        if (strcmp(cognome, "exit") == 0) {  // controllo uscita
            pulisciSchermo();
            return NULL;
        }

        // --- Ricerca dell'abbonamento nella lista ---
        list* curr = abbonati;  // puntatore temporaneo per scorrere la lista
        while (curr != NULL) {
            // Confronta id, nome e cognome con quelli correnti nella lista
            if (getCodiceAbbonamento(getValue(curr)) == id &&
                strcmp(getNome(getValue(curr)), nome) == 0 &&
                strcmp(getCognome(getValue(curr)), cognome) == 0) {
                
                pulisciSchermo();
                printf("%sUtente riconosciuto ciao %s %s !%s\n", VERDE, nome, cognome, RESET);
                printf("Premi INVIO per accedere......\n");
                getchar();
                return curr;  // ritorna il nodo trovato
            }
            curr = getNext(curr);  // passa al prossimo nodo
        }

        // --- Se non trovato ---
        printf("%sAbbonamento NON trovato!%s\n", ROSSO, RESET);
        printf("Premi INVIO per riprovare...\n");
        getchar();
    }
}
/*
 * Funzione: menu_cliente
 * ----------------------
 * Mostra il menu principale dedicato ai clienti.
 * 
 * Prima carica le liste di prenotazioni, abbonamenti e lezioni dai rispettivi file,
 * inizializzando liste vuote se i file non esistono o sono vuoti.
 * 
 * Successivamente chiama la funzione controlloCliente per permettere all'utente di
 * autenticarsi inserendo nome, cognome e codice abbonamento.
 * 
 * Se il cliente viene riconosciuto, viene mostrato un menu con varie opzioni per gestire:
 *  - la validità dell'abbonamento,
 *  - la prenotazione di lezioni,
 *  - la visualizzazione di report mensili,
 *  - la visualizzazione e cancellazione delle prenotazioni.
 * 
 * L'utente può tornare al menu principale con l'opzione 0.
 */
void menu_cliente() {

    // Carica la lista delle prenotazioni da file, se non caricata
    if (lista_prenotazioni == NULL)
        lista_prenotazioni = caricaPrenotazioniDaFile(lista_prenotazioni, "programma/prenotazioni.txt");
    
    // Se non esiste ancora una lista (file vuoto o inesistente), ne crea una nuova vuota
    if (lista_prenotazioni == NULL)
        lista_prenotazioni = prenotazione_newList();

    // Carica gli abbonamenti da file se non caricati
    if (abbonati == NULL)
        abbonati = caricaAbbonamentiDaFile(abbonati, "programma/abbonamenti.txt");

    // Se non esiste ancora una lista abbonati, ne crea una nuova vuota
    if (abbonati == NULL)
        abbonati = newList();

    // Carica le lezioni da file se non caricate
    if (lezioni == NULL)
        lezioni = caricaLezioniDaFile(lezioni, "programma/lezioni.txt");

    // Se non esiste ancora una lista lezioni, ne crea una nuova vuota
    if (lezioni == NULL)
        lezioni = lezione_newList();

    // Controlla l'identità del cliente e ottiene il nodo corrispondente alla sua iscrizione
    list* nodo = controlloCliente();
    if (nodo == NULL) {
        // Se l'utente annulla il login, ritorna al menu principale
        return;
    }

    // Ottiene i dati dell'abbonato dalla lista
    abbonamento* abbonato_corrente = getValue(nodo);
    printf("Benvenuto %s %s\n", getNome(abbonato_corrente), getCognome(abbonato_corrente));

    int scelta = -1;  // Variabile per memorizzare la scelta dell'utente

    while (1) {  // Ciclo infinito finché l'utente non decide di uscire
        // Pulisce lo schermo e stampa il menu cliente con intestazione
        pulisciSchermoCliente(getNome(abbonato_corrente), getCognome(abbonato_corrente));
        printf("\n%s=================================%s\n", BLU, RESET);
        printf("%s   BENVENUTO NEL MENU CLIENTI  %s\n", GIALLO, RESET);
        printf("%s===================================%s\n", BLU, RESET);
        printf("%s 1.%s Controlla Validità Abbonamento \n", VERDE, RESET);
        printf("%s 2.%s Prenota Lezione\n", VERDE, RESET);
        printf("%s 3.%s Visualizza report mensile\n", VERDE, RESET);
        printf("%s 4.%s Visualizza tue prenotazioni\n", VERDE, RESET);
        printf("%s 5.%s Disdici una prenotazione\n", VERDE, RESET);
        printf("%s 0. TORNA AL MENU PRINCIPALE\n %s", ROSSO, RESET);
        printf("%s------------------------------%s\n", BLU, RESET);
        printf("%s  Inserisci la tua scelta: %s", GIALLO, RESET);

        char extra;  // Variabile di controllo per input errato

        // Legge la scelta e verifica che sia un numero intero seguito da newline
        if (scanf("%d%c", &scelta, &extra) != 2 || extra != '\n') {
            pulisciSchermo();
            messaggio_errore();
            while (getchar() != '\n');  // Svuota il buffer in caso di input errato
            continue;  // Ripete il menu
        }

        // Gestione delle scelte tramite switch
        switch (scelta) {
            case 0:
                pulisciSchermo();
                return;  // Torna al menu principale

            case 1:
                controllo_abbonamento(abbonato_corrente);  // Controlla validità abbonamento
                break;

            case 2:
                aggiungi_prenotazioneCliente(abbonato_corrente);  // Prenota una lezione
                break;

            case 3:
                reportLezioniUltimoMeseCliente(getCodiceAbbonamento(abbonato_corrente), lista_prenotazioni, lezioni);  // Visualizza report
                break;

            case 4:
                visualizzaPrenotazioniCliente(getCodiceAbbonamento(abbonato_corrente), lista_prenotazioni, lezioni);  // Mostra prenotazioni
                break;

            case 5:
                disdici_prenotazioneCliente(abbonato_corrente);  // Disdice una prenotazione
                break;

            default:
                pulisciSchermo();
                printf("\n\n");
                messaggio_errore();  // Se la scelta è fuori dai casi previsti
        }
    }
}