#pragma once

#include <iostream>
#include <windows.h>

#define RGB_BMP(r, g ,b)  ((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))

enum class BMP
{
	Left_margin	=	10,
	Top_margin	=	50,
	Width		=	1200,
	Height		=	610,
	Depth		=	1000,
	View_X		=	Width / 2,
	View_Y		=	Height / 2,
	View_Z		=	750
};

enum class COLOR
{
	Background	=	RGB_BMP(255, 255, 255),
	Shadow		=	RGB_BMP(180, 180, 180),
	Shadowa		=	RGB_BMP(181, 180, 180),
	Line		=	RGB_BMP(0, 0, 0),
	Linea		=	RGB_BMP(1, 0, 0),
	Ray			=	RGB_BMP(255, 216, 0)
};

enum class LIGHT
{
	Src_X = 650,
	Src_Y = 180,
	Src_Z = 650,
	Floor = 650
};

class Bitmap {
	const int B = 8;

	HDC hDC;
	BITMAP bmp;
	HBITMAP hBitmap;
	COLORREF color;
	COLORREF* mapIMG;
	HBITMAP hOldbm;
	HDC hMemDC;
	BITMAP bm;
	POINT ptSize, ptOrg;

public:
	double	viewx, viewy, viewz,
			srcx, srcy, srcz, floor;

	Bitmap(int w, int h, int x0, int y0, COLORREF col);
	~Bitmap();
	void DrawBitmap();
	void Line(int x0, int y0, int x1, int y1, COLORREF col);
	void Clear(int x0, int y0, int x1, int y1);
	//void Clear();
	COLORREF& operator()(int i, int j);

	const int width;
	const int height;
	const int x;
	const int y;
};