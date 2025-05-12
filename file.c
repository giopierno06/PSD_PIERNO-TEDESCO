#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "abbonamento.h"
#include "lezione.h"
#include "data.h"


// Carica gli abbonamenti da un file di testo formattato.
// Ogni riga contiene i dati di un abbonamento (codice, nome, cognome, data di inizio e durata).
// Ogni abbonamento viene creato, riempito con i dati letti e inserito in testa alla lista.
list* caricaAbbonamentiDaFile(list* l, const char* nome_file) {
    FILE* file = fopen(nome_file, "r");
    if (file == NULL) {
        printf("Errore nell'aprire il file %s\n", nome_file);
        return l;
    }

    char buffer[50];
    fgets(buffer, sizeof(buffer), file);  // "Abbonamenti:\n"
    fgets(buffer, sizeof(buffer), file);  // "===========================\n"

    int codice_temp, durata_temp, giorno_temp, mese_temp, anno_temp;

    while (fscanf(file, "Codice Abbonamento: %d\n", &codice_temp) == 1) {
        abbonamento* nuovo_abbonamento = creaAbbonamento();
        setCodiceAbbonamento(nuovo_abbonamento, codice_temp);

        fscanf(file, "Nome: %[^\n]\n", buffer);
        setNome(nuovo_abbonamento, buffer);

        fscanf(file, "Cognome: %[^\n]\n", buffer);
        setCognome(nuovo_abbonamento, buffer);

        fscanf(file, "Data di inizio: %d-%d-%d\n", &giorno_temp, &mese_temp, &anno_temp);
        Data* data_temp = newData();
        if (data_temp != NULL) {
            setGiorno(data_temp, giorno_temp);
            setMese(data_temp, mese_temp);
            setAnno(data_temp, anno_temp);
            setDataInizio(nuovo_abbonamento, data_temp);
        } else {
            printf("Errore nell'allocazione della memoria per la data.\n");
            fclose(file);
            return l;
        }

        fscanf(file, "Durata (in mesi): %d\n", &durata_temp);
        setDurata(nuovo_abbonamento, durata_temp);

        fgets(buffer, sizeof(buffer), file);  // separatore

        l = consList(nuovo_abbonamento, l);
    }

    fclose(file);
    return l;
}


// Salva su file gli abbonamenti presenti nella lista.
// Ogni campo è scritto in modo leggibile e compatibile con la funzione di caricamento.
void salvaAbbonamentiSuFile(list* l, const char* nome_file) {
    FILE* file = fopen(nome_file, "w");
    if (file == NULL) {
        printf("Errore nell'aprire il file %s\n", nome_file);
        return;
    }

    fprintf(file, "Abbonamenti:\n");
    fprintf(file, "===========================\n");

    while (l != NULL) {
        const abbonamento* a = getValue(l);
        int codice = getCodiceAbbonamento(a);
        const char* nome = getNome(a);
        const char* cognome = getCognome(a);
        Data* d = getDataInizio(a);
        int durata = getDurata(a);

        fprintf(file, "Codice Abbonamento: %d\n", codice);
        fprintf(file, "Nome: %s\n", nome);
        fprintf(file, "Cognome: %s\n", cognome);
        fprintf(file, "Data di inizio: %02d-%02d-%04d\n", getGiorno(d), getMese(d), getAnno(d));
        fprintf(file, "Durata (in mesi): %d\n", durata);
        fprintf(file, "---------------------------\n");

        l = getNext(l);
    }

    fclose(file);
    printf("Abbonamenti salvati con successo nel file %s\n", nome_file);
}



