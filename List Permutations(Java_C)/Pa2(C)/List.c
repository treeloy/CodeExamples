/**********************************************************************************
* Eloy Salinas
* eloys@ucsc.edu | 1243517
* Pa2
* List.c
* Implementation file for List ADT
***********************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "List.h"

/*** Private Structs ***/
typedef struct Node
{
    int data;
    struct Node* next;
    struct Node* prev;
} Node;

typedef Node* NodeRef;

typedef struct List
{
    NodeRef front;
    NodeRef back;
    NodeRef current;
    int length;
    int index;
} List;


/************** Constructors-Destructors ******************************************/

/*
*  newNode
*  Returns pointer to new Node struct. Initializes next field to NULL, and sets
*  data field to input parameter node_data. Private.
*/
NodeRef newNode(int node_data)
{
    NodeRef N = malloc(sizeof(Node));
    N->data = node_data;
    N->next = N->prev = NULL;
    return(N);
}

/*
*  freeNode
*  Frees heap memory pointed to by pN. Private.
*/
void freeNode(NodeRef* pN)
{
    if( pN!=NULL && *pN!=NULL )
    {
        free(*pN);
        *pN = NULL;
    }
}


/*
*  newList
*  Returns ListRef pointing to new ListSturct which represents an empty List.
*  Initializes front and back fields to NULL, sets length field to 0.  Exported.
*/
ListRef newList(void)
{
    ListRef L;
    L = malloc(sizeof(List));
    L->front = L->back = L->current = NULL;
    L->length = 0;
    L->index = -1;

    return(L);
}

/*
*  freeList
*  Frees all heap memory associated with the ListRef *pL, including all memory
*  in existing Nodes.  Sets *pL to NULL.  Exported.
*/
void freeList(ListRef* pL)
{
    if(pL==NULL || *pL==NULL)
    {
        return;
    }
    while( !isEmpty(*pL) )
    {
        deleteFront(*pL);
    }
    free(*pL);
    *pL = NULL;
}


/***************** Access functions ***********************************************/

/*
*  getLength
*  Returns the length of L
*/
int getLength(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling getLength() on NULL ListRef\n");
        exit(1);
    }
    return(L->length);
}

/*
*  isEmpty
*  Returns True if L is empty, otherwise returns 0
*/
int isEmpty(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling isEmpty() on NULL ListRef\n");
        exit(1);
    }
    if(L->length == 0)
    {
        return(1);
    }
    else
        return(0);
}

/*
*  offEnd
*  Returns true(1) if current is undefined
*/
int offEnd(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling offEnd() on NULL ListRef\n");
        exit(1);
    }
    if (L->current == NULL)
    {
        return (1);
    }
    else
        return (0);

}

/*
* getIndex
* If current element is defined, returns its position in
* this List, ranging from 0 to getLength()-1 inclusive.
* If current element is undefined, returns -1.
*/

int getIndex(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling getIndex() on NULL ListRef\n");
        exit(1);
    }
    return (L->index);
}

/*
*  getFront
*  Returns the value at the front of L.
*  Pre: !isEmpty(L)
*/
int getFront(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling getFront() on NULL ListRef\n");
        exit(1);
    }
    if( isEmpty(L) )
    {
        printf("List Error: calling getFront() on an empty List\n");
        exit(1);
    }
    return(L->front->data);
}

/*
*  getBack
*  Returns the value at the front of L.
*  Pre: !isEmpty(L)
*/
int getBack(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling getBack() on NULL ListRef\n");
        exit(1);
    }
    if( isEmpty(L) )
    {
        printf("List Error: calling getBack() on an empty List\n");
        exit(1);
    }
    return(L->back->data);
}

/*
*  getCurrent
*  Returns current element.
*  Pre: !isEmpty(), !offEnd().
*/
int getCurrent(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling getCurrent() on NULL ListRef\n");
        exit(1);
    }
    if( isEmpty(L)|| offEnd(L) )
    {
        printf("List Error: calling getCurrent() on an empty List or no current pointer\n");
        exit(1);
    }
    else
    {
        NodeRef temp = L->front;
        for(int j = 0; j < L->index; j++)
        {
            temp = temp->next;
        }
        return(temp->data);

    }

}

/*
*  equals
*  returns 1 if A is identical to B, 0 otherwise
*/
int equals(ListRef A, ListRef B)
{

    int flag = 1;
    Node* N = NULL;
    Node* M = NULL;

    if( A==NULL || B==NULL )
    {
        printf("List Error: calling equals() on a NULL ListRef\n");
        exit(1);
    }
    N = A->front;
    M = B->front;
    if( A->length != B->length )
    {
        return 0;
    }
    while( flag && N!=NULL)
    {
        flag = (N->data==M->data);
        N = N->next;
        M = M->next;
    }
    return flag;
}
/**************** Manipulation procedures ****************************************/

