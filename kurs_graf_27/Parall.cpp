#include "Parall.h"

Parall::Parall(Bitmap* bmp)
{
	this->bmp = bmp;
	ptnum = 8;

	static int sort[6] = { 0, 1, 2, 3, 4, 5 };
	this->sort = sort;
	static int pl[24] = {

		4,	5,	6,	7,
		0,	1,	5,	4,
		1,	5,	6,	2,
		0,	4,	7,	3,
		3,	2,	6,	7,
		0,	1,	2,	3
	};
	this->pl = pl;

	Reset();

	minx = min(min(ProjX(x[0], z[0]), ProjX(x[2], z[0])), min(ProjX(x[0], z[4]), ProjX(x[2], z[4])));
	miny = min(min(ProjY(y[0], z[0]), ProjY(y[1], z[0])), min(ProjY(y[0], z[4]), ProjY(y[1], z[4])));
	maxx = max(max(ProjX(x[0], z[0]), ProjX(x[2], z[0])), max(ProjX(x[0], z[4]), ProjX(x[2], z[4])));
	maxy = max(max(ProjY(y[0], z[0]), ProjY(y[1], z[0])), max(ProjY(y[0], z[4]), ProjY(y[1], z[4])));
}

Parall::~Parall() {}

void Parall::DrawEdge(int a, int b, int c, int d)
{
	double	plx[4] = { x[a], x[b], x[c], x[d] },
			ply[4] = { y[a], y[b], y[c], y[d] },
			plz[4] = { z[a], z[b], z[c], z[d] },
			x0 = GetCenter(4, plx), 
			y0 = GetCenter(4, ply), 
			z0 = GetCenter(4, plz);
	SetMaxMin(4, plx, ply, plz);

	DrawPlane(4, plx, ply, plz, (COLORREF)COLOR::Linea);
	Fill(ProjX(x0, z0), ProjY(y0, z0), (COLORREF)PARALL::Filla);
	Fill(ProjX(x0, z0), ProjY(y0, z0), color);
	DrawPlane(4, plx, ply, plz, (COLORREF)COLOR::Line);
}

