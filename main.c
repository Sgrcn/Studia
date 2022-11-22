#include <stdio.h>
#include <stdlib.h>

//przyjmujac: 0-krol, 1-hetman, 2-wieza, 3-goniec, 4-skoczek,
//5-pionek, 6-krol_k, 7-hetman_k, 8-wieza_k, 9-goniec_k,
//10-skoczek_k, 11-pionek_k, 12-pole puste

int MAX_KIER[]={8,8,4,4,8,3,8,8,4,4,8,3,0};
int MAX_ODL[]={2,8,8,8,2,2,2,8,8,8,2,2,0};
int WX[12][8]={{0,1,1,1,0,-1,-1,-1},{0,1,1,1,0,-1,-1,-1},{0,1,0,-1},
               {1,1,-1,-1},{1,2,2,1,-1,-2,-2,-1},{-1,0,1},{0,1,1,1,0,-1,-1,-1},
               {0,1,1,1,0,-1,-1,-1},{0,1,0,-1},{1,1,-1,-1},{1,2,2,1,-1,-2,-2,-1},{-1,0,1} };

int WY[12][8]={{-1,-1,0,1,1,1,0,-1},{-1,-1,0,1,1,1,0,-1},{-1,0,1,0},{-1,1,1,-1},
               {-2,-1,1,2,2,1,-1,-2},{-1,-1,-1},{-1,-1,0,1,1,1,0,-1},{-1,-1,0,1,1,1,0,-1},
               {-1,0,1,0},{-1,1,1,-1},{-2,-1,1,2,2,1,-1,-2},{1,1,1} };

#define Wygrana 1000
#define Przegrana -1000
#define Puste 12

int ocena(char plansza[8][8])
{
    int w = 0;
    int oc[]={Przegrana, -9, -5, -4, -3, -1, Wygrana, 9, 5, 4, 3, 1, 0};
    for(int i = 0 ; i < 8 ; ++i)
    {
        for(int j = 0 ; j < 8 ; ++j)
        {
            w += oc[plansza[i][j]];
        }
    }
    return w;
}

int plansza[8][8] = {
        {8,10,9,7,6,9,10,8},
        {11,11,11,11,11,11,11,11},
        {12,12,12,12,12,12,12,12},
        {12,12,12,12,12,12,12,12},
        {12,12,12,12,12,12,12,12},
        {12,12,12,12,12,12,12,12},
        {5,5,5,5,5,5,5,5},
        {2,4,3,1,0,3,4,2}};

void wyswietl(int p[8][8])
{
    char fig[]="khwgspKHWGSP ";

    for (int y = 0 ; y < 17 ; ++y)
    {
        if(y%2==0)
        {
            printf("   +---+---+---+---+---+---+---+---+\n");
            continue;
        }

        printf(" %d |",8-(y/2));
        for (int x = 0 ; x < 8 ; ++x)
        {
            printf(" %c |", fig[plansza[y/2][x]]);
        }
        printf("\n");
    }
    printf("     A   B   C   D   E   F   G   H\n");
}

