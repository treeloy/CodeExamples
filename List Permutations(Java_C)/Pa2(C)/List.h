/**********************************************************************************
* Eloy Salinas
* eloys@ucsc.edu | 1243517
* Pa2
* List.h
* Header file for List ADT
***********************************************************************************/
#if !defined(_List_H_INCLUDE_)
#define _List_H_INCLUDE_

/************** Types ******************************************/

typedef struct List * ListRef;


/************** Constructors-Destructors ******************************************/

/*
*  newList
*  Returns ListRef pointing to new ListSturct which represents an empty List.
*  Initializes front and back fields to NULL, sets length field to 0.  Exported.
*/
ListRef newList(void);

/*
*  freeList
*  Frees all heap memory associated with the ListRef *pL, including all memory
*  in existing Nodes.  Sets *pL to NULL.  Exported.
*/
void freeList(ListRef* pL);

/***************** Access functions ***********************************************/


/*
*  getLength
*  Returns the length of L
*/
int getLength(ListRef L);

/*
*  isEmpty
*  Returns True if L is empty, otherwise returns false
*/
int isEmpty(ListRef L);

/*
*  offEnd
*  Returns true(1) if current is undefined.
*/
int offEnd(ListRef L);

/*
* If current element is defined, returns its position in
* this List, ranging from 0 to getLength()-1 inclusive.
* If current element is undefined, returns -1.
*/

int getIndex(ListRef L);

/*
*  getFront
*  Returns the value at the front of L.
*  Pre: !isEmpty(L)
*/
int getFront(ListRef L);

/*
*  getBack
*  Returns the value at the front of L.
*  Pre: !isEmpty(L)
*/
int getBack(ListRef L);

/*
*  getCurrent
*  Returns current element.
*  Pre: !isEmpty(), !offEnd().
*/
int getCurrent(ListRef L);

/*
*  equals
*  returns true if A is identical to B, false otherwise
*/
int equals(ListRef A, ListRef B);


/****************************** Manipulation procedures ***************************/

/*
*  makeEmpty
*  Sets this List to the empty state.
*  Post: !isEmpty(L)
*/
void makeEmpty(ListRef L);

/*
*  moveTo
*  If 0 <= i <= getLength()-1, moves current element
*  marker to position i in this List.  Otherwise current
*  element becomes undefined.
*/
void moveTo(ListRef L, int i);

/*
*  movePrev
*  Moves current one step toward front element.  If the current
*  element is already the front element, current element becomes
*  undefined.
*  Pre: !isEmpty(), !offEnd().
*/
void movePrev(ListRef L);

/*
*  moveNext
*  Moves current one step towards back front.  If the current
*  element is already the front element, current element becomes
*  undefined.
*  Pre: !isEmpty(), !offEnd().
*/
void moveNext(ListRef L);

/*
*  insertFront
*  Places new data element at the beginning of L
*  Post: !isEmpty(L)
*/
void insertFront(ListRef L, int data);

/*
*  insertBack
*  Places new data element at the end of L
*  Post: !isEmpty(L)
*/
void insertBack(ListRef L, int data);

/*
*  insertBeforeCurrent
*  Inserts new element before current element.
*  Pre: !isEmpty(), !offEnd().
*/
void insertBeforeCurrent(ListRef L, int data);

/*
*  insertAfterCurrent
*  Inserts new element before current element.
*  Pre: !isEmpty(), !offEnd().
*/
void insertAfterCurrent(ListRef L, int data);

/*
*  deleteFront
*  Deletes element at front of L
*  Pre: !isEmpty(L)
*/
void deleteFront(ListRef L);

/*
*  deleteBack
*  Deletes element at back of L
*  Pre: !isEmpty(L)
*/
void deleteBack(ListRef L);

/*
*  deleteCurrent
*  Deletes current element, which then becomes undefined.
*  Pre: !isEmpty(), !offEnd(); Post: offEnd()
*/
void deleteCurrent(ListRef L);


/*************** Other Functions *************************************************/

/*
*  printList
*  Prints data elements in L on a single line to stdout.
*/
void printListStd(ListRef L);

/*
*  printList
*  Prints data elements in L on a single line to an out file.
*/
void printList(FILE* out, ListRef L);

/*
*  copyList
*  Returns a new list which is identical to this list.  The current
*  element in the new list is undefined, regardless of the state of
*  the current element in this List.  The state of this List is
*  unchanged.
*/
ListRef copyList(ListRef L);

#endif