void Parall::SortEdges()
{
	double x0[6], y0[6], z0[6], tmp[4], buf, maxz, minz;
	int maxzn, minzn;

	for (int i = 0; i < 6; ++i)
	{
		tmp[0] = z[pl[0 + 4 * sort[i]]];
		tmp[1] = z[pl[1 + 4 * sort[i]]];
		tmp[2] = z[pl[2 + 4 * sort[i]]];
		tmp[3] = z[pl[3 + 4 * sort[i]]];
		z0[i] = GetCenter(4, tmp);
	}

	for (int i = 5; i >= 0; --i)
	{
		maxz = minz = z0[0];
		maxzn = minzn = 0;
		for (int j = 1; j <= i; ++j)
		{
			if (z0[j] >= maxz)
			{
				maxz = z0[j];
				maxzn = j;
			}
			if (z0[j] <= minz)
			{
				minz = z0[j];
				minzn = j;
			}
		}

		buf = z0[i];
		z0[i] = z0[maxzn];
		z0[maxzn] = buf;

		buf = sort[i];
		sort[i] = sort[maxzn];
		sort[maxzn] = buf;
	}

/*	double maxx, maxy, minx, miny;
	int maxxn, maxyn, minxn, minyn;

	for (int i = 0; i < 6; ++i)
	{
		tmp[0] = ProjX(x[pl[0 + 4 * sort[i]]], z[pl[0 + 4 * sort[i]]]);
		tmp[1] = ProjX(x[pl[1 + 4 * sort[i]]], z[pl[1 + 4 * sort[i]]]);
		tmp[2] = ProjX(x[pl[2 + 4 * sort[i]]], z[pl[2 + 4 * sort[i]]]);
		tmp[3] = ProjX(x[pl[3 + 4 * sort[i]]], z[pl[3 + 4 * sort[i]]]);
		x0[i] = GetCenter(4, tmp);
		if (i == 0)
		{
			maxx = minx = x0[i];
			maxxn = minxn = i;
		}
		if (x0[i] >= maxx)
		{
			maxx = x0[i];
			maxxn = i;
		}
		if (x0[i] <= minx)
		{
			minx = x0[i];
			minxn = i;
		}
	}
	for (int i = 0; i < 6; ++i)
	{
		tmp[0] = ProjY(y[pl[0 + 4 * sort[i]]], z[pl[0 + 4 * sort[i]]]);
		tmp[1] = ProjY(y[pl[1 + 4 * sort[i]]], z[pl[1 + 4 * sort[i]]]);
		tmp[2] = ProjY(y[pl[2 + 4 * sort[i]]], z[pl[2 + 4 * sort[i]]]);
		tmp[3] = ProjY(y[pl[3 + 4 * sort[i]]], z[pl[3 + 4 * sort[i]]]);
		y0[i] = GetCenter(4, tmp);
		if (i == 0)
		{
			maxy = miny = y0[i];
			maxyn = minyn = i;
		}
		if (y0[i] >= maxy)
		{
			maxy = y0[i];
			maxyn = i;
		}
		if (y0[i] <= miny)
		{
			miny = y0[i];
			minyn = i;
		}
	}

	if (maxy > (int)BMP::View_Y)
	{
		if (minx < (int)BMP::View_X)
		{
			//std::cout << "I" << std::endl;
			sort[0] = minzn;
			sort[1] = minxn;
			sort[2] = minyn;
			sort[3] = maxyn;
			sort[4] = maxxn;
			sort[5] = maxzn;
		}
		else if (maxx > (int)BMP::View_X)
		{
			//std::cout << "II" << std::endl;
			sort[0] = minzn;
			sort[1] = maxxn;
			sort[2] = minyn;
			sort[3] = maxyn;
			sort[4] = minxn;
			sort[5] = maxzn;
		}
	}
	else if (miny < (int)BMP::View_Y)
	{
		if (minx < (int)BMP::View_X)
		{
			//std::cout << "III" << std::endl;
			sort[0] = minzn;
			sort[1] = minxn;
			sort[2] = maxyn;
			sort[3] = minyn;
			sort[4] = maxxn;
			sort[5] = maxzn;
		}
		else if (maxx > (int)BMP::View_X)
		{
			//std::cout << "IV" << std::endl;
			sort[0] = minzn;
			sort[1] = maxxn;
			sort[2] = maxyn;
			sort[3] = minyn;
			sort[4] = minxn;
			sort[5] = maxzn;
		}
	}*/

	//system("cls");
	//std::cout << maxx << " " << maxy << " " << minx << " " << miny << " " << std::endl;
	//std::cout << maxxn << " " << maxyn << " " << minxn << " " << minyn << std::endl;
	//std::cout << sort[0] << " " << sort[1] << " " << sort[2] << " " << sort[3] << " " << sort[4] << " " << sort[5] << std::endl;

	//if (maxy > (int)BMP::View_Y)
	//{
	//	if (minx < (int)BMP::View_X)
	//	{
	//		std::cout << "I" << std::endl;
	//		if (minxn > minyn) Swap(sort, minxn, minyn);
	//		if (maxyn > maxxn) Swap(sort, maxyn, maxxn);
	//	}
	//	else if(maxx > (int)BMP::View_X)
	//	{
	//		std::cout << "II" << std::endl;
	//		if (maxxn > minyn) Swap(sort, maxxn, minyn);
	//		if (maxyn > minxn) Swap(sort, maxyn, minxn);
	//	}
	//}
	//else if (miny < (int)BMP::View_Y)
	//{
	//	if (minx < (int)BMP::View_X)
	//	{
	//		std::cout << "III" << std::endl;
	//		if (minxn > maxyn) Swap(sort, minxn, maxyn);
	//		if (minyn > maxxn) Swap(sort, minyn, maxxn);
	//	}
	//	else if (maxx > (int)BMP::View_X)
	//	{
	//		std::cout << "IV" << std::endl;
	//		if (maxxn > maxyn) Swap(sort, maxxn, maxyn);
	//		if (minyn > minxn) Swap(sort, minyn, minxn);
	//	}
	//}

	//for (int i = 0; i < 6; ++i)
	//{
	//	tmp[0] = z[pl[0 + 4 * sort[i]]];
	//	tmp[1] = z[pl[1 + 4 * sort[i]]];
	//	tmp[2] = z[pl[2 + 4 * sort[i]]];
	//	tmp[3] = z[pl[3 + 4 * sort[i]]];
	//	z0[i] = GetCenter(4, tmp);
	//}

	/*for (int i = 5; i >= 0; --i)
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
	}*/

	/*system("cls");
	std::cout << "z0: " << z0[0] << " " << z0[1] << " " << z0[2] << " " << z0[3] << " " << z0[4] << " " << z0[5] << " " << std::endl;
	std::cout << "sort: " << sort[0] << " " << sort[1] << " " << sort[2] << " " << sort[3] << " " << sort[4] << " " << sort[5] << " " << std::endl;*/
}

