#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h" 
#include "util.h"
#include "list.h"

extern list* abbonati;

int main_program() {
    int scelta = -1; // variabile per scelta menu
    char buffer[100]; // buffer per leggere l'intera riga

    pulisciSchermo();
    while (1) {
        menu_iniziale(); // 0: termina, 1: menu gestore, 2: menu cliente

        if (!fgets(buffer, sizeof(buffer), stdin)) {
            pulisciSchermo();
            messaggio_errore();
            continue;
        }

        // Rimuove eventuale newline alla fine
        buffer[strcspn(buffer, "\r\n")] = '\0';

        if (sscanf(buffer, " %d", &scelta) != 1 || scelta < 0 || scelta > 2) {
            pulisciSchermo();
            messaggio_errore();
            continue;
        }

        switch (scelta) {
            case 0:
                pulisciSchermo();
                schermata_arrivederci();
                freeList(abbonati);
                return 0;
            case 1:
                pulisciSchermo();
                menu_gestore();
                break;
            case 2:
                pulisciSchermo();
                menu_cliente();
                break;
        }
    }
    return 0;
}

int main() {
    return main_program();
}