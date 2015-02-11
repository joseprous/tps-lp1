CFLAGS=-Wall `pkg-config --cflags gtk+-2.0`
LIBS=`pkg-config --libs gtk+-2.0` -lexpat

stratego: stratego.o gui.o conf.o jugadas.o ai.o
	gcc -o stratego stratego.o gui.o conf.o jugadas.o ai.o $(LIBS)
 
stratego.o : stratego.c
	gcc $(CFLAGS) -c stratego.c

gui.o: gui.c
	gcc $(CFLAGS) -c gui.c

conf.o : conf.c
	gcc $(CFLAGS) -c conf.c
	
jugadas.o: jugadas.c
	gcc $(CFLAGS) -c jugadas.c

ai.o: ai.c
	gcc $(CFLAGS) -c ai.c

clean :
	rm stratego *.o
