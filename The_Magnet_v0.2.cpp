#include <graphics.h>
#include <conio.h>

#define MAXX 700
#define MAXY 650
#define BGC 11
#define STARTX 70
#define STARTY 50
#define WIDTH 70

struct Change {
    char type, x, y, i, j; // 0: Move Magnet - 1: Change Switch - 2: Move MagnetBox
};

struct ListChange {
    char count;
    Change change[4];
};

ListChange List[200];
int CountList;

int Map[8][8];
int MB[8][8];
int X, Y, P, tmpX, tmpY, tmpP, tmpX2, tmpY2, tmp, tmp2, CountMB, mx, my;
int BoolMove[4], DoneMove, Level;
char txtSelectLevel[] = "Select Level";
char txtTheMagnet[] = "The Magnet";
char txtByKnightZ[] = "By KnightZ";
char txtArrowKeysToMove[] = "arrow keys to move";
char txtEnterToAction[] = "enter to action";
char txtQToUndo[] = "q to undo";
char txtRToRestart[] = "r to restart";
char txtSharp[] = "#";
char txtNewGame[] = "new game";
char txtContinue[] = "continue";
int Mx[4] = {1, 0, -1, 0}, My[4] = {0, 1, 0, -1};
char c, NumText[18][3] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18"};
int canMove[] = {0, 1, 0, 1};

void DrawMagnet() {
    tmpX = STARTX + X * WIDTH + 14;
    tmpY = STARTY + Y * WIDTH + 14;
    setfillstyle(1, 0);
    bar(tmpX, tmpY, tmpX + 28, tmpY + 14);
    bar(tmpX, tmpY + 14, tmpX + 14, tmpY + 28);
    bar(tmpX, tmpY + 28, tmpX + 28, tmpY + 42);
    tmp = 4 - P * 3;
    setfillstyle(1, tmp);
    bar(tmpX + 28, tmpY, tmpX + 42, tmpY + 14);
    bar(tmpX + 28, tmpY + 28, tmpX + 42, tmpY + 42);
}

void ClearMagnet() {
    tmpX = STARTX + X * WIDTH + 14;
    tmpY = STARTY + Y * WIDTH + 14;
    setfillstyle(1, BGC);
    bar(tmpX, tmpY, tmpX + 42, tmpY + 14);
    bar(tmpX, tmpY + 14, tmpX + 14, tmpY + 28);
    bar(tmpX, tmpY + 28, tmpX + 42, tmpY + 42);
}

void DrawMagnetBox(int x, int y, int p) {
    tmpX = STARTX + x * WIDTH + 14;
    tmpY = STARTY + y * WIDTH + 14;
    tmp = 4 - p * 3;
    setfillstyle(1, tmp);
    bar(tmpX, tmpY, tmpX + 42, tmpY + 14);
    bar(tmpX, tmpY + 14, tmpX + 14, tmpY + 28);
    bar(tmpX + 28, tmpY + 14, tmpX + 42, tmpY + 28);
    bar(tmpX, tmpY + 28, tmpX + 42, tmpY + 42);
}

void ClearMagnetBox(int x, int y) {
    tmpX = STARTX + x * WIDTH + 14;
    tmpY = STARTY + y * WIDTH + 14;
    setfillstyle(1, BGC);
    bar(tmpX, tmpY, tmpX + 42, tmpY + 14);
    bar(tmpX, tmpY + 14, tmpX + 14, tmpY + 28);
    bar(tmpX + 28, tmpY + 14, tmpX + 42, tmpY + 28);
    bar(tmpX, tmpY + 28, tmpX + 42, tmpY + 42);
}

void DrawWall(int x, int y) {
    x = STARTX + x * WIDTH;
    y = STARTY + y * WIDTH;
    setfillstyle(1, 8);
    bar(x, y, x + 60, y + 10);
    bar(x, y + 10, x + 10, y + 60);
    setfillstyle(1, 0);
    bar(x + 10, y + 60, x + 60, y + 70);
    bar(x + 60, y + 10, x + 70, y + 70);
    setbkcolor(0);
    setfillstyle(9, 8);
    bar(x + 10, y + 10, x + 60, y + 60);
}

