#pragma once

#include "BaseApp.h"

#include <vector>

#define FIGURES_COUNT 7

// Standart figure types in order to use
// standartFigures array more comfortable
enum class StandartFigureTypes : char
{
	Cleverland = 0,		// Z
	OrangeRicky,		// L
	Smashboy,			// O
	RhodeIsland,		// S
	Hero,				// I
	BlueRicky,			// J
	Teewee				// T
};

// figures point
// table for this points:
// ---------
// | 0 | 1 |
// | 2 | 3 |
// | 4 | 5 |
// | 6 | 7 |
// ---------
const short standartFiguresCellsCount = 4;
const short standartFiguresCellsInRow = 2;
const short standartFigures[FIGURES_COUNT][4] = {
	{ 1, 2, 3, 4 },  // Z
	{ 0, 2, 4, 5 },  // L
	{ 0, 1, 2, 3 },	 // O
	{ 0, 3, 2, 5 },  // S
	{ 1, 3, 5, 7 },  // I
	{ 1, 3, 4, 5 },  // J
	{ 1, 2, 3, 5 }	 // T
};

class Figure
{
private:
	struct Pixel
	{
		COORD coord;
		wchar_t graphic;
	};

	std::vector<Pixel> _pixels;	// how our figure looks like;
	bool _canRotate = true;

public:
	// default constructor
	Figure(void) {}

	// cellInRow - count of cells in each row;
	// figurePixelsCount - how many pixels figure states;
	// pixelGraphic - which character will have pixel;
	// 
	// every figure can rotate by default;
	// 
	// example table for making figure:
	// ---------
	// | 0 | 1 |
	// | 2 | 3 |
	// | 4 | 5 |
	// | 6 | 7 |
	// ---------
	// there is 2 cells in each row!;
	Figure(const short* cellNumbers, const short cellsInRow, const short figurePixelsCount, const wchar_t pixelGraphic);

	// cellInRow - count of cells in each row;
	// figurePixelsCount - how many pixels figure states;
	// 
	// every figure can rotate by default;
	// 
	// example table for making figure:
	// ---------
	// | 0 | 1 |
	// | 2 | 3 |
	// | 4 | 5 |
	// | 6 | 7 |
	// ---------
	// there is 2 cells in each row!;
	Figure(const short* cellNumbers, const short cellsInRow, short figurePixelsCount) : Figure(cellNumbers, cellsInRow, figurePixelsCount, L'▧') {}

	// Add figure to console buffer;
	// 
	// appWindow - where is console buffer contain;
	void DrawFigure(BaseApp* appWindow);

	// Delete figure last pusition from buffer;
	//
	// appWindow - where is console buffer contain;
	void ClearLastPositionFromBuffer(BaseApp* appWindow);

	// Move figure By X and Y coord;
	void Move(const short moveByX, const short moveByY);

	void SetPosition(const short startCoordX, const short startCoordY);

	// return coord of every pixel of figure;
	//
	// figureCoord - where we write these coords;
	void GetPixelsCoord(std::vector<COORD>& figureCoord) const;

	// rotate figure;
	//
	// degree - degree of rotation;
	void Rotate(int degree);

	// Can figure rotate or not?
	//
	// just make it possible or unable to rotate!
	void ChangeRotationStatus(bool canRotate);

	wchar_t GetPixelGraphic(void);
};

