using namespace std;
class Node {
private:
	Node* next;
public:
	int processID, arrivalTime, runTime, priority;
	Node(const int& item1, const int& item2, const int& item3, const int& item4, Node* ptrNext = 0);
	Node(Node* ptrNext = 0);
	void InsertAfter(Node* p);
	Node* DeleteAfter(void);
	Node* NextNode(void) const;
};
Node::Node(Node* ptrNext)
{
    next = ptrNext;
}
Node::Node(const int& item1, const int& item2, const int& item3, const int& item4, Node* ptrNext)
{
	next = ptrNext;
	processID = item1;
	arrivalTime = item2;
	runTime = item3;
	priority = item4;
}

Node* Node::NextNode(void) const {
	return next;
}

void Node::InsertAfter(Node* p) {
	p->next = next;
	next = p;
}
Node* Node::DeleteAfter(void) {
	Node* tempPtr = next;
	if (next == nullptr)
		return nullptr;
	next = tempPtr->next;
	return tempPtr;
}




