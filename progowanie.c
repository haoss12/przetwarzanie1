#define MAX 1024

int progowanie(int obraz_pgm [ ][MAX], int wymx, int wymy, int szarosci, int procprog) //procprog - wprowadzony w menu przez uzytkownika procent przy ktorym ma zostac wykonane progowanie
{
    int i, j;       /*wartprog - liczbowa wartosc progu dla ktorej nastepuje "zalamanie" sie czerni z biela*/
    float wartprog; /*jest to wybrany przez uzytkownika procent z maksymalnej wartosci szarosci*/
        
        wartprog = (0.01*procprog*szarosci);
        
        printf ("%d", wartprog); //tymczasowa czesc funkcji do debbugowania, ma sprawdzac czy program odpowiednio wylicza wartosc

        for (i=0; i<wymy; ++i)
        {
            for (j=0; j<wymx; ++j)
            {
                if ((float) obraz_pgm [i][j] <= wartprog) //progowanie "w dol", do czerni
                {
                    obraz_pgm [i][j] = 0; /* powinienem chyba jednak dac (float) *obraz_pgm... */
                }
                
                if ((float) obraz_pgm [i][j] > wartprog) //progowanie "w gore", do bieli
                {
                    obraz_pgm [i][j] = szarosci;
                }
            }
        }
}