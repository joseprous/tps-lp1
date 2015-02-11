//CREADO POR JOSE PROUS(50139)
#include "stratego.h"
//de stratego.c
extern struct config conf;
extern int to_h,to_t_h;
extern int t_h;
extern int F,B,E,M,G,C,T,Y,N,O,A,S,V;
extern int prep;
extern int mov;
extern int fin_h;
extern int turno_h;
extern char ruta[200];
extern int tipo;
extern int posini_h[2];
extern int posfin_h[2];
extern int atacado_h;
extern int ganador_h;
extern int ai;
extern int li_h,ls_h;
extern int num_h;
extern char nom_ai[100],nom_h[100];
extern int p_h[11][2];
extern int tablero_h[10][10];
extern int m;
extern int desc;
extern int uposh[5][2];

//de ai.c
extern int to_ai,to_t_ai;
extern int t_ai;
extern int fin_ai;
extern int turno_ai;
extern int posini_ai[2];
extern int posfin_ai[2];
extern int atacado_ai;
extern int ganador_ai;
extern int li_ai,ls_ai;
extern int num_ai;
extern int p_ai[11][2];
extern int tablero_ai[10][10];

static GtkWidget *window;//ventana principal
static GtkWidget *image[11][11];//array de imagenes
static GtkWidget *table0;//tabla de 4*1 contiene el menu,el tablero y la barra de estado
static GtkWidget *menu_bar;//barra de menu donde se encuentra el menu principal
static GtkWidget *table;//tabla de 11*11 que contiene a las imagenes
static GtkWidget *table2;//tabla de 1*2 contiene el label de turno y el boton de confirmar
static GtkWidget *confpos;//boton para confirmar posiciones
static GtkWidget *event_box;//para controlar los eventos del tablero
static GtkWidget *menu2;//menu contextual
static GtkWidget *label[2];//nombre del jugador
static GtkWidget *text[2];//nombre del jugador
static GtkWidget *turno;//label que contiene el turno
static GtkWidget *ltiempo;//label que contiene el tiempo
static GtkWidget *check;//checkbox de dejar descubiertas las piezas
static GtkWidget *logo;
static int f,c;//guarda la fila y columna de la la ultima ficha seleccionada
static GdkPixbuf *imagenes[27];//contiene las imagenes de las fichas
GtkWidget *status_bar;//barra de estado


gint delete_event( GtkWidget *widget,GdkEvent *event, gpointer data);
static gint button_press_event( GtkWidget *widget, GdkEventButton *event);
void creartablero();
void crearmenu();
void crearmenu2();
void modificar_tablero(GtkWidget *widget,int* v);
void cargar_imagenes();
/*
	emite un mensaje con el texto especificado
*/
void msg(char *s)
{
	GtkWidget *msg_box;
	msg_box=gtk_message_dialog_new_with_markup(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_NONE,s);
	gtk_dialog_add_button(GTK_DIALOG (msg_box),"Aceptar",1);
	gtk_dialog_run (GTK_DIALOG (msg_box));
 	gtk_widget_destroy (msg_box);
}

/*
	Resumen
*/
#define RCAT0(R,M) strcat(s,#R);sprintf(aux,"(%2.2d)",M); strcat(s,aux);strcat(s,":\t");\
		  sprintf(aux,"%d\t%d\t  %d\n",p_h[R-2][0],p_h[R-2][1],(p_h[R-2][0]-p_h[R-2][1])*M);strcat(s,aux);tot+=(p_h[R-2][0]-p_h[R-2][1])*M;
#define RCAT1(R,M) sprintf(aux,"\t%d\t%d\t  %d\n",p_h[R-2][1],p_h[R-2][0],(p_h[R-2][1]-p_h[R-2][0])*M);strcat(s,aux);tot+=(p_h[R-2][1]-p_h[R-2][0])*M;