void DrawNothing(int x, int y) {
    x = STARTX + x * WIDTH;
    y = STARTY + y * WIDTH;
    setbkcolor(BGC);
    setfillstyle(9, 7);
    bar(x, y, x + 28, y + 14);
    bar(x + 42, y, x + 70, y + 14);
    bar(x, y + 14, x + 14, y + 28);
    bar(x + 56, y + 14, x + 70, y + 28);
    bar(x, y + 42, x + 14, y + 56);
    bar(x + 56, y + 42, x + 70, y + 56);
    bar(x, y + 56, x + 28, y + 70);
    bar(x + 42, y + 56, x + 70, y + 70);
}

void DrawDest(int x, int y) {
    x = STARTX + x * WIDTH + 14;
    y = STARTY + y * WIDTH + 14;
    setbkcolor(10);
    setfillstyle(9, 2);
    bar(x, y, x + 42, y + 42);
}

void DrawSwitch(int x, int y) {
    x = STARTX + x * WIDTH + 28;
    y = STARTY + y * WIDTH + 28;
    setfillstyle(1, 2);
    bar(x, y, x + 14, y + 14);
}

void ClearSwitch(int x, int y) {
    x = STARTX + x * WIDTH + 28;
    y = STARTY + y * WIDTH + 28;
    setfillstyle(1, BGC);
    bar(x, y, x + 14, y + 14);
}

void (*DrawPath[4])(int x, int y);

void UndoFunc(Change c) {
    switch (c.type) {
    case 0:
        ClearMagnet();
        X -= c.i;
        Y -= c.j;
        DrawMagnet();
        break;
    case 1:
        Map[Y][X] = 3;
        DrawSwitch(X, Y);
        P = 1 - P;
        DrawMagnet();
        break;
    case 2:
        ClearMagnetBox(c.x + c.i, c.y + c.j);
        MB[c.y][c.x] = MB[c.y + c.j][c.x + c.i];
        MB[c.y + c.j][c.x + c.i] = -1;
        DrawMagnetBox(c.x, c.y, MB[c.y][c.x]);
        break;
    }
}

void BackGround() {
    setcolor(8);
    setfillstyle(1, BGC);
    for (int i = 1; i < 8; i++)
        rectangle(i, i, MAXX - i, MAXY - i);
    bar(9, 9, MAXX - 8, MAXY - 8);
}

int ImportMap() {
    FILE *f;
    int *n;
    char *s = new char(10);
    sprintf(s, "Map%d.txt", Level);
    f = fopen(s, "r");
    delete s;
    if (f == NULL)
        return 0;
    fscanf(f, "%d%d%d", &X, &Y, &P);
    DrawMagnet();
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++) {
            MB[i][j] = -1;
            n = &Map[i][j];
            fscanf(f, "%d", n);
            (*n)--;
            if ((*n) > -1)
                DrawPath[(*n)](j, i);
            if ((*n) > 1)
                DrawPath[1](j, i);
        }
    fscanf(f, "%d", &CountMB);
    for (int i = 0; i < CountMB; i++) {
        fscanf(f, "%d%d%d", &tmpX, &tmpY, &tmpP);
        MB[tmpY][tmpX] = tmpP;
        DrawMagnetBox(tmpX, tmpY, tmpP);
    }
    fclose(f);
    return 1;
}

void DrawSelect(int x, int y) {
    setfillstyle(1, 4);
    x = 110 + x * 100;
    y = 160 + y * 100;
    bar(x, y, x + 15, y + 5);
    bar(x, y + 5, x + 5, y + 15);
    bar(x + 65, y, x + 80, y + 5);
    bar(x + 75, y + 5, x + 80, y + 15);
    bar(x, y + 65, x + 5, y + 75);
    bar(x, y + 75, x + 15, y + 80);
    bar(x + 75, y + 65, x + 80, y + 75);
    bar(x + 65, y + 75, x + 80, y + 80);
}

void ClearSelect(int x, int y) {
    setfillstyle(1, BGC);
    x = 110 + x * 100;
    y = 160 + y * 100;
    bar(x, y, x + 15, y + 5);
    bar(x, y + 5, x + 5, y + 15);
    bar(x + 65, y, x + 80, y + 5);
    bar(x + 75, y + 5, x + 80, y + 15);
    bar(x, y + 65, x + 5, y + 75);
    bar(x, y + 75, x + 15, y + 80);
    bar(x + 75, y + 65, x + 80, y + 75);
    bar(x + 65, y + 75, x + 80, y + 80);
}

