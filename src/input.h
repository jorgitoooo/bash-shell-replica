#pragma once

#include "decorator.h"


class Input : public Decorator {
public:

Input(Base* child);

bool evaluate();

std::string stringify();

std::string getClassType(); 
};

