//Creado por José Prous(50139)
#include <windows.h>
#include "resource.h"
#define TY 40
#define ID_NUEVO 5001
#define ID_PRINCIPIANTE 5002
#define ID_INTERMEDIO 5003
#define ID_EXPERTO 5004
#define ID_SALIR 5005
#define ID_ACERCADE 5006

int tablero[16][30];
HBITMAP imagenes[12],bboton[2],bcara[3],num[10];
LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM);
void inicio();
BOOL    CALLBACK AboutDlgProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
                    PSTR szCmdLine, int iCmdShow)
{
	static TCHAR szAppName[] = TEXT ("Buscaminas") ;
	HWND         hwnd ;
	MSG          msg ;
	WNDCLASS     wndclass ;
	
	srand(time(NULL));

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon       = LoadIcon (hInstance, szAppName);
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (BLACK_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;
	if (!RegisterClass (&wndclass)){
		MessageBox (NULL, TEXT ("This program requires Windows NT!"),
		szAppName, MB_ICONERROR) ;
		return 0 ;
	}
	
	hwnd = CreateWindow (szAppName, TEXT ("Buscaminas"),
				WS_BORDER|WS_SYSMENU|WS_MINIMIZEBOX,
				CW_USEDEFAULT, CW_USEDEFAULT,
				608,408,
				NULL, NULL, hInstance, NULL) ;
	
	ShowWindow (hwnd,iCmdShow) ;
	UpdateWindow (hwnd);
	while (GetMessage (&msg, NULL, 0, 0)){  
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}

/*
	Carga el tablero
*/
void inicio(int cminas,int fil,int col)
{
	int i,j,c=0;
	for(i=0;i<fil;i++)
		for(j=0;j<col;j++)
			tablero[i][j]=100;
	for(;c<cminas;){
		i=rand()%fil;
		j=rand()%col;
		if(tablero[i][j]==100){
			tablero[i][j]=109;
			c++;
		}
	}
	for(i=0;i<fil;i++)
		for(j=0;j<col;j++){
			if(tablero[i][j]==109){
				if(i>0&&tablero[i-1][j]!=109)tablero[i-1][j]+=1;
				if(j>0&&tablero[i][j-1]!=109)tablero[i][j-1]+=1;
				if(i<fil-1&&tablero[i+1][j]!=109)tablero[i+1][j]+=1;
				if(j<col-1&&tablero[i][j+1]!=109)tablero[i][j+1]+=1;
				if((i>0)&&(j>0)&&tablero[i-1][j-1]!=109)tablero[i-1][j-1]+=1;					
				if((i>0)&&(j<col-1)&&tablero[i-1][j+1]!=109)tablero[i-1][j+1]+=1;					
				if((i<fil-1)&&(j>0)&&tablero[i+1][j-1]!=109)tablero[i+1][j-1]+=1;					
				if((i<fil-1)&&(j<col-1)&&tablero[i+1][j+1]!=109)tablero[i+1][j+1]+=1;					
			}
		}	
}
LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static struct{
		HWND h;
		int b;//estado del boton
		}hwndButton[16][30],//array de botones
		cara,
		m,//cantidad de minas
		r;//reloj
	static RECT  rect ;
	static int n;//nivel
	static int t;//para saber si se termino el juego
	static int cb;//cantidad de bombas restantes 
	static int ca;//cantidad de casillas abiertas
	static int tiempo;
	static int cminas,fil,col;//cantidad de minas,filas y columnas del nivel
	static HINSTANCE hInstance ;		
	HDC          hdc ;
	PAINTSTRUCT  ps ;
	int          i,j ;
		
	switch (message){
	case WM_CREATE :{
		//crea los objetos
		for (i = 0 ; i < 16 ; i++)
			for(j=0;j<30;j++){
				hwndButton[i][j].h = CreateWindow ( TEXT("button"),"",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
					j*20,20*i+TY,20,20,hwnd, (HMENU)(i*100+j),
					((LPCREATESTRUCT) lParam)->hInstance, NULL);
				hwndButton[i][j].b=0;
			}
		cara.h = CreateWindow ( TEXT("button"),"",
				WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				287,2,35,35,hwnd, (HMENU)ID_NUEVO,
				((LPCREATESTRUCT) lParam)->hInstance, NULL);	
		m.h = CreateWindow ( TEXT("button"),"",
				WS_CHILD | WS_VISIBLE |BS_GROUPBOX,
				-2,-9,80,48,hwnd, (HMENU)6000,
				((LPCREATESTRUCT) lParam)->hInstance, NULL);
		r.h = CreateWindow ( TEXT("button"),"",
				WS_CHILD | WS_VISIBLE |BS_GROUPBOX,
				525,-9,80,48,hwnd, (HMENU)6000,
				((LPCREATESTRUCT) lParam)->hInstance, NULL);
		
		//carga las imagenes
		imagenes[11] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(MINAX));	
		imagenes[10] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(MINA2));
		imagenes[9] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(MINA));
		imagenes[0] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(GRIS));
		imagenes[1] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(UNO));
		imagenes[2] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(DOS));
		imagenes[3] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(TRES));
		imagenes[4] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(CUATRO));
		imagenes[5] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(CINCO));
		imagenes[6] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(SEIS));
		imagenes[7] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(SIETE));
		imagenes[8] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(OCHO));
		bboton[1] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(BANDERA));
		bboton[0] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(GRIS2));
		bcara[0] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(CARA0));
		bcara[1] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(CARA1));
		bcara[2] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(CARA2));		
		num[9] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(NUEVED));
		num[0] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(CEROD));
		num[1] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(UNOD));
		num[2] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(DOSD));
		num[3] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(TRESD));
		num[4] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(CUATROD));
		num[5] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(CINCOD));
		num[6] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(SEISD));
		num[7] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(SIETED));
		num[8] = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(OCHOD));
		
		//crea el menu
		{
			HMENU hMenu, hSubMenu;
			hMenu = CreateMenu(); 
			hSubMenu = CreatePopupMenu(); 
			AppendMenu(hSubMenu, MF_STRING, ID_NUEVO,"&Nuevo");
			AppendMenu (hSubMenu, MF_SEPARATOR,6000,NULL) ;			
			AppendMenu(hSubMenu, MF_STRING, ID_PRINCIPIANTE, "&Principiante"); 
			AppendMenu(hSubMenu, MF_STRING, ID_INTERMEDIO, "&Intermedio"); 			
			AppendMenu(hSubMenu, MF_STRING, ID_EXPERTO, "&Experto"); 
			AppendMenu (hSubMenu, MF_SEPARATOR,6000,NULL) ;			
			AppendMenu(hSubMenu, MF_STRING, ID_SALIR, "&Salir"); 
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Juego");
			hSubMenu = CreatePopupMenu();
			AppendMenu(hSubMenu, MF_STRING, ID_ACERCADE,"&Acerca de");
			AppendMenu(hMenu, MF_STRING | MF_POPUP, (UINT)hSubMenu, "&Ayuda");			
			SetMenu(hwnd, hMenu);		
		}
		//inicializa las variables para un juego en experto
		n=3;
		t=0;
		cminas=99;
		cara.b=0;
		ca=0;
		cb=99;
		fil=16;
		col=30;	
		tiempo=0;
		
		//inicializa el timer para dibujar cada medio segundo
		SetTimer (hwnd, 2, 500, NULL);	
		return 0 ;
	}
	case WM_PAINT :{
		BITMAP bm;
		ValidateRect (hwnd, &rect);
		hdc = BeginPaint (hwnd, &ps);

		{
			HDC hdcMem = CreateCompatibleDC(hdc);
			HBITMAP hbmOld;
			for(i=0;i<fil;i++)
				for(j=0;j<col;j++){
					if(tablero[i][j]<100){
						hbmOld = SelectObject(hdcMem, imagenes[tablero[i][j]]);
						GetObject(imagenes[tablero[i][j]], sizeof(bm), &bm);					
						BitBlt(hdc, j*20, i*20+TY, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);
					}
					{
						HDC hdc2;
						hdc2 = GetDC(hwndButton[i][j].h);
						hbmOld = SelectObject(hdcMem, bboton[hwndButton[i][j].b]);
						GetObject(bboton[hwndButton[i][j].b], sizeof(bm), &bm);
						BitBlt(hdc2, 2, 2, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);						
						ReleaseDC(hwndButton[i][j].h, hdc2);
					}
				}
			{
				HDC hdc2;
				hdc2 = GetDC(cara.h);
				hbmOld = SelectObject(hdcMem, bcara[cara.b]);
				GetObject(bcara[cara.b], sizeof(bm), &bm);
				BitBlt(hdc2, 2, 2, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);						
				ReleaseDC(cara.h, hdc2);
			}								
			if(cb>=0){
				HDC hdc2;
				hdc2 = GetDC(m.h);
				hbmOld = SelectObject(hdcMem, num[0]);
				GetObject(num[0], sizeof(bm), &bm);
				BitBlt(hdc2, 3, 8, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);						
				hbmOld = SelectObject(hdcMem, num[cb/10]);
				GetObject(num[cb/10], sizeof(bm), &bm);
				BitBlt(hdc2, 27, 8, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);						
				hbmOld = SelectObject(hdcMem, num[cb%10]);
				GetObject(num[cb%10], sizeof(bm), &bm);
				BitBlt(hdc2, 27+24, 8, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);						
				ReleaseDC(m.h, hdc2);				
			}
			{
				HDC hdc2;
				hdc2 = GetDC(r.h);
				hbmOld = SelectObject(hdcMem, num[tiempo/100]);
				GetObject(num[tiempo/100], sizeof(bm), &bm);
				BitBlt(hdc2, 3, 8, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);						
				hbmOld = SelectObject(hdcMem, num[(tiempo%100)/10]);
				GetObject(num[(tiempo%100)/10], sizeof(bm), &bm);
				BitBlt(hdc2, 27, 8, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);						
				hbmOld = SelectObject(hdcMem, num[(tiempo%100)%10]);
				GetObject(num[(tiempo%100)%10], sizeof(bm), &bm);
				BitBlt(hdc2, 27+24, 8, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);						
				ReleaseDC(r.h, hdc2);				
			}			
			SelectObject(hdcMem, hbmOld);
			DeleteDC(hdcMem);
		}
		EndPaint (hwnd, &ps);
		return 0 ;
	}
	case WM_COMMAND :{
		switch(LOWORD(wParam)){
		case ID_NUEVO:{
			//detiene el timer del reloj
			KillTimer(hwnd,1);
			
			//inicializa las variables dependiendo del nivel
			t=0;
			cara.b=0;
			ca=0;
			tiempo=0;
			switch(n){
			case 1:
				cminas=10;
				cb=10;
				fil=8;
				col=8;
				break;			
			case 2:
				cminas=40;
				cb=40;
				fil=16;
				col=16;
				break;			
			case 3:
				cminas=99;
				cb=99;
				fil=16;
				col=30;
				break;			
			}	
			
			//muestra los botones necesarios			
			for(i=0;i<16;i++)
				for(j=0;j<30;j++){
					if((i<fil)&&(j<col)){
						ShowWindow (hwndButton[i][j].h, 1) ;
					}else
						ShowWindow (hwndButton[i][j].h, 0) ;						
					hwndButton[i][j].b=0;
				}
			break;
		}
		case ID_PRINCIPIANTE:{
			RECT rect2;
			GetWindowRect (hwnd, &rect2) ;
			n=1;
			SetWindowPos(cara.h,NULL,88,2,35,35,0);
			SetWindowPos(r.h,NULL,127,-9,80,48,0);
			SetWindowPos(hwnd,NULL,rect2.left, rect2.top,210,250,0);
			WndProc (hwnd,WM_COMMAND,ID_NUEVO,0);
			break;
		}
		case ID_INTERMEDIO:{
			RECT rect2;
			GetWindowRect (hwnd, &rect2) ;
			n=2;
			SetWindowPos(cara.h,NULL,148,2,35,35,0);
			SetWindowPos(r.h,NULL,247,-9,80,48,0);
			SetWindowPos(hwnd,NULL,rect2.left, rect2.top,330,410,0);		
			WndProc (hwnd,WM_COMMAND,ID_NUEVO,0);
			break;
		}
		case ID_EXPERTO:{
			RECT rect2;
			GetWindowRect (hwnd, &rect2) ;
			n=3;
			SetWindowPos(cara.h,NULL,287,2,35,35,0);
			SetWindowPos(r.h,NULL,525,-9,80,48,0);		
			SetWindowPos(hwnd,NULL,rect2.left, rect2.top,608,408,0);
			WndProc (hwnd,WM_COMMAND,ID_NUEVO,0);
			break;		
		}
		case ID_SALIR:
			exit(0);
			break;
		case ID_ACERCADE:
			DialogBox (hInstance, TEXT ("AboutBox"),hwnd,AboutDlgProc);
			break;		
		default:{
			char s[50];
			int y=LOWORD (wParam)/100;
			int x=LOWORD (wParam)%100;
			
			//si termino el juego
			if(t==2)break;
			
			//si es el primer movimiento carga el tablero
			//hasta que no haya una mina en la posición seleccionada
			for(;!t;tiempo=0){
				inicio(cminas,fil,col);
				if(tablero[y][x]!=109)t=1;
				SetTimer (hwnd, 1, 1000, NULL);
			}
			
			//si no hay una bandera en el boton
			if(!hwndButton[y][x].b){
				tablero[y][x]-=100;
				
				//si se apreto sobre una mina
				if(tablero[y][x]==9){
					int bd=0;
					tablero[y][x]=10;
					t=2;
					cara.b=1;
					KillTimer(hwnd,1);
					for(i=0;i<fil;i++)
						for(j=0;j<col;j++){
							if(tablero[i][j]==109&&!hwndButton[i][j].b){
								tablero[i][j]=9;
								ShowWindow (hwndButton[i][j].h,0);
							}
							if(hwndButton[i][j].b==1&&tablero[i][j]!=9&&tablero[i][j]!=109){
								tablero[i][j]=11;
								ShowWindow (hwndButton[i][j].h,0);
							}
							if(tablero[i][j]==109&&hwndButton[i][j].b)bd++;
						}
					ShowWindow (hwndButton[y][x].h, 0) ;
					hwndButton[y][x].b=2;
					WndProc (hwnd,WM_PAINT, 0,0);
					sprintf(s,"Casillas destapadas:%d\nBombas detectadas:%d",ca,bd);
					MessageBox(hwnd, s, "Fin del Juego", MB_OK | MB_ICONINFORMATION);	
				}else{
					ca++;
				}
				
				//oculta el boton
				ShowWindow (hwndButton[y][x].h, 0) ;
				hwndButton[y][x].b=2;
				
				//si se selecciono un lugar en blanco
				if(!tablero[y][x]){
					int ban;
					do{
						ban=0;
						for(i=0;i<fil;i++)
							for(j=0;j<col;j++){
								//si la posicion esta en blanco revisa si debe abrir las posiciones
								//de los alrededores y si es asi activa la bandera 
								if(!tablero[i][j]){
									if(i>0&&!hwndButton[i-1][j].b){tablero[i-1][j]-=100;hwndButton[i-1][j].b=2;ShowWindow(hwndButton[i-1][j].h,0);ban=1;ca++;}
									if(j>0&&!hwndButton[i][j-1].b){tablero[i][j-1]-=100;hwndButton[i][j-1].b=2;ShowWindow(hwndButton[i][j-1].h,0);ban=1;ca++;}
									if(i<fil-1&&!hwndButton[i+1][j].b){tablero[i+1][j]-=100;hwndButton[i+1][j].b=2;ShowWindow(hwndButton[i+1][j].h,0);ban=1;ca++;}
									if(j<col-1&&!hwndButton[i][j+1].b){tablero[i][j+1]-=100;hwndButton[i][j+1].b=2;ShowWindow(hwndButton[i][j+1].h,0);ban=1;ca++;}
									if((i>0)&&(j>0)&&!hwndButton[i-1][j-1].b){tablero[i-1][j-1]-=100;hwndButton[i-1][j-1].b=2;ShowWindow(hwndButton[i-1][j-1].h,0);ban=1;ca++;}					
									if((i>0)&&(j<col-1)&&!hwndButton[i-1][j+1].b){tablero[i-1][j+1]-=100;hwndButton[i-1][j+1].b=2;ShowWindow(hwndButton[i-1][j+1].h,0);ban=1;ca++;}					
									if((i<fil-1)&&(j>0)&&!hwndButton[i+1][j-1].b){tablero[i+1][j-1]-=100;hwndButton[i+1][j-1].b=2;ShowWindow(hwndButton[i+1][j-1].h,0);ban=1;ca++;}					
									if((i<fil-1)&&(j<col-1)&&!hwndButton[i+1][j+1].b){tablero[i+1][j+1]-=100;hwndButton[i+1][j+1].b=2;ShowWindow(hwndButton[i+1][j+1].h,0);ban=1;ca++;}										
								}
							}
					}while(ban);					
				}
			}
			
			//si gano el juego
			if(ca==fil*col-cminas){
				t=2;
				cara.b=2;
				for(i=0;i<fil;i++)
					for(j=0;j<col;j++){
						if(!hwndButton[i][j].b)hwndButton[i][j].b=1;
					}
				WndProc (hwnd,WM_PAINT, 0,0);
				KillTimer(hwnd,1);	
				sprintf(s,"Casillas destapadas:%d\nBombas detectadas:%d",ca,cminas);
				MessageBox(hwnd, s, "Fin del Juego", MB_OK | MB_ICONINFORMATION);	
			}
		}
		}
		WndProc (hwnd,WM_PAINT, 0,0);
		break;
	}
	case WM_DESTROY :
		PostQuitMessage(0);
		return 0;
	case WM_MBUTTONDOWN:{//boton del medio
		int x=LOWORD (lParam)/20;
		int y=(HIWORD (lParam)-TY)/20;
		int c=0;
		if(y>0&&hwndButton[y-1][x].b==1)c++;
		if(x>0&&hwndButton[y][x-1].b==1)c++;
		if(y<fil-1&&hwndButton[y+1][x].b==1)c++;
		if(x<col-1&&hwndButton[y][x+1].b==1)c++;
		if((y>0)&&(x>0)&&hwndButton[y-1][x-1].b==1)c++;					
		if((y>0)&&(x<col-1)&&hwndButton[y-1][x+1].b==1)c++;					
		if((y<fil-1)&&(x>0)&&hwndButton[y+1][x-1].b==1)c++;					
		if((y<fil-1)&&(x<col-1)&&hwndButton[y+1][x+1].b==1)c++;
		if(c==tablero[y][x]&&c){
			if(y>0&&!hwndButton[y-1][x].b){WndProc(hwnd,WM_COMMAND,((y-1)*100+x),0);}
			if(x>0&&!hwndButton[y][x-1].b){WndProc(hwnd,WM_COMMAND,(y*100+x-1),1);}
			if(y<fil-1&&!hwndButton[y+1][x].b){WndProc(hwnd,WM_COMMAND,((y+1)*100+x),2);}
			if(x<col-1&&!hwndButton[y][x+1].b){WndProc(hwnd,WM_COMMAND,(y*100+x+1),3);}
			if((y>0)&&(x>0)&&!hwndButton[y-1][x-1].b){WndProc(hwnd,WM_COMMAND,((y-1)*100+x-1),4);}					
			if((y>0)&&(x<col-1)&&!hwndButton[y-1][x+1].b){WndProc(hwnd,WM_COMMAND,((y-1)*100+x+1),5);}				
			if((y<fil-1)&&(x>0)&&!hwndButton[y+1][x-1].b){WndProc(hwnd,WM_COMMAND,((y+1)*100+x-1),6);}					
			if((y<fil-1)&&(x<col-1)&&!hwndButton[y+1][x+1].b){WndProc(hwnd,WM_COMMAND,((y+1)*100+x+1),7);}					
		}			
		break;
	}
	
	case WM_PARENTNOTIFY:{
		BITMAP bm;
		//si se termino el juego
		if(t==2)break;
			
		//click derecho sobre un boton
		if(wParam==516){
			int x=LOWORD (lParam)/20;
			int y=(HIWORD (lParam)-TY)/20;
			if((HIWORD (lParam)-TY)<0)break;
			hwndButton[y][x].b=!hwndButton[y][x].b;
			if(hwndButton[y][x].b){
				cb--;
			}else{
				cb++;
			}	
			WndProc (hwnd,WM_PAINT, 0,0);
		}
		break;
	}

	case WM_LBUTTONDOWN:{
		//dos botones juntos(primero derecho, luego izquierdo)
		if(wParam==3)
			WndProc (hwnd,WM_MBUTTONDOWN,wParam,lParam);
		break;	
	}
	case WM_RBUTTONDOWN:
		//dos botones juntos(primero izquierdo, luego derecho)
		if(wParam==3)
			WndProc (hwnd,WM_MBUTTONDOWN,wParam,lParam);
		break;
	case WM_TIMER:
		if(wParam==1){
			tiempo++;
			if(tiempo==999){
				int i,j;
				for(i=0;i<fil;i++)
					for(j=0;j<col;j++){
						if(tablero[i][j]==109){
							tablero[i][j]=10;
							ShowWindow(hwndButton[i][j].h,0);							
						}else{
							if(hwndButton[i][j].b==1){
								tablero[i][j]=11;
								ShowWindow(hwndButton[i][j].h,0);
							}
						}
					}
				KillTimer(hwnd,1);
				WndProc (hwnd,WM_PAINT, 0,0);
				MessageBox(hwnd, "Te has quedado sin tiempo", "Fin del Juego", MB_OK | MB_ICONINFORMATION);		
			}
		}
		if(wParam==2){
			WndProc (hwnd,WM_PAINT, 0,0);
		}
		break;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}

BOOL CALLBACK AboutDlgProc (HWND hDlg, UINT message, 
                            WPARAM wParam, LPARAM lParam)
{
     switch (message)
     {
     case WM_INITDIALOG :
          return TRUE ;
     case WM_COMMAND :
          switch (LOWORD (wParam))
          {
          case IDOK :
          case IDCANCEL :
               EndDialog (hDlg, 0) ;
               return TRUE ;
          }
          break ;
     }
     return FALSE ;
}
