#pragma once

#include "decorator.h"


class OutputOverride : public Decorator {
public:

OutputOverride(Base* child);

bool evaluate();

std::string stringify();

std::string getClassType(); 
};

