#include "Bitmap.h"

Bitmap::Bitmap(int w, int h, int x0, int y0, COLORREF col) : width(w), height(h), x(x0), y(y0) {
	viewx = (double)BMP::View_X;
	viewy = (double)BMP::View_Y;
	viewz = (double)BMP::View_Z;

	hDC = GetDC(GetConsoleWindow());
	color = col;
	mapIMG = new COLORREF[width * height * B];
	for (int i = 0; i < width * height * B; ++i)
		mapIMG[i] = col;
	bmp = { 0, w, h, 32 * w, 1, 32, NULL };
	bmp.bmBits = (COLORREF*)mapIMG;
}

Bitmap::~Bitmap() {
	delete[] mapIMG;
}

COLORREF& Bitmap::operator()(int i, int j) {
	if (i >= height) i = height - 1;
	if (j >= width) j = width - 1;
	if (i < 0) i = 0;
	if (j < 0) j = 0;
	return mapIMG[i * width * B + j];
}

void Bitmap::Line(int x0, int y0, int x1, int y1, COLORREF col) {
	int	d = 0,
		dy = (y1 - y0 > 0) ? 1 : -1,
		dx = (x1 - x0 > 0) ? 1 : -1,
		lengthX = abs(x1 - x0),
		lengthY = abs(y1 - y0),
		length = max(lengthX, lengthY);

	if ((x0 >= 0) && (y0 >= 0) && (x0 < width) && (y0 < height))
		mapIMG[y0 * width * B + x0] = col;

	if (lengthX >= lengthY) {
		while (length--) {
			d += lengthY;
			if (d >= lengthX) {
				y0 += dy;
				d -= lengthX;
			}
			x0 += dx;
			if (x0 >= 0 && y0 >= 0 && x0 < width && y0 < height)
				mapIMG[y0 * width * B + x0] = col;
		}
	}
	else {
		while (length--) {
			d += lengthX;
			if (d >= lengthY) {
				x0 += dx;
				d -= lengthY;
			}
			y0 += dy;
			if (x0 >= 0 && y0 >= 0 && x0 < width && y0 < height)
				mapIMG[y0 * width * B + x0] = col;
		}
	}
}

void Bitmap::Clear(int x0, int y0, int x1, int y1) {
	if (x0 < 0) x0 = 0; if (y0 < 0) y0 = 0;
	if (x1 < 0) x1 = 0; if (y1 < 0) y1 = 0;

	for (int j; y0 <= y1; ++y0) {
		for (j = x0; j <= x1; ++j) {
			if ((y0 * width * B + j) >= (width * height * B)) return;
			mapIMG[y0 * width * B + j] = color;
		}
	}
}

//void Bitmap::Clear() {
//	for (int i = 0; i < width * height * B; ++i)
//		mapIMG[i] = color;
//}

void Bitmap::DrawBitmap() {
	hBitmap = CreateBitmapIndirect(&bmp);

	// —оздаем контекст пам€ти, совместимый
	// с контекстом отображени€
	hMemDC = CreateCompatibleDC(hDC);

	// ¬ыбираем изображение bitmap в контекст пам€ти
	hOldbm = (HBITMAP)SelectObject(hMemDC, hBitmap);

	// ≈сли не было ошибок, продолжаем работу
	if (hOldbm) {
		// ƒл€ контекста пам€ти устанавливаем тот же
		// режим отображени€, что используетс€ в
		// контексте отображени€
		SetMapMode(hMemDC, GetMapMode(hDC));

		// ќпредел€ем размеры изображени€
		GetObject(hBitmap, sizeof(BITMAP), (COLORREF*)&bm);

		ptSize.x = bm.bmWidth;   // ширина
		ptSize.y = bm.bmHeight;  // высота

		// ѕреобразуем координаты устройства в логические
		// дл€ устройства вывода
		DPtoLP(hDC, &ptSize, 1);

		ptOrg.x = 0;
		ptOrg.y = 0;

		// ѕреобразуем координаты устройства в логические
		// дл€ контекста пам€ти
		DPtoLP(hMemDC, &ptOrg, 1);

		// –исуем изображение bitmap
		BitBlt(hDC, x, y, ptSize.x, ptSize.y,
			hMemDC, ptOrg.x, ptOrg.y, SRCCOPY);

		// ¬осстанавливаем контекст пам€ти
		SelectObject(hMemDC, hOldbm);
	}
	else {
		system("pause");
	}

	// ”дал€ем контекст пам€ти
	DeleteDC(hMemDC);
	DeleteObject(hBitmap);
}