void resumen(int p_h[][2],int g)
{
	GtkWidget *dialogo;
	GtkWidget *table0;
	gchar *stringPlainText;
    	PangoAttrList  *attrList;
    	GtkWidget      *displayLabelWidget;
	
	char s[500]="";
	char aux[100];
	int tot;

	sprintf(s,"<b>RESUMEN:</b>\n<b>Ganador:</b>");
	if((ai&&!g)||(!ai&&g))
		strcat(s,"<span foreground=\"red\"><b>");
	else
		strcat(s,"<span foreground=\"blue\"><b>");
	strcat(s,conf.jugador[g]);
	strcat(s,"</b></span>\n");
	dialogo=gtk_message_dialog_new_with_markup(GTK_WINDOW(window),GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_NONE,s);
	gtk_dialog_add_button(GTK_DIALOG (dialogo),"Aceptar",1);

	table0= gtk_table_new (1, 2, 0);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialogo)->vbox), table0);
	
	sprintf(s,"\t\t<b>Jugador:</b>");
	if((ai&&tipo!=1)||(!ai&&tipo==1))
		strcat(s,"<span foreground=\"red\"><b>");
	else
		strcat(s,"<span foreground=\"blue\"><b>");
	if(tipo==1)
		strcat(s,conf.jugador[1]);
	else
		strcat(s,conf.jugador[0]);		
	strcat(s,"</b></span>\n\t\t<b>Ga</b>\t<b>Pe</b>\t<b>Pu</b>\n");
	if((!g&&tipo!=1)||(g&&tipo==1)){
		strcat(s,"F(20):\t1\t0\t  20\n");
		tot=20;
	}else{
		strcat(s,"F(20):\t0\t1\t  -20\n");
		tot=-20;
	}		
	RCAT0(B,1);
	RCAT0(E,1);	
	RCAT0(M,M);
	RCAT0(G,G);
	RCAT0(C,C);
	RCAT0(T,T);
	RCAT0(Y,Y);
	RCAT0(N,N);
	RCAT0(O,O);
	RCAT0(A,A);
	RCAT0(S,S);
	strcat(s,"\t\t<b>Puntaje Total:</b>\n\t\t\t<big><b>");
	if((ai&&tipo!=1)||(!ai&&tipo==1))
		strcat(s,"<span foreground=\"red\">");
	else
		strcat(s,"<span foreground=\"blue\">");
	sprintf(aux,"%d",tot);
	strcat(s,aux);					
	strcat(s,"</span></b></big>");
	//printf("%d\n",strlen(s));
    pango_parse_markup(s, -1, 0, &attrList, &stringPlainText, NULL, NULL);
    displayLabelWidget = gtk_label_new(stringPlainText);
    gtk_label_set_attributes(GTK_LABEL(displayLabelWidget), attrList);
	gtk_table_attach_defaults (GTK_TABLE (table0), displayLabelWidget, 0, 1,0, 1);
    gtk_widget_show(displayLabelWidget);	

	sprintf(s,"\t<b>Jugador:</b>");
	if((!ai&&tipo!=1)||(ai&&tipo==1))
		strcat(s,"<span foreground=\"red\"><b>");
	else
		strcat(s,"<span foreground=\"blue\"><b>");
	if(tipo==1)
		strcat(s,conf.jugador[0]);		
	else
		strcat(s,conf.jugador[1]);				
	strcat(s,"</b></span>\n\t<b>Ga</b>\t<b>Pe</b>\t<b>Pu</b>\n");
	if((g&&tipo!=1)||(!g&&tipo==1)){
		strcat(s,"\t1\t0\t  20\n");
		tot=20;
	}else{
		strcat(s,"\t0\t1\t  -20\n");
		tot=-20;
	}		
	RCAT1(B,1);
	RCAT1(E,1);	
	RCAT1(M,M);
	RCAT1(G,G);
	RCAT1(C,C);
	RCAT1(T,T);
	RCAT1(Y,Y);
	RCAT1(N,N);
	RCAT1(O,O);
	RCAT1(A,A);
	RCAT1(S,S);
	strcat(s,"\t<b>Puntaje Total:</b>\n\t\t<big><b>");
	if((!ai&&tipo!=1)||(ai&&tipo==1))
		strcat(s,"<span foreground=\"red\">");
	else
		strcat(s,"<span foreground=\"blue\">");
	sprintf(aux,"%d",tot);
	strcat(s,aux);	
	strcat(s,"</span></b></big>");

    pango_parse_markup(s, -1, 0, &attrList, &stringPlainText, NULL, NULL);
    displayLabelWidget = gtk_label_new(stringPlainText);
    gtk_label_set_attributes(GTK_LABEL(displayLabelWidget), attrList);
	gtk_table_attach_defaults (GTK_TABLE (table0), displayLabelWidget, 1, 2,0, 1);
    gtk_widget_show(displayLabelWidget);	


	gtk_widget_show(table0);
	
	gtk_dialog_run (GTK_DIALOG (dialogo));
 	gtk_widget_destroy (dialogo);
}

/*
	cambia el tipo de juego y muestra los textos necesarios
	de acuerdo al tipo
*/
void ctipo( GtkWidget *widget,
               gpointer   data )
{
	tipo=*((int *)data);
	switch(tipo){
		case 0:
			gtk_widget_show(text[0]);
			gtk_widget_show(label[0]);
			gtk_widget_show(text[1]);
			gtk_widget_show(label[1]);
			break;
		case 1:
			gtk_widget_hide(text[0]);
			gtk_widget_hide(label[0]);
			gtk_widget_show(text[1]);
			gtk_widget_show(label[1]);
			break;
		case 2:
			gtk_widget_show(text[0]);
			gtk_widget_show(label[0]);
			gtk_widget_hide(text[1]);
			gtk_widget_hide(label[1]);
			break;		
		
	}
}

