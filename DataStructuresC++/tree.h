using namespace std;
class Tree
{
    private:
        Tree *left;
        Tree *right;
    public:
        int processID1, arrivalTime1, runTime1, priority1,counterInsert=0,count_search=0;
        Tree(Tree* lptr=nullptr,Tree* rptr=nullptr);
        Tree(const int& item11, const int& item21, const int& item31, const int& item41,Tree* lptr=nullptr,Tree* rptr=nullptr);
        Tree* Left(void) const;
        Tree* Right(void) const;
        Tree* Insert(Tree* head, int processID11, int arrivalTime11, int runTime11, int priority11) ;
        int FindMinValue(Tree* p12);
        void DeleteLeaf(Tree*&p13,int minvalue);
        Tree* getMinimumKey(Tree* temp,int minvalue2);

};
Tree::Tree(Tree* lptr,Tree* rptr)
{
    left=nullptr;
    right=nullptr;
}
Tree::Tree(const int& item11, const int& item21, const int& item31, const int& item41,Tree* lptr,Tree* rptr):
left(lptr),right(rptr),processID1(item11),arrivalTime1(item21),runTime1(item31),priority1(item41)
{}
Tree* Tree::Left(void) const
{
    return left;
}
Tree* Tree::Right(void) const
{
    return right;
}
//since min value is putted on left in BST code directly goes leftmost node of the tree to find min value.
int Tree::FindMinValue(Tree* p12)
{
    while(true)
    {
        if(p12->Left()==nullptr)
            break;
        else
            p12=p12->Left();
    }
    return p12->priority1;
}
//Below code is inspired from https://gist.github.com/harish-r/a7df7ce576dda35c9660
Tree* Tree::Insert(Tree* head, int processID11, int arrivalTime11, int runTime11, int priority11)
{    // if it is first time a process arrived so head is null code creates new tree
    if (head==NULL)
    {
        counterInsert+=1;
        head = new Tree(processID11,arrivalTime11,runTime11,priority11);
        return head;
    }
    //checks if arrived priority is less than or higher than current node if it is higher puts on right if less puts on left.
    if (priority11 < head->priority1)
    {
        counterInsert+=1;
        head->left = Insert(head->left, processID11,arrivalTime11,runTime11,priority11);
    }
    else
    {
        counterInsert+=1;
        head->right = Insert(head->right, processID11,arrivalTime11,runTime11,priority11);
    }
    return head;
}
//finds the minumum node in the tree by comparing the priority values with found minvalue in findminvalue function
Tree* Tree::getMinimumKey(Tree* temp,int minvalue2)
{
    while(temp!=nullptr && temp->priority1!=minvalue2)
    {
        count_search+=1;
        if(temp->priority1<minvalue2)
            temp=temp->right;
        else
            temp=temp->left;
    }
    count_search+=1;
    return temp;
}
void Tree::DeleteLeaf(Tree*&p13,int minvalue)
{
    Tree* head_node=nullptr;
    Tree* temp=p13;
    //in here head of min node is found so that we can preserve if it has right node
    while(temp!=nullptr && temp->priority1!=minvalue)
    {

        head_node=temp;
        if(temp->priority1<minvalue)
            temp=temp->right;
        else
            temp=temp->left;
    }
    //if node has no left right node then we set node's left right nodes to null before the searched node
    if(temp->left==nullptr && temp->right==nullptr )
    {
        if(temp!=p13)
        {
            if(head_node->right==temp)
                head_node->right=nullptr;
            else if(head_node->left==temp)
                head_node->left=nullptr;

        }
        else {
          //if node is head node then we directly set to nullptr
            p13=nullptr;
        }
    }
    else
    {//here we preserve the right node of the min node (if it exists) since min node will always be leftmost for this application there will be no left node for min node
        Tree* right_node=temp->right;;
        if (temp != p13)
        {
            if (temp == head_node->left) {
                head_node->left = right_node;

            }
            else {
                head_node->right = right_node;

            }
        }
        else{
            p13=right_node;
            }
    }

}




