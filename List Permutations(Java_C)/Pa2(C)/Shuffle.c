/**********************************************************************************
* Eloy Salinas
* eloys@ucsc.edu | 1243517
* Pa2
* Shuffle.c
* Shuffles a doubly linked list with values from a sepearete linked list,
* then counts the number of times it takes to get back to the original list.
***********************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "List.h"

#define MAX_LEN 250

static void shuffle(ListRef L, ListRef P);

int main(int argc, char* argv[])
{
    int n =0;
    FILE *in, *out;
    char line[MAX_LEN];
    char* token;
    int linenumber = 0;


    /* check command line for correct number of arguments */
    if( argc != 3 )
    {
        printf("Usage: %s infile outfile\n", argv[0]);
        exit(1);
    }

    /* open files for reading and writing */
    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");
    if( in==NULL )
    {
        printf("Unable to open file %s for reading\n", argv[1]);
        exit(1);
    }
    if( out==NULL )
    {
        printf("Unable to open file %s for writing\n", argv[2]);
        exit(1);
    }
    fgets(line, MAX_LEN, in);
    token = strtok(line, " \n");
    int permu = atoi(token);




    for(int lines = 1; lines<= permu; lines++)
    {
        ListRef P = newList();
        ListRef L = newList();
        ListRef T = NULL;
        n = 0;
        fprintf(out,"(");
        linenumber++;
        fgets(line, MAX_LEN, in);
        token = strtok(line, " \n");

        while( token!=NULL )
        {
            insertBack(P, atoi(token));
            n++;
            token = strtok(NULL, " \n");
        }
        for(int i = 1; i <= n; i++)
        {
            insertBack(L, i);
        }

        T = copyList(L);

        shuffle(L, P);
        int order = 1;


        printList(out, L);

        while(!equals(L, T))
        {
            shuffle(L,P);
            order++;

        }

        fprintf(out,") order=%d", order);
        fprintf(out, " \n");



        freeList(&P);
        freeList(&L);
        freeList(&T);


    }

    fclose(in);
    fclose(out);



    return(0);
}

static void shuffle(ListRef L, ListRef P)
{

    moveTo(L, 0);
    moveTo(P, 0);
    int numleft = getLength(P);

    for(int len = 0; len < getLength(P); len++)
    {
        int am = getCurrent(L);
        int be = getCurrent(P) + numleft;

        moveTo(L, be-2); //would be -1 to not go off end but started index at 0
        insertAfterCurrent(L, am);
        deleteFront(L);
        moveTo(L, 0);
        moveNext(P);
        numleft--;




    }


}
