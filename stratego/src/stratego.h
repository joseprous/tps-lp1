#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include "conf.h"
#include "jugadas.h"
#include "ai.h"
#include "gui.h"
#define TO 500
#define DEBUG 0
#define NOM_AI "50139ai"
#define NOM_H "50139h"

void inicializartablero(int n,int tablero[10][10]);
void juegonuevo();
int esperar_mov_ai(void *data);
int esperar_mov_h(void *data);
void cargar_ai();
int tiempo_h(void *data);
int lucha(int r1,int r2);
void inicializartablero(int n,int tablero[10][10]);
int mover(int fo,int co,int fd,int cd,int q,int atacado,int ganador,int tablero[10][10]);
