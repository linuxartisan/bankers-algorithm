#include <iostream>
#include <fstream>
#include <sstream>
#include "DeadlockDetector.h"

using namespace std;

/*
 * Class constructor
 * Instantiates all members by reading input file "filename".
 */
DeadlockDetector::DeadlockDetector(string filename)
{
    // try opening the file
    ifstream infile(filename);
    if (!infile.is_open()) {
        cout << "Cannot open file\n";
        this->init = false;
        return;
    }

    string line;
    int num;

    // read line 1
    getline(infile, line);
    istringstream ss(line);
    ss >> this->numProc >> this->numResType;

    // read line 2
    getline(infile, line);
    ss.str(line); // give line to ss
    ss.seekg(0);  // set ss to begining of line
    for (int i = 0; i < this->numResType; i++)
    {
        ss >> num;
        this->E_exist.push_back(num);
    }

    // read allocation matrix C
    for (int i = 0; i < this->numProc; i++)
    {
        getline(infile, line);
        ss.str(line);
        ss.seekg(0);
        vector<int> procAlloc;

        for (int j = 0; j < this->numResType; j++)
        {
            ss >> num;
            procAlloc.push_back(num);
        }

        this->C_alloc.push_back(procAlloc);
    }

    // read request matrix R
    for (int i = 0; i < this->numProc; i++)
    {
        getline(infile, line);
        ss.str(line);
        ss.seekg(0);
        vector<int> procReq;

        for (int j = 0; j < this->numResType; j++)
        {
            ss >> num;
            procReq.push_back(num);
        }

        this->R_request_need.push_back(procReq);
    }

    // initialise remaining members
    // available vector
    for (int i = 0; i < this->numResType; i++)
    {
        this->A_work.push_back(0);
    }

    // finish vector
    for (int i = 0; i < this->numProc; i++)
    {
        this->finish.push_back(false);
    }

    // init
    this->init = true;

    infile.close();
}

/*
 * run the simulation
 */
void DeadlockDetector::run()
{
    if (!this->init) {
        return;
    }

    cout << "Initial Values:\n";

    cout << "Number of processes: " << this->numProc << '\n';
    cout << "Number of resource types: " << this->numResType << '\n';

    cout << "Existing Resource Vector\n";
    printVector(this->E_exist);

    cout << "Current Allocation Matrix\n";
    printMatrix(this->C_alloc);

    cout << "Request Matrix\n";
    printMatrix(this->R_request_need);

    // (re)calculate available matrix A
    updateAvailableResources();

    cout << "Available Resource Vector\n";
    printVector(this->A_work);

    int idx = 0;      // the current process number
    int idxCount = 0; // # of processes that can't execute

    // loop until all processes are finished
    while (!this->isDone()) {
        // Find a process that can execute
        bool procFoundFlag = false;
        if (!this->finish[idx] && this->enoughResourcesAvailable(idx)) {
            procFoundFlag = true;
            idxCount = 0;
            // simulate execution of the process
            for (int i = 0; i < this->numResType; i++)
            {
                // allocated resources are freed
                this->C_alloc[idx][i] = 0;
            }

            // add freed resources to available
            updateAvailableResources();

            // mark process
            this->finish[idx] = true;
            cout << "\nProcess " << idx << " marked\n";
            sequence.push(idx);

            cout << "Available Resource Vector\n";
            printVector(this->A_work);
        }

        idx = (idx + 1) % this->numProc; // so idx loops back to 0

        // if current process can't be executed, increment the count
        if (!procFoundFlag) {
            idxCount++;
        }

        // if the count of processes that can't execute reaches total processes,
        // we have detected a deadlock
        if (idxCount == this->numProc) {
            break;
        }
    }

    if (!this->isDone()) {
        // system is in deadlock
        cout << "\nSystem is deadlocked\n";
        cout << "Deadlocked processes: ";
        for (int i = 0; i < this->numProc; i++)
        {
            if (!this->finish[i]) {
                cout << i << ' ';
            }
        }
        cout << '\n';
    } else {
        // system is not in deadlock
        cout << "\nSystem is in safe state\n";
        cout << "The safe sequence is:\n";
        this->printSafeSequence();
    }
}

/*
 * (re)calculate the available resources matrix A
 * adds freed resources to available
 */
void DeadlockDetector::updateAvailableResources()
{
    // store column-wise sum of C_alloc
    vector<int> colSum;

    for (int col = 0; col < this->numResType; col++)
    {
        int sum = 0;
        for (int row = 0; row < this->numProc; row++)
        {
            sum += this->C_alloc[row][col];
        }
        colSum.push_back(sum);
    }

    // update currently available resources
    for (int i = 0; i < this->numResType; i++)
    {
        this->A_work[i] = this->E_exist[i] - colSum[i];
    }
}

/*
 * determine whether all processes are finished
 */
bool DeadlockDetector::isDone()
{
    for (int i = 0; i < this->numProc; i++)
    {
        if (!this->finish[i]) {
            return false;
        }
    }

    return true;
}

/*
 * determine whether enough resources are avaliable
 * for process #idx to execute
 */
bool DeadlockDetector::enoughResourcesAvailable(int idx)
{
    vector<int> req_need = this->R_request_need[idx];

    for (int i = 0; i < this->numResType; i++)
    {
        if (req_need[i] > this->A_work[i]) {
            return false;
        }
    }

    return true;
}

/*
 * print the contents of the vector v
 */
void DeadlockDetector::printVector(std::vector<int> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        cout << v[i] << ' ';
    }
    cout << '\n';
}

/*
 * print the contents of the matrix v
 */
void DeadlockDetector::printMatrix(vector<vector<int>> v)
{
    for (int i = 0; i < v.size(); i++)
    {
        vector<int> row = v[i];

        for (int j = 0; j < row.size(); j++)
        {
            cout << row[j] << ' ';
        }
        cout << '\n';
    }
}

/*
 * print the safe sequence
 */
void DeadlockDetector::printSafeSequence()
{
    while (!this->sequence.empty()) {
        cout << this->sequence.front() << " ";
        this->sequence.pop();
    }
    cout << endl;
}
