#include<iostream>
#include<fstream>
#include<iomanip>
#include<cstring>
#include<conio.h>
#include<windows.h>
using namespace std;
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4
void color(int k)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, k);
}
void hideConsoleCursor()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hConsole, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void gotoRowCol(int rpos, int cpos)
{
	COORD scrn;
	HANDLE hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	scrn.X = cpos;
	scrn.Y = rpos;
	SetConsoleCursorPosition(hOutput, scrn);
}
struct position
{
	int ri, ci;
};
struct snake
{
	position* Ps;
	int size;
	char sym;
	int score, direction;
	char Lkey, Rkey, Ukey, Dkey;
};
bool bonus = false;
position bonusp;
char bonussym = 219;
int bonusst = 0;
int bonusd = 8000;
void innit(snake& S, position& fp, char& fsym, int& rows, int& cols, int sc = -1)
{
	rows = 40;
	cols = 100;
	S.size = 3;
	S.sym = -37;
	S.Ps = new position[S.size]{};
	int sr = rows / 2;
	if (sc == -1) 
		sc = cols / 2;
	S.Ps[0].ri = sr;
	S.Ps[1].ri = sr;
	S.Ps[2].ri = sr;
	S.Ps[0].ci = sc;
	S.Ps[1].ci = sc - 1;
	S.Ps[2].ci = sc - 2;
	S.score = 0;
	S.direction = RIGHT;
	S.Lkey = 75, S.Rkey = 77, S.Ukey = 72, S.Dkey = 80;
	if (sc == cols / 2)
	{
		fp.ri = 10;
		fp.ci = 10;
		fsym = 254;
	}
}
void displaysnake(const snake& S, char sym = ' ')
{
	color(10);
	for (int si = 0; si < S.size; si++)
	{
		gotoRowCol(S.Ps[si].ri, S.Ps[si].ci);
		cout << S.sym;
	}
	color(7);
}
void displayfruit(const position& fp, char foodsym)
{
	color(4);
	gotoRowCol(fp.ri, fp.ci);
	cout << foodsym;
	color(7);
}
bool foodeeaten(snake& s, position& fp)
{
	return s.Ps[0].ri == fp.ri and s.Ps[0].ci == fp.ci;
}
int level = 1;
position hurdles[20];
int hurdleCount = 0;
void drawHurdles()
{
	if (level != 3)
		return;
	hurdleCount = 0;
	hurdles[hurdleCount++] = { 5, 20 };
	hurdles[hurdleCount++] = { 6, 20 };
	hurdles[hurdleCount++] = { 7, 20 };
	hurdles[hurdleCount++] = { 7, 21 };
	hurdles[hurdleCount++] = { 7, 22 };
	hurdles[hurdleCount++] = { 15, 40 };
	hurdles[hurdleCount++] = { 15, 41 };
	hurdles[hurdleCount++] = { 15, 42 };
	hurdles[hurdleCount++] = { 14, 41 };
	hurdles[hurdleCount++] = { 16, 41 };

	hurdles[hurdleCount++] = { 25, 60 };
	hurdles[hurdleCount++] = { 26, 60 };
	hurdles[hurdleCount++] = { 27, 60 };
	hurdles[hurdleCount++] = { 28, 60 };

	color(12);
	for (int i = 0; i < hurdleCount; i++)
	{
		gotoRowCol(hurdles[i].ri, hurdles[i].ci);
		cout << char(178);
	}
	color(7);
}
bool hitsHurdle(snake& s)
{
	if (level != 3) return false;
	for (int i = 0; i < hurdleCount; i++)
		if (s.Ps[0].ri == hurdles[i].ri and s.Ps[0].ci == hurdles[i].ci)
			return true;
	return false;
}
void movesnakestep(snake& s, int rows, int cols)
{
	for (int si = s.size - 1; si > 0; si--)
		s.Ps[si] = s.Ps[si - 1];

	switch (s.direction)
	{
	case UP:
		s.Ps[0].ri--;
		break;
	case DOWN:
		s.Ps[0].ri++;
		break;
	case LEFT:
		s.Ps[0].ci--;
		break;
	case RIGHT:
		s.Ps[0].ci++;
		break;
	}
	if (level == 1)
	{
		if (s.Ps[0].ri <= 1)
			s.Ps[0].ri = rows - 2;
		if (s.Ps[0].ri >= rows - 1)
			s.Ps[0].ri = 2;
		if (s.Ps[0].ci <= 1)
			s.Ps[0].ci = cols - 2;
		if (s.Ps[0].ci >= cols - 1)
			s.Ps[0].ci = 2;
	}
	else
	{
		if (s.Ps[0].ri <= 1 or s.Ps[0].ri >= rows-1 or s.Ps[0].ci <= 1 or s.Ps[0].ci >= cols-1)
			s.size = -1;
	}
}
void erasesnake(snake& S, char sym = ' ')
{

	for (int si = 0; si < S.size; si++)
	{
		gotoRowCol(S.Ps[si].ri, S.Ps[si].ci);
		cout << sym;
	}
}
bool isvalidfcord(position& fp, snake& s)
{
	for (int si = 0; si < s.size; si++)
	{
		if (s.Ps[si].ri == fp.ri and s.Ps[si].ci == fp.ci)
		{
			return false;
		}
	}
	return true;
}
void foodgen(position& fp, snake& s, int rows, int cols)
{
	do
	{
		fp.ri = rand() % (rows - 4) + 2;
		fp.ci = rand() % (cols - 4) + 2;
	} while (!isvalidfcord(fp, s));
}
void increasescoreandsnake(snake& s)
{
	s.score++;
	position* pPs = s.Ps;
	s.Ps = new position[s.size + 1];
	for (int si = 0; si < s.size; si++)
	{
		s.Ps[si] = pPs[si];

	}
	s.Ps[s.size] = s.Ps[s.size - 1];
	s.size += 1;
	delete[] pPs;
	pPs = nullptr;
}
void displayscore(snake& s1, snake& s2, int cols)
{
	color(10);
	gotoRowCol(4, cols + 4);
	cout << "Score: " << s1.score << "   ";
	color(7);
	color(14);
	gotoRowCol(6, cols + 4);
	cout << "Controls: Arrow Keys   ";
	color(7);
	if (s2.size > 0)
	{
		color(13);
		gotoRowCol(10, cols + 4);
		cout << "Score P2: " << s2.score << "    ";

		color(14);
		gotoRowCol(12, cols + 4);
		cout << "Controls: WASD ";
		color(7);
	}
}
bool gameover(const snake& s)
{
	for (int i = 1; i < s.size; i++)
	{
		if (s.Ps[0].ri == s.Ps[i].ri and s.Ps[0].ci == s.Ps[i].ci)
		{
			return true;
		}
	}
	return false;
}
void drawRectangle(int sr, int sc, int er, int ec, char sym)
{

	for (int r = sr; r <= er; r++)
	{
		for (int c = sc; c <= ec; c++)
		{
			if (r == sr or r == er or c == sc or c == ec)
			{
				gotoRowCol(r, c);
				cout << sym;
			}
		}
	}
	color(7);
}
void displaybf(const position& bp, char sym)
{
	color(6);
	gotoRowCol(bp.ri, bp.ci);
	cout << sym;
	color(7);
}
void bonusfg(position& bp, snake& s, int rows, int cols)
{
	do
	{
		bp.ri = rand() % (rows - 4) + 2;
		bp.ci = rand() % (cols - 4) + 2;
	} while (!isvalidfcord(bp, s));
}
void saveGame(snake& s1, snake& s2, position& fp, bool multiplayer, int level, char fsym)
{
	ofstream fout("save.txt");
	if (!fout)
		return;
	fout << (multiplayer ? 1 : 0) << " " << level << "\n";
	fout << s1.size << " " << s1.score << " " << s1.direction << "\n";
	for (int i = 0; i < s1.size; i++)
		fout << s1.Ps[i].ri << " " << s1.Ps[i].ci << "\n";
	fout << s2.size << " " << s2.score << " " << s2.direction << "\n";
	if (multiplayer && s2.size > 0) {
		for (int i = 0; i < s2.size; i++)
			fout << s2.Ps[i].ri << " " << s2.Ps[i].ci << "\n";
	}
	fout << fp.ri << " " << fp.ci << " " << (int)fsym << "\n";
	fout << (bonus ? 1 : 0) << " " << bonusp.ri << " " << bonusp.ci << " " << bonusst << "\n";

	fout.close();
}
bool loadGame(snake& s1, snake& s2, position& fp, bool& multiplayer, int& level, char& fsym)
{
	ifstream fin("save.txt");
	if (!fin) return false;
	int m;
	fin >> m >> level;
	multiplayer = (m == 1);
	fin >> s1.size >> s1.score >> s1.direction;
	if (s1.Ps) delete[] s1.Ps;
	s1.Ps = new position[s1.size];
	for (int i = 0; i < s1.size; i++) fin >> s1.Ps[i].ri >> s1.Ps[i].ci;
	int s2size, s2score, s2dir;
	fin >> s2size >> s2score >> s2dir;
	if (multiplayer && s2size > 0) {
		s2.size = s2size; s2.score = s2score; s2.direction = s2dir;
		if (s2.Ps) delete[] s2.Ps;
		s2.Ps = new position[s2.size];
		for (int i = 0; i < s2.size; i++) fin >> s2.Ps[i].ri >> s2.Ps[i].ci;
	}
	int fs;
	fin >> fp.ri >> fp.ci >> fs;
	fsym = (char)fs;
	int b;
	fin >> b >> bonusp.ri >> bonusp.ci >> bonusst;
	bonus = (b == 1);

	fin.close();
	return true;
}
bool snakesCollision(const snake& s1, const snake& s2)
{
	for (int i = 0; i < s2.size; i++)
		if (s1.Ps[0].ri == s2.Ps[i].ri and s1.Ps[0].ci == s2.Ps[i].ci)
			return true;
	for (int i = 0; i < s1.size; i++)
		if (s2.Ps[0].ri == s1.Ps[i].ri and s2.Ps[0].ci == s1.Ps[i].ci)
			return true;
	return false;
}
void drawSnake()
{
	color(12);
	gotoRowCol(2, 10);
	cout << "        /^\/^\"";
	gotoRowCol(3, 10);
	cout << "      *|__|  O|";
	gotoRowCol(4, 10);  cout << "\/     /~     \*/ \"";
	gotoRowCol(5, 10);  cout << " \****|**********/  \"";
	gotoRowCol(6, 10);  cout << "        \**_____      \"";
	gotoRowCol(7, 10);  cout << "                `\     \                 \"";
	gotoRowCol(8, 10);  cout << "                  |     |                  \"";
	gotoRowCol(9, 10);  cout << "                 /      /                    \"";
	gotoRowCol(10, 10);  cout << "                /     /                       \\";
	gotoRowCol(11, 10);  cout << "              /      /                         \ \"";
	gotoRowCol(12, 10);  cout << "             /     /                            \  \"";
	gotoRowCol(13, 10);  cout << "           /     /             *----*            \   \"";
	gotoRowCol(14, 10);  cout << "          /     /           *-~      ~-*         |   |";
	gotoRowCol(15, 10);  cout << "         (      (        *-~    *--*    ~-*     */   |";
	gotoRowCol(16, 10);  cout << "          \      ~-****-~    *-~    ~-*    ~-*-~    /";
	gotoRowCol(17, 10);  cout << "            ~-_           *-~          ~-*       *-~";
	gotoRowCol(18, 10);  cout << "               ~--******-~                ~-***-~";
	color(7);
}
void selectLevelMenu()
{
	system("cls");
	color(12);
	drawRectangle(10, 40, 18, 80, 219);
	color(6);
	gotoRowCol(11, 50);
	cout << "Select Level:";
	gotoRowCol(12, 50);
	cout << "1. Level 1 (Normal Wrap)";
	gotoRowCol(13, 50);
	cout << "2. Level 2 (Walls = Death)";
	gotoRowCol(14, 50);
	cout << "3. Level 3 (Walls + Hurdles)";
	gotoRowCol(15, 50);
	cout << "Enter choice: ";
	color(7);
	char ch = _getch();
	if (ch == '2')
		level = 2;
	else if (ch == '3')
		level = 3;
	else
		level = 1;
}
void menu(snake& s1, snake& s2, position& fp, char& fsym, int& rows, int& cols, bool& multiplayer, int& level)
{
	drawSnake();
	int menuStartRow = 20;
	color(11);
	drawRectangle(menuStartRow, 50, menuStartRow + 10, 115, 219);
	gotoRowCol(menuStartRow + 1, 60);
	cout << "    Nokia Snake Game    ";
	gotoRowCol(menuStartRow + 3, 60);
	cout << "1. Single Player";
	gotoRowCol(menuStartRow + 4, 60);
	cout << "2. Multiplayer";
	gotoRowCol(menuStartRow + 5, 60);
	cout << "3. Load Game";
	gotoRowCol(menuStartRow + 7, 60);
	cout << "Choose option: ";
	char choice = _getch();

	if (choice == '3')
	{
		if (!loadGame(s1, s2, fp, multiplayer, level, fsym))
		{
			gotoRowCol(menuStartRow + 9, 60);
			color(12);
			cout << "No saved game found!";
			_getch();
			multiplayer = false;
			innit(s1, fp, fsym, rows, cols);
		}
		else
		{
			gotoRowCol(menuStartRow + 9, 60);
			color(10);
			cout << "Game Loaded Successfully!";
			_getch();
		}
	}
	else if (choice == '2')
	{
		multiplayer = true;
		innit(s1, fp, fsym, rows, cols);
		innit(s2, fp, fsym, rows, cols, 20);
		s2.Lkey = 'A'; s2.Rkey = 'D'; s2.Ukey = 'W'; s2.Dkey = 'S';
		s2.sym = 178;
	}
	else
	{
		multiplayer = false;
		innit(s1, fp, fsym, rows, cols);
	}
}
void updateInfoPanel(const snake& s1, const snake& s2, int cols, bool multiplayer)
{
	int base = 1;
	gotoRowCol(base + 6, cols + 5);
	color(10);
	cout << "Player1 Score: " << setw(5) << s1.score << "   ";
	color(7);
	if (multiplayer)
	{
		gotoRowCol(base + 8, cols + 5);
		color(13);
		cout << "Player2 Score: " << setw(5) << s2.score << "   ";
		color(7);
	}
	gotoRowCol(base + 12, cols + 6);
	color(7);
}
int main()
{
	_getch();
	srand(time(0));
	hideConsoleCursor();
	snake s1, s2;
	s1.Ps = nullptr; s2.Ps = nullptr;
	position fp;
	char fsym;
	int rows, cols;
	bool multiplayer = false;
	innit(s1, fp, fsym, rows, cols);
	menu(s1, s2, fp, fsym, rows, cols, multiplayer, level);
	if (s2.Ps != nullptr) multiplayer = true;
	selectLevelMenu();
	system("cls");
	color(11);
	drawRectangle(1, 1, rows - 1, cols, 219);
	int panelLeft = cols + 2;
	int panelRight = cols + 40;
	int panelTop = 1;
	int panelBottom = 25;
	color(12);
	drawRectangle(panelTop, panelLeft, panelBottom, panelRight, 219);
	if (level == 3)
		drawHurdles();
	displaysnake(s1);
	if (multiplayer)
		displaysnake(s2);
	displayfruit(fp, fsym);
	gotoRowCol(8, cols + 5);
	color(14);
	cout << "Use Arrow Keys to move Player1";
	if (multiplayer) {
		gotoRowCol(10, cols + 5);
		cout << "Use WASD to move Player2";
	}
	color(7);
	bonus = false;
	while (true)
	{
		updateInfoPanel(s1, s2, cols, multiplayer);
		if (!bonus and rand() % 150 == 0)
		{
			bonusfg(bonusp, s1, rows, cols);
			bonus = true;
			bonusst = clock();
		}
		if (_kbhit())
		{
			int kp = _getch();
			if (kp == 224)
			{
				kp = _getch();
				if (kp == s1.Ukey and s1.direction != DOWN)
					s1.direction = UP;
				else if (kp == s1.Dkey and s1.direction != UP)
					s1.direction = DOWN;
				else if (kp == s1.Lkey and s1.direction != RIGHT)
					s1.direction = LEFT;
				else if (kp == s1.Rkey and s1.direction != LEFT)
					s1.direction = RIGHT;
			}
			if (multiplayer)
			{
				kp = toupper(kp);
				if (kp == s2.Ukey and s2.direction != DOWN)
					s2.direction = UP;
				else if (kp == s2.Dkey and s2.direction != UP)
					s2.direction = DOWN;
				else if (kp == s2.Lkey and s2.direction != RIGHT)
					s2.direction = LEFT;
				else if (kp == s2.Rkey and s2.direction != LEFT)
					s2.direction = RIGHT;
			}
			if (kp == 'P' or kp == 'p')
			{
				saveGame(s1, s2, fp, multiplayer, level, fsym);
				color(11);
				gotoRowCol(16, cols + 5);
				cout << "Game Saved!           ";
				color(7);
			}
		}
		erasesnake(s1);
		if (multiplayer)
			erasesnake(s2);
		movesnakestep(s1, rows, cols);
		if (multiplayer)
			movesnakestep(s2, rows, cols);
		if (gameover(s1) or (multiplayer and gameover(s2)) or (multiplayer and snakesCollision(s1, s2)) or hitsHurdle(s1) or (multiplayer and hitsHurdle(s2))
			or s1.size == -1 or (multiplayer and s2.size == -1))
		{
			gotoRowCol(rows + 2, 5);
			cout << "GAME OVER!";
			break;
		}
		displaysnake(s1);
		if (multiplayer) displaysnake(s2);
		if (foodeeaten(s1, fp))
		{
			increasescoreandsnake(s1);
			foodgen(fp, s1, rows, cols);
		}
		else if (multiplayer and foodeeaten(s2, fp))
		{
			increasescoreandsnake(s2);
			foodgen(fp, s2, rows, cols);
		}
		displayfruit(fp, fsym);
		if (bonus)
		{
			displaybf(bonusp, bonussym);
			int elapsed = clock() - bonusst;
			int remaining = (bonusd - elapsed) / 1000;
			color(9);
			gotoRowCol(14, cols + 5);
			cout << "Bonus Time: " << max(0, remaining) << "       ";
			color(7);
			if (elapsed >= bonusd)
			{
				bonus = false;
				gotoRowCol(bonusp.ri, bonusp.ci);
				cout << " ";
			}
			if (s1.Ps[0].ri == bonusp.ri and s1.Ps[0].ci == bonusp.ci)
			{
				s1.score += 5;
				increasescoreandsnake(s1);
				bonus = false;
			}
			else if (multiplayer and s2.Ps[0].ri == bonusp.ri and s2.Ps[0].ci == bonusp.ci)
			{
				s2.score += 5;
				increasescoreandsnake(s2);
				bonus = false;
			}
		}
		Sleep(10);
	}
	gotoRowCol(rows + 4, 5);
	cout << "Press any key to exit...";
	_getch();
	return 0;

}