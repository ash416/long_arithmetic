#include <gtest/gtest.h>

#include "BigIntLittle.h"
#include "BigIntLittle2.h"
#include "BigDoubleLittle2.h"
#include "BigDoubleLittle3.h"
#include "BigIntBig.h"
#include "BigIntBig2.h"
#include "BigDoubleLittle.h"
#include "BigDoubleBig.h"
#include "BigDoubleBig2.h"
#include "BigDoubleBig3.h"
#include "BigIntLittleBin.h"
#include "Reader.h"

#include <fstream>
#include <iostream>

#include <ctime>
#include <ratio>
#include <chrono>
#include <float.h>
#include <algorithm>
#include <time.h>


template<class BigT>
void test_func(const std::string & read_file, const std::string & write_file, const char oper) {
	std::ofstream resFile;
	resFile.open(write_file);
	CReader reader(read_file);
	double time = 0;
	std::chrono::duration<double, std::milli> time_span;

	for (int i = 0; i < reader.num_string; i++) {
		BigT lit1(reader.first_number[i]), lit2(reader.second_number[i]);
		BigT origin(reader.result_number[i]);
		BigT res;
		if (oper == '+') {
			std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
			res = lit1 + lit2;
			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
			time_span = t2 - t1;
			resFile << std::to_string(time_span.count()) << " " << std::to_string(lit1.getSize() > lit2.getSize() ? lit1.getSize() : lit2.getSize()) << "\n";
		}
		else if (oper == '-') {
			std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
			res = lit1 - lit2;
			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
			time_span = t2 - t1;
			resFile << std::to_string(time_span.count()) << " " << std::to_string(lit1.getSize() > lit2.getSize() ? lit1.getSize() : lit2.getSize()) << "\n";
		}
		else if (oper == '*') {
			std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
			res = lit1 * lit2;
			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
			time_span = t2 - t1;
			resFile << std::to_string(time_span.count()) << " " << std::to_string(lit1.getSize() + lit2.getSize()) << "\n";
		}
		else if (oper == '/') {
			std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
			res = lit1 / lit2;
			std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
			time_span = t2 - t1;
			resFile << std::to_string(time_span.count()) << " " << std::to_string(lit1.getSize() >= lit2.getSize() ? lit1.getSize() - lit2.getSize() : 0) << "\n";
		}
		
		time += time_span.count();
		EXPECT_EQ(res, origin);
		lit1.deleteNumber();
		lit2.deleteNumber();
		origin.deleteNumber();
		res.deleteNumber();
		std::cout << i << " ";
	}
	resFile.close();
	std::cout << " " << time << std::endl;
}

/*
TEST(LETestSuite, Int1SumTest)
{
	test_func<CBigIntLittle>(std::string("data/Sum.txt"), std::string("results_int_1/resultsLsum.txt"), '+');
}

TEST(BETestSuite, Int1SumTest)
{
	test_func<CBigIntBig>(std::string("data/Sum.txt"), std::string("results_int_1/resultsBsum.txt"), '+');
}

TEST(LETestSuite, Int1DiffTest)
{
	test_func<CBigIntLittle>(std::string("data/Diff.txt"), std::string("results_int_1/resultsLdiff.txt"), '-');
}

TEST(BETestSuite, Int1DiffTest)
{
	test_func<CBigIntBig>(std::string("data/Diff.txt"), std::string("results_int_1/resultsBdiff.txt"), '-');
}*/

TEST(LETestSuite, Int1MultTest)
{
	test_func<CBigIntLittle>(std::string("data/Mult.txt"), std::string("results_int_1/resultsLmult.txt"), '*');
}

TEST(BETestSuite, Int1MultTest)
{
	test_func<CBigIntBig>(std::string("data/Mult.txt"), std::string("results_int_1/resultsBmult.txt"), '*');
}
/*
TEST(LETestSuite, Int1DivTest)
{
	test_func<CBigIntLittle>(std::string("data/Div.txt"), std::string("results_int_1/resultsLdiv.txt"), '/');
}

TEST(BETestSuite, Int1DivTest)
{
	test_func<CBigIntBig>(std::string("data/Div.txt"), std::string("results_int_1/resultsBdiv.txt"), '/');
}

TEST(LETestSuite, Int2SumTest)
{
	test_func<CBigIntLittle2>(std::string("data/Sum.txt"), std::string("results_int_2/resultsLsum.txt"), '+');
}

TEST(BETestSuite, Int2SumTest)
{
	test_func<CBigIntBig2>(std::string("data/Sum.txt"), std::string("results_int_2/resultsBsum.txt"), '+');
}

TEST(LETestSuite, Int2DiffTest)
{
	test_func<CBigIntLittle2>(std::string("data/Diff.txt"), std::string("results_int_2/resultsLdiff.txt"), '-');
}

TEST(BETestSuite, Int2DiffTest)
{
	test_func<CBigIntBig2>(std::string("data/Diff.txt"), std::string("results_int_2/resultsBdiff.txt"), '-');
}

TEST(LETestSuite, Int2MultTest)
{
	test_func<CBigIntLittle2>(std::string("data/Mult.txt"), std::string("results_int_2/resultsLmult.txt"), '*');
}

TEST(BETestSuite, Int2MultTest)
{
	test_func<CBigIntBig2>(std::string("data/Mult.txt"), std::string("results_int_2/resultsBmult.txt"), '*');
}

TEST(LETestSuite, Int2DivTest)
{
	test_func<CBigIntLittle2>(std::string("data/Div.txt"), std::string("results_int_2/resultsLdiv.txt"), '/');
}

TEST(BETestSuite, Int2DivTest)
{
	test_func<CBigIntBig2>(std::string("data/Div.txt"), std::string("results_int_2/resultsBdiv.txt"), '/');
}

*/