int SelectLevel() {
    int tmpLv = 1, i, j, maxi, maxj;
    FILE *f;
    char s[10];
    BackGround();
    setcolor(1);
    settextstyle(8, 0, 5);
    outtextxy(190, 80, txtSelectLevel);
    setcolor(8);
    settextstyle(4, 0, 3);
    do {
        sprintf(s, "Map%d.txt", tmpLv);
        f = fopen(s, "r");
        if (f == NULL)
            break;
        i = (tmpLv - 1) / 5 * 100;
        j = (tmpLv - 1) % 5 * 100;
        rectangle(120 + j, 170 + i, 180 + j, 230 + i);
        rectangle(121 + j, 171 + i, 179 + j, 229 + i);
        outtextxy(130 + j, 185 + i, NumText[tmpLv - 1]);
        fclose(f);
        tmpLv++;
    } while (1);
    tmpLv -= 2;
    maxi = tmpLv / 5;
    maxj = tmpLv % 5;
    i = j = 0;
    DrawSelect(j, i);
    while (1) {
        c = getch();
        switch (c) {
        case 72:
            ClearSelect(j, i);
            i--;
            if (i < 0)
                i = ((j > maxj) ? (tmpLv / 5 - 1) : (tmpLv / 5));
            DrawSelect(j, i);
            break;
        case 80:
            ClearSelect(j, i);
            i++;
            if (i > (tmpLv / 5 - ((j > maxj) ? 1 : 0)))
                i = 0;
            DrawSelect(j, i);
            break;
        case 75:
            ClearSelect(j, i);
            j--;
            if (j < 0)
                j = ((i == maxi) ? (tmpLv % 5) : 4);
            DrawSelect(j, i);
            break;
        case 77:
            ClearSelect(j, i);
            j++;
            if (j > ((i == maxi) ? (tmpLv % 5) : 4))
                j = 0;
            DrawSelect(j, i);
            break;
        case 13:
            return i * 5 + j + 1;
        }
    }
}

void MainMenu() {
    int Choice = 0;
    BackGround();
    settextstyle(8, 0, 8);
    setbkcolor(BGC);
    setcolor(5);
    outtextxy(115, 100, txtTheMagnet);
    settextstyle(3, 0, 2);
    setcolor(9);
    outtextxy(450, 175, txtByKnightZ);
    setcolor(0);
    settextstyle(8, 0, 3);
    outtextxy(150, 475, txtArrowKeysToMove);
    outtextxy(150, 510, txtEnterToAction);
    outtextxy(150, 545, txtQToUndo);
    outtextxy(150, 580, txtRToRestart);
    settextstyle(2, 0, 10);
    setcolor(1);
    outtextxy(205, 293, txtSharp);
    outtextxy(470, 293, txtSharp);
    outtextxy(250, 290, txtNewGame);
    outtextxy(263, 360, txtContinue);
    while (1) {
        c = getch();
        switch (c) {
        case 72:
        case 75:
        case 80:
        case 77:
            setcolor(BGC);
            outtextxy(205, 293 + 70 * Choice, txtSharp);
            outtextxy(470, 293 + 70 * Choice, txtSharp);
            Choice = 1 - Choice;
            setcolor(1);
            outtextxy(205, 293 + 70 * Choice, txtSharp);
            outtextxy(470, 293 + 70 * Choice, txtSharp);
            break;
        case 13:
            if (Choice == 0) {
                Level = 1;
            } else {
                Level = SelectLevel();
            }
            return;
        }
    }
}

