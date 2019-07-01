#pragma once

#include "base.hpp" 

class Or : public Base {
public: 

Or(Base* leftChild, Base* rightChild); 

bool evaluate(); 

std::string stringify(); 

std::string getClassType(); 

}; 
