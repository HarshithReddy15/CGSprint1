/*
FILE NAME: assign.c

DESCRIPTION: For Every defect passed it searcher for a programmer with the same functional area
of intreast and if found it allocates to the programmer else it printd the defect details 
in a file namae unassigned.txt
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "../Header/fun_head.h"

//Macros used 
#define SUCCESS 1
#define ERROR 0
#define MAXDEFECT 10
#define MAXSTRLEN 200
#define MAXEMP 10

//Global variables 
extern Emp *emp_arr[MAXEMP];
extern int n_emp;

/* 
Function Name: unassignedDefect();

Description: if there is no programmer found for the defect 
it will append the defect details in the file name unassignedDefects.txt

*/

void unassignedDefect(defect *defectptr)
{
    char *fileDefectPtr = "../data/out/unassignedDefect.txt";
    FILE *flp;
    flp = fopen(fileDefectPtr, "a");
    if (flp == NULL)
    {
        printf("\n--- Cannot open %s file ---", fileDefectPtr);
        exit(1);
    }
    fprintf(flp, "%s : %s : %s : %s : %s : %s : %s", defectptr->defectID,
            defectptr->description,
            defectptr->moduleName,
            defectptr->functionalArea,
            defectptr->date,
            defectptr->status,
            defectptr->type);
    fclose(flp);
}

/*
Function Name:searchProgrammer

Description:This function block is used to search programmer for respective defect.
If the programmer is found then the defect status is updated to assigned or else defect is added to unassigned defect category.
Return type: void

*/

void searchProgrammer(defect *defectptr, Emp *arr[], int n_emp)
{
    int i, foundflag = 0;
    for (i = 0; i < n_emp; i++)
    {
        if (strcmp(defectptr->functionalArea, arr[i]->Expertise) == 0)
        {
            foundflag = 1;
            defectptr->status = "Assigned";
            pthread_mutex_lock(&arr[i]->emplock);
            arr[i]->n_defect++;
            arr[i]->assigned_arr[(arr[i]->n_defect) - 1] = defectptr;
            createEmployeeFile(arr[i], defectptr);
            printf("\nDefect Id: %s\nStatus: %s\n", defectptr->defectID, defectptr->status);
            printf("Module Name: %s\nFunctional Area: %s\nDescription: %s\n\nHas been assigned to:-\nEmployee Id: %s\nEmployee Name: %s\n", defectptr->moduleName, defectptr->functionalArea, defectptr->description, arr[i]->Id, arr[i]->Name);
            pthread_mutex_unlock(&arr[i]->emplock);
            break;
        }
    }
    if (foundflag == 0)
    {
        printf("\n--- Programmer not found for defect Id: %s ---", defectptr->defectID);
        unassignedDefect(defectptr);
        return;
    }
}

/*
Function Name: createEmployeeFile();

Description: This Function reads the string from file and append this information to the employee file.

*/
void createEmployeeFile(Emp *emp_ptr, defect *defectptr)
{
    char out_file[MAXSTRLEN];
    sprintf(out_file, "../data/out/%s_assignments.txt", emp_ptr->Id);
    FILE *fp = fopen(out_file, "a");
    if (fp == NULL)
    {
        printf("\n--- Unable to write into output Employee file ---\n");
        pthread_exit(NULL);
    }
    fprintf(fp, "%s : %s : %s : %s : %s : %s : %s : %s", emp_ptr->Id, emp_ptr->Name, defectptr->defectID, defectptr->description, defectptr->moduleName, defectptr->functionalArea, defectptr->date, defectptr->type);
    fclose(fp);
}

/*
Function Name: assignEmployee()

Description: for every defect file received it is going to check each defect 
and if the status is open it is going to search for programmer and update open
defect count(odc)

*/

void assignEmployee(defect *arr[], int vdc)
{

    int odc = 0; // odc: Open Defects Count
    for (int i = 0; i < vdc; i++)
    {
        if (!strcmp(arr[i]->status, "open"))
        {
            odc++;
            printf("\n\n--- Searching Programmer for defect Id: %s ---", arr[i]->defectID);
            searchProgrammer(arr[i], emp_arr, n_emp);
        }
    }
    
}