#include "Figure.h"

Figure::Figure() {}

Figure::~Figure() {}

void Figure::DrawPlane(int ptnum, double* x, double* y, double* z, COLORREF color)
{
	int j;
	for (int i = 0; i < ptnum; ++i)
	{
		j = (i == (ptnum - 1)) ? 0 : (i + 1);
		bmp->Line(ProjX(x[i], z[i]), ProjY(y[i], z[i]), ProjX(x[j], z[j]), ProjY(y[j], z[j]), color);
		bmp->Line(ProjX(x[i] + 0.25, z[i]), ProjY(y[i] + 0.25, z[i]), ProjX(x[j] + 0.25, z[j]), ProjY(y[j] + 0.25, z[j]), color);
		bmp->Line(ProjX(x[i] + 0.45, z[i]), ProjY(y[i] + 0.45, z[i]), ProjX(x[j] + 0.45, z[j]), ProjY(y[j] + 0.45, z[j]), color);
		bmp->Line(ProjX(x[i] - 0.25, z[i]), ProjY(y[i] - 0.25, z[i]), ProjX(x[j] - 0.25, z[j]), ProjY(y[j] - 0.25, z[j]), color);
		bmp->Line(ProjX(x[i] - 0.45, z[i]), ProjY(y[i] - 0.45, z[i]), ProjX(x[j] - 0.45, z[j]), ProjY(y[j] - 0.45, z[j]), color);
		//bmp->Line(ProjX(x[i] + 0.75, z[i]), ProjY(y[i] + 0.75, z[i]), ProjX(x[j] + 0.75, z[j]), ProjY(y[j] + 0.75, z[j]), color);
		//bmp->Line(ProjX(x[i] + 1, z[i]), ProjY(y[i] + 1, z[i]), ProjX(x[j] + 1, z[j]), ProjY(y[j] + 1, z[j]), color);
	}
}

void Figure::Fill(double x0, double y0, COLORREF color)
{
	WaveFill(x0, y0, color);

	//struct Point
	//{
	//	double x, y;
	//};
	//Point pt; pt.x = x0; pt.y = y0;

	//std::stack<Point> st;

	//st.push(pt);
	//while (!st.empty())
	//{
	//	Point p = st.top();
	//	st.pop();
	//	(*bmp)(p.y, p.x) = color;
	//	bmp->DrawBitmap();

	//	Point p1, p2, p3, p4;
	//	p1.x = p.x + 1; p1.y = p.y;
	//	p2.x = p.x; p2.y = p.y + 1;
	//	p3.x = p.x; p3.y = p.y - 1;
	//	p4.x = p.x - 1; p4.y = p.y;

	//	if ((*bmp)(p1.y, p1.x) == (COLORREF)COLOR::Background/*((*bmp)(p1.y, p1.x) != (COLORREF)COLOR::Line_unfilled) && ((*bmp)(p1.y, p1.x) != color)*/)
	//		st.push(p1);
	//	if ((*bmp)(p1.y, p1.x) == (COLORREF)COLOR::Background/*((*bmp)(p2.y, p2.x) != (COLORREF)COLOR::Line_unfilled) && ((*bmp)(p1.y, p1.x) != color)*/)
	//		st.push(p2);
	//	if ((*bmp)(p1.y, p1.x) == (COLORREF)COLOR::Background/*((*bmp)(p3.y, p3.x) != (COLORREF)COLOR::Line_unfilled) && ((*bmp)(p1.y, p1.x) != color)*/)
	//		st.push(p3);
	//	if ((*bmp)(p1.y, p1.x) == (COLORREF)COLOR::Background/*((*bmp)(p4.y, p4.x) != (COLORREF)COLOR::Line_unfilled) && ((*bmp)(p1.y, p1.x) != color)*/)
	//		st.push(p4);
	//}

	////std::cout << "Color = " << (*bmp)(x0, y0) << std::endl;
	//if (((*bmp)(y0, x0) == (COLORREF)COLOR::Line_unfilled) || ((*bmp)(y0, x0) == color)) 
	//	return;
	//(*bmp)(y0, x0) = color;
	//bmp->DrawBitmap();
	//Fill(x0 - 1, y0, color);
	//Fill(x0 + 1, y0, color);
	//Fill(x0, y0 - 1, color);
	//Fill(x0, y0 + 1, color);
}

//Вспом. функция к WaveFill (проверка условия заполнения точки)
void Figure::NearPix(int x, int y, int* numStack, POINT* Stack, COLORREF color)
{
	if (x >= 0 && y >= 0 && x <= (int)BMP::Width && y <= (int)BMP::Height &&
		x >= minx && y >= miny && x <= maxx && y <= maxy)
	{
		if((*bmp)(y, x) != color && (*bmp)(y, x) != (COLORREF)COLOR::Linea)
		{
			(*bmp)(y, x) = color;
			Stack[*numStack].x = x;
			Stack[*numStack].y = y;
			(*numStack)++;
		}
	}
}

//Вспом. функция к WaveFill (переход к соседним к точке координатам)
void Figure::OneStep(int* numSrc, int* numDest, POINT* Src, POINT* Dest, COLORREF color)
{
	int x, y, i;
	*numDest = 0;
	for (i = 0; i < *numSrc; i++)
	{
		x = Src[i].x;
		y = Src[i].y;
		NearPix(x + 1, y, numDest, Dest, color);
		NearPix(x - 1, y, numDest, Dest, color);
		NearPix(x, y + 1, numDest, Dest, color);
		NearPix(x, y - 1, numDest, Dest, color);
	}
}

