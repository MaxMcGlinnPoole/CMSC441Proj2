/*

CMSC 441, Introduction to Algorithims
Fall 16, Dr. Marron


Author: Michael Aebig
Project 1, Dynamic Programming (DP).


This program uses an input file containing a single, one line string.  The

string must be comprised of only {H,G,W,T} characters.  OPTij() is a recursive

function which navigates through pairs of indices, starting from a 'center'

and working 'outwards'.  This is the general idea behind DP, where we 

identify subproblems, and the smallest of such, and solve them from 

the bottom up, using a table to look up previously solved sub problems.


int main() will test OPTij() by taking in the string from the input file to 

create a substring of 32 varying lengths, ranging from 10 to 5000.  Each substring 

is treated by OPTij() 100 times, with the elapsed time recorded.  The average time

is computed before moving on to the next, larger, substring.  


The times will be copied from the gl terminal and a multilinear regression

model will be fit to the data using a python program I did eariler this 

semester for CMSC 455.  We can obtain visuals via MS Excel.

*/




#include<string>
#include<fstream>
#include<iostream>
#include<cstdlib>
#include<ctime>

      ///////////////////////////////////////////////////
     ///////////////////////////////////////////////////
    unsigned int len = 0;   ///////////////////////////
   /////////////////////////////////////////////////// 
  ///////////////////////////////////////////////////

  
  
const unsigned int FILENAMELENGTH = 301;
const char inFileName[FILENAMELENGTH] = 
//        "test1.txt";
//		"//afs//umbc.edu//users//m//a//maebig1//home//CMSC341//dna//test1.txt";
        "//afs//umbc.edu//users//m//a//maebig1//home//CMSC341//dna//test2.txt";




// Check two indicies to see if they are a matching pair
bool isPair(std::string & s, unsigned int a, unsigned int b ) {
    if(s[a] == 'H'){
        if(s[b] == 'G')
            return true;
    }else if(s[a] == 'G'){
            if(s[b] == 'H')
                return true;
    }else if(s[a] == 'W'){
            if(s[b] == 'T')
                return true;
    }else if(s[a] == 'T'){
            if(s[b] == 'W')
                return true;
    } 
    return false;
}







unsigned int max(unsigned int x, unsigned int y){
    if(x >= y) return x;
    return y;
}






// Checks to see if an x,y pair is in a look up table
int lookUp(int** t, unsigned int x, unsigned int y) {
    if(x < len && y < len) {
        if (x >= 0 && y >= 0){
            return t[x][y];
        }
    }
    return 0;
}








// Puts an x,y entry pair in the table
void insert(int **t, unsigned int x, unsigned int y, unsigned int v) {
    if(x < len && y < len) {
        if(x >= 0 && y >= 0) {
            t[x][y] = (int)v;
        }
    }
}





///////////////////////////////////////////////////////////////////////////////
// The main recursive fn for the program.  It requires a look up table, a string
// and a start and ending index.  Some base cases are in the beginning, otherwise 
// it will call itself either once or twice, depending on if the upper index, j 
// is a part of a pair. If j is in a pair, then we iterate through all the 
// the elements it could possibly pair with, t, and call OPTij for each element
// that is before j and after i, but on either side of t. It j is not in a pair,  
// then we simply call  OPT with i and j - 1.  
// In order to filter out cases where indicies are out of bounds,
//  we use intermidiate variables for storing recursivly found values, and 
// operate on those values directly.//////////////////////////////////////////
unsigned int OPTij(int** table, std::string & s, unsigned int i, unsigned int j) {
    if(j > s.length()) 
        return -1;
    if(j <= i) 
        return -1;
    int temp3 = ((int)j - 4);
    if((int)i >= temp3) { 
        return 0;
    } else {
        if(lookUp(table, i, j) >= 0) {
            if(isPair(s, i, j))
                return (lookUp(table, i, j));
            else {
                if((unsigned int)lookUp(table, i, j) > len || lookUp(table, i, j) == 0) 
                    return 0;
                else 
                    return (lookUp(table, i, j));
            }
        } else {
            unsigned int temp1 = 0;
            unsigned int temp2 = 0;
            for(unsigned int t = i; (int)t < temp3; t++) {
                if(isPair(s, j, t)) {
                    unsigned int Tsit = OPTij(table, s, i, t - 1);
                    unsigned int Tstj = OPTij(table, s, t + 1, j - 1);
                    if(Tsit > s.length() && Tstj > s.length()) temp1 = 1;
                    if(Tsit > s.length()) Tsit = 0;
                    if(Tstj > s.length()) Tstj = 0;
                    temp1 = max(temp1, 1 + Tsit + Tstj);
                }
            }
            unsigned int T2 = OPTij(table, s, i, j - 1);
            temp2 = max(temp2, T2);
            insert(table, i, j, max( temp1, temp2));
            return max( temp1, temp2);
        }//end else
    }//end if else 
}//end f.OPTij











