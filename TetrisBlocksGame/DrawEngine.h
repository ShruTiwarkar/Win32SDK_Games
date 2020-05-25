#ifndef DRAW_ENGINE_H
#define DRAW_ENGINE_H

#include "Piece.h"
#include <windows.h>

class DrawEngine
{

protected:
	HDC hdc;     
	HWND hwnd;   
	RECT rect;   
	int width;   
	int height;  

public:
	
	DrawEngine(HDC hdc, HWND hwnd, int pxPerBlock = 25,int width = 10, int height = 20);
	~DrawEngine();

	void drawBlock(int x, int y, COLORREF color);

	// (gray area)
	void drawInterface();

	// Draws a text message
	void drawText(const wchar_t *szText, int x, int y) const;

	// Display info
	void drawScore(int score, int x, int y) const;
	void drawSpeed(int speed, int x, int y) const;
	void drawNextPiece(Piece &piece, int x, int y);

};

#endif // DRAW_ENGINE_H