/*
	modifica el nombre del humano cuando se modifica el texto
*/
void cnom_h( GtkWidget *widget,
                     GtkWidget *entry )
{
	strcpy(nom_h,gtk_entry_get_text (GTK_ENTRY (entry)));
}

/*
	modifica el nombre de la computadora cuando se modifica el texto
*/
void cnom_ai( GtkWidget *widget,
                     GtkWidget *entry )
{
	strcpy(nom_ai,gtk_entry_get_text (GTK_ENTRY (entry)));
}

/*
	alternar dejar descubiertas las piezas
*/
void cdesc( GtkWidget *widget,
               gpointer   data )
{
	int *d=(int *)data;
	(*d)=!(*d);
}

/*
	crea y muestra el dialogo para juego nuevo
*/
int dnuevo()
{
	GtkWidget *dialogo;
	GtkWidget *table0;
	GtkWidget *table1;	
	GtkWidget *titulo;
	GtkWidget *button[3];
	GSList *group;
	int cero=0,uno=1,dos=2,r,d=desc;
 	dialogo = gtk_dialog_new_with_buttons ("Juego Nuevo",
                                                  GTK_WINDOW(window),
                                                  GTK_DIALOG_MODAL,NULL);
	gtk_window_set_resizable (GTK_WINDOW(dialogo),0);
	gtk_dialog_add_button(GTK_DIALOG (dialogo),"Aceptar",1);	
	table0= gtk_table_new (7, 1, 0);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialogo)->vbox), table0);
	titulo= gtk_label_new("Selecione el tipo de juego");
	gtk_widget_show  (titulo);		
	gtk_table_attach_defaults (GTK_TABLE (table0), titulo, 0, 1,0, 1);
	button[0] = gtk_radio_button_new_with_label (NULL, "humano vs ai");
	g_signal_connect (G_OBJECT (button[0]),"clicked",G_CALLBACK (ctipo),&cero);	
	gtk_widget_show  (button[0]);
	gtk_table_attach_defaults (GTK_TABLE (table0), button[0], 0, 1,1, 2);	
	group = gtk_radio_button_get_group (GTK_RADIO_BUTTON (button[0]));
	button[1] = gtk_radio_button_new_with_label (group, "ai vs ai");
	g_signal_connect (G_OBJECT (button[1]),"clicked",G_CALLBACK (ctipo),&uno);	
	gtk_widget_show  (button[1]);	
	gtk_table_attach_defaults (GTK_TABLE (table0), button[1], 0, 1,2, 3);	
	button[2] = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (button[1]),
                                                          "humano vs humano");
	g_signal_connect (G_OBJECT (button[2]),"clicked",G_CALLBACK (ctipo),&dos);	
	gtk_widget_show  (button[2]);
	gtk_table_attach_defaults (GTK_TABLE (table0), button[2], 0, 1,3, 4);
		
	table1= gtk_table_new (2, 2, 0);
	gtk_table_attach_defaults (GTK_TABLE (table0), table1, 0, 1,5, 6);
	
	label[0]= gtk_label_new("Nombre Humano:");
	gtk_widget_show  (label[0]);				
	gtk_table_attach_defaults (GTK_TABLE (table1), label[0], 0, 1,0, 1);

	label[1]= gtk_label_new("Nombre AI:");
	gtk_widget_show  (label[1]);				
	gtk_table_attach_defaults (GTK_TABLE (table1), label[1], 0, 1,1, 2);
	
	text[0]=gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(text[0]),nom_h);
    	g_signal_connect (G_OBJECT (text[0]), "changed",G_CALLBACK (cnom_h),(gpointer) text[0]);
	gtk_widget_show  (text[0]);				
	gtk_table_attach_defaults (GTK_TABLE (table1), text[0], 1, 2,0, 1);

	text[1]=gtk_entry_new();
	gtk_widget_show  (text[1]);				
	gtk_entry_set_text(GTK_ENTRY(text[1]),nom_ai);
    	g_signal_connect (G_OBJECT (text[1]), "changed",G_CALLBACK (cnom_ai),(gpointer) text[1]);
	gtk_table_attach_defaults (GTK_TABLE (table1), text[1], 1, 2,1, 2);
	
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (button[tipo]), 1);
	gtk_widget_show  (table1);
	
	check=gtk_check_button_new_with_label ("Dejar descubiertas las piezas");
	gtk_table_attach_defaults (GTK_TABLE (table0), check, 0, 1,6, 7);
	gtk_toggle_button_set_active    (GTK_TOGGLE_BUTTON(check),d);
	g_signal_connect (G_OBJECT (check),"clicked",G_CALLBACK (cdesc),&d);		
	gtk_widget_show  (check);		

	gtk_widget_show  (table0);	
	
	r=gtk_dialog_run (GTK_DIALOG (dialogo));
	if(r==1){
		desc=d;	
	}
	gtk_widget_destroy (dialogo);
	return r;
}

