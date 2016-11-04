# Author: Max Poole
# Class: CMSC441
# Date: 10/30/16
# Prof: Marron
import time
import sys


def file_read(file_name):
    """
    Read a file and turn it into a string
    :param file_name: string representing the name of a file
    :return: string of the file's contents
    """
    new_line = ""
    with open(file_name, 'r') as line_file:
        for row in line_file:
            new_line += row
    return new_line


def will_talk(first, second):
    """
    Helper function for seeing if two people in a line will talk to eachother
    :param first: Character representing the first person
    :param second: Character representing the second person
    :return: boolean value
    """
    return first == "H" and second == "G" or \
           first == "G" and second == "H" or \
           first == "W" and second == "T" or \
           first == "T" and second == "W"




def rebuild(j_matches):
    """
    Rebuilds a set of optimal pairs in a line folding
    Needs to be called after max_line_folds
    :param j_matches: 2D list created by max_line_folds
    :return: list of lists representing pairs
    """
    pairs = []
    my_stack = [(0, len(j_matches) - 1)]

    # while there are more items to visit
    while len(my_stack) > 0:

        i, j = my_stack.pop()

        if j - i <= 4: continue

        if j_matches[i][j] == -1:
            my_stack.append((i, j - 1))
        else:
            t = j_matches[i][j]
            if not j-4 > t >= i:
                continue
            pairs.append([t, j])
            my_stack.append((i, t - 1))
            my_stack.append((t + 1, j - 1))

    return pairs


def create_table(n):
    """Creates a table of n x n"""
    return [[0]*n for i in range(n)]


def max_line_folds(A):
    """
    Computes the max line folds
    :param A: list of characters H, G, T, and/or W
    :return: (max_val, j_matches) where max_val is a number representing the max number of line folds
            And j_matches is a 2D list that can be used with rebuild(j_matches) to get a list of the pairs that
            form the max line folding.
    """
    start = time.time()
    n = len(A)
    table = create_table(n)
    # This table lists what it is optimal for j to pair with for a given i
    # If it is optimal to not pair j at all, then the value at [i][j] will be -1
    j_matches = create_table(n)
    # k is the distance between the pairs of i and j
    # we calculate the smallest possible pairs first
    # then build out and have larger subproblem be composed of the smaller
    # subproblems we originally calculated
    for k in range(5, n):
        for i in range(n-k):
            j = i + k
            best_t = table[i][j-1]
            # We at first assume we will not match j
            j_match = -1
            for t in range(i, j-4):

                if will_talk(A[t], A[j]):
                    if t > i:
                        curr_t = 1 + table[i][t-1]  + table[t+1][j-1]
                    else:
                        curr_t = 1 + table[t+1][j-1]

                    if curr_t > best_t:
                        best_t = curr_t
                        j_match = t
            table[i][j] = best_t
            j_matches[i][j] = j_match
    end = time.time()
    print("time elapsed to calculate OPT(1,n)", end - start)
    return table[0][-1], j_matches


def main():
    if len(sys.argv) < 2:
        print("please provide a file containing the input")
        sys.exit()
    file_name = sys.argv[1]
    my_curr = file_read(file_name)
    #for i in [10]:#,20,50,100,500,1000,2000,5000,10000]:
    max_val, j_matches = max_line_folds(my_curr)
    print("OPT(1,n) is ", max_val)
    assert(len(rebuild(j_matches)) == max_val)
    print("Matched pairs are ")
    print(rebuild(j_matches))

main()