// Carica le lezioni da un file strutturato contenente i dettagli di ogni lezione.
// Ogni lezione contiene codice, disciplina, nome, numero posti, data, orario e durata.
// Ogni voce viene convertita in una struttura lezione e inserita nella lista.
listL* caricaLezioniiDaFile(listL* l, const char* nome_file) {
    FILE* file = fopen(nome_file, "r");
    if (file == NULL) {
        printf("Errore nell'aprire il file %s\n", nome_file);
        return l;
    }

    char buffer[50];
    fgets(buffer, sizeof(buffer), file);  // "Lezioni:\n"
    fgets(buffer, sizeof(buffer), file);  // "===========================\n"

    int codice_temp, posti_occupati_temp;
    short int postimax_temp, durata_temp;
    int giorno_temp, mese_temp, anno_temp;
    int ora_temp, minuto_temp;

    while (fscanf(file, "Codice Lezione: %d\n", &codice_temp) == 1) {
        lezione* nuova_lezione = creaLezione();
        if (nuova_lezione == NULL) {
            printf("Errore nell'allocazione della lezione.\n");
            fclose(file);
            return l;
        }

        setCodiceLezione(nuova_lezione, codice_temp);

        fscanf(file, "Disciplina: %[^\n]\n", buffer);
        setDisciplinaLezione(nuova_lezione, buffer);

        fscanf(file, "Nome: %[^\n]\n", buffer);
        setNomeLezione(nuova_lezione, buffer);

        fscanf(file, "Posti massimi: %hd\n", &postimax_temp);
        setPostiMax(nuova_lezione, postimax_temp);

        fscanf(file, "Posti occupati: %d\n", &posti_occupati_temp);
        for (int i = 0; i < posti_occupati_temp; i++) {
            addPostiOccupati(nuova_lezione);
        }

        fscanf(file, "Data: %d-%d-%d\n", &giorno_temp, &mese_temp, &anno_temp);
        Data* data_temp = newData();
        if (data_temp == NULL) {
            printf("Errore nell'allocazione della data.\n");
            fclose(file);
            return l;
        }
        setGiorno(data_temp, giorno_temp);
        setMese(data_temp, mese_temp);
        setAnno(data_temp, anno_temp);
        setDataLezione(nuova_lezione, data_temp);

        fscanf(file, "Ora di inizio: %d:%d\n", &ora_temp, &minuto_temp);
        Orario* orario_temp = newOrario();
        setOra(orario_temp, ora_temp);
        setMinuto(orario_temp, minuto_temp);
        setOrarioInizio(nuova_lezione, orario_temp);

        fscanf(file, "Durata (in minuti): %hd\n", &durata_temp);
        setDurataLezione(nuova_lezione, durata_temp);

        fgets(buffer, sizeof(buffer), file);  // separatore

        l = lezione_consList(nuova_lezione, l);
    }

    fclose(file);
    return l;
}


// Salva tutte le lezioni su un file di testo in formato leggibile.
// Il formato del file è compatibile con la funzione di caricamento per permettere il ripristino completo.
void salvaLezioniSuFile(listL* l, const char* nome_file) {
    FILE* file = fopen(nome_file, "w");
    if (file == NULL) {
        printf("Errore nell'aprire il file %s\n", nome_file);
        return;
    }

    fprintf(file, "Lezioni:\n");
    fprintf(file, "===========================\n");

    while (l != NULL) {
        const lezione* le = lezione_getValue(l);

        if (le == NULL) {
            l = lezione_getNext(l);
            continue;
        }

        int codice = getCodiceLezione(le);
        const char* disciplina = getDisciplinaLezione(le);
        const char* nome = getNomeLezione(le);
        short int postimax = getPostiMax(le);
        int posti_occupati = getPostiOccupati(le);
        Data* data = getDataLezione(le);
        Orario* orario = getOrarioLezione(le);
        short int durata = getDurataLezione(le);

        // Verifica se i puntatori sono NULL e imposta valori di fallback
        fprintf(file, "Codice Lezione: %d\n", codice);
        fprintf(file, "Disciplina: %s\n", disciplina ? disciplina : "(N/D)");
        fprintf(file, "Nome: %s\n", nome ? nome : "(N/D)");
        fprintf(file, "Posti massimi: %d\n", postimax);
        fprintf(file, "Posti occupati: %d\n", posti_occupati);

        // Verifica Data
        if (data != NULL) {
            fprintf(file, "Data: %02d-%02d-%04d\n", getGiorno(data), getMese(data), getAnno(data));
        } else {
            fprintf(file, "Data: (N/D)\n");
        }

        // Verifica Orario
        if (orario != NULL) {
            fprintf(file, "Ora di inizio: %02d:%02d\n", getOra(orario), getMinuto(orario));
        } else {
            fprintf(file, "Ora di inizio: (N/D)\n");
        }

        // Durata della lezione
        fprintf(file, "Durata (in minuti): %d\n", durata);
        fprintf(file, "---------------------------\n");

        l = lezione_getNext(l);
    }

    fclose(file);
    printf("Lezioni salvate con successo nel file %s\n", nome_file);
}