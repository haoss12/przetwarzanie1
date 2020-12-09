#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX 1024        /* Maksymalny rozmiar wczytywanego obrazu */
#define DL_LINII 1024   /* Dlugosc buforow pomocniczych */
#define KRAWEDZ 0       /* Krawedz obrazu */

/************************************************************************************
 * Funkcja wczytuje obraz PGM z pliku do tablicy       	       	       	       	    *
 *										    *
 * \param[in] plik_we uchwyt do pliku z obrazem w formacie PGM			    *
 * \param[out] obraz_pgm tablica, do ktorej zostanie zapisany obraz		    *
 * \param[out] wymx szerokosc obrazka						    *
 * \param[out] wymy wysokosc obrazka						    *
 * \param[out] szarosci liczba odcieni szarosci					    *
 * \return liczba wczytanych pikseli						    *
 ************************************************************************************/

int czytaj(FILE *plik_we,int obraz_pgm[][MAX],int *wymx,int *wymy, int *szarosci)
{
  char buf[DL_LINII];      /* bufor pomocniczy do czytania naglowka i komentarzy */
  int znak;                /* zmienna pomocnicza do czytania komentarzy */
  int koniec=0;            /* czy napotkano koniec danych w pliku */
  int i,j;

  /*Sprawdzenie czy podano prawidłowy uchwyt pliku */
  if (plik_we==NULL) {
    fprintf(stderr,"Blad: Nie podano uchwytu do pliku\n");
    return(0);
  }

  /* Sprawdzenie "numeru magicznego" - powinien być P2 */
  if (fgets(buf,DL_LINII,plik_we)==NULL)   /* Wczytanie pierwszej linii pliku do bufora */
    koniec=1;                              /* Nie udalo sie? Koniec danych! */

  if ( (buf[0]!='P') || (buf[1]!='2') || koniec) {  /* Czy jest magiczne "P2"? */
    fprintf(stderr,"Blad: To nie jest plik PGM\n");
    return(0);
  }

  /* Pominiecie komentarzy */
  do {
    if ((znak=fgetc(plik_we))=='#') {         /* Czy linia rozpoczyna sie od znaku '#'? */
      if (fgets(buf,DL_LINII,plik_we)==NULL)  /* Przeczytaj ja do bufora                */
	koniec=1;                   /* Zapamietaj ewentualny koniec danych */
    }  else {
      ungetc(znak,plik_we);                   /* Gdy przeczytany znak z poczatku linii */
    }                                         /* nie jest '#' zwroc go                 */
  } while (znak=='#' && !koniec);   /* Powtarzaj dopoki sa linie komentarza */
                                    /* i nie nastapil koniec danych         */

  /* Pobranie wymiarow obrazu i liczby odcieni szarosci */
  if (fscanf(plik_we,"%d %d %d",wymx,wymy,szarosci)!=3) {
    fprintf(stderr,"Blad: Brak wymiarow obrazu lub liczby stopni szarosci\n");
    return(0);
  }
  /* Pobranie obrazu i zapisanie w tablicy obraz_pgm*/
  for (i=0;i<*wymy;i++) {
    for (j=0;j<*wymx;j++) {
      if (fscanf(plik_we,"%d",&(obraz_pgm[i][j]))!=1) {
	fprintf(stderr,"Blad: Niewlasciwe wymiary obrazu\n");
	return(0);
      }
    }
  }
  return *wymx**wymy;   /* Czytanie zakonczone sukcesem    */
}                       /* Zwroc liczbe wczytanych pikseli */

/*Funckja zapisujaca plik pod wybrana przez uzytkownika nazwa*/
int zapisz(FILE *plik_wy, int obraz_pgm [][MAX], int *wymx, int *wymy, int *szarosci)
{
    int i, j;

    if (plik_wy == NULL)
    {
        fprintf(stderr "Blad: Nie podano uchwytu do pliku\n");
        return(0);
    }
    
    fprintf(plik_wy, "P2\n");
    fprintf(plik_wy, "%d %d %d", *wymx, *wymy, *szarosci);
    fprintf(plik_wy, "\n");
    for (i=0; i<*wymy; ++i){
        for (j=0; j<*wymx; ++j){
            fprintf(plik_wy, "%d ", &(obraz_pgm [i][j]));
        }
        fprintf(plik_wy, "\n");
    }

    return 0;
}

