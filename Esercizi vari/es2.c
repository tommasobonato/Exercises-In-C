#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#define MAX 101
#define N_ESAMI 6

// Definisco delle condizioni al pre-processore per permettere il funzionamento della funzione pausa sia su Windows sia sugli altri OS
#ifdef _WIN32
#define CLEAR "cls"
#else // Negli altri OS
#define CLEAR "clear"
#endif

typedef struct StudentS{
    int code;
    char name[MAX];
    char surname[MAX];
    char address[MAX];
    int voti[N_ESAMI];
    int esami [N_ESAMI];
    struct StudentS *next;
} TypeStudent;

typedef struct YearS {
    int year;
    int code; // 0 se c'� il numero programmato, 1 se non c'�
    int maxStudents; // Se c'� il numero programmato, questa variabile indica il numero massimo di studenti
    int nStudents; // Numero di studenti iscritti
    TypeStudent *student;
    struct YearS *next;
} TypeYear;

void insertYear (TypeYear **head);
void insertStudent (TypeYear *head);
void cancel(TypeYear *head);
void printList(TypeYear *head);
int checkValue(int value, int check, int min, int max);
void clearBuffer();
int isEmpty(TypeYear *head);
void todo(TypeYear *head);
void searchHighest (TypeYear *head);
void searchStud(TypeYear *head);
void freeMemory(TypeYear *head);