int minmax(int p[8][8], int glebokosc, int *x, int *y, int *k, int*o)
{
    int wynik, px_pom, py_pom, k_pom, o_pom, wmax, wmin;
    if(glebokosc == 0) return ocena(p);
    if(glebokosc%2 == 0)
    {
        wynik = 2*Przegrana;
        for(int py = 0 ; py < 8 ; ++py)
        {
            for(int px = 0 ; px < 8 ; ++px)
            {
                if(p[py][px] < 6 || !p[py][px] > 11) continue;
                int figura = p[py][px];
                for(int kierunek = 0 ; kierunek < MAX_KIER[figura] ; ++kierunek)
                {
                    for(int odleglosc = 0 ; odleglosc < MAX_ODL[figura] ; ++odleglosc)
                    {
                        int dx = px + (WX[figura][kierunek] * odleglosc);
                        int dy = py + (WY[figura][kierunek] * odleglosc);

                        if(dx < 0 || dx > 7 || dy < 0 || dy > 7)break;
                        if(p[dy][dx] >= 6 && p[dy][dx] <= 11) break;
                        if(!((p[dy][dx] == Puste && dx-px==0)||(p[dy][dx] != Puste && dx-px !=0)||p[dy][dx] != 11)) break;
                        int ruch_fig = plansza[py][px];
                        int bita_fig = plansza[dy][dx];
                        plansza[dy][dx] = plansza[py][px];
                        plansza[py][px] = Puste;
                        if (plansza[dy][dx]==11 && dy==7)plansza[dy][dx]=7;
                        wynik = minmax(p, glebokosc-1,&px_pom,&py_pom,&k_pom,&o_pom);
                        plansza[py][px] = ruch_fig;
                        plansza[dy][dx] = bita_fig;
                        if(wynik>=wmax){
                            wmax=wynik, *x=px, *y=py, *k=kierunek, *o=odleglosc;
                            printf("iteracja: %d %d %d %d\n",*x,*y,*k,*o);
                        }

                        if(p[dy][dx] != Puste) break;
                    }
                }
            }
        }
        return wmax;
    }
    else
    {
        wynik = 2*Wygrana;
        for(int py = 0 ; py < 8 ; ++py)
        {
            for(int px = 0 ; px < 8 ; ++px)
            {
                if(p[py][px] >= 6) continue;
                int figura = p[py][px];
                for(int kierunek = 0 ; kierunek < MAX_KIER[figura] ; ++kierunek)
                {
                    for(int odleglosc = 0 ; odleglosc < MAX_ODL[figura] ; ++odleglosc)
                    {
                        int dx = px + (WX[figura][kierunek] * odleglosc);
                        int dy = py + (WY[figura][kierunek] * odleglosc);

                        if(dx < 0 || dx > 7 || dy < 0 || dy > 7 ||
                                p[dy][dx] < 6 ||
                                !((p[dy][dx] == Puste && dx-px==0)||(p[dy][dx] != Puste && dx-px !=0)|| p[dy][dx] != 5)) break;

                        int ruch_fig = plansza[py][px];
                        int bita_fig = plansza[dy][dx];
                        plansza[dy][dx] = plansza[py][px];
                        plansza[py][px] = Puste;
                        if (plansza[dy][dx]==5 && dy==0)plansza[dy][dx]=1;
                        wynik = minmax(p, glebokosc-1,&px_pom,&py_pom,&k_pom,&o_pom);
                        plansza[py][px] = ruch_fig;
                        plansza[dy][dx] = bita_fig;
                        if(wynik<=wmin){
                            wmin=wynik, *x=px, *y=py, *k=kierunek, *o=odleglosc;
                            printf("iteracja: %d %d %d %d\n",x,y,k,o);
                        }

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
    int figura = p[sy][sx];
    float vy = ey - sy;
    float vx = ex - sx;
    for(int kierunek = 0 ; kierunek < MAX_KIER[figura] ; ++kierunek)
    {
        for (int odleglosc = 1; odleglosc < MAX_ODL[figura]; ++odleglosc)
        {
            if(vx / odleglosc == WX[figura][kierunek] && vy / odleglosc == WY[figura][kierunek]) return 1;
        }
    }
    return 0;
}

int main(void)
{
    int x, y, wyn, k, o, dx, dy, x2, y2;
    while(1)
    {
        minmax(plansza,4,&x,&y,&k,&o);

        printf("%d %d %d %d\n", x, y, k, o);
        //dx=o*WX[plansza[y][x]][k];
        //dy=o*WY[plansza[y][x]][k];
        //plansza[y+dy][x+dx] = plansza[y][x];
        //plansza[y][x] = Puste;
        //if(plansza[y+dy][x+dx]==11 && y+dy==7) plansza[x+dx][y+dy]=7;
        wyswietl(plansza);
        printf("podaj ruch");
        scanf("%d%d%d%d", &x, &y, &x2, &y2);
        plansza[y2][x2] = plansza[y][x];
        plansza[y][x] = Puste;
        if(plansza[x2][y2]==5&&y2==0)plansza[x2][y2]=1;
        wyswietl(plansza);
    }
    return 0;
}