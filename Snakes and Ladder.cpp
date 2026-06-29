#include <iostream>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <fstream>
using namespace std;
const int capacity = 100;
const int maxplayers = 5;
void gotoRowCol(int r, int c)
{
    COORD pos = { (SHORT)c, (SHORT)r };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void color(int c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void hideConsoleCursor()
{
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);
}
void drawrectangle(int sr, int sc, int rows, int cols, char sym)
{
    for (int r = sr; r <= rows; r++)
    {
        for (int c = sc; c <= cols; c++)
        {
            if (r == sr or r == rows or c == sc or c == cols)
            {
                gotoRowCol(r, c);
                cout << sym;
            }
        }
    }
}
void oneDtotwoD(int num, int sr, int sc, int bcol, int brow, int& r1, int& c1, int& r2, int& c2)
{
    int row = (num - 1) / 10;
    int col = (num - 1) % 10;
    bool lefttoright = (row % 2 == 0);
    r1 = sr + (9 - row) * brow;
    r2 = r1 + brow - 1;
    if (lefttoright)
        c1 = sc + col * bcol;
    else
        c1 = sc + (9 - col) * bcol;
    c2 = c1 + bcol - 1;
}
void drawNumber(int sr, int sc, int rows, int cols, int num)
{
    int rc = sr + (rows - sr) / 2;
    int cc = sc + (cols - sc) / 2 - 1;
    gotoRowCol(rc, cc);
    cout << num;
}

int rollDice(int dr1, int dc1)
{
    int n = 1;
    for (int i = 0; i < 12; i++)
    {
        n = (rand() % 6) + 1;
        gotoRowCol(dr1 + 4, dc1 + 3);
        color(14);
        cout << " " << n << " ";
        Sleep(80);
    }
    color(15);
    Sleep(500);
    return n;
}
void sidepanel(int players, int positions[], char sym)
{
    int panelCol = 80;
    color(5);
    drawrectangle(1, 75, 29, 110, sym);
    color(6);
    gotoRowCol(3, panelCol);
    cout << "- Press any key to roll dice";
    gotoRowCol(4, panelCol);
    cout << "- Press Q to Save & Quit";
    gotoRowCol(6, panelCol);
    cout << "Current Player:";
    gotoRowCol(10, panelCol);
    cout << "Dice Roll:";
    gotoRowCol(12, panelCol);
    cout << "Move This Turn:";
    gotoRowCol(14, panelCol);
    cout << "Player Positions:";

    for (int p = 0; p < players; p++)
    {
        gotoRowCol(15 + p, panelCol);
        cout << char(65 + p) << ": " << positions[p] << "   ";
    }

    int dr1 = 20, dc1 = panelCol, dr2 = dr1 + 6, dc2 = dc1 + 8;
    color(11);
    drawrectangle(dr1, dc1, dr2, dc2, sym);
    gotoRowCol(dr1 + 2, dc1 + 2);
    color(12); 
    cout << " DICE ";
    color(15);
}
void credits()
{
    int creditcol = 80;

    color(12);
    drawrectangle(40, 75, 70, 112, -37);
    color(5);
    gotoRowCol(50, creditcol);
    cout << "=======Snakes and Ladder=======";
    color(4);
    gotoRowCol(52, creditcol);
    cout << "- Red color"<< "=== Snake";
    color(10);
    gotoRowCol(54, creditcol);
    cout << "- Green color===Ladder";
    color(15);
    gotoRowCol(56, creditcol);
    cout << "Made by: Muhammad Husnain Ali";
}
void board(int positions[], int players,
    int ladderStart[], int ladderEnd[], int ladderCount, int snakeStart[], int snakeEnd[], int snakeCount)
{
    system("cls");
    int sr = 2, sc = 2;
    int boxH = 7, boxW = 7;
    char sym = -37;

    for (int n = 1; n <= 100; n++)
    {
        int r1, c1, r2, c2;
        oneDtotwoD(n, sr, sc, boxH, boxW, r1, c1, r2, c2);

        bool isSnakeHead = false, isSnakeTail = false, isLadderStart = false, isLadderEnd = false;
        for (int i = 0; i < snakeCount; i++)
        {
            if (n == snakeStart[i])
                isSnakeHead = true;
            if (n == snakeEnd[i])
                isSnakeTail = true;
        }
        for (int i = 0; i < ladderCount; i++)
        {
            if (n == ladderStart[i])
                isLadderStart = true;
            if (n == ladderEnd[i])
                isLadderEnd = true;
        }
        int boxcolor = 15;


        if (isSnakeHead)
            boxcolor = 12;

        else if (isSnakeTail)
            boxcolor = 4;
        else if (isLadderStart)
            boxcolor = 10;
        else if (isLadderEnd)
            boxcolor = 2;
        else
            boxcolor = 14;

        color(boxcolor);
        drawrectangle(r1, c1, r2, c2, sym);
        color(15);

        color(boxcolor);
        drawNumber(r1, c1, r2, c2, n);
        color(15);

        for (int p = 0; p < players; p++)
        {
            if (positions[p] == n)
            {
                color(11 + p);
                gotoRowCol(r1 + 1, c1 + 1 + p);
                cout << char(219);
                gotoRowCol(r1 + 1, c1 + 2 + p);
                cout << char(219);
                color(15);
            }
        }
    }
    sidepanel(players, positions, sym);
    credits();
}


int checkladder(int pos, int ladderstart[], int ladderend[], int laddercount)
{
    for (int i = 0; i < laddercount; i++)
    {
        if (pos == ladderstart[i])
            return ladderend[i];


    }
    return pos;
}
int checksnake(int pos, int snakestart[], int snakeend[], int snakecount)
{
    for (int i = 0; i < snakecount; i++)
    {
        if (pos == snakestart[i])
            return snakeend[i];


    }
    return pos;
}
void savegame(int players, char names[][20], int positions[], int ladderstart[], int ladderend[], int laddercount,
    int snakestart[], int snakeend[], int snakecount, int turn, int winner[], int wincount)
{

    ofstream fout("save.txt");
    fout << players << "\n" << laddercount << "\n" << snakecount << "\n" << turn << "\n" << wincount << "\n";
    for (int i = 0; i < players; i++)
        fout << positions[i] << " ";
    fout << "\n";
    for (int i = 0; i < players; i++)
        fout << names[i] << " ";
    fout << "\n";
    for (int i = 0; i < laddercount; i++)
        fout << ladderstart[i] << " ";
    fout << "\n";
    for (int i = 0; i < laddercount; i++)
        fout << ladderend[i] << " ";
    fout << "\n";
    for (int i = 0; i < snakecount; i++)
        fout << snakestart[i] << " ";
    fout << "\n";
    for (int i = 0; i < snakecount; i++)
        fout << snakeend[i] << " ";
    fout << "\n";
}
bool loadinput(const char* filename, int& players, char names[][20], int positions[], int& laddercount, int ladderstart[], int ladderend[],
    int& snakecount, int snakestart[], int snakeend[], int& turn, int winner[], int& wincount)
{
    ifstream fin(filename);
    if (!fin)
        return false;
    fin >> players >> laddercount >> snakecount >> turn >> wincount;
    for (int i = 0; i < players; i++)
    {
        fin >> positions[i];
    }
    for (int i = 0; i < players; i++)
    {
        fin >> names[i];
    }
    for (int i = 0; i < laddercount; i++)
    {
        fin >> ladderstart[i];

    }
    for (int i = 0; i < laddercount; i++)
    {
        fin >> ladderend[i];

    }
    for (int i = 0; i < snakecount; i++)
        fin >> snakestart[i];
    for (int i = 0; i < snakecount; i++)
        fin >> snakeend[i];
    for (int i = 0; i < wincount; i++)
        fin >> winner[i];
    fin.close();
    return true;
}
void sinwave2(int cr, int st, int sc, int cols, int amp, int nw, char sym = -37)
{
    int cc = sc;
    float dt = (360.0 * nw) / cols;

    for (float th = st; th < 360 * nw + st; th += dt)
    {
        int f = cr - int(amp * sin(th * 3.14 / 180));

        int clr = 1 + (int(th) % 15);
        color(clr);

        gotoRowCol(f, cc);
        cout << sym;

        cc++;
    }
}
void menu()

{
    system("cls");
    hideConsoleCursor();
    int mr = 40;
    int sc = 1;
    int ec = 115;
    int w = ec - sc;
    for (int i = 0; i < 20; i++)
    {
        sinwave2(mr - 10, i, sc, w, 5, 2);
        sinwave2(mr + 10, i + 180, sc, w, 5, 2);
        color(11);
        gotoRowCol(mr, 50);
        cout << "Welcome to Husnain's Game";
        Sleep(20);
        sinwave2(mr - 10, i, sc, w, 5, 2, ' ');
        sinwave2(mr + 10, i + 180, sc, w, 5, 2, ' ');
    }
    system("cls");
    int boxrow = mr;
    int boxcol = 55;
    drawrectangle(boxrow, boxcol, boxrow + 10, boxcol + 23, -37);
    gotoRowCol(boxrow + 6, boxcol + 2);
    cout << "Press N for New Game";
    gotoRowCol(boxrow + 2, boxcol + 2);
    cout << "Press L for Load Game";
}

void playgame()
{
    int players;
    char names[maxplayers][20];
    int positions[maxplayers] = {};
    int ladderstart[10], ladderend[10], snakestart[10], snakeend[10];
    int laddercount, snakecount, turn = 0;
    int winners[maxplayers] = { 0 }, wincount = 0;
    char choice;
    int rr = 80/2, cc = 119/2;
    system("cls");
  
    menu();

    do {
       
        choice = _getch();
    } while (choice!='N' and choice != 'n' and choice!='L' and choice != 'l'  );
    color(15);
    system("cls");
    if (choice == 'l' or choice == 'L')
    {
        if (!loadinput("save.txt", players, names, positions, laddercount, ladderstart, ladderend,
            snakecount, snakestart, snakeend, turn, winners, wincount))
        {
            cout << "No saved game found.Starting new game\n";
            choice = 'N';
        }
    }
    if (choice == 'n' or choice == 'N')
    {
        cout << "\nEnter number of players (2-5)";
        cin >> players;
        for (int i = 0; i < players; i++)
        {
            cout << "Enter name of player" << i + 1 << ": ";
            cin >> names[i];
            positions[i] = 0;
        }
        laddercount = 3; snakecount = 3;
        ladderstart[0] = 2; ladderend[0] = 23;
        ladderstart[1] = 8; ladderend[1] = 34;
        ladderstart[2] = 20; ladderend[2] = 77;
        snakestart[0] = 99; snakeend[0] = 5;
        snakestart[1] = 50; snakeend[1] = 10;
        snakestart[2] = 67; snakeend[2] = 45;
        turn = 0; wincount = 0;
    }
    bool quit = false;
    do {
        board(positions, players, ladderstart, ladderend, laddercount,
            snakestart, snakeend, snakecount);

        int p = turn % players;
        bool alreadyWon = false;
        for (int w = 0; w < wincount; w++) if (winners[w] == p) alreadyWon = true;

        if (!alreadyWon)
        {
            int panelCol = 80;
            gotoRowCol(7, panelCol);
            color(11);
            cout << names[p] << "        ";
            color(15);

            int diceRow = 20, diceCol = panelCol;
            bool rolled = false;
            while (!rolled)
            {
                if (_kbhit())
                {
                    char ch = _getch();
                    if (ch == 'q' or ch == 'Q')
                    {
                        savegame(players, names, positions,
                            ladderstart, ladderend, laddercount,
                            snakestart, snakeend, snakecount,
                            turn, winners, wincount);
                        quit = true;
                        rolled = true;
                    }
                    else rolled = true;
                }
            }
            if (quit)
                break;

            int total = 0, sixCount = 0;
            bool turn = true;
            do {
                int roll = rollDice(diceRow, diceCol);
                gotoRowCol(11, panelCol); cout << "Roll: " << roll << "   ";

                if (positions[p] == 0)
                {
                    if (roll != 6)
                    {
                        gotoRowCol(13, panelCol);
                        color(12);
                        cout << "Need 6 to start!";
                        Sleep(700);
                        color(15);
                        turn = false;
                        break;
                    }
                    else
                    {
                        gotoRowCol(13, panelCol); cout << "Rolled 6! Roll again to move... ";
                        Sleep(700);
                        roll = rollDice(diceRow, diceCol);
                    }
                }

                total += roll;
                gotoRowCol(13, panelCol);
                cout << "Move this turn:" << total << "   ";
                Sleep(700);
                if (roll == 6)
                    sixCount++;
                else
                    sixCount = 0;
                if (sixCount == 3)
                {
                    total = 0;
                    turn = false;
                    break;
                }
            } while (sixCount > 0 && sixCount < 3);

            if (turn)
            {

                int newPos = positions[p] + total;
                if (newPos > 100) newPos = positions[p];
                positions[p] = newPos;
                positions[p] = checksnake(positions[p], snakestart, snakeend, snakecount);
                positions[p] = checkladder(positions[p], ladderstart, ladderend, laddercount);

                if (positions[p] == 100)
                {
                    winners[wincount] = p;
                    wincount++;
                    turn = false;
                }
            }

            for (int i = 0; i < players; i++)
            {
                gotoRowCol(15 + i, panelCol);
                cout << char(65 + i) << ": " << positions[i] << "     ";
            }
        }

        turn += 1;
    } while (wincount < players - 1 && !quit);
    if (!quit)
    {
        gotoRowCol(120, 45);
        color(11);
        cout << "=== FINAL RANKINGS ===\n";
        for (int i = 0; i < wincount; i++)
            cout << i + 1 << ". " << names[winners[i]] << endl;
    }

    _getch();
}
int main()
{
    hideConsoleCursor();
    srand(time(0));
    playgame();
    return 0;
}
