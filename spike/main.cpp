#include <iostream>
#include "MyClass.h"
using spike::MyClass;
int main(int argc, char **argv) {
	MyClass myClass;
	myClass.squareRoot(-15.0);
	std::cout << argc << argv;
}
