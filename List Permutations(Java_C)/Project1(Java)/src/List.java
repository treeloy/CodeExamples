//Eloy Salinas
//esalina1@ucsc.edu | 1243517
//Pa1
// List.java
// Doubly linked list integer queue
public class List {

		
		private class Node{
			//Fields
			int data;
			Node next;
			Node prev;
			
			//Constructor
			Node(int data){
				this.data = data;
				next = null;
				prev = null;
				
			}
			
			//toString: overrides Object's toString method
			public String toString(){
				return String.valueOf(data);
			}
			
		}
		
		//Fields
		private Node front;
		private Node back;
		private Node current;
		private int index;
		private int length;
		
		//Constructors
		List(){
			index = -1;
			front= null;
			back = null;
			current = null;
			length = 0;
		}
		
		//Access functions
		
		//Return the integer at the front of the queue
		//Pre-Condition: !isEmpty()
		int getFront(){
			if( this.isEmpty() ){
		         throw new RuntimeException("Error performing getFront(), Queue is empty.");
		      }
			return front.data;
		}
		
		//Return the integer at the back of the queue
		//Pre-Condition: !isEmpty()
		int getBack(){
			if( this.isEmpty() ){
				   throw new RuntimeException("Error performing getBack(), Queue is empty.");
			}
					return back.data;
		}
		
		// Returns current element. 
		//Pre-Condition: !isEmpty(), !offEnd()
		int getCurrent(){
			if( this.isEmpty() || this.offEnd() ){
				   throw new RuntimeException("Error performing getCurrent(), No current element selected.");
			}
			
			return current.data;
		}
		
		// Returns true if this List and L are the same integer  
        // sequence.  Ignores the current element in both Lists. 
		boolean equals(List L){
			boolean temp = true;
			Node T = this.front;
			Node V = L.front;
			
			if(this.getLength() == L.getLength()){
				while(temp && T!=null){
					temp = (T.data == V.data);
					T = T.next;
					V = V.next;
				}
				return temp;
			}else{
				return false;
			}
		}
		
		//Returns length of this List.
		int getLength(){
			return length;
		}
		
		//Return true if length is zero, false otherwise 
		boolean isEmpty(){
			if (length == 0){
				return true;
			}else 
				return false;
		}
		
		// Returns true if current is undefined. 
		boolean offEnd(){
			if (current == null){
				return true;
			}else
				return false;		
			
		}
		
		// If current element is defined, returns its position in  
        // this List, ranging from 0 to getLength()-1 inclusive.   
        // If current element is undefined, returns -1. 
		int getIndex(){
			return index;
		}
		
		
		
		//Manipulation procedures
		
		// Sets this List to the empty state. Post: isEmpty(). 
		void makeEmpty(){
			this.length = 0;
		}
		
		// If 0 <= i <= getLength()-1, moves current element  
        // marker to position i in this List.  Otherwise current  
        // element becomes undefined. 
		void moveTo(int i){
			if(0 > i || i <= getLength() - 1){
			Node temp = this.front;
			for(int j = 0; j < i; j++){
				temp = temp.next;
			}
			
			this.current = temp;
			current.next = temp.next;
			current.prev = temp.prev;
			index = i;
			}else{
				this.current = null;
			}
		}
		
		// Moves current one step toward front element.  If the current 
        // element is already the front element, current element becomes 
        // undefined.  Pre: !isEmpty(), !offEnd(). 
		void movePrev(){
			if( this.isEmpty() || this.offEnd() ){
				   throw new RuntimeException("Error performing getPrev(), Queue is empty.");
			}
			if(this.front == this.current){
				this.current = null;
			}else
				this.current = this.current.prev;
				index--;
		}
		
		// Moves current one step toward back element.  If the current 
        // element is already the back element, current element becomes 
        // undefined.  Pre: !isEmpty(), !offEnd().
		void moveNext(){
			if( this.isEmpty() || this.offEnd() ){
				   throw new RuntimeException("Error performing moveNext(), Queue is empty.");
			}
			if(this.current == this.back){
				this.current = null;
			}else
				this.current = this.current.next;
				index++;
		}
		
