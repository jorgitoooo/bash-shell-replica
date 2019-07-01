#pragma once
#include "base.hpp" 


class Decorator : public Base {
public: 

Base* child; 
std::string rightPath; 

Decorator(Base* child) 
{
	this->child = child; 
}

virtual std::string stringify()
{
	return this->child->stringify(); 
} 

virtual bool evaluate()
{
	return this->child->evaluate(); 
}

virtual std::string getClassType()
{
	return "decorator"; 	
}

};
