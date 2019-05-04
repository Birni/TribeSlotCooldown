#include <iostream>
#include "gtest/gtest.h"
#include "test.h"

using namespace std;

int main(int argc, char **argv)
{
	::testing::InitGoogleTest(&argc, argv);
	
	//::testing::GTEST_FLAG(filter) = "test1";
	return RUN_ALL_TESTS();

}