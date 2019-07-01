#pragma once

#include "decorator.h"


class Pipe : public Decorator {
private:
	Base* r_child;
public:

Pipe(Base* l_child, Base* r_child);

bool evaluate();

std::string stringify();

std::string getClassType(); 
};

