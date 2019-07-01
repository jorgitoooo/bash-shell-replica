#include "and.h" 


And::And(Base* leftChild, Base* rightChild) 
{
	this->leftChild = leftChild; 
	this->rightChild = rightChild; 
} 

bool And::evaluate() 
{
	//Check for weird input
	if (leftChild == NULL || rightChild == NULL) 
	{
		std::cout << "Error on input. Check you have valid number of connectors/commands." << std::endl;
		exit(EXIT_FAILURE); 
	}


	bool leftChildStatus = leftChild->evaluate(); 

	//grab right command's return status if the left command succeeded (is true)
	if (leftChildStatus == true) 
	{
		bool rightChildStatus = rightChild->evaluate(); 
		return rightChildStatus; 
	}

	//automatically return false because left command failed, and the && connector runs right command IFF left command suceeds. 
	else {	
		return false; 
	}


}

std::string And::stringify() 
{

	return this->leftChild->stringify() + " && " + this->rightChild->stringify();

}

std::string And::getClassType() 
{
return "connector"; 
}
