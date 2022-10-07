/*
FILE NAME: main.c

DESCRIPTION: main() function files used as a strating point for the project 
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "../Header/fun_head.h"

//Macros used in this project 

#define SUCCESS 1
#define ERROR 0
#define MAXFILES 5 // Maximum Input Defect files is set to 5
#define MAXEMP 10
#define MAXSTRLEN 200

//Global variables

pthread_t threadIDarr[MAXFILES]; // Array to store threadID of all child threads.
Emp *emp_arr[MAXEMP]; // Array of Employee Structure
int n_emp; // Total count of Employee


/*Function name: displayEmployee(arr[], n_emp);

Description: Used to display details of all employees.

*/

void displayEmployees(Emp *arr[], int n_emp)
{
    for (int i = 0; i < n_emp; i++)
    {
        printf("\nID: %s Name: %s", arr[i]->Id, arr[i]->Name);
    }
}


/* Function name: getEmployee(arr[]);

Description: read the details of employee and Tokenises each line and stores the details as a 
employee struction in an array of employee structure variable (emp_arr[])

*/

int getEmployee(Emp *arr[])
{
    char *f_loc = "../data/employees.txt"; // Location of Employee Database

    FILE *fpr = fopen(f_loc, "r");
    if (fpr == NULL)
    {
        printf("\n---- unable to open filepointer at location:  %s ---\n", f_loc);
        exit(1);
    }

    char str[MAXSTRLEN];
    int i = 0;
    while (1)
    {
        if (fgets(str, MAXSTRLEN, fpr) == NULL)
        {
            break;
        }

        arr[i] = (Emp *)calloc(1, sizeof(Emp));

        char *newstr = (char *)calloc(strlen(str), sizeof(char));
        strcpy(newstr, str);
        char *token = strtok(newstr, ":");
        arr[i]->Id = (char *)calloc(strlen(token), sizeof(char));
        arr[i]->Id = token;
        token = strtok(NULL, ":");
        arr[i]->Name = (char *)calloc(strlen(token), sizeof(char));
        arr[i]->Name = token;
        token = strtok(NULL, ":");
        arr[i]->BUnit = (char *)calloc(strlen(token), sizeof(char));
        arr[i]->BUnit = token;
        token = strtok(NULL, ":");
        arr[i]->Expertise = (char *)calloc(strlen(token), sizeof(char));
        arr[i]->Expertise = token;
        token = strtok(NULL, ":");
        arr[i]->Designation = (char *)calloc(strlen(token), sizeof(char));
        arr[i]->Designation = token;
        arr[i]->n_defect = 0;
        arr[i]->assigned_arr[MAXDEFECT] = (defect *)calloc(5, sizeof(defect));
        
        i++;
    }

    return i;
}

/*
FUNCTION NAME: main()

DESCRIPTION: Input Defect files are taken as command line arguments.
Separate threads are created for each input file passed, an array is given to store all thread IDs.
Passes file location with the thread to getdefect function Wait for all child threads to join.

RETURNS: SUCCESS on Successfull execution of program
         ERROR on failure at any point.
*/
int main(int argc, char *argv[])
{
    int n_files = argc - 1; // Total Defect Files to be checked
    printf(" Total files in queue: %d ", n_files);
    if (argc < 2)
    {
        printf("--- Insufficient argument passed. ---");
        printf("--- Usage: <executable> <defectfile1> <defectfile2> ... ---");
        exit(1);
    }

    n_emp = getEmployee(emp_arr);
    printf("\n\n--- Total Employee: %d ---\n", n_emp);
    displayEmployees(emp_arr, n_emp);

    // Creating Threads for each defect file
    int err; // err: captures error
    for (int i = 1; i < argc; i++)
    {
        printf("\n Creating Thread for file %d: %s \n", i, argv[i]);
        err = pthread_create(&threadIDarr[i - 1], NULL, (void *)getDefect, (void *)argv[i]);
        if (err)
        {
            printf("\n--- ERROR: %d while creating thread for file %d: %s ---\n", err, i, argv[i]);
            exit(1);
        }
    }

    // Waiting for child threads to join
    for (int i = 0; i < n_files; i++)
    {
        pthread_join(threadIDarr[i], NULL);
    }
    
    pthread_exit(NULL);
    return SUCCESS;
}