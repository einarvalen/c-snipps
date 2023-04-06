/*
 * MyClass.h
 *
 *      Author: einar
 */

#ifndef MYCLASS_H_
#define MYCLASS_H_

namespace spike {
	
class MyClass {
public:
	MyClass();
	virtual ~MyClass();
	double squareRoot(const double a);
};

void print_modulo(const std::vector<int>& v, std::ostream& os, int m);
} // namespace
#endif /* MYCLASS_H_ */
