#include <iostream>
#include <fstream>
#include "node.h"
#include "tree.h"
#include "linklist_search.h"
#include "BST.h"

using namespace std;

int main()
{
    //to see BST uncomment the below part
        int time = 0, CPUBusy = 0, processIndex = 0, inserted = 1;
        ifstream processesFile; /* input file stream */
        processesFile.open("processes100.txt");
        int numProcesses; /* number of processes */
        processesFile >> numProcesses; /* read from file */
        Node* process=new Node[numProcesses]; /* create Node array */
        int processID, arrivalTime, runTime, priority;
        for (int i = 0; i < numProcesses; ++i) {
            /* read 4 integers from file */
            processesFile >> processID >> arrivalTime >> runTime >> priority;
            /* construct i'th element of the array */
            process[i] = Node(processID, arrivalTime, runTime, priority);
        }
        processesFile.close();
        linklist_schedule(time,CPUBusy,processIndex ,inserted,process,numProcesses);

        //tree start
        time=0;CPUBusy=0;processIndex=0;
        ifstream processesFile1;
        processesFile1.open("processes100.txt");
        numProcesses=0;
        processesFile1 >> numProcesses;
        Tree* process_tree=new Tree[numProcesses];
        for (int i = 0; i < numProcesses; ++i) {
            processesFile1 >> processID >> arrivalTime >> runTime >> priority;
            process_tree[i] = Tree(processID, arrivalTime, runTime, priority);
        }
        processesFile.close();
        BST_schedule(time,CPUBusy,processIndex , process_tree, numProcesses);

    return 0;
}

