//evaluationlist.hpp
//evaluation list specification file
//@remarks used to hold a list of possible hands so that they can later be compared to the 
//	       players hands.

#include "hand.hpp"
#include <vector>
	using std::vector;

class EvaluationList
{
private:
	//create node that is to be used for the list
	struct LNode
	{
		Hand   hand;
		String handClassification;
		LNode* next = nullptr;
	};

	LNode* head = nullptr;
public:
	EvaluationList(); //will initialize list to common poker hands
	void   addToList(string classification, Hand& newHand); //used to add new hand to list
	string evaluateHand(const Hand& comparingHand) const; //returns if hand matches any hands in list
	bool   isEmpty() //returns true if list is empty
		   {return !head;}
    void   clear(); //clears list and deallocates memory
    ~EvaluationList() //calls clear method
    	   {clear();}
};

//will initialize list to common poker hands other than pairs
EvaluationList()
{
	vector<Hand> hands;
	//push back royal flush
	addToList("Royal Flush", Hand(vector<Card>{Card("Ace","Heart"), Card("King", "Heart"), Card("Queen", "Heart"), Card("Jack","Heart"), Card("Ten","Heart")}));
	addToList("Straight Flush", Hand(vector<Card>{Card("Ten","Heart"), Card("Nine", "Heart"), Card("Eight", "Heart"), Card("Seven","Heart"), Card("Six","Heart")}));
	addToList("Flush", Hand(vector<Card>{Card("King","Club"), Card("Queen", "Club"), Card("Eight", "Heart"), Card("Seven","Heart"), Card("Six","Heart")}));

}

//used to add new hand to list
void   addToList(string classification, Hand& newHand)
{
	LNode* newNode = new LNode{newHand,classification};
	LNode* current = head;

	if (isEmpty()) //if list is empty
	{
		head = newNode; //add new node to head of list
	}
	else
	{
		while (current->next) //move to the end of the list
		{
			current = current->next; //used to move to next node
		}
		current->next = newNode; //add new node at end of list

	}
}

//returns if hand matches any hands in list
string evaluateHand(const Hand& comparingHand) const
{

}

//clears list and deallocates memory
void   clear()
{
	LNode* oldNode = nullptr;

	while (!isEmpty())
	{
 		oldNode = head;
 		head = oldNode->next;
 		delete oldNode;
 	}
}





