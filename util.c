#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "data.h"
#include "orario.h"

// Definizione di alcune costanti per il colore del testo
#define VIOLA    "\033[1;35m"  // Colore viola per il testo
#define ROSSO    "\033[1;31m"  // Colore rosso per il testo
#define VERDE    "\033[1;32m"  // Colore verde per il testo
#define GIALLO   "\033[1;33m"  // Colore giallo per il testo
#define RESET    "\033[0m"     // Codice di reset per tornare al colore di default
#define BLU      "\033[1;34m"  // Colore blu per il testo

// Funzione per verificare se una stringa contiene solo lettere
int soloLettere(char *str) {
    // Cicla attraverso ogni carattere della stringa
    for (int i = 0; str[i] != '\0'; i++) {
        // Se il carattere non è una lettera maiuscola o minuscola
        if (!((str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= 'a' && str[i] <= 'z')||(str[i]== ' ')||(str[i]== '\''))) {
            return 0;  // Restituisce 0 se trova un carattere non lettera o non spazio o non apostrofo
        }
    }
    return 1;  // Restituisce 1 se la stringa contiene solo lettere
}

// Funzione per pulire lo schermo, utilizzando comandi specifici per il sistema operativo
void pulisciSchermo() {
    #ifdef _WIN32
        // Se il sistema operativo è Windows, utilizza il comando cls per pulire lo schermo
        system("cls");
    #else
        // Altrimenti, su Linux e macOS, usa il comando clear
        system("clear");
    #endif
    
    // Mostra la data odierna, utilizzando il formato di colore definito prima
    printf("%sDATA ODIERNA:  %s", BLU, RESET);
    
    // Chiama la funzione oggi() per ottenere la data odierna e la passa alla funzione stampaData() per mostrarla
    stampaData(oggi());  // stampaData() mostra la data odierna formattato
    printf("\n");
}

// Funzione per pulire lo schermo, utilizzando comandi specifici per il sistema operativo per il cliente
void pulisciSchermoCliente(const char nome[50],const char cognome[50]){
    #ifdef _WIN32
        // Se il sistema operativo è Windows, utilizza il comando cls per pulire lo schermo
        system("cls");
    #else
        // Altrimenti, su Linux e macOS, usa il comando clear
        system("clear");
    #endif
    
    // Mostra la data odierna, utilizzando il formato di colore definito prima
    printf("%sDATA ODIERNA:  %s", BLU, RESET);
    
    // Chiama la funzione oggi() per ottenere la data odierna e la passa alla funzione stampaData() per mostrarla
    stampaData(oggi());  // stampaData() mostra la data odierna formattat
    printf("\n");
 
    printf("%sBENVENUTO NELLA TUA AREA PERSONALE: %s %s %s",VERDE,nome,cognome, RESET);

}
// Funzione per stampare un messaggio di errore quando l'utente inserisce una scelta non valida
void messaggio_errore() {
    // Stampa il messaggio di errore con testo rosso
    printf("\033[1;31mATTENZIONE!! DEVI IMMETTERE 0,1,2 A SECONDA DELLA TUA SCELTA RIPROVA: \033[0m\n");
}

// Funzione per stampare un messaggio di arrivederci quando l'utente esce dal programma
void schermata_arrivederci() {
    // Stampa il messaggio di arrivederci con una cornice gialla
    printf("\n\n%s====================================================%s\n", GIALLO, RESET);
    printf("                 Grazie per la tua visita! \n");
    printf("                 Torna a trovarci presto! \n");
    printf("%s====================================================%s\n", GIALLO, RESET);
}