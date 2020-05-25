#include "Game.h"
#include <iostream>

using namespace std;

Game::Game(DrawEngine &de) :
	de(de), isPaused(false)
{
	level = new Level(de, 10, 20);
}

Game::~Game()
{
	delete level;
}


void Game::restart()
{
	delete level;
	level = new Level(de, 10, 20);
	isPaused = false;
	repaint();
}

bool Game::keyPress(int vk)
{
	
	if (vk != VK_PAUSE && vk != VK_RETURN && isPaused)
		return false;

	switch (vk)
	{
	// Up Arrow Key
	case VK_UP:
		level->rotate();
		break;
	// Down Arraw Key
	case VK_DOWN:
		level->move(0, -1);
		break;
	// Left Arrow Key
	case VK_LEFT:
		level->move(-1, 0);
		break;
	// Right Arraw Key
	case VK_RIGHT:
		level->move(1, 0);
		break;
	// Space Bar
	case VK_SPACE:
		level->rotate();
		break;
	// Fn + P
	case VK_PAUSE:
		pause(!isPaused);
		break;
	case VK_RETURN:
		// Restart on game over
		if (level->isGameOver())
			restart();
	default:
		return false;
	}
	return true;
}

void Game::timerUpdate()
{
	// No time Change when pause
	if (isPaused)
		return;

	// GAME OVER
	if (level->isGameOver()) {
		isPaused = true;
		drawGameOver();
		return;
	}

	level->timerUpdate();
	level->drawBoard();
}

void Game::pause(bool paused)
{
	if (isGameOver())
		return;

	isPaused = paused;
	if (paused)
		drawPause();
	level->drawScore();
	level->drawSpeed();
}

void Game::repaint() const
{

	de.drawInterface();
	level->drawScore();
	level->drawSpeed();
	level->drawNextPiece();
	level->drawBoard();
	if (level->isGameOver())
		drawGameOver();
	else if (isPaused)
		drawPause();
}

bool Game::isGameOver() const
{
	return level->isGameOver();
}


