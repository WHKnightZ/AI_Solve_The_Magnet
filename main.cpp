#include <conio.h>
#include <graphics.h>

#define MAXX 700
#define MAXY 650
#define BGC 11
#define STARTX 70
#define STARTY 50
#define WIDTH 70
#define MAXDEG 25
#define MAXLIST 20000
#define MAXUNDO 200
#define DELAY 300

struct Point {
    char x, y;
};

struct Change {
    char type, x, y, i, j;
    Change() {
    }
    Change(char type) {
        this->type = type;
    }
    Change(char type, char x, char y, char i, char j) {
        this->type = type;
        this->x = x;
        this->y = y;
        this->i = i;
        this->j = j;
    }
};

struct ListUndo {
    char count;
    Change change[4];
};

struct Path {
    Point point;
    struct Path *next;
};

struct ListChange {
    char x, y;
    Path *path;
    Change *change[4];
    char count;
    int prev;
};

struct Degree {
    ListChange *list[MAXLIST];
    int count;
};

struct ExistList {
    char x, y, i, j;
};

ListChange *tmpList;
Path *tmpPath;
int *ptr;
Point listPath[MAXDEG];
int currentPath;
Point listDest[20];
int currentDest;
ExistList existList[MAXDEG];
int countExistList;
Degree deg[MAXDEG];
int currentDeg;
Point listPoint[2][16];
int countPoint[2];
int currentList = 0, rCurrentList;
ListUndo listUndo[MAXUNDO];
int countListUndo;
Point tmpMap[8][8];
int ListPrev[MAXDEG];
int tX, tY, nn, xx, yy, ii, jj, i, j, k, l;
int Map[8][8], saveMap[8][8];
int MB[8][8], saveMB[8][8];
int X, Y, P, saveP, tmpX, tmpY, tmpP, tmpX2, tmpY2, tmp, tmp2, CountMB, mx, my, tmpX3, isChange, isCreatePath;
int BoolMove[4], DoneMove, Level, nList;
char txtSelectLevel[] = "Select Level";
char txtTheMagnet[] = "The Magnet";
char txtByKnightZ[] = "By KnightZ";
char txtArrowsToMove[] = "arrows to move";
char txtEnterToAction[] = "enter to action";
char txtQToUndo[] = "q to undo";
char txtRToRestart[] = "r to restart";
char txtEToSolve[] = "e to solve";
char txtESCToQuit[] = "esc to quit";
char txtSharp[] = "#";
char txtNewGame[] = "new game";
char txtContinue[] = "continue";
char txtDone[] = "Done!";
char txtCanSolve[] = "Can't Solve!";
int Mx[] = {1, 0, -1, 0}, My[] = {0, 1, 0, -1};
char c, NumText[][3] = {"01", "02", "03", "04", "05", "06", "07", "08", "09", "10", "11", "12", "13", "14", "15", "16", "17", "18", "19", "20", "21", "22", "23", "24", "25", "26", "27", "28", "29", "30"};
int canMove[] = {0, 1, 0, 1};
int canMove2[] = {0, 1, 1, 1};
char str[20];

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
    sprintf(str, "Maps/%02d.txt", Level);
    f = fopen(str, "r");
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
            if ((*n) > -1){
            	DrawPath[(*n)](j, i);
            	if ((*n) > 1)
                	DrawPath[1](j, i);
			}
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

void ChangeFunc(Change *c) {
    switch (c->type) {
    case 0:
        Map[Y][X] = 1;
        P = 1 - P;
        break;
    case 1:
        MB[c->y + c->j][c->x + c->i] = MB[c->y][c->x];
        MB[c->y][c->x] = -1;
        break;
    }
}

void UndoChangeFunc(Change *c) {
    switch (c->type) {
    case 0:
        Map[Y][X] = 3;
        P = 1 - P;
        break;
    case 1:
        MB[c->y][c->x] = MB[c->y + c->j][c->x + c->i];
        MB[c->y + c->j][c->x + c->i] = -1;
        break;
    }
}

