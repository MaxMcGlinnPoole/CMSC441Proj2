//
// Created by Maxwell Poole on 11/5/16.
//

#include <iostream>
#include <stack>
#include <omp.h>
#include <stdio.h>
#include <ctime>
#include <fstream> 
#include <string>
#include <cstdlib>

using namespace std; //I know, forgive me

 bool will_talk(char first, char second) {
     return (first == 'H' && second == 'G') ||
            (first == 'G' && second == 'H') ||
            (first == 'W' && second == 'T') ||
            (first == 'T' && second == 'W');
 }
/*
void rebuild(int** j_matches, int max_pairs, int n, int** pairs) {
    int pair[2];
    stack<int[2]> sp;
    sp.push({0,n - 1});
    int stack_size = 1; //represent the number of i, j pairs, not total items in stack
    int i, j, t, pair_no = 0;
    //while there are more items to visit
    while (!sp.empty()) {
        int pair[2] = sp.pop();
        j = pair[1];
        i = pair[0];
        if (j - i <= 4) {
          continue;
        }
        if (j_matches[i][j] == -1) {
            sp.push({i, j-1});
        }
        else {
            t = j_matches[i][j];
            if (!(j - 4 > t && t >= i)) {
                continue;
            }
            pairs[pair_no][0] = t;
            pairs[pair_no][1] = j;
            pair_no++;
            sp.push({i, t-1});
            stack_size++;
            sp.push({t+1, j-1});
            stack_size++;
        }
    }
}
*/

void zero_out(int** table, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            table[i][j] = 0;
        }
    }
}
int max_line_folds(char* A, int** j_matches, int n) {
    int *table[n];
    for (int i = 0; i < n; i++) {
        table[i] = new int[n];
    }
    zero_out(table, n);
    int curr_t;
    int j, best_t, j_match;
    for (int k = 5; k < n; k++) {
        #pragma omp parallel for
        for (int i = 0; i < n -k; i++) {
            //int thread_id = omp_get_thread_num();
            //printf("thread no %d\n", thread_id);
            j = i + k;
            best_t = table[i][j-1];
            j_match = -1;
            for (int t = i; t < j-4; t++) {
                if (will_talk(A[t], A[j])) {
                    if (t > i) {
                        curr_t = 1 + table[i][t - 1] + table[t + 1][j - 1];
                    } else {
                        curr_t = 1 + table[t + 1][j - 1];
                    }

                    if (curr_t > best_t) {
                        best_t = curr_t;
                        j_match = t;
                    }
                }
            }

            table[i][j] = best_t;
            j_matches[i][j] = j_match;
        }
    }
	int m = table[0][n-1];
	for (int i = 0; i < n; i++) {
        delete table[i];
    }
    return m ;
}

float avg(float* f, int n){
	float runTotal = 0.0;
	for(int i = 0; i < n; i++)
		runTotal += f[i];
	return (runTotal / (float)n);
};

int main() {
	
	//time_t bgn, end; 
	int num_trials = 1; 
	int STR_LEN_INDX = 1;
	int STR_LEN [1] = {
		//10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 500, //11
		//1000, 1250, 1500, 1750, 2000, 2250, 2500, 2750, //+8
		//3000, 3250, 3500, 3750, 4000, 4250, 4500, 4750, //+8
		//5000, 5250, 5500, 5750,6000, 6250, 6500, 6750, 7000, 8000, 
		9000 };//+8	
	char inFileName[301] = 
		//"//afs//umbc.edu//users//m//a//maebig1//home//CMSC341//dna//multi//dna.txt";
   "/home/maxpool1/CMSC441/test1.txt"; 
	
	ifstream inFile;
	inFile.open(inFileName, ios::in);
	if(!inFile) exit(1);
	string full_str, part_str;
	inFile >> full_str;
	inFile.close();
    int num_threads;
    #pragma omp parallel 
    {
        num_threads = omp_get_num_threads();
    
    }printf("threads: %d\n", num_threads);	
	
	
	cout 
		<<"\nUp to STR_LEN[" << STR_LEN_INDX 
		<<"] = " << STR_LEN[STR_LEN_INDX] 
		<< "\nNumber of runs per length: " << num_trials
		<< "\n\nPairs\tLength\tavg time(sec)\n";
	
	//for each element in STR_LEN array
	for(int indx = 0; indx < STR_LEN_INDX; indx++){	
		//cout << endl;
		int n = STR_LEN[indx];
		size_t const MAX = n;
		int *j_matches[n];
		char test[MAX];
		float results[num_trials];
	
		
		//partition off a sub string of the large
		// 9000 long one we made
		part_str = full_str.substr(0, n);
		part_str.copy(test, MAX);
		
		
		for (int i = 0; i < n; i++) {
			j_matches[i] = new int[n];
		}
		
		int maximum = 0;
		
		for(int jndx = 0; jndx < num_trials; jndx++){
			zero_out(j_matches, n);
			double bgn = omp_get_wtime();
			maximum = max_line_folds(test, j_matches, n);
			results[jndx] = (float)(omp_get_wtime() - bgn);
		}
			
		
		float a = avg(results, num_trials);
		
		cout << maximum 
			<< "\t" << n 
			<< "\t" << a << endl;
		 
		 
		part_str.clear();
		for (int i = 0; i < n; i++) {
			delete j_matches[i];
		}
		//delete  j_matches;
	}//end for  indx
	
	
	
}


/*

open MP tutorial on yt
https://www.youtube.com/playlist?list=PLLX-Q6B8xqZ8n8bwjGdzBJ25X2utwnoEG




http://hpcf.umbc.edu/how-to-run-programs-on-maya/




https://www.csee.umbc.edu/courses/undergraduate/441/fall16_marron/projects/








*/
//EoF
