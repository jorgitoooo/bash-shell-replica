#pragma once

#include "base.hpp" 


class And : public Base {
public: 

And(Base* leftChild, Base* rightChild); 

bool evaluate(); 

std::string stringify(); 

std::string getClassType(); 
}; 
