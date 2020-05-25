#ifndef PIECE_H
#define PIECE_H

#include <windows.h>

class Piece
{

protected:

	POINT *body;
	int nPoints;
	int width;
	int height;
	int id;
	int rotation;
	COLORREF color;

public:
	
	Piece(int pieceId, int pieceRotation, COLORREF pieceColor,const POINT *apt, int numPoints = 4);
	~Piece();

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
	inline int getNPoints() const { return nPoints; }
	inline int getId() const { return id; }
	inline int getRotation() const { return rotation; }
	inline COLORREF getColor() const { return color; }

	void getBody(POINT *apt) const;

	int getSkirt(POINT *apt) const;
	int getLeftSide(POINT *apt) const;
	int getRightSide(POINT *apt) const;

	bool isPointExists(int x, int y) const;

	void print() const;
};

#endif // PIECE_H

