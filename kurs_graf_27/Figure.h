#pragma once

#include "Bitmap.h"

const double
	Step	=	5.0,
	Angle	=	0.1,
	Rate	=	1.1;

class Figure
{
protected:
	Bitmap* bmp;
	double maxx, maxy, minx, miny;
	double* x, * y, * z;
	int ptnum;

	void DrawPlane(int ptnum, double* x, double* y, double* z, COLORREF color);
	void Fill(double x0, double y0, COLORREF color);

	void WaveFill(int xst, int yst, COLORREF color);
	void OneStep(int* numSrc, int* numDest, POINT* Src, POINT* Dest, COLORREF color);
	void NearPix(int x, int y, int* numStack, POINT* Stack, COLORREF color);

	double ProjX(double x, double z);
	double ProjY(double y, double z);
	double ProjShX(double x, double y);
	double ProjShY(double z, double y);
	double GetCenter(int ptnum, double* c);
	void SetMaxMin(int ptnum, double* x, double* y, double* z);
	void Swap(int* mas, int a, int b);

public:
	COLORREF color;
	int rays;

	Figure();
	virtual ~Figure();
	virtual void Draw() = 0;
	virtual void Reset() = 0;

	void Move(double dx, double dy, double dz);
	void Rotate(double ax, double ay, double az);
	void Scale(double rate);
	void Clear();
};