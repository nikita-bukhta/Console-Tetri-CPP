// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#include "TestApp.h"
#include "Frame.h"
#include "Config.h"
#include "Figure.h"

#include <random>
#include <conio.h>
#include <winuser.h>

short TestApp::Random(short min, short max)
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<> range(min, max);

	return range(generator);
}

void TestApp::SetNewFigures(void)
{
	_actiongFigures[0] = _actiongFigures[1];
	_actiongFigures[1] = _playingFigures[Random(0, FIGURES_COUNT - 1)];

	// centre of tetris field;
	_actiongFigures[0].SetPosition(Config::tetrisFrameSize.width / 2, 1);
	// centre of field with next figure;
	_actiongFigures[1].SetPosition(Config::totalWindowSize.width - Config::nextFigureSpaceSize.width / 2, Config::nextFigureSpaceSize.height / 4);
}

void TestApp::DrawCalculationInfo(COORD startCoord)
{
	short rowsCount = short(_calculationField.size());
	short columsCount = short(_calculationField[0].size());
	for (short i = 0; i < rowsCount; ++i)
	{
		for (short j = 0; j < columsCount; ++j)
		{
			if (_calculationField[i][j] != 0)
				SetChar(startCoord.X + j, startCoord.Y + i, _calculationField[i][j]);
		}
	}
}

void TestApp::DrawTotalScore(void)
{
	std::wstring scoreTitle = L"Score: ";
	scoreTitle += std::to_wstring(_score);

	short titleSize = short(scoreTitle.size());
	for (short i = 0; i < titleSize; ++i)
		SetChar(3 + i, Config::tetrisFrameSize.height + Config::scoreSpaceSize.height / 2 - 1, scoreTitle[i]);
}

int TestApp::DestroyFilledRows(void)
{
	static int scoreForOneLine = 100;
	int destroyedLines = 0;
	auto pixelGraphic = _actiongFigures[0].GetPixelGraphic();

	short rowsCount = short(_calculationField.size());
	short columsCount = short(_calculationField[0].size());
	for (short i = (rowsCount - 1); i >= 0; --i)
	{
		bool lineIsFilled = true;
		// check if this row is filled;
		for (short j = 0; j < columsCount; ++j)
		{
			auto c = _calculationField[i][j];
			if (_calculationField[i][j] != pixelGraphic)
			{
				lineIsFilled = false;
				break;
			}
		}

		if (lineIsFilled)
		{
			++destroyedLines;

			// destroy row;
			for (short j = 0; j < columsCount; ++j)
				_calculationField[i][j] = 0;

			// move others rows down :);
			for (short row = i; row > 0; --row)
				_calculationField[row].swap(_calculationField[row - 1]);
		}
	}

	return 100 * destroyedLines;
}

bool TestApp::CheckForGameOver(void)
{
	std::vector<COORD> pixelsCoord;
	_actiongFigures[0].GetPixelsCoord(pixelsCoord);

	for (auto& pixelCoord : pixelsCoord)
	{
		if (!_gameOver && (pixelCoord.Y - 2) <= 0)
		{
			_gameOver = true;
			return true;
		}
	}

	return false;
}

TestApp::TestApp() : Parent(Config::totalWindowSize.width, Config::totalWindowSize.height)
{
	for (short i = 0; i < FIGURES_COUNT; ++i)
		_playingFigures[i] = Figure(standartFigures[i], standartFiguresCellsInRow, standartFiguresCellsCount);

	// square can't rotate, so just put away this ability;
	_playingFigures[int(StandartFigureTypes::Smashboy)].ChangeRotationStatus(false);

	short actionFiguresCount = short(_actiongFigures.size());
	for (short i = 0; i < actionFiguresCount; ++i)
		_actiongFigures[i] = _playingFigures[Random(0, FIGURES_COUNT - 1)];

	_actiongFigures[0].SetPosition(Config::tetrisFrameSize.width / 2, 1);
	_actiongFigures[1].SetPosition(Config::totalWindowSize.width - Config::nextFigureSpaceSize.width / 2, Config::nextFigureSpaceSize.height / 4);

	// -2 because these is frames colums;
	for (short i = 0; i < (Config::tetrisFrameSize.height - 2); ++i)
	{
		std::vector<wchar_t> temp;
		for (short j = 0; j < (Config::tetrisFrameSize.width - 2); ++j)
			temp.push_back(0);

		_calculationField.push_back(temp);
	}
}