/*
	evento que ocurre al hacer click en confirmar posiciones
*/
static gint confpos_event( GtkWidget *widget,GdkEventButton *event)
{
	prep=0;
	gtk_statusbar_pop(GTK_STATUSBAR(status_bar),0);
	gtk_widget_hide(confpos);
	return 1;	
}
/*
	crea la ventana y totos los objetos
*/
void crear_objetos()
{
	char nom[200];
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	table0= gtk_table_new (4, 1, 0);
	gtk_window_set_title(GTK_WINDOW(window),"Stratego");
	gtk_container_add (GTK_CONTAINER (window), table0);
	crearmenu();
	
	table2= gtk_table_new (1, 2, 0);	
	gtk_table_attach_defaults (GTK_TABLE (table0), table2, 0, 1,1, 2);	
	turno=gtk_label_new("");

	gtk_table_attach_defaults (GTK_TABLE (table2), turno, 0, 1,0, 1);	
	gtk_widget_show (turno);
	confpos=gtk_button_new_with_label("Confirmar posiciones");
	gtk_table_attach_defaults (GTK_TABLE (table2), confpos, 1, 2,0, 1);
	g_signal_connect (G_OBJECT (confpos), "clicked",G_CALLBACK (confpos_event),NULL);

	ltiempo=gtk_label_new("");
	gtk_table_attach_defaults (GTK_TABLE (table2), ltiempo, 2, 3,0, 1);	
	gtk_widget_show (ltiempo);

	gtk_widget_show (table2);
	cargar_imagenes();
	creartablero();
    	g_signal_connect (G_OBJECT (window), "delete_event",G_CALLBACK (delete_event), NULL);
	gtk_window_set_resizable (GTK_WINDOW(window),0);
	crearmenu2();
	
	status_bar = gtk_statusbar_new ();
	gtk_table_attach_defaults (GTK_TABLE (table0), status_bar, 0, 1,3, 4);
	gtk_widget_show (status_bar);
	
	gtk_widget_show (table0);
	
	sprintf(nom,"%simagenes/logo.png",ruta);
	logo=gtk_image_new_from_file(nom);
	gtk_window_set_icon(GTK_WINDOW(window),gtk_image_get_pixbuf(GTK_IMAGE(logo)));
	gtk_widget_show  (window);	
}
#define MOD(L) if(cant[*v]<L){if(tablero_h[f][c])cant[tablero_h[f][c]-!ai*100]--;tablero_h[f][c]=*v+!ai*100;cant[*v]++;}break;
/*
	modifica el tablero cuando el humano carga una ficha
*/
void modificar_tablero(GtkWidget *widget,int* v)
{
	int i,j,c1=0;
	int cant[14]={0};
	char s[100]="";
	for(i=0+ai*6;i<4+ai*6;i++)
		for(j=0;j<10;j++)
			if(tablero_h[i][j])
				cant[tablero_h[i][j]-!ai*100]++;
	switch(*v){
		case 13:
		case 11:	
		case 10:
		case 9:
			MOD(1);				
		case 12:
			MOD(6);	
		case 8:
			MOD(2);	
		case 7:
			MOD(3);	
		case 6:
		case 5:
		case 4:
			MOD(4);	
		case 3:
			MOD(5);	
		case 2:
			MOD(8);
		case 0:
			if(tablero_h[f][c]){
				cant[tablero_h[f][c]-!ai*100]--;
				tablero_h[f][c]=0;
			}
	}
	//tablero_h[f][c]+=!ai*100;
	sprintf(s,"F:%d B:%d E:%d M:%d G:%d C:%d T:%d Y:%d N:%d O:%d A:%d S:%d",1-cant[13],6-cant[12],1-cant[11],1-cant[10],1-cant[9],2-cant[8],3-cant[7],4-cant[6],4-cant[5],4-cant[4],5-cant[3],8-cant[2]);	
	gtk_statusbar_pop(GTK_STATUSBAR(status_bar),0);	
	gtk_statusbar_push(GTK_STATUSBAR(status_bar),0,s);	

	for(i=0+ai*6;i<4+ai*6;i++)
		for(j=0;j<10;j++)
			if(tablero_h[i][j])c1++;
			//if(tablero_h[i][j]!=1+!ai*100)c1++;
	if(c1==40){
		gtk_widget_show(confpos);	
	}else{
		gtk_widget_hide(confpos);			
	}
	dibujartablero(0,0,0,tablero_h);
}
#undef MOD
/*
	guardar configuarcion inicial
*/
void guardar_ini()
{
	GtkWidget *dialog;
	int i,j;
	if(!prep){
		msg("Solo se puede guardar cuando se está preparando el tablero.");
		return;
	}
	dialog = gtk_file_chooser_dialog_new ("Guardar archivo",GTK_WINDOW(window),
		GTK_FILE_CHOOSER_ACTION_SAVE,NULL);
	gtk_dialog_add_button(GTK_DIALOG (dialog),"Cancelar",0);	
	gtk_dialog_add_button(GTK_DIALOG (dialog),"Guardar",1);
	if (gtk_dialog_run (GTK_DIALOG (dialog))==1){
		char *filename;
		FILE *file;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		file=fopen(filename,"w");
		fprintf(file,"%d\n",ai);		
		for(i=ai*6;i<4+ai*6;i++)
			for(j=0;j<10;j++)
				fprintf(file,"%d\n",tablero_h[i][j]);
		fclose(file);
		//msg(filename);
	}
	gtk_widget_destroy (dialog);
}