//Функция заливки
void Figure::WaveFill(int xst, int yst, COLORREF color)
{
	if ((*bmp)(yst, xst) == (COLORREF)COLOR::Linea) return;

	int numA, numB;
	POINT* stackA, * stackB;
	stackA = new POINT[100000];
	stackB = new POINT[100000];
	numA = 1;
	stackA[0].x = xst;
	stackA[0].y = yst;
	numB = 0;
	while (1)
	{
		if (numA > 0)
			OneStep(&numA, &numB, stackA, stackB, color);
		else
			break;
		if (numB > 0)
			OneStep(&numB, &numA, stackB, stackA, color);
		else
			break;
	}
	delete[] stackB;
	delete[] stackA;
}

void Figure::Clear()
{
	bmp->Clear(0, 0, (int)BMP::Width, (int)BMP::Height);
}

double Figure::ProjX(double x, double z)
{
	return (x * bmp->viewz - z * bmp->viewx) / (bmp->viewz - z);
}

double Figure::ProjY(double y, double z)
{
	return (y * bmp->viewz - z * bmp->viewy) / (bmp->viewz - z);
}

double Figure::ProjShX(double x, double z)
{
	return (x * bmp->srcz/*bmp->viewz*/ - z * bmp->srcx/*bmp->viewx*/) / (bmp->viewz - z);
}

double Figure::ProjShY(double y, double z)
{
	return (- z * bmp->srcy/*bmp->viewy*/ + y * bmp->srcz/*bmp->viewz*/) / (bmp->viewz - z);
}

//double Figure::ProjShX(double x, double y)
//{
//	return (x * bmp->srcy/*bmp->viewy*/ - y * bmp->srcx/*bmp->viewx*/) / (bmp->srcy/*bmp->viewy*/ - y);
//	return x;
//}

//double Figure::ProjShZ(double z, double y)
//{
//	return (z * bmp->srcy/*bmp->viewy*/ - y * bmp->srcz/*bmp->viewz*/) / (bmp->srcy/*bmp->viewy*/ - y);
//	return z;
//}

double Figure::GetCenter(int ptnum, double* c)
{
	int c0 = 0.0;
	for (int i = 0; i < ptnum; ++i)
	{
		c0 += c[i];
	}
	return (c0 / (double)ptnum);
}

void Figure::SetMaxMin(int ptnum, double* x, double* y, double* z)
{
	maxx = maxy = 0;
	minx = (int)BMP::Width;
	miny = (int)BMP::Height;
	for (int i = 0; i < ptnum; ++i)
	{
		maxx = (maxx < ProjX(x[i], z[i])) ? ProjX(x[i], z[i]) : maxx;
		maxy = (maxy < ProjY(y[i], z[i])) ? ProjY(y[i], z[i]) : maxy;
		minx = (minx > ProjX(x[i], z[i])) ? ProjX(x[i], z[i]) : minx;
		miny = (miny > ProjY(y[i], z[i])) ? ProjY(y[i], z[i]) : miny;
	}
}

void Figure::Swap(int* mas, int a, int b)
{
	int buf;
	buf = mas[a];
	mas[a] = mas[b];
	mas[b] = buf;
}

void Figure::Move(double dx, double dy, double dz)
{
	for (int i = 0; i < ptnum; ++i)
	{
		x[i] += dx;
		y[i] += dy;
		z[i] += dz;
	}
}

void Figure::Rotate(double ax, double ay, double az)
{
	double
		x0 = GetCenter(ptnum, x),
		y0 = GetCenter(ptnum, y),
		z0 = GetCenter(ptnum, z),
		tmp1, tmp2;

	if (ax)
	{
		for (int i = 0; i < ptnum; ++i)
		{
			tmp1 = y[i], tmp2 = z[i];
			y[i] = y0 + (tmp1 - y0) * cos(ax) - (tmp2 - z0) * sin(ax);
			z[i] = z0 + (tmp1 - y0) * sin(ax) + (tmp2 - z0) * cos(ax);
		}
	}
	if (ay)
	{
		for (int i = 0; i < ptnum; ++i)
		{
			tmp1 = x[i], tmp2 = z[i];
			x[i] = x0 + (tmp1 - x0) * cos(ay) - (tmp2 - z0) * sin(ay);
			z[i] = z0 + (tmp1 - x0) * sin(ay) + (tmp2 - z0) * cos(ay);
		}
	}
	if (az)
	{
		for (int i = 0; i < ptnum; ++i)
		{
			tmp1 = x[i], tmp2 = y[i];
			x[i] = x0 + (tmp1 - x0) * cos(az) - (tmp2 - y0) * sin(az);
			y[i] = y0 + (tmp1 - x0) * sin(az) + (tmp2 - y0) * cos(az);
		}
	}
}

void Figure::Scale(double rate)
{
	double
		x0 = GetCenter(ptnum, x),
		y0 = GetCenter(ptnum, y),
		z0 = GetCenter(ptnum, z);

	for (int i = 0; i < ptnum; ++i)
	{
		x[i] = rate * x[i] + x0 * (1 - rate);
		y[i] = rate * y[i] + y0 * (1 - rate);
		z[i] = rate * z[i] + z0 * (1 - rate);
	}
}