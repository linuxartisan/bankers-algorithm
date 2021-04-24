// https://www.geeksforgeeks.org/bankers-algorithm-in-operating-system-2/
#include <iostream>
#include "DeadlockDetector.h"

using namespace std;

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        cout << "Invalid number of arguments!\n";
        cout << "Usage: " << argv[0] << " <input_file>\n";
        return -1;
    }

    DeadlockDetector detector(argv[1]);
    detector.run();

    return 0;
}
