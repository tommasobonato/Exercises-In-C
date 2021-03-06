/*
    * Autore: Tommaso Bonato
    * Numero matricola: 0000723575
    * E-Mail: tommaso.bonato@studio.unibo.it

    * Testo esercizio: Realizzare un programma per il calcolo del fattoriale in modo da visualizzare le sue variabili locali
      e i parametri delle chiamate ricorsive. Per ogni chiamata ricorsiva, visualizzare l'output su una riga separata
      e aggiungere un livello di rientro. Fate del vostro meglio per rendere l'output chiaro, interessante e significativo.
      L'obiettivo � quello di disegnare e implementare un formato dell'output che aiuti l'utente a capire la ricorsione.

*/


#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define CLEAR_BUFFER do { c = getchar(); } while (c != '\n' && c != EOF); // Dichiaro una macro per pulire il buffer

int livello = 0; // Dichiaro una variabile globale per contare il livello di indentazione

unsigned long fattoriale(int n); // Prototipo della funzione che utilizzerò

// Questa funzione calcola il fattoriale di un numero n (parametro) e restituisce il risultato
unsigned long fattoriale(int n) // Uso un unsigned long int in quanto il fattoriale può assumere valori molto alti
 {
    int i;
    unsigned long fatt;

    if (n == 0) // Se n � uguale a 0 stampo restituisco direttamente 1 che � il suo fattoriale
        return 1;
    else {
        for (i = 0; i < livello; i++) // Il ciclo for tiene conto del livello di indentazione
            printf("   ");
        if (n > 1) // Se n � maggiore di uno stampo una certa stringa
        printf("%d non e\' uguale a 0 e quindi chiama il fattoriale di (%d)\n",n,n-1);
        else if (n == 1) {
            printf("%d non e\' uguale a 0 e quindi chiama il fattoriale di (%d)\n",n,n-1);
            livello++; // Aumento di uno il livello di indentazione
            for (i = 0; i < livello; i++)
                printf("   ");
            printf("%d e\' uguale a 0 e quindi ritorna il suo valore\n", n-1);
        }
        livello++; // Sposto in avanti l'indentazione
        fatt = n * fattoriale(n-1); // Calcolo il fattoriale richiamando la funzione stessa "fattoriale"
        for (i = 0; i < livello; i++)
            printf("   ");
        printf("%d * fattoriale di %d restituisce %lu\n",n, n-1,fatt);
        livello--; // Sposto indietro l'indentazione
        return fatt;
    }
 }

int main()
{
    int n, ris, c;
    unsigned long y;

    // Richiedo all'utente un numero e controllo che sia valido
    printf("Inserisci un numero: ");
    ris = scanf("%d", &n);
    CLEAR_BUFFER
    while (n < 0 || n > INT_MAX || ris != 1) {
        printf("Errore, devi inserire un numero compreso tra 0 e %d: ", INT_MAX);
        ris = scanf("%d", &n);
        CLEAR_BUFFER
    }
    y = fattoriale(n); // Chiamo la funzione fattoriale passando come argomento il numero n

    printf("\n\nIl fattoriale di %d e\' quindi: %lu", n, y); // Stampo il risultato

    system("pause"); // Compatibile solo con Windows

    return 0;
}
