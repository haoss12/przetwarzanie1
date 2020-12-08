#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX 1024        /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024   /* Dlugosc buforow pomocniczych */
#define KRAWEDZ 0       /* Krawedz obrazu */

int negatyw(int obraz_pgm [][MAX], int *wymx, int *wymy, int szarosci)
{
    int i, j;
        for (i=0; i<*wymy; ++i)
        {
            for (j=0; j<*wymx; ++j)
            {
            obraz_pgm [i][j] = szarosci - obraz_pgm [i][j]; //odbicie wartosci koloru piksela na przeciwna strone tabeli
            }
        }
    return 0;
}

int progowanie(int obraz_pgm [ ][MAX], int *wymx, int *wymy, int szarosci, int procprog) /*procprog - wprowadzony w menu przez uzytkownika procent przy ktorym ma zostac wykonane progowanie*/
{
    int i, j, wartprog;     /*wartprog - liczbowa wartosc progu dla ktorej nastepuje "zalamanie" sie czerni z biela*/
                            /*jest to wybrany przez uzytkownika procent z maksymalnej wartosci szarosci*/
        
        wartprog = ((procprog*szarosci)/100);
        
        printf ("%d", wartprog); //tymczasowa czesc funkcji do debbugowania, ma sprawdzac czy program odpowiednio wylicza wartosc

        for (i=0; i<wymy; ++i)
        {
            for (j=0; j<wymx; ++j)
            {
                if (obraz_pgm [i][j] <= wartprog) //progowanie "w dol", do czerni
                {
                    obraz_pgm [i][j] = 0;
                }
                
                if (obraz_pgm [i][j] > wartprog) //progowanie "w gore", do bieli
                {
                    obraz_pgm [i][j] = szarosci;
                }
            }
        }
    return (i*j);
}

void konturowanie(int obraz_pgm [ ][MAX], int *wymx, int *wymy, int szarosci)
{
    int i, j;

        for (i=0; i<*wymy; ++i)  
        {
            for (j=0; j<*wymx; ++j)
            {
                if (i==(*wymy-1))        /*wyjatek dla dolnych krawedzi obrazu*/
                {
                    if (j==(*wymx-1))    /*wyjatek dla rogu obrazu*/
                    {
                        obraz_pgm [i][j] = 0;
                    }
                    else
                    {
                        obraz_pgm [i][j] = (abs(obraz_pgm [i][j+1]-obraz_pgm [i][j])); /*wyjatek dla dolnej krawedzi obrazu*/
                    }
                    
                }
                else
                {
                    if (j==(*wymx-1))    /*wyjatek dla prawej krawedzi obrazu*/
                    {
                        obraz_pgm [i][j] = (abs(obraz_pgm [i+1][j]-obraz_pgm [i][j]));
                    }
                    else                /*standardowa sytuacja*/
                    {
                        obraz_pgm [i][j] = (abs(obraz_pgm [i+1][j]-obraz_pgm [i][j]) + abs(obraz_pgm [i][j+1]-obraz_pgm [i][j]));
                    }
                    /*konturowanie wedlug wzoru z pliku obrazy_filtry.pdf, abs (int) to wart. bezwzgledna z int*/
                } 
            }
        }
}

