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
void calculaMovimento();
void setaProximaPosicao(STICKMAN posDes);
void togglePassagem();

STICKMAN henry = {0};
int passagem = ANIMACAO_FASE_CONTRACT;
//int passagem = ANIMACAO_FASE_DOWN;
//int passagem = ANIMACAO_FASE_PASSO;
//int passagem = ANIMACAO_FASE_UP;

int settempo = 0;
int vez = 0;
int modo = ANIMACAO_MODO_ANDANDO;

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
					calculaMovimento();
					desenhaMundo();
					//desenhaLinha((PONTO){ -1.0f, -0.3f }, (PONTO){ 1.0f, -0.3f });
					SwapBuffers(GetDC(hwnd));
					break;
			}
			break;
		case WM_KEYDOWN:
			switch(wp){
				case 'S':
					modo = ANIMACAO_MODO_PARADO;
					break;
				case 'W':
					modo = ANIMACAO_MODO_ANDANDO;
					break;
				case 'R':
					modo = ANIMACAO_MODO_CAORRENDO;
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

void inverteFloat(float *a, float *b){
	float aux = *a;
	*a = *b;
	*b = aux;
}

void togglePassagem(){
	switch (passagem){
		case ANIMACAO_FASE_CONTRACT:
			passagem = ANIMACAO_FASE_DOWN;
			break;
		case ANIMACAO_FASE_DOWN:
			passagem = ANIMACAO_FASE_PASSO;
			break;
		case ANIMACAO_FASE_PASSO:
			passagem = ANIMACAO_FASE_UP;
			break;
		case ANIMACAO_FASE_UP:
			passagem = ANIMACAO_FASE_CONTRACT;
			if(vez == 1){
				vez = 0;
			}else{
				vez = 1;
			}
			break;
	}
}

void startaStickman(){
	//angulos
	
	henry.ang.cabeca 		= 270;
	henry.ang.cotovelo1		= 275;
	henry.ang.cotovelo2		= 265;
	henry.ang.joelho1		= 270;
	henry.ang.joelho2		= 270;
	henry.ang.ombro1		= 280;
	henry.ang.ombro2		= 260;
	henry.ang.ombroQ		= 270;
	henry.ang.quadril1		= 280;
	henry.ang.quadril2		= 260;
	henry.ang.tornozelo1	= 360;
	henry.ang.tornozelo2	= 360;
	
	//angulos
	// cabeça
	henry.cabeca.x = henry.cabeca.x + 0.01;
	henry.cabeca.y = 0.5f;
	
	//ombro
	henry.ombro.x = distanciax(H_PESCOCO, henry.ang.cabeca, henry.cabeca.x);
	henry.ombro.y = distanciay(H_PESCOCO, henry.ang.cabeca, henry.cabeca.y);
	
	//cotovelos
	henry.cotovelo1.x = distanciax(H_BRACOS, henry.ang.ombro1, henry.ombro.x);
	henry.cotovelo1.y = distanciay(H_BRACOS, henry.ang.ombro1, henry.ombro.y);
	
	henry.cotovelo2.x = distanciax(H_BRACOS, henry.ang.ombro2, henry.ombro.x);
	henry.cotovelo2.y = distanciay(H_BRACOS, henry.ang.ombro2, henry.ombro.y);
	
	//mãos
	henry.mao1.x = distanciax(H_ANTEBRACO, henry.ang.cotovelo1, henry.cotovelo1.x);
	henry.mao1.y = distanciay(H_ANTEBRACO, henry.ang.cotovelo1, henry.cotovelo1.y);
	
	henry.mao2.x = distanciax(H_ANTEBRACO, henry.ang.cotovelo2, henry.cotovelo2.x);
	henry.mao2.y = distanciay(H_ANTEBRACO, henry.ang.cotovelo2, henry.cotovelo2.y);
	
	//quadril
	henry.quadril.x = distanciax(H_TRONCO, henry.ang.ombroQ, henry.ombro.x);
	henry.quadril.y = distanciay(H_TRONCO, henry.ang.ombroQ, henry.ombro.y);
	
	//joelho
	henry.joelho1.x = distanciax(H_COXAS, henry.ang.quadril1, henry.quadril.x);
	henry.joelho1.y = distanciay(H_COXAS, henry.ang.quadril1, henry.quadril.y);
	
	henry.joelho2.x = distanciax(H_COXAS, henry.ang.quadril2, henry.quadril.x);
	henry.joelho2.y = distanciay(H_COXAS, henry.ang.quadril2, henry.quadril.y);
	
	//calcanhar
	henry.calcanhar1.x = distanciax(H_PANTURRILHA, henry.ang.joelho1, henry.joelho1.x);
	henry.calcanhar1.y = distanciay(H_PANTURRILHA, henry.ang.joelho1, henry.joelho1.y);
	
	henry.calcanhar2.x = distanciax(H_PANTURRILHA, henry.ang.joelho2, henry.joelho2.x);
	henry.calcanhar2.y = distanciay(H_PANTURRILHA, henry.ang.joelho2, henry.joelho2.y);
	
	//pé
	henry.pe1.x = distanciax(H_PES, henry.ang.tornozelo1, henry.calcanhar1.x);
	henry.pe1.y = distanciay(H_PES, henry.ang.tornozelo1, henry.calcanhar1.y);
	
	henry.pe2.x = distanciax(H_PES, henry.ang.tornozelo2, henry.calcanhar2.x);
	henry.pe2.y = distanciay(H_PES, henry.ang.tornozelo2, henry.calcanhar2.y);
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
	
}

void calculaMovimento(){// cabeça
	STICKMAN aux;
	glClear(GL_COLOR_BUFFER_BIT);
	//angulos
	
	if(modo == ANIMACAO_MODO_ANDANDO || modo == ANIMACAO_MODO_CAORRENDO){
	
		switch (passagem){
			case ANIMACAO_FASE_CONTRACT:
				// cabeça
				aux.cabeca.x = 0.01;
				aux.cabeca.y = 0.5f;
				aux.ang.cabeca 			= 270;	//
				aux.ang.cotovelo1		= 380;	//
				aux.ang.cotovelo2		= 260;	//
				aux.ang.joelho1			= 290;	//
				aux.ang.joelho2			= 235;	//
				aux.ang.ombro1			= 280;	//
				aux.ang.ombro2			= 240;	//
				aux.ang.ombroQ			= 267;	//
				aux.ang.quadril1		= 290; 	//
				aux.ang.quadril2		= 250;	//
				aux.ang.tornozelo1		= 360;	//
				aux.ang.tornozelo2		= 330;	//
				break;
			case ANIMACAO_FASE_DOWN:
				// cabeça
				aux.cabeca.x = 0.01;
				aux.cabeca.y = 0.49f;
				aux.ang.cabeca 			= 270;	//
				aux.ang.cotovelo1		= 350;	//
				aux.ang.cotovelo2		= 270;	//
				aux.ang.joelho1			= 290;	//
				aux.ang.joelho2			= 230;	//
				aux.ang.ombro1			= 280;	//
				aux.ang.ombro2			= 245;	//
				aux.ang.ombroQ			= 267;	//
				aux.ang.quadril1		= 305; 	//
				aux.ang.quadril2		= 250;	//
				aux.ang.tornozelo1		= 360;	//
				aux.ang.tornozelo2		= 320;	//
				
				break;
			case ANIMACAO_FASE_PASSO:
				
				aux.cabeca.x = 0.01;
				aux.cabeca.y = 0.525f;
				aux.ang.cabeca 			= 270;	
				aux.ang.cotovelo1		= 280;	//
				aux.ang.cotovelo2		= 280;	//
				aux.ang.joelho1			= 270;	//
				aux.ang.joelho2			= 225;	//
				aux.ang.ombro1			= 270;	//
				aux.ang.ombro2			= 260;	//
				aux.ang.ombroQ			= 267;	//
				aux.ang.quadril1		= 270; 	//
				aux.ang.quadril2		= 280;	//
				aux.ang.tornozelo1		= 360;	//
				aux.ang.tornozelo2		= 280;	//
				break;
			case ANIMACAO_FASE_UP:
				
				aux.cabeca.x = 0.01;
				aux.cabeca.y = 0.535f;
				aux.ang.cabeca 			= 270;	//
				aux.ang.cotovelo1		= 275;	//
				aux.ang.cotovelo2		= 310;	//
				aux.ang.joelho1			= 250;	//
				aux.ang.joelho2			= 230;	//
				aux.ang.ombro1			= 250;	//
				aux.ang.ombro2			= 280;	//
				aux.ang.ombroQ			= 267;	//
				aux.ang.quadril1		= 260; 	//
				aux.ang.quadril2		= 290;	//
				aux.ang.tornozelo1		= 320;	//
				aux.ang.tornozelo2		= 330;	//
		}
		
		if(vez == 1){
			inverteFloat(&aux.ang.cotovelo1, &aux.ang.cotovelo2);
			inverteFloat(&aux.ang.joelho1, &aux.ang.joelho2);
			inverteFloat(&aux.ang.ombro1, &aux.ang.ombro2);
			inverteFloat(&aux.ang.quadril1, &aux.ang.quadril2);
			inverteFloat(&aux.ang.tornozelo1, &aux.ang.tornozelo1);
		}
		
		settempo--;
		if(settempo <= 0){
			switch (passagem){
			case ANIMACAO_FASE_CONTRACT:
				settempo = 15 * modo;
				break;
			case ANIMACAO_FASE_DOWN:
				settempo = 15 * modo;
				break;
			case ANIMACAO_FASE_PASSO:
				settempo = 5 * modo;
				break;
			case ANIMACAO_FASE_UP:
				settempo = 10 * modo;
				break;
			}
			
			togglePassagem();
		}
	}else{
		aux.cabeca.x = 0.01f;
		aux.cabeca.y = 0.5f;
		aux.ang.cabeca 			= 270;
		aux.ang.cotovelo1		= 275;
		aux.ang.cotovelo2		= 265;
		aux.ang.joelho1			= 270;
		aux.ang.joelho2			= 270;
		aux.ang.ombro1			= 280;
		aux.ang.ombro2			= 260;
		aux.ang.ombroQ			= 270;
		aux.ang.quadril1		= 280;
		aux.ang.quadril2		= 260;
		aux.ang.tornozelo1		= 360;
		aux.ang.tornozelo2		= 360;
	}
	setaProximaPosicao(aux);
	//angulos
	//henry.ang = aux.ang;
	//henry.cabeca = aux.cabeca;
	//ombro
	henry.ombro.x = distanciax(H_PESCOCO, henry.ang.cabeca, henry.cabeca.x);
	henry.ombro.y = distanciay(H_PESCOCO, henry.ang.cabeca, henry.cabeca.y);
	
	//cotovelos
	henry.cotovelo1.x = distanciax(H_BRACOS, henry.ang.ombro1, henry.ombro.x);
	henry.cotovelo1.y = distanciay(H_BRACOS, henry.ang.ombro1, henry.ombro.y);
	
	henry.cotovelo2.x = distanciax(H_BRACOS, henry.ang.ombro2, henry.ombro.x);
	henry.cotovelo2.y = distanciay(H_BRACOS, henry.ang.ombro2, henry.ombro.y);
	
	//mãos
	henry.mao1.x = distanciax(H_ANTEBRACO, henry.ang.cotovelo1, henry.cotovelo1.x);
	henry.mao1.y = distanciay(H_ANTEBRACO, henry.ang.cotovelo1, henry.cotovelo1.y);
	
	henry.mao2.x = distanciax(H_ANTEBRACO, henry.ang.cotovelo2, henry.cotovelo2.x);
	henry.mao2.y = distanciay(H_ANTEBRACO, henry.ang.cotovelo2, henry.cotovelo2.y);
	
	//quadril
	henry.quadril.x = distanciax(H_TRONCO, henry.ang.ombroQ, henry.ombro.x);
	henry.quadril.y = distanciay(H_TRONCO, henry.ang.ombroQ, henry.ombro.y);
	
	//joelho
	henry.joelho1.x = distanciax(H_COXAS, henry.ang.quadril1, henry.quadril.x);
	henry.joelho1.y = distanciay(H_COXAS, henry.ang.quadril1, henry.quadril.y);
	
	henry.joelho2.x = distanciax(H_COXAS, henry.ang.quadril2, henry.quadril.x);
	henry.joelho2.y = distanciay(H_COXAS, henry.ang.quadril2, henry.quadril.y);
	
	//calcanhar
	henry.calcanhar1.x = distanciax(H_PANTURRILHA, henry.ang.joelho1, henry.joelho1.x);
	henry.calcanhar1.y = distanciay(H_PANTURRILHA, henry.ang.joelho1, henry.joelho1.y);
	
	henry.calcanhar2.x = distanciax(H_PANTURRILHA, henry.ang.joelho2, henry.joelho2.x);
	henry.calcanhar2.y = distanciay(H_PANTURRILHA, henry.ang.joelho2, henry.joelho2.y);
	
	//pé
	henry.pe1.x = distanciax(H_PES, henry.ang.tornozelo1, henry.calcanhar1.x);
	henry.pe1.y = distanciay(H_PES, henry.ang.tornozelo1, henry.calcanhar1.y);
	
	henry.pe2.x = distanciax(H_PES, henry.ang.tornozelo2, henry.calcanhar2.x);
	henry.pe2.y = distanciay(H_PES, henry.ang.tornozelo2, henry.calcanhar2.y);
}

void setaProximaPosicao(STICKMAN posDes){
	
	henry.ang.cabeca 		+= (float)(posDes.ang.cabeca - henry.ang.cabeca) / (float)settempo;
	henry.ang.cotovelo1 	+= (float)(posDes.ang.cotovelo1 - henry.ang.cotovelo1) / (float)settempo;
	henry.ang.cotovelo2 	+= (float)(posDes.ang.cotovelo2 - henry.ang.cotovelo2) / (float)settempo;
	henry.ang.joelho1 		+= (float)(posDes.ang.joelho1 - henry.ang.joelho1) / (float)settempo;
	henry.ang.joelho2 		+= (float)(posDes.ang.joelho2 - henry.ang.joelho2) / (float)settempo;
	henry.ang.ombro1 		+= (float)(posDes.ang.ombro1 - henry.ang.ombro1) / (float)settempo;
	henry.ang.ombro2 		+= (float)(posDes.ang.ombro2 - henry.ang.ombro2) / (float)settempo;
	henry.ang.ombroQ 		+= (float)(posDes.ang.ombroQ - henry.ang.ombroQ) / (float)settempo;
	henry.ang.quadril1 		+= (float)(posDes.ang.quadril1 - henry.ang.quadril1) / (float)settempo;
	henry.ang.quadril2 		+= (float)(posDes.ang.quadril2 - henry.ang.quadril2) / (float)settempo;
	henry.ang.tornozelo1 	+= (float)(posDes.ang.tornozelo1 - henry.ang.tornozelo1) / (float)settempo;
	henry.ang.tornozelo2 	+= (float)(posDes.ang.tornozelo2 - henry.ang.tornozelo2) / (float)settempo;
	henry.cabeca.x			+= (posDes.cabeca.x - henry.cabeca.x) / (float)settempo;
	henry.cabeca.y			+= (posDes.cabeca.y - henry.cabeca.y) / (float)settempo;
	
}


