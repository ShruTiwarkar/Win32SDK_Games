#include "Piece.h"
#include "Game.h"
#include <windows.h>
#include <iostream>
#include "SetIcon.h"
#include <Mmsystem.h>

using namespace std;

const int PX_PER_BLOCK = 25;
const int SCREEN_WIDTH = 10;
const int SCREEN_HEIGHT = 20;
const int GAME_SPEED = 33;
const int TIMER_ID = 1;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {

	WNDCLASSEX wndclass;						
	HWND hwnd;							
	MSG msg;							
	TCHAR szAppName[] = TEXT("Block Game");

	wndclass.cbSize = sizeof(WNDCLASSEX);				
	wndclass.style = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;			
	wndclass.cbClsExtra = 0;					
	wndclass.cbWndExtra = 0;					
	wndclass.lpfnWndProc = WndProc;					
	wndclass.hInstance = hInstance;					
	wndclass.hIcon = LoadIcon(hInstance, (LPWSTR)MYICON);		
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);			
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);	
	wndclass.lpszClassName = szAppName;				
	wndclass.lpszMenuName = NULL;					
	wndclass.hIconSm = LoadIcon(hInstance, (LPWSTR)MYICON);

	RegisterClassEx(&wndclass);					

	hwnd = CreateWindow(szAppName,
		TEXT("Block game"),								
		WS_MINIMIZEBOX | WS_SYSMENU,
		550 ,				
		150 ,						
		SCREEN_WIDTH * PX_PER_BLOCK + 156,						
		SCREEN_HEIGHT * PX_PER_BLOCK + 25,						
		NULL,							
		NULL,							
		hInstance,						
		NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return((int)msg.wParam);
}

static int pageNumber = 0;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	static HDC hMemdc;
	PAINTSTRUCT ps;
	RECT rc;
	BITMAP bmp;
	static Game *game;
	static DrawEngine *de;
	static HINSTANCE hInst;
	static HBITMAP hBitmap = NULL, hBitmap1 = NULL;
	static TCHAR szStartUpLine[] = TEXT("Press 'S' to Start the Game...!");
	HFONT hFont;

	switch (message)
	{
	case WM_CREATE:
		// PlaySound(TEXT("Splashscreen.wav"), NULL, SND_ASYNC && SND_FILENAME);
		PlaySound(TEXT("Splashscreen.wav"), NULL, SND_LOOP | SND_ASYNC);
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hBitmap = (HBITMAP)LoadBitmap(hInst, (LPCWSTR)MYBITMAP);
		hBitmap1 = (HBITMAP)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(MYBITMAPS));

		hdc = GetDC(hwnd);

		de = new DrawEngine(hdc, hwnd, PX_PER_BLOCK);
		game = new Game(*de);
		SetTimer(hwnd, TIMER_ID, GAME_SPEED, NULL);

		ReleaseDC(hwnd, hdc);
		break;

	case WM_KEYDOWN:
		if (pageNumber == 1) {
			game->keyPress(wParam);
		}
		break;

	case WM_TIMER:
		if (pageNumber == 1) {
			game->timerUpdate();
		}
		break;

	case WM_KILLFOCUS:
		if (pageNumber == 1) {
			KillTimer(hwnd, TIMER_ID);
			game->pause(true);
		}
		break;

	case WM_SETFOCUS:
		if (pageNumber == 1) {
			SetTimer(hwnd, TIMER_ID, GAME_SPEED, NULL);
		}
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);
		if (pageNumber == 0) {
			hMemdc = CreateCompatibleDC(NULL);
			SelectObject(hMemdc, (HGDIOBJ)hBitmap);
			//SelectObject(hMemdc, (HGDIOBJ)hBitmap1);
			GetObject(hBitmap, sizeof(BITMAP), &bmp);
			StretchBlt(hdc, 3, 12,10, 5, hMemdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			DeleteDC(hMemdc);

			hMemdc = CreateCompatibleDC(NULL);
			SelectObject(hMemdc, (HGDIOBJ)hBitmap1);
			GetObject(hBitmap1, sizeof(BITMAP), &bmp);
			StretchBlt(hdc, 3, 3, 10, 5, hMemdc, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
			DeleteDC(hMemdc);
			break;
		}
		else if(pageNumber == 1){
			game->repaint();
		}
		EndPaint(hwnd, &ps);
		break;
	
	case WM_CHAR :
		
		switch (LOWORD(wParam)) {
		case 'S':
		case 's':
			if (pageNumber == 0) {
				PlaySound(NULL, NULL, 0);
				InvalidateRect(hwnd, NULL, TRUE);
				PlaySound(TEXT("Background.wav"), NULL, SND_LOOP | SND_ASYNC);
				pageNumber = 1;
			}
			break;
		case 27:
			DestroyWindow(hwnd);
			break;
		default:
			break;
		}
		
		break;
	case WM_DESTROY:
		if (pageNumber == 1) {
			delete de;
			delete game;
			KillTimer(hwnd, TIMER_ID);
		}
		PostQuitMessage(0);
		return 0;

	}

	return DefWindowProc(hwnd, message, wParam, lParam);
}
