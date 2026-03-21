#include <stdio.h>

void drukuj(int tablica[], int liczba_elementow){
    for (int i = 0; i < liczba_elementow; ++i){
        if (tablica[i] > 10 && tablica[i] < 100)
            printf("%d ", tablica[i]);
    }
}

int main(int argc, char const *argv[])
{
    int liczby[50];

    int liczba = -1;
    int index = 0;

    while (liczba != 0 && index < 50)
    {

        scanf("%d", &liczba);

        if (liczba != 0){
            liczby[index] = liczba;
            ++index;
        }
        
        //printf("Indeks: %d, liczba: %d", index, liczba);
    }

    drukuj(liczby, index);
    

    return 0;
}

/*
funkcja z pojednynczym wskaznikiem - tekst
funckja ze wskaźnikiem i liczbą bajtów - ciąg bajtów
*/
