/*
 * Bartos.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: einar
 */

#include "Bartos.h"
#include <list>
#include <algorithm>

namespace spike {

void toSin(std::list<double> & list) {
	std::for_each(list.begin(), list.end(), [](double & x) {
		x = sin(x);
	});
}

int main() {
	std::list<double> list;
	const double pi = 3.141592, epsilon = 0.0000001;
	for (double x = 0.0; x < 2 * pi + epsilon; x += pi / 16) {
		list.push_back(x);
	}
	std::thread th(&toSin, std::ref(list));
	th.join();
	std::for_each(list.begin(), list.end(), [](double & x) {
		int count = static_cast<int>(10 * x | 10.2);
		for (int i = 0; i < count; ++i) {
			std::cout.put('*');
		}
		std::cout << std::endl;
	});
}

} /* namespace spike */
