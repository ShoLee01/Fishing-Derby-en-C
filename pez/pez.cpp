#include <iostream>
#include <conio.h>
#include <time.h>
#include <Windows.h>
#include <chrono>

using namespace std;
using namespace std::chrono_literals;

//Bloques::  t:todo d=abajo c=centro a=arriba e:espacio
const int t = 219, d = 220, c = 254, a = 223, e = 32;
//Teclas direccionales
const int ARRIBA = 72, ABAJO = 80, IZQUIERDA = 75, DERECHA = 77, ESPACIO = 32, ACELERAR = 119, ACELERABAJADA = 115;
/******Dibujando el pez*******/
int length_fish = 2;
int pezd1[2][8] = { {a,c,d,t,t,t,c,d},{d,c,a,t,t,t,c,a} };//parte superior del pez a la derecha
int pezd2[2][8] = { {c,c,d,t,t,t,c,d},{c,c,a,t,t,t,c,a} };//parte inferior del pez a la derecha
int pezI1[2][8] = { {d,c,t,t,t,d,c,a},{a,c,t,t,t,a,c,d} };//parte superior del pez a la izquierda
int pezI2[2][8] = { {d,c,t,t,t,d,c,c},{a,c,t,t,t,a,c,c} };//parte inferior del pez a la izquierda
/*****Dibujando el tiburon*****/
int length_shark = 8;
int TiburonD1[3][17] = { {a,c,d,d,e,e,e,a,c,d,d,e,e,e,e,e,e}, {e,e,e,a,t,t,t,t,t,t,t,t,t,t,t,t,a}, {e,e,d,c,a,e,e,e,c,a,e,e,e,e,e,e,e} }; //Tiburon derecha 1
int TiburonD2[3][17] = { {a,c,d,d,e,e,e,a,c,d,d,e,e,e,e,e,e}, {e,e,e,a,t,t,t,t,t,t,t,t,t,t,a,a,a}, {e,e,d,c,a,e,e,e,c,a,e,e,e,e,a,a,e} }; //Tiburon derecha 2
int TiburonI1[3][17] = { {e,e,e,e,e,e,d,d,c,a,e,e,e,d,d,c,a}, {a,t,t,t,t,t,t,t,t,t,t,t,t,a,e,e,e}, {e,e,e,e,e,e,e,a,c,e,e,e,a,c,d,e,e} }; //Tiburon izquierda 1
int TiburonI2[3][17] = { {e,e,e,e,e,e,d,d,c,a,e,e,e,d,d,c,a}, {a,a,a,t,t,t,t,t,t,t,t,t,t,a,e,e,e}, {e,a,a,e,e,e,e,a,c,e,e,e,a,c,d,e,e} }; //Tiburon izquierda 1
//Obtener el ID del console windows
HWND console_handle = GetConsoleWindow();

//Funciones Generales____________________________________________________________________________________________________
void Consola(); void gotoxy(int x, int y); void color(int color); void b(int b, int x); void hora(int segundos);//Tiempo
//_____________________________________Gráficos______________________________________
void BlockPaint(int x, int y, int i, int j, int asc, COLORREF colores, int escala);
void line(int x1, int y1, int x2, int y2, COLORREF colores, int grosor);
void circle(int x, int y, int r, COLORREF colores);
void rectangulo(int x, int y, int col, int filas, int columnas);
void titulo(int x, int y, int col); void ganaste(int x, int y, int col);
void perdiste(int x, int y, int col); void LogoPez(int x, int y, int col);
void LogoPezI(int x, int y, int col); void alga1(int x, int y, int col);
void alga2(int x, int y, int col); void alga3(int x, int y, int col);
void columnas(int i); void Muelles(); void jugador(); //Graficos del Escenario Juego
//Escenarios_________________________________________________________________________________________________
void Inicio(); void Creditos(); void Reglas(); void B_Ganaste(); void B_Perdiste(); void EscenarioJuego();
//___________________________________________________________________________________JUEGO______________________________________________________________________________________
//Pez
void Borrar_Pez(int x, int y, int escala = 8); void PezI(int x, int y, bool e, int escala, COLORREF colores); void PezD(int x, int y, bool e, int escala, COLORREF colores);
void Pez(int x, int y, COLORREF colores, bool inv, bool e, int escala = 8);
//Tiburon
void TiburonD(int x, int y, bool e, int escala, COLORREF colores); void TiburonI(int x, int y, bool e, int escala, COLORREF colores); void Borrar_Tiburon(int x, int y, int escala = 8);
void Tiburon(int x, int y, COLORREF colores, bool inv, bool e, int escala = 8);
//Movimientos
void GenerarPosicion_Marinos(int* x, int* x1, bool* invP);
void Mover_tiburon(int* x, bool* invP);
void Mover_peces(int* x, bool* invP, int l = 0);
void Mover_Objetos(bool& gameover, int& puntos, int& vidas);
//**************buscador de colision************************
int matchpez(int xc, int yc, int* x, int y[], bool* Pinv);
//****************Verifica las vidas************************
void Verificar_Vidas(bool& gameover, int& vidas);
//____________________________________________________________________________________________MENU_____________________________________________________________________________
void Menu();

int main() {
	// bloquea la interacción del mouse en la pantalla
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD prev_mode;
	GetConsoleMode(hInput, &prev_mode);
	SetConsoleMode(hInput, prev_mode & ~ENABLE_QUICK_EDIT_MODE);
	//Bloque la redimensión de la consola del juego
	SetWindowLong(console_handle, GWL_STYLE, GetWindowLong(console_handle, GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);

	Menu();
	_getch();
	return 0;
}

//____________________________________________________________________Funciones Generales________________________________________________________________________________________
void Consola() {
	//Cambiando titulo de consola
	system("title FISHING DERBY");
	//Tamaño pantalla
	system("mode con: cols=100 lines=40");
	//Color por defecto
	system("color 1F");
	//Ocultar cursor
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO inf = { 1 , FALSE };
	SetConsoleCursorInfo(h, &inf);
}

void gotoxy(int x, int y) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x , y };
	SetConsoleCursorPosition(h, c);
}

