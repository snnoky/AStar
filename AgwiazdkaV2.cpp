#include <iostream>
#include <fstream>

using namespace std;

int wym2 = 20;
int wym1 = 20;
int startX = 0;
int startY = 0;
int koniecX = 19;
int koniecY = 19;
int aktualnyX = koniecX;
int aktualnyY = koniecY;
bool drogaZnaleziona = false;
bool koniecSzukania = false;
bool drogaMozliwa = true;
double kosztRuchu = 1;
int najmniejszyKosztRuchuX = startX;
int najmniejszyKosztRuchuY = startY;
int listaOtwarta[20][20] = { 0 }; // zawiera rozwa¿ane pola
int listaZamknieta[20][20] = { 0 };
int listaPomocnicza[20][20] = { 0 }; //pola ktore mozna odwiedzic
double listaKosztow[20][20] = { 0 };

double ObliczH(int pozX, int pozY); //Przewidywany ruch od pola (pozX,pozY) do koñca (koniecX, koniecY)( Heurystyka )
void znajdzDroge(); // algorytm wyszuknia drogi
void znajdzRodzica(int x, int y); // Dopisanie rodzica do pola
void znajdzMozliwePola(int x, int y); // Szuka mozliwe pola wkolo polo xy
void ustalNajmniejszyKosztRuchu(); // Ustala min (najmniejszy koszt ruchu z wybranych pol)

int main()
{
    string nazwap = "grid.txt";

    cout << "Wczytywanie danych z pliku\n";

    //teraz deklarujemy dynamicznie tablice do, której wczytamyu nasz plik,
    int rows = wym2 + 1;
    double** G;
    G = new double* [rows];
    while (rows--) G[rows] = new double[wym1 + 1];

    cout << "\n\nNacisnij ENTER aby wczytac tablice o nazwie " << nazwap;
    getchar();

    std::ifstream plik(nazwap.c_str());

    for (unsigned int i = 1; i < wym2 + 1; i++)
    {
        for (unsigned int j = 1; j < wym1 + 1; j++)
        {
            plik >> G[i][j];
            listaZamknieta[i - 1][j - 1] = G[i][j];
        }
    }
    plik.close();

    cout << "\nWypisujemy na ekran\n\n";
    for (int i = 1; i < wym2 + 1; i++)
    {
        for (int j = 1; j < wym1 + 1; j++)
        {
            cout << " " << G[i][j];
        }cout << "\n";
    }


    znajdzDroge();

    if (!drogaMozliwa)
    {
        cout << "\nBrak mozliwych drog!\n";
    }
    else
    {
        cout << "\n Wyznaczona trasa wyglada nastepujaco\n\n\n";
        for (int i = 0; i < wym2; i++)
        {
            for (int j = 0; j < wym1; j++)
            {
                cout << " " << listaZamknieta[i][j];
            }
            cout << "\n";
        }
    }
    //na koniec czyœcimy pamiêæ po naszej tablicy
    for (int i = 0; i < wym2 + 1; i++)
    {
        delete[] G[i];
    }//czyscimy wiersze
    delete[] G;//zwalniamy tablice wskaznikow do wierszy

    cout << "\n\nNacisnij ENTER aby zakonczyc";
    getchar();

    return 0;
}

double ObliczH(int pozX, int pozY)
{
    return sqrt(pow((pozX - koniecX), 2) + pow((pozY - koniecY), 2)); //metryka Euklidesa
}

void znajdzDroge()
{
    if (listaZamknieta[startX][startY] == 5)
    {
        drogaMozliwa = false;
        cout << "\nPunkt startowy nie moze byc sciana!\n";
    }
    while (!koniecSzukania && drogaMozliwa) //szuka mozliwe pola i wybiera najmniejszy koszt
    {
        znajdzMozliwePola(najmniejszyKosztRuchuX, najmniejszyKosztRuchuY);
        ustalNajmniejszyKosztRuchu();
        kosztRuchu++;
    }
    while (!drogaZnaleziona && drogaMozliwa)
    {
        znajdzRodzica(aktualnyX, aktualnyY);
        if (aktualnyX == startX && aktualnyY == startY)
        {
            listaZamknieta[aktualnyX][aktualnyY] = 3;
            drogaZnaleziona = true;
        }
    }

}

