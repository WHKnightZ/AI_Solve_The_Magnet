#include <stdio.h>
#include <stdlib.h>

#define MAXDEG 40
#define MAXLIST 20000

typedef struct Point {
    char x, y;
} Point;

Point listPath[MAXDEG];
int currentPath;
Point listDest[20];
int currentDest;

Point listPoint[2][16];
int countPoint[2];
int currentList = 0, rCurrentList;
int canMove[] = {0, 0, 1, 0, 1};
int canMove2[] = {0, 0, 1, 1, 1};
int Mx[] = {0, 1, 0, -1};
int My[] = {-1, 0, 1, 0};

typedef struct Path {
    Point point;
    struct Path *next;
} Path;

typedef struct Change {
    char type, x, y, i, j;
} Change;

typedef struct ListChange {
    char x, y;
    Path *path;
    Change *change[4];
    int count, prev;
} ListChange;

typedef struct Degree {
    ListChange *list[MAXLIST];
    int count;
} Degree;

typedef struct ExistList {
    char x, y, i, j;
} ExistList;

ExistList existList[MAXDEG];
int countExistList;

ListChange *tmpList;
Path *tmpPath;

Degree deg[MAXDEG];
int currentDeg;

int i, j, k, l, nList;
int X, Y, P, countMB, tmpX, tmpY, tmpX2, tmpY2, tmpX3;
int Map[8][8];
int MB[8][8];
int isChange;
Point tmpMap[8][8];
int nn;
int ListPrev[MAXDEG];
int tX, tY;

void importMap(const char *map) {
    FILE *f = fopen(map, "r");
    fscanf(f, "%d%d%d", &X, &Y, &P);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 8; j++) {
            MB[i][j] = -1;
            fscanf(f, "%d", &Map[i][j]);
        }
    }
    fscanf(f, "%d", &countMB);
    for (i = 0; i < countMB; i++) {
        fscanf(f, "%d%d%d", &j, &k, &l);
        MB[k][j] = l;
    }
    fclose(f);
}

void ChangeFunc(Change *c) {
    switch (c->type) {
    case 0:
        Map[Y][X] = 2;
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
        Map[Y][X] = 4;
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
            if (Map[tY][tX] == 3) {
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
            if (Map[tY][tX] == 4) {
                isChange = 1;
                deg[currentDeg + 1].list[deg[currentDeg + 1].count] = (ListChange *)malloc(sizeof(ListChange));
                tmpList = deg[currentDeg + 1].list[deg[currentDeg + 1].count];
                deg[currentDeg + 1].count++;
                tmpList->prev = nList;
                tmpList->x = tX;
                tmpList->y = tY;
                tmpList->change[0] = (Change *)malloc(sizeof(Change));
                tmpList->change[0]->type = 0;
                tmpList->count = 1;
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
                                    deg[currentDeg + 1].list[deg[currentDeg + 1].count] = (ListChange *)malloc(sizeof(ListChange));
                                    tmpList = deg[currentDeg + 1].list[deg[currentDeg + 1].count];
                                    deg[currentDeg + 1].count++;
                                    tmpList->count = 0;
                                    tmpList->prev = nList;
                                    tmpList->x = tX;
                                    tmpList->y = tY;
                                }
                                tmpList->change[tmpList->count] = (Change *)malloc(sizeof(Change));
                                tmpList->change[tmpList->count]->type = 1;
                                tmpList->change[tmpList->count]->x = tmpX;
                                tmpList->change[tmpList->count]->y = tmpY;
                                tmpList->change[tmpList->count]->i = Mx[j];
                                tmpList->change[tmpList->count]->j = My[j];
                                tmpList->count++;
                            }
                            break;
                        } else if (MB[tmpY][tmpX] == 1 - P) {
                            if (tmpX - Mx[j] != tX || tmpY - My[j] != tY) {
                                if (!isChange) {
                                    isChange = 1;
                                    deg[currentDeg + 1].list[deg[currentDeg + 1].count] = (ListChange *)malloc(sizeof(ListChange));
                                    tmpList = deg[currentDeg + 1].list[deg[currentDeg + 1].count];
                                    deg[currentDeg + 1].count++;
                                    tmpList->count = 0;
                                    tmpList->prev = nList;
                                    tmpList->x = tX;
                                    tmpList->y = tY;
                                }
                                tmpList->change[tmpList->count] = (Change *)malloc(sizeof(Change));
                                tmpList->change[tmpList->count]->type = 1;
                                tmpList->change[tmpList->count]->x = tmpX;
                                tmpList->change[tmpList->count]->y = tmpY;
                                tmpList->change[tmpList->count]->i = -Mx[j];
                                tmpList->change[tmpList->count]->j = -My[j];
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
                tmpX = tX;
                tmpY = tY;
                tmpList->path = (Path *)malloc(sizeof(Path));
                tmpPath = tmpList->path;
                tmpPath->next = NULL;
                tmpPath->point.x = tmpX;
                tmpPath->point.y = tmpY;
                while (tmpMap[tmpY][tmpX].x != -1) {
                    tmpPath->next = (Path *)malloc(sizeof(Path));
                    tmpPath = tmpPath->next;
                    tmpPath->next = NULL;
                    tmpPath->point.x = tmpMap[tmpY][tmpX].x;
                    tmpPath->point.y = tmpMap[tmpY][tmpX].y;
                    tmpX3 = tmpX;
                    tmpX = tmpMap[tmpY][tmpX3].x;
                    tmpY = tmpMap[tmpY][tmpX3].y;
                }
                if (tmpList->count == 1 && tmpList->change[0]->type == 1) {
                    if (IsExist(tmpList->change[0]))
                        deg[currentDeg + 1].count--;
                    else {
                        existList[countExistList].x = tmpList->change[0]->x;
                        existList[countExistList].y = tmpList->change[0]->y;
                        existList[countExistList].i = tmpList->change[0]->i;
                        existList[countExistList].j = tmpList->change[0]->j;
                        countExistList++;
                    }
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
        if (listDest[l].x == X) {
            if (listDest[l].y == Y + 1)
                printf("Down -> ");
            else
                printf("Up -> ");
            Y = listDest[l].y;
        } else {
            if (listDest[l].x == X + 1)
                printf("Right -> ");
            else
                printf("Left -> ");
            X = listDest[l].x;
        }
    }
    printf("Dest");
}

int main() {
    importMap("Map1.txt");
    currentDeg = -1;
    deg[currentDeg + 1].count = 0;
    int done = BFS();
    if (done) {
        printf("Solve!\n\n");
        GotoDest();
        return 0;
    }
    int tmpN;
    currentDeg++;
    int ii, jj, xx, yy;
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
                printf("Solve!\n\n");
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
                        if (listPath[l].x == X) {
                            if (listPath[l].y == Y + 1)
                                printf("Down -> ");
                            else
                                printf("Up -> ");
                            Y = listPath[l].y;
                        } else {
                            if (listPath[l].x == X + 1)
                                printf("Right -> ");
                            else
                                printf("Left -> ");
                            X = listPath[l].x;
                        }
                    }
                    printf("Enter\n");
                }
                GotoDest();
                return 0;
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
    } while (deg[currentDeg].count);
    printf("Can't Solve!");
    return 0;
}