/*
	cargar configuarcion inicial
*/
#define MOD(L) if(cant[v]<L){tablero_h[f][c]=v+!ai*100;cant[v]++;}break;
void cargar_ini()
{
	int i,j,v,p,c1=0;
	int cant[14]={0};
	GtkWidget *dialog;
	if(!prep){
		msg("Solo se puede cargar cuando se está preparando el tablero.");
		return;
	}
	inicializartablero(!ai,tablero_h);
	dialog = gtk_file_chooser_dialog_new ("Abrir archivo",GTK_WINDOW(window),
		GTK_FILE_CHOOSER_ACTION_OPEN,NULL);
	gtk_dialog_add_button(GTK_DIALOG (dialog),"Cancelar",0);	
	gtk_dialog_add_button(GTK_DIALOG (dialog),"Abrir",1);
	if (gtk_dialog_run (GTK_DIALOG (dialog)) == 1){
		char *filename;
		FILE *file;
		filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
		file=fopen(filename,"r");
		//p=0 arriba
		fscanf(file,"%d\n",&p);
		if(p==ai){
			for(f=ai*6;f<4+ai*6;f++)
				for(c=0;c<10;c++){
					fscanf(file,"%d\n",&v);
					v-=!p*100;
					switch(v){
						case 13:
						case 11:	
						case 10:
						case 9:
							MOD(1);				
						case 12:
							MOD(6);	
						case 8:
							MOD(2);	
						case 7:
							MOD(3);	
						case 6:
						case 5:
						case 4:
							MOD(4);	
						case 3:
							MOD(5);	
						case 2:
							MOD(8);
					}
					if(feof(file))break;
				}
			
		}else{
			for(f=3+ai*6;f>=ai*6;f--)
				for(c=9;c>=0;c--){
					fscanf(file,"%d\n",&v);
					v-=!p*100;
					switch(v){
						case 13:
						case 11:	
						case 10:
						case 9:
							MOD(1);				
						case 12:
							MOD(6);	
						case 8:
							MOD(2);	
						case 7:
							MOD(3);	
						case 6:
						case 5:
						case 4:
							MOD(4);	
						case 3:
							MOD(5);	
						case 2:
							MOD(8);
					}					
					if(feof(file))break;
				}		
		}
		fclose(file);	
		for(i=0+ai*6;i<4+ai*6;i++)
			for(j=0;j<10;j++)
				if(tablero_h[i][j])c1++;
		if(c1==40){
			gtk_widget_show(confpos);	
		}else{
			gtk_widget_hide(confpos);			
		}
		dibujartablero(0,0,0,tablero_h);			
	}
	gtk_widget_destroy (dialog);
}