void znajdzRodzica(int x, int y)
{
    listaZamknieta[x][y] = 3;
    switch (listaOtwarta[x][y])
    {
    case 1:
    {
        aktualnyX = x + 1; //Jezeli pole x-1 czyli gorne to rodzicem jest pole x+1
        break;
    }
    case 2:
    {
        aktualnyX = x - 1; //Jezeli pole x+1 czyli dolne to rodzicem jest pole x-1
        break;
    }
    case 3:
    {
        aktualnyY = y + 1; //Jezeli pole y-1 czyli lewe to rodzicem jest pole y+1
        break;
    }
    case 4:
    {
        aktualnyY = y - 1; //Jezeli pole y+1 czyli prawe to rodzicem jest pole y-1
        break;
    }
    }
}
void znajdzMozliwePola(int x, int y)
{
    listaPomocnicza[x][y] = 1;
    if (x == koniecX && y == koniecY) // Je¿eli x i y s¹ polami koñcowymi zakoñcz
    {
        koniecSzukania = true;
    }
    if (x > -1 && x < 20 && y > -1 && y < 20)
    {
        int gornePole = x - 1;
        int dolnePole = x + 1;
        int lewePole = y - 1; 
        int prawePole = y + 1;

        if (gornePole > -1 && listaPomocnicza[gornePole][y] == 0 && listaKosztow[gornePole][y] == 0) // wiêksze ni¿-1 zeby nie wyjsc za obszar
        {
            listaKosztow[gornePole][y] = kosztRuchu + ObliczH(gornePole, y);
            listaOtwarta[gornePole][y] = 1;
            if (listaZamknieta[gornePole][y] == 5)
            {
                listaKosztow[gornePole][y] = -1;
            }
        }
    	
        if (dolnePole < 20 && listaPomocnicza[dolnePole][y] == 0 && listaKosztow[dolnePole][y] == 0) // Jezeli nie ma kosztu tzn nie by³o sprawdzane mniejsze od 20 zeby nie wychodzic poza obszar
        {
            listaKosztow[dolnePole][y] = kosztRuchu + ObliczH(dolnePole, y);
            listaOtwarta[dolnePole][y] = 2;
            if (listaZamknieta[dolnePole][y] == 5) // jezeli dane pole jest sciana zamien koszt na -1 ( nie uwzgledniaj potem w ustalaniu najmniejszego kosztu )
            {
                listaKosztow[dolnePole][y] = -1;
            }
        }

        if (lewePole > -1 && listaPomocnicza[x][lewePole] == 0 && listaKosztow[x][lewePole] == 0)
        {
            listaKosztow[x][lewePole] = kosztRuchu + ObliczH(x, lewePole);
            listaOtwarta[x][lewePole] = 3;
            if (listaZamknieta[x][lewePole] == 5)
            {
                listaKosztow[x][lewePole] = -1;
            }
        }
    	
        if (prawePole < 20 && listaPomocnicza[x][prawePole] == 0 && listaKosztow[x][prawePole] == 0)
        {
            listaKosztow[x][prawePole] = kosztRuchu + ObliczH(x, prawePole);
            listaOtwarta[x][prawePole] = 4;
            if (listaZamknieta[x][prawePole] == 5)
            {
                listaKosztow[x][prawePole] = -1;
            }
        }
        
    }
}
void ustalNajmniejszyKosztRuchu()
{
    double min = 10000;
    
    if (koniecX > startX)
    {
        for (int i = 0; i < wym2; i++)
        {
            for (int j = 0; j < wym1; j++)
            {
                if (listaPomocnicza[i][j] == 0 && listaKosztow[i][j] > 0 && listaKosztow[i][j] < min) // koszt -1 to sciana a 0 to pole nie ustalone/ brak pola(najpierw lista kosztow wypelniona jest 0) dlatego wieksze od 0
                {

                    min = listaKosztow[i][j];
                    najmniejszyKosztRuchuX = i;
                    najmniejszyKosztRuchuY = j;

                }
            }
        }
    }
    else
    {
        for (int i = 0; i < wym2; i++)
        {
            for (int j = 0; j < wym1; j++)
            {
                if (listaPomocnicza[i][j] == 0 && listaKosztow[i][j] > 0 && listaKosztow[i][j] <= min) // koszt -1 to sciana a 0 to pole nie ustalone/ brak pola(najpierw lista kosztow wypelniona jest 0) dlatego wieksze od 0
                {

                    min = listaKosztow[i][j];
                    najmniejszyKosztRuchuX = i;
                    najmniejszyKosztRuchuY = j;

                }
            }
        }
    }
    
	
    

    if (min == 10000)
    {
        drogaMozliwa = false;
    }
    if (najmniejszyKosztRuchuX == koniecX && najmniejszyKosztRuchuY == koniecY)
    {
        drogaMozliwa = true;
    }
}