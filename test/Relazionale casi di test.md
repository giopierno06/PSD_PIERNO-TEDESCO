RAZIONALE DEL CASO DI TESTNome caso di test: prenotazioni_test1
Funzionalità testata: aggiungi_prenotazione(int codice_abbonamento, int codice_lezione)+

1. IntroduzioneQuesto caso di test verifica il comportamento corretto della funzione aggiungi_prenotazione quando vengono forniti in input un codice abbonamento valido e un codice lezione riferito a una lezione disponibile (cioè con posti liberi).

2. Obiettivi del testVerificare che la funzione:
Riconosca correttamente un abbonamento valido (non scaduto e presente nella lista).
Verifichi la disponibilità di posti nella lezione selezionata.
Registri correttamente la prenotazione nel sistema.
Aggiorni correttamente il numero di posti occupati.

3. Criteri di selezione dei testIl test è stato progettato secondo i seguenti criteri:
Classe di equivalenza valida: abbonamento attivo e lezione con disponibilità.
Percorso principale del flusso: si vuole testare il caso "ideale" in cui tutto funziona senza errori.
Verifica dello stato finale del sistema: confronto tra la disponibilità iniziale e finale della lezione.

4. Strategia di testI dati di input (codice abbonamento e codice lezione) vengono letti da un file .in.
La funzione aggiungi_prenotazione viene eseguita in modalità "silenziosa" per i test automatici.
I risultati (output) vengono confrontati con un file .out (oracle).
Il test è automatizzato.

5. Ambiente di testSistema operativo: Linux Ubuntu (sottosistema WSL)
Compilatore: gcc
File coinvolti:
abbonamenti.txt: contiene l'abbonamento con codice 4.
lezioni.txt: contiene la lezione con codice 25.
prenotazioni.txt: inizialmente vuoto o con prenotazioni precedenti.
I file oracle/prenotazioni_test1.txt.out e output/prenotazioni_test1.txt.out vengono usati per la verifica automatica.

6. Risorse necessarieFile di configurazione dei test (.in)
File di abbonamenti, lezioni e prenotazioni coerenti
Accesso in scrittura alla cartella output/
Script di test runner configurato

7. Rischi e mitigazioniRischioMitigazioneIl file abbonamenti.txt non contiene l'abbonamento specificatoVerificare manualmente che il codice 4 sia presente nel fileDifferenze di formattazione tra oracle e outputUsare diff -b o normalizzare le righe prima del confrontoL'ID della lezione è errato o duplicatoVerificare la consistenza del file lezioni.txt prima del test8. Criteri di accettazioneIl file output/prenotazioni_test1.txt.out deve contenere:
Prenotazione aggiunta con successo
Disponibilità iniziale: 20
Disponibilità finale: 19
Codice abbonamento usato: 4
Codice lezione usato: 25

La differenza tra disponibilità iniziale e finale deve essere esattamente 1.
Nessun messaggio di errore deve essere stampato a schermo.
