//CREADO POR JOSE PROUS(50139)
#include <getopt.h>
#include "stratego.h"

//de gui.c
extern GtkWidget *status_bar;//barra de estado

//de ai.c
extern int tablero_ai[10][10];
extern int p_ai[12][2];
extern int t_ai;
extern int atacado_ai;
extern int ganador_ai;
extern int li_ai,ls_ai;
extern int posini_ai[2];
extern int posfin_ai[2];
extern int turno_ai;
extern int fin_ai;
extern int to_ai,to_t_ai;
extern int num_ai;
extern int uposai[5][2];

struct config conf;//guarda las opciones de configuracion
int F=13,B=12,E=11,M=10,G=9,C=8,T=7,Y=6,N=5,O=4,A=3,S=2,V=0;//rangos
int prep;//para saber si el humano esta cargando sus fichas
int tipo=0;//0:h vs ai 1:ai vs ai 2:h vs h
char ruta[200];//ruta completa a la carpeta donde esta el ejecutable
char *ruta_c=NULL,*ruta_j=NULL;//rutas de los archivos de configuracion y jugadas
char nom_ai[100]="",nom_h[100]="";//nombres para identificar el programa
char *nombre;//nombre del ejecutable(con la ruta completa)

int to_h,to_t_h;//controlar los timers
int mov;//para saber si el humano selecciono una ficha o intenta moverla
int fin_h;//para saber si se termino el juego
int turno_h;//1 si es el turno del humano

//registro de las posiciones anteriores para saber que hacer cuando 
//hay un resultado de un ataque anterior
int posini_h[2];
int posfin_h[2];

int atacado_h;
int ganador_h;
int t_h;//tiempos
int p_h[12][2];//puntajes
int tablero_h[10][10];
int li_h,ls_h;//para saber que numeros le corresponden a quien
int num_h;//numero de jugada
int uposh[5][2];//guarda las ultimas posiciones para saber si hay 5 repeticiones

int ai;//0 si empieza la computadora

int m=0;//para mostrar las fichas durante un tiempo
int desc=1;//dejar descubiertas o no

void limpiartablero(int tablero[10][10]);
void uso(FILE* stream, int exit_code);

int main( int   argc,char **argv)
{
	int i,l=strlen(argv[0]);
	int opcion; 
	/* opciones cortas */ 
	const char* const short_options =  "hu:a:c:j:"; 
	/* opciones largas */ 
	const struct option long_options[] = { 
		{"help" , 0, NULL,  'h'}, 
		{"humano" , 1, NULL,  'u'},
		{"ai" , 1, NULL,  'a'},		
		{"conf" , 1, NULL,  'c'},		
		{"jugadas" , 1, NULL,  'j'},		
		{ NULL, 0, NULL, 0 }
	};
	srand(time(NULL));	
	//carga la ruta actual
	strcpy(ruta,argv[0]);	
	for(i=l-1;argv[0][i]!='/';i--){
		ruta[i]=0;
	}
	/*Procesa los parametros pasados por linea de comandos*/
	nombre = argv[0];
	do { 
		opcion = getopt_long (argc, argv, short_options, long_options, NULL); 
		switch (opcion) { 
		case 'h' :
			uso (stdout, 0); 
		case 'u' : 
			strcpy(nom_h,optarg); 
			break; 
		case 'a' : 
			strcpy(nom_ai,optarg); 
			break;
		case 'c' : 
			ruta_c = optarg; 
			break;
		case 'j' : 
			ruta_j = optarg; 
			break; 		
		case '?' : /*opcion invalida*/
			uso (stderr, 1); 
		case -1: /* no hay mas opciones */ 
			break; 
		default:
			abort (); 
		} 
	} while (opcion != -1);
	if(!strlen(nom_h))strcpy(nom_h,NOM_H);
	if(!strlen(nom_ai))strcpy(nom_ai,NOM_AI);
	if(ruta_c==NULL)ruta_c=ruta;
	if(ruta_j==NULL)ruta_j=ruta;
		
	//inicia gtk
	gtk_init (&argc, &argv);
	
	crear_objetos();
	limpiartablero(tablero_h);
	dibujartablero(0,0,0,tablero_h);
	
	//loop principal
	gtk_main ();
	return 0;
}

