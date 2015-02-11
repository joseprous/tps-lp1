//CREADO POR JOSE PROUS(50139)
#include <expat.h>
#include "stratego.h"
#define BUFFSIZE        8192
static char Buff[BUFFSIZE];
static int Depth;
static char numero[100],jugador[100],posinicial[100],posfinal[100],atacante[100],atacado[100];
static char ganador[100];
/*
	devuelve el valor numerico del rango
*/
int rango(char c)
{
	switch(c){
		case 'F':return 13;
		case 'B':return 12;
		case 'E':return 11;
		case 'M':return 10;
		case 'G':return 9;
		case 'C':return 8;
		case 'T':return 7;
		case 'Y':return 6;
		case 'N':return 5;		
		case 'O':return 4;
		case 'A':return 3;
		case 'S':return 2;
		default:return -1;
	}	
}
/*
	devuelve el caracter correspondiente al rango
*/
char simbolo(int n)
{
	switch(n){
		case 13:return 'F';
		case 12:return 'B';
		case 11:return 'E';
		case 10:return 'M';
		case 9:return 'G';
		case 8:return 'C';
		case 7:return 'T';
		case 6:return 'Y';
		case 5:return 'N';		
		case 4:return 'O';
		case 3:return 'A';
		case 2:return 'S';
		default:
		printf("%d",n);
		return ' ';
	}		
}
static void XMLCALL start(void *data, const char *el, const char **attr)
{
	int i;
	if(!strcmp(el,"jugada")){
		for (i = 0; attr[i]; i += 2){
			if(!strcmp(attr[i],"numero"))strcpy(numero,attr[i+1]);
			if(!strcmp(attr[i],"jugador"))strcpy(jugador,attr[i+1]);							
		}
	}
	if(!strcmp(el,"movimiento")){
		for (i = 0; attr[i]; i += 2){
			if(!strcmp(attr[i],"posInicial"))strcpy(posinicial,attr[i+1]);
			if(!strcmp(attr[i],"posFinal"))strcpy(posfinal,attr[i+1]);							
		}
	}
	if(!strcmp(el,"ataque")){
		for (i = 0; attr[i]; i += 2){
			if(!strcmp(attr[i],"simboloAtacante"))strcpy(atacante,attr[i+1]);
		}
	}
	if(!strcmp(el,"resultado")){
		for (i = 0; attr[i]; i += 2){
			if(!strcmp(attr[i],"simboloAtacado"))strcpy(atacado,attr[i+1]);
			if(!strcmp(attr[i],"ganador"))strcpy(ganador,attr[i+1]);							
		}
	}
	Depth++;
}

static void XMLCALL end(void *data, const char *el)
{
	Depth--;
}
/*
	lee la jugada del archivo XML
*/
struct jugada leer_jugada(char *ruta,char *nombre)
{
	FILE *fichero;
	struct jugada jug;
	char dir[200];
	strcpy(numero,"");
	strcpy(jugador,"");
	strcpy(posinicial,"");
	strcpy(posfinal,"");
	strcpy(atacante,"");
	strcpy(atacado,"");
	strcpy(ganador,"");
	sprintf(dir,"%s%s.xml",ruta,nombre);
	if(!(fichero=fopen(dir,"r"))){
		jug.num=-1;
		return jug;
	}	
	XML_Parser p = XML_ParserCreate(NULL);
	if (!p) {
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		exit(-1);
	}

	XML_SetElementHandler(p, start, end);
	for (;;) {
		int done;
		int len;
		len = fread(Buff, 1, BUFFSIZE, fichero);
		if (ferror(fichero)) {
			fprintf(stderr, "Read error\n");
			exit(-1);
		}
		done = feof(fichero);

		if (XML_Parse(p, Buff, len, done) == XML_STATUS_ERROR) {
			fprintf(stderr, "Parse error at line %d:\n%s\n",
				XML_GetCurrentLineNumber(p),
				XML_ErrorString(XML_GetErrorCode(p)));
			exit(-1);
		}

		if (done)
			break;
	}
	fclose(fichero);
	sscanf(numero,"%d",&jug.num);
	strcpy(jug.jugador,jugador);
	jug.posini[0]=posinicial[0]-'A';
	sscanf(posinicial+1,"%d",&jug.posini[1]);
	jug.posini[1]--;
	jug.posfin[0]=posfinal[0]-'A';
	sscanf(posfinal+1,"%d",&jug.posfin[1]);
	jug.posfin[1]--;
	jug.atacante=rango(atacante[0]);
	jug.atacado=rango(atacado[0]);
	if(!(strcmp(ganador,"atacante")))jug.ganador=0;
	if(!(strcmp(ganador,"atacado")))jug.ganador=1;
	if(!(strcmp(ganador,"ambos")))jug.ganador=2;
	return jug;	
}
/*
	escribe la jugada en el archivo XML
*/
void escribir_jugada(struct jugada jug,char *ruta,int tablero[10][10])
{
	FILE *fichero;
	char dir[200];
#if DEBUG
//imprime las jugadas en un archivo
{
	FILE *log;
	char s[200];
	int i,j;
	sprintf(s,"%slog",ruta);
	log=fopen(s,"a");
	fprintf(log,"%d\n",jug.num);	
	fprintf(log,"J:%s\t",jug.jugador);
	fprintf(log,"I=\"%c%d\"\tF=\"%c%d\"\t",jug.posini[0]+'A',jug.posini[1]+1,jug.posfin[0]+'A',jug.posfin[1]+1);
	fprintf(log,"atacante:%d\tatacado:%d\tg:%d\n",jug.atacante,jug.atacado,jug.ganador);
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			fprintf(log,"%3d ",tablero[i][j]);	
		}			
		fprintf(log,"\n");		
	}
	fprintf(log,"\n");
	fclose(log);
}
#endif
	sprintf(dir,"%s%s.xml",ruta,jug.jugador);
	fichero=fopen(dir,"w");
	fprintf(fichero,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(fichero,"<jugada numero=\"%d\" jugador=\"%s\">\n",jug.num,jug.jugador);
	if(jug.posini[0]!=-1)
		fprintf(fichero,"\t<movimiento posInicial=\"%c%d\" posFinal=\"%c%d\"/>\n",jug.posini[0]+'A',jug.posini[1]+1,jug.posfin[0]+'A',jug.posfin[1]+1);
	if(jug.atacante)
		fprintf(fichero,"\t<ataque simboloAtacante=\"%c\"/>\n",simbolo(jug.atacante));
	if(jug.atacado){
		switch(jug.ganador){
			case 0:
				fprintf(fichero,"\t<resultado simboloAtacado=\"%c\" ganador=\"atacante\"/>\n",simbolo(jug.atacado));
				break;
			case 1:
				fprintf(fichero,"\t<resultado simboloAtacado=\"%c\" ganador=\"atacado\"/>\n",simbolo(jug.atacado));
				break;
			case 2:
				fprintf(fichero,"\t<resultado simboloAtacado=\"%c\" ganador=\"ambos\"/>\n",simbolo(jug.atacado));
				break;			
		}
	}
	fprintf(fichero,"</jugada>\n");
	fclose(fichero);
}
