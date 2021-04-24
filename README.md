# Bankers Algorithm
A C++ implementation of Banker's Algorithm for Deadlock Detection.

The program reads the information from a file. The file contains the number of processes, the number
of resource types, the number of resources of each type in existence, the current
allocation matrix (first row, followed by the second row, and so on), and the request matrix
(first row, followed by the second row, and so on).

#### Sample input file
```
4 5
2 4 1 4 4
0 1 1 1 2
0 1 0 1 0
0 0 0 0 1
2 1 0 0 0
1 1 0 2 1
0 1 0 2 1
0 2 0 3 1
```
All fields on a line are separated by whitespace.

The first line of the input file contains two integers, the number of processes and the number of
resource types. The next line contains the total number of resources of each type.
The next lines contain the data for the current allocation matrix (first row, followed by the
second row, and so on). There must be one line for each process in this matrix. The final set of
lines contain the data for the request matrix. Again, there must be one line for each process.


### Compile
```
$ g++ -Wall -std=c++11 -o main.o DeadlockDetector.cpp main.cpp
```
### Execute
```
Usage: ./main.o <input_file>
```

**Requirements**
- gcc


[![Contact me on Codementor](https://www.codementor.io/m-badges/sandesh87/im-a-cm-b.svg)](https://www.codementor.io/@sandesh87?refer=badge)
