#include "Parall.h"
#include "Pyr3.h"

#include <iostream>
#include <conio.h>

using namespace std;

Bitmap bmp(
	(int)BMP::Width,
	(int)BMP::Height,
	(int)BMP::Left_margin,
	(int)BMP::Top_margin,
	(COLORREF)COLOR::Background );
Bitmap* p_bmp = &bmp;

Parall fig1(p_bmp);
Pyr3 fig2(p_bmp);

void Update()
{
	fig1.Draw();
	fig2.Draw();
	bmp.DrawBitmap();
	fig1.Clear();
	fig2.Clear();
}

int main()
{
	system("color 1b");
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE), CONSOLE_FULLSCREEN_MODE, nullptr);

	int c, exit = 0;
	int r, g, b;
	
	Figure* cur = &fig1;

	while (1)
	{
		if (exit) break;

		Update();

		c = _getch();
		switch (c)
		{
		case 'w':
			cur->Move(0, -Step, 0);
			break;

		case 's':
			cur->Move(0, Step, 0);
			break;

		case 'a':
			cur->Move(-Step, 0, 0);
			break;

		case 'd':
			cur->Move(Step, 0, 0);
			break;

		case 'q':
			cur->Move(0, 0, -Step);
			break;

		case 'z':
			cur->Move(0, 0, Step);
			break;

		case '[':
			cur->Rotate(-Angle, 0, 0);
			break;

		case ']':
			cur->Rotate(Angle, 0, 0);
			break;

		case ';':
			cur->Rotate(0, Angle, 0);
			break;

		case '\'':
			cur->Rotate(0, -Angle, 0);
			break;

		case '.':
			cur->Rotate(0, 0, -Angle);
			break;

		case '/':
			cur->Rotate(0, 0, Angle);
			break;

		case '=':
			cur->Scale(Rate);
			break;

		case '-':
			cur->Scale(1 / Rate);
			break;

		case 'r':
			bmp.Clear(0, 0, (int)BMP::Width, (int)BMP::Height);
			fig1.Reset();
			fig2.Reset();
			break;

		case '\\':
			do
			{
				cout << "1. Color" << endl;
				cout << "2. View" << endl;
				cout << "3. Shadow" << endl;

				c = _getch();
				system("cls");
				Update();
				switch (c)
				{
				case '1':
					do
					{
						r = (cur->color >> 16) & 255;
						g = (cur->color >> 8) & 255;
						b = cur->color & 255;
						cout << "Red: " << r << endl;
						cout << "Green: " << g << endl;
						cout << "Blue: " << b << endl;

						c = _getch();
						switch (c)
						{
						case '[':
							if (r != 0) r--;
							break;
						case ']':
							if (r != 255) r++;
							break;
						case ';':
							if (g != 0) g--;
							break;
						case '\'':
							if (g != 255) g++;
							break;
						case '.':
							if (b != 0) b--;
							break;
						case '/':
							if (b != 255) b++;
							break;
						case '0':
							system("pause");
							return 0;
						}
						system("cls");
						cur->color = RGB_BMP(r, g, b);
						Update();
					} while (c != '\\');
					break;

				case '2':
					do
					{
						cout << "View_X: " << bmp.viewx << endl;
						cout << "View_Y: " << bmp.viewy << endl;
						cout << "View_Z: " << bmp.viewz << endl;

						c = _getch();
						switch (c)
						{
						case '[':
							bmp.viewx -= 10;
							break;
						case ']':
							bmp.viewx += 10;
							break;
						case ';':
							bmp.viewy -= 10;
							break;
						case '\'':
							bmp.viewy += 10;
							break;
						case '.':
							if (bmp.viewz != 10) bmp.viewz -= 10;
							break;
						case '/':
							bmp.viewz += 10;
							break;
						case '0':
							system("pause");
							return 0;
						}
						system("cls");
						Update();
					} while (c != '\\');
					break;

				case '3':
					do
					{
						cout << "Src_X: " << bmp.srcx << endl;
						cout << "Src_Y: " << bmp.srcy << endl;
						cout << "Src_Z: " << bmp.srcz << endl;
						cout << "Floor: " << bmp.floor << endl;

						c = _getch();
						switch (c)
						{
						case '[':
							bmp.srcx -= 10;
							break;
						case ']':
							bmp.srcx += 10;
							break;
						case ';':
							bmp.srcy -= 10;
							break;
						case '\'':
							bmp.srcy += 10;
							break;
						case '.':
							bmp.srcz -= 10;
							break;
						case '/':
							bmp.srcz += 10;
							break;
						case 'p':
							bmp.floor -= 10;
							break;
						case 'l':
							bmp.floor += 10;
							break;
						case '0':
							system("pause");
							return 0;
						}
						system("cls");
						Update();
					} while (c != '\\');
					break;
					break;

				case '0':
					system("pause");
					return 0;
				}
			} while (c != '\\');
			break;

		case 't':
			fig1.rays = (fig1.rays == 1) ? 0 : 1;
			fig2.rays = (fig2.rays == 1) ? 0 : 1;
			break;

		case 'c':
			if (cur == &fig1) cur = &fig2;
			else cur = &fig1;
			break;

		case '0':
			exit = 1;
			break;
		}
	}

	system("pause");
	return 0;
}