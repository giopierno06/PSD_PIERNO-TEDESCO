#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Includi le dichiarazioni delle funzioni del progetto
#include "/mnt/c/users/giova/OneDrive/Desktop/PROGETTO_PSD/programma/prenotazioni.h"
#include "/mnt/c/users/giova/OneDrive/Desktop/PROGETTO_PSD/programma/lezione.h"
#include "/mnt/c/users/giova/OneDrive/Desktop/PROGETTO_PSD/programma/file.h"
#include "/mnt/c/users/giova/OneDrive/Desktop/PROGETTO_PSD/programma/gestione_prenotazioni.h"
#include "/mnt/c/users/giova/OneDrive/Desktop/PROGETTO_PSD/programma/gestione_lezioni.h"

// Dichiara le variabili globali come extern 
extern listP* lista_prenotazioni;
extern listL* lezioni;
extern list* abbonati;


// Confronta due file, ritorna 1 se sono identici, 0 altrimenti
int file_equal(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");
    if (!f1 || !f2) {
        if (!f1) printf("Errore apertura file %s\n", file1);
        if (!f2) printf("Errore apertura file %s\n", file2);
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return 0;
    }

    int ch1, ch2;
    do {
        ch1 = fgetc(f1);
        ch2 = fgetc(f2);
        if (ch1 != ch2) {
            fclose(f1);
            fclose(f2);
            return 0;
        }
    } while (ch1 != EOF && ch2 != EOF);

    fclose(f1);
    fclose(f2);
    return ch1 == ch2;
}

void test_aggiungiPrenotazione(const char* input_file, const char* output_file) {
    FILE* fin = fopen(input_file, "r");
    if (!fin) {
        printf("Errore apertura file input %s\n", input_file);
        return;
    }

    int codice_abbonamento = -1;
    int codice_lezione = -1;
    char line[256];

    while (fgets(line, sizeof(line), fin)) {
        if (strncmp(line, "CODICE_ABBONAMENTO=", 19) == 0) {
            codice_abbonamento = atoi(line + 19);
        } else if (strncmp(line, "CODICE_LEZIONE=", 15) == 0) {
            codice_lezione = atoi(line + 15);
        }
    }
    fclose(fin);

    if (codice_abbonamento == -1 || codice_lezione == -1) {
        printf("Codici abbonamento o lezione non trovati nel file input\n");
        return;
    }
    
    // Inizializza le liste globali caricandole da file
    lista_prenotazioni = caricaPrenotazioniDaFile(NULL, "/mnt/c/users/giova/OneDrive/Desktop/PROGETTO_PSD/programma/prenotazioni.txt");
    lezioni = caricaLezioniDaFile(NULL, "/mnt/c/users/giova/OneDrive/Desktop/PROGETTO_PSD/programma/lezioni.txt");
    abbonati = caricaAbbonamentiDaFile(NULL, "/mnt/c/users/giova/OneDrive/Desktop/PROGETTO_PSD/programma/abbonamenti.txt");

    if (!lista_prenotazioni || !lezioni || !abbonati) {
        printf("Errore caricamento dati da file\n");
        return;
    }

    lezione* l = trovaLezionePerID(lezioni, codice_lezione);
    if (!l) {
        printf("Lezione con codice %d non trovata\n", codice_lezione);
        return;
    }

    int disponibilita_iniziale = getPostiMax(l) - getPostiOccupati(l);

    // Chiama la funzione reale con parametri
    aggiungi_prenotazione(codice_abbonamento, codice_lezione);

    // Ricarica la lezione per aggiornare i dati dopo la prenotazione
    l = trovaLezionePerID(lezioni, codice_lezione);
    int disponibilita_finale = getPostiMax(l) - getPostiOccupati(l);

    FILE* fout = fopen(output_file, "w");
    if (!fout) {
        printf("Errore apertura file output %s\n", output_file);
        return;
    }

    if (disponibilita_finale == disponibilita_iniziale - 1) {
        fprintf(fout, "Prenotazione aggiunta con successo\n");
    } else {
        fprintf(fout, "Errore: disponibilità non aggiornata correttamente\n");
    }

    fprintf(fout, "Disponibilità iniziale: %d\n", disponibilita_iniziale);
    fprintf(fout, "Disponibilità finale: %d\n", disponibilita_finale);
    fprintf(fout, "Codice abbonamento usato: %d\n", codice_abbonamento);
    fprintf(fout, "Codice lezione usato: %d\n", codice_lezione);

    fclose(fout);
}

void test_reportDiscipline(const char* file_output) {
    // Carica le liste da file
    listP* lista_prenotazioni = caricaPrenotazioniDaFile(NULL, "input/test3/prenotazioni_test3.txt");
    listL* lista_lezioni = caricaLezioniDaFile(NULL, "input/test3/lezioni_test3.txt");

    if (!lista_prenotazioni || !lista_lezioni) {
        printf("Errore caricamento dati da file\n");
        return;
    }

    FILE* fout = fopen(file_output, "w");
    if (!fout) {
        printf("Errore apertura file output %s\n", file_output);
        // libera memoria se serve
        return;
    }

    // Chiama la funzione che genera il report nel file
    reportDisciplineUltimoMese(lista_prenotazioni, lista_lezioni, fout);

    fclose(fout);

    // Qui puoi liberare memoria delle liste se necessario
}

int main() {
    // Test aggiungiPrenotazione abbonamento scaduto
    const char *input2 = "input/prenotazioni_test2.txt";
    const char *output2 = "output/prenotazioni_test2.txt.out";
    const char *oracle2 = "oracle/prenotazioni_test2.txt.out";

    printf("Eseguo test aggiungiPrenotazione abbonamento scaduto...\n");
    test_aggiungiPrenotazione(input2, output2);

    // Test aggiungiPrenotazione caso valido
    const char *input = "input/prenotazioni_test1.txt";
    const char *output = "output/prenotazioni_test1.txt.out";
    const char *oracle = "oracle/prenotazioni_test1.txt.out";

    printf("Eseguo test aggiungiPrenotazione in caso valido...\n");
    test_aggiungiPrenotazione(input, output);

    if (file_equal(output, oracle)) {
        printf("TEST aggiungiPrenotazione: PASS\n");
    } else {
        printf("TEST aggiungiPrenotazione: FAIL\n");
        printf("Controlla:\n - Output: %s\n - Oracle: %s\n", output, oracle);
    }

    if (file_equal(output2, oracle2)) {
        printf("TEST aggiungiPrenotazione abbonamento scaduto: PASS\n");
    } else {
        printf("TEST aggiungiPrenotazione abbonamento scaduto: FAIL\n");
        printf("Controlla:\n - Output: %s\n - Oracle: %s\n", output2, oracle2);
    }

    // --- Test report discipline ---
    const char *output_report = "output/test3/output_test3.txt.out";
    const char *oracle_report = "oracle/test3/oracle_test3.txt.out";

    printf("Eseguo test report discipline ultimo mese...\n");
    test_reportDiscipline(output_report);

    if (file_equal(output_report, oracle_report)) {
        printf("TEST report discipline: PASS\n");
    } else {
        printf("TEST report discipline: FAIL\n");
        printf("Controlla:\n - Output: %s\n - Oracle: %s\n", output_report, oracle_report);
    }

    return 0;
}