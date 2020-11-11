#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>


LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);

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
	                0, 0, 800, 600,
	                NULL, NULL, NULL, NULL
	            );
	HDC dc;
	dc = GetDC(hwnd);
	PIXELFORMATDESCRIPTOR pfd;
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
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
