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

//menu gestione lezioni
void gestione_Lezioni() {
    
       if (lezioni == NULL) {
        lezioni = lezione_newList();  // Assicurati che la lista venga inizializzata correttamente
        lezioni = caricaLezioniDaFile(lezioni, "lezioni.txt");
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
                break;

            case 3:
                // Funzione per eliminare una lezione
                break;

            case 4:
                pulisciSchermo();
                visualizzaLezioni(lezioni);  // Passa direttamente il puntatore alla lista
                getchar();
                pulisciSchermo();
                break;

            default:
                pulisciSchermo(); // In caso di scelta non valida
                messaggio_errore(); // Funzione che stampa un messaggio d’errore generico
        }
    }
}

  


int aggiungi_lezione(listL** lezioni) {
    lezione* nuova_lezione = creaLezione();
    
    if (nuova_lezione == NULL) {
        printf("Errore nella creazione della lezione!\n");
        return 0;
    }

    int max_id=0;
    max_id = getMaxCodiceLezione(*lezioni);
    setCodiceLezione(nuova_lezione, max_id + 1);
    
    // Scelta disciplina
    while (1) {
        printf("\n========================================================================================================\n");
        printf("                                     Aggiungi Nuova Lezione\n");
        printf("========================================================================================================\n"); 
        printf("Se vuoi annullare e tornare al menu gestore digitare 'exit' nel campo 'nome'\n");
        printf("========================================================================================================\n");



        int scelta;
        char buffer_nome[50];

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
                printf("%sInserisci manualmente la disciplina: %s", VERDE, RESET);
                if (fgets(buffer_nome, sizeof(buffer_nome), stdin) != NULL) {
                    buffer_nome[strcspn(buffer_nome, "\n")] = '\0';
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

    // Inserimento nome lezione
    while (1) {
        char buffer_nome[50];
        printf("%sInserisci nome lezione: %s", VERDE, RESET);
        if (fgets(buffer_nome, sizeof(buffer_nome), stdin) != NULL) {
            buffer_nome[strcspn(buffer_nome, "\n")] = '\0';
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

    // Numero massimo posti
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

    // Data
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
    // Orario
    printf("%sOrario della lezione: %s\n", VERDE, RESET);
    Orario* orario_inizio = leggiOrario();
    setOrarioInizio(nuova_lezione, orario_inizio);
    // Durata
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

    pulisciSchermo();
    Orario* orario_fine = aggiungiMinuti(getOrarioLezione(nuova_lezione), getDurataLezione(nuova_lezione));

    // Riepilogo
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
    
    *lezioni = lezione_consList(nuova_lezione, *lezioni);
    salvaLezioniSuFile(*lezioni,"lezioni.txt");
    getchar();
    printf("\nPremi INVIO per continuare...\n");
    getchar();
    pulisciSchermo();

    return 1;
}

void lezione_printByDate(listL* lezioni, Data* data_input) {
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

    pulisciSchermo();
    printf("\n%sVisualizzazione lezioni per la data: ",GIALLO);
    stampaData(data_input);  // Funzione per stampare la data
    printf("\n%s",RESET);

    // Intestazione
    printf("%-15s | %-20s | %-20s | %-15s | %-17s\n",
           "Codice", "Nome", "Disciplina", "Orario", "Posti Rimanenti");
    printf("-----------------------------------------------------------------------------------------------\n");

    while (temp != NULL) {
        lezione* lezione_corrente = lezione_getValue(temp);

        if (lezione_corrente == NULL) {
            temp = lezione_getNext(temp);
            continue;
        }

        Data* data_lezione = getDataLezione(lezione_corrente);
        if (data_lezione == NULL) {
            temp = lezione_getNext(temp);
            continue;
        }

        if (confrontaDate(data_input, data_lezione) == 0) {
            int posti_max = getPostiMax(lezione_corrente);
            int prenotati = getPostiOccupati(lezione_corrente);
            int posti_rimanenti = posti_max - prenotati;

            Orario* orario_inizio = getOrarioLezione(lezione_corrente);
            int durata = getDurataLezione(lezione_corrente);

          // Calcola l'orario di fine con la funzione aggiungiMinuti
         Orario* orario_fine = aggiungiMinuti(orario_inizio, durata);

         // Ora costruiamo la stringa per l'orario di inizio e fine
        char orario_str[20];
        snprintf(orario_str, sizeof(orario_str), "%02d:%02d - %02d:%02d",
        getOra(orario_inizio), getMinuto(orario_inizio),
        getOra(orario_fine), getMinuto(orario_fine));

            // Stampa tabella
            printf("%-15d | %-20s | %-20s | %-15s | %-17d\n",
                   getCodiceLezione(lezione_corrente),
                   getNomeLezione(lezione_corrente),
                   getDisciplinaLezione(lezione_corrente),
                   orario_str,
                   posti_rimanenti);

            trovato = 1;
        }

        temp = lezione_getNext(temp);
    }

    if (!trovato) {
        printf("\n%sNessuna lezione in questa data.%s\n", ROSSO, RESET);
    }

    getchar();
    printf("\nPremi INVIO per tornare indietro.....");
    getchar();
    pulisciSchermo();
}

// Funzione per visualizzare tutte le lezioni
void visualizzaLezioni(listL* lezioni) {
    int scelta;
    Data* data_input = NULL;

    while (1) {
        // Mostra il menu per la scelta dell'utente
        printf("\n%s===================================\n", GIALLO);
        printf("          Scegli un'opzione:\n");
        printf("===================================%s\n", RESET);
        printf("1. Visualizza lezioni per un giorno specifico\n");
        printf("2. Visualizza tutte le lezioni\n");
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