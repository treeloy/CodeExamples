// Eloy Salinas
// esalina1@ucsc.edu | 1243517
// Pa3
// Shuffle.java
import java.io.*;
import java.util.Scanner;
public class Shuffle {
	
	//Constructors
	
		//Pre-Conditions: L.getLength() == P.getLength(), 
		Shuffle(){
			
		}
	
	 public static void main(String[] args) throws IOException{
		 Scanner in = null;
		 PrintWriter out = null;
		 String line = null;
		 int i, m, lineNumber = 0;
		 String[] l = null; 
		 
		 if(args.length < 2){
	         System.out.println("Please specify both an input and output file ex: Shuffle infile outfile.");
	         System.exit(1);
	      }
		 
		 in = new Scanner(new File(args[0]));
		 out = new PrintWriter(new FileWriter(args[1]));
		 
		 line = in.nextLine();
		 int permu = Integer.parseInt(line);
		 
		 
		 for(int lines = 1; lines <= permu; lines++ ){
			
			 out.print("(");
			 lineNumber++;
			 line = in.nextLine();		 
			 l = line.split("\\s+");
			 m = l.length;
			 List P = new List();
			 for(i=0; i < m; i++){
		            P.insertBack(Integer.parseInt(l[i]));
		         }
			 List L = new List();
			 for(int j=1; j<=P.getLength(); j++){
					L.insertBack(j);
				}
			 List T = new List();
			 T = L.copy();
			 
			 shuffle(L, P);
			 int order = 1;
			 
			//System.out.println(L);
			
			 
			 out.print(L);
			 
				 while(!L.equals(T)){
					 shuffle(L,P);
					 order++;
					 //System.out.println(L);
				 }
				 
			 
				 
			 out.print(")  order=" + order);	 
			 
			 
			
			 
			
			//System.out.println(T);
			
			out.println();
			
			
				 
			 }
			 in.close();
			 out.close();
			 
		
		 }
	 
	 static void shuffle(List L,List P) {
		
		 
		 
		 L.moveTo(0);
		 P.moveTo(0);
		 int numleft = P.getLength();
		 
		 for(int len = 0; len < P.getLength(); len++){
			 int am = L.getCurrent();		 
			 int be = P.getCurrent() + numleft;		
			
				 L.moveTo(be-2); //would be -1 to not go off end but started index at 0
				 L.insertAfterCurrent(am);
				 L.deleteFront();
				 L.moveTo(0);
				 P.moveNext();
				 numleft--;
				 
			
		 }
		 
	}
	
}


