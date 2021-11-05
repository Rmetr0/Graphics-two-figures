#include "Pyr3.h"

Pyr3::Pyr3(Bitmap* bmp)
{
	this->bmp = bmp;
	ptnum = 4;

	static int sort[6] = { 0, 1, 2, 3 };
	this->sort = sort;
	static int pl[12] = {

		0,	1,	2,
		0,	2,	3,
		0,	1,	3,
		3,	1,	2,
	};
	this->pl = pl;

	Reset();

	minx = min(min(ProjX(x[0], z[0]), ProjX(x[2], z[0])), min(ProjX(x[0], z[4]), ProjX(x[2], z[4])));
	miny = min(min(ProjY(y[0], z[0]), ProjY(y[1], z[0])), min(ProjY(y[0], z[4]), ProjY(y[1], z[4])));
	maxx = max(max(ProjX(x[0], z[0]), ProjX(x[2], z[0])), max(ProjX(x[0], z[4]), ProjX(x[2], z[4])));
	maxy = max(max(ProjY(y[0], z[0]), ProjY(y[1], z[0])), max(ProjY(y[0], z[4]), ProjY(y[1], z[4])));
}

Pyr3::~Pyr3() {}

void Pyr3::DrawEdge(int a, int b, int c)
{
	double	plx[3] = { x[a], x[b], x[c] },
			ply[3] = { y[a], y[b], y[c] },
			plz[3] = { z[a], z[b], z[c] },
			x0 = GetCenter(3, plx),
			y0 = GetCenter(3, ply),
			z0 = GetCenter(3, plz);
	SetMaxMin(3, plx, ply, plz);

	DrawPlane(3, plx, ply, plz, (COLORREF)COLOR::Linea);
	Fill(ProjX(x0, z0), ProjY(y0, z0), (COLORREF)PYR3::Filla);
	Fill(ProjX(x0, z0), ProjY(y0, z0), color);
	DrawPlane(3, plx, ply, plz, (COLORREF)COLOR::Line);
}

void Pyr3::SortEdges()
{
	double x0[4], y0[4], z0[4], tmp[3], buf, max;
	int s;

	for (int i = 0; i < 4; ++i)
	{
		tmp[0] = z[pl[0 + 3 * sort[i]]];
		tmp[1] = z[pl[1 + 3 * sort[i]]];
		tmp[2] = z[pl[2 + 3 * sort[i]]];
		z0[i] = GetCenter(3, tmp);
	}

	for (int i = 3; i >= 0; --i)
	{
		max = z0[0];
		s = 0;
		for (int j = 1; j <= i; ++j)
		{
			if (z0[j] >= max)
			{
				max = z0[j];
				s = j;
			}
		}

		buf = z0[i];
		z0[i] = z0[s];
		z0[s] = buf;

		buf = sort[i];
		sort[i] = sort[s];
		sort[s] = buf;
	}
}

void Pyr3::DrawShadow(int a, int b, int c)
{
	double	plx[3] = { x[a], x[b], x[c] },
			ply[3] = { y[a], y[b], y[c] },
			plz[3] = { z[a], z[b], z[c] };

	for (int i = 0; i < 3; ++i)
	{
		if ((-ply[i] + bmp->srcy) == 0) return;
		plx[i] = ((ply[i] - bmp->srcy) * (plx[i] - bmp->srcx)) / (ply[i] - bmp->srcy) + bmp->srcx;
		plz[i] = ((ply[i] - bmp->srcy) * (plz[i] - bmp->srcz)) / (ply[i] - bmp->srcy) + bmp->srcz;
		//plx[i] = (-bmp->srcx * ply[i] + plx[i] * bmp->srcy) / (-ply[i] + bmp->srcy);
		//plz[i] = (-bmp->srcz * ply[i] + plz[i] * bmp->srcy) / (-ply[i] + bmp->srcy);
		ply[i] = bmp->floor;
	}
	double	x0 = GetCenter(3, plx),
			y0 = GetCenter(3, ply),
			z0 = GetCenter(3, plz);

	maxx = maxy = 0;
	minx = (int)BMP::Width;
	miny = (int)BMP::Height;
	for (int i = 0; i < 3; ++i)
	{
		maxx = (maxx < ProjShX(plx[i], plz[i])) ? ProjShX(plx[i], plz[i]) : maxx;
		maxy = (maxy < ProjShY(ply[i], plz[i])) ? ProjShY(ply[i], plz[i]) : maxy;
		minx = (minx > ProjShX(plx[i], plz[i])) ? ProjShX(plx[i], plz[i]) : minx;
		miny = (miny > ProjShY(ply[i], plz[i])) ? ProjShY(ply[i], plz[i]) : miny;
	}

	int j;
	for (int i = 0; i < 3; ++i)
	{
		j = (i == (3 - 1)) ? 0 : (i + 1);
		bmp->Line(ProjShX(plx[i], plz[i]), ProjShY(ply[i], plz[i]), ProjShX(plx[j], plz[j]), ProjShY(ply[j], plz[j]), (COLORREF)COLOR::Linea);
	}
	Fill(ProjShX(x0, z0), ProjShY(y0, z0), (COLORREF)COLOR::Shadowa);
	Fill(ProjShX(x0, z0), ProjShY(y0, z0), (COLORREF)COLOR::Shadow);
	for (int i = 0; i < 3; ++i)
	{
		j = (i == (3 - 1)) ? 0 : (i + 1);
		bmp->Line(ProjShX(plx[i], plz[i]), ProjShY(ply[i], plz[i]), ProjShX(plx[j], plz[j]), ProjShY(ply[j], plz[j]), (COLORREF)COLOR::Shadow);
	}

	if (rays)
	{
		for (int i = 0; i < 3; ++i)
		{
			bmp->Line(ProjX(bmp->srcx, bmp->srcz), ProjY(bmp->srcy, bmp->srcz), ProjShX(plx[i], plz[i]), ProjShY(ply[i], plz[i]), (COLORREF)COLOR::Ray);
		}
	}
}

void Pyr3::Draw()
{
	SortEdges();
	for (int i = 0; i < 4; ++i)
	{
		DrawEdge(pl[0 + 3 * sort[i]], pl[1 + 3 * sort[i]], pl[2 + 3 * sort[i]]);
		DrawShadow(pl[0 + 3 * sort[i]], pl[1 + 3 * sort[i]], pl[2 + 3 * sort[i]]);
	}
}

void Pyr3::Reset()
{
	rays = 0;
	color = (COLORREF)PYR3::Fill;
	bmp->viewx = (double)BMP::View_X;
	bmp->viewy = (double)BMP::View_Y;
	bmp->viewz = (double)BMP::View_Z;
	bmp->srcx = (double)LIGHT::Src_X;
	bmp->srcy = (double)LIGHT::Src_Y;
	bmp->srcz = (double)LIGHT::Src_Z;
	bmp->floor = (double)LIGHT::Floor;
	static double x[4], y[4], z[4];

	x[0] = bmp->viewx + (double)PYR3::X0;
	x[1] = x[0] + (double)PYR3::Shift_X;
	x[2] = x[0] + (double)PYR3::Width;
	x[3] = x[1];
	this->x = x;

	y[0] = bmp->viewy + (double)PYR3::Y0;
	y[1] = y[0] - (double)PYR3::Height;
	y[2] = y[0];
	y[3] = y[0];
	this->y = y;

	z[0] = (double)PYR3::Z0;
	z[1] = z[0] + (double)PYR3::Shift_Z;
	z[2] = z[0];
	z[3] = z[0] + (double)PYR3::Depth;
	this->z = z;
}