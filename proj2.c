#include <iostream>
typedef enum{false, true}; bool;
int stack[1000];

int *sp;

#define push(sp, n) (*((sp)++) = (n))
#define pop(sp) (*--(sp))

bool will_talk(first, second) {
    return first == "H" && second == "G" ||
           first == "G" && second == "H" ||
           first == "W" && second == "T" ||
           first == "T" && second == "W";
}
int** rebuild(int** j_matches, int max_pairs) {
    int pairs[2][max_pairs];
    sp = my_stack;
    push(sp, 0);
    push(sp, j_matches.length-1);
    int stack_size = 1; //represent the number of i, j pairs, not total items in stack
    int i, j, pair_no = 0;
    //while there are more items to visit
    while (stack_size > 0) {

        j = pop(sp);
        i = pop(sp);
        stack_size--;
        if (j - i <= 4) {
          continue;
        }

        if (j_matches[i][j] == -1)
        {
            push(sp, i);
            push(sp, j-1);
        }
        else {
            t = j_matches[i][j];
            if (!(j - 4 > t && t >= i)) {
                continue;
            }
            pairs[pair_no][0] = t;
            pairs[pair_no][1] = j;
            pair_no++;
            push(sp, i);
            push(sp, t-1);
            stack_size++;
            push(sp, t+1);
            push(sp, j-1);
            stack_size++;
        }
    }
        return pairs;

}
int max_line_folds(int** A, int** j_matches) {
    int n = A.length;
    int table[n][n];

    int j, best_t, j_match;
    for (int k = 5; k < n; k++) {
        for (int i = 0; i < n -k; i++) {
            j = i + k;
            best_t = table[i][j-1];
            j_match = -1;
            for (int t = i; t < j-4; t++) {
                if (t > i) {
                    curr_t = 1 + table[i][t - 1] + table[t + 1][j - 1];
                }
                else {
                    curr_t = 1 + table[t + 1][j - 1];
                }

                if (curr_t > best_t) {
                    best_t = curr_t;
                    j_match = t;
                }
            }

            table[i][j] = best_t;
            j_matches[i][j] = j_match;
        }
    }
    return table[0][n-1];
}

