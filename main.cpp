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

float rai = 0;

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

void desenhaCirculo(PONTO centro, float raio, COR c){

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
	PONTO p, a, b;
	COR c;
	glClear(GL_COLOR_BUFFER_BIT);
	
	p.x = 0.5f;
	p.y = 0.3f;
	
	a.x = 0.5f;
	a.y = 0.3f;
	
	b.x = cos(2 * PI * rai / 100) * (0.5f);
	b.y = sin(2 * PI * rai / 100) * (0.5f);
	
	if(rai < 100){
		rai++;
	}else{
		rai = 0;
	}
	
	c.R = 1;
	c.G = 0;
	c.B = 0;
	
	desenhaCirculo(p, 0.5f, c);
	desenhaLinha(a, b);
	
	c.R = 0.4f;
	c.G = 0.3f;
	c.B = 1;
	desenhaCirculo(b, 0.1f, c);
}