void Parall::DrawShadow(int a, int b, int c, int d)
{
	double	plx[4] = { x[a], x[b], x[c], x[d] },
			ply[4] = { y[a], y[b], y[c], y[d] },
			plz[4] = { z[a], z[b], z[c], z[d] };

	for (int i = 0; i < 4; ++i)
	{
		if ((-ply[i] + bmp->srcy) == 0) return;
		plx[i] = ((ply[i] - bmp->srcy) * (plx[i] - bmp->srcx)) / (ply[i] - bmp->srcy);// +bmp->srcx;
		plz[i] = ((ply[i] - bmp->srcy) * (plz[i] - bmp->srcz)) / (ply[i] - bmp->srcy);// +bmp->srcz;
		//plx[i] = (-bmp->srcx * ply[i] + plx[i] * bmp->srcy) / (-ply[i] + bmp->srcy);
		//plz[i] = (-bmp->srcz * ply[i] + plz[i] * bmp->srcy) / (-ply[i] + bmp->srcy);
		ply[i] = bmp->floor;
	}
	double	x0 = GetCenter(4, plx),
			y0 = GetCenter(4, ply),
			z0 = GetCenter(4, plz);
	
	maxx = maxy = 0;
	minx = (int)BMP::Width;
	miny = (int)BMP::Height;
	for (int i = 0; i < 4; ++i)
	{
		maxx = (maxx < ProjShX(plx[i], plz[i])) ? ProjShX(plx[i], plz[i]) : maxx;
		maxy = (maxy < ProjShY(ply[i], plz[i])) ? ProjShY(ply[i], plz[i]) : maxy;
		minx = (minx > ProjShX(plx[i], plz[i])) ? ProjShX(plx[i], plz[i]) : minx;
		miny = (miny > ProjShY(ply[i], plz[i])) ? ProjShY(ply[i], plz[i]) : miny;
	}

	int j;
	for (int i = 0; i < 4; ++i)
	{
		j = (i == (4 - 1)) ? 0 : (i + 1);
		bmp->Line(ProjShX(plx[i], plz[i]), ProjShY(ply[i], plz[i]), ProjShX(plx[j], plz[j]), ProjShY(ply[j], plz[j]), (COLORREF)COLOR::Linea);
	}
	Fill(ProjShX(x0, z0), ProjShY(y0, z0), (COLORREF)COLOR::Shadowa);
	Fill(ProjShX(x0, z0), ProjShY(y0, z0), (COLORREF)COLOR::Shadow);
	for (int i = 0; i < 4; ++i)
	{
		j = (i == (4 - 1)) ? 0 : (i + 1);
		bmp->Line(ProjShX(plx[i], plz[i]), ProjShY(ply[i], plz[i]), ProjShX(plx[j], plz[j]), ProjShY(ply[j], plz[j]), (COLORREF)COLOR::Shadow);
	}

	if (rays)
	{
		for (int i = 0; i < 4; ++i)
		{
			bmp->Line(ProjX(bmp->srcx, bmp->srcz), ProjY(bmp->srcy, bmp->srcz), ProjShX(plx[i], plz[i]), ProjShY(ply[i], plz[i]), (COLORREF)COLOR::Ray);
		}
	}
}

void Parall::Draw()
{
	SortEdges();
	for (int i = 0; i < 6; ++i)
	{
		DrawEdge(pl[0 + 4 * sort[i]], pl[1 + 4 * sort[i]], pl[2 + 4 * sort[i]], pl[3 + 4 * sort[i]]);
		DrawShadow(pl[0 + 4 * sort[i]], pl[1 + 4 * sort[i]], pl[2 + 4 * sort[i]], pl[3 + 4 * sort[i]]);
	}
}

void Parall::Reset()
{
	rays = 0;
	color = (COLORREF)PARALL::Fill;
	bmp->viewx = (double)BMP::View_X;
	bmp->viewy = (double)BMP::View_Y;
	bmp->viewz = (double)BMP::View_Z;
	bmp->srcx = (double)LIGHT::Src_X;
	bmp->srcy = (double)LIGHT::Src_Y;
	bmp->srcz = (double)LIGHT::Src_Z;
	bmp->floor = (double)LIGHT::Floor;
	static double x[8], y[8], z[8];

	x[0] = bmp->viewx + (double)PARALL::X0;
	x[1] = x[0];
	x[2] = x[0] + (double)PARALL::Width;
	x[3] = x[2];
	x[4] = x[0];
	x[5] = x[0];
	x[6] = x[2];
	x[7] = x[2];
	this->x = x;

	y[0] = bmp->viewy + (double)PARALL::Y0;
	y[1] = y[0] - (double)PARALL::Height;
	y[2] = y[1];
	y[3] = y[0];
	y[4] = y[0];
	y[5] = y[1];
	y[6] = y[1];
	y[7] = y[0];
	this->y = y;

	z[0] = (double)PARALL::Z0;
	z[1] = z[0];
	z[2] = z[0];
	z[3] = z[0];
	z[4] = z[0] + (double)PARALL::Depth;
	z[5] = z[4];
	z[6] = z[4];
	z[7] = z[4];
	this->z = z;
}