/* Wyswietlenie obrazu o zadanej nazwie za pomoca programu "display"   */
void wyswietl(char *n_pliku)
{
  char polecenie[DL_LINII];      /* bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"display ");  /* konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);     /* display "nazwa_pliku" &       */
  strcat(polecenie," &");
  printf("%s\n",polecenie);      /* wydruk kontrolny polecenia */
  system(polecenie);             /* wykonanie polecenia        */
}

/*Funkcja czyszczaca terminal dla wiekszej czytelnosci menu*/
void wyczysc()
{
  char polecenie[DL_LINII];     /*bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"clear ");   /*konstrukcja polecenia postaci */
  system(polecenie);            /*wykonanie polecenia           */
}

/*Funkjca usuwajaca plik tymczasowy stworzony na potrzeby wyswietlania*/
void usun(char *n_pliku)
{    
  char polecenie[DL_LINII];     /*bufor pomocniczy do zestawienia polecenia */

  strcpy(polecenie,"rm ");      /*konstrukcja polecenia postaci */
  strcat(polecenie,n_pliku);    /* rm "nazwa_pliku" &       */
  strcat(polecenie," &");
  system(polecenie);            /*wykonanie polecenia           */

}
/*Funkcja wykonujaca negatyw na wczytanym pliku*/
void negatyw(int obraz_pgm [][MAX], int *wymx, int *wymy, int szarosci)
{
    int i, j;
        for (i=0; i<*wymy; ++i)
        {
            for (j=0; j<*wymx; ++j)
            {
            obraz_pgm [i][j] = szarosci - obraz_pgm [i][j]; //odbicie wartosci koloru piksela na przeciwna strone tabeli
            }
        }
}

/*Funkcja wykonujaca progowanie na wczytanym pliku dla zadanego przez uzytkownika procentu*/
void progowanie(int obraz_pgm [][MAX], int *wymx, int *wymy, int szarosci, int procprog) 
{                           /*procprog - wprowadzony w menu przez uzytkownika procent przy ktorym ma zostac wykonane progowanie*/
    int i, j, wartprog;     /*wartprog - liczbowa wartosc progu dla ktorej nastepuje "zalamanie" sie czerni z biela*/
                            /*jest to wybrany przez uzytkownika procent z maksymalnej wartosci szarosci*/
        wartprog = ((procprog*szarosci)/100);

        for (i=0; i<*wymy; ++i)
        {
            for (j=0; j<*wymx; ++j)
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
}

/*Funkcja wykonujaca konturowanie na wczytanym pliku*/
void konturowanie(int obraz_pgm [][MAX], int *wymx, int *wymy, int szarosci)
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

                        if (obraz_pgm [i][j] > szarosci)    /*moze wystapic sytuacja w ktorej powyzszy wzor odda wart. wieksza niz  */
                        {                                   /*maksymalna mozliwa, ten if stanowi zabezpieczenie przed czyms takim   */
                            obraz_pgm [i][j] = szarosci; 
                        }
                        
                    }
                    /*konturowanie wedlug wzoru z pliku obrazy_filtry.pdf, abs (int) to wart. bezwzgledna z int*/
                } 
            }
        }
}

