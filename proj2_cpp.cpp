//
// Created by Maxwell Poole on 11/5/16.
//

#include <iostream>
#include <stack>
#include <omp.h>
#include <stdio.h>
using namespace std; //I know, forgive me
char test[] = 
"WGHTHTWTWHHWTHWHGHHTHWTWTTHGHGTTGGGTGGWTTTGHWGGTTHHTGWHHWHWHTHGHGGGHHHGWWGHWTHWGHGTTTWTGTWHHGGWHWWGTHHWWHWHWHTGTTTHWTWHTHHGGTHGHTGTWGWGTHTGGTTWHTWWGWWGWTWTHTHWTWTWGWHHHGTGTHGHWTWTTTGTHHTTGHWGWHGWGHWWWWTHGTHHHWGGTWGWWWTTWTGWHGHWTWWHTHGTHTHTTWWHWHTGTHGHHWGHTWHGGHHHWTTGHGGHWWGGGTGTGWGHGHGGGGHWWWHGGGHGHHGGWGHWGGHTTWTGWHTHGTGWWWWHTHGGGHGWGTTGWTHGGGWTGHGWTWGGWHWHTWHTTWTHTGTTGHHHHTHTWWHGTWTWGGWHTTWGHHTHGGWGWTTTTWWHTWTHGGHHWTHTHTGTGGGWWHWHGGGGWGWHHWWHGWGHGHWGWHGGGHTHTTGGHWGGHWWWHGGTTWTGGGTGGWTWGGHTWHTHHHHHHWWWHGWGWGTWTHTHGTHTTTHHGHGHGWTGHGGHHWHGHHGHWWGTTTTWGHWWHTGGHHWHTHWTWTGGHGWHWTHHGWGHHGWWTHWHHWHGGHWWGTHWGTTGGHWTTHWGGWHHWWTTGHGWGGGGGGTTWHWTHWTHTTHTHHHHHWGGGTWHWTHGWTWWHWWTGGGTGHWGTTGWGHWGWTTHGTWWTHWHWWTHHWHTTGHHTWTWTTHWWGTTGHGGGWWGTWTTTTHTWTWWTWWTGTHHTTWTTHGWTWGHTGGWTWHGHHWHHHWHGGWWTWTGWGWGWTWHGGGTWTWHHTGWTGHTWGGHTWGWHGHGTHGWHTTGHWWGHGHGTWHWWGTTTWTWGHTGGHGHTHHTGTHTWWHHWGWGGHHHWGHTHWGWGGHGGHTGHHGTWTTGWGHHHTWTHHWWHHHGGWHHWWHWTWHGGTTGGHTHWWHGTWHGTTTHTWGTGWTHGWTTWWHGGGHWTGGTHGGWHHHGWHTWTGHHHHGGGWWHGWWWHHGHWWTTGTTHWWGTGTTHTGWHGGGWHHHGTWGGGHTTGGWWWGTGWTGGGGTGGTGHWTGGHHGGHWWTHHHGGWGTWHGHHGHWHWWHWHTTTHWTTTTHHHGHWWTWTHGHWWTTTTHWWHGGHHGWHWHHGTHTHHTGGHWHTWGGGGTWWTHTGTHHTHWGHTWGTWHWTTTGWWHTHWGTGTTHHHTHGTTGTGWTWWGTGTWHGWWTHGHHWHHGTTGWTWWHWWWHHGHWGGHWWTHWWTTTHTHGWTGTTWGHGWWTTTGWTTGHHTTHGHTHWGWHWHTGHGGHHTHTTGTHGHHGTGHHHGTWGWHHGWTWGTHGWGTGHGWTWWTHTWHHHTGTHGGWWTTHTGGTTHTHTTWTTTHHWTGTWHHGGGTWGTWTWGWTTGGWHGWTWHGHWTWTTGTHHGTTGWTWGHWWWWWWHHTHHGWHHHWTTHGHGGHHGGTTHGGGHTGHTGHWGTWWTTHWGTGTGHHTGWHGWHTWGHWHWHHGGHWTGGWWHWTHWHTWWHHHTTWGGWHHTGWGWHGGWGTHHHWGTTTWGTGGGGWGHGTWGHWTTHGGTGWWWTHGTGTGHGGWHWHHTGTHGHTGTWHGTWTWWHGHGGGHTWWGWTHTWHWTHHGHHGTHGWWHTTTTWTGHWTTTWTGWWHHTHGTGGWTWWWHWWWHTHWHGGGWWGTTHWWGGWGTWTGHTGGHHGHGTTGGHHTTHTGGTHHHHTHHGHHGTTGGHWWTTHGGTHWHGTGWTTWWTGGWTGTHTGTWHHHHTWHHWTWGTTGGHHWHTGTTTGGGWGGWGGWWHWTGTGWTTGHGWTWWTTTGTWTHGHHWHGWWTWWHTHHGTHGGHTTGGWGHTGGTTWTWWGGTHWTHHTWTWTTHWTHWTGHHTGGGTWWTGHGGWTGTTTTHGTWTHHHTGTWGTTHHTTTTWHHTWWWWWHGTTWHWWWGWGTWTTTTHGTHHHWWHTTWWWGHGHGWHWWTHWGWWWGTWWGWWHTTHGWWWWW";
bool will_talk(char first, char second) {
    return first == 'H' && second == 'G' ||
           first == 'G' && second == 'H' ||
           first == 'W' && second == 'T' ||
           first == 'T' && second == 'W';
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
            int thread_id = omp_get_thread_num();
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
    return table[0][n-1];
}

int main() {
    int n = 2000;
    int *j_matches[n];
    for (int i = 0; i < n; i++) {
        j_matches[i] = new int[n];
    }
    zero_out(j_matches, n);
    int maximum = max_line_folds(test, j_matches, n);
     printf("max pairs %d\n", maximum);
}
