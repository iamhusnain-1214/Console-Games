#include<iostream>
#include <windows.h>
#include <conio.h>
#include<fstream>
#include <ctime>
using namespace std;
#define MAXROW 100
#define MAXCOL 200
#define capacity 10
#define MAXNAME 40
#define MAXN 15
char B[MAXROW][MAXCOL];
char PSym[capacity];
char PNames[capacity][MAXCOL];
int NOP = 0;
int Turn = 0;
int winC = 5;
int R = 0, C = 0;
bool isComputer[capacity];
bool firstBoardDraw = true;
void getRowColbyLeftClick(int& r, int& c, int sr, int sc, int brow, int bcol, int R, int C, int btnSR, int btnSC)
{
    HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
    DWORD prev_mode;
    GetConsoleMode(hInput, &prev_mode);
    SetConsoleMode(hInput, ENABLE_EXTENDED_FLAGS | ENABLE_MOUSE_INPUT);

    INPUT_RECORD ir;
    DWORD count;

    while (true)
    {
        ReadConsoleInput(hInput, &ir, 1, &count);
        if (ir.EventType == MOUSE_EVENT && ir.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
        {
            int mC = ir.Event.MouseEvent.dwMousePosition.X;
            int mR = ir.Event.MouseEvent.dwMousePosition.Y;
            if (mR >= btnSR + 20 && mR <= btnSR + 22 && mC >= btnSC + 5 && mC <= btnSC + 25)
            {
                r = -1;
                return;
            }
            int clickedCol = (mC - sc) / bcol;
            int clickedRow = (mR - sr) / brow;
            if (clickedRow >= 0 && clickedRow < R && clickedCol >= 0 && clickedCol < C)
            {
                r = clickedRow;
                c = clickedCol;
                return;
            }
        }
    }
}

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
void cellboundries(int ri, int ci, int sr, int sc, int brow, int bcol, int& r1, int& c1, int& r2, int& c2)
{
    r1 = sr + ri * brow;
    r2 = r1 + brow - 1;
    c1 = sc + ci * bcol;
    c2 = c1 + bcol - 1;
}

void cellcentre(int ri, int ci, int sr, int sc, int brow, int bcol, int& rr, int& cc)
{
    int r1, c1, r2, c2;
    cellboundries(ri, ci, sr, sc, brow, bcol, r1, c1, r2, c2);
    rr = (r1 + r2) / 2;
    cc = (c1 + c2) / 2;
}
void drawgrid(int rows, int cols, int sr, int sc, int brow, int bcol, char sym)
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            int r1, c1, r2, c2;
            cellboundries(i, j, sr, sc, brow, bcol, r1, c1, r2, c2);
            color(8 + (i + j) % 6);
            drawrectangle(r1, c1, r2, c2, sym);
            color(15);
        }
    }
}
void drawsidebox(int sr, int sc)
{
    color(5);
    drawrectangle(sr, sc, sr + 25, sc + 36, -37);
    color(15);
    gotoRowCol(sr + 1, sc + 1);
    cout << "GOMOKU - Five in a Row";
    gotoRowCol(sr + 12, sc + 3);
    cout << "Controls:";
    gotoRowCol(sr + 13, sc + 3);
    cout << "- Enter row and col to place";
    gotoRowCol(sr + 14, sc + 3);
    cout << "==Made by: Muhammad Husnain Ali==";
}

void drawplayer(int ri, int ci, int sr, int sc, int brow, int bcol, char sym, int clr)
{
    int rr, cc;
    cellcentre(ri, ci, sr, sc, brow, bcol, rr, cc);
    color(clr);
    gotoRowCol(rr, cc);
    cout << sym;
    color(15);
}