/*Funkcja wykonujaca rozmywanie pionowe na wczytanym pliku*/
void rozmywanie_pion(int obraz_pgm [][MAX], int *wymx, int *wymy, int prog, int tabela_temp [][MAX])
{
    int i, j;   /*rozmywanie jest wykonywane do tabeli tymczasowej (tabela_temp)*/
                /*po to by pobierane wartosci sie zgadzaly z oczekiwaniami      */
                /*gdyby nie ten zabieg funkcja pobieralaby wartosci juz rozmyte */
                /*dla ktorych w dalszej czesci obrazu moglyby pojawiac sie bledy*/

    if (prog == 1) //pierwszy prog rozmywania
    {
        for (i=0; i<wymy; ++i)
        {
            for (j=0; j<wymx; ++j)
            {
                if (i==KRAWEDZ)     //wyjatek dla gornej krawedzi obrazu, by funkcja nie pobierala czegos co nie istnieje
                {
                tabela_temp[i][j] = (obraz_pgm[i][j]+obraz_pgm[i+1][j])/2;
                }
                if (i==(*wymy-1))   //wyjatek dla dolnej krawedzi obrazu, jw.
                {
                tabela_temp[i][j] = (obraz_pgm[i-1][j]+obraz_pgm[i][j])/2;
                }
                else                //standardowa sytuacja
                {
                tabela_temp[i][j] = (obraz_pgm[i-1][j]+obraz_pgm[i][j]+obraz_pgm[i+1][j])/3;  
                }         
            }
        }
    }

    if (prog == 2)  //drugi prog rozmywania
    {
        for (i=0; i<wymy; ++i)
        {
            for (j=0; j<wymx; ++j)
            {   
                if (i==KRAWEDZ)     //wyjatek dla gornej krawedzi obrazu, jw.
                {
                tabela_temp[i][j] = (obraz_pgm[i][j]+obraz_pgm[i+1][j]+obraz_pgm[i+2][j])/3;
                }
                if (i==(KRAWEDZ+1)) //wyjatek dla drugiego wiersza obrazu, jw.
                {
                tabela_temp[i][j] = (obraz_pgm[i-1][j]+obraz_pgm[i][j]+obraz_pgm[i+1][j]+obraz_pgm[i+2][j])/4;
                }
                if (i==(*wymy-2))   //wyjatek dla przedostatniego wiersza obrazu, jw.
                {
                tabela_temp[i][j] = (obraz_pgm[i-2][j]+obraz_pgm[i-1][j]+obraz_pgm[i][j]+obraz_pgm[i+1][j])/4;
                }
                if (i==(*wymy-1))   //wyjatek dla dolnej krawedzi obrazu, jw.
                {
                tabela_temp[i][j] = (obraz_pgm[i-2][j]+obraz_pgm[i-1][j]+obraz_pgm[i][j])/3;
                }
                else                //standardowa sytuacja
                {
                tabela_temp[i][j] = (obraz_pgm[i-2][j]+obraz_pgm[i-1][j]+obraz_pgm[i][j]+obraz_pgm[i+1][j]+obraz_pgm[i+2][j])/5;  
                }         
            }
        }
    }

    for (i=0; i<*wymy; ++i)
    {
        for (j=0; j<*wymx; ++j)                 /*czesc funkcji odpowiedzialna za przepisanie              */
        {                                       /*wartosci z tabeli tymczasowej spowrotem do glownej tabeli*/
        obraz_pgm [i][j] = tabela_temp[i][j];   /*oraz wyczyszczenie tabeli tymczasowej                    */
        tabela_temp[i][j] = 0;
        }
    }

}

