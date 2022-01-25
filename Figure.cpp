#include "Figure.h"
#include "Config.h"

Figure::Figure(const short* cellNumbers, const short cellsInRow, const short figurePixelsCount, const wchar_t pixelGraphic)
{
	for (short cellIndex = 0; cellIndex < figurePixelsCount; ++cellIndex)
		_pixels.push_back(Pixel{ short(cellNumbers[cellIndex] % 2), short(cellNumbers[cellIndex] / 2), pixelGraphic});
}

void Figure::DrawFigure(BaseApp* appWindow)
{
	for (auto& pixel : _pixels)
		appWindow->SetChar(pixel.coord.X, pixel.coord.Y, pixel.graphic);
}

void Figure::ClearLastPositionFromBuffer(BaseApp* appWindow)
{
	for (auto& pixel : _pixels)
		appWindow->SetChar(pixel.coord.X, pixel.coord.Y, L' ');
}

void Figure::Move(const short moveByX, const short moveByY)
{
	for (auto& pixel : _pixels)
	{
		pixel.coord.X += moveByX;
		pixel.coord.Y += moveByY;
	}
}

void Figure::GetPixelsCoord(std::vector<COORD>& figureCoord) const
{
	for (auto& pixel : _pixels)
		figureCoord.push_back(pixel.coord);
}

void Figure::Rotate(int degree)
{
	if (!_canRotate)
		return;

	COORD rotationCenterCoord = _pixels[1].coord;
	// Point& p_0 = rotationCenter;
	const double radian = degree * Config::PI / 180.0;
	const float rotationSin = (float)sin(radian);
	const float rotationCos = (float)cos(radian);
	for (auto& pixel : _pixels)
	{
		// formula for rotation
		// X = x_0 + (x − x_0) * cos⁡(a) −(y − y_0) * sin⁡(a);
		// Y = y_0 + (y − y_0) * cos⁡(a) + (x − x_0) * sin⁡(a);
		// where:
		//	x and y		- current pixel coord
		//	x_0 and y_0 - rotation center
		//	a - radian count
		COORD currentPixelCoord = { pixel.coord.X, pixel.coord.Y };

		pixel.coord.X = int(rotationCenterCoord.X +
			(currentPixelCoord.X - rotationCenterCoord.X) * rotationCos -
			(currentPixelCoord.Y - rotationCenterCoord.Y) * rotationSin);
		pixel.coord.Y = int(rotationCenterCoord.Y +
			(currentPixelCoord.Y - rotationCenterCoord.Y) * rotationCos +
			(currentPixelCoord.X - rotationCenterCoord.X) * rotationSin);
	}
}

void Figure::ChangeRotationStatus(bool canRotate)
{
	_canRotate = canRotate;
}

void Figure::SetPosition(const short startCoordX, const short startCoordY)
{
	Move(startCoordX - _pixels[0].coord.X, startCoordY - _pixels[0].coord.Y);
}

wchar_t Figure::GetPixelGraphic(void)
{
	return _pixels[0].graphic;
}