/*
*  makeEmpty
*  Sets this List to the empty state.
*  Post: isEmpty(L)
*/
void makeEmpty(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling makeEmpty() on NULL ListRef\n");
        exit(1);
    }
    L->length = 0;
}

/*
*  moveTo
*  If 0 <= i <= getLength()-1, moves current element
*  marker to position i in this List.  Otherwise current
*  element becomes undefined.
*/
void moveTo(ListRef L, int i)
{
    if( L==NULL )
    {
        printf("List Error: calling moveTo() on NULL ListRef\n");
        exit(1);
    }

    if(0 <= i && i <= getLength(L) - 1)
    {
        NodeRef temp = L->front;
        for(int j = 0; j < i; j++)
        {
            temp = temp->next;
        }

        L->current = temp;
        L->current->next = temp->next;
        L->current->prev = temp->prev;
        L->index = i;

    }
    else
    {
        L->current = NULL;
    }

}

/*
*  movePrev
*  Moves current one step toward front element.  If the current
*  element is already the front element, current element becomes
*  undefined.
*  Pre: !isEmpty(), !offEnd().
*/
void movePrev(ListRef L)
{
    NodeRef temp = L->front;
    if( L == NULL )
    {
        printf("List Error: calling movePrev() on NULL ListRef\n");
        exit(1);
    }
    if( isEmpty(L) || offEnd(L))
    {
        printf("List Error: calling movePrev() on an empty List or no current pointer\n");
        exit(1);
    }
    if(L->front == L->current)
    {
        L->current = NULL;
        L->index--;
    }
    else
        L->index--;
    for(int j = 0; j < L->index; j++)
    {
        temp = temp->next;
    }
    L->current = temp;


}

/*
*  moveNext
*  Moves current one step towards back front.  If the current
*  element is already the back element, current element becomes
*  undefined.
*  Pre: !isEmpty(), !offEnd().
*/
void moveNext(ListRef L)
{
    NodeRef temp = L->front;
    if( L == NULL )
    {
        printf("List Error: calling moveNext() on NULL ListRef\n");
        exit(1);
    }
    if( isEmpty(L) || offEnd(L))
    {
        printf("List Error: calling moveNext() on an empty List or no current pointer\n");
        exit(1);
    }
    if(L->back == L->current)
    {
        L->current = NULL;
        L->index = -1;
    }
    else
        L->index++;
    for(int j = 0; j < L->index; j++)
    {
        temp = temp->next;
    }
    L->current = temp;


}

/*
*  insertFront
*  Places new data element at the beginning of L
*  Post: !isEmpty(L)
*/
void insertFront(ListRef L, int data)
{
    NodeRef N = newNode(data);

    if( L==NULL )
    {
        printf("List Error: calling insertFront() on NULL ListRef\n");
        exit(1);
    }
    if( isEmpty(L) )
    {
        L->front = L->back = N;
        L->length++;
    }
    else
    {
        L->front->prev = N;
        N->next = L->front;
        L->front = N;
        L->length++;
        L->index++;

    }

}

/*
*  insertBack
*  Places new data element at the end of L
*  Post: !isEmpty(L)
*/
void insertBack(ListRef L, int data)
{
    NodeRef N = newNode(data);

    if( L==NULL )
    {
        printf("List Error: calling insertBack() on NULL ListRef\n");
        exit(1);
    }
    if( isEmpty(L) )
    {
        L->front = L->back = N;
        L->length++;
    }
    else
    {
        L->back->next = N;
        L->back = N;
        L->length++;
    }

}

/*
*  insertBeforeCurrent
*  Inserts new element before current element.
*  Pre: !isEmpty(), !offEnd().
*/
void insertBeforeCurrent(ListRef L, int data)
{
    if( L==NULL )
    {
        printf("List Error: calling insertBeforeCurrent() on NULL ListRef\n");
        exit(1);
    }
    NodeRef temp = newNode(data);
    NodeRef back = L->front;
    NodeRef front = L->front;
    for(int j = 0; j < L->index-1; j++)
    {
        back = back->next;
    }
    for(int j = 0; j < L->index; j++)
    {
        front = front->next;
    }
    if( isEmpty(L) || offEnd(L))
    {
        printf("List Error: calling insertBeforeCurrent() on an empty List or no current pointer\n");
        exit(1);
    }
    if(L->current == L->front)
    {

        insertFront(L, data);
    }
    else
    {


        temp->next = front;
        temp->prev = back;
        back->next = temp;

        front->prev = temp;
        L->index++;
        L->length++;
    }

}

