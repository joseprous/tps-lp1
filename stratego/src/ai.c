//CREADO POR JOSE PROUS(50139)
#include "stratego.h"
//de stratego.c
extern int F,B,E,M,G,C,T,Y,N,O,A,S;
extern struct config conf;
extern int tipo;
extern int ai;
extern int prep;
extern char ruta[200];
extern char *ruta_c,*ruta_j;
extern char nom_ai[100],nom_h[100];
extern int num_h;
extern int li_h,ls_h;

int num_ai;//numero de jugada
int tablero_ai[10][10];
int p_ai[12][2];//puntajes
int t_ai;//tiempo
int atacado_ai;
int ganador_ai;
int li_ai,ls_ai;//para saber que numeros le corresponden
int posini_ai[2];
int posfin_ai[2];
int turno_ai;//1 cuando empieza la computadora
int fin_ai;//para saber si se termino el juego
int to_ai,to_t_ai;//controlar los timers
int uposai[5][2];
#define MOD(L) if(cant[v]<L){b=0;cant[v]++;}break;
/*
	carga las fichas de la computadora
	n=0 abajo
	n=1 arriba
*/
void cargar_ai(int n,int tablero[10][10])
{
	int i,j,v,b;
	int cant[14]={0};
	inicializartablero(2,tablero);
	v=rand()%10;
	tablero[!n*9][v]=F+n*100;
	cant[F]++;
	if(v!=9){
		tablero[!n*9][v+1]=B+n*100;
		cant[B]++;		
	}
	if(v){
		tablero[!n*9][v-1]=B+n*100;
		cant[B]++;		
	}
	if(n)
		tablero[1][v]=B+100;
	else
		tablero[8][v]=B;
	cant[B]++;
	b=1;
	do{
		v=rand()%10;
		if(tablero[!n*9][v]==1||tablero[!n*9][v]==101){
			tablero[!n*9][v]=A+n*100;
			cant[A]++;
			b=0;
		}
	}while(b);
	for(i=0+!n*6;i<4+!n*6;i++)
		for(j=0;j<10;j++){
			if(tablero[i][j]==1||tablero[i][j]==101){
				b=1;
				do{
					v=rand()%11+2;
					switch(v){
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
}
#undef MOD
/*
	mueve una ficha de la computadora
*/
void mover_ai(int tablero[10][10])
{
	int fi,co,dir,p,i,j,b;
	time_t hora=time(NULL);
	if(ai){
		p=1;	
	}else{
		p=-1;	
	}
	b=1;
	for(i=!ai*9;(ai&&i<10)||(!ai&&i>=0);i+=1*p)
		for(j=0;j<10;j++){
			if(tablero[i][j]>=li_ai&&tablero[i][j]<ls_ai&&tablero[i][j]-ai*100!=B&&tablero[i][j]-ai*100!=F){
				if(j-1>=0){
					if(!tablero[i][j-1])b=0;
					if(tablero[i][j-1]>=li_h&&tablero[i][j-1]<ls_h&&lucha(tablero[i][j]-ai*100,tablero[i][j-1]-!ai*100)!=1)
						if(mover(i,j,i,j-1,1,atacado_ai,ganador_ai,tablero)){
							int k,l;
							for(k=3;k>0;k--)
								for(l=0;l<2;l++)
									uposai[k][l]=uposai[k-1][l];
							uposai[0][0]=i;
							uposai[0][1]=j-1;							
							return;
						}
				}
				if(j+1<10){
					if(!tablero[i][j+1])b=0;
					if(tablero[i][j+1]>=li_h&&tablero[i][j+1]<ls_h&&lucha(tablero[i][j]-ai*100,tablero[i][j+1]-!ai*100)!=1)
						if(mover(i,j,i,j+1,1,atacado_ai,ganador_ai,tablero)){
							int k,l;
							for(k=3;k>0;k--)
								for(l=0;l<2;l++)
									uposai[k][l]=uposai[k-1][l];
							uposai[0][0]=i;
							uposai[0][1]=j+1;							
							return;
						}
				}
				if(i-1>=0){
					if(!tablero[i-1][j])b=0;
					if(tablero[i-1][j]>=li_h&&tablero[i-1][j]<ls_h&&lucha(tablero[i][j]-ai*100,tablero[i-1][j]-!ai*100)!=1)
						if(mover(i,j,i-1,j,1,atacado_ai,ganador_ai,tablero)){
							int k,l;
							for(k=3;k>0;k--)
								for(l=0;l<2;l++)
									uposai[k][l]=uposai[k-1][l];
							uposai[0][0]=i-1;
							uposai[0][1]=j;							
							return;
						}
				}
				if(i+1<10){
					if(!tablero[i+1][j])b=0;
					if(tablero[i+1][j]>=li_h&&tablero[i+1][j]<ls_h&&lucha(tablero[i][j]-ai*100,tablero[i+1][j]-!ai*100)!=1)
						if(mover(i,j,i+1,j,1,atacado_ai,ganador_ai,tablero)){
							int k,l;
							for(k=3;k>0;k--)
								for(l=0;l<2;l++)
									uposai[k][l]=uposai[k-1][l];
							uposai[0][0]=i+1;
							uposai[0][1]=j;							
							return;
						}
				}					
			}
		}
	for(;;){
		if(b||difftime(time(NULL),hora)>=conf.tiempo){
			fin_ai=1;
			gtk_timeout_remove(to_t_ai);
			to_t_ai=0;	
			if(tipo==1)resumen(p_ai,0);		
			return;			
		}
		fi=rand()%10;
		co=rand()%10;
		if(tablero[fi][co]<li_ai||tablero[fi][co]>ls_ai)continue;
		dir=rand()%11;
		switch(dir){
			case 0:case 1:case 2:case 3:case 4:case 5:
				if(fi+p<10&&fi+p>=0&&!lucha(tablero[fi][co]-ai*100,tablero[fi+p][co]-!ai*100)&&mover(fi,co,fi+p,co,1,atacado_ai,ganador_ai,tablero_ai)){
					int k,l;
					for(k=3;k>0;k--)
						for(l=0;l<2;l++)
							uposai[k][l]=uposai[k-1][l];
					uposai[0][0]=fi;
					uposai[0][1]=co+p;							
					return;
				}
				break;
			case 6:case 7:
				if(co-1>=0&&!lucha(tablero[fi][co]-ai*100,tablero[fi][co-1]-!ai*100)&&mover(fi,co,fi,co-1,1,atacado_ai,ganador_ai,tablero_ai)){
					int k,l;
					for(k=3;k>0;k--)
						for(l=0;l<2;l++)
							uposai[k][l]=uposai[k-1][l];
					uposai[0][0]=fi;
					uposai[0][1]=co-1;							
					return;
				}
				break;
			case 8:case 9:
				if(co+1<10&&!lucha(tablero[fi][co]-ai*100,tablero[fi][co+1]-!ai*100)&&mover(fi,co,fi,co+1,1,atacado_ai,ganador_ai,tablero_ai)){
					int k,l;
					for(k=3;k>0;k--)
						for(l=0;l<2;l++)
							uposai[k][l]=uposai[k-1][l];
					uposai[0][0]=fi;
					uposai[0][1]=co+1;							
					return;
				}
				break;
			case 10:
				if(fi-p>=0&&fi-p<10&&!lucha(tablero[fi][co]-ai*100,tablero[fi-p][co]-!ai*100)&&mover(fi,co,fi-p,co,1,atacado_ai,ganador_ai,tablero_ai)){
					int k,l;
					for(k=3;k>0;k--)
						for(l=0;l<2;l++)
							uposai[k][l]=uposai[k-1][l];
					uposai[0][0]=fi-p;
					uposai[0][1]=co;							
					return;
				}
				break;
		}	
	}
}
/*
	lee y procesa la jugada del contrincante y mueve una ficha
*/
int esperar_mov_ai(void *data)
{
	struct jugada jug;
	char s[200];
	if(fin_ai||prep)return 1;
	if(turno_ai){//si empiza la computadora
		turno_ai=0;
		mover_ai(tablero_ai);
		t_ai=time(NULL);
		if(!to_t_ai){
			to_t_ai=gtk_timeout_add(1000,tiempo_ai,NULL);
		}
		if(tipo==1)
			dibujartablero(0,0,0,tablero_ai);
		return 1;		
	}
	jug=leer_jugada(ruta_j,conf.jugador[0]);
	if(jug.num==-1)
		return 1;
	else{
		sprintf(s,"%s%s.xml",ruta,conf.jugador[0]);
		remove(s);
		//hay un resultado del ataque anterior
		if(jug.atacado!=-1){
			//si ganó el atacante 
			if(!jug.ganador){
				p_ai[jug.atacado-2][0]++;
				tablero_ai[posfin_ai[0]][posfin_ai[1]]=tablero_ai[posini_ai[0]][posini_ai[1]];
				tablero_ai[posini_ai[0]][posini_ai[1]]=0;
				if(jug.atacado==F){//si gano
					fin_ai=1;
					gtk_timeout_remove(to_t_ai);
					to_t_ai=0;	
					if(tipo==1)resumen(p_ai,1);		
					return 1;					
				}				
			}else{
				//si ganó el atacado
				if(jug.ganador==1){
					p_ai[tablero_ai[posini_ai[0]][posini_ai[1]]-ai*100-2][1]++;
					if(conf.estilo||tablero_ai[posini_ai[0]][posini_ai[1]]-ai*100==S||jug.atacado==B){
						tablero_ai[posfin_ai[0]][posfin_ai[1]]=jug.atacado+!ai*100;
						tablero_ai[posini_ai[0]][posini_ai[1]]=0;					
					}else{
						tablero_ai[posini_ai[0]][posini_ai[1]]=jug.atacado+!ai*100;						
						tablero_ai[posfin_ai[0]][posfin_ai[1]]=0;										
					}
				}
				//si fué empate
				if(jug.ganador==2){
					tablero_ai[posfin_ai[0]][posfin_ai[1]]=0;
					tablero_ai[posini_ai[0]][posini_ai[1]]=0;
				}
			}			
		}
		
		//si el contrincante no atacó
		if(jug.atacante==-1){
			tablero_ai[jug.posfin[0]][jug.posfin[1]]=tablero_ai[jug.posini[0]][jug.posini[1]];
			tablero_ai[jug.posini[0]][jug.posini[1]]=0;
			atacado_ai=0;
			ganador_ai=0;
		}else{
			atacado_ai=tablero_ai[jug.posfin[0]][jug.posfin[1]]-ai*100;
			ganador_ai=lucha(jug.atacante,tablero_ai[jug.posfin[0]][jug.posfin[1]]-ai*100);
			switch(ganador_ai){
				case 0://ganó el atacante
					p_ai[atacado_ai-2][1]++;
					tablero_ai[jug.posfin[0]][jug.posfin[1]]=jug.atacante+!ai*100;
					tablero_ai[jug.posini[0]][jug.posini[1]]=0;
					break;
				case 1://ganó el atacado
					p_ai[jug.atacante-2][0]++;
					if(conf.estilo||jug.atacante==S||atacado_ai==B){
						tablero_ai[jug.posini[0]][jug.posini[1]]=0;						
					}else{
						tablero_ai[jug.posini[0]][jug.posini[1]]=tablero_ai[jug.posfin[0]][jug.posfin[1]];
						tablero_ai[jug.posfin[0]][jug.posfin[1]]=0;
					}
					break;
				case 2://empate
					tablero_ai[jug.posfin[0]][jug.posfin[1]]=0;
					tablero_ai[jug.posini[0]][jug.posini[1]]=0;
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
		fprintf(log,"posini:%d %d\tposfin:%d %d\n",posini_ai[0],posini_ai[1],posfin_ai[0],posfin_ai[1]);
		for(i=0;i<10;i++){
			for(j=0;j<10;j++){
				fprintf(log,"%3d ",tablero_ai[i][j]);	
			}			
			fprintf(log,"\n");		
		}
		fclose(log);		
}	
#endif
		num_ai=jug.num+1;
		mover_ai(tablero_ai);
		if(tipo==1)
			dibujartablero(0,0,0,tablero_ai);
		t_ai=time(NULL);//esperar_mov_h
		if(!to_t_ai){
			to_t_ai=gtk_timeout_add(1000,tiempo_ai,NULL);
		}
	}
	if(atacado_ai==F){//si perdio
		struct jugada jug;
		strcpy(jug.jugador,conf.jugador[1]);
		jug.num=num_h;
		jug.posini[0]=-1;
		jug.atacante=0;
		jug.atacado=F;
		jug.ganador=0;
		fin_ai=1;	
		gtk_timeout_remove(to_t_ai);
		to_t_ai=0;
		if(tipo==1)resumen(p_ai,0);		
	}	
	return 1;
}

/*
	para saber si se termino el tiempo
*/
int tiempo_ai(void *data)
{
	if(!fin_ai&&difftime(time(NULL),t_ai)>=conf.tiempo){
		fin_ai=1;
		gtk_timeout_remove(to_t_ai);
		to_t_ai=0;	
		if(tipo==1)resumen(p_ai,1);		
		return 0;						
	}
	return 1;
}
