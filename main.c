#include <stdio.h>
#include <stdlib.h>
#include "menu.h" 
#include "util.h"
#include "list.h"


extern list* abbonati;


int main (){
    
    
    int scelta=-1; //variabile usata per la prima scelta del menù
    pulisciSchermo();
    while(1){
    
    menu_iniziale(); //0 terminare 1 per menu gestore 2 per menu cliente
    
    char extra;


if (scanf("%d%c", &scelta, &extra) != 2 || extra != '\n') {
    pulisciSchermo();
    messaggio_errore();
    while (getchar() != '\n'); // svuota il buffer
    continue;
}

    switch(scelta){               //swtich per la scelta del menu 
        case 0:
        pulisciSchermo();           
        schermata_arrivederci();
        freeList(abbonati);
        return 0;
            break;
        case 1:
        pulisciSchermo();
        menu_gestore();
        break;
        case 2:
        pulisciSchermo();    
        menu_cliente();
        break;
        default:
        pulisciSchermo();  
        messaggio_errore();
    }



     }
    return 0;
    
}