void TestApp::KeyPressed(int btnCode)
{
	bool inGameFrame = true;	// if figure in game frame;
	auto& currentFifure = _actiongFigures[0];	// current playing figure;
	std::vector<COORD> currentFigureCoord;	// coord of current playing figure;
	COORD moveBy = COORD();		// move current figure by...;
	static auto pixelGraphic = _actiongFigures[0].GetPixelGraphic();	// graphic of figures;

	switch (btnCode)
	{
	// arrows
	case 224:
		btnCode = _getch();
		currentFifure.GetPixelsCoord(currentFigureCoord);

#pragma region Check arrows
		switch (btnCode)
		{
		// LEFT;
		case 75:
			// check if future figure will be in frame;
			for (auto& pixelCoord : currentFigureCoord)
			{
				// pixelCoord.Y - 1 - remove game frame;
				// pixelCoord.X - 1 - 1	remove game frame and move left
				//	in order to check for another figure;
				if ((pixelCoord.X - 1) < 1 ||
					_calculationField[pixelCoord.Y - 1][pixelCoord.X - 2] == pixelGraphic)
				{
					inGameFrame = false;
					break;
				}
			}
			if (inGameFrame)
				moveBy = { -1, 0 };	// move right

			break;
		// RIGHT;
		case 77:
			// check if future figure will be in frame;
			for (auto& pixelCoord : currentFigureCoord)
			{
				// pixelCoord.Y - 1 - remove game frame;
				// pixelCoord.X - 1 + 1	remove game frame and move right
				//	in order to check for another figure;
				if ((pixelCoord.X + 1) >= (Config::tetrisFrameSize.width - 1) ||
					_calculationField[pixelCoord.Y - 1][pixelCoord.X] == pixelGraphic)
				{
					inGameFrame = false;
					break;
				}
			}
			if (inGameFrame)
				moveBy = { 1, 0 };	// move right

			break;
		// DOWN;
		case 80:
			// check if future figure will be in frame;
			for (auto& pixelCoord : currentFigureCoord)
			{
				// pixelCoord.Y - 1 + 1- remove game frame and move down;
				//	in order to check for another figure;
				// pixelCoord.X - 1 + 1	remove game frame;
				if ((pixelCoord.Y + 1) >= (Config::tetrisFrameSize.height - 2) ||
					_calculationField[pixelCoord.Y][pixelCoord.X - 1] == pixelGraphic)
				{
					inGameFrame = false;
					break;
				}
			}
			if (inGameFrame)
				moveBy = { 0, 1 };
			else
				CheckForGameOver();
		}
#pragma endregion

		currentFifure.ClearLastPositionFromBuffer(this);
		currentFifure.Move(moveBy.X, moveBy.Y);
		break;

	// SPACE
	case 32:
		auto temp = currentFifure;
		temp.Rotate(90);
		temp.GetPixelsCoord(currentFigureCoord);

		// TODO:
		//	1) Check for Y position;
		//	2) Check for another figures below;
		for (auto pixel : currentFigureCoord)
		{
			// check if our rotation don't push any other figures or is out of game frame;
			if (!(pixel.X > 0 && pixel.X < (Config::tetrisFrameSize.width - 1)) ||
				!(pixel.Y > 1 && pixel.Y < (Config::tetrisFrameSize.height - 1)) ||
				_calculationField[pixel.Y - 2][pixel.X - 1] == pixelGraphic)
				inGameFrame = false;
		}

		if (inGameFrame)
		{
			currentFifure.ClearLastPositionFromBuffer(this);
			currentFifure = temp;
		}
		break;
	}
}

void TestApp::UpdateF(float deltaTime)
{
#pragma region Game frame drawing
	static Frame gameFrame(Config::tetrisFrameSize, Frame::FrameChar::Dot);
	static Frame informationFrame(Config::nextFigureSpaceSize);
	static Frame scoreFrame(Config::scoreSpaceSize);
	static bool frameHasBeenCreated = false;

	if (!frameHasBeenCreated)
	{
		// Chenge some characters in frame in order to make more beautiful image;
		informationFrame.ChangeChar(0, 0, Frame::FrameChar::GoDown);
		scoreFrame.ChangeChar(0, 0, Frame::FrameChar::GoRight);
		scoreFrame.ChangeChar(0, Config::tetrisFrameSize.width - 1, Frame::FrameChar::GoUp);
		scoreFrame.ChangeChar(0, Config::totalWindowSize.width, Frame::FrameChar::GoLeft);
	}

	// draw frames
	gameFrame.DrawFrame(0, 0, this);
	informationFrame.DrawFrame(COORD{ short(Config::tetrisFrameSize.width - 1) , 0 }, this);
	scoreFrame.DrawFrame(0, short(Config::tetrisFrameSize.height - 1), this);
#pragma endregion
#pragma region Move figure down
	if (deltaTime >= _timeToFigureFall)
	{
		auto temp = _actiongFigures[0];
		temp.Move(0, 1);
		
		std::vector<COORD> pixelsCoord;
		temp.GetPixelsCoord(pixelsCoord);
		static auto pixelGraphic = _actiongFigures[0].GetPixelGraphic();

		bool canMoveDown = true;
		for (auto& pixelCoord : pixelsCoord)
		{
			if (pixelCoord.Y >= (Config::tetrisFrameSize.height - 1) || 
				_calculationField[pixelCoord.Y - 1][pixelCoord.X - 1] == pixelGraphic)
			{
				canMoveDown = false;
				break;
			}
		}

		if (canMoveDown)
		{
			_actiongFigures[0].ClearLastPositionFromBuffer(this);
			_actiongFigures[0] = temp;
		}
		else
		{
			// IDK why -2, but in this way we get the right COORD for calculation field;
			for (auto& pixelCoord : pixelsCoord)
				_calculationField[pixelCoord.Y - 2][pixelCoord.X - 1] = pixelGraphic;

			CheckForGameOver();

			SetNewFigures();
		}
	}
#pragma endregion

#pragma region Draw figures and score;
	_score += DestroyFilledRows();
	DrawTotalScore();
	DrawCalculationInfo(COORD{ 1, 1 });
	_actiongFigures[0].DrawFigure(this);
	_actiongFigures[1].DrawFigure(this);

	if (_gameOver)
	{
		std::wstring scoreTitle = L"Totle score: ";
		scoreTitle += std::to_wstring(_score);
		wchar_t* buf = &scoreTitle[0];

		MessageBox(NULL, buf, L"Game over!", MB_OK);
		exit(0);
	}
#pragma endregion
}