#pragma once

#include "Config.h"
#include "BaseApp.h"

using namespace Config;

class Frame
{
public:
	// char codes for frame;
	enum class FrameChar : short
	{
		VerticalStick = L'║',
		HorizontStick = L'═',
		UpRightCorner = L'╗',
		UpLeftCorner = L'╔',
		DownRightCorner = L'╝',
		DownLeftCorner = L'╚',
		GoLeft = L'╣',
		GoRight = L'╠',
		GoUp = L'╩',
		GoDown = L'╦',
		Cross = L'╬',
		Space = L' ',
		Dot = L'.'
	};

protected:
	Size _frameSize;
	FrameChar** _frame;
	FrameChar _freeSpaceFeeling;

	// fill _frame with characters;
	virtual void MakeGameFrame(void);

public:
	Frame(void);
	Frame(const Size& gameFrameSize, FrameChar freeSpace = FrameChar::Space);
	Frame(const short width, const short height, FrameChar freeSpace = FrameChar::Space) : Frame(Size{ width, height}, freeSpace) {}
	Frame(const Frame& other);

	~Frame(void);

	Frame operator= (const Frame& other);

	// Change width of frame;
	void SetGameFrameWidth(const short width);
	// Change height of frame;
	void SetGameFrameHeight(const short height);

	// return size of frame;
	Size GetFrameSize(void) const;

	// Push game frame in the console buffer;
	//
	// startCoord - where you want to draw this frame;
	// appWindow - where we want to draw it;
	void DrawFrame(const COORD& startCoord, BaseApp* appWindow) const;
	// Push game frame in the console buffer;
	//`
	// coordX && coordY - where you want to draw this frame;
	// appWindow - where we want to draw it;
	void DrawFrame(const short coordX, const short coordY, BaseApp* appWindow) const;

	// change character in game frame in specific coord;
	//
	// row - coord of index i;
	// column - coord of index j;
	// character - we want change to this character;
	//
	// return false if coord out of range;
	// return true if coord in range and changing was happen successfully;
	bool ChangeChar(const unsigned short row, const unsigned short column, FrameChar character);
};