/*
 * MyClassTest.cpp
 *
 *      Author: einar
 */

#include <gtest/gtest.h>
#include "MyClass.h"
using spike::MyClass;
TEST(MyClassTest, dummy) {
	ASSERT_EQ(true, true);
}
TEST(SquareRootTest, PositiveNos) {
	MyClass w;
	ASSERT_EQ(6, w.squareRoot(36.0));
	ASSERT_EQ(18.0, w.squareRoot(324.0));
	ASSERT_EQ(25.4, w.squareRoot(645.16));
	ASSERT_EQ(0, w.squareRoot(0.0));
}

TEST(SquareRootTest, NegativeNos) {
	MyClass w;
	ASSERT_EQ(-1.0, w.squareRoot(-15.0));
	ASSERT_EQ(-1.0, w.squareRoot(-0.2));
}

TEST(MyClassTest, print_modulo_4) {
	std::vector<int> myvector;
	myvector.push_back(10);
	myvector.push_back(20);
	myvector.push_back(30);
	spike::print_modulo(myvector, std::cout, 4);
}