/*
	imprime el nombre del programa y los parametros
*/
void uso(FILE* stream, int exit_code) 
{ 
	fprintf (stream,  "uso: %s opciones\n",nombre); 
	fprintf (stream,  "-a, --ai nombre      Nombre por el cual sera identificada la computadora.\n");
	fprintf (stream,  "-c, --conf ruta      Ruta de donde buscara el archivo de configuracion.\n");
	fprintf (stream,  "                     (Debe terminar con /)\n");	
	fprintf (stream,  "-h, --help           Imprime esta informacion.\n");
	fprintf (stream,  "-j, --jugadas ruta   Ruta de donde buscara los archivos de jugadas.\n");
	fprintf (stream,  "                     (Debe terminar con /)\n");	
	fprintf (stream,  "-u, --humano nombre  Nombre por el cual sera identificado el humano.\n");
	exit (exit_code); 
}

/*
	deja solo los lagos
*/
void limpiartablero(int tablero[10][10])
{
    int i,j;
    for(i=0;i<10;i++)
	for(j=0;j<10;j++)
	    	tablero[i][j]=0;
    for(i=4;i<6;i++)
	for(j=2;j<4;j++){
	    tablero[i][j]=-1;
	    tablero[i][j+4]=-1;			
	}
}

#if DEBUG
//carga las fichas del humano al azar
void _cargar_ai(int n,int tablero[10][10])
{
	int i,j,v,b;
	int cant[14]={0};	
	for(i=0+!n*6;i<4+!n*6;i++)
		for(j=0;j<10;j++){
			b=1;
			do{
				v=rand()%12+2;
				switch(v){
					case 13:
					case 11:	
					case 10:
					case 9:
						if(cant[v]<1){
							b=0;
							cant[v]++;
						}
						break;
					case 12:
						if(cant[v]<6){
							b=0;
							cant[v]++;
						}
						break;
					case 8:
						if(cant[v]<2){
							b=0;
							cant[v]++;
						}
						break;
					case 7:
						if(cant[v]<3){
							b=0;
							cant[v]++;
						}
						break;
					case 6:
					case 5:
					case 4:
						if(cant[v]<4){
							b=0;
							cant[v]++;
						}
						break;
					case 3:
						if(cant[v]<5){
							b=0;
							cant[v]++;
						}
						break;
					case 2:
						if(cant[v]<8){
							b=0;
							cant[v]++;
						}
						break;
				}
			}while(b);
			tablero[i][j]=v+n*100;			
		}
}
#endif

/*
	devuelve el ganador en el encuentro
	0:atacante
	1:atacado
	2:empate
*/
int lucha(int r1,int r2)
{
	if(r2==F){
		return 0;
	}
	if(r1==r2){
		if(!conf.estilo){
			return 0;
		}else{
			return 2;
		}
	}	
	if(r1==E){
		if(r2==M||!r2){
			return 0;
		}else{
			return 1;		
		}
	}else{
		if(r2==B){
			if(r1==A){
				return 0;
			}else{
				if(conf.bombas)
					return 2;
				else
					return 1;
			}
		}else{
			if((r2==E)||(r1>r2)){
				return 0;
			}else{
				return 1;					
			}
		}
	}
}
/*
	elimina las marcas dejadas al buscar una ruta para los soldados
*/
void limpiar(int tablero[10][10])
{
	int i,j;
	for(i=0;i<10;i++)
		for(j=0;j<10;j++)
			if(tablero[i][j]==-4)tablero[i][j]=0;
}

