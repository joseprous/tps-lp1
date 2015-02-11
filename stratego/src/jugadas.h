struct jugada{
	int num;
	char jugador[20];
	int posini[2];
	int posfin[2];
	int atacante;
	int atacado;
	int ganador;	
};
struct jugada leer_jugada(char *ruta,char *nombre);
void escribir_jugada(struct jugada,char *ruta,int tablero[10][10]);
