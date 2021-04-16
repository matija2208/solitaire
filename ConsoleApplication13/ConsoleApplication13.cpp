#include "podaci.h"

using namespace solitaire;

int brojacUkoloni[7] = { 0,0,0,0,0,0,0 };

void punjenje_spila(karta spil[52])
{
    srand(time(NULL));
    for (int i = 0; i < 52; i++)
    {
    ponovo:
        spil[i].br = rand() % 13;
        spil[i].zn = rand() % 4;
        for (int j = 0; j < i; j++)
        {
            if (spil[i].br == spil[j].br && spil[i].zn == spil[j].zn)
            {
                spil[i].br = 0;
                spil[i].zn = 0;
                goto ponovo;
            }
        }
    }
}

int punjenje_talona(karta spil[52], karta talon[52][7])
{
    int counter = 52;
    for (int i = 0; i < 7; i++)
    {
        for (int j = i; j < 7; j++)
        {
            talon[i][j].br = spil[0].br;
            talon[i][j].zn = spil[0].zn;
            brojacUkoloni[j]++;
            if (j != i)
            {
                talon[i][j].ot = false;
            }
            counter--;
            for (int z = 0; z < counter; z++)
            {
                spil[z] = spil[z + 1];
            }
            
        }
    }
    return counter;
}

void stampanje_talona(karta talon[52][7])
{
    int pozicije[7] = { 60,71,82,93,104,115,126 };
    for (int i = 0; i < 7; i++)
    {
        int pozicija_i = 2;
        for (int j = 0; j < 52; j++)
        {
            if (talon[j][i].br >= 0 && talon[j][i].br <= 12)
            {
                for (int y = pozicija_i; y < pozicija_i + 6; y++)
                {
                    for (int x = pozicije[i]; x < pozicije[i] + 8; x++)
                    {
                        if (talon[j][i].ot == true)
                            i_tabla[y][x] = karte[talon[j][i].zn][talon[j][i].br][(y - pozicija_i)][(x - pozicije[i])];
                        else
                            i_tabla[y][x] = prazna_karta[(y - pozicija_i)][(x - pozicije[i])];
                    }
                }
                pozicija_i += 3;
            }
        }
    }
}

void draw(karta spil[52], int counter)
{
    karta t = spil[0];
    for (int i = 0; i < counter - 1; i++)
    {
        spil[i] = spil[i + 1];
    }
    spil[counter - 1] = t;
}

void ispis_spila(karta spil[52], int counter)
{
    
    for (int y = 0; y < 6; y++)
    {
        for (int x = 0; x < 8; x++)
        {
            if (counter != 0)
                i_tabla[(y + 2)][(x + 201)] = karte[spil[0].zn][spil[0].br][y][x];
            else
                i_tabla[(y + 2)][(x + 201)] = prazna_karta[y][x];
        }
    }
}

void ispis_odlaganja(karta odlaganje[4])
{
    int pozicije[4] = { 1,7,13,19 };
    for (int i = 0; i < 4; i++)
    {
        for (int y = 0; y < 6; y++)
        {
            for (int x = 0; x < 8; x++)
            {
                if (odlaganje[i].br == -1)
                {
                    i_tabla[(y + pozicije[i])][(x + 2)] = prazna_karta[y][x];
                }
                else
                {
                    i_tabla[(y + pozicije[i])][(x + 2)] = karte[odlaganje[i].zn][odlaganje[i].br][y][x];
                }
            }
        }
    }
}

bool stack(int col, karta talon[52][7], karta odlaganje[4], karta spil[52], int *counter)
{
    if (col == 8)
    {
        if (odlaganje[spil[0].zn].br + 1 == spil[0].br)
        {
            odlaganje[spil[0].zn] = spil[0];
            *counter -= 1;
            for (int i = 0; i < *counter; i++)
            {
                spil[i] = spil[i + 1];
            }
        }
    }
    else if(col>=1&&col<=7)
    {
        col -= 1;
        karta* pokazivac = &talon[(brojacUkoloni[col] - 1)][col];

        if ((odlaganje[(*pokazivac).zn].br + 1) == (*pokazivac).br)
        {
            odlaganje[(*pokazivac).zn] = (*pokazivac);
            talon[(brojacUkoloni[col] - 1)][col].br = (-1);
            talon[(brojacUkoloni[col] - 1)][col].zn = (-1);
            talon[(brojacUkoloni[col] - 1)][col].ot = true;
            brojacUkoloni[col]-=1;
            if(brojacUkoloni[col]>0)
                talon[(brojacUkoloni[col] - 1)][col].ot = true;
        }
    }
    bool t = true;
    for (int i = 0; i < 4; i++)
    {
        if (odlaganje[i].br != 12)
        {
            t = false;
            break;
        }
    }
    return t;
}