int Game() {
    CountMB = 0;
    CountList = 0;
    BackGround();
    if (!ImportMap())
        return 0;
    while (1) {
        c = getch();
        switch (c) {
        case 72:
            mx = 0;
            my = -1;
            break;
        case 80:
            mx = 0;
            my = 1;
            break;
        case 75:
            mx = -1;
            my = 0;
            break;
        case 77:
            mx = 1;
            my = 0;
            break;
        case 'q':
            if (CountList > 0) {
                CountList--;
                for (int i = 0; i < List[CountList].count; i++) {
                    UndoFunc(List[CountList].change[i]);
                }
            }
            mx = my = 0;
            break;
        case 27:
            return 0;
        case 'r':
            return 1;
        case 13:
            mx = my = 0;
            if (Map[Y][X] == 3) {
                Map[Y][X] = 1;
                ClearSwitch(X, Y);
                P = 1 - P;
                DrawMagnet();
                List[CountList].change[0].type = 1;
                List[CountList].count = 1;
                CountList++;
            } else {
                BoolMove[0] = BoolMove[1] = BoolMove[2] = BoolMove[3] = 1;
                DoneMove = 0;
                tmp2 = 1;
                List[CountList].count = 0;
                while (DoneMove < 4) {
                    for (int i = 0; i < 4; i++) {
                        if (BoolMove[i]) {
                            tmpX2 = X + Mx[i] * tmp2;
                            tmpY2 = Y + My[i] * tmp2;
                            if (!canMove[Map[tmpY2][tmpX2]]) {
                                BoolMove[i] = 0;
                                DoneMove++;
                            } else if (MB[tmpY2][tmpX2] > -1) {
                                BoolMove[i] = 0;
                                DoneMove++;
                                if (MB[tmpY2][tmpX2] == P) {
                                    if (canMove[Map[tmpY2 + My[i]][tmpX2 + Mx[i]]] && MB[tmpY2 + My[i]][tmpX2 + Mx[i]] == -1) {
                                        ClearMagnetBox(tmpX2, tmpY2);
                                        DrawMagnetBox(tmpX2 + Mx[i], tmpY2 + My[i], MB[tmpY2][tmpX2]);
                                        MB[tmpY2 + My[i]][tmpX2 + Mx[i]] = MB[tmpY2][tmpX2];
                                        MB[tmpY2][tmpX2] = -1;
                                        List[CountList].change[List[CountList].count].type = 2;
                                        List[CountList].change[List[CountList].count].x = tmpX2;
                                        List[CountList].change[List[CountList].count].y = tmpY2;
                                        List[CountList].change[List[CountList].count].i = Mx[i];
                                        List[CountList].change[List[CountList].count].j = My[i];
                                        List[CountList].count++;
                                    }
                                } else {
                                    if (tmp2 > 1) {
                                        ClearMagnetBox(tmpX2, tmpY2);
                                        DrawMagnetBox(tmpX2 - Mx[i], tmpY2 - My[i], MB[tmpY2][tmpX2]);
                                        MB[tmpY2 - My[i]][tmpX2 - Mx[i]] = MB[tmpY2][tmpX2];
                                        MB[tmpY2][tmpX2] = -1;
                                        List[CountList].change[List[CountList].count].type = 2;
                                        List[CountList].change[List[CountList].count].x = tmpX2;
                                        List[CountList].change[List[CountList].count].y = tmpY2;
                                        List[CountList].change[List[CountList].count].i = -Mx[i];
                                        List[CountList].change[List[CountList].count].j = -My[i];
                                        List[CountList].count++;
                                    }
                                }
                            }
                        }
                    }
                    tmp2++;
                }
                if (List[CountList].count > 0)
                    CountList++;
            }
            break;
        default:
            mx = my = 0;
            break;
        }
        if (mx || my) {
            if (Map[Y + my][X + mx] > 0 && MB[Y + my][X + mx] == -1) {
                List[CountList].change[0].type = 0;
                List[CountList].change[0].i = mx;
                List[CountList].change[0].j = my;
                List[CountList].count = 1;
                CountList++;
                ClearMagnet();
                X += mx;
                Y += my;
                DrawMagnet();
                if (Map[Y][X] == 2) {
                    Level++;
                    return 1;
                }
            }
        }
    }
}

int main() {
    DrawPath[0] = &DrawWall;
    DrawPath[1] = &DrawNothing;
    DrawPath[2] = &DrawDest;
    DrawPath[3] = &DrawSwitch;
    initwindow(MAXX + 9, MAXY + 13);
    do {
        MainMenu();
        do {
            if (!Game())
                break;
            delay(300);
        } while (1);
    } while (1);
    getch();
    closegraph();
    return 0;
}
