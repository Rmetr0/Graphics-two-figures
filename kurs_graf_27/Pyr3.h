#pragma once

#include "Figure.h"

enum class PYR3
{
	X0 = 150,
	Y0 = 0,
	Z0 = 0,
	Width = 200,
	Height = 100,
	Depth = -100,
	Shift_X = Width / 2,
	Shift_Z = Depth / 2,
	Fill = RGB_BMP(0, 255, 0),
	Filla = RGB_BMP(0, 254, 0),
};

class Pyr3 : public Figure
{
	int* pl;
	int* sort;

	void DrawEdge(int a, int b, int c);
	void SortEdges();
	void DrawShadow(int a, int b, int c);
public:
	Pyr3(Bitmap* bmp);
	~Pyr3();
	void Draw() override;
	void Reset() override;
};
