// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include "Config.h"

#include <iostream>
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

class BaseApp
{
private:
	HANDLE mConsole;
	HANDLE mConsoleIn;

	CHAR_INFO* mChiBuffer;
	COORD mDwBufferSize;
	COORD mDwBufferCoord;
	SMALL_RECT mLpWriteRegion;

	void Render();

protected:
	float _timeToFigureFall = 0.3f;	// time to fall figure in seconds;

public:
	// Horizontal and vertical output area sizes in characters
	const int X_SIZE;
	const int Y_SIZE;

	// the arguments are the horizontal and vertical size of the output area in characters
	BaseApp(int xSize=100, int ySize=80);
	virtual ~BaseApp();

	// Starts the game cycle
	void Run();

	// you can fill the x,y-character of the screen with a certain character, or read it
	void SetChar(int x, int y, wchar_t c);
	wchar_t GetChar(int x, int y);

	/* this function is called every game iteration, it can be overridden, in the class heir.
		it gets deltaTime - the time difference between the previous iteration and this one, in seconds*/
	virtual void UpdateF (float deltaTime){}
	/* This function is called when you press a key on the keyboard, it receives a key code - btnCode.
		If you use arrows or function keys, you will get a common number for them, e.g. 224. 
		Next comes the code of the key itself (you can get it by calling getch() method).
		You can also override the KeyPressed method in the inherited class.
	 */
	virtual void KeyPressed (int btnCode){}
};