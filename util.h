#ifndef UTIL_H
#define UTIL_H


int soloLettere(char *str); // Funzione per verificare se la stringa contiene solo lettere
void pulisciSchermo();  //comando che pulisce lo schermo in fase di esecuzione stampa data e ora
void pulisciSchermoCliente(const char*,const char*);  //comando che pulisce lo schermo in fase di esecuzione  stampa data ora nome e cognome del cliente in corso
void schermata_arrivederci(); //schermata visuallizata nel momengo in cui si abbandona il programma
void messaggio_errore(); //messaggio di errore

#endif