void color(int color) {
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, color);
}

void b(int b, int x) {
	for (int i = 1; i <= b; i++) {
		cout << char(x);
	}
}

void hora(int segundos)
{
	int actual = 600; actual -= segundos;
	color(180);  gotoxy(48, 1);
	if (actual / 60 > 10)cout << actual / 60 << ":";
	else cout << " " << actual / 60 << ":";
	if ((actual % 60) > 9)cout << actual % 60;
	else cout << "0" << actual % 60;
}

//_____________________________________Gráficos______________________________________
void BlockPaint(int x, int y, int i, int j, int asc, COLORREF colores, int escala)
{
	int posx = x + (escala)* i, posy = y + (2 * escala) * j;
	HDC device_context = GetDC(console_handle);
	if (asc == e) {
		HBRUSH relleno = CreateSolidBrush(RGB(0, 55, 218));
		HPEN borde = CreatePen(PS_SOLID, 0, RGB(0, 55, 218));
		SelectObject(device_context, relleno);
		SelectObject(device_context, borde);
		Rectangle(device_context, posx, posy, posx + escala, posy + 2 * escala);
		ReleaseDC(console_handle, device_context);
		DeleteDC(device_context);
		DeleteObject(relleno);
		DeleteObject(borde);
	}
	else {
		HBRUSH relleno = CreateSolidBrush(colores);
		HPEN borde = CreatePen(PS_SOLID, 0, colores);
		SelectObject(device_context, relleno);
		SelectObject(device_context, borde);
		switch (asc)
		{
		case t: Rectangle(device_context, posx, posy, posx + escala, posy + 2 * escala); break;
		case d: Rectangle(device_context, posx, posy + escala, posx + escala, posy + 2 * escala); break;
		case a: Rectangle(device_context, posx, posy, posx + escala, posy + escala); break;
		case c: Rectangle(device_context, posx, posy + (5 * escala) / 8, posx + escala, posy + (13 * escala) / 8); break;
		}
		ReleaseDC(console_handle, device_context);
		DeleteDC(device_context);
		DeleteObject(relleno);
		DeleteObject(borde);
	}
}

void line(int x1, int y1, int x2, int y2, COLORREF colores, int grosor)
{
	HDC device_context = GetDC(console_handle);
	HPEN pen = CreatePen(PS_SOLID, grosor, colores);
	SelectObject(device_context, pen);
	MoveToEx(device_context, (int)x1, (int)y1, (LPPOINT)NULL);
	LineTo(device_context, (int)x2, (int)y2);
	ReleaseDC(console_handle, device_context);
	DeleteDC(device_context);
	DeleteObject(pen);
}

void circle(int x, int y, int r, COLORREF colores)
{
	HDC consoleDC = GetDC(console_handle);
	HBRUSH relleno = CreateSolidBrush(colores);
	HPEN borde = CreatePen(PS_SOLID, 0, colores);
	SelectObject(consoleDC, relleno);
	SelectObject(consoleDC, borde);
	Ellipse(consoleDC, x - r, y - r, x + r, y + r);
	ReleaseDC(console_handle, consoleDC);
	DeleteObject(borde);
	DeleteObject(relleno);
	DeleteDC(consoleDC);
}

void rectangulo(int x, int y, int col, int filas, int columnas) {
	gotoxy(x, y); color(col);
	for (int i = 1; i <= filas; i++) {
		for (int j = 1; j <= columnas; j++) {
			b(1, e);
		}
		gotoxy(x, y + i);
	}

}

