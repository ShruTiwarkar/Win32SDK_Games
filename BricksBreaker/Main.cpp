#include <Windows.h>
#include<Windowsx.h>
#include<Mmsystem.h>

#define ID_TIMER	100

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void PrintGameTitle(HWND, HDC, long, long, long, long);
void SetBackGroundWhite(HWND, HDC, RECT);
void Ball(HWND hwnd, HBITMAP * hBitmapBall, long cxTotal, long cyTotal, long cxMove, long cyMove, HRGN * B_hRgn, RECT *rcBall);
void BallTimer(HWND hwnd, HBITMAP * hBitMapBall, long cxClient, long cyClient, long *cxTotal, long *cyTotal, long *xHalf, long *yHalf, long *cxMove, long *cyMove, long *cxRadius, long *cyRadius);
void DrawPlate(HWND hwnd, long xPos, long yPos, long cxClient, long cyClient, long xHalfOfQuad, long yHalfOfQuad, HRGN *P_hRgn, HRGN *PP_hRgn, RECT *rcPlate);
void BallPlateColision(HWND hwnd, HRGN *P_hRgn, long xHalf, long yHalf, long  cxRadius, long cyRadius, long *cxMove, long *cyMove, long cxClient, long cyClient, int*, HRGN *hRgnArrayBricks);
int CheckLives(HWND hwnd, int count);
void GameOver(HWND hwnd, long cxClient, long cyClient, long xHalf, long yHalf);
void DrawBricks(HWND hwnd, long cxClient, long cyClient, RECT*, HRGN *);
void BallBricksCollision(HWND hwnd, HRGN B_hrgn, RECT rcBAll, RECT *rcArrayBricks, HRGN *hRgnArrayBricks, long xHalf, long yHalf, long cxRadius, long cyRadius, long *cxMove, long *cyMove, long cxClient, long cyClient);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPreInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("My App");

	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_VREDRAW | CS_HREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	RegisterClassEx(&wndclass);


	hwnd = CreateWindow(szAppName,
		TEXT("Bricks Breaker"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	HDC hdc;
	PAINTSTRUCT ps;																		// Used by Os internally
	RECT rc;
	static RECT rcArrayBricks[6];
	static HRGN hRgnArrayBricks[6];
	static long xPos, yPos;
	static long cxClient, cyClient, xHalf, yHalf, xQuad, yQuad, xHalfOfQuad, yHalfOfQuad,xPixel, yPixel, iScale, cxRadius, cyRadius, cxMove, cyMove, cxTotal,cyTotal;
	static int pageNumber = 1, LivesCount = 0;
	static HBITMAP hBitmapBall;
	static HRGN B_hRgn,P_hRgn, PP_hRgn;
	static RECT rcBall, rcPlate;
	static int array[] = {0,0,0,0,0,0};
	static int count;

	switch (iMsg) {

	case WM_CREATE:
		PlaySound(TEXT("BackGroundSong.wav"), NULL, SND_ASYNC && SND_FILENAME);
		hdc = GetDC(hwnd);
		xPixel = GetDeviceCaps(hdc, ASPECTX);
		yPixel = GetDeviceCaps(hdc, ASPECTY);
		//hBrush_white = CreateSolidBrush(RGB(255, 255, 255));

		ReleaseDC(hwnd, hdc);

		
		break;

	case WM_CHAR:

		switch (LOWORD(wParam))
		{
		case 'S':
		case 's':
			pageNumber = 2;
			InvalidateRect(hwnd, NULL, TRUE);
			break;
		}

	break;
	case WM_SIZE:

		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);

		xHalf = cxClient / 2;
		yHalf = cyClient / 2;

		xQuad = xHalf / 2;
		yQuad = yHalf / 2;

		xHalfOfQuad = xQuad / 2;
		yHalfOfQuad = yQuad / 2;

		iScale = min(cxClient * xPixel, cyClient * yPixel) / 16;

		cxRadius = iScale / xPixel;
		cyRadius = iScale / yPixel;

		cxMove = max(1, cxRadius / 2);
		cyMove = max(1, cyRadius / 2);

		cxTotal = 2 * (cxRadius + cxMove);
		cyTotal = 2 * (cyRadius + cyMove);
		
		if (!hBitmapBall)
			DeleteObject(hBitmapBall);

		Ball(hwnd,&hBitmapBall, cxTotal, cyTotal, cxMove, cyMove, &B_hRgn, &rcBall);
		//InvalidateRect(hwnd, &rc, FALSE);
		InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_PAINT:
		GetClientRect(hwnd, &rc);
		hdc = BeginPaint(hwnd, &ps);

		switch (pageNumber) {
		case 1:
			PrintGameTitle(hwnd, hdc, cxClient, cyClient, xQuad, yQuad);
			break;
		case 2:
			SetBackGroundWhite(hwnd, hdc, rc);
			SetTimer(hwnd, ID_TIMER, 60, NULL);
			pageNumber = 3;
			DrawBricks(hwnd, cxClient, cyClient, rcArrayBricks, hRgnArrayBricks);
			SetTimer(hwnd, ID_TIMER, 60, NULL);
			break;
		case 3:
			DrawPlate(hwnd,xPos, yPos, cxClient, cyClient, xHalfOfQuad, yHalfOfQuad, &P_hRgn, &PP_hRgn,&rcPlate);
			break;
		}
	
		UpdateWindow(hwnd);
		EndPaint(hwnd, &ps);
		break;

	case WM_MOUSEMOVE:
		xPos = GET_X_LPARAM(lParam);
		yPos = GET_Y_LPARAM(lParam);
		InvalidateRect(hwnd, &rc, FALSE);
		//InvalidateRect(hwnd, NULL, FALSE);
		break;

	case WM_TIMER:
		if (!hBitmapBall) {
			break;
		}
		BallTimer(hwnd, &hBitmapBall,cxClient, cyClient,&cxTotal, &cyTotal, &xHalf, &yHalf, &cxMove, &cyMove, &cxRadius, &cyRadius);
		BallPlateColision(hwnd, &P_hRgn, xHalf, yHalf, cxRadius, cyRadius, &cxMove, &cyMove, cxClient, cyClient, &LivesCount, hRgnArrayBricks);
		//BallBricksCollision(hwnd, B_hRgn,rcBall,rcArrayBricks, hRgnArrayBricks, xHalf, yHalf, cxRadius, cyRadius, &cxMove, &cyMove, cxClient, cyClient);
		break;

	case WM_DESTROY:
		KillTimer(hwnd, ID_TIMER);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void PrintGameTitle(HWND hwnd, HDC hdc, long cxClient, long cyClient, long xQuad, long yQuad) {

	static TCHAR szTitle[] = TEXT("Bricks Breaker");
	static TCHAR szStartUpLine[] = TEXT("Press 'S' to Start the Game...!");
	HFONT hFont;
	SIZE size;
	HBRUSH hBrush;
	HRGN hRgn;
	int xTemp = 0, yTemp = 0;

	hFont = CreateFont(100, 40, 0, 10, FW_EXTRABOLD, TRUE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Comic Sans MS"));
	//Mistral

	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);

	GetTextExtentPoint32(hdc, szTitle, strlen(szTitle), &size);

	BeginPath(hdc);
	//TextOut(hdc, (cxClient - size.cx) / 2, (cyClient - size.cy) / 2, szTitle, lstrlen(szTitle));
	TextOut(hdc, (xQuad + xQuad), (yQuad), szTitle, lstrlen(szTitle));
	TextOut(hdc, (xQuad / 2), cyClient - yQuad, szStartUpLine, lstrlen(szStartUpLine));
	EndPath(hdc);

	SelectObject(hdc, CreateHatchBrush(HS_BDIAGONAL, RGB(255, 0, 0)));
	SetBkColor(hdc, RGB(0, 0, 255));
	SetBkMode(hdc, OPAQUE);

	StrokeAndFillPath(hdc);
	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
	SelectObject(hdc, GetStockObject(SYSTEM_FONT));
	DeleteObject(hFont);

	for (int i = 0; i < 6; i++) {
		xTemp = xTemp + ((xQuad / 2) / 2);
		yTemp = yTemp + ((yQuad / 2) / 2);
		//hBrush = CreateSolidBrush(RGB(139,69,19));//saddleBrown
		hBrush = CreateSolidBrush(RGB(160,82,45));//sienna
		hRgn = CreateRectRgn(xTemp, yTemp, xTemp + 50 , yTemp + 20);
		FillRgn(hdc, hRgn, hBrush);
		//Rectangle(hdc, xQuad, yQuad, xQuad + 20, yQuad + 10);
	}
	hBrush = CreateSolidBrush(RGB(255, 0, 255));//magenta
	hRgn = CreateEllipticRgn(xTemp/2 + 50, yTemp/2 - 50, xTemp/2 + 150, yTemp/2 + 50);
	FillRgn(hdc, hRgn, hBrush);

}

void SetBackGroundWhite(HWND hwnd, HDC hdc, RECT rc) {
	HBRUSH hBrush;

	hBrush = CreateSolidBrush(RGB(255,255,255));
	SelectObject(hdc, hBrush);
	FillRect(hdc, &rc, hBrush);

}

void Ball(HWND hwnd, HBITMAP *hBitmapBall, long cxTotal, long cyTotal, long cxMove, long cyMove, HRGN *B_hRgn, RECT *rcBall) {
	HDC hdc;
	HDC hdcBall;
	HBRUSH hBrush;

	hdc = GetDC(hwnd);
	hdcBall = CreateCompatibleDC(hdc);
	*hBitmapBall = CreateCompatibleBitmap(hdc, cxTotal, cyTotal);
	ReleaseDC(hwnd, hdc);

	SelectObject(hdcBall, *hBitmapBall);
	Rectangle(hdcBall, -1, -1, cxTotal + 1, cyTotal + 1);

	hBrush = CreateHatchBrush(HS_DIAGCROSS, 0L);
	SelectObject(hdcBall, hBrush);
	SetBkColor(hdcBall, RGB(255, 0, 255));
	Ellipse(hdcBall, cxMove, cyMove, cxTotal - cxMove, cyTotal - cyMove);
	GetRgnBox(*B_hRgn, rcBall);

	DeleteDC(hdcBall);
	DeleteObject(hBrush);
	ReleaseDC(hwnd, hdc);
	}

void BallTimer(HWND hwnd, HBITMAP * hBitMapBall, long cxClient, long cyClient, long *cxTotal, long *cyTotal, long *xHalf, long *yHalf, long *cxMove, long *cyMove, long *cxRadius, long *cyRadius) {
	HDC hdc;
	HDC hdcBall;

	hdc = GetDC(hwnd);
	hdcBall = CreateCompatibleDC(hdc);
	SelectObject(hdcBall, *hBitMapBall);

	BitBlt(hdc, (*xHalf) - (*cyTotal) / 2, (*yHalf) - (*cyTotal) / 2, (*cxTotal), (*cyTotal), hdcBall, 0, 0, SRCCOPY);

	ReleaseDC(hwnd, hdc);
	DeleteDC(hdcBall);

	*xHalf += *cxMove;
	*yHalf += *cyMove;
	ReleaseDC(hwnd, hdc);
}

void DrawPlate(HWND hwnd, long xPos, long yPos, long cxClient, long cyClient, long xHalfOfQuad, long yHalfOfQuad, HRGN *P_hRgn, HRGN *PP_hRgn, RECT *rcPlate) {
	HBRUSH hBrush;
	HDC hdc;

	hdc = GetDC(hwnd);
	hBrush = CreateSolidBrush(RGB(255,255,255));//White
	FillRgn(hdc,*PP_hRgn, hBrush);

	hBrush = CreateSolidBrush(RGB(255, 0, 0));//RED
	*P_hRgn = CreateRectRgn(xPos - 50, (cyClient - yHalfOfQuad), xPos + 20, (cyClient - yHalfOfQuad) + 20);
	GetRgnBox(*P_hRgn, rcPlate);
	FillRgn(hdc, *P_hRgn, hBrush);

	*PP_hRgn = *P_hRgn;
	ReleaseDC(hwnd, hdc);
}

void BallPlateColision(HWND hwnd, HRGN *P_hRgn, long xHalf, long yHalf, long cxRadius, long cyRadius, long *cxMove, long *cyMove, long cxClient, long cyClient, int *countLives, HRGN *hRgnArrayBricks) {
	
	HBRUSH hBrush;
	HDC hdc;
	RECT rcArrayBricks;
	static int array[6] = { 0,0,0,0,0,0 };
	static int count = 0;

	hdc = GetDC(hwnd);

	if ((xHalf + cxRadius >= cxClient) || (xHalf - cxRadius <= 0))
		*cxMove = -(*cxMove);

	if (PtInRegion(*P_hRgn, (xHalf + cxRadius), yHalf + cyRadius)) {
		
		*cyMove = -(*cyMove);
	}
	else if (PtInRegion(*P_hRgn, (xHalf + cxRadius) - 50, (yHalf + cyRadius))) {
		*cyMove = -(*cyMove);
	}
	else if (PtInRegion(*P_hRgn, (xHalf + cxRadius) - 100, yHalf + cyRadius)) {
		*cyMove = -(*cyMove);
	}
	else if ((yHalf + cyRadius >= cyClient) || (yHalf - cyRadius <= 0)) {
		*cyMove = -(*cyMove);
		if ((yHalf + cyRadius >= cyClient)) {
			*countLives = *countLives + 1;
			if (CheckLives(hwnd, *countLives)) {
				GameOver(hwnd, cxClient, cyClient, xHalf, yHalf);
			}
		}
	}
	
	for (int i = 0; i < 6; i++) {

		if (array[i] == 0) {
			if (PtInRegion(hRgnArrayBricks[i], (xHalf + cxRadius), (yHalf + cyRadius))) {
				GetRgnBox(hRgnArrayBricks[i], &rcArrayBricks);
				*cyMove = -(*cyMove);
				InvalidateRect(hwnd, &rcArrayBricks, TRUE);
				count++;
				array[i] = 1;
			}
			else if (PtInRegion(hRgnArrayBricks[i], (xHalf + cxRadius) - 50, (yHalf + cyRadius))) {
				GetRgnBox(hRgnArrayBricks[i], &rcArrayBricks);
				*cyMove = -(*cyMove);
				InvalidateRect(hwnd, &rcArrayBricks, TRUE);
				count++;
				array[i] = 1;
			}
			else if (PtInRegion(hRgnArrayBricks[i], (xHalf + cxRadius) - 100, (yHalf + cyRadius))) {
				GetRgnBox(hRgnArrayBricks[i], &rcArrayBricks);
				*cyMove = -(*cyMove);
				InvalidateRect(hwnd, &rcArrayBricks, TRUE);
				count++;
				array[i] = 1;
			}
		
		}
		else{
			if (count == 6) {
				GameOver(hwnd, cxClient, cyClient, xHalf, yHalf);
			}
		}
	}
	ReleaseDC(hwnd, hdc);
}

int CheckLives(HWND hwnd, int count) {

	if (count == 3) {
		return TRUE;
	}
	else {
		return FALSE;
	}
}

void GameOver(HWND hwnd, long cxClient, long cyClient, long xHalf, long yHalf) {
	

	TCHAR szTitle[] = TEXT("GameOver...!");
	HDC hdc;

	HFONT hFont;
	SIZE size;
	
	hdc = GetDC(hwnd);
	//TextOut(hdc, (xHalf), (yHalf/2), szTitle, lstrlen(szTitle));

	hFont = CreateFont(100, 40, 0, 10, FW_EXTRABOLD, TRUE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, VARIABLE_PITCH, TEXT("Comic Sans MS"));
	//Mistral

	SelectObject(hdc, hFont);
	SetBkMode(hdc, TRANSPARENT);

	GetTextExtentPoint32(hdc, szTitle, strlen(szTitle), &size);

	BeginPath(hdc);
	TextOut(hdc, (cxClient - size.cx) / 2, (cyClient - size.cy), szTitle, lstrlen(szTitle));
	EndPath(hdc);

	SelectObject(hdc, CreateHatchBrush(HS_BDIAGONAL, RGB(255, 0, 0)));
	SetBkColor(hdc, RGB(0, 0, 255));
	SetBkMode(hdc, OPAQUE);

	StrokeAndFillPath(hdc);
	DeleteObject(SelectObject(hdc, GetStockObject(WHITE_BRUSH)));
	SelectObject(hdc, GetStockObject(SYSTEM_FONT));
	DeleteObject(hFont);

	ReleaseDC(hwnd, hdc);

	Sleep(2200);
	DestroyWindow(hwnd);
}

void DrawBricks(HWND hwnd, long cxClient, long cyClient, RECT *rcArrayBricks, HRGN *hRgnArrayBricks) {
	
	HBRUSH hBrush;
	HDC hdc;
	//HRGN hRgnArrayBricks;

	int xHalfOfQuad = ((cxClient / 2) / 2) / 2; 
	int yHalfOfQuad = ((cyClient / 2) / 2) / 2;

	int xTemp = 0; 
	int yTemp = 0;


	hdc = GetDC(hwnd);
	
	for (int i = 0; i < 6; i++) {
		if (i < 3) {
			xTemp = xTemp + xHalfOfQuad;
			yTemp = yTemp + yHalfOfQuad;
			hBrush = CreateSolidBrush(RGB(160, 82, 45));
			hRgnArrayBricks[i] = CreateRectRgn((xTemp), (yTemp), xTemp + 50, yTemp + 20);
			GetRgnBox(hRgnArrayBricks[i], &rcArrayBricks[i]);
			FillRgn(hdc, hRgnArrayBricks[i], hBrush);
		}
		else {
			xTemp = xTemp + xHalfOfQuad;
			hBrush = CreateSolidBrush(RGB(160, 82, 45));
			hRgnArrayBricks[i] = CreateRectRgn((xTemp), (yTemp), xTemp + 50, yTemp + 20);
			FillRgn(hdc, hRgnArrayBricks[i], hBrush);
			GetRgnBox(hRgnArrayBricks[i], &rcArrayBricks[i]);
			yTemp = yTemp - yHalfOfQuad;
		}
	}

	ReleaseDC(hwnd, hdc);
}


void BallBricksCollision(HWND hwnd, HRGN B_hrgn, RECT rcBall, RECT *rcArrayBricks, HRGN *hRgnArrayBricks, long xHalf, long yHalf, long cxRadius, long cyRadius, long *cxMove, long *cyMove, long cxClient, long cyClient) {

	HDC hdc;
	HRGN D_hRgn;
	HBRUSH hBrush;

	hdc = GetDC(hwnd);

		if (!(RectInRegion(hRgnArrayBricks[0],&rcBall))) {
			hBrush = CreateSolidBrush(RGB(0,0,0));
			FillRgn(hdc, hRgnArrayBricks[0], hBrush);
		}
	ReleaseDC(hwnd, hdc);
}