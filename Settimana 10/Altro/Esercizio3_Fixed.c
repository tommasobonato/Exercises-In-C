/*
    * Versione 2: corretto gli errori riguardanti la liberazione della memoria con la free, mentre il il valore di ritorno della malloc
                  o realloc viene controllato o con la funzione checkAllocation quando possibile o attraverso degli if

    * Autore: Tommaso Bonato
    * Numero matricola: 0000723575
    * E-Mail: tommaso.bonato@studio.unibo.it

    * Testo esercizio: La segreteria del nostro Corso di Studi ci chiede di sviluppare un programma per la gestione degli studenti.
      Di ogni studente si vogliono mantenere tutte le informazioni anagrafiche e quelle relative al curriculum. Il programma
      deve permettere di eseguire le principali operazioni di gestione dell�archivio, quali l�aggiunta di un nuovo studente, la
      cancellazione di uno studente, la visualizzazione di tutti gli studenti. Il programma deve inoltre permettere la
      ricerca di uno studente e la conseguente stampa di tutti i suoi voti e la ricerca degli studenti che ancora non
      hanno sostenuto uno specifico esame.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

// Definisco delle condizioni al pre-processore per permettere il funzionamento della funzione pausa sia su Windows sia sugli altri OS
#ifdef _WIN32
#define CLEAR "cls"
#else // Negli altri OS
#define CLEAR "clear"
#endif

#define MAX 100 // Definisco il numero massimo di caratteri per il nome, cognome e indirizzo
#define N_EXAMS 3
#define TRUE 1
#define FALSE 0

typedef struct { // Definisco una prima struttura per la gestione della data di tipo DATE_T
    int day;
    int month;
    int year;
} DATE_T;

typedef struct { // Definisico la struttura per la gestione dei dati dello studente di tipo STUDENT_T
    int code;
    char *name;
    char *surname;
    char *address;
    DATE_T date;
    int *exams;
} STUDENT_T;

STUDENT_T *student; // Dichiaro un puntatore student alla struttura STUDENT_T
char courses[][15] = {"Programmazione", "Inglese", "Analisi"}; // Questa matrice contiene le materie che si vogliono utilizzare
int myIndex = 0; // Contatore che conta il numero degli studenti

// Definisco i prototipi delle funzioni
int checkValue(int value, int check, int min, int max);
void clearBuffer();
void checkAllocation(STUDENT_T *student);
void menu();
void add();
void printDatabase();
void cancel();
void search();
void searchSubject();
void exitmenu();

int main()
{
    int i;

    student = (STUDENT_T*) malloc(sizeof(STUDENT_T)); // Alloco la struttura di tipo STUDENT_T e la collego ad un puntatore
    checkAllocation(student); // Controllo che l'allocazione sia andata a buon fine attraverso una funzione

    menu(); // Chiamo la funzione menu


    /* Libero la memoria allocata coi puntatori usati per le stringhe (e mi liberano anche i corrispondenti str della funzione readString
       in quanto hanno lo stesso indirizzo di memoria) */
    for (i = 0; i < myIndex; i++) {
        free(student[i].name);
        free(student[i].surname);
        free(student[i].address);
        free(student[i].exams);
    }
    // Non serve che libero la memoria allocata con il puntatore str in quanto viene gi� liberata precedentemente dato che gli indirizzi
    // di memoria delle str vengono fatti corrispondere a quelli di student[i].name, student[i].surname ecc..
    free(student); // Libero lo spazio in memoria



    system("pause"); // Compatibile solo con Windows
    return 0;
}

void menu()
{
    int menu, check, i;

    system(CLEAR);

    printf("\t\t ---- MENU GESTIONE SEGRETERIA ---- \n\n");
    printf("1 - Inserisci un nuovo studente\n");
    printf("2 - Stampa il registro\n");
    printf("3 - Elimina uno studente\n");
    printf("4 - Ricerca uno studente\n");
    printf("5 - Ricerca studenti per esami da sostenere\n");
    printf("6 - Esci dal programma\n\n");
    printf("Scegli un opzione dal menu: ");
    check = scanf("%d", &menu);
    menu = checkValue(menu, check, 1, 6);

    switch (menu) {
    case 1 :
        add();
        break;
    case 2 :
        printDatabase();
        break;
    case 3 :
        cancel();
        break;
    case 4 :
        search();
        break;
    case 5 :
        searchSubject();
        break;
    case 6 :
        for (i = 0; i < myIndex; i++) { // Libero prima lo spazio in memoria pi� interno
            free(student[i].name);
            free(student[i].surname);
            free(student[i].address);
            free(student[i].exams);
        }
        free(student); // Libero lo spazio in memoria
        exit(EXIT_SUCCESS);
        break;
    default :
        printf("Errore, scelta non valida");
        exit(EXIT_FAILURE);
    }

    return;
}

void clearBuffer() // Definisco una funzione per pulire il buffer da tastiera che � completamente portabile
{
    char ch;
    do
    {
        ch = getchar();
    } while (('\n' != ch) && (EOF != ch));
}