void updatesidebox(int sr, int sc, int players, char names[][MAXCOL], char sym[], int cp)
{
    int col = sc + 2;
    for (int i = 3; i <= 11; i++)
    {
        gotoRowCol(sr + i, col);
        cout << "                              ";
    }
    gotoRowCol(sr + 3, col);
    cout << "Players:";
    for (int p = 0; p < players; p++)
    {
        gotoRowCol(sr + 5 + p, col);
        cout << "                              ";
        gotoRowCol(sr + 5 + p, col);
        color(11 + p);
        cout << char(65 + p) << ": ";
        color(15);
        cout << names[p] << " (" << sym[p] << ")";
        if (p == cp)
        {
            gotoRowCol(sr + 5 + p, sc + 24);
            color(12); cout << "<- Turn";
            color(15);
        }
        else
        {
            gotoRowCol(sr + 5 + p, sc + 24);
            cout << "      ";
        }
    }
    for (int i = players; i < 4; i++)
    {
        gotoRowCol(sr + 5 + i, col);
        cout << "                              ";
    }
    color(15);
}
void saveGame()
{
    ofstream rdr("gomoku_save.txt");
    if (!rdr) 
        return;
    rdr << R << " " << C << " " << NOP << " " << Turn << " " << winC << "\n";
    for (int i = 0; i < NOP; i++)
    {
        rdr << PNames[i] << "\n";
    }
    for (int i = 0; i < NOP; i++)
    {
        rdr << PSym[i] << " " << isComputer[i] << " ";
    }
    rdr << "\n";

    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            rdr << B[i][j] << " ";
        }
        rdr << "\n";
    }
    rdr.close();
}
bool loadGame()
{
    ifstream fin("gomoku_save.txt");
    if (!fin)
        return false;
    if (!(fin >> R >> C >> NOP >> Turn >> winC))
        return false;
    fin.ignore(1000, '\n'); 
    for (int i = 0; i < NOP; i++)
    {
        fin.getline(PNames[i], MAXNAME);
    }

    for (int i = 0; i < NOP; i++)
    {
        fin >> PSym[i] >> isComputer[i];
    }
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            fin >> B[i][j];
        }
    }
    fin.close();
    firstBoardDraw = true; 
    return true;
}
bool checkWin(char board[][MAXCOL], int rows, int cols, int r, int c, int dr, int dc, char sym, int winLen)
{
    int count = 1;
    int rr = r + dr, cc = c + dc;
    while (rr >= 0 and rr < rows and cc >= 0 and cc < cols and board[rr][cc] == sym)
    {
        count += 1;
        rr += dr;
        cc += dc;
    }
    rr = r - dr;
    cc = c - dc;
    while (rr >= 0 and rr < rows and cc >= 0 and cc < cols and board[rr][cc] == sym)
    {
        count++;
        rr -= dr;
        cc -= dc;
    }
    return count >= winLen;
}
void init()
{
    system("cls");
    hideConsoleCursor();
    srand(time(0));
    gotoRowCol(2, 4); color(11);
    cout << "Enter rows R (3 - " << MAXN << ") : ";
    color(15);
    int tr;
    cin >> tr;
    if (tr < 3)
        tr = 3;
    if (tr > MAXN)
        tr = MAXN;
    R = tr;
    gotoRowCol(3, 4);
    color(11);
    cout << "Enter cols C (3 - " << MAXN << ") : ";
    color(15);
    int tc;
    cin >> tc;
    if (tc < 3)
        tc = 3;
    if (tc > MAXN)
        tc = MAXN;
    C = tc;
    gotoRowCol(5, 4);
    color(11);
    cout << "Enter number of players (2 - " << capacity << "): ";
    color(15);
    int p;
    cin >> p;
    if (p < 2)
        p = 2;
    if (p > capacity)
        p = capacity;
    NOP = p;
    gotoRowCol(7, 4);
    cout << "Enter win streak(minimun 3): ";
    int ws;
    cin >> ws;
    if (ws < 3)
        ws = 3;
    winC = ws;
    cin.ignore();
    for (int pi = 0; pi < NOP; pi++)
    {
        gotoRowCol(9 + pi * 2, 4);
        cout << "Enter name of player " << pi + 1 << " : ";
        cin.getline(PNames[pi], MAXNAME);

        int namel = 0;
        while (PNames[pi][namel] != '\0') namel++;

        gotoRowCol(9 + pi * 2, 45 + namel);
        cout << "Enter single character symbol: ";
        char ch = _getch();
        cout << ch;
        PSym[pi] = ch;
        gotoRowCol(9 + pi * 2, 90 + namel);
        cout << "Is this player a computer? (0 = No, 1 = Yes): ";
        int comp;
        cin >> comp;
        if (comp == 1)
            isComputer[pi] = true;
        else
            isComputer[pi] = false;
        cin.ignore();
    }
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            B[i][j] = '-';
        }

    }
    Turn = rand() % NOP;
}
void displayBoard(bool fullRefresh = false)
{
    int sr = 2, sc = 2;
    int crow = 6, ccol = 6;
    if (R <= 10)
    {
        crow = 4;
        ccol = 7;
    }
    else if (R <= 15)
    {
        crow = 3;
        ccol = 6;
    }

    if (firstBoardDraw or fullRefresh)
    {
        system("cls");
        drawgrid(R, C, sr, sc, crow, ccol, -37);
        int boardw = sc + C * ccol;
        int pCol = 90;
        if (pCol <= boardw + 2)
            pCol = boardw + 3;
        drawsidebox(2, pCol);
        firstBoardDraw = false;
    }
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            char ch = B[i][j];
            if (ch != '-')
            {
                int rr, cc;
                cellcentre(i, j, sr, sc, crow, ccol, rr, cc);
                int id = 0;
                for (int pi = 0; pi < NOP; pi++)
                {
                    if (PSym[pi] == ch)
                        id = pi;
                }
                drawplayer(i, j, sr, sc, crow, ccol, ch, 11 + (id % 6));
            }
        }
    }

    int boardw = sc + C * ccol;
    int pCol = 90;
    if (pCol <= boardw + 2)
        pCol = boardw + 3;
    updatesidebox(2, pCol, NOP, PNames, PSym, Turn);
}
void displayMsg()
{
    int sr = 2;
    int sc = 2 + C * 6 + 3;
    if (sc + 32 > 120)
        sc = 120 - 32;
    int msgRow = sr + 36;
    gotoRowCol(msgRow + 5, sc);
    if (isComputer[Turn])
        cout << "Computer " << PNames[Turn] << "'s turn. Symbol: " << PSym[Turn] << " ";
    else
        cout << "Player " << PNames[Turn] << "'s turn. Symbol: " << PSym[Turn] << " ";
}