/*
	devuelve 1 si un soldado puede llegar al destino
*/
int recorrer(int tablero[][10],int fo,int co,int fd,int cd)
{
	struct pos{
		char f,c,n;
	};
	int gr=tablero[fo][co];
	int f=fo,c=co;
	int m=10,n=10;
	int p=0;
	struct pos r[100];
	r[0].f=f;
	r[0].c=c;
	r[0].n=0;
	for(;;){
		tablero[f][c]=-4;
		if(f<(m-1)&&(tablero[f+1][c]==0||(f+1==fd&&c==cd)))r[p].n++;
		if(f&&(tablero[f-1][c]==0||(f-1==fd&&c==cd)))r[p].n++;
		if(c<(n-1)&&(tablero[f][c+1]==0||(f==fd&&c+1==cd)))r[p].n++;
		if(c&&(tablero[f][c-1]==0||(f==fd&&c-1==cd)))r[p].n++;
		if(r[p].n){
			r[p].n--;
			if(f<(m-1)&&tablero[f+1][c]==0)
				f++;
			else if(f&&tablero[f-1][c]==0)(f)--;
			else if(c<(n-1)&&tablero[f][c+1]==0)(c)++;
			else if(c&&tablero[f][c-1]==0)(c)--;
			p++;
			r[p].f=f;
			r[p].c=c;
			r[p].n=0;
			if(f<(m-1)&&(f+1==fd&&c==cd)){
				f++;
				limpiar(tablero);
				tablero[fo][co]=gr;
				return 1;
			}
			if(f&&(f-1==fd&&c==cd)){
				f--;
				limpiar(tablero);
				tablero[fo][co]=gr;
				return 1;
			}
			if(c<(n-1)&&(f==fd&&c+1==cd)){
				c++;
				limpiar(tablero);
				tablero[fo][co]=gr;
				return 1;
			}
			if(c&&(f==fd&&c-1==cd)){
				c--;
				limpiar(tablero);
				tablero[fo][co]=gr;
				return 1;
			}				
		}else{
			if(p){
				p--;
				f=r[p].f;
				c=r[p].c;
			}else{
				limpiar(tablero);
				tablero[fo][co]=gr;
				return 0;
			}
		}
	}
}

/*
	intenta mover una pieza en la posición especificada
	q:0 humano;1 computadora
	si pudo mover escribe la jugada en el archivo XML
	devulve 1 si pudo mover y 0 en caso contrario
*/
int mover(int fo,int co,int fd,int cd,int q,int atacado,int ganador,int tablero[10][10])
{
	struct jugada jug;
	int s=0;

	//comprueba si la computadora repitio 5 veces el mismo movimiento
	if(q&&fo==uposai[1][0]&&co==uposai[1][1]){
		int b=0,i;
		for(i=0;i<2;i++){
			if(uposai[1][i]!=uposai[3][i]||uposai[0][i]!=uposai[2][i])b=1;
		}
		if(!b){
			return 0;
		}
	}
	
	if((q&&ai)||(!q&&!ai)){
		s=100;
	}
	strcpy(jug.jugador,conf.jugador[q]);
	if(q){
		jug.num=num_ai;
	}else{
		jug.num=num_h;
	}
	jug.posini[0]=fo;
	jug.posini[1]=co;
	jug.posfin[0]=fd;
	jug.posfin[1]=cd;
	jug.atacado=atacado;
	jug.ganador=ganador;
	if((q&&tablero[fo][co]>li_ai&&tablero[fo][co]<ls_ai)||(!q&&tablero[fo][co]>li_h&&tablero[fo][co]<ls_h)){
		if((tablero[fo][co]!=B+s)&&(tablero[fo][co]!=F+s)){
			if(((fd==fo+1||fd==fo-1)&&(cd==co))||((cd==co+1||cd==co-1)&&(fd==fo))||(conf.soldados&&(tablero[fo][co]==S||tablero[fo][co]-100==S)&&recorrer(tablero,fo,co,fd,cd))){
				if(!tablero[fd][cd]){
					if(q){
						posini_ai[0]=fo;
						posini_ai[1]=co;
						posfin_ai[0]=fd;
						posfin_ai[1]=cd;											
					}else{
						posini_h[0]=fo;
						posini_h[1]=co;
						posfin_h[0]=fd;
						posfin_h[1]=cd;																	
					}
					tablero[fd][cd]=tablero[fo][co];
					tablero[fo][co]=0;
					jug.atacante=0;
					escribir_jugada(jug,ruta_j,tablero);
					return 1;
				}
				if(q){
					if(tablero[fd][cd]<ls_h&&tablero[fd][cd]>=li_h){
						posini_ai[0]=fo;
						posini_ai[1]=co;
						posfin_ai[0]=fd;
						posfin_ai[1]=cd;											
						jug.atacante=tablero[fo][co]-s;
						escribir_jugada(jug,ruta_j,tablero);
						return 1;
					}						
				}else{
					if(tablero[fd][cd]>=li_ai&&tablero[fd][cd]<ls_ai){
						posini_h[0]=fo;
						posini_h[1]=co;
						posfin_h[0]=fd;
						posfin_h[1]=cd;																	
						jug.atacante=tablero[fo][co]-s;
						escribir_jugada(jug,ruta_j,tablero);						
						return 1;
					}
				}
			}
		}
	}
	return 0;	
}
/*
	terminar el juego para el humano
*/
void fin(int g)
{
	fin_h=1;
	gtk_timeout_remove(to_t_h);
	to_t_h=0;	
	resumen(p_h,g);			
}

