#include <stdio.h>
#include "raylib.h"
#include <math.h>

//przyjmujac: 0-krol, 1-hetman, 2-wieza, 3-goniec, 4-skoczek,
//5-pionek, 6-krol_k, 7-hetman_k, 8-wieza_k, 9-goniec_k,
//10-skoczek_k, 11-pionek_k, 12-pole puste

int MAX_KIER[]={8,8,4,4,8,4,8,8,4,4,8,4,0};
int MAX_ODL[]={2,8,8,8,2,2,2,8,8,8,2,2,0};
int WX[12][8]={{0,1,1,1,0,-1,-1,-1},{0,1,1,1,0,-1,-1,-1},{0,1,0,-1},
               {1,1,-1,-1},{1,2,2,1,-1,-2,-2,-1},{-1,0,1, 0},{0,1,1,1,0,-1,-1,-1},
               {0,1,1,1,0,-1,-1,-1},{0,1,0,-1},{1,1,-1,-1},{1,2,2,1,-1,-2,-2,-1},{-1,0,1, 0} };

int WY[12][8]={{-1,-1,0,1,1,1,0,-1},{-1,-1,0,1,1,1,0,-1},{-1,0,1,0},{-1,1,1,-1},
               {-2,-1,1,2,2,1,-1,-2},{-1,-1,-1, -2},{-1,-1,0,1,1,1,0,-1},{-1,-1,0,1,1,1,0,-1},
               {-1,0,1,0},{-1,1,1,-1},{-2,-1,1,2,2,1,-1,-2},{1,1,1, 2} };

#define Wygrana 1000
#define Przegrana -1000
#define Puste 12

int ocena(int plansza[8][8])
{
    int w = 0;
    int oc[13]={Przegrana, -9, -5, -4, -3, -1, Wygrana, 9, 5, 4, 3, 1, 0};
    for(int i = 0 ; i < 8 ; ++i)
    {
        for(int j = 0 ; j < 8 ; ++j)
        {
            w += oc[plansza[i][j]];
        }
    }
    return w;
}

int plansza[8][8] =
        {
                {8,10,9,7,6,9,10,8},
                {11,11,11,11,11,11,11,11},
                {12,12,12,12,12,12,12,12},
                {12,12,12,12,12,12,12,12},
                {12,12,12,12,12,12,12,12},
                {12,12,12,12,12,12,12,12},
                {5,5,5,5,5,5,5,5},
                {2,4,3,1,0,3,4,2}};

