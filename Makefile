all: driver.cpp
	    icc -openmp driver.cpp -o driver
