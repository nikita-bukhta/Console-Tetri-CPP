// Copyright 2009-2014 Blam Games, Inc. All Rights Reserved.

#pragma once

#include "BaseApp.h"
#include "Figure.h"

#include <array>
#include <vector>

class TestApp : public BaseApp
{
	typedef BaseApp Parent;

	private:
		std::array<Figure, FIGURES_COUNT> _playingFigures;
		std::array<Figure, 2> _actiongFigures;

		std::vector<std::vector<wchar_t>> _calculationField;
		
		int _score = 0;
		bool _gameOver = false;
		
		// return random number in range [min; max];
		short Random(short min, short max);

		// set new playing and next figure;
		void SetNewFigures(void);

		// add pushed figures to console buffer;
		void DrawCalculationInfo(COORD startCoord);

		void DrawTotalScore(void);

		int DestroyFilledRows(void);

		bool CheckForGameOver(void);

	public:
		TestApp();
		virtual void KeyPressed(int btnCode);
		virtual void UpdateF(float deltaTime);
};