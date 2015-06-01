
/**********************************************************************************
* Eloy Salinas
* eloys@ucsc.edu | 1243517
* Pa2
* ListTest.c
* A test client for List ADT
***********************************************************************************/

#include<stdio.h>
#include"List.h"

int main(int argc, char* argv[])
{
   int i;
   ListRef A = newList();
   ListRef B = newList();

   for(i=1; i<=10; i++)
   {
      insertBack(A, i);
      insertBack(B, 11-i);
   }
   printListStd(A);
   printListStd(B);
   printf("\n");

   for(i=1; i<=6; i++)
   {
      insertBack(A, getFront(B));
      deleteFront(B);
   }
   printListStd(A);
   printListStd(B);
   printf("\n");

   printf("%s\n", equals(A,B)?"true":"false");
   printf("%s\n", equals(A,A)?"true":"false");
   printf("\n");
   printf("offEnd(A) %d", offEnd(A));
   printf("\n");
   printf("getIndex(A) %d", getIndex(A));
   printf("\n");
   printf("getBack(A) %d", getBack(A));
   printf("\n");
   /*printf("getCurrent(A) %d", getCurrent(A));*/
   printf("\n");

   ListRef C = newList();
   insertBack(C, 4);
   insertBack(C, 3);
   insertBack(C, 2);
   insertBack(C, 1);

   printListStd(C);
   printf("%s\n", equals(B,C)?"true":"false");
   printf("\n");
   makeEmpty(C);
   printf("isEmpty(C) ");
   printf("%s\n", isEmpty(C)?"true":"false");

   moveTo(B, 1);
   printf("getCurrent(B) %d", getCurrent(B));
   printf("\n");
   printf("getLength(B) %d", getLength(B));
   printf("\n");
   printf("getIndex(B) %d", getIndex(B));
   movePrev(B);
   printf("\n");
   printf("getCurrent(B) %d", getCurrent(B));
   moveNext(B);
   moveNext(B);
   printf("\n");
   printf("getCurrent(B) %d", getCurrent(B));
   printf("\n");
   printf("getIndex(B) %d", getIndex(B));
   moveNext(B);
   printf("\n");
   printf("getCurrent(B) %d", getCurrent(B));
   printf("\n");
   printf("getIndex(B) %d", getIndex(B));
   insertFront(B, 9);
   printf("\n");
   printListStd(B);
   printf("\n");
   printf("getCurrent(B) %d", getCurrent(B));
   printf("\n");
   printf("getIndex(B) %d", getIndex(B));
   insertBeforeCurrent(B, 8);
   printf("\n");
   printListStd(B);
   printf("\n");
   printf("getCurrent(B) %d", getCurrent(B));
   printf("\n");
   printf("getIndex(B) %d", getIndex(B));
   moveTo(B, 3);
   insertAfterCurrent(B, 5);
   printf("\n");
   printListStd(B);
   printf("\n");
   printf("getCurrent(B) %d", getCurrent(B));
   printf("\n");
   printf("getIndex(B) %d", getIndex(B));
   printf("\n");
   printf("getLength(B) %d", getLength(B));
   deleteFront(B);
   deleteBack(B);
   printf("\n");
   printListStd(B);
   printf("\n");
   printf("getCurrent(B) %d", getCurrent(B));
   printf("\n");
   printf("getIndex(B) %d", getIndex(B));
   deleteCurrent(B);
   printf("\n");
   printListStd(B);
   ListRef D = copyList(B);
   printf("\n");
   printListStd(B);
   printf("\n");
   printListStd(D);
   printf("%s\n", equals(B,D)?"true":"false");



   freeList(&A);
   freeList(&B);
   freeList(&C);
   freeList(&D);

   return(0);
}
