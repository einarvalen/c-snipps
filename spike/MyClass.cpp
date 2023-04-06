/*
 * MyClass.cpp
 *
 *  Created on: May 12, 2014
 *      Author: einar
 */

#include "MyClass.h"
#include <math.h>
#include <iostream>
#include <vector>
#include <algorithm>

namespace spike {
MyClass::MyClass() {
	// TODO Auto-generated constructor stub

}

MyClass::~MyClass() {
	// TODO Auto-generated destructor stub
}


double MyClass::squareRoot(const double a) {
    double b = sqrt(a);
    if(b != b) { // nan check
        return -1.0;
    }else{
        return sqrt(a);
    }
}

void print_modulo(const std::vector<int>& v, std::ostream& os, int m) {
	std::for_each(std::begin(v), std::end(v), [&os, m](int x) {
		if (x%m==0) os << x << '\n';
	});
}

} // namespace