/*
	muestra el dialogo acerca de
*/
void acercade()
{
	char *stringPlainText;
	PangoAttrList  *attrList;
	GtkWidget *dialogo;
	char s[200];
	GtkWidget *label;
	GtkWidget *table0;
	
	sprintf(s,"%simagenes/logo.png",ruta);
	logo=gtk_image_new_from_file(s);

	sprintf(s,"<span size=\"20000\"><b>     Stratego</b></span>\n");
	strcat(s,"\n<big><b>Creado por:</b> José Prous</big>\n");
	strcat(s,"\n                  2005"); 	
	dialogo = gtk_dialog_new_with_buttons ("Acerca de",GTK_WINDOW(window),GTK_DIALOG_MODAL,NULL);
	gtk_dialog_add_button(GTK_DIALOG (dialogo),"Aceptar",1);
	label=gtk_label_new("");
	table0= gtk_table_new (2, 1, 0);
	gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialogo)->vbox), table0);
	gtk_table_attach_defaults (GTK_TABLE (table0), logo, 0, 1,0, 1);
	gtk_widget_show(logo);
	gtk_table_attach_defaults (GTK_TABLE (table0), label, 0, 1,1, 2);	
	pango_parse_markup(s, -1, 0, &attrList, &stringPlainText, NULL, NULL);
	gtk_label_set_text (GTK_LABEL(label),stringPlainText);
	gtk_label_set_attributes(GTK_LABEL(label), attrList);
	gtk_widget_show(label);
	gtk_widget_show(table0);
	gtk_window_set_resizable (GTK_WINDOW(dialogo),0);
	gtk_dialog_run (GTK_DIALOG (dialogo));
 	gtk_widget_destroy (dialogo);
}
/*
	crea el menu principal
*/
void crearmenu()
{
	GtkWidget *menu;
	GtkWidget *root_menu;
	GtkWidget *menu_items;
	GtkWidget *imagen;
	GtkAccelGroup *accel_group;
	accel_group = gtk_accel_group_new ();
	
	menu_bar = gtk_menu_bar_new ();
	gtk_table_attach_defaults (GTK_TABLE (table0), menu_bar, 0, 1, 0, 1);
	
	menu = gtk_menu_new ();
	//Nuevo
	menu_items = gtk_image_menu_item_new_with_mnemonic ("_Nuevo");
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);
	imagen = gtk_image_new_from_stock ("gtk-new", GTK_ICON_SIZE_MENU);
  	gtk_widget_show (imagen);
  	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_items), imagen);
	
	g_signal_connect (G_OBJECT (menu_items), "activate",G_CALLBACK (juegonuevo),NULL);	

	gtk_widget_add_accelerator (menu_items, "activate", accel_group,
                              gdk_keyval_from_name("n"), GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);
	gtk_widget_show (menu_items);
	//barra
	menu_items =  gtk_separator_menu_item_new();
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);
	gtk_widget_show (menu_items);
	
	//Guardar Configuracion Inicial
	menu_items = gtk_image_menu_item_new_with_mnemonic ("_Guardar Configuracion Inicial");
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);
	
	imagen = gtk_image_new_from_stock ("gtk-save", GTK_ICON_SIZE_MENU);
  	gtk_widget_show (imagen);
  	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_items), imagen);

	g_signal_connect (G_OBJECT (menu_items), "activate",G_CALLBACK (guardar_ini),NULL);	

	gtk_widget_add_accelerator (menu_items, "activate", accel_group,
                              gdk_keyval_from_name("g"), GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);
	gtk_widget_show (menu_items);
	//Cargar Configuracion Inicial
	menu_items = gtk_image_menu_item_new_with_mnemonic ("_Cargar Configuracion Inicial");
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);

	imagen = gtk_image_new_from_stock ("gtk-open", GTK_ICON_SIZE_MENU);
  	gtk_widget_show (imagen);
  	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_items), imagen);

	g_signal_connect (G_OBJECT (menu_items), "activate",G_CALLBACK (cargar_ini),NULL);	
	
	gtk_widget_add_accelerator (menu_items, "activate", accel_group,
                              gdk_keyval_from_name("o"), GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);
	gtk_widget_show (menu_items);
	//barra
	menu_items =  gtk_separator_menu_item_new();
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);
	gtk_widget_show (menu_items);
	
	//salir
	menu_items = gtk_image_menu_item_new_with_mnemonic("_Salir");
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);
	
	imagen = gtk_image_new_from_stock ("gtk-quit", GTK_ICON_SIZE_MENU);
  	gtk_widget_show (imagen);
  	gtk_image_menu_item_set_image (GTK_IMAGE_MENU_ITEM (menu_items), imagen);

	g_signal_connect (G_OBJECT (menu_items), "activate",G_CALLBACK (delete_event),NULL);	
	
	gtk_widget_add_accelerator (menu_items, "activate", accel_group,
                              gdk_keyval_from_name("q"), GDK_CONTROL_MASK,
                              GTK_ACCEL_VISIBLE);
	gtk_widget_show (menu_items);

	gtk_widget_show (menu);
	
	root_menu = gtk_menu_item_new_with_mnemonic ("_Juego");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (root_menu), menu);
	gtk_widget_show (root_menu);

	gtk_widget_show (menu_bar);	
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), root_menu);
	
	menu = gtk_menu_new ();
	//Acerca de
	menu_items = gtk_image_menu_item_new_with_mnemonic ("_Acerca de");
	gtk_menu_shell_append (GTK_MENU_SHELL (menu), menu_items);
	
	g_signal_connect (G_OBJECT (menu_items), "activate",G_CALLBACK (acercade),NULL);	
	gtk_widget_show (menu_items);	
	
	root_menu = gtk_menu_item_new_with_mnemonic ("_Ayuda");
	gtk_menu_item_set_submenu (GTK_MENU_ITEM (root_menu), menu);
	gtk_widget_show (root_menu);

	gtk_widget_show (menu_bar);	
	gtk_menu_shell_append (GTK_MENU_SHELL (menu_bar), root_menu);

	gtk_window_add_accel_group (GTK_WINDOW (window), accel_group);
}

#define AGREGAR_MENU(NOMBRE,VALOR)	menu_items=gtk_menu_item_new_with_mnemonic (#NOMBRE); \
					gtk_menu_shell_append (GTK_MENU_SHELL (menu2), menu_items); \
					g_signal_connect(G_OBJECT (menu_items), "activate",G_CALLBACK (modificar_tablero),&VALOR); \
					gtk_widget_show (menu_items);