bool isValid(int r, int c)
{
    if (r < 0 or r >= R or c < 0 or c >= C)
        return false;
    if (B[r][c] != '-')
        return false;
    return true;
}
bool isWin(int r, int c)
{
    char sym = B[r][c];
    if (sym == '-') return false;
    if (checkWin(B, R, C, r, c, 0, 1, sym, winC))
        return true;
    if (checkWin(B, R, C, r, c, 1, 0, sym, winC))
        return true;
    if (checkWin(B, R, C, r, c, 1, 1, sym, winC))
        return true;
    if (checkWin(B, R, C, r, c, 1, -1, sym, winC))
        return true;
    return false;
}
bool willWin(char sym, int r, int c)
{
    B[r][c] = sym;
    bool result = isWin(r, c);
    B[r][c] = '-';
    return result;
}
void ComputerMove(int& r, int& c)
{
    char compSym = PSym[Turn];
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++)
            if (B[i][j] == '-' and willWin(compSym, i, j))
            {
                r = i;
                c = j;
                return;
            }
    for (int p = 0; p < NOP; p++)
    {
        if (p != Turn)  
        {
            char playerSym = PSym[p];
            for (int i = 0; i < R; i++)
                for (int j = 0; j < C; j++)
                    if (B[i][j] == '-' and willWin(playerSym, i, j))
                    {
                        r = i;
                        c = j;
                        return;
                    }
        }
    }
    do
    {
        r = rand() % R;
        c = rand() % C;
    } while (!isValid(r, c));
}
void SelectPos(int& r, int& c)
{
    if (isComputer[Turn])
    {
        ComputerMove(r, c);
        gotoRowCol(2 + R * 3 + 10, 90);
        cout << "Computer " << PNames[Turn] << " placed at (" << r + 1 << "," << c + 1 << ")   ";
        _getch();
        return;
    }
    int sr = 2, sc = 2;
    int cellH = (R <= 10) ? 4 : 3;
    int cellW = (R <= 10) ? 7 : 6;
    int panelCol = 90;
    if (panelCol <= (sc + C * cellW) + 2) 
        panelCol = (sc + C * cellW) + 3;
    drawrectangle(sr + 20, panelCol + 5, sr + 22, panelCol + 25, -37);
    gotoRowCol(sr + 21, panelCol + 10);
    cout << "SAVE & EXIT";
    color(15);
    bool validi = false;
    while (!validi)
    {
        int tr, tc;
        getRowColbyLeftClick(tr, tc, sr, sc, cellH, cellW, R, C, sr, panelCol);
        if (tr == -1)
        {
            saveGame();
            system("cls");
            color(10);
            cout << "\n\n\tGame saved! Exiting...\n";
            color(15);
            Sleep(1000);
            exit(0);
        }
        if (isValid(tr, tc))
        {
            r = tr;
            c = tc;
            validi = true;
        }
        else
        {
            gotoRowCol(sr + 24, panelCol + 5);
            color(12); cout << "Invalid! Try again."; color(15);
        }
    }
}
void updateBoard(int r, int c)
{
    B[r][c] = PSym[Turn];
}
void TurnChange()
{
    Turn = (Turn + 1) % NOP;
}
bool isDraw()
{
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++) 
            if (B[i][j] == '-') 
                return false;
    return true;
}
void menu()
{
    int w = 170;
    int h = 44;
    int bw = 40;
    int bh = 10;
    int cr = (h - bh) / 2;
    int cc = (w - bw) / 2;
    color(11);
    drawrectangle(cr, cc, cr + bh, cc + bw, char(-37));
    color(15);
    color(14);
    gotoRowCol(cr + 2, cc + 10);
    cout << "GOMOKU MAIN MENU";
    color(15);
    gotoRowCol(cr + 5, cc + 5);
    cout << "1. Load Game";
    gotoRowCol(cr + 6, cc + 5);
    cout << "2. New Game";
    gotoRowCol(cr + 8, cc + 5);
    cout << "Enter your choice: ";
    int ch;
    cin >> ch;
    if (ch == 1)
    {
        if (!loadGame())
        {
            cout << "No saved game found..Lets start fresh\n";
            _getch();
            init();
        }
    }
    else
    {
        init();
    }
}
int main()
{
    _getch();
    menu();
    int sr = 2, sc = 2;
    int cellH = 6, cellW = 6;
    if (R <= 10)
    {
        cellH = 4;
        cellW = 7;
    }
    else if (R <= 15)
    {
        cellH = 3;
        cellW = 6;
    }
    int Moves = 0;
    int maxMoves = R * C;
    int winner = -1;
    while (true)
    {
        displayBoard();
        displayMsg();
        int pr, pc;
        SelectPos(pr, pc);
        if (pr == -1 and pc == -1)
        {
            gotoRowCol(2 + R * cellH + 6, 4);
            cout << "Player quit. Exiting...";
            break;
        }
        updateBoard(pr, pc);
        drawplayer(pr, pc, sr, sc, cellH, cellW, B[pr][pc], 11 + (Turn % 6));
        Moves++;
        if (isWin(pr, pc))
        {
            winner = Turn;
            break;
        }
        if (isDraw())
        {
            break;
        }
        TurnChange();
    }

    int boardWidth = sc + C * cellW;
    int panelCol = 150;
    if (panelCol <= boardWidth + 2)
        panelCol = boardWidth + 3;
    int inputRow = 2 + R * cellH + 2;
    gotoRowCol(inputRow + 4, panelCol);
    for (int i = 0; i < 40; i++)
    {
        cout << ' ';
    }
    if (winner != -1)
    {
        color(10);
        gotoRowCol(inputRow + 5, panelCol + 5);
        cout << "Player " << PNames[winner] << " (" << PSym[winner] << ") WINS!";
        color(15);
    }
    else
    {
        color(14);
        gotoRowCol(inputRow + 5, panelCol + 5);
        cout << "Game Draw! No winner.";
        color(15);
    }
    gotoRowCol(inputRow + 7, panelCol + 5);
    cout << "Press any key to exit...";
    _getch();
    return 0;
}