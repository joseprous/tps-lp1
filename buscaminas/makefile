all: buscaminas
buscaminas: buscaminas.o buscaminas.res
	gcc -o buscaminas buscaminas.o buscaminas.res -mwindows
 
buscaminas.o : buscaminas.c
	gcc -c buscaminas.c -mwindows

buscaminas.res:buscaminas.rc
	windres -i buscaminas.rc --input-format=rc -o buscaminas.res -O coff
clean:
	rm *.o buscaminas.res buscaminas.exe
