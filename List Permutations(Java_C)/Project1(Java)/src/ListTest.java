//Eloy Salinas
//esalina1@ucsc.edu | 1243517
//Pa1
//ListTest.java
//Tests the List class functions
public class ListTest {
	public static void main(String[] args){
		List A = new List();
		List B = new List();
		List C = new List();
		List D = new List();
		
		for(int i=1; i<=10; i++){
			A.insertBack(i);
		}
		for(int i=1; i<=10; i++){
			B.insertFront(i);
		}
		for(int i=1; i<=10; i++){
			C.insertFront(i);
		}
		for(int i=1; i<=5; i++){
			D.insertFront(i);
		}
		
		D = A;
		System.out.println("A = " + A);
		System.out.println("B = " + A);
		System.out.println("D = " + D);
		System.out.println("A getFront()" + A.getFront());
		System.out.println("B getFront()" + B.getFront());
		System.out.println("A equals(B)" + A.equals(B) + " Should be false, they were created in different directions.");
		System.out.println("B equals(C)" + B.equals(C) + " Should be true, created the same way.");
		
		A.insertFront(2);
		System.out.println("A = " + A);
		System.out.println(A.getLength());
		
		A.insertBack(5);	
		System.out.println("A = " + A);
		System.out.println(A.getLength());
		
		A.deleteFront();
		System.out.println("A = " + A);
		System.out.println(A.getLength());
		
		A.deleteBack();
		System.out.println("A = " + A);
		System.out.println("A getLength()" + A.getLength());
		System.out.println("A getBack()" + A.getBack());
		System.out.println("A getFront()" + A.getFront());
		System.out.println("A getIndex()" + A.getIndex());
		A.moveTo(9);
		System.out.println("A getCurrent()" + A.getCurrent());
		
		A.moveTo(2);
		System.out.println("A getCurrent()" + A.getCurrent());
		A.moveNext();
		System.out.println("A getCurrent()" + A.getCurrent());
		A.movePrev();
		System.out.println("A getCurrent()" + A.getCurrent());
		
		A.insertAfterCurrent(8);
		System.out.println("A = " + A);
		
		A.insertBeforeCurrent(9);
		System.out.println("A getCurrent()" + A.getCurrent());
		System.out.println("A = " + A);
		
		A.deleteCurrent();
		System.out.println("A = " + A);
		System.out.println("A getLength()" + A.getLength());
		System.out.println("A offEnd()" + A.offEnd());
		
		A.makeEmpty();
		System.out.println("A isEmpty()" + A.isEmpty());
		
	}
	

}