int minmax(int p[8][8], int glebokosc, int *x, int *y, int *k, int *o, int alpha, int beta, bool swapped)
{
    int wynik, px_pom, py_pom, k_pom, o_pom, wmax, wmin;
    wynik = ocena(p);
    if(glebokosc == 0 || 2*wynik>1000 || 2*wynik<-1000) return wynik;

    if((glebokosc%2 == 0 && !swapped) || (glebokosc%2 == 1 && swapped))
    {
        wmax = 2*Przegrana;
        for(int py = 0 ; py < 8 ; ++py)
        {
            for(int px = 0 ; px < 8 ; ++px)
            {
                if(p[py][px] < 6 || !p[py][px] > 11) continue;
                int figura = p[py][px];
                for(int kierunek = 0 ; kierunek < MAX_KIER[figura] ; ++kierunek)
                {
                    for(int odleglosc = 1 ; odleglosc < MAX_ODL[figura] ; ++odleglosc)
                    {
                        int dx = px + (WX[figura][kierunek] * odleglosc);
                        int dy = py + (WY[figura][kierunek] * odleglosc);

                        if(dx < 0 || dx > 7 || dy < 0 || dy > 7 ||
                           (p[dy][dx] >= 6 && p[dy][dx] <= 11)) break;
                        if (((p[py][px] == 11) && ((dx==px&&p[dy][dx]!=Puste)||(dx!=px&&p[dy][dx]==Puste))) ||
                           ((p[py][px] == 11)&&(dy-py==2)&&((p[dy][dx]!=Puste)||(py!=1))))continue;

                        int ruch_fig = p[py][px];
                        int bita_fig = p[dy][dx];
                        if(bita_fig == 0)
                        {
                            wmax=2*Wygrana, *x=px, *y=py, *k=kierunek, *o=odleglosc;
                            return 2*Wygrana;
                        }
                        p[dy][dx] = p[py][px];
                        p[py][px] = Puste;
                        if (p[dy][dx]==11 && dy==7)p[dy][dx]=7;
                        wynik = minmax(p, glebokosc-1,&px_pom,&py_pom,&k_pom,&o_pom, alpha, beta, swapped);
                        p[py][px] = ruch_fig;
                        p[dy][dx] = bita_fig;
                        if(wynik>=wmax){
                            wmax=wynik, *x=px, *y=py, *k=kierunek, *o=odleglosc;
                        }
                        if(wynik > alpha) alpha = wynik;
                        if(beta <= alpha) return wmax;
                        if(p[dy][dx] != Puste) break;
                    }
                }
            }
        }
        return wmax;
    }
    else
    {
        wmin = 2*Wygrana;
        for(int py = 0 ; py < 8 ; ++py)
        {
            for(int px = 0 ; px < 8 ; ++px)
            {
                if(p[py][px] >= 6) continue;
                int figura = p[py][px];
                for(int kierunek = 0 ; kierunek < MAX_KIER[figura] ; ++kierunek)
                {
                    for(int odleglosc = 1 ; odleglosc < MAX_ODL[figura] ; ++odleglosc)
                    {
                        int dx = px + (WX[figura][kierunek] * odleglosc);
                        int dy = py + (WY[figura][kierunek] * odleglosc);

                        if(dx < 0 || dx > 7 || dy < 0 || dy > 7 ||
                           p[dy][dx] < 6) break;
                        if (((p[py][px] == 5) && ((dx==px&&p[dy][dx]!=Puste)||(dx!=px&&p[dy][dx]==Puste))) ||
                           ((p[py][px] == 5)&&(dy-py==-2)&&((p[dy][dx]!=Puste)||(py!=6)))) continue;

                        int ruch_fig = p[py][px];
                        int bita_fig = p[dy][dx];
                        if(bita_fig == 6)
                        {
                            wmin=2*Przegrana, *x=px, *y=py, *k=kierunek, *o=odleglosc;
                            return 2*Przegrana;
                        }
                        p[dy][dx] = p[py][px];
                        p[py][px] = Puste;
                        if (p[dy][dx]==5 && dy==0)p[dy][dx]=1;
                        wynik = minmax(p, glebokosc-1,&px_pom,&py_pom,&k_pom,&o_pom,alpha,beta, swapped);
                        p[py][px] = ruch_fig;
                        p[dy][dx] = bita_fig;
                        if(wynik<=wmin){
                            wmin=wynik, *x=px, *y=py, *k=kierunek, *o=odleglosc;
                        }
                        if(wynik < beta) beta = wynik;
                        if(beta <= alpha) return wmin;
                        if(p[dy][dx] != Puste) break;
                    }
                }
            }
        }
        return wmin;
    }
}

int SprawdzRuch(int p[8][8], int sx, int sy, int ex, int ey)
{
    for(int py = 0 ; py < 8 ; ++py) {
        for (int px = 0; px < 8; ++px) {
            if (p[py][px] >= 6) continue;
            int figura = p[py][px];
            for (int kierunek = 0; kierunek < MAX_KIER[figura]; ++kierunek) {
                for (int odleglosc = 1; odleglosc < MAX_ODL[figura]; ++odleglosc) {
                    int dx = px + (WX[figura][kierunek] * odleglosc);
                    int dy = py + (WY[figura][kierunek] * odleglosc);

                    if (dx < 0 || dx > 7 || dy < 0 || dy > 7 ||
                        p[dy][dx] < 6) break;
                    if(((p[py][px] == 5) && ((dx == px && p[dy][dx] != Puste) || (dx != px && p[dy][dx] == Puste))) ||
                        ((p[py][px] == 5) && (dy - py == -2) && ((p[dy][dx] != Puste) || (py != 6)))) continue;
                    if(sx == px && sy == py && ex == dx && ey == dy)return 1;
                    if(p[dy][dx] != Puste) break;
                }
            }
        }
    }
    return 0;
}

