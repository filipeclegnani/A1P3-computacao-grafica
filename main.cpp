#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>

#include "parametros.h"


LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

void desenhaCirculo(PONTO centro, float raio, COR c);
void desenhaLinha(PONTO a, PONTO b);
void desenhaMundo();
void startaStickman();

STICKMAN henry = {0};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow) {

	WNDCLASS wc = {};

	wc.lpszClassName = "My Window Class";
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProcedure;
	wc.hInstance = hInst;

	RegisterClass(&wc);
	HWND hwnd =	CreateWindow(
	                wc.lpszClassName,
	                "My OpenGL App",
	                WS_OVERLAPPEDWINDOW | WS_VISIBLE,
	                0, 0, 600, 600,
	                NULL, NULL, NULL, NULL
	            );
	HDC dc;
	dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;

	int pf = ChoosePixelFormat(dc, &pfd);
	if(pf == 0) {
		MessageBox(NULL,"ChoosePixelFormat(dc, &pfd);","ERRO",MB_OK);
		return 0;
	}
	if(SetPixelFormat(dc, pf, &pfd) == FALSE) {
		MessageBox(NULL, "SetPixelFormat(dc, pf, &pfd)", "ERRO", MB_OK);
		return 0;
	}
	DescribePixelFormat(dc, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

	HGLRC hRC;
	hRC = wglCreateContext(dc);
	wglMakeCurrent(dc, hRC);

	ShowWindow(hwnd, ncmdshow);

	SetTimer(hwnd, SET_TIMER_60, 1000/60, NULL);
	
	MSG msg = {};

	while(GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}

LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
	
	switch (msg) {

		case WM_CREATE:
			startaStickman();
			break;


		case WM_PAINT:
			//redesenhatodas();
			break;

		case WM_MOUSEMOVE:
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			break;
		case WM_TIMER:
			switch(wp){
				case SET_TIMER_60:
					desenhaMundo();
					SwapBuffers(GetDC(hwnd));
					break;
			}
			break;
		case WM_SIZE:
			PostMessage(hwnd, WM_PAINT, 0, 0);
			break;
		case WM_COMMAND:
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}

	return DefWindowProc(hwnd, msg, wp, lp);
}

void startaStickman(){
	// cabeça
	henry.cabeca.x = 0.0f;
	henry.cabeca.y = 0.5f;
	
	//ombro
	henry.ombro.x = distanciax(H_PESCOCO, 270, henry.cabeca.x);
	henry.ombro.y = distanciay(H_PESCOCO, 270, henry.cabeca.y);
	
	//cotovelos
	henry.cotovelo1.x = distanciax(H_BRACOS, 280, henry.ombro.x);
	henry.cotovelo1.y = distanciay(H_BRACOS, 280, henry.ombro.y);
	
	henry.cotovelo2.x = distanciax(H_BRACOS, 260, henry.ombro.x);
	henry.cotovelo2.y = distanciay(H_BRACOS, 260, henry.ombro.y);
	
	//mãos
	henry.mao1.x = distanciax(H_ANTEBRACO, 275, henry.cotovelo1.x);
	henry.mao1.y = distanciay(H_ANTEBRACO, 275, henry.cotovelo1.y);
	
	henry.mao2.x = distanciax(H_ANTEBRACO, 265, henry.cotovelo2.x);
	henry.mao2.y = distanciay(H_ANTEBRACO, 265, henry.cotovelo2.y);
	
	//quadril
	henry.quadril.x = distanciax(H_TRONCO, 270, henry.ombro.x);
	henry.quadril.y = distanciay(H_TRONCO, 270, henry.ombro.y);
	
	//joelho
	henry.joelho1.x = distanciax(H_COXAS, 280, henry.quadril.x);
	henry.joelho1.y = distanciay(H_COXAS, 280, henry.quadril.y);
	
	henry.joelho2.x = distanciax(H_COXAS, 260, henry.quadril.x);
	henry.joelho2.y = distanciay(H_COXAS, 260, henry.quadril.y);
	
	//calcanhar
	henry.calcanhar1.x = distanciax(H_PANTURRILHA, 270, henry.joelho1.x);
	henry.calcanhar1.y = distanciay(H_PANTURRILHA, 270, henry.joelho1.y);
	
	henry.calcanhar2.x = distanciax(H_PANTURRILHA, 270, henry.joelho2.x);
	henry.calcanhar2.y = distanciay(H_PANTURRILHA, 270, henry.joelho2.y);
	
	//pé
	henry.pe1.x = distanciax(H_PES, 0, henry.calcanhar1.x);
	henry.pe1.y = distanciay(H_PES, 0, henry.calcanhar1.y);
	
	henry.pe2.x = distanciax(H_PES, 0, henry.calcanhar2.x);
	henry.pe2.y = distanciay(H_PES, 0, henry.calcanhar2.y);
}

void desenhaCirculo(PONTO centro, float raio, COR c){
	// (cos(2 * PI * a / 360) * (r)) + px)
	// (sin(2 * PI * a/ 360) * (r)) + py)
	GLfloat vertices = 100;
	GLfloat angulo;
	
	glBegin(GL_POLYGON);
	glColor3f(c.R, c.G, c.B);
	for(int i=0; i < vertices; i++) {
		angulo = 2 * PI * i / vertices;
		glVertex2f(
		    (cos(angulo) * (raio)) + centro.x,
		    (sin(angulo) * (raio)) + centro.y);
	}
	glEnd();
	glFlush();
}

void desenhaLinha(PONTO a, PONTO b){
	glBegin(GL_LINES);
	glColor3f (1, 1, 1);
	glVertex2f(a.x, a.y);
	glVertex2f(b.x, b.y);
	glEnd();
	glFlush();
}

PONTO pto(float x, float y){
	PONTO a;
	a.x = (float)x;
	a.y = (float)y;
	return a;
}

void desenhaMundo(){
	// cabeça
	desenhaCirculo(henry.cabeca, RAIO_CABECA, T_COLOR_WHITE);
	
	desenhaLinha(henry.cabeca, henry.ombro);
	//ombro
	desenhaCirculo(henry.ombro, RAIO_JUNTA, T_COLOR_RED);
	
	desenhaLinha(henry.ombro, henry.cotovelo1);
	desenhaLinha(henry.ombro, henry.cotovelo2);
	//cotovelos
	desenhaCirculo(henry.cotovelo1, RAIO_JUNTA, T_COLOR_RED);
	desenhaCirculo(henry.cotovelo2, RAIO_JUNTA, T_COLOR_RED);
	
	desenhaLinha(henry.cotovelo1, henry.mao1);
	desenhaLinha(henry.cotovelo2, henry.mao2);
	//mãos
	
	//quadril
	desenhaLinha(henry.ombro, henry.quadril);
	
	desenhaCirculo(henry.quadril, RAIO_JUNTA, T_COLOR_RED);
	
	desenhaLinha(henry.quadril, henry.joelho1);
	desenhaLinha(henry.quadril, henry.joelho2);
	//joelho
	desenhaCirculo(henry.joelho1, RAIO_JUNTA, T_COLOR_RED);
	desenhaCirculo(henry.joelho2, RAIO_JUNTA, T_COLOR_RED);
	
	//calcanhar
	desenhaLinha(henry.joelho1, henry.calcanhar1);
	desenhaLinha(henry.joelho2, henry.calcanhar2);
	
	//pé
	desenhaLinha(henry.calcanhar1, henry.pe1);
	desenhaLinha(henry.calcanhar2, henry.pe2);
	
	
	desenhaLinha(henry.quadril, pto(0.5f, 0.5f));
}



