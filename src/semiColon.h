#pragma once 
#include "base.hpp" 

class SemiColon : public Base {
public: 

SemiColon(Base* rightChild, Base* leftChild); 

bool evaluate(); 

std::string stringify(); 

std::string getClassType();  
}; 
