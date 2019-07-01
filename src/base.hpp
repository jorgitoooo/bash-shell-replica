#pragma once 
#include <iostream> 
#include <string> 

class Base {

public: 
Base* rightChild = nullptr; 
Base* leftChild = nullptr;  


virtual bool evaluate() = 0; 

virtual std::string stringify() = 0; 

virtual std::string getClassType() = 0; 

}; 
