#pragma once

#include "Figure.h"

enum class PARALL 
{
	X0		=	-300,
	Y0		=	0,
	Z0		=	0,
	Width	=	200,
	Height	=	100,
	Depth	=	-100,
	Fill	=	RGB_BMP(255, 0, 0),
	Filla	=	RGB_BMP(254, 0, 0),
};

class Parall : public Figure
{
	int* pl;
	int* sort;
	
	void DrawEdge(int a, int b, int c, int d);
	void SortEdges();
	void DrawShadow(int a, int b, int c, int d);
public:
	Parall(Bitmap* bmp);
	~Parall();
	void Draw() override;
	void Reset() override;
};