/*Funkcja wykonujaca rozmywanie poziome na wczytanym pliku*/
void rozmywanie_poziom(int obraz_pgm [][MAX], int *wymx, int *wymy, int prog, int tabela_temp [][MAX])
{
    int i, j;   /*rozmywanie jest wykonywane do tabeli tymczasowej (tabela_temp)*/
                /*po to by pobierane wartosci sie zgadzaly z oczekiwaniami      */
                /*gdyby nie ten zabieg funkcja pobieralaby wartosci juz rozmyte */
                /*dla ktorych w dalszej czesci obrazu moglyby pojawiac sie bledy*/

        if (prog == 1) //pierwszy prog rozmywania
    {
        for (i=0; i<wymy; ++i)
        {
            for (j=0; j<wymx; ++j)
            {
                if (j==KRAWEDZ)     //wyjatek dla gornej krawedzi obrazu, by funkcja nie pobierala czegos co nie istnieje
                {
                tabela_temp[i][j] = (obraz_pgm[i][j]+obraz_pgm[i][j+1])/2;
                }
                if (j==(*wymx-1))   //wyjatek dla dolnej krawedzi obrazu, jw.
                {
                tabela_temp[i][j] = (obraz_pgm[i][j-1]+obraz_pgm[i][j])/2;
                }
                else                //standardowa sytuacja
                {
                tabela_temp[i][j] = (obraz_pgm[i][j-1]+obraz_pgm[i][j]+obraz_pgm[i][j+1])/3;  
                }         
            }
        }
    }

    if (prog == 2)  //drugi prog rozmywania
    {
        for (i=0; i<wymy; ++i)
        {
            for (j=0; j<wymx; ++j)
            {   
                if (j==KRAWEDZ)     //wyjatek dla gornej krawedzi obrazu, jw.
                {
                tabela_temp[i][j] = (obraz_pgm[i][j]+obraz_pgm[i][j+1]+obraz_pgm[i][j+2])/3;
                }
                if (j==(KRAWEDZ+1)) //wyjatek dla drugiego wiersza obrazu, jw.
                {
                tabela_temp[i][j] = (obraz_pgm[i][j-1]+obraz_pgm[i][j]+obraz_pgm[i][j+1]+obraz_pgm[i][j+2])/4;
                }
                if (j==(*wymx-2))   //wyjatek dla przedostatniego wiersza obrazu, jw.
                {
                tabela_temp[i][j] = (obraz_pgm[i][j-2]+obraz_pgm[i][j-1]+obraz_pgm[i][j]+obraz_pgm[i][j]+1)/4;
                }
                if (j==(*wymx-1))   //wyjatek dla dolnej krawedzi obrazu, jw.
                {
                tabela_temp[i][j] = (obraz_pgm[i][j-2]+obraz_pgm[i][j-1]+obraz_pgm[i][j])/3;
                }
                else                //standardowa sytuacja
                {
                tabela_temp[i][j] = (obraz_pgm[i][j-2]+obraz_pgm[i][j-1]+obraz_pgm[i][j]+obraz_pgm[i][j]+1+obraz_pgm[i][j+2])/5;  
                }         
            }
        }
    }

    for (i=0; i<*wymy; ++i)
    {
        for (j=0; j<*wymx; ++j)                 /*czesc funkcji odpowiedzialna za przepisanie              */
        {                                       /*wartosci z tabeli tymczasowej spowrotem do glownej tabeli*/
        obraz_pgm [i][j] = tabela_temp[i][j];   /*oraz wyczyszczenie tabeli tymczasowej                    */
        tabela_temp[i][j] = 0;
        }
    }
}

/*Funkcja sprawdzająca czy wczytano plik*/

int main() {
  int obraz[MAX][MAX];
  int tabelatemp[MAX][MAX]; /*tabela tymczasowa opisana w funkcji rozmywania*/
  int wymx, wymy, szarosci, wart, procprog; /*wart - zmienna dla menu uzytkownika*/
  int odczytano = 0;                        /*procprog - opisane w funkcji progowania*/
  FILE *plik;
  char nazwa[100];

  /*Aby program mogl pozwolic uzytkownikowi dalsza prace nalezy wczytac plik*/
  printf("Musisz wczytac plik aby rozpoczac prace programu\n");
  /* Wczytanie zawartosci wskazanego pliku do pamieci */
  printf("Podaj nazwe pliku:\n");
  scanf("%s",nazwa);
  plik=fopen(nazwa,"r");

  if (plik != NULL) {       /* co spowoduje zakomentowanie tego warunku */
    odczytano = czytaj(plik,obraz,&wymx,&wymy,&szarosci);
    fclose(plik);
  }

  /* Wyswietlenie poprawnie wczytanego obraza zewnetrznym programem */
  if (odczytano != 0)
    wyswietl(nazwa);

  return odczytano;
}