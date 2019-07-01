#include "semiColon.h"


SemiColon::SemiColon(Base* leftChild, Base* rightChild)
{
        this->leftChild = leftChild;
        this->rightChild = rightChild;
}

bool SemiColon::evaluate()
{
	bool leftChildStatus = this->leftChild->evaluate(); 

	if (this->rightChild == NULL) 
	{
		return leftChildStatus;
	}

	bool rightChildStatus = this->rightChild->evaluate(); 

	return rightChildStatus; 


}

std::string SemiColon::stringify()
{
	if (this->rightChild != NULL) 
	{
		return this->leftChild->stringify() + "; " + this->rightChild->stringify();
	}

	else {
		return this->leftChild->stringify(); 
	}
}

std::string SemiColon::getClassType() 
{
	return "connector"; 
}
