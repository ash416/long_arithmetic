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
#include <windows.h>

#include <ctime>
#include <ratio>
#include <chrono>
#include <float.h>
#include <algorithm>

#include <vector>

#include <Windows.h>

__int64 __cdecl GetTSC() {
	__asm RDTSC
}


template<class BigT>
void test_func(const std::string & read_file, const std::string & write_file, const char oper) {
	__int64 StartTscValue, EndTscValue;
	StartTscValue = GetTSC();
	Sleep(1000);
	EndTscValue = GetTSC();
	__int64 freq = (EndTscValue - StartTscValue);
	std::ofstream resFile;
	resFile.open(write_file);
	CReader reader(read_file);
	double time = 0;
	double sum_t = 0.0;
	for (int i = 0; i < reader.num_string; i++) {
		std::vector<double> count;
		BigT lit1(reader.first_number[i]), lit2(reader.second_number[i]);
		BigT origin(reader.result_number[i]);
		BigT res;
		if (oper == '+') {
			sum_t = 0;
			for (int k = 0; k < 10; k++) {
				StartTscValue = GetTSC();
				res = lit1 + lit2;
				count.push_back(double(GetTSC() - StartTscValue));
			}
			std::sort(count.begin(), count.end());
			sum_t = (count[4] + count[5]) / 2;
			std::cout << sum_t << std::endl;
			std::cout << lit1.getSize() << " " << lit2.getSize() << std::endl;
			resFile << std::to_string((sum_t / freq) * 1000 ) << " " << std::to_string(lit1.getSize() > lit2.getSize() ? lit1.getSize() : lit2.getSize()) << "\n";
		}
		else if (oper == '-') {
			sum_t = 0;
			for (int k = 0; k < 10; k++) {
				StartTscValue = GetTSC();
				res = lit1 - lit2;
				count.push_back(double(GetTSC() - StartTscValue));
			}
			std::sort(count.begin(), count.end());
			sum_t = (count[4] + count[5]) / 2;
			std::cout << sum_t << std::endl;
			resFile << std::to_string((sum_t / freq) * 1000) << " " << std::to_string(lit1.getSize() > lit2.getSize() ? lit1.getSize() : lit2.getSize()) << "\n";
		}
		else if (oper == '*') {
			sum_t = 0;
			for (int k = 0; k < 10; k++) {
				StartTscValue = GetTSC();
				res = lit1 * lit2;
				count.push_back(double(GetTSC() - StartTscValue));
			}
			std::sort(count.begin(), count.end());
			sum_t = (count[4] + count[5]) / 2;
			std::cout << sum_t << std::endl;
			resFile << std::to_string((sum_t / freq) * 1000) << " " << std::to_string(lit1.getSize() > lit2.getSize() ? lit1.getSize() : lit2.getSize()) << "\n";
		}
		else if (oper == '/') {
			sum_t = 0;
			for (int k = 0; k < 10; k++) {
				StartTscValue = GetTSC();
				res = lit1 / lit2;
				count.push_back(double(GetTSC() - StartTscValue));
			}
			std::sort(count.begin(), count.end());
			sum_t = (count[4] + count[5]) / 2;
			std::cout << sum_t << std::endl;
			resFile << std::to_string((sum_t / freq) * 1000) << " " << std::to_string(lit1.getSize() > lit2.getSize() ? lit1.getSize() : lit2.getSize()) << "\n";
		}
		
		time += sum_t;
		EXPECT_EQ(res, origin);
		lit1.deleteNumber();
		lit2.deleteNumber();
		origin.deleteNumber();
		res.deleteNumber();
		std::cout << i << " ";
	}
	resFile.close();
}




TEST(LETestSuite, Int1SumTest)
{
	test_func<CBigDoubleLittle>(std::string("data/diff_double.txt"), std::string("results_double_2/resultsLdiff11.txt"), '-');
}

TEST(BETestSuite, Int1SumTest)
{
	test_func<CBigDoubleBig>(std::string("data/diff_double.txt"), std::string("results_double_2/resultsBdiff11.txt"), '-');
}
/*
TEST(LETestSuite, Int1DiffTest)
{
	test_func<CBigIntLittle>(std::string("data/Diff2.txt"), std::string("results_int_1/resultsLdiff.txt"), '-');
}

TEST(BETestSuite, Int1DiffTest)
{
	test_func<CBigIntBig>(std::string("data/Diff2.txt"), std::string("results_int_1/resultsBdiff.txt"), '-');
}


TEST(LETestSuite, Int1MultTest)
{
	test_func<CBigIntLittle>(std::string("data/Mult2.txt"), std::string("results_int_1/resultsLmult.txt"), '*');
}

TEST(BETestSuite, Int1MultTest)
{
	test_func<CBigIntBig>(std::string("data/Mult2.txt"), std::string("results_int_1/resultsBmult.txt"), '*');
}*/
/*

TEST(LETestSuite, Int1DivTest)
{
	test_func<CBigIntLittle>(std::string("data/Div2.txt"), std::string("results_int_1/resultsLdiv.txt"), '/');
}

TEST(BETestSuite, Int1DivTest)
{
	test_func<CBigIntBig>(std::string("data/Div2.txt"), std::string("results_int_1/resultsBdiv.txt"), '/');
}

TEST(LETestSuite, Int2SumTest)
{
	test_func<CBigIntLittle2>(std::string("data/Sum2.txt"), std::string("results_int_2/resultsLsum.txt"), '+');
}

TEST(BETestSuite, Int2SumTest)
{
	test_func<CBigIntBig2>(std::string("data/Sum2.txt"), std::string("results_int_2/resultsBsum.txt"), '+');
}*/
/*
TEST(LETestSuite, Int2DiffTest)
{
	test_func<CBigIntLittle2>(std::string("data/Diff2.txt"), std::string("results_int_2/resultsLdiff.txt"), '-');
}

TEST(BETestSuite, Int2DiffTest)
{
	test_func<CBigIntBig2>(std::string("data/Diff2.txt"), std::string("results_int_2/resultsBdiff.txt"), '-');
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
	test_func<CBigIntLittle2>(std::string("data/Div2.txt"), std::string("results_int_2/resultsLdiv.txt"), '/');
}

TEST(BETestSuite, Int2DivTest)
{
	test_func<CBigIntBig2>(std::string("data/Div2.txt"), std::string("results_int_2/resultsBdiv.txt"), '/');
}
*/
/*
TEST(LETestSuite, Double1SumTest)
{
	CBigDoubleLittle a, b;
	std::cin >> a >> b;
	std::cout << a / b;
	//test_func<CBigDoubleLittle>(std::string("data/div_double.txt"), std::string("results_double_1/resultsLdiv_exp.txt"), '/');
}

TEST(BETestSuite, Double1SumTest)
{
	test_func<CBigDoubleBig3>(std::string("data/sum_double.txt"), std::string("results_double_3/resultsBsum.txt"), '+');
}*/