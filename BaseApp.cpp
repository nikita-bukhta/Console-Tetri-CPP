// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include "TestApp.h"
#include "Frame.h"
#include "Config.h"

#include <algorithm>
#include <time.h>
#include <conio.h>
#include <assert.h>
#include <strsafe.h>
#include <cmath>

#define MY_PERFORMENCE_COUNTER

#include "PerformanceCounter.h"

BaseApp::BaseApp(int xSize, int ySize) : X_SIZE(xSize), Y_SIZE(ySize)
{
	SMALL_RECT windowSize = { 0, 0, short(X_SIZE), short(Y_SIZE) };
	COORD windowBufSize = { short(X_SIZE + 1), short(Y_SIZE + 1) };

	mConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	mConsoleIn = GetStdHandle(STD_INPUT_HANDLE);

	SMALL_RECT const minimal_window = { 0, 0, 1, 1 };
	// in order to the window buffer can't be smaller than the internal buffer;
	// answer I have found there:
	//	https://stackoverflow.com/questions/12900713/reducing-console-size#:~:text=The%20window%20buffer%20cannot%20be,usually%2087%20'invalid%20argument'.
	if (!SetConsoleWindowInfo(mConsole, TRUE, &minimal_window))
		cout << "SetConsoleScreenBufferSize Minimal window failed with error " << GetLastError() << endl;

	if(!SetConsoleScreenBufferSize(mConsole,  windowBufSize))
		cout << "SetConsoleScreenBufferSize failed with error " << GetLastError() << endl;
	if(!SetConsoleWindowInfo(mConsole, TRUE, &windowSize))
		cout << "SetConsoleWindowInfo failed with error " << GetLastError() << endl;

	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(mConsole, &cursorInfo);
	cursorInfo.bVisible = FALSE;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(mConsole, &cursorInfo);

	mChiBuffer = (CHAR_INFO*)malloc((X_SIZE+1)*(Y_SIZE+1)*sizeof(CHAR_INFO));

	mDwBufferSize.X = X_SIZE + 1;
	mDwBufferSize.Y = Y_SIZE + 1;		// data buffer size;

	mDwBufferCoord.X = 0;
	mDwBufferCoord.Y = 0;				// the cell coord;

	mLpWriteRegion.Left = 0;
	mLpWriteRegion.Top = 0;
	mLpWriteRegion.Right = X_SIZE + 1;
	mLpWriteRegion.Bottom = Y_SIZE + 1;	// the rectangle for reading;


	for (int x=0; x<X_SIZE+1; x++)
	{
		for (int y=0; y<Y_SIZE+1; y++)
		{
			SetChar(x, y, L' ');
		}
	}
}

BaseApp::~BaseApp()
{
	free(mChiBuffer);
}

void BaseApp::SetChar(int x, int y, wchar_t c)
{
	mChiBuffer[x + (X_SIZE+1)*y].Char.UnicodeChar = c;
	mChiBuffer[x + (X_SIZE+1)*y].Attributes = FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_RED;
}

wchar_t BaseApp::GetChar(int x, int y)
{
	return mChiBuffer[x + (X_SIZE+1)*y].Char.AsciiChar;
}

void BaseApp::Render()
{
	if (!WriteConsoleOutput(mConsole, mChiBuffer, mDwBufferSize, mDwBufferCoord, &mLpWriteRegion)) 
	{
		printf("WriteConsoleOutput failed - (%d)\n", GetLastError()); 
	}
}

void BaseApp::Run()
{
	CStopwatch timer;
	CStopwatch fpsTimer;

	int fps = 0;

	int deltaTime = 0;

	timer.Start();
	fpsTimer.Start();
	while (1)
	{
		if (_kbhit())
		{
			KeyPressed(_getch());
			if (!FlushConsoleInputBuffer(mConsoleIn))
				cout<<"FlushConsoleInputBuffer failed with error "<<GetLastError();
		}

		deltaTime = timer.Now();

		// if one second left output fps;
		if (fpsTimer.Now() >= CLOCKS_PER_SEC)
		{
			TCHAR  szbuff[255];
			StringCchPrintf(szbuff, 255, TEXT("FPS: %d"), fps);
			SetConsoleTitle(szbuff);

			fps = 0;
			fpsTimer.Start();
		}

		UpdateF((float)deltaTime / CLOCKS_PER_SEC);
		Render();

		if ((float(deltaTime) / CLOCKS_PER_SEC) >= _timeToFigureFall)
		{
			timer.Start();
			deltaTime = 0;
		}

		++fps;
	}
}