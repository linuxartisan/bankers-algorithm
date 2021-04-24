#ifndef DEADLOACKDETECTOR_H
#define DEADLOACKDETECTOR_H

#include <string>
#include <vector>
#include <queue>

class DeadlockDetector
{
private:
    unsigned numProc;    // # of processes
    unsigned numResType; // # of resource types 
    std::vector<int> E_exist; // # of resources of each type in existence
    std::vector<std::vector<int>> C_alloc; // the current allocation matrix C
    std::vector<std::vector<int>> R_request_need; // the request matrix R
    std::vector<int> A_work;  // the available resources matrix A
    std::vector<bool> finish; // the finish vector
    std::queue<int> sequence; // safe sequence
    bool init; // set to true if constructor reads the input file, false otherwise

    void printVector(std::vector<int> v);
    void printMatrix(std::vector<std::vector<int>> v);
    void updateAvailableResources();
    bool isDone();
    bool enoughResourcesAvailable(int idx);
    void printSafeSequence();

public:
    DeadlockDetector(std::string filename);
    void run();

};

#endif