/*
	crea el menu contextual para cargar fichas
*/
void crearmenu2()
{
	GtkWidget *menu_items;
	menu2 = gtk_menu_new ();
	AGREGAR_MENU(10-Mariscal(_M),M);
	AGREGAR_MENU(9-General(_G),G);
	AGREGAR_MENU(8-Coronel(_C),C);
	AGREGAR_MENU(7-Teniente(_T),T);
	AGREGAR_MENU(6-Mayor(_Y),Y);
	AGREGAR_MENU(5-Capitan(_N),N);
	AGREGAR_MENU(4-Sargento(_O),O);
	AGREGAR_MENU(3-Cabo Desarmador(_A),A);
	AGREGAR_MENU(2-Soldado(_S),S);
	AGREGAR_MENU(Espia(_E),E);
	AGREGAR_MENU(Bandera(_F),F);
	AGREGAR_MENU(Bomba(_B),B);
	AGREGAR_MENU(BO_RRAR,V);	
}
/*
	evento que ocurre al cerrar la ventana
*/
gint delete_event( GtkWidget *widget,
                   GdkEvent  *event,
                   gpointer   data )
{
    gtk_main_quit ();
    return 0;
}
/*
	cambia el texto donde dice el turno
*/
void cturno(int t)
{
	char s[100]="";
	char *stringPlainText;
	PangoAttrList  *attrList;
	if(t!=-1){
		if((t&&ai)||(!t&&!ai))
			sprintf(s,"<b>Turno de:</b><span foreground=\"red\">");
		else
			sprintf(s,"<b>Turno de:</b><span foreground=\"blue\">");
		strcat(s,conf.jugador[!t]);
		strcat(s,"</span>");
	}
	pango_parse_markup(s, -1, 0, &attrList, &stringPlainText, NULL, NULL);
	gtk_label_set_text (GTK_LABEL(turno),stringPlainText);
	gtk_label_set_attributes(GTK_LABEL(turno), attrList);
	gtk_widget_show(turno);
}
/*
	cambia el texto donde dice el tiempo
*/
void ctiempo(int t)
{
	char s[100]="";
	char *stringPlainText;
	PangoAttrList  *attrList;
	if(t!=-1){
		sprintf(s,"<b>Tiempo Restante:</b>%d",t);
	}
	pango_parse_markup(s, -1, 0, &attrList, &stringPlainText, NULL, NULL);
	gtk_label_set_text (GTK_LABEL(ltiempo),stringPlainText);
	gtk_label_set_attributes(GTK_LABEL(ltiempo), attrList);
	gtk_widget_show(ltiempo);
}

/*
	evento que ocurre al hacer click sobre el tablero
*/
static gint button_press_event( GtkWidget      *widget,
                                GdkEventButton *event)
{
	int i,j;
	if(tipo==1)return 1;
	//si esta fuera del limite
	if(((int)event->y/50)>=10||((int)event->x/50)>=10)return 1;
	//si se está cargando fichas
	if(prep){
		f=(int)event->y/50;
		c=(int)event->x/50;
		if((ai&&f>5)||(!ai&&f<4))
			gtk_menu_popup (GTK_MENU (menu2), NULL, NULL, NULL, NULL,0,event->time);
	}else{
		if(!fin_h&&turno_h){
			if(mov){
				int y=(int)event->y/50,x=(int)event->x/50;
				if(y==uposh[1][0]&&x==uposh[1][1]){
					int b=0;
					for(i=0;i<2;i++){
						if(uposh[1][i]!=uposh[3][i]||uposh[0][i]!=uposh[2][i])b=1;
					}
					if(!b){
						msg("No se puede repetir el mismo movimiento 5 veces");
						return 1;
					}
				}
				if(mover(f,c,y,x,0,atacado_h,ganador_h,tablero_h)){
					t_h=time(NULL);//esperar_mov_ai
					if(!to_t_h){
						to_t_h=gtk_timeout_add(1000,tiempo_h,NULL);			
					}
					for(i=3;i>0;i--)
						for(j=0;j<2;j++)
							uposh[i][j]=uposh[i-1][j];
					uposh[0][0]=y;
					uposh[0][1]=x;
					num_h+=2;
					turno_h=0;
					if(tipo==2||tipo==0)cturno(0);
					to_h=gtk_timeout_add(TO,esperar_mov_h,NULL);
				}
			}
			mov=!mov;
			f=(int)event->y/50;
			c=(int)event->x/50;
			if(mov&&tablero_h[f][c]>li_h&&tablero_h[f][c]<ls_h)
				dibujartablero(f,c,1,tablero_h);
			else
				dibujartablero(0,0,0,tablero_h);								
		}
	}
	return 1;
}

