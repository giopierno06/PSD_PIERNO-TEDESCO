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

// Funzione che mostra il menu iniziale (menu principale)
void menu_iniziale() {
    printf("\n%s========================================================================%s\n", GIALLO, RESET);
    printf("  %s BENVENUTO NEL MENU PRINCIPALE DELLA TUA PLAESTRA PREFERITA%s\n", GIALLO, RESET);
    printf("%s========================================================================%s\n", GIALLO, RESET);
    printf("\n%s A quale menù vuoi accedere?:  %s\n\n", BLU, RESET);
    printf("  %s1. Menu Gestore %s\n", VERDE, RESET);
    printf("  %s2. Menu Cliente %s\n", VERDE,RESET);
    printf("  %s0. Esci %s (Ci vediamo alla prossima!)\n", ROSSO, RESET);
    printf("\n%s========================================================================%s\n", GIALLO, RESET);
    printf("    %sSeleziona un'opzione (1, 2, 0): %s", BLU, RESET);
}

// Funzione che mostra il menu gestionale (per amministratori della palestra)
void menu_gestore() {
    int scelta = -1; // Variabile per memorizzare la scelta dell'utente
    while (1) { // Ciclo infinito, si esce solo con "return"
        // Stampa il menu
        printf("\n%s=================================%s\n", VIOLA, RESET);
        printf("%s   BENVENUTO NEL MENU GESTIONALE   %s\n", GIALLO, RESET);
        printf("%s===================================%s\n", VIOLA, RESET);
        printf("%s 1.%s Gestisci gli Abbonamenti\n", VERDE, RESET);
        printf("%s 2.%s Gestisci le Prenotazioni e Report\n", VERDE, RESET);
        printf("%s 3.%s Gestisci le Lezioni\n", VERDE, RESET);
        printf("%s 0. TORNA AL MENU PRINCIPALE %s\n", ROSSO, RESET);
        printf("%s------------------------------%s\n", VIOLA, RESET);
        printf("%s  Inserisci la tua scelta: %s", GIALLO, RESET);

        char extra; // Carattere usato per controllare input "sporco"

        // Legge la scelta e controlla che sia un numero valido senza altri caratteri
        if (scanf("%d%c", &scelta, &extra) != 2 || extra != '\n') {
            pulisciSchermo();        // Funzione per pulire lo schermo
            messaggio_errore();       // Funzione che stampa messaggio di errore
            while (getchar() != '\n'); // Pulisce eventuali caratteri residui nel buffer
            continue;
        }

        // Gestione delle scelte tramite switch
        switch (scelta) {
            case 0:
                pulisciSchermo();
                return; // Esce dal menu gestionale e torna al menu principale
            case 1:
                pulisciSchermo();
                gestione_abbonamenti(); // Funzione per gestire gli abbonamenti
                break;
            case 2:
                 pulisciSchermo();
                 gestione_prenotazioni();
                 break;
            case 3:
                 pulisciSchermo();
                 gestione_Lezioni();
                 break;
            
            default:
                pulisciSchermo();
                messaggio_errore(); // Se la scelta non è valida
        }
    }
}


