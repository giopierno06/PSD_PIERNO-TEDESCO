## Casi di Test Implementati

### 1. Prenotazione valida
- **Input:** Codice abbonamento valido, codice lezione disponibile.
- **Output atteso:** Prenotazione aggiunta con successo.
- **Risultato:** PASS
- può essere effettuato solo una volta, dal secondo caso in poi si verrà restituito fail poiche la prenotazione già esiste 
  quindi può "essere considerato" un test sulle prenotazioni duplicate

### 2. Prenotazione con abbonamento scaduto
- **Input:** Codice abbonamento scaduto, codice lezione disponibile.
- **Output atteso:** Errore: Abbonamento scaduto alla data scelta.
- **Risultato:** PASS

### 3. Report discipline più frequentate nell'ultimo mese
* **Input:** File prenotazioni e lezioni con dati delle ultime 4 settimane.
* **Output atteso:** Report stampato con elenco discipline ordinate per frequenza, con numero corretto di prenotazioni per ciascuna.
* **Risultato:** PASS



