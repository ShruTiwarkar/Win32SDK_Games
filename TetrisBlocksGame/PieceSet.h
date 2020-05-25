#ifndef PIECESET_H
#define PIECESET_H

#include "Piece.h"

/*
* 7 types of pieces.4 rotations each
* 7 x 4 = 28 total.
*/

class PieceSet
{
public:
	
	enum { NUM_ROTATIONS = 4, NUM_PIECES = 7 };

	PieceSet();
	~PieceSet();

	Piece *getPiece(int id, int rotation = 0) const;

	// random piece
	inline Piece *getRandomPiece() const
	{
		return getPiece(rand() % NUM_PIECES, rand() % NUM_ROTATIONS);
	}

protected:
	// 28 pieces store
	Piece *pieces[NUM_PIECES][NUM_ROTATIONS];

	void rotateAll();

	// Rotates 90 degrees counterclockwise
	void rotate(POINT *apt, int numPoints = 4);
};

#endif // PIECESET_H

