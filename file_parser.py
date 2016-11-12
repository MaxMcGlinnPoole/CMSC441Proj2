"""
Converts output file from the cpp code into a csv file for use with excel
"""
import sys

def parse(file_name):
    new_line = ""
    new_file_name = ""
    with open(file_name, 'r') as my_file:
        row_num = 0
        for row in my_file:
            #print(row)
            if (row_num == 0):
                new_file_name += row.split(":")[1].strip() + "thread_"
                print(new_file_name)
            if (row_num == 3):
                print(row)
                new_file_name += row.split(":")[1].strip() + "runs.csv"
                print(new_file_name)
                break
            row_num += 1
    with open(new_file_name, 'w') as new_file:
        with open(file_name, 'r') as my_file:
            row_num = 0
            new_file.write("N, time(sec)\n")
            for row in my_file:

                if (row_num > 5):
                    vals = row.split(" ")

                    vals = filter(None, vals)

                    new_row = vals[1] + "," + vals[2]
                    print(new_row)
                    new_file.write(new_row)
                row_num += 1

def main():
    if len(sys.argv) < 2:
        print("please provide a file containing the input")
        sys.exit()
    file_name = sys.argv[1]
    parse(file_name)
main()