int main() {
    std::ifstream iFile;
    std::string totalStr;
    iFile.open(inFileName, std::ios::in);
    if(!iFile) exit(1);
    iFile >> totalStr;
    iFile.close();

    //unsigned int totalLength = totalStr.length();
    unsigned int loopLength = 4;
    unsigned int totalPairs = 0;
	//intermediate value for i + j
    unsigned int inter = 0;
	//we will run each size 100 times
    unsigned int numTrials = 1;
	//trials[] will hold the resulting number of pairs
	// for each of the 32 size sub strings
    unsigned int trials[32] = {};
    unsigned int sizes[32] = {10,20,30,40,50,60,70,80,90,100,
                    200,300,400,500,600,700,800,900,
                    1000,1100,1200,1300,1400,1500,1600,
                    1700,1800,1900,2000,3000,4000,5000};

	//how many of the above sizes do we want to use?
    for(unsigned int counter = 0; counter < 30; counter++) {
		
		//len is a global variable
        len = sizes[counter];
        
        double* times = new double[numTrials];

		//iterate through a size string numTrials times
        for(unsigned int index = 0; index < numTrials; index++) {
            times[index] = 0.0;
            std::string tempStr = totalStr.substr(0, len);

			//table will hold the values for memoization
            int** table = new int*[len];
            for(unsigned int i = 0; i < len; i++) {
                table[i] = new int[len];
                for(unsigned int j = 0; j < len; j++) {
                    table[i][j] = -1;

                }
            }

            time_t t_begin, t_end, t_diff;
            t_begin = (time(NULL));

			//here is the start of the recursion, most of the program is
			// is in this for loop.  one pass through the entire string,
			// excluding either edge, which is trimmed off due to the 
			// no folds under a length of 4 property.  This loop finds
			// out which of the largerst problems yields the best result.
			// OPTij() traverses to the deepest level and builds the soltn
			// back up to be returned here.
			// This for loop could  be implemented in a bootstrap manner, 
			// however having it in int main() will do just fine for us.
            for(unsigned int i = loopLength + 1; i < tempStr.length(); i++) {
                for(unsigned int j = 0; j < tempStr.length() - i; j++) {
                    inter = i + j;
                    totalPairs = OPTij(table, tempStr, j, inter);
                }//end for j
            }//end for i

            t_end = (time(NULL));
            t_diff = (t_end - t_begin);

            times[index] = (double)t_diff;

            for(unsigned int i = 0; i < len; i++)
                delete[] table[i];
            delete[] table;

			//std::cout << "\n run time: " << t_diff << std::endl;
			//std::cout << " total pairs: " << totalPairs << std::endl;
            
        }//end for index

        trials[counter] = totalPairs;
        double runTime = 0.0;
        for(unsigned int idx = 0; idx < numTrials; idx++) 
            runTime += times[idx];
        
        runTime /= (double)numTrials;
        std::cout << "\n Running time avg: " << runTime << std::endl;
        std::cout << " Number of pairs: " << trials[counter] << std::endl;
        std::cout << " String length: " << len << std::endl << std::endl;
        delete [] times;
    }//end for counter

    system("PAUSE");
    return 0;
}//end f.main





/*

Some helpful resources 

https://www.youtube.com/playlist?list=PLFDnELG9dpVynxWOGI9F1QSAEYFQg_2LY

https://www.youtube.com/playlist?list=PLLH73N9cB21W1TZ6zz1dLkyIm50HylGyg




//////////////////////////////////////////
  the Makefile      /////////////////////
////////////////////////////////////////

FLAGS = g++ -ansi -Wall

FLAGS1 = g++ -ansi -Wall -std=c++0x

OBJSO = driver.o  
OBJSC = driver.cpp 

all: $(OBJSO)
	cat /dev/urandom | tr -dc 'HGWT' | fold -w 5000 | head -n 1 > test2.txt
	$(FLAGS) $(OBJSO) -o driver.out
	$(FLAGS) $(OBJSO) -o Driver.out


driver.o: driver.cpp
	$(FLAGS) driver.cpp -c

run:
	make all
	./driver.out

Driver:
	./driver.out

driver:
	make all
	./driver.out

val:
	clear
	make all
	valgrind --leak-check=full ./driver.out

cat:
	cat Makefile

clean:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#

cls:
	rm -rf *.o
	rm -f *.out
	rm -f *~ *.h.gch *#
	clear
	ls -l















EoF*/