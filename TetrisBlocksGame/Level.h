#ifndef LEVEL_H
#define LEVEL_H

#include "DrawEngine.h"
#include "PieceSet.h"
#include <windows.h>
#include <timeapi.h>

class Level
{

protected:
	bool place(int x, int y, const Piece &piece);

	void clear(const Piece& piece);

	void dropRandomPiece();

	bool isHitBottom() const;
	bool isHitLeft() const;
	bool isHitRight() const;

	bool isCovered(const Piece &piece, int x, int y) const;

	int clearRows();

	COLORREF **board;    
	DrawEngine &de;      
	PieceSet pieceSet;   
	Piece *current;      
	Piece *next;         
	int width;           
	int height;          
	int posX;            
	int posY;            
	int speed;           
	double lastTime;     
	double currentTime;  
	int score;           

public:
	
	Level(DrawEngine &de, int width = 10, int height = 20);
	~Level();

	void drawBoard() const;

	bool rotate();

	bool move(int cxDistance, int cyDistance);

	void timerUpdate()
	{
		currentTime = timeGetTime();
		
		if (currentTime - lastTime < speed)
			return;

		// If player gets score, increase more speed
		if (current == NULL || !move(0, -1))
		{
			
			int lines = clearRows();
			speed = max(speed - 2 * lines, 100);
			score += 1 + lines * lines * 5;
			dropRandomPiece();
			drawScore();
			drawSpeed();
			drawNextPiece();
		}

		lastTime = timeGetTime();
	}

	bool isGameOver();

	// Info
	void drawSpeed() const;
	void drawScore() const;
	void drawNextPiece() const;
};

#endif // LEVEL_H