/*
*  insertAfterCurrent
*  Inserts new element before current element.
*  Pre: !isEmpty(), !offEnd().
*/
void insertAfterCurrent(ListRef L, int data)
{
    if( L==NULL )
    {
        printf("List Error: calling insertAfterCurrent() on NULL ListRef\n");
        exit(1);
    }
    if( isEmpty(L) || offEnd(L))
    {
        printf("List Error: calling insertAfterCurrent() on an empty List or no current pointer\n");
        exit(1);
    }

    NodeRef back = L->front;
    NodeRef front = L->front;
    for(int i = 0; i < L->index; i++)
    {
        back = back->next;
    }
    for(int j = 0; j < L->index+1; j++)
    {
        front = front->next;
    }

    if(L->current == L->back)
    {

        insertBack(L, data);

    }

    else
    {NodeRef Q = newNode(data);


        Q->next = front;
        Q->prev = back;
        back->next = Q;

        front->prev = Q;

        L->length++;


    }




}

/*
*  deleteFront
*  Deletes element at front of L
*  Pre: !isEmpty(L)
*/
void deleteFront(ListRef L)
{
    NodeRef N = NULL;


    if( L==NULL )
    {
        printf("List Error: calling deleteFront() on NULL ListRef\n");
        exit(1);
    }
    if( isEmpty(L) )
    {
        printf("List Error: calling deleteFront() on an empty List\n");
        exit(1);
    }
    if(L->current == L->front)
    {
        L->current = NULL;
    }
    N = L->front;
    if( getLength(L)>1 )
    {
        L->front = L->front->next;
        L->front->prev = NULL;
        L->index--;
    }
    else
    {
        L->front = L->back = NULL;
    }
    L->length--;
    freeNode(&N);

}

/*
*  deleteBack
*  Deletes element at back of L
*  Pre: !isEmpty(L)
*/
void deleteBack(ListRef L)
{


    if( L==NULL )
    {
        printf("List Error: calling deleteBack() on NULL ListRef\n");
        exit(1);
    }
    if( isEmpty(L) )
    {
        printf("List Error: calling deleteBack() on an empty List\n");
        exit(1);
    }
    if(L->current == L->back)
    {
        L->current = NULL;
    }


    if( getLength(L)>1 )
    {
        L->back = L->back->prev;
        L->back->next = NULL;
    }
    else
    {
        L->front = L->back = NULL;
    }
    L->length--;

}

/*
*  deleteCurrent
*  Deletes current element, which then becomes undefined.
*  Pre: !isEmpty(), !offEnd();
*  Post: offEnd()
*/
void deleteCurrent(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling deleteCurrent() on NULL ListRef\n");
        exit(1);
    }
    NodeRef behind = L->front;
    NodeRef infront = L->front;
    for(int j = 0; j < L->index-1; j++)
    {
        behind = behind->next;
    }
    for(int j = 0; j < L->index+1; j++)
    {
        infront = infront->next;
    }
    if( isEmpty(L) || offEnd(L) )
    {
        printf("List Error: calling deleteCurrent() on an empty List or no current pointer\n");
        exit(1);
    }
    if(L->current == L->front)
    {
        deleteFront(L);
    }
    if(L->current == L->back)
    {
        deleteBack(L);
    }
    else
    {


        infront->prev = behind;
        behind->next = infront;
        L->length--;

    }
    L->current = NULL;


}
/*************** Other Functions *************************************************/

/*
*  printListStd
*  Prints data elements in L on a single line to stdout.
*/
void printListStd(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling printListStd() on NULL ListRef\n");
        exit(1);
    }
    Node* N = NULL;

    if( L==NULL )
    {
        printf("List Error: calling printList() on NULL ListRef\n");
        exit(1);
    }
    for(N = L->front; N != NULL; N = N->next)
    {
        printf("%d ", N->data);
    }
    printf("\n");
}

/*
*  printList
*  Prints data elements in L on a single line to out file.
*/
void printList(FILE* out, ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling printList() on NULL ListRef\n");
        exit(1);
    }
    Node* N = NULL;

    if( L==NULL )
    {
        printf("List Error: calling printList() on NULL ListRef\n");
        exit(1);
    }
    for(N = L->front; N != NULL; N = N->next)
    {
        fprintf(out, "%d ", N->data);

    }

}

/*
*  copyList
*  Returns a new list which is identical to this list.  The current
*  element in the new list is undefined, regardless of the state of
*  the current element in this List.  The state of this List is
*  unchanged.
*/
ListRef copyList(ListRef L)
{
    if( L==NULL )
    {
        printf("List Error: calling copyList() on NULL ListRef\n");
        exit(1);
    }
    ListRef C = newList();
    NodeRef N = L->front;

    while( N!=NULL )
    {
        insertBack(C, N->data);
        N = N->next;
    }
    return C;
}