int rozmywanie(int obraz_pgm [ ][MAX], int *wymx, int *wymy, int szarosci, int prog, int tabela_temp [ ][MAX])
{
    int i, j;   /*rozmywanie jest wykonywane do tabeli tymczasowej          */
                /*po to by pobierane wartosci sie zgadzaly z oczekiwaniami  */

    if (obraz_pgm[0][0] == NULL) /*funkcja sprawdzająca czy wczytano plik*/
    {
        fprintf(stderr,"Blad: Nie wczytano pliku\n");
        return 0;
    }
    else
    {
        if (prog == 1) //pierwszy prog rozmywania
    {
        for (i=0; i<wymy; ++i)
        {
            for (j=0; j<wymx; ++j)
            {
                if (i==KRAWEDZ)     //wyjatek dla krawedzi obrazu
                {
                tabela_temp[i][j] = (obraz_pgm[i][j]+obraz_pgm[i+1][j])/2;
                }
                if (i==(*wymy-1))   //wyjatek dla krawedzi obrazu
                {
                tabela_temp[i][j] = (obraz_pgm[i-1][j]+obraz_pgm[i][j])/2;
                }
                else                //standardowa sytuacja
                {
                tabela_temp[i][j] = (obraz_pgm[i-1][j]+obraz_pgm[i][j]+obraz_pgm[i+1][j])/3;  
                }         
            }
        }

        return 0;
    }

    if (prog == 2)  //drugi prog rozmywania
    {
        for (i=0; i<wymy; ++i)
        {
            for (j=0; j<wymx; ++j)
            {   
                if (i==KRAWEDZ)             //wyjatek dla krawedzi obrazu
                {
                tabela_temp[i][j] = (obraz_pgm[i][j]+obraz_pgm[i+1][j]+obraz_pgm[i+2][j])/3;
                }
                if (i==(KRAWEDZ+1))         //wyjatek dla drugiego rzedu obrazu
                {
                tabela_temp[i][j] = (obraz_pgm[i-1][j]+obraz_pgm[i][j]+obraz_pgm[i+1][j]+obraz_pgm[i+2][j])/4;
                }
                if (i==(*wymy-2))           //wyjatek dla przedostatniego rzedu obrazu
                {
                tabela_temp[i][j] = (obraz_pgm[i-1][j]+obraz_pgm[i][j])/2;
                }
                if (i==(*wymy-1))           //wyjatek dla krawedzi obrazu
                {
                tabela_temp[i][j] = (obraz_pgm[i-1][j]+obraz_pgm[i][j])/2;
                }
                else                //standardowa sytuacja
                {
                tabela_temp[i][j] = (obraz_pgm[i-1][j]+obraz_pgm[i][j]+obraz_pgm[i+1][j])/3;  
                }         
            }
        }

        return 0;
    }
    }     

}

int zapisz(FILE *plik_wy, int obraz_pgm [ ][MAX], int wymx, int wymy, int szarosci)
{
    int i, j;

    if (plik_wy == NULL)
    {
        fprintf(stderr "Blad: Nie podano uchwytu do pliku\n");
        return(0);
    }
    
    fprintf(plik_wy, "P2\n");
    fprintf(plik_wy, "%d %d %d", wymx, wymy, szarosci);

    for (i=0; i<wymy; ++i){
        for (j=0; j<wymx; ++j){
            fprintf(plik_wy, "%d", &(obraz_pgm [i][j]));
        }
    }

    return 1;
}

void wyczysc()  /*funkcja czyszczaca terminal dla wiekszej czytelnosci menu*/ 
{
  char polecenie[DL_LINII];     /* bufor pomocniczy do zestawienia polecenia */
  strcpy(polecenie,"clear ");   /* konstrukcja polecenia postaci */
  system(polecenie);            /* wykonanie polecenia */
}

int menu() { //main chyba to bedzie

int wart;

do
{
    printf ("Menu: \n 1 - Wczytaj \n 2 - Zapisz \n 3 - Wyswietl \n 4 - Negatyw \n 5 - Progowanie \n 
    6 - Konturowanie \n 7 - Rozmycie \n 8 - Maski \n 9 - Wyjscie \n Twój wybor: ");
    scanf ("%d", &wart);
    
    switch (wart)
    {
    case 1:
        printf("Wybrałeś opcję %d. \n", wart);
        int czytaj(FILE *plik_we,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci);
        break;
    case 2:
        printf("Wybrałeś opcję %d. \n", wart);
        break;
    case 3:
        printf("Wybrałeś opcję %d. \n", wart);
        break;
    case 4:
        printf("Wybrałeś opcję %d. \n", wart);
        break;
    case 5:
        printf("Wybrałeś opcję %d. \n", wart);
        break;
    case 6:
        printf("Wybrałeś opcję %d. \n", wart);
        break;
    case 7:
        printf("Wybrałeś opcję %d. \n", wart);
        break;
    case 8:
        printf("Wybrałeś opcję %d. \n", wart);
        break;
    case 9:
        puts ("Koniec działania programu");
        return 0;
        break;

    default:
        puts ("Błędna wartość!");
        break;
    }

} while (wart != 9);



}