list* controlloCliente() {
 
    // Carica gli abbonamenti dal file se non sono già stati caricati
     if(abbonati == NULL)    
     abbonati = caricaAbbonamentiDaFile(abbonati, "abbonamenti.txt");  // Carica gli abbonamenti dal file
    
    
    
    char buffer[100];
    char nome[50], cognome[50];
    int id;
    char *endptr;

    while (1) {
        pulisciSchermo();

        printf("%s=====================================================================================\n", GIALLO);
        printf("        PER ACCEDERE ALL'AREA PERSONALE INSERIRE NOME, COGNOME E CODICE ABBONAMENTO\n");
        printf("=======================================================================================%s\n", RESET);
        printf("Per annullare e tornare indietro inserire 'exit' nel campo nome o cognome oppure '0' nel campo codice abbonamento.\n\n");

        // --- Input ID Abbonamento ---
        printf("Inserisci l'ID dell'abbonamento da cercare: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;

        // Controllo se l'utente vuole uscire
        if (strcmp(buffer, "0") == 0)  {
            pulisciSchermo();
            return NULL;}
        id = strtol(buffer, &endptr, 10);
        if (endptr == buffer || *endptr != '\0') {
            printf("%sInserisci solo numeri!%s\n", ROSSO, RESET);
            printf("Premi INVIO per continuare...\n");
            getchar();
            continue;
        }

        // --- Input Nome ---
        printf("Inserisci nome: ");
        fgets(nome, sizeof(nome), stdin);
        nome[strcspn(nome, "\n")] = 0;

        if (strcmp(nome, "exit") == 0) {
            pulisciSchermo();
            return NULL;}

        // --- Input Cognome ---
        printf("Inserisci cognome: ");
        fgets(cognome, sizeof(cognome), stdin);
        cognome[strcspn(cognome, "\n")] = 0;

        if (strcmp(cognome, "exit") == 0){
            pulisciSchermo();
            return NULL;}

        // --- Ricerca nella lista ---
        list* curr = abbonati;
        while (curr != NULL) {
            if (getCodiceAbbonamento(getValue(curr)) == id &&
                strcmp(getNome(getValue(curr)), nome) == 0 &&
                strcmp(getCognome(getValue(curr)), cognome) == 0) {
                pulisciSchermo();
                printf("%sUtente riconosciuto ciao %s %s !%s\n", VERDE,nome,cognome, RESET);
                printf("Premi INVIO per accedere......\n");
                getchar();
                return curr;
            }
            curr = getNext(curr);
        }

        // --- Se non trovato ---
        printf("%sAbbonamento NON trovato!%s\n", ROSSO, RESET);
        printf("Premi INVIO per riprovare...\n");
        getchar();
    }
}

// Funzione che mostra il menu clienti
void menu_cliente() {
     

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




    list* nodo = controlloCliente();
    if (nodo == NULL) {
         return;
    }
    
    //inserisci i dati dell abbonato all'interno di abbinato_corrente
    abbonamento* abbonato_corrente = getValue(nodo);
    printf("Benvenuto %s %s\n", getNome(abbonato_corrente), getCognome(abbonato_corrente));
    
    int scelta = -1; // Variabile per memorizzare la scelta dell'utente
    while (1) { // Ciclo infinito, si esce solo con "return"
        // Stampa il menu
        pulisciSchermoCliente(getNome(abbonato_corrente), getCognome(abbonato_corrente));
        printf("\n%s=================================%s\n", BLU, RESET);
        printf("%s   BENVENUTO NEL MENU CLIENTI  %s\n", GIALLO, RESET);
        printf("%s===================================%s\n", BLU, RESET);
        printf("%s 1.%s Controlla Validità Abbonamento \n", VERDE, RESET);
        printf("%s 2.%s Prenota Lezione\n", VERDE, RESET);
        printf("%s 3.%s Visualizza report mensile\n", VERDE, RESET);
        printf("%s 4.%s Visualizza tue prenotazioni\n", VERDE, RESET); // 3 e 4 aggiungere allo switch e al progetto
        printf("%s 5.%s Disdici una prenotazione\n", VERDE, RESET);
        printf("%s 0. TORNA AL MENU PRINCIPALE\n %s", ROSSO, RESET);
        printf("%s------------------------------%s\n", BLU, RESET);
        printf("%s  Inserisci la tua scelta: %s", GIALLO, RESET);

        char extra; // Carattere di controllo

        // Legge la scelta e verifica che sia solo un numero
        if (scanf("%d%c", &scelta, &extra) != 2 || extra != '\n') {
            pulisciSchermo();
            messaggio_errore();
            while (getchar() != '\n'); // Svuota il buffer in caso di input errato
            continue;
        }

        // Gestione delle scelte tramite switch
        switch (scelta) {
            case 0:
                pulisciSchermo();
                return; // Torna al menu principale
            
            case 1:
                controllo_abbonamento(abbonato_corrente); // Funzione per controllare validità abbonamento
                break;
            
            case 2:
                aggiungi_prenotazioneCliente(abbonato_corrente);// Funzione per prenotare una lezione
                break;
            case 3:
                reportLezioniUltimoMeseCliente(getCodiceAbbonamento(abbonato_corrente),lista_prenotazioni , lezioni);
                break;
            case 4:
                visualizzaPrenotazioniCliente(getCodiceAbbonamento(abbonato_corrente),  lista_prenotazioni, lezioni);
                 break;
            case 5:
                disdici_prenotazioneCliente(abbonato_corrente);
                break;            
            default:
                pulisciSchermo();
                printf("\n\n");
                messaggio_errore(); // Se la scelta è fuori dai casi previsti
        }
    }
}
