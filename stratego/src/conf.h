struct config{
	int soldados;//soldadosMuchosPasos
	int bombas;//sacarBombasCuandoExplotan
	int estilo;//0 americano; 1 europeo
	int tiempo;
	char jugador[2][20];
	int empieza;
};
struct config cargar_conf(char *ruta);