void titulo(int x, int y, int col) {
	color(col);
	gotoxy(x, y); b(3, t); b(1, e); b(1, t); b(1, e); b(3, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(3, e); b(1, t); b(1, e); b(4, t); b(4, e); b(2, t); b(2, e); b(3, t); b(1, e); b(2, t); b(3, e); b(3, t); b(1, e); b(1, t); b(3, e); b(1, t);
	gotoxy(x, y + 1); b(1, t); b(3, e); b(1, t); b(1, e); b(1, t); b(3, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(2, t); b(2, e); b(1, t); b(1, e); b(1, t); b(7, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(3, e); b(1, t); b(1, e); b(1, t); b(2, e); b(1, t); b(1, e); b(1, t); b(2, e); b(1, t); b(1, e); b(1, t);
	gotoxy(x, y + 2); b(2, t); b(2, e); b(1, t); b(1, e); b(3, t); b(1, e); b(3, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(2, t); b(4, e); b(1, t); b(1, e); b(1, t); b(1, e); b(2, t); b(2, e); b(2, t); b(3, e); b(4, t); b(2, e); b(1, t);
	gotoxy(x, y + 3); b(1, t); b(3, e); b(1, t); b(3, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(2, e); b(2, t); b(1, e); b(1, t); b(2, e); b(1, t); b(4, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(3, e); b(1, t); b(1, e); b(1, t); b(2, e); b(1, t); b(2, e); b(1, t); b(2, e); b(1, t);
	gotoxy(x, y + 4); b(1, t); b(3, e); b(1, t); b(1, e); b(3, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(3, e); b(1, t); b(1, e); b(4, t); b(4, e); b(2, t); b(2, e); b(3, t); b(1, e); b(1, t); b(2, e); b(1, t); b(1, e); b(4, t); b(2, e); b(1, t);

}

void ganaste(int x, int y, int col) {
	color(col);
	gotoxy(x, y); b(3, e); b(1, t); b(12, e); b(1, t); b(4, e); b(1, t); b(7, e); b(1, t);
	gotoxy(x, y + 1); b(4, t); b(11, e); b(3, t); b(1, e); b(3, t); b(5, e); b(3, t);
	gotoxy(x, y + 2); b(1, t); b(5, e); b(1, t); b(2, e); b(1, t); b(3, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(3, e); b(3, t); b(1, e); b(1, t);
	gotoxy(x, y + 3); b(1, t); b(1, e); b(2, t); b(1, e); b(3, t); b(1, e); b(2, t); b(2, e); b(1, t); b(1, e); b(3, t); b(1, e); b(3, t); b(2, e); b(1, t); b(2, e); b(2, t);
	gotoxy(x, y + 4); b(1, t); b(2, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(3, e); b(1, t); b(2, e); b(1, t); b(2, e); b(1, t);
	gotoxy(x, y + 5); b(4, t); b(1, e); b(3, t); b(1, e); b(1, t); b(2, e); b(2, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(3, t); b(2, e); b(1, t); b(2, e); b(3, t);
	gotoxy(x, y + 6); b(3, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(3, e); b(1, t); b(5, e); b(1, t); b(4, e); b(1, t); b(4, e); b(1, t);
	gotoxy(x, y + 7); b(2, e); b(2, t); b(1, e); b(1, t); b(1, e); b(1, t); b(16, e); b(1, t);
}

void perdiste(int x, int y, int col) {
	color(col);
	gotoxy(x, y); b(4, e); b(1, t); b(18, e); b(1, t);
	gotoxy(x, y + 1); b(1, e); b(4, t); b(14, e); b(1, t); b(1, e); b(3, t);
	gotoxy(x, y + 2); b(1, e); b(1, t); b(2, e); b(1, t); b(1, e); b(3, t); b(6, e); b(2, t); b(4, e); b(1, t); b(3, e); b(5, t); b(1, e); b(3, t);
	gotoxy(x, y + 3); b(1, e); b(4, t); b(1, e); b(1, t); b(3, e); b(3, t); b(2, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(3, t); b(3, e); b(1, t); b(3, e); b(1, t);
	gotoxy(x, y + 4); b(1, e); b(1, t); b(4, e); b(2, t); b(2, e); b(1, t); b(1, e); b(1, t); b(2, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(3, e); b(1, t); b(3, e); b(1, t); b(3, e); b(2, t);
	gotoxy(x, y + 5); b(1, e); b(1, t); b(4, e); b(1, t); b(3, e); b(2, t); b(3, e); b(1, t); b(1, e); b(1, t); b(1, e); b(1, t); b(1, e); b(3, t); b(3, e); b(1, t); b(3, e); b(1, t);
	gotoxy(x, y + 6); b(1, e); b(1, t); b(4, e); b(3, t); b(1, e); b(1, t); b(1, e); b(1, t); b(2, e); b(2, t); b(2, e); b(1, t); b(1, e); b(1, t); b(5, e); b(1, t); b(3, e); b(3, t);
	gotoxy(x, y + 7);  b(2, t); b(8, e); b(1, t); b(2, e); b(1, t); b(5, e); b(1, t); b(7, e); b(1, t);
}

void LogoPez(int x, int y, int col) {
	color(col);
	gotoxy(x, y); b(1, a); b(1, c); b(1, d); b(3, e); b(1, d); b(7, t); b(1, d);
	gotoxy(x, y + 1); b(2, e); b(1, a); b(1, c); b(1, d); b(8, t); b(1, e); b(2, t); b(1, d);
	gotoxy(x, y + 2); b(2, e); b(1, d); b(1, c); b(1, a); b(4, t); b(2, 178); b(5, t); b(1, a);
	gotoxy(x, y + 3); b(1, d); b(1, c); b(1, a); b(3, e); b(1, a); b(7, t); b(1, a);
}

void LogoPezI(int x, int y, int col) {
	color(col);
	gotoxy(x, y); b(2, e); b(1, d); b(7, t); b(1, d); b(3, e); b(1, d); b(1, c); b(1, a);
	gotoxy(x, y + 1); b(1, d); b(2, t); b(1, e); b(8, t); b(1, d); b(1, c); b(1, a);
	gotoxy(x, y + 2); b(1, a); b(5, t); b(2, 178); b(4, t); b(1, a); b(1, c); b(1, d);
	gotoxy(x, y + 3); b(2, e); b(1, a); b(7, t); b(1, a); b(3, e); b(1, a); b(1, c); b(1, d);
}

void alga1(int x, int y, int col) {
	color(col); //18
	gotoxy(x, y); b(5, e); b(1, t);
	gotoxy(x, y + 1); b(1, e); b(1, t); b(1, e); b(1, t); b(2, e); b(1, t);
	gotoxy(x, y + 2); b(1, t); b(3, e); b(1, t); b(2, e); b(1, t); b(1, e); b(1, t);
	gotoxy(x, y + 3); b(1, e); b(1, t); b(1, e); b(1, t); b(4, e); b(1, t);
	gotoxy(x, y + 4); b(2, e); b(1, t); b(4, e); b(1, t);
	gotoxy(x, y + 5); b(3, e); b(2, t); b(1, e); b(1, t);
	gotoxy(x, y + 6); b(4, e); b(2, t);
	gotoxy(x, y + 7); b(4, e); b(3, t);
	rectangulo(x + 5, y + 8, 34, 1, 3);
}

void alga2(int x, int y, int col) {
	rectangulo(x + 6, y - 1, 34, 1, 1);
	color(col);
	gotoxy(x, y); b(4, e); b(1, t); b(2, e); b(1, t);
	gotoxy(x, y + 1); b(1, t); b(2, e); b(1, t); b(2, e); b(1, t);
	gotoxy(x, y + 2); b(1, t); b(1, e); b(1, t); b(2, e); b(1, t);
	gotoxy(x, y + 3); b(1, e); b(1, t); b(2, e); b(1, t);
	gotoxy(x, y + 4); b(2, e); b(3, t);
	rectangulo(x + 1, y + 5, 34, 1, 3);
}

void alga3(int x, int y, int col) {
	color(col);//18
	gotoxy(x, y + 2); b(1, t);
	gotoxy(x, y + 3); b(1, e); b(1, t); b(1, e); b(1, t); b(4, e); b(1, t);
	gotoxy(x, y + 4); b(2, e); b(1, t); b(4, e); b(1, t);
	gotoxy(x, y + 5); b(3, e); b(2, t); b(1, e); b(1, t);
	gotoxy(x, y + 6); b(4, e); b(2, t);
	gotoxy(x, y + 7); b(4, e); b(3, t);
	rectangulo(x + 5, y + 8, 34, 1, 3);
}

void columnas(int i) {
	for (int j = 1; j <= 15; j++) {
		if ((j >= 4 && j <= 6) || (j >= 10 && j <= 12)) {
			if (i == 1) b(1, a);
			else b(1, e);
		}
		else b(1, t);
	}
}

void Muelles() {
	int y = 11;
	for (int i = 1; i <= 29; i++) {
		gotoxy(0, y);
		if (i == 1) color(186);
		else color(18);
		columnas(i); gotoxy(85, y); columnas(i); y++;
	}
}

void jugador() {
	gotoxy(0, 6); color(176); b(1, e); b(1, t); b(2, d); b(1, t); b(2, d);
	gotoxy(0, 7); color(188); b(2, e); b(3, t); b(1, c); b(2, e); b(1, d); color(176); b(7, d);
	gotoxy(0, 8); color(180); b(1, e); b(3, t); b(2, c); b(2, a);
	gotoxy(0, 9); b(1, e); b(3, t); b(1, e); b(1, t); b(1, a); b(1, t);
	gotoxy(0, 10); b(1, e); b(3, t); b(1, a); b(2, e); b(1, t); b(1, d);
}

//_____________________________________________________________________Escenarios_______________________________________________________________________________________________
void Inicio() {
	rectangulo(0, 0, 17, 18, 100);
	rectangulo(0, 18, 238, 22, 100);
	LogoPez(11, 3, 22);
	LogoPezI(71, 9, 22);
	alga1(4, 10, 18);
	alga2(86, 13, 18);
	titulo(24, 21, 228);
	rectangulo(36, 28, 153, 3, 28);
	rectangulo(36, 34, 167, 3, 28);
	rectangulo(36, 37, 34, 1, 28);
	rectangulo(8, 34, 204, 3, 16);
	rectangulo(76, 34, 204, 3, 16);
	color(225); gotoxy(24, 32); cout << char(196) << char(62) << " Presione la tecla espacio para comenzar a jugar";
	color(148); gotoxy(48, 29); cout << "JUGAR"; //144 negro
	color(160); gotoxy(44, 34); cout << "REALIZADO EN";
	gotoxy(39, 35); cout << "Universidad Peruana de";
	gotoxy(41, 36); cout << "Ciencias Aplicadas";
	color(192); gotoxy(11, 35); cout << "C" << char(224) << "MO JUGAR";
	gotoxy(80, 35); cout << "CR" << char(144) << "DITOS";
	color(236); gotoxy(2, 38); cout << "Presione la tecla izquierda";
	gotoxy(71, 38); cout << "Presione la tecla derecha";
	circle(32 * 8, 4 * 16, 10, RGB(122, 232, 245)); circle(37 * 8, 2 * 16, 20, RGB(122, 232, 245));
	circle(60 * 8, 8 * 16, 20, RGB(122, 232, 245)); circle(67 * 8, 10 * 16, 10, RGB(122, 232, 245));
}

void Creditos() {
	rectangulo(0, 18, 238, 22, 100);
	LogoPez(11, 3, 22);
	LogoPezI(71, 9, 22);
	alga1(4, 10, 18);
	alga2(86, 13, 18);
	titulo(24, 21, 228);
	rectangulo(36, 28, 153, 3, 28);
	rectangulo(70, 37, 160, 1, 28);
	color(148); gotoxy(46, 29); cout << "CR" << char(144) << "DITOS"; //144 negro
	color(225); gotoxy(33, 32); cout << char(196) << char(62) << " Anghel Melanie Trillo Hern" << char(160) << "ndez";
	gotoxy(33, 33);
	cout << char(196) << char(62) << " Miguel Angel Alfaro Gonzales";
	color(160); gotoxy(76, 37); cout << "VOLVER AL INICIO";
	color(236); gotoxy(70, 38); cout << char(62) << "Presione la tecla izquierda";
	circle(32 * 8, 4 * 16, 10, RGB(122, 232, 245)); circle(37 * 8, 2 * 16, 20, RGB(122, 232, 245));
	circle(60 * 8, 8 * 16, 20, RGB(122, 232, 245)); circle(67 * 8, 10 * 16, 10, RGB(122, 232, 245));
}

void Reglas() {
	rectangulo(0, 32, 17, 6, 100);
	rectangulo(0, 38, 238, 2, 100);
	LogoPez(21, 33, 22);
	alga3(4, 30, 18);
	alga2(86, 33, 18);
	rectangulo(0, 0, 238, 32, 100);
	rectangulo(36, 7, 153, 1, 28);
	rectangulo(56, 36, 170, 1, 28);
	titulo(24, 1, 228);
	color(160); gotoxy(62, 36); cout << "VOLVER AL INICIO";
	color(20); gotoxy(56, 37); cout << char(62) << "Presione la tecla izquierda";
	color(148); gotoxy(45, 7); cout << "C" << char(224) << "MO JUGAR"; //144 negro
	//REGLAS DE JUEGO
	color(224); gotoxy(0, 8);
	cout << "  DESCRIPCI" << char(224) << "N DEL JUEGO " << endl <<
		"   En este juego de pesca solo existe un jugador que se encuentra sentado en un lado de los" << endl <<
		"   muelles sobre un lago lleno de peces y un tibur" << char(162) << "n que pasa a trav" << char(130) << "s de este. Cuando un pez" << endl <<
		"   est" << char(160) << " enganchado, la l" << char(161) << "nea sube lentamente a la superficie del agua. El tibur" << char(162) << "n que se" << endl <<
		"   desplaza por el agua intentar" << char(160) << " comer los peces enganchados antes de que salgan a la superficie." << endl <<
		"  REGLAS" << endl <<
		"   Solo existe un jugador. Al inicio del juego, el jugador tiene 3 vidas para lograr el" << endl <<
		"   objetivo de cumplir los 3 niveles. Tome en cuenta que para cada nivel usted cuenta con" << endl <<
		"   un tiempo para lograr el objetivo. Cuando no logra recolectar las 100 libras en el tiempo" << endl <<
		"   fijado, el jugador pierde una vida y vuelve a iniciar el juego en el nivel que se encuentre." << endl <<
		"   La l" << char(161) << "nea se desplaza de forma vertical y horizontal con las teclas direccionales arriba, abajo," << endl <<
		"   izquierda y derecha. Al presionar la tecla w aumenta la velocidad con la que la l" << char(161) << "nea sube " << endl <<
		"   y al presionar la tecla s aumenta la velocidad con la que la l" << char(161) << "nea baja. " << endl <<
		"  OBJETIVO DEL JUEGO POR NIVEL" << endl <<
		"   Llegar a las 100 libras de pescado antes que termine el tiempo." << endl <<
		"  VALOR DE LOS PECES" << endl <<
		"   Los peces de las dos filas superiores tienen 2 libras de peso. Los peces de las filas del" << endl <<
		"   medio tienen 4 libras de peso. Por " << char(163) << "ltimo, los peces de las dos filas inferiores tienen" << endl <<
		"   6 libras de peso." << endl <<
		"  C" << char(224) << "MO GANAR" << endl <<
		"   El juego est" << char(160) << " dividido en tres niveles. En cada nivel el grado de dificultad es mayor." << endl <<
		"   Para ganar el juego, el jugador debe lograr terminar los 3 niveles.";
}

void B_Ganaste() {
	rectangulo(0, 0, 17, 18, 100);
	rectangulo(0, 18, 238, 22, 100);
	LogoPez(11, 3, 22);
	LogoPezI(71, 9, 22);
	alga1(4, 10, 18);
	alga2(86, 13, 18);
	titulo(24, 31, 228);
	rectangulo(70, 37, 160, 1, 28);
	ganaste(35, 20, 229);
	color(160); gotoxy(76, 37); cout << "VOLVER AL INICIO";
	color(236); gotoxy(70, 38); cout << char(62) << "Presione la tecla izquierda";
	circle(32 * 8, 4 * 16, 10, RGB(122, 232, 245)); circle(37 * 8, 2 * 16, 20, RGB(122, 232, 245));
	circle(60 * 8, 8 * 16, 20, RGB(122, 232, 245)); circle(67 * 8, 10 * 16, 10, RGB(122, 232, 245));
}

void B_Perdiste() {
	rectangulo(0, 0, 17, 18, 100);
	rectangulo(0, 18, 238, 22, 100);
	LogoPez(11, 3, 22);
	LogoPezI(71, 9, 22);
	alga1(4, 10, 18);
	alga2(86, 13, 18);
	titulo(24, 31, 228);
	rectangulo(70, 37, 160, 1, 28);
	perdiste(33, 20, 229);
	color(160); gotoxy(76, 37); cout << "VOLVER AL INICIO";
	color(236); gotoxy(70, 38); cout << char(62) << "Presione la tecla izquierda";
	circle(32 * 8, 4 * 16, 10, RGB(122, 232, 245)); circle(37 * 8, 2 * 16, 20, RGB(122, 232, 245));
	circle(60 * 8, 8 * 16, 20, RGB(122, 232, 245)); circle(67 * 8, 10 * 16, 10, RGB(122, 232, 245));
}

void EscenarioJuego() {
	system("cls");
	system("color 1F");
	rectangulo(0, 0, 187, 12, 100);
	Muelles();
	jugador();
	gotoxy(2, 1); cout << "PUNTOS 000";
	gotoxy(48, 1); cout << "10:00";
	SetConsoleOutputCP(65001); gotoxy(92, 1); cout << "♥ ♥ ♥"; SetConsoleOutputCP(850);
}

//___________________________________________________________________________________JUEGO______________________________________________________________________________________
//Pez
void Borrar_Pez(int x, int y, int escala) {
	HDC device_context = GetDC(console_handle);
	HBRUSH relleno = CreateSolidBrush(RGB(0, 55, 218)); HPEN borde = CreatePen(PS_SOLID, 0, RGB(0, 55, 218)); //Azul
	SelectObject(device_context, relleno); SelectObject(device_context, borde);
	Rectangle(device_context, x, y, x + 8 * escala, y + 2 * 2 * escala);
	ReleaseDC(console_handle, device_context); DeleteDC(device_context); DeleteObject(relleno); DeleteObject(borde);
}

void PezD(int x, int y, bool e, int escala, COLORREF colores) {
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			if (e == 0) BlockPaint(x, y, i, j, pezd1[j][i], colores, escala);
			else BlockPaint(x, y, i, j, pezd2[j][i], colores, escala);
		}
	}
}

void PezI(int x, int y, bool e, int escala, COLORREF colores) {
	for (int j = 0; j < 2; j++)
	{
		for (int i = 0; i < 8; i++)
		{
			if (e == 0) BlockPaint(x, y, i, j, pezI1[j][i], colores, escala);
			else BlockPaint(x, y, i, j, pezI2[j][i], colores, escala);
		}
	}
}

void Pez(int x, int y, COLORREF colores, bool inv, bool e, int escala) {
	inv ? PezI(x, y, e, escala, colores) : PezD(x, y, e, escala, colores);
}

void TiburonD(int x, int y, bool e, int escala, COLORREF colores) {
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 17; i++)
		{
			if (e == 0) BlockPaint(x, y, i, j, TiburonD1[j][i], colores, escala);
			else BlockPaint(x, y, i, j, TiburonD2[j][i], colores, escala);
		}
	}
}

void TiburonI(int x, int y, bool e, int escala, COLORREF colores) {
	for (int j = 0; j < 3; j++)
	{
		for (int i = 0; i < 17; i++)
		{
			if (e == 0) BlockPaint(x, y, i, j, TiburonI1[j][i], colores, escala);
			else BlockPaint(x, y, i, j, TiburonI2[j][i], colores, escala);
		}
	}
}

void Tiburon(int x, int y, COLORREF colores, bool inv, bool e, int escala) {
	inv ? TiburonI(x, y, e, escala, colores) : TiburonD(x, y, e, escala, colores);
}

void Borrar_Tiburon(int x, int y, int escala) {
	HDC device_context = GetDC(console_handle);
	HBRUSH relleno = CreateSolidBrush(RGB(0, 55, 218)); HPEN borde = CreatePen(PS_SOLID, 0, RGB(0, 55, 218)); //Azul
	SelectObject(device_context, relleno); SelectObject(device_context, borde);
	Rectangle(device_context, x, y, x + 17 * escala, y + 3 * 2 * escala);
	ReleaseDC(console_handle, device_context); DeleteDC(device_context); DeleteObject(relleno); DeleteObject(borde);
}

void GenerarPosicion_Marinos(int* x, int* x1, bool* invP) {
	for (int i = 0; i < 7; i++) {
		x[i] = rand() % 70 + 16; //(85-15)
		invP[i] = bool(rand() % 2);//false = izquierda, true = derecha;
		if (i == 0 && x[i] > (85 - 18))x[i] -= 18;
		if ((x[i] > (85 - 9)))x[i] -= 9;
		x1[i] = x[i];
	}
}

void Mover_peces(int* x, bool* invP, int l)
{
	int c = 2;
	for (int i = 1; i < 7; ++i) {
		if (l != i)(invP[i] ? ++(x[i]) : --(x[i]));
		if (x[i] > (85 - c) + (1 - length_fish)) invP[i] = false;
		else if (x[i] < 16) invP[i] = true;
	}
}

void Mover_tiburon(int* x, bool* invP)
{
	int c = 11;
	invP[0] ? ++(x[0]) : --(x[0]);
	if (x[0] > (85 - c) + (1 - length_shark)) invP[0] = false;
	else if (x[0] < 16) invP[0] = true;
}

void Verificar_Vidas(bool& gameover, int& vidas) {
	if (gameover == true) vidas--; color(187);
	if (vidas == 2) { gotoxy(92, 1); cout << " "; }
	if (vidas == 1) { gotoxy(94, 1); cout << " "; }
	if (vidas == 0) { gotoxy(96, 1); cout << " "; }
	if (vidas > 0) { gameover = false; }
}

int matchpez(int xc, int yc, int* x, int y[], bool* Pinv)
{
	for (int i = 1; i < 7; ++i)
	{
		if ((8 * x[i] <= xc && xc <= (8 * x[i] + 2 * length_fish)) && (16 * y[i] <= yc && yc <= (16 * y[i] + 2 * 2 * length_fish)) && !Pinv[i]) return i;//pez mirando a la izquierda
		else if ((8 * x[i] + 6 * length_fish <= xc && xc <= (8 * x[i] + 8 * length_fish)) && (16 * y[i] <= yc && yc <= (16 * y[i] + 2 * 2 * length_fish)) && Pinv[i])return i;//pez mirando a la derecha
	}
	return 0;
}

void Mover_Objetos(bool& gameover, int& puntos, int& vidas) {
	HDC device_context = GetDC(console_handle);
	//DECLARACION DE VARIABLES
	//****************************************Tiempo******************************************/
	auto tm1 = std::chrono::system_clock::now(); auto tm2 = tm1; int hour = 0; srand(time(NULL));
	/******************************************Caña*******************************************/
	int corx = 71, cory = 124;
	int xc = corx + 120; // xc final de la caña y corx inicio de la caña
	int yc = cory + 68;  // yc final de cuerda y cory inicio de la caña e inicio de la cuerda
	int xcuerda = corx + 120;
	auto t1 = std::chrono::system_clock::now();
	auto t2 = t1;
	int input; //Tecla de movimento de la caña
	/**************************************Peces Y Tiburon*********************************************/
	int* x = new int[7], * x1 = new int[7], k = 0, pesco = 0, cont = 0, yc2, y[] = { 18,22,25,28,31,34,37 };
	bool* invP = new bool[7];
	GenerarPosicion_Marinos(x, x1, invP);
	//Pintar vidas
	gotoxy(92, 1); vidas == 2 ? b(2, 32) : (vidas == 1) ? b(4, 32) : color(187);
	//Bucle juego
	while (!gameover) {
		while (!_kbhit()) {
			tm2 = std::chrono::system_clock::now(); // TIEMPO PC
			gotoxy(9, 1); color(180);
			if (puntos < 10)cout << "00" << puntos; else if (puntos < 100)cout << "0" << puntos; else cout << puntos;
			if (puntos >= 100) { gameover = true; break; }
			if ((int)((tm2 - tm1) / 1s) >= 1)
			{
				hour++;
				if (hour == 601 && puntos < 100)
				{
					gameover = true; color(187);
					if (vidas == 3) { gotoxy(92, 1); cout << " "; }
					if (vidas == 2) { gotoxy(94, 1); cout << " "; }
					hour = 0; break;
				}
				hora(hour); tm1 = tm2;
			}
			/*****************************CAÑA DE PESCAR*************************************/
			line(xc, cory, xcuerda, yc, RGB(255, 214, 214), 1);// se pinta blanco
			line(corx, cory, xc, cory, RGB(0, 0, 0), 7);// se pinta negro
			/********************************ANIMALES MARINOS************************************/
			for (int i = 0; i < 7; ++i) {
				if (i == 0) {
					Borrar_Tiburon(8 * x1[0], 16 * y[i], length_shark);
					Tiburon(8 * x[0], 16 * 18, RGB(30, 0, 26), !invP[0], 0, length_shark);
				}
				else {
					Borrar_Pez(8 * x1[i], 16 * y[i], length_fish);
					Pez(8 * x[i], 16 * y[i], RGB(193, 156, 0), !invP[i], 0, length_fish);
				}
			}
			Sleep(80);
			for (int i = 0; i < 7; ++i) {
				if (i == 0) {
					Borrar_Tiburon(8 * x1[0], 16 * y[i], length_shark);
					Tiburon(8 * x[0], 16 * 18, RGB(30, 0, 26), !invP[0], 1, length_shark);
					x1[0] = x[0];
				}
				else {
					Borrar_Pez(8 * x[i], 16 * y[i], length_fish);
					Pez(8 * x[i], 16 * y[i], RGB(193, 156, 0), !invP[i], 1, length_fish);
					x1[i] = x[i];
				}
			}
			Sleep(80);
			/******************movimiento*****************************/
			if (!bool(pesco))pesco = matchpez(xcuerda, yc, x, y, invP);
			Mover_peces(x, invP, pesco);
			Mover_tiburon(x, invP);
			/****************************************caña de pescar*******************************************/
			t1 = std::chrono::system_clock::now();
			line(xc, cory, xcuerda, yc, RGB(255, 214, 214), 1); // se p
			line(corx, cory, xc, cory, RGB(0, 0, 0), 7); // se pinta negro
			if (((int)((t1 - t2) / 1ms)) > 500 && xcuerda != xc && !(bool(pesco)))
			{
				line(xc, cory, xc + ((xcuerda - xc) * 68) / (yc - cory), cory + 68, RGB(97, 214, 214), 3);// celeste cielo
				line(xc + ((xcuerda - xc) * 68) / (yc - cory), cory + 68, xcuerda, yc, RGB(0, 55, 218), 3);// azul mar
				if (xcuerda > xc && (xcuerda - 10) < xc)xcuerda = xc;
				if (xcuerda > xc) xcuerda -= 10;
				else if (xcuerda < xc) xcuerda += 10;
			}
			else if ((bool(pesco)))
			{
				line(xc, cory, xc + ((xcuerda - xc) * 68) / (yc - cory), cory + 68, RGB(97, 214, 214), 3);// celeste cielo
				line(xc + ((xcuerda - xc) * 68) / (yc - cory), cory + 68, xcuerda, yc, RGB(0, 55, 218), 3);// azul mar
				if (!invP[pesco]) { x[pesco] -= 2; xcuerda = 8 * x[pesco]; ++cont; yc = 16 * y[pesco] + 4; }
				else { x[pesco] += 2; xcuerda = 8 * x[pesco] + 8 * length_fish; yc = 16 * y[pesco] + 4; ++cont; }
				if (x[pesco] > (50 - 2) + (1 - length_fish)) { invP[pesco] = false; --(x[pesco]); xcuerda = 8 * x[pesco]; }
				else if (x[pesco] < 16) { invP[pesco] = true; ++(x[pesco]); xcuerda = 8 * x[pesco] + (8 * length_fish); }
				if (cont > 2) {
					cont = 0; Borrar_Pez(8 * x1[pesco], 16 * y[pesco], length_fish); y[pesco] -= 2; yc = 16 * y[pesco] + 4;
				}
			}
			if ((((x[0] < x[pesco]) && (x[pesco] < (x[0] + 17))) || ((x[0] < (x[pesco] + 8)) && ((x[pesco] + 8) < (x[0] + 17)))) && (((y[0] < y[pesco]) && (y[pesco] < (y[0] + 3))) || ((y[0] < (y[pesco] + 2)) && ((y[pesco] + 2) < (y[0] + 3)))) && bool(pesco)) {
				Borrar_Pez(8 * x1[pesco], 16 * y[pesco], length_fish);
				y[pesco] = 22 + 3 * (pesco - 1);
				x[pesco] = xc / 8;
				x1[pesco] = xc / 8;
				cont = 0;
				pesco = 0;
			}

			if (bool(pesco) && yc < 192) { //cuando el pez sube a la superficie
				y[pesco] = 22 + 3 * (pesco - 1);
				x[pesco] = xc / 8;
				x1[pesco] = xc / 8;
				cont = 0;
				yc = 192;
				puntos += 2 + 2 * ((pesco - 1) / 2);
				pesco = 0;
			}
			/***************************************************************************************************************/
		}
		if (!bool(pesco)) {//bloquea el teclado cuando se atrapa al pez
			if (_kbhit())
			{
				t2 = std::chrono::system_clock::now();
				line(xc, cory, xc + ((xcuerda - xc) * 68) / (yc - cory), cory + 68, RGB(97, 214, 214), 3);// celeste cielo
				line(xc + ((xcuerda - xc) * 68) / (yc - cory), cory + 68, xcuerda, yc, RGB(0, 55, 218), 3);// azul mar
				input = _getch();
				switch (input)
				{
				case ARRIBA: yc -= 15; if (yc < 192) yc = 192; break;//arriba
				case ACELERAR: yc -= 30; if (yc < 192) yc = 192; break;//Acelera hacia arriba
				case IZQUIERDA: line(xc - 10, cory, xc, cory, RGB(97, 214, 214), 7); xc -= 10; if ((xc - 5) < 116) xc = 121; break;//izquierda
				case DERECHA: xc += 10; if (xc > 381) xc = 381; break;//derecha
				case ABAJO: yc += 15; if (yc > 632) yc = 632; break;//abajo
				case ACELERABAJADA: yc += 30; if (yc > 632) yc = 632; break;//Acelera hacia abajo
				}
			}
		}
		else { _getch(); }
	}
}

void Menu() {
	Consola();
	Inicio();
	char tecla_menus, teclajuego; int escenario = 1;
	bool gameover; int nivel, vidas, puntos;
	while (true) {
		tecla_menus = _getch();
		switch (escenario)
		{
		case 1:
			gameover = false;
			vidas = 3;
			nivel = 1;
			puntos = 0;
			if (tecla_menus == IZQUIERDA) {
				Reglas();
				escenario = 2;
			}
			if (tecla_menus == DERECHA) {
				Creditos();
				escenario = 3;
			}
			if (tecla_menus == ESPACIO) {
				EscenarioJuego();
				gotoxy(28, 6); cout << "Presione cualquier tecla para empezar a jugar";
				escenario = 4;
			}
			break;
		case 2:
			if (tecla_menus == IZQUIERDA) {
				Inicio();
				escenario = 1;
			}
			break;
		case 3:
			if (tecla_menus == IZQUIERDA) {
				Inicio();
				escenario = 1;
			}
			break;
		case 4:
			while (!gameover) {
				switch (nivel) {
				case 1:
					puntos = 0; length_fish = 6;
					EscenarioJuego(); gotoxy(3, 0); cout << "Nivel 1";
					Mover_Objetos(gameover, puntos, vidas);
					if (puntos >= 100) gameover = false;
					Verificar_Vidas(gameover, vidas);
					if (puntos >= 100 && gameover == false)
					{
						nivel = 2;
						color(180); gotoxy(40, 5); cout << "Pasaste al nivel " << nivel;
						gotoxy(28, 6); cout << "Presione cualquier tecla para continuar";
						_getch();
					}
					else {
						if (vidas == 0) {
							B_Perdiste();
							escenario = 6;
						}
						else {
							gotoxy(28, 6); color(180); cout << "Presione cualquier tecla para continuar";
							gotoxy(40, 5); cout << "Pierdes una vida";
							_getch();
						}
					}
					break;
				case 2:
					puntos = 0; length_fish = 5;
					EscenarioJuego(); gotoxy(3, 0); cout << "Nivel 2";
					Mover_Objetos(gameover, puntos, vidas);
					if (puntos >= 100) gameover = false;
					Verificar_Vidas(gameover, vidas);
					if (puntos >= 100 && gameover == false)
					{
						nivel = 3;
						color(180); gotoxy(40, 5); cout << "Pasaste al nivel " << nivel;
						gotoxy(28, 6); cout << "Presione cualquier tecla para continuar";
						_getch();
					}
					else {
						if (vidas == 0) {
							B_Perdiste();
							escenario = 6;
						}
						else {
							gotoxy(28, 6); color(180); cout << "Presione cualquier tecla para continuar";
							gotoxy(40, 5); cout << "Pierdes una vida";
							_getch();
						}
					}
					break;
				case 3:
					puntos = 0; length_fish = 4;
					EscenarioJuego(); gotoxy(3, 0); cout << "Nivel 3";
					Mover_Objetos(gameover, puntos, vidas);
					if (puntos >= 100) gameover = false;
					Verificar_Vidas(gameover, vidas);
					if (puntos >= 100 && gameover == false) nivel = 0;
					else {
						if (vidas == 0) {
							B_Perdiste();
							escenario = 6;
						}
						else {
							gotoxy(28, 6); color(180); cout << "Presione cualquier tecla para continuar";
							gotoxy(40, 5); cout << "Pierdes una vida";
							_getch();
						}
					}
					break;
				}
				if (puntos >= 100 && gameover == false && nivel == 0) {
					B_Ganaste();
					escenario = 5;
					break;
				}
			}
			break;
		case 5:
			if (tecla_menus == IZQUIERDA) {
				Inicio();
				escenario = 1;
			}
			break;
		case 6:
			if (tecla_menus == IZQUIERDA) {
				Inicio();
				escenario = 1;
			}
			break;
		}
	}
}