void unstack(int col[2], karta talon[52][7], karta odlaganje[4])
{
    col[0] -= 1;
    col[1] -= 1;
    
    if ((talon[brojacUkoloni[col[1]] - 1][col[1]].br == (odlaganje[col[0]].br + 1)) && ((talon[brojacUkoloni[col[1]] - 1][col[1]].zn + odlaganje[col[0]].zn) >= 2) && ((talon[brojacUkoloni[col[1]] - 1][col[1]].zn + odlaganje[col[0]].zn) <= 4))
    {
        brojacUkoloni[col[1]] += 1;
        talon[(brojacUkoloni[col[1]]-1)][col[1]] = odlaganje[col[0]];
        odlaganje[col[0]].br -= 1;
    }
}

void move(karta spil[52], int *counter, karta talon[52][7], int col[2], int num)
{
    col[0] -= 1;
    col[1] -= 1;
    if (col[1] >= 0 && col[1] <= 6)
    {
        if (col[0] == 7)
        {
            if (((talon[brojacUkoloni[col[1]]-1][col[1]].br == (spil[0].br + 1)) && (talon[brojacUkoloni[col[1]]-1][col[1]].zn + spil[0].zn >= 2) && (talon[brojacUkoloni[col[1]]-1][col[1]].zn + spil[0].zn <= 4)) || ((talon[brojacUkoloni[col[1]]][col[1]].br == -1) && (spil[0].br == 12) && brojacUkoloni[col[1]]==0))
            {
                talon[brojacUkoloni[col[1]]][col[1]] = spil[0];
                *counter -= 1;
                brojacUkoloni[col[1]] += 1;
                for (int i = 0; i < *counter; i++)
                {
                    spil[i] = spil[i + 1];
                }
            }
        }
        else if (col[0] >= 0 && col[1] <= 6)
        {
            int poz = 0;
            for (poz = 0; poz < 52; poz++)
            {
                if (talon[poz][col[0]].ot == true)
                {
                    poz -= 1;
                    break;
                }
            }
            int i=poz;
            for (i = poz; i < 52; i++)
            {
                if (talon[i][col[0]].br == num)
                {
                    break;
                }
            }
            
            if (((talon[brojacUkoloni[col[1]] - 1][col[1]].br == (talon[i][col[0]].br + 1)) && (talon[brojacUkoloni[col[1]] - 1][col[1]].zn + talon[i][col[0]].zn >= 2) && (talon[brojacUkoloni[col[1]] - 1][col[1]].zn + talon[i][col[0]].zn <= 4)) || ((talon[brojacUkoloni[col[1]]][col[1]].br == -1) && (talon[i][col[0]].br == 12) && brojacUkoloni[col[1]] == 0))
            {
                for (int j = i; j < 52; j++)
                {
                    if (talon[j][col[0]].br == -1)
                    {
                        break;
                    }
                    talon[brojacUkoloni[col[1]]][col[1]] = talon[j][col[0]];
                    talon[j][col[0]].br = -1;
                    talon[j][col[0]].zn = -1;
                    if(j>0)
                        talon[j-1][col[0]].ot = true;
                    brojacUkoloni[col[0]] -= 1;
                    brojacUkoloni[col[1]] += 1;
                }
            }
        }
    }
}
int map(string num)
{
    if (num[0] == 'a')return 0;
    else if (num[0] == '2')return 1;
    else if (num[0] == '3')return 2;
    else if (num[0] == '4')return 3;
    else if (num[0] == '5')return 4;
    else if (num[0] == '6')return 5;
    else if (num[0] == '7')return 6;
    else if (num[0] == '8')return 7;
    else if (num[0] == '9')return 8;
    else if (num[0] == '1')return 9;
    else if (num[0] == 'j')return 10;
    else if (num[0] == 'q')return 11;
    else if (num[0] == 'k')return 12;
}

int main()
{
    karta spil[52];
    karta talon[52][7];
    karta odlaganje[4];
    punjenje_spila(spil);
    int counter= punjenje_talona(spil, talon);
ponovo:
    inicijalizacija_ispisa();
    stampanje_talona(talon);
    ispis_spila(spil, counter);
    ispis_odlaganja(odlaganje);
    ispis();
    string komanda;
    string num;
    int col[2];
    cin >> komanda;

    if (komanda == "stack")
    {
        cin >> col[0];
        bool t = stack(col[0], talon, odlaganje, spil, &counter);
        if (t)
        {
            system("cls");
            cout << "YOU WON!!!\n";
            goto kraj;
        }
    }

    else if (komanda == "unstack")
    {
        cin >> col[0];
        cin >> col[1];
        unstack(col, talon, odlaganje);
    }

    else if (komanda == "move")
    {
        cin >> num;
        cin >> col[0];
        cin >> col[1];
        move(spil, &counter, talon, col, map(num));
    }

    else if(komanda == "draw")
    {
        draw(spil,counter);
    }

    goto ponovo;
kraj:
    return 0;
}