int IsExist(Change *c) {
    for (nn = 0; nn < countExistList; nn++) {
        if (existList[nn].x == c->x && existList[nn].y == c->y && existList[nn].i == c->i && existList[nn].j == c->j)
            return 1;
    }
    return 0;
}

int BFS() {
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++)
            tmpMap[i][j].x = -2;
    }
    tmpMap[Y][X].x = -1;
    listPoint[currentList][0].x = X;
    listPoint[currentList][0].y = Y;
    countPoint[currentList] = 1;
    countExistList = 0;
    while (countPoint[currentList] > 0) {
        rCurrentList = 1 - currentList;
        countPoint[rCurrentList] = 0;
        for (i = 0; i < countPoint[currentList]; i++) {
            tX = listPoint[currentList][i].x;
            tY = listPoint[currentList][i].y;
            isChange = 0;
            if (Map[tY][tX] == 2) {
                currentDest = 0;
                tmpX = tX;
                tmpY = tY;
                listDest[currentDest].x = tmpX;
                listDest[currentDest].y = tmpY;
                currentDest++;
                while (tmpMap[tmpY][tmpX].x != -1) {
                    listDest[currentDest].x = tmpMap[tmpY][tmpX].x;
                    listDest[currentDest].y = tmpMap[tmpY][tmpX].y;
                    currentDest++;
                    tmpX3 = tmpX;
                    tmpX = tmpMap[tmpY][tmpX3].x;
                    tmpY = tmpMap[tmpY][tmpX3].y;
                }
                return 1;
            }
            if (Map[tY][tX] == 3) {
                isChange = 1;
                deg[currentDeg + 1].list[deg[currentDeg + 1].count] = new ListChange();
                tmpList = deg[currentDeg + 1].list[deg[currentDeg + 1].count];
                tmpList->prev = nList;
                tmpList->x = tX;
                tmpList->y = tY;
                tmpList->change[0] = new Change(0);
                tmpList->count = 1;
                deg[currentDeg + 1].count++;
            } else {
                for (j = 0; j < 4; j++) {
                    tmpX = tX + Mx[j];
                    tmpY = tY + My[j];
                    while (1) {
                        if (!canMove[Map[tmpY][tmpX]])
                            break;
                        if (MB[tmpY][tmpX] == P) {
                            if (canMove[Map[tmpY + My[j]][tmpX + Mx[j]]] && MB[tmpY + My[j]][tmpX + Mx[j]] == -1) {
                                if (!isChange) {
                                    isChange = 1;
                                    tmpList = deg[currentDeg + 1].list[deg[currentDeg + 1].count] = new ListChange();
                                    tmpList->count = 0;
                                    tmpList->prev = nList;
                                    tmpList->x = tX;
                                    tmpList->y = tY;
                                    deg[currentDeg + 1].count++;
                                }
                                tmpList->change[tmpList->count] = new Change(1, tmpX, tmpY, Mx[j], My[j]);
                                tmpList->count++;
                            }
                            break;
                        } else if (MB[tmpY][tmpX] == 1 - P) {
                            if (tmpX - Mx[j] != tX || tmpY - My[j] != tY) {
                                if (!isChange) {
                                    isChange = 1;
                                    tmpList = deg[currentDeg + 1].list[deg[currentDeg + 1].count] = new ListChange();
                                    tmpList->count = 0;
                                    tmpList->prev = nList;
                                    tmpList->x = tX;
                                    tmpList->y = tY;
                                    deg[currentDeg + 1].count++;
                                }
                                tmpList->change[tmpList->count] = new Change(1, tmpX, tmpY, -Mx[j], -My[j]);
                                tmpList->count++;
                            }
                            break;
                        }
                        tmpX += Mx[j];
                        tmpY += My[j];
                    }
                }
            }
            if (isChange) {
                isCreatePath = 0;
                if (tmpList->count == 1 && tmpList->change[0]->type == 1) {
                    if (IsExist(tmpList->change[0])) {
                        deg[currentDeg + 1].count--;
                        delete tmpList->change[0];
                        delete tmpList;
                    } else
                        isCreatePath = 1;
                } else
                    isCreatePath = 1;
                if (isCreatePath) {
                    tmpX = tX;
                    tmpY = tY;
                    tmpList->path = new Path();
                    tmpPath = tmpList->path;
                    tmpPath->next = NULL;
                    tmpPath->point.x = tmpX;
                    tmpPath->point.y = tmpY;
                    while (tmpMap[tmpY][tmpX].x != -1) {
                        tmpPath->next = new Path();
                        tmpPath = tmpPath->next;
                        tmpPath->next = NULL;
                        tmpPath->point.x = tmpMap[tmpY][tmpX].x;
                        tmpPath->point.y = tmpMap[tmpY][tmpX].y;
                        tmpX3 = tmpX;
                        tmpX = tmpMap[tmpY][tmpX3].x;
                        tmpY = tmpMap[tmpY][tmpX3].y;
                    }
                    existList[countExistList].x = tmpList->change[0]->x;
                    existList[countExistList].y = tmpList->change[0]->y;
                    existList[countExistList].i = tmpList->change[0]->i;
                    existList[countExistList].j = tmpList->change[0]->j;
                    countExistList++;
                }
            }
            if (canMove2[Map[tY - 1][tX]] && MB[tY - 1][tX] == -1 && tmpMap[tY - 1][tX].x == -2) {
                tmpMap[tY - 1][tX].x = tX;
                tmpMap[tY - 1][tX].y = tY;
                listPoint[rCurrentList][countPoint[rCurrentList]].x = tX;
                listPoint[rCurrentList][countPoint[rCurrentList]].y = tY - 1;
                countPoint[rCurrentList]++;
            }
            if (canMove2[Map[tY][tX + 1]] && MB[tY][tX + 1] == -1 && tmpMap[tY][tX + 1].x == -2) {
                tmpMap[tY][tX + 1].x = tX;
                tmpMap[tY][tX + 1].y = tY;
                listPoint[rCurrentList][countPoint[rCurrentList]].x = tX + 1;
                listPoint[rCurrentList][countPoint[rCurrentList]].y = tY;
                countPoint[rCurrentList]++;
            }
            if (canMove2[Map[tY + 1][tX]] && MB[tY + 1][tX] == -1 && tmpMap[tY + 1][tX].x == -2) {
                tmpMap[tY + 1][tX].x = tX;
                tmpMap[tY + 1][tX].y = tY;
                listPoint[rCurrentList][countPoint[rCurrentList]].x = tX;
                listPoint[rCurrentList][countPoint[rCurrentList]].y = tY + 1;
                countPoint[rCurrentList]++;
            }
            if (canMove2[Map[tY][tX - 1]] && MB[tY][tX - 1] == -1 && tmpMap[tY][tX - 1].x == -2) {
                tmpMap[tY][tX - 1].x = tX;
                tmpMap[tY][tX - 1].y = tY;
                listPoint[rCurrentList][countPoint[rCurrentList]].x = tX - 1;
                listPoint[rCurrentList][countPoint[rCurrentList]].y = tY;
                countPoint[rCurrentList]++;
            }
        }
        currentList = rCurrentList;
    }
    return 0;
}

void GotoDest() {
    for (l = currentDest - 2; l >= 0; l--) {
        ClearMagnet();
        X = listDest[l].x;
        Y = listDest[l].y;
        DrawMagnet();
        delay(DELAY);
    }
    settextstyle(10, 0, 5);
    setcolor(2);
    setbkcolor(BGC);
    outtextxy(300, 550, txtDone);
    delay(500);
}

void MoveFunc(char &x, char &y) {
    ClearMagnet();
    X = x;
    Y = y;
    DrawMagnet();
}

void ActionFunc(ListChange *lc) {
    if (lc->count == 1) {
        if (lc->change[0]->type == 0) {
            ClearSwitch(X, Y);
            Map[Y][X] = 1;
            P = 1 - P;
            DrawMagnet();
        } else {
            ptr = &MB[lc->change[0]->y + lc->change[0]->j][lc->change[0]->x + lc->change[0]->i];
            *ptr = MB[lc->change[0]->y][lc->change[0]->x];
            MB[lc->change[0]->y][lc->change[0]->x] = -1;
            ClearMagnetBox(lc->change[0]->x, lc->change[0]->y);
            DrawMagnetBox(lc->change[0]->x + lc->change[0]->i, lc->change[0]->y + lc->change[0]->j, *ptr);
        }
    } else
        for (nn = 0; nn < lc->count; nn++) {
            ptr = &MB[lc->change[nn]->y + lc->change[nn]->j][lc->change[nn]->x + lc->change[nn]->i];
            *ptr = MB[lc->change[nn]->y][lc->change[nn]->x];
            MB[lc->change[nn]->y][lc->change[nn]->x] = -1;
            ClearMagnetBox(lc->change[nn]->x, lc->change[nn]->y);
            DrawMagnetBox(lc->change[nn]->x + lc->change[nn]->i, lc->change[nn]->y + lc->change[nn]->j, *ptr);
        }
}

void FreeMemory() {
    Path *tmpPath2;
    for (i = 0; i <= currentDeg; i++) {
        for (j = 0; j < deg[i].count; j++) {
            for (k = 0; k < deg[i].list[j]->count; k++)
                delete deg[i].list[j]->change[k];
            tmpPath = deg[i].list[j]->path;
            while (tmpPath != NULL) {
                tmpPath2 = tmpPath->next;
                delete tmpPath;
                tmpPath = tmpPath2;
            }
            delete deg[i].list[j];
        }
    }
}

int Solve() {
    currentDeg = -1;
    deg[currentDeg + 1].count = 0;
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            saveMap[i][j] = Map[i][j];
            saveMB[i][j] = MB[i][j];
        }
    }
    saveP = P;
    int done = BFS();
    currentDeg++;
    if (done) {
        GotoDest();
        FreeMemory();
        Level++;
        return 1;
    }
    int tmpN;
    do {
        deg[currentDeg + 1].count = 0;
        for (nList = 0; nList < deg[currentDeg].count; nList++) {
            tmpN = nList;
            for (k = currentDeg; k >= 0; k--) {
                ListPrev[k] = tmpN;
                tmpN = deg[k].list[tmpN]->prev;
            }
            xx = X;
            yy = Y;
            for (k = 0; k <= currentDeg; k++) {
                X = deg[k].list[ListPrev[k]]->x;
                Y = deg[k].list[ListPrev[k]]->y;
                for (l = 0; l < deg[k].list[ListPrev[k]]->count; l++) {
                    ChangeFunc(deg[k].list[ListPrev[k]]->change[l]);
                }
            }
            done = BFS();
            if (done) {
                for (i = 0; i < 8; i++) {
                    for (j = 0; j < 8; j++) {
                        Map[i][j] = saveMap[i][j];
                        MB[i][j] = saveMB[i][j];
                    }
                }
                P = saveP;
                X = xx;
                Y = yy;
                for (k = 0; k <= currentDeg; k++) {
                    tmpPath = deg[k].list[ListPrev[k]]->path;
                    currentPath = 0;
                    while (tmpPath != NULL) {
                        listPath[currentPath] = tmpPath->point;
                        currentPath++;
                        tmpPath = tmpPath->next;
                    }
                    for (l = currentPath - 2; l >= 0; l--) {
                        MoveFunc(listPath[l].x, listPath[l].y);
                        delay(DELAY);
                    }
                    ActionFunc(deg[k].list[ListPrev[k]]);
                    delay(DELAY);
                }
                GotoDest();
                currentDeg++;
                FreeMemory();
                Level++;
                return 1;
            }
            for (k = currentDeg; k >= 0; k--) {
                for (l = deg[k].list[ListPrev[k]]->count - 1; l >= 0; l--) {
                    UndoChangeFunc(deg[k].list[ListPrev[k]]->change[l]);
                    if (k > 0) {
                        X = deg[k - 1].list[ListPrev[k - 1]]->x;
                        Y = deg[k - 1].list[ListPrev[k - 1]]->y;
                    } else {
                        X = xx;
                        Y = yy;
                    }
                }
            }
        }
        currentDeg++;
        if (currentDeg == MAXDEG - 1)
            break;
    } while (deg[currentDeg].count);
    FreeMemory();
    settextstyle(10, 0, 5);
    setcolor(2);
    setbkcolor(BGC);
    outtextxy(210, 550, txtCanSolve);
    delay(500);
    return 0;
}

int SelectLevel() {
    int tmpLv = 1, maxi, maxj;
    FILE *f;
    char s[10];
    BackGround();
    setcolor(1);
    settextstyle(8, 0, 5);
    outtextxy(190, 80, txtSelectLevel);
    setcolor(8);
    settextstyle(4, 0, 3);
    do {
        sprintf(s, "Maps/%02d.txt", tmpLv);
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
    outtextxy(145, 485, txtArrowsToMove);
    outtextxy(145, 520, txtEnterToAction);
    outtextxy(145, 555, txtQToUndo);
    outtextxy(395, 485, txtRToRestart);
    outtextxy(395, 520, txtEToSolve);
    outtextxy(395, 555, txtESCToQuit);
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
    countListUndo = 0;
    BackGround();
    if (!ImportMap())
        return 0;
    while (1) {
        c = getch();
        switch (c) {
        case 27:
            return 0;
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
            if (countListUndo > 0) {
                countListUndo--;
                for (int i = 0; i < listUndo[countListUndo].count; i++) {
                    UndoFunc(listUndo[countListUndo].change[i]);
                }
            }
            mx = my = 0;
            break;
        case 'e':
            Solve();
            return 1;
        case 'r':
            return 1;
        case 13:
            mx = my = 0;
            if (Map[Y][X] == 3) {
                Map[Y][X] = 1;
                ClearSwitch(X, Y);
                P = 1 - P;
                DrawMagnet();
                listUndo[countListUndo].change[0].type = 1;
                listUndo[countListUndo].count = 1;
                countListUndo++;
            } else {
                BoolMove[0] = BoolMove[1] = BoolMove[2] = BoolMove[3] = 1;
                DoneMove = 0;
                tmp2 = 1;
                listUndo[countListUndo].count = 0;
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
                                        listUndo[countListUndo].change[listUndo[countListUndo].count].type = 2;
                                        listUndo[countListUndo].change[listUndo[countListUndo].count].x = tmpX2;
                                        listUndo[countListUndo].change[listUndo[countListUndo].count].y = tmpY2;
                                        listUndo[countListUndo].change[listUndo[countListUndo].count].i = Mx[i];
                                        listUndo[countListUndo].change[listUndo[countListUndo].count].j = My[i];
                                        listUndo[countListUndo].count++;
                                    }
                                } else {
                                    if (tmp2 > 1) {
                                        ClearMagnetBox(tmpX2, tmpY2);
                                        DrawMagnetBox(tmpX2 - Mx[i], tmpY2 - My[i], MB[tmpY2][tmpX2]);
                                        MB[tmpY2 - My[i]][tmpX2 - Mx[i]] = MB[tmpY2][tmpX2];
                                        MB[tmpY2][tmpX2] = -1;
                                        listUndo[countListUndo].change[listUndo[countListUndo].count].type = 2;
                                        listUndo[countListUndo].change[listUndo[countListUndo].count].x = tmpX2;
                                        listUndo[countListUndo].change[listUndo[countListUndo].count].y = tmpY2;
                                        listUndo[countListUndo].change[listUndo[countListUndo].count].i = -Mx[i];
                                        listUndo[countListUndo].change[listUndo[countListUndo].count].j = -My[i];
                                        listUndo[countListUndo].count++;
                                    }
                                }
                            }
                        }
                    }
                    tmp2++;
                }
                if (listUndo[countListUndo].count > 0)
                    countListUndo++;
            }
            break;
        default:
            mx = my = 0;
            break;
        }
        if (mx || my) {
            if (Map[Y + my][X + mx] > 0 && MB[Y + my][X + mx] == -1) {
                listUndo[countListUndo].change[0].type = 0;
                listUndo[countListUndo].change[0].i = mx;
                listUndo[countListUndo].change[0].j = my;
                listUndo[countListUndo].count = 1;
                countListUndo++;
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