/*
	carga las imagenes en un array
*/
void cargar_imagenes()
{
	int i;
	char nom[200];
	sprintf(nom,"%simagenes/%d.png",ruta,0);
	imagenes[0]=gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file(nom)));
	for(i=1;i<14;i++){
		sprintf(nom,"%simagenes/%d.png",ruta,i);
		imagenes[i]=gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file(nom)));
		sprintf(nom,"%simagenes/%d.png",ruta,i+100);
		imagenes[i+13]=gtk_image_get_pixbuf(GTK_IMAGE(gtk_image_new_from_file(nom)));		
	}
}
/*
	dibuja el tablero
	p sirve para saber si se marco una posicion
	p==1 se selecciono una fichas
	p==2 se debe mostrar la ficha
*/
void dibujartablero(int f,int c,int p,int tablero[10][10])
{
	int i,j;
	char nom[200];
	for(i=0;i<10;i++){
		for(j=0;j<10;j++){
			if(tablero[i][j]>-1){
				if(p==1){
					if(i==f&&j==c){
						sprintf(nom,"%simagenes/c%d.png",ruta,tablero[i][j]);	
						gtk_image_set_from_file(GTK_IMAGE(image[i][j]),nom);
						gtk_widget_show(image[i][j]);					
					}						
				}else{
					if(desc||(p==2&&i==f&&j==c)||!tablero[i][j]||(tipo!=1&&tablero[i][j]>li_h&&tablero[i][j]<ls_h)||(tipo==1&&tablero[i][j]>li_ai&&tablero[i][j]<ls_ai)){
						gtk_image_set_from_pixbuf(GTK_IMAGE(image[i][j]),imagenes[tablero[i][j]>100?tablero[i][j]-87:tablero[i][j]]);
					}else{
						gtk_image_set_from_pixbuf(GTK_IMAGE(image[i][j]),imagenes[tablero[i][j]>100?14:1]);
					}						
					gtk_widget_show(image[i][j]);
				}
			}
		}
	}
}
/*
	crea los objetos del tablero
*/
void creartablero()
{
	int i,j;
	char nom[200];
	event_box = gtk_event_box_new (); 
	gtk_table_attach_defaults (GTK_TABLE (table0), event_box, 0, 1, 2, 3);
	table = gtk_table_new (10, 10, 1);
	gtk_container_add (GTK_CONTAINER (event_box), table);	
	for(i=0;i<4;i++)
		for(j=0;j<10;j++){
			image[i][j] = gtk_image_new();
			gtk_table_attach_defaults (GTK_TABLE (table), image[i][j], j, j+1, i, i+1);
		}
	for(i=4;i<6;i++)
		for(j=0;j<2;j++){
			image[i][j] = gtk_image_new();
			gtk_table_attach_defaults (GTK_TABLE (table), image[i][j], j, j+1, i, i+1);
			image[i][j+4] = gtk_image_new();
			gtk_table_attach_defaults (GTK_TABLE (table), image[i][j+4], j+4, j+4+1, i, i+1);
			image[i][j+8] = gtk_image_new();
			gtk_table_attach_defaults (GTK_TABLE (table), image[i][j+8], j+8, j+8+1, i, i+1);
		}
	for(i=6;i<10;i++)
		for(j=0;j<10;j++){
			image[i][j] = gtk_image_new();
			gtk_table_attach_defaults (GTK_TABLE (table), image[i][j], j, j+1, i, i+1);
		}
	for(j=0;j<10;j++){
		sprintf(nom,"%simagenes/%db.png",ruta,j+1);
		image[10][j] = gtk_image_new_from_file(nom);
		gtk_table_attach_defaults (GTK_TABLE (table), image[10][j], j, j+1, 10, 11);
		gtk_widget_show(image[10][j]);	
		sprintf(nom,"%simagenes/%dr.png",ruta,j+1);
		image[j][10] = gtk_image_new_from_file(nom);
		gtk_table_attach_defaults (GTK_TABLE (table), image[j][10], 10, 11, j, j+1);		
		gtk_widget_show(image[j][10]);	
	}
	sprintf(nom,"%simagenes/negro.png",ruta);
	image[10][10] = gtk_image_new_from_file(nom);
	gtk_table_attach_defaults (GTK_TABLE (table), image[10][10], 10, 11, 10, 11);	
	gtk_widget_show(image[10][10]);
	sprintf(nom,"%simagenes/agua.png",ruta);
	image[4][2] = gtk_image_new_from_file(nom);
	gtk_table_attach_defaults (GTK_TABLE (table), image[4][2], 2, 4, 4, 6);
	gtk_widget_show(image[4][2]);
	image[4][6] = gtk_image_new_from_file(nom);
	gtk_table_attach_defaults (GTK_TABLE (table), image[4][6], 6, 8, 4, 6);
	gtk_widget_show(image[4][6]);
	gtk_widget_show (table);
	gtk_widget_show (event_box);		
    	g_signal_connect (G_OBJECT (event_box),"button_press_event",G_CALLBACK (button_press_event),NULL);	
}