/*
 * Funzione checkValue
 * --------------------
 * Controlla che il numero inserito dall'utente sia valido.
 * Prende in ingresso quattro valori: il numero su cui eseguire il controllo, il valore di ritorno della scanf e i due estremi (inferiore e superiore) su cui fare il controllo
 * Restituisce il valore (un intero) dopo che il controllo � risultato valido
*/
int checkValue(int value, int check, int min, int max)
{

    clearBuffer();
    while (value < min || value > max || check != 1) {
        printf("Errore, devi inserire un valore valido compreso tra %d e %d: ", min, max);
        check = scanf("%d", &value);
        clearBuffer();
    }

    return value;
}

void checkAllocation(STUDENT_T *student) // Funzione che controlla che l'allocazione di memoria sia andata a buon fine.
{
    if (student == NULL) {
        printf("Errore durante l'allocazione dinamica della memoria");
        exit(EXIT_FAILURE);
    }

    return;
}

char* readString(char *str) // Questa funzione si occupa di leggere una stringa inserita da tastiera e restituisce un puntatore a carattere
{
    char c;
    int j;

    str = (char*) malloc(sizeof(char));
    if (str == NULL) {
            printf("Errore durante l'allocazione dinamica della memoria");
            exit(EXIT_FAILURE);
        }

    for (j = 0; j < MAX && (c = getchar()) != '\n'; j++) {
        str = (char*)realloc(str, (j+1) * sizeof(char));
        // Libero la memoria per alla fine del main usando lo stesso indirizzo di memoria anche se con diversi nomi (student[i].name ecc..)
        if (str == NULL) {
            printf("Errore durante l'allocazione dinamica della memoria");
            exit(EXIT_FAILURE);
        }
        str[j] = c;
    }
    str[j] = '\0'; // Aggiungo il terminatore di stinga alla fine

    return str;
}

void exitmenu() // Questa funzione chiede all'utente se vuole tornare al menu o se vuole uscire dal programma
{
    int option, check;

    printf("\n\nPremi 0 per tornare al main o 1 per uscire: ");
    check = scanf("%d", &option);
    option = checkValue(option, check, 0, 1);

    if (option == 0)
        menu();
    else if (option == 1) {
        free(student);
        exit(EXIT_SUCCESS);
    }
}

void add() // Questa funzione permette di aggiungere uno studente al registro
{

    int check, j;
    int i = myIndex;

    system(CLEAR);

    student = (STUDENT_T*) realloc(student, (myIndex+1) * sizeof(STUDENT_T)); // Alloco lo spazio per un nuovo studente
        if (student == NULL) {
            printf("Errore durante l'allocazione della memoria");
            exit(EXIT_FAILURE);
        }

    printf("\t\t ---- MENU GESTIONE SEGRETERIA ---- \n\n");
    printf("Inserisci il numero di matricola: ");
    check = scanf("%d", &student[i].code);
    student[i].code = checkValue(student[i].code, check, 0, INT_MAX);
    for (j = 0; j < myIndex; j++) // Controllo che il numero di matricola non sia gi� presente nel database
        if (student[i].code == student[j].code) {
            printf("Il numero di matricola inserito e\' gia presente nel database");
            exitmenu();
        }

    printf("Inserisci il nome: ");
    student[i].name = readString(student[i].name);

    printf("Inserisci il cognome: ");
    student[i].surname = readString(student[i].surname);

    printf("Inserisci l'indirizzo: ");
    student[i].address = readString(student[i].address);
    printf("Inserisci la data di nascita: \n");
    printf("\tGiorno: ");
    check = scanf("%d", &student[i].date.day);
    student[i].date.day = checkValue(student[i].date.day, 1, 1, 31);
    printf("\tMese: ");
    check = scanf("%d", &student[i].date.month);
    student[i].date.month = checkValue(student[i].date.month, 1, 1, 12);
    printf("\tAnno: ");
    check = scanf("%d", &student[i].date.year);
    student[i].date.year = checkValue(student[i].date.year, 1, 1, 2014);

    printf("Inserisci il codice degli esami svolti.\nInserisci -1 se l'esame non e\' stato sostenuto.\n");
    student[i].exams = (int*)malloc(N_EXAMS * sizeof(int)); // Alloco lo spazio per il numero di esami
    if (student[i].exams == NULL) {
        printf("Errore durante l'allocazione dinamica della memoria");
        exit(EXIT_FAILURE);
    }
    for (j = 0; j <= 2; j++) {
        printf("Voto di %s: ", courses[j]);
        check = scanf("%d", &student[i].exams[j]);
        student[i].exams[j] = checkValue(student[i].exams[j], check, -1, 30);
    }

    myIndex++; // Incremento il contatore degli studenti

    exitmenu();

    return;

}

void printDatabase() // Questa funzione stampa il contenuto del database
{
    int i, j;

    system(CLEAR);

    printf("\t\t ---- VISUALIZZA CONTENUTO DATABASE ---- \n\n");

    if (myIndex == 0) // Controllo che ci sia almeno uno studente nel database
        printf("Non ci sono elementi da visualizzare nel database");
    else {
    for (i = 0; i < myIndex; i++) {
            printf("Numero matricola: %d\n", student[i].code);
            printf("\tNome e Cognome: %s %s\n", student[i].name, student[i].surname);
            printf("\tIndirizzo: %s\n", student[i].address);
            printf("\tData di nascita: %d/%d/%d\n", student[i].date.day, student[i].date.month, student[i].date.year);
            for (j = 0; j <= 2; j++) {
                if (student[i].exams[j] != -1)
                    printf("\tVoto di %s: %d\n", courses[j], student[i].exams[j]);
                else
                    printf("\tEsame di %s non sostenuto\n", courses[j]);
            }

        }

    }

    exitmenu();

    return;

}

void cancel() // Funzione che permette di cancellare uno studente dal databse
{
    int i, cancelCode, check, trovato = FALSE;

    system(CLEAR);

    printf("\t\t ---- CANCELLA UNO STUDENTE DAL DATABASE ---- \n\n");
    printf("Inserire il numero di matricola dello studente da eliminare: ");
    check = scanf("%d", &cancelCode);
    cancelCode = checkValue(cancelCode, check, 0, INT_MAX);

    for (i = 0; i < myIndex; i++)
        if (cancelCode == student[i].code) { // Se la matricola inserita � presente nel database
            trovato = TRUE;
            /* Libero la memoria allocata coi puntatori usati per le stringhe (e mi liberano anche i corrispondenti str della funzione readString
                   in quanto hanno lo stesso indirizzo di memoria) */
                free(student[i].name);
                free(student[i].surname);
                free(student[i].address);
                free(student[i].exams);
            student[i] = student[myIndex-1]; // Copio l'ultimo studente del registro nella posizione di quello che si desidera eliminare
            if (myIndex > 1) { // Controllo che il numero di studenti sia maggiore di uno in quanto altrimenti eliminerei tutto lo spazio con la funzione realloc
                student = (STUDENT_T*) realloc(student, (myIndex - 1) * sizeof(STUDENT_T)); // Riduco di un unit� la dimensione dello spazio allocato
                checkAllocation(student); // Controllo il valore di ritorno della realloc

            }
            myIndex--; // Diminuisco il contatore del numero di studenti
        printf("Studente cancellato con successo!");
        }
    if (trovato != TRUE) // Se non � stato trovato nessuno studente stampo un avviso
        printf("Codice matricola non trovato nel database");

    exitmenu();

    return;
}

void search() // La funzione cerca permette di cercare uno studente e visualizzare i suoi voti
{
    int i, check, code, j, trovato = FALSE;

    system(CLEAR);
    printf("\t ---- RICERCA VOTI DI UNO STUDENTE DAL DATABASE ---- \n\n");
    printf("Inserire il numero di matricola dello studente da ricercare: ");
    check = scanf("%d", &code);
    code = checkValue(code, check, 0, INT_MAX);

    for (i = 0; i < myIndex && trovato != TRUE; i++)
        if (code == student[i].code) {
            trovato = TRUE; // Se lo studente � stato trovato, salvo nella variabile trovato il valore "TRUE"
            printf("Matricola: %d (%s %s)", student[i].code, student[i].name, student[i].surname);
            printf("\nVoti: ");
            for (j = 0; j <= 2; j++) {
                if (student[i].exams[j] != -1) // Se il voto dell'esame � diverso da -1 (non sostenuto), stampo il voto
                    printf("\t%s: %d\n", courses[j], student[i].exams[j]);
                else // Altrimenti dico che non � stato sostenuto
                    printf("\t%s non sostenuto\n", courses[j]);
            }
        }
    if (trovato != TRUE) // Se non � stato trovato nessuno studente stampo un avviso
        printf("Codice matricola non trovato nel database");

    exitmenu();

    return;
}

void searchSubject() // Questa funzione stampa tutti gli studenti che non hanno sostenuto un esame
{
    int i, check, code, trovato = FALSE;

    system(CLEAR);
    printf("\t ---- RICERCA STUDENTI PER ESAMI DA SOSTENERE ---- \n\n");
    printf("Legenda: Programmazione = 0, Inglese = 1 e Analisi = 2\n");
    printf("Inserire il codice dell\'esame : ");
    check = scanf("%d", &code);
    code = checkValue(code, check, 0, 2);

    printf("\nStudenti che devono ancora svolgere l'esame di %s\n", courses[code]);
    for (i = 0; i < myIndex; i++) {
        if (student[i].exams[code] == -1) { // Se lo studente non ha sostenuto l'esame il suo voto � -1
            trovato = TRUE;
            printf("\t%s %s (Matricola numero: %d)\n", student[i].name, student[i].surname, student[i].code);
        }
    }
    if (trovato == FALSE && myIndex > 0) // Se non � stato trovato neussuno studente significa che tutti hanno gi� svolto quell'esame
        printf("Tutti gli studenti hanno gia\' svolto l'esame di %s", courses[code]);

    exitmenu();


    return;
}

