#pragma once

#include "decorator.h"


class OutputAppend : public Decorator {
public:

OutputAppend(Base* child);

bool evaluate();

std::string stringify();

std::string getClassType(); 
};