int screenWidth = 750;
int screenHeight = 750;

Texture2D boardTexture;
Texture2D chessPiecesTextures[12];

Vector2 boardPos;
Vector2 chessPiecePos;

void Init(void)
{
    boardPos.x = 0;
    boardPos.y = 0;

    boardTexture = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\board.png");
    chessPiecesTextures[3] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-bishop-black.png");
    chessPiecesTextures[9] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-bishop-white.png");
    chessPiecesTextures[0] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-king-black.png");
    chessPiecesTextures[6] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-king-white.png");
    chessPiecesTextures[4] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-knight-black.png");
    chessPiecesTextures[10] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-knight-white.png");
    chessPiecesTextures[5]= LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-pawn-black.png");
    chessPiecesTextures[11] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-pawn-white.png");
    chessPiecesTextures[1] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-queen-black.png");
    chessPiecesTextures[7] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-queen-white.png");
    chessPiecesTextures[2] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-rook-black.png");
    chessPiecesTextures[8] = LoadTexture("C:\\Users\\Aleksander\\CLionProjects\\untitled1\\sprites\\chess-rook-white.png");
}

void RenderBoard(void)
{
    BeginDrawing();
    ClearBackground(RAYWHITE);

    DrawTextureEx(boardTexture, boardPos, 0, 0.366, RAYWHITE);
    for(int j = 0 ; j < 8 ; ++j)
    {
        for(int i = 0 ; i < 8 ; ++i)
        {
            chessPiecePos.x = -10 + i * 92;
            chessPiecePos.y = -10 + j * 92;
            DrawTextureEx(chessPiecesTextures[plansza[j][i]],chessPiecePos,0,0.25,RAYWHITE);
        }
    }

    EndDrawing();
}

int main(void)
{
    int x, y, k, o, dx, dy, x2, y2, wynMat;
    char fX, fY, sX, sY;
    bool fCc;

    InitWindow(screenWidth, screenHeight, "Chess");
    SetTargetFPS(60);

    Init();

    while (!WindowShouldClose())
    {
        RenderBoard();
        int wynik = minmax(plansza,6,&x,&y,&k,&o, 10*Przegrana, 10*Wygrana, false);
        printf("wynik : %d\n", wynik);
        dx=o*WX[plansza[y][x]][k];
        dy=o*WY[plansza[y][x]][k];
        plansza[y+dy][x+dx] = plansza[y][x];
        plansza[y][x] = Puste;
        if(plansza[y+dy][x+dx]==11 && y+dy==7) plansza[y+dy][x+dx]=7;

        wynMat = minmax(plansza,2,&x,&y,&k,&o, 10*Przegrana, 10*Wygrana, true);
        if(wynMat >= Wygrana)
        {
            printf("Przegrales\n");
            break;
        }

        fCc = false;
        while(1) {
            RenderBoard();

            if(WindowShouldClose())
            {
                CloseWindow();

                return 0;
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                if (!fCc) {
                    x = floor(GetMouseX() / 94);
                    y = floor(GetMouseY() / 94);
                    fCc = true;
                } else {
                    x2 = floor(GetMouseX() / 94);
                    y2 = floor(GetMouseY() / 94);
                    fCc = false;
                    if(SprawdzRuch(plansza, x, y, x2, y2)) break;
                }
            }
        }
        plansza[y2][x2] = plansza[y][x];
        plansza[y][x] = Puste;
        if(plansza[y2][x2]==5&&y2==0)plansza[y2][x2]=1;
        wynMat = minmax(plansza,2,&x,&y,&k,&o, 10*Przegrana, 10*Wygrana, false);
        if(wynMat <= Przegrana)
        {
            printf("Wygrales\n");
            break;
        }
    }

    CloseWindow();

    return 0;
}