int main()
{
    int MenuChoice, check;
    TypeYear *head = NULL;

    while (MenuChoice != 9) {
        system(CLEAR);
        printf("\t\t ---- GESTIONE ANNI ACCADEMICI ----\n\n");
        printf("1 - Inserisci un nuovo anno accademico\n");
        printf("2 - Inserisci un nuovo studente\n");
        printf("3 - Visualizza l\'archivio\n");
        printf("4 - Elimina uno studente\n");
        printf("5 - Cerca uno studente\n");
        printf("6 - Cerca gli studenti che non hanno ancora svolto un esame\n");
        printf("7 - Cerca l\'anno con la media piu\' alta\n");
        printf("8 - Esci dal programma\n\n");
        printf("Scegli un opzione dal menu: ");
        check = scanf("%d", &MenuChoice);
        MenuChoice = checkValue(MenuChoice, check, 1, 8);

        switch(MenuChoice) {
            case 1 :
                insertYear(&head);
                break;
            case 2 :
                insertStudent(head);
                break;
            case 3 :
                printList(head);
                break;
            case 4 :
                cancel(head);
                break;
            case 5 :
                searchStud(head);
                break;
            case 6 :
                todo(head);
                break;
            case 7 :
                searchHighest(head);
                break;
            case 8 :
                freeMemory(head);
                free(head);
                exit(EXIT_SUCCESS);
                break;
            default :
                printf("Errore, scelta non valida");
                freeMemory(head);
                free(head);
                exit(EXIT_FAILURE);
        }

    }

    freeMemory(head);
    free(head);

    return 0;
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

void clearBuffer() // Definisco una funzione per pulire il buffer da tastiera che � completamente portabile
{
    char ch;
    do
    {
        ch = getchar();
    } while (('\n' != ch) && (EOF != ch));
}

int isEmpty(TypeYear *head) // Controllo se la lista principale � vuota
{
    if (head == NULL)
        return 1;
    return 0;
}

void freeMemory(TypeYear *head)
{
    TypeYear *temp, *prev;
    TypeStudent *tempS, *prevS;

    temp = head;

    while(temp != NULL) {
        tempS = temp->student;
        while(tempS != NULL) {
            prevS = tempS;
            tempS = tempS->next;
            free(prevS);
        }
        prev = temp;
        temp = temp->next;
        free(prev);
    }
}

void insertYear (TypeYear **head)
{
    TypeYear *newYear, *temp, *prev;
    int check;

    temp = *head;
    system(CLEAR);


    newYear = (TypeYear*) malloc(sizeof(TypeYear));
    if (newYear == NULL) {
        printf("Errore durante l\'allocazione dinamica della memoria");
        exit(EXIT_FAILURE);
    }

    printf("\t\t\t ---- INSERISCI UN ANNO ACCADEMICO ----\n\n");
    printf("Inserisci l\'anno: ");
    check = scanf("%d", &newYear->year);
    newYear->year = checkValue(newYear->year, check, 1088, INT_MAX);

    printf("Inserisci 0 se e\' presente il numero programmato o 1 se non e\' presente: ");
    check = scanf("%d", &newYear->code);
    newYear->code = checkValue(newYear->code, check, 0, 1);

    if (newYear->code == 0) {
        printf("Inserisci il numero massimo di studenti: ");
        check = scanf("%d", &newYear->maxStudents);
        newYear->maxStudents = checkValue(newYear->maxStudents, check, 0, 2000);
    }

    prev = NULL;
    if (isEmpty(*head)) {
        newYear->next = *head;
        *head = newYear;
    } else {
        while (temp != NULL && newYear->year > temp->year) { // Scorro la lista fino alla fine o fino a quando incontro un anno maggiore
            prev = temp;
            temp = temp->next;
        }
        if (temp != NULL && newYear->year == temp->year) {
            printf("Errore, anno %d gia\' presente", temp->year);
            printf("\nPremere invio per continuare...");
            free(newYear);
            getchar();
            return;
        }
        if (newYear->year < (*head)->year) { // Se l'elemento da inserire � in testa
            newYear->next = *head;
            *head = newYear;

        } else { // Altrimenti se l'inserimento sta avvenendo in mezzo alla lista
            newYear->next = temp;
            prev->next = newYear;
        }
    }

    newYear->nStudents = 0; // Inizializzo il numero di studenti a 0
    newYear->student = NULL; // Inizializzo a NULL il puntatori a student

    return;
}

void insertStudent (TypeYear *head)
{
    TypeYear *temp, *tempY;
    TypeStudent *newStudent, *tempS;
    int check, length, year, found = 0, i;
    char nomiesami[][50] = {"Programmazione", "Inglese", "Analisi", "Algebra", "Architettura degli elaboratori", "Algoritmi"};

    temp = tempY = head;
    system(CLEAR);

    printf("\t\t\t ---- INSERISCI UNO STUDENTE ----\n\n");
    newStudent = (TypeStudent*) malloc(sizeof(TypeStudent));
    if (newStudent == NULL) {
        printf("Errore durante l\'allocazione dinamica della memoria");
        exit(EXIT_FAILURE);
    }

    if (isEmpty(head)) { // Se la lista principale � vuota stampo un errore
        printf("Errore, la lista e\' vuota. Premere invio per continuare...");
        getchar();
        return;
    } else { // Se la lista principale non � vuota
        printf("Inserisci l\'anno in cui inserire lo studente: ");
        check = scanf("%d", &year);
        year = checkValue(year, check, 1088, INT_MAX);
        while (temp != NULL && found != 1) { // Controllo se l'anno cercato � presente nella lista principale (MIGLIORARE)
            if (year == temp->year)
                found = 1;
            else
                temp = temp->next;
        }
        if (!found) {
            printf("Errore, anno cercato non trovato. Premere invio per continuare....");
            free(newStudent);
            getchar();
            return;
        }
        temp->nStudents++; // Incremento il numero di studenti iscritti all'anno corrente
        if (temp->nStudents > temp->maxStudents && temp->code == 0) { // Se c'� il numero programmato controllo se c'� ancora posto
            printf("Errore, il numero massimo di studenti per l\'anno e\' stato raggiunto.\n");
            printf("Premere invio per continuare...");
            temp->nStudents--;
            free(newStudent);
            getchar();
            return;
        }
        printf("Inserisci la matricola: ");
        check = scanf("%d", &newStudent->code);
        newStudent->code = checkValue(newStudent->code, check, 0, INT_MAX);

        while (tempY != NULL) {
            tempS = tempY->student;
            while (tempS != NULL) {
                if (newStudent->code == tempS->code) {
                    printf("Errore, matricola %d gia\' presente nell\'archivio!", newStudent->code);
                    printf("\nPremere un tasto per continuare");
                    free(newStudent);
                    getchar();
                    return;
                }
                tempS = tempS->next;
            }
            tempY = tempY->next;
        }

        newStudent->next = temp->student;
        temp->student = newStudent;

        printf("Inserisci il nome: ");
        fgets(newStudent->name, MAX, stdin);
        length = strlen(newStudent->name) - 1;
            if (newStudent->name[length] == '\n')
                newStudent->name[length] = '\0';

        printf("Inserisci il cognome: ");
        fgets(newStudent->surname, MAX, stdin);
        length = strlen(newStudent->surname) - 1;
            if (newStudent->surname[length] == '\n')
                newStudent->surname[length] = '\0';

        printf("Inserisci l'indirizzo: ");
        fgets(newStudent->address, MAX, stdin);
        length = strlen(newStudent->address) - 1;
            if (newStudent->address[length] == '\n')
                newStudent->address[length] = '\0';

        printf("\nInserisci -1 se l'esami non e\' stato svolto\n");
        for (i = 0; i < N_ESAMI; i++) {
            printf("Inserisci il voto di %s: ", nomiesami[i]);
            check = scanf("%d", &newStudent->voti[i]);
            newStudent->voti[i] = checkValue(newStudent->voti[i], check, -1, 30);
        }
    }

    return;
}

void printList(TypeYear *head)
{
    TypeYear *temp;
    TypeStudent *tempS;
    int i;
    char nomiesami[][50] = {"Programmazione", "Inglese", "Analisi", "Algebra", "Architettura degli elaboratori", "Algoritmi"};


    system(CLEAR);
    printf("\t\t\t ---- VISUALIZZA ARCHIVIO ----\n");
    if (isEmpty(head)) { // Se la lista principale � vuota stampo un errore
        printf("\nErrore, la lista e\' vuota. Premere invio per continuare...");
        getchar();
        return;
    } else { // Se la lista principale non � vuota
        temp = head;
        while(temp != NULL) {
            tempS = temp->student;
            printf("\nAnno: %d-%d", temp->year, temp->year+1);
            printf(" - Iscritti: %d - Numero Programmato: ", temp->nStudents); temp->code == 0 ? printf("si") : printf("no");
            while (tempS != NULL) {
                printf("\n\n\tMatricola: %d", tempS->code);
                printf("\n\tNome: %s", tempS->name);
                printf("\n\tCognome: %s", tempS->surname);
                printf("\n\tIndirizzo: %s", tempS->address);
                for (i = 0; i < N_ESAMI; i++) {
                    if (tempS->voti[i] == -1 || tempS->voti[i] < 18)
                        printf("\n\tEsame di %s non svolto o superato", nomiesami[i]);
                    else
                        printf("\n\tVoto di %s: %d", nomiesami[i], tempS->voti[i]);
                }
                tempS = tempS->next;
            }
            printf("\n");
            temp = temp->next;
        }
    }

    getchar();
    return;
}

void cancel(TypeYear *head)
{
    int codeDel, check, found = 0;
    TypeYear *temp;
    TypeStudent *tempS, *prev = NULL;

    system(CLEAR);
    printf("\t\t\t ---- ELIMINA STUDENTE ----\n\n");

    if (isEmpty(head)) {
        printf("Errore, la lista e\' vuota. Premere invio per continuare...");
        getchar();
        return;
    } else {
        temp = head;
        printf("Inserire il numero di matricola dello studente da eliminare: ");
        check = scanf("%d", &codeDel);
        codeDel = checkValue(codeDel, check, 0, INT_MAX);

        while (temp != NULL && found != 1) {
            tempS = temp->student;
            while (tempS != NULL && found != 1) {
                if (tempS->code == codeDel) {
                    if (prev == NULL)
                        temp->student = tempS->next;
                    else {
                        prev->next = tempS->next;
                    }
                    found = 1;
                    temp->nStudents--;
                    free(tempS);
                }
                if (!found) {
                    prev = tempS;
                    tempS = tempS->next;
                }
            }
            if (!found)
                temp = temp->next;
        }
        if (!found) {
        printf("Errore, la matricola cercata non esiste.\nPremere invio per continuare...");
        getchar();
        return;
    }
    }
    return;
}

void searchStud(TypeYear *head)
{
    int yearSearch, codeSearch, check, i, found = 0;
    TypeYear *temp;
    TypeStudent *tempS;
    char nomiesami[][50] = {"Programmazione", "Inglese", "Analisi", "Algebra", "Architettura degli elaboratori", "Algoritmi"};


    system(CLEAR);
    temp = head;

    printf("\t\t ---- CERCA UNO STUDENTE ----\n\n");
    printf("Inserisci l\'anno in cui cercare lo studente: ");
    check = scanf("%d", &yearSearch);
    yearSearch = checkValue(yearSearch, check, 1088, INT_MAX);

    while (temp != NULL && found != 1) {
        if (yearSearch == temp->year) {
            found = 1;
        }
        if (!found)
            temp = temp->next;
    }
    if (!found) {
        printf("Errore, l\'anno cercato non esiste.\nPremere invio per continuare...");
        getchar();
        return;
    }

    printf("Inserisci la matricola dello studente da cercare: ");
    check = scanf("%d", &codeSearch);
    codeSearch = checkValue(codeSearch, check, 0, INT_MAX);

    found = 0;
    tempS = temp->student;
    while (tempS != NULL && found != 1) {
        if (codeSearch == tempS->code)
            found = 1;
        if (!found)
            tempS = tempS->next;
    }

    if (found) {
        printf("\n\n\tMatricola: %d", tempS->code);
        for (i = 0; i < N_ESAMI; i++) {
                if (tempS->voti[i] == -1 || tempS->voti[i] < 18)
                    printf("\n\tEsame di %s non svolto o superato", nomiesami[i]);
                else
                    printf("\n\tVoto di %s: %d", nomiesami[i], tempS->voti[i]);
        }
        getchar();
    } else {
        printf("Errore, matricola cercata non esistente.\nPremere invio per continuare...");
        getchar();
        return;
    }


}

void todo(TypeYear *head)
{
    int exam, yearSearch, check, i, found = 0;
    TypeYear *temp;
    TypeStudent *tempS;
    char nomiesami[][50] = {"Programmazione", "Inglese", "Analisi", "Algebra", "Architettura degli elaboratori", "Algoritmi"};


    system(CLEAR);
    temp = head;

    printf("\t ---- CERCA STUDENTI PER ESAMI DA SVOLGERE ----\n\n");
    printf("Inserisci l\'anno in cui cercare lo studente: ");
    check = scanf("%d", &yearSearch);
    yearSearch = checkValue(yearSearch, check, 1088, INT_MAX);

    while (temp != NULL && found != 1) {
        if (yearSearch == temp->year) {
            found = 1;
        }
        if (!found)
            temp = temp->next;
    }
    if (!found) {
        printf("Errore, l\'anno cercato non esiste.\nPremere invio per continuare...");
        getchar();
        return;
    }

    for (i = 0; i < N_ESAMI; i++) {
        printf("%d - %s\n", i+1, nomiesami[i]);
    }
    printf("Selezionare l\'esame su cui fare la ricerca: ");
    check = scanf("%d", &exam);
    exam = checkValue(exam, check, 1, 6);

    printf("\nStudenti che devono ancora svolgere l\'esame: \n");
    found = 0;
    tempS = temp->student;
    while (tempS != NULL) {
        if (tempS->voti[exam-1] == -1) {
            found = 1;
            printf(" - Matricola %d (%s %s)\n", tempS->code, tempS->name, tempS->surname);
        }
        tempS = tempS->next;
    }
    if (!found)
        printf("Tutti gli studenti hanno svolto correttamente l\'esame cercato");
    getchar();

}

void searchHighest (TypeYear *head)
{
    TypeYear *temp;
    TypeStudent *tempS;
    int i, count, yearHigh;
    float avg, highAvg = 0;

    system(CLEAR);
    printf("\t\t\t ---- CERCA ANNO MIGLIORE ----\n\n");
    if (isEmpty(head)) {
        printf("Errore, l\'archivio e\' vuoto!\nPremere invio per continuare...");
        getchar();
        return;
    } else {
        temp = head;
        while (temp != NULL) {
            avg = count = 0;
            tempS = temp->student;
            while (tempS != NULL) {
                for (i = 0; i < N_ESAMI; i++) {
                    if (tempS->voti[i]  != -1 && tempS->voti[i] > 17) {
                        avg += tempS->voti[i];
                        count++;
                    }
                }
                tempS = tempS->next;
            }
            avg = avg / count;
            if (avg > highAvg) {
                yearHigh = temp->year;
                highAvg = avg;
            }
            temp = temp->next;
        }
        printf("Anno migliore: %d con %g come voto medio", yearHigh, highAvg);
        getchar();
    }
}