/*
	lee y procesa la jugada del contrincante
*/
int esperar_mov_h(void *data)
{
	static struct jugada jug;
	char s[200];
	if(!m)jug=leer_jugada(ruta_j,conf.jugador[1]);
	if(jug.num==-1)
		return 1;
	else{	
		sprintf(s,"%s%s.xml",ruta,conf.jugador[1]);
		remove(s);
		t_h=time(NULL);//esperar_mov_h
		if(!to_t_h){
			to_t_h=gtk_timeout_add(1000,tiempo_h,NULL);			
		}
		//hay un resultado del ataque anterior
		if((jug.atacado!=-1)&&(m!=2)){
			if(!m){
				m=1;
				tablero_h[posfin_h[0]][posfin_h[1]]=jug.atacado+ai*100;
				dibujartablero(posfin_h[0],posfin_h[1],2,tablero_h);
				return 1;
			}
			m=0;
			//si ganó el atacante 
			if(!jug.ganador){
				p_h[jug.atacado-2][0]++;
				tablero_h[posfin_h[0]][posfin_h[1]]=tablero_h[posini_h[0]][posini_h[1]];
				tablero_h[posini_h[0]][posini_h[1]]=0;
				if(jug.atacado==F){//gano el humano
					fin(0);
					return 0;					
				}												
			}else{
				//si ganó el atacado
				if(jug.ganador==1){
					p_h[tablero_h[posini_h[0]][posini_h[1]]-!ai*100-2][1]++;
					if(conf.estilo||tablero_h[posini_h[0]][posini_h[1]]-!ai*100==S||jug.atacado==B){
						tablero_h[posfin_h[0]][posfin_h[1]]=jug.atacado+ai*100;
						tablero_h[posini_h[0]][posini_h[1]]=0;					
					}else{
						tablero_h[posini_h[0]][posini_h[1]]=jug.atacado+ai*100;						
						tablero_h[posfin_h[0]][posfin_h[1]]=0;										
					}
				
				}
				//si fué empate
				if(jug.ganador==2){
					tablero_h[posfin_h[0]][posfin_h[1]]=0;
					tablero_h[posini_h[0]][posini_h[1]]=0;
				}					
			}
		}
		//si el contrincante no atacó
		if(jug.atacante==-1){		
			tablero_h[jug.posfin[0]][jug.posfin[1]]=tablero_h[jug.posini[0]][jug.posini[1]];
			tablero_h[jug.posini[0]][jug.posini[1]]=0;
			atacado_h=0;
			ganador_h=0;			
		}else{
			if(!m){
				m=2;
				tablero_h[jug.posini[0]][jug.posini[1]]=jug.atacante+ai*100;
				dibujartablero(jug.posini[0],jug.posini[1],2,tablero_h);
				return 1;
			}
			m=0;			
			atacado_h=tablero_h[jug.posfin[0]][jug.posfin[1]]-!ai*100;			
			ganador_h=lucha(jug.atacante,tablero_h[jug.posfin[0]][jug.posfin[1]]-!ai*100);
			switch(ganador_h){
				case 0://ganó el atacante
					p_h[atacado_h-2][1]++;
					tablero_h[jug.posfin[0]][jug.posfin[1]]=jug.atacante+ai*100;
					tablero_h[jug.posini[0]][jug.posini[1]]=0;
					break;
				case 1://ganó el atacado
					p_h[jug.atacante-2][0]++;
					if(conf.estilo||jug.atacante==S||atacado_h==B){
						tablero_h[jug.posini[0]][jug.posini[1]]=0;						
					}else{
						tablero_h[jug.posini[0]][jug.posini[1]]=tablero_h[jug.posfin[0]][jug.posfin[1]];
						tablero_h[jug.posfin[0]][jug.posfin[1]]=0;
					}
					break;
				case 2://empate
					tablero_h[jug.posfin[0]][jug.posfin[1]]=0;
					tablero_h[jug.posini[0]][jug.posini[1]]=0;
					break;					
			}
		}
#if DEBUG
{
		FILE *log;
		char s[200];
		int i,j;
		sprintf(s,"%slog",ruta);
		log=fopen(s,"a");
		fprintf(log,"J:%s\t",jug.jugador);
		fprintf(log,"I=\"%c%d\"\tF=\"%c%d\"\t",jug.posini[0]+'A',jug.posini[1]+1,jug.posfin[0]+'A',jug.posfin[1]+1);
		fprintf(log,"atacante:%d\tatacado:%d\tg:%d\n",jug.atacante,jug.atacado,jug.ganador);
		fprintf(log,"posini:%d %d\tposfin:%d %d\n",posini_h[0],posini_h[1],posfin_h[0],posfin_h[1]);
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				fprintf(log,"%3d ",tablero_h[i][j]);	
			}			
			fprintf(log,"\n");		
		}
		fclose(log);		
}	
#endif
		num_h=jug.num+1;
		turno_h=1;
		if(tipo==2||tipo==0)cturno(1);
		dibujartablero(0,0,0,tablero_h);
		if(atacado_h==F){//gano la computadora
			struct jugada jug;
			strcpy(jug.jugador,conf.jugador[0]);
			jug.num=num_h;
			jug.posini[0]=-1;
			jug.atacante=0;
			jug.atacado=F;
			jug.ganador=0;
			escribir_jugada(jug,ruta_j,tablero_h);		
			fin(1);	
		}
		return 0;		
	}	
}
/*
	limpia el tablero dejando los colores de los ejercitos
*/
void inicializartablero(int n,int tablero[10][10])
{
    int i,j;
    for(i=0;i<4;i++)
	for(j=0;j<10;j++)
		if(!n||n==2)
	    		tablero[i][j]=101;
		else
	    		tablero[i][j]=0;			
    for(i=6;i<10;i++)
	for(j=0;j<10;j++)
		if(n)
	    		tablero[i][j]=1;
		else
	    		tablero[i][j]=0;			
    for(i=4;i<6;i++)
	for(j=0;j<10;j++)
	    tablero[i][j]=0;
    for(i=4;i<6;i++)
	for(j=2;j<4;j++){
	    tablero[i][j]=-1;
	    tablero[i][j+4]=-1;			
	}
}
/*
	para saber si se termino el tiempo
*/
int tiempo_h(void *data)
{
	if(!fin_h&&difftime(time(NULL),t_h)>=conf.tiempo){
		fin(turno_h);
		return 0;		
	}
	ctiempo(conf.tiempo-difftime(time(NULL),t_h));
	return 1;
}
/*
	inicializa las variables,procesa el archivo de configuracion e inicia el juego
*/
void juegonuevo()
{
	int i,j;
	char s[100];
	if(dnuevo()!=1)return;
	gtk_statusbar_pop(GTK_STATUSBAR(status_bar),0);
	m=0;
	atacado_h=0;
	atacado_ai=0;
	ganador_h=0;
	ganador_ai=0;
	num_ai=1;
	num_h=1;
	mov=0;
	fin_ai=0;
	fin_h=0;
	for(i=0;i<12;i++){
		for(j=0;j<2;j++){
			p_h[i][j]=0;
			p_ai[i][j]=0;
		}			
	}
	for(i=0;i<5;i++){
		uposh[i][0]=-1;
		uposai[i][0]=-1;		
	}
	cturno(-1);
	ctiempo(-1);
	conf=cargar_conf(ruta_c);
	if(conf.estilo==-1){
		limpiartablero(tablero_h);
		dibujartablero(0,0,0,tablero_h);
		fin_ai=1;
		fin_h=1;
		msg("No se pudo abrir el archivo:config.xml");
		return;
	}
	//se asegura que la computadora sea el jugador 0 y el humano el 1	
	if(tipo<2&&!(strcmp(conf.jugador[0],nom_ai))){
		strcpy(conf.jugador[0],conf.jugador[1]);
		strcpy(conf.jugador[1],nom_ai);
		conf.empieza=!conf.empieza;
	}
	if(tipo==2&&!(strcmp(conf.jugador[1],nom_h))){
		strcpy(conf.jugador[1],conf.jugador[0]);
		strcpy(conf.jugador[0],nom_h);
		conf.empieza=!conf.empieza;
	}
	
	if(tipo==1)
		prep=0;
	else
		prep=1;
	#if DEBUG
		prep=0;
	#endif
	switch(tipo){
		case 0:
			if(strcmp(conf.jugador[1],nom_ai)){
				sprintf(s,"El jugador %s no se encuentra en el archivo de configuracion",nom_ai);
				msg(s);
				return;
			}
			if(strcmp(conf.jugador[0],nom_h)){
				sprintf(s,"El jugador %s no se encuentra en el archivo de configuracion",nom_h);
				msg(s);
				return;				
			}
			break;
		case 1:
			if(strcmp(conf.jugador[1],nom_ai)){
				sprintf(s,"El jugador %s no se encuentra en el archivo de configuracion",nom_ai);
				msg(s);
				return;
			}
			break;
		case 2:
			if(strcmp(conf.jugador[0],nom_h)){
				sprintf(s,"El jugador %s no se encuentra en el archivo de configuracion",nom_h);
				msg(s);
				return;
			}
			break;		
	}
#if DEBUG
{
	FILE *log;
	char s[200];	
	sprintf(s,"%slog",ruta);
	log=fopen(s,"w");
	fclose(log);
}
#endif
	//si empieza la computadora
	if(conf.empieza){
		ai=0;
		turno_h=0;	
		turno_ai=1;
		cargar_ai(0,tablero_ai);
		inicializartablero(1,tablero_h);
#if DEBUG
		_cargar_ai(1,tablero_h);
#endif
	}else{
		ai=1;
		turno_ai=0;
		turno_h=1;	
		cargar_ai(1,tablero_ai);
		inicializartablero(0,tablero_h);
#if DEBUG
		_cargar_ai(0,tablero_h);
#endif
	}
	if(ai){
		li_h=1;		
		ls_h=100;
		li_ai=101;
		ls_ai=200;			
	}else{
		li_h=101;
		ls_h=200;					
		li_ai=1;
		ls_ai=100;						
	}
	if(tipo==1)
		dibujartablero(0,0,0,tablero_ai);
	else{
		dibujartablero(0,0,0,tablero_h);
		gtk_statusbar_push(GTK_STATUSBAR(status_bar),0,"Haga click en una posición para agregar una pieza");
	}
	if(!to_ai){
		if(tipo!=2)to_ai=gtk_timeout_add(TO,esperar_mov_ai,NULL);
	}else{
		if(tipo==2){
			gtk_timeout_remove(to_ai);
			to_ai=0;
		}			
	}
	if((tipo!=1)&&(!ai)){
		to_h=gtk_timeout_add(TO,esperar_mov_h,NULL);
	}
	t_h=time(NULL);
	t_ai=time(NULL);
	if(to_t_h){
		gtk_timeout_remove(to_t_h);
		to_t_h=0;
	}						
	if(to_t_ai){
		gtk_timeout_remove(to_t_ai);
		to_t_ai=0;
	}						
}
