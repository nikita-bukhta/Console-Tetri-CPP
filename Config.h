#pragma once

namespace Config
{
	const double PI = 3.14159265;

	struct Size
	{
		short width;
		short height;

		bool operator == (const Size& other) const;
		bool operator != (const Size& other) const;
	};

	// + 2 for frames;
	const Size tetrisFrameSize = { 15 + 2, 20 + 2};
	const Size nextFigureSpaceSize = { 9 + 2, 20 + 2 };
	const Size scoreSpaceSize = {
		tetrisFrameSize.width + nextFigureSpaceSize.width - 1,
		5
	};

	const Size totalWindowSize =
	{
		tetrisFrameSize.width + nextFigureSpaceSize.width - 2,
		tetrisFrameSize.height + scoreSpaceSize.height - 2
	};
};