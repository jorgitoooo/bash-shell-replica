#include "or.h"


Or::Or(Base* leftChild, Base* rightChild)
{
        this->leftChild = leftChild;
        this->rightChild = rightChild;
}

bool Or::evaluate()
{

	//Check for weird input
        if (leftChild == NULL || rightChild == NULL)
        {
                std::cout << "Error on input. Check you have valid number of connectors/commands." << std::endl;
                exit(EXIT_FAILURE);
        }

	bool leftChildStatus = leftChild->evaluate();

	//Just need one child for an '||' connector to return true to return true up to parent node. 
        if (leftChildStatus == true)
        {
		return true; 
        }
	
	//Return execution of right child if left child is false. Will be true or false. 
	else {
		return rightChild->evaluate(); 
	}
	

}

std::string Or::stringify()
{

        return this->leftChild->stringify() + " || " + this->rightChild->stringify();

}

std::string Or::getClassType() 
{
	return "connector";  
}
