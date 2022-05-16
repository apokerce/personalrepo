using namespace std;
void BST_schedule(int time,int CPUBusy,int processIndex ,Tree* process_tree,int numProcesses)
{
    Tree* currentProcess = new Tree;
    Tree RQ_tree,*head=NULL;
    Tree* temp_tree = new Tree;
    int minvalue1;
    while (processIndex < numProcesses || (head!=nullptr)|| CPUBusy == 1)
    {
        if (processIndex < numProcesses)
        {
            if (process_tree[processIndex].arrivalTime1 == time)
            {
                //if the process is arrived first time code creates the node with arrived values
                if(processIndex==0 || head==nullptr)
                {
                    head=RQ_tree.Insert(head,process_tree[processIndex].processID1,process_tree[processIndex].arrivalTime1,process_tree[processIndex].runTime1,process_tree[processIndex].priority1);
                    processIndex += 1;
                }
                else
                {//after first arrive code inserts according to BST rule

                    RQ_tree.Insert(head,process_tree[processIndex].processID1,process_tree[processIndex].arrivalTime1,process_tree[processIndex].runTime1,process_tree[processIndex].priority1);
                    processIndex += 1;
                }
            }
        }
        if (CPUBusy == 1)
        {
            currentProcess->runTime1 = (currentProcess->runTime1) - 1;

        }
        if (currentProcess != nullptr && CPUBusy!=0)
        {
            if (currentProcess->runTime1 == 0)
            {
                cout << currentProcess->processID1<<endl;
                CPUBusy = 0;
            }
        }
        if (CPUBusy == 0 && (head!=nullptr))
        {
            Tree* temp_tree2 =new Tree;
            minvalue1=RQ_tree.FindMinValue(head);
            //founded node does not directly set to currentProcess since we delete that node, deleting without setting each value
            //cause currentProcess to be deleted
            temp_tree2=RQ_tree.getMinimumKey(head,minvalue1);
            currentProcess->processID1=temp_tree2->processID1;
            currentProcess->arrivalTime1=temp_tree2->arrivalTime1;
            currentProcess->runTime1=temp_tree2->runTime1;
            currentProcess->priority1=temp_tree2->priority1;
            RQ_tree.DeleteLeaf(head,temp_tree2->priority1);
            CPUBusy = 1;
        }
        time += 1;
    }
    cout<<endl<<RQ_tree.count_search<<" nodes visited for searching  "<<endl;
    cout<<RQ_tree.counterInsert<<" nodes visited for insertion "<<endl;


}
