//CREADO POR JOSE PROUS(50139)
#include <expat.h>
#include "stratego.h"
#define BUFFSIZE        8192
static char Buff[BUFFSIZE];
static int Depth;
static char estilo[100],tiempo[100],bombas[100],soldados[100],jugador1[100],jugador2[100];
static char et[100];
static int empieza;

static void XMLCALL start(void *data, const char *el, const char **attr)
{
	int i;
	if(!strcmp(el,"jugador")){
		if(!strlen(jugador1)){
			for (i = 0; attr[i]; i += 2) {
				if(!strcmp(attr[i],"empieza"))empieza=0;
				if(!strcmp(attr[i],"nombre"))strcpy(jugador1,attr[i+1]);	
			}			
		}else{
			for (i = 0; attr[i]; i += 2) {
				if(!strcmp(attr[i],"empieza"))empieza=1;
				if(!strcmp(attr[i],"nombre"))strcpy(jugador2,attr[i+1]);	
			}					
		}
	}
	strcpy(et,el);
	Depth++;
}

static void XMLCALL end(void *data, const char *el)
{
	Depth--;
}
static void XMLCALL charhndl(void *userData,
                                    const XML_Char *s,
                                    int len)
{
	int i,c=0;
	char s1[100];
		for(i=0;i<len;i++){
			if(s[i]!='\t' && s[i]!='\n' && s[i]!=' '){
				s1[c]=s[i];
				c++;
			}
		}
		s1[c]=0;
	if(!strcmp(et,"tiempo")&&!strlen(tiempo))strcpy(tiempo,s1);
	if(!strcmp(et,"estilo")&&!strlen(estilo))strcpy(estilo,s1);
	if(!strcmp(et,"sacarBombasCuandoExplotan")&&!strlen(bombas))strcpy(bombas,s1);
	if(!strcmp(et,"soldadosMuchosPasos")&&!strlen(soldados))strcpy(soldados,s1);		
}
struct config cargar_conf(char *ruta)
{
	FILE *fichero;
	struct config conf;
	char dir[200];
	strcpy(jugador1,"");
	strcpy(tiempo,"");
	strcpy(estilo,"");
	strcpy(bombas,"");
	strcpy(soldados,"");	
	sprintf(dir,"%sconfig.xml",ruta);
	if(!(fichero=fopen(dir,"r"))){
		conf.estilo=-1;
		return conf;
	}	
	XML_Parser p = XML_ParserCreate(NULL);
	if (!p) {
		fprintf(stderr, "Couldn't allocate memory for parser\n");
		exit(-1);
	}

	XML_SetElementHandler(p, start, end);
	XML_SetCharacterDataHandler(p,charhndl);
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
	if(!strcmp(estilo,"americano"))
		conf.estilo=0;
	else
		conf.estilo=1;		
	if(!strcmp(soldados,"si"))
		conf.soldados=1;
	else
		conf.soldados=0;		
	if(!strcmp(bombas,"si"))
		conf.bombas=1;
	else
		conf.bombas=0;		
	strcpy(conf.jugador[0],jugador1);
	strcpy(conf.jugador[1],jugador2);
	conf.empieza=empieza;
	sscanf(tiempo,"%d",&conf.tiempo);	
	return conf;	
}