		// Inserts new element before front element. 
        // Post: !isEmpty(). 
		void insertFront(int data){
				Node temp = new Node(data);
				if( this.isEmpty() ) { 
					front = back = temp;
				}else{
					front.prev = temp;
					temp.next = front;
					front = temp;
				}
				this.length++;
		}
		
		// Inserts new element after back element. 
        // Post: !isEmpty(). 
		void insertBack(int data){
			Node temp = new Node(data);
			if( this.isEmpty() ) { 
				front = back = temp;
			}else{
				back.next = temp;
				temp.prev = back;
				back = temp;
				
			}
			
		    length++;
			
		}
		
		//Remove an element from the front of the queue
		//Pre-Condition: !isEmpty()
		void deleteFront(){
			if( this.isEmpty() ){
		         throw new RuntimeException("Error performing deleteFront(), Queue is empty.");
		      }
			if(this.current == front){
				this.current = null;
			}

			front = front.next;
			front.prev = null;
				 

		      length--;
			
		}
		
		//Remove an element from the back of the queue
		//Pre-Condition: !isEmpty()
		void deleteBack(){
			if( this.isEmpty() ){
				 throw new RuntimeException("Error performing deleteBack(), Queue is empty.");
			  }
			if(this.current == back){
				this.current = null;
			}
			back = back.prev;
			back.next = null;
			
		      
		      length--;
					
		}
		
		// Deletes current element, which then becomes undefined. 
        // Pre: !isEmpty(), !offEnd(); Post: offEnd() 
		void deleteCurrent(){
			if( this.isEmpty() && this.offEnd() ){
		         throw new RuntimeException("Error performing deleteCurrent(), Queue is empty.");
		      }
			if(current == front){
				front = front.next;
				front.prev = null;
			}
			if(current == back){
				back = back.prev;
				back.next = null;
			}else{
			
			Node behind = this.current.prev;
			Node infront = this.current.next;
			infront.prev = behind;
			behind.next = infront;
			length--;
			
			}
			current = null;
			
		}
		
		// Inserts new element before current element. 
        // Pre: !isEmpty(), !offEnd(). 
		void insertBeforeCurrent(int data){
			if( this.isEmpty() && this.offEnd() ){
		         throw new RuntimeException("Error performing insertBeforeCurrent(), Queue is empty or current is offend.");
		      }
			if(current == front){
				Node temp = new Node(data);
				front.prev = temp;
				temp.next = front;
				front = temp;
			}else{
			Node temp = new Node(data);
			
			temp.next = current;
			temp.prev = current.prev;
			current.prev.next = temp;
			
			current.prev = temp;
			index++;
			length++;
			}
		}
		
		// Inserts new element after current element. 
        // Pre: !isEmpty(), !offEnd(). 
		void insertAfterCurrent(int data){
			if( this.isEmpty() || this.offEnd() ){
		         throw new RuntimeException("Error performing insertAfterCurrent(), Queue is empty or no current marker.");
		      }
			
			if(current == back){
				insertBack(data);
				
			}else {
		    Node temp = new Node(data);
			
			temp.next = current.next;
			
			temp.prev = current;
			current.next = temp;
			current.next.prev = temp;
			length++;
			index++;
			}
		}
		
		// other methods 
		// toString:  Overrides Object's toString method. 
		public String toString() {
			StringBuilder str = new StringBuilder();
			for(Node node=front; node!=null; node=node.next){
		         str.append(node.data);
		         str.append(" ");
		      }
			
			return str.toString();
		} 
		
		// Returns a new list which is identical to this list.  The current 
        // element in the new list is undefined, regardless of the state of  
        // the current element in this List.  The state of this List is 
        // unchanged. 
		List copy(){
		      List C = new List();
		      Node N = this.front;

		      while( N!=null ){
		         C.insertBack(N.data);
		         N = N.next;
		      }
		      return C;
		   }

	
	
}
