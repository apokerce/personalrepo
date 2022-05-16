using namespace std;
void linklist_schedule(int time,int CPUBusy,int processIndex ,int inserted,Node* process,int numProcesses)
{
    Node* currentProcess = new Node;
    currentProcess=nullptr;
    Node* RQ = new Node();
    Node* temp = new Node();
    int counter=0;
    int search_c=0;
    while (processIndex < numProcesses || RQ->NextNode() != nullptr || CPUBusy == 1)
    {
        if (processIndex < numProcesses)
        {
            if (process[processIndex].arrivalTime == time)
            {
                inserted = 1;
                temp = RQ;
                while (inserted) //waits untill an insertion made
                {
                    //here is the starting point if next node is null it directly make head to point arrived process
                    if (temp->NextNode() == nullptr)
                    {
                        temp->InsertAfter(&process[processIndex]);
                        processIndex += 1;
                        inserted = 0;
                    }
                    //if linked list is not empty it looks for possible position of arrived process
                    //if next node priority is bigger than arrived priority so it needs to be inserted after current node.
                    else if (temp->NextNode()->priority > (process[processIndex].priority))  //&&
                    {
                        temp->InsertAfter(&process[processIndex]);
                        processIndex += 1;
                        inserted = 0;
                    }
                    else
                    {
                        temp = temp->NextNode();
                    }
                    counter+=1; //counter to count how many times searched in linked list
                }
            }
        }

        if (CPUBusy == 1)
        {
            currentProcess->runTime = (currentProcess->runTime) - 1;
        }
        if (currentProcess != nullptr)
        {

            if (currentProcess->runTime == 0)
            {
                cout << currentProcess->processID<<endl;
                CPUBusy = 0;
            }
        }
        if (CPUBusy == 0 && RQ->NextNode() != nullptr)
        {
            currentProcess = RQ->DeleteAfter();
            CPUBusy = 1;
            //for deletion linked list is in order so it directly takes node pointed by head
            search_c+=1;
        }
        time += 1;
    }
    cout<<endl<<search_c<<" nodes visited for searching"<<endl;
    cout<<counter<<" nodes visited for insertion"<<endl;
}
