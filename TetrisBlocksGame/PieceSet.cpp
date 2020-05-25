#include "PieceSet.h"

PieceSet::PieceSet()
{
	for (int i = 0; i < NUM_PIECES; i++)
		for (int j = 0; j < NUM_ROTATIONS; j++)
			pieces[i][j] = 0;

	POINT apt[NUM_ROTATIONS];

	// index = 0, I piece, red
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 0;    apt[1].y = 1;
	apt[2].x = 0;    apt[2].y = 2;
	apt[3].x = 0;    apt[3].y = 3;
	pieces[0][0] = new Piece(0, 0, RGB(255, 0, 0), apt);

	// index 1, L piece, yellow
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 1;    apt[1].y = 0;
	apt[2].x = 0;    apt[2].y = 1;
	apt[3].x = 0;    apt[3].y = 2;
	//RGB(230,130,24)
	pieces[1][0] = new Piece(1, 0, RGB(255, 255, 0), apt);

	// index = 2, counter-L piece, white
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 1;    apt[1].y = 0;
	apt[2].x = 1;    apt[2].y = 1;
	apt[3].x = 1;    apt[3].y = 2;
	pieces[2][0] = new Piece(2, 0, RGB(255, 255, 255), apt);

	// index = 3, S piece, light blue
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 1;    apt[1].y = 0;
	apt[2].x = 1;    apt[2].y = 1;
	apt[3].x = 2;    apt[3].y = 1;
	// RGB(122, 0, 0) maroon
	pieces[3][0] = new Piece(3, 0, RGB(122, 255, 255), apt);

	// index = 4, Z piece, green
	apt[0].x = 1;    apt[0].y = 0;
	apt[1].x = 2;    apt[1].y = 0;
	apt[2].x = 0;    apt[2].y = 1;
	apt[3].x = 1;    apt[3].y = 1;
	// RGB(100, 180, 255) // persian blue
	pieces[4][0] = new Piece(4, 0, RGB(122, 255, 0), apt);

	// index = 5, Square piece, dark blue
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 1;    apt[1].y = 0;
	apt[2].x = 0;    apt[2].y = 1;
	apt[3].x = 1;    apt[3].y = 1;
	pieces[5][0] = new Piece(5, 0, RGB(20, 100, 200), apt);

	// index = 6, T piece, purple
	apt[0].x = 0;    apt[0].y = 0;
	apt[1].x = 1;    apt[1].y = 0;
	apt[2].x = 2;    apt[2].y = 0;
	apt[3].x = 1;    apt[3].y = 1;
	pieces[6][0] = new Piece(6, 0, RGB(220, 180, 255), apt);

	// Create piece rotations
	rotateAll();
}

PieceSet::~PieceSet()
{
	for (int i = 0; i < NUM_PIECES; i++)
		for (int j = 0; j < NUM_ROTATIONS; j++)
			if (pieces[i][j] != 0)
				delete pieces[i][j];
}

Piece *PieceSet::getPiece(int id, int rotation) const
{
	if (id >= NUM_PIECES || id < 0 || rotation >= NUM_ROTATIONS || rotation < 0)
		return NULL;
	return pieces[id][rotation];
}

void PieceSet::rotateAll()
{
	POINT apt[NUM_ROTATIONS];
	for (int i = 0; i < NUM_PIECES; i++)
	{
		pieces[i][0]->getBody(apt);
		for (int j = 1; j < NUM_ROTATIONS; j++)
		{
			rotate(apt);
			if (pieces[i][j] != 0)
				delete pieces[i][j];
			pieces[i][j] = new Piece(i, j, pieces[i][0]->getColor(), apt);
		}
	}
}

void PieceSet::rotate(POINT* apt, int numPoints)
{
	int tmp;

	// X' = -Y
	// Y' = X
	for (int i = 0; i < numPoints; i++)
	{
		tmp = apt[i].x;
		apt[i].x = -apt[i].y;
		apt[i].y = tmp;
	}
}

