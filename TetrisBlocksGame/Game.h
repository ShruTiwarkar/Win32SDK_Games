#ifndef GAME_H
#define GAME_H

#include "Level.h"
#include "PieceSet.h"
#include "Piece.h"

class Game
{

protected:
	// GAME OVER message
	inline void drawGameOver() const
	{

		de.drawText(TEXT("GAME OVER"), 3, 10);
		de.drawText(TEXT("Press ENTER to restart"), 2, 9);
	}

	// PAUSE message
	inline void drawPause() const
	{
		de.drawText(TEXT("PAUSE"), 4, 10);
		de.drawText(TEXT("Press PAUSE again to continue"), 1, 9);
	}

	Level *level;
	DrawEngine &de;

	bool isPaused;


public:
	Game(DrawEngine &de);
	~Game();

	void restart();
	bool keyPress(int vk);

	void timerUpdate();

	void pause(bool paused);

	//WM_PAINT
	void repaint() const;

	bool isGameOver() const;
};

#endif // GAME_H

