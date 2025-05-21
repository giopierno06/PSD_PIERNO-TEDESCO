palestra: main.o abbonamento.o data.o file.o gestione_Abbonamenti.o gestione_lezioni.o gestione_prenotazioni.o lezione.o list.o list_Lezioni.o list_prenotazioni.o menu.o orario.o prenotazioni.o util.o
	gcc main.o abbonamento.o data.o file.o gestione_Abbonamenti.o gestione_lezioni.o gestione_prenotazioni.o lezione.o list.o list_Lezioni.o list_prenotazioni.o menu.o orario.o prenotazioni.o util.o -o palestra

main.o: main.c
	gcc -c main.c

abbonamento.o: abbonamento.c abbonamento.h
	gcc -c abbonamento.c

data.o: data.c data.h
	gcc -c data.c

file.o: file.c file.h
	gcc -c file.c

gestione_Abbonamenti.o: gestione_Abbonamenti.c gestione_Abbonamenti.h
	gcc -c gestione_Abbonamenti.c

gestione_lezioni.o: gestione_lezioni.c gestione_lezioni.h
	gcc -c gestione_lezioni.c

gestione_prenotazioni.o: gestione_prenotazioni.c gestione_prenotazioni.h
	gcc -c gestione_prenotazioni.c

lezione.o: lezione.c lezione.h
	gcc -c lezione.c

list.o: list.c list.h
	gcc -c list.c

list_Lezioni.o: list_Lezioni.c list_Lezioni.h
	gcc -c list_Lezioni.c

list_prenotazioni.o: list_prenotazioni.c list_prenotazioni.h
	gcc -c list_prenotazioni.c

menu.o: menu.c menu.h
	gcc -c menu.c

orario.o: orario.c orario.h
	gcc -c orario.c

prenotazioni.o: prenotazioni.c prenotazioni.h
	gcc -c prenotazioni.c

util.o: util.c util.h
	gcc -c util.c

clean:
	rm -f *.o palestra
