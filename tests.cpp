#include <gtest/gtest.h>

#include "BigIntLittle.h"
#include "BigIntBig.h"
#include "Reader.h"

#include <fstream>
#include <iostream>

#include <ctime>
#include <ratio>
#include <chrono>

#include <time.h>

TEST(LETestSuite, LittleSumCorrectnessTest)
{
	std::ofstream resSum;
	resSum.open("results_1/resultsLsum.txt");
	CReader sumReader("data/Sum.txt");
	double time = 0;
	std::chrono::duration<double, std::milli> time_span;

	for (int i = 0; i < 100; i++) {
		CBigIntLittle lit1(sumReader.first_number[i]), lit2(sumReader.second_number[i]);
		CBigIntLittle origin(sumReader.result_number[i]);
		CBigIntLittle res;
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		res = lit1 + lit2;
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		time_span = t2 - t1;
		resSum << std::to_string(time_span.count()) << " " << std::to_string(lit1.getSize() > lit2.getSize() ? lit1.getSize() : lit2.getSize()) << "\n";
		time += time_span.count();
		EXPECT_EQ(res, origin);
		lit1.deleteNumber();
		lit2.deleteNumber();
		origin.deleteNumber();
		res.deleteNumber();
		std::cout << i << " ";
	}
	resSum.close();
	std::cout << " " << time << std::endl;
}

TEST(LETestSuite, SimpleDiffCorrectnessTest)
{
	std::ofstream resDiff;
	resDiff.open("results_1/resultsLdiff.txt");
	CReader diffReader("data/Diff.txt");
	double time = 0;
	std::chrono::duration<double, std::milli> time_span;
	for (int i = 0; i < 100; i++) {
		CBigIntLittle lit1(diffReader.first_number[i]), lit2(diffReader.second_number[i]);
		CBigIntLittle origin(diffReader.result_number[i]);
		CBigIntLittle res;
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		res = lit1 - lit2;
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		time_span = t2 - t1;
		resDiff << std::to_string(time_span.count()) << " " << std::to_string(lit1.getSize() > lit2.getSize() ? lit1.getSize() : lit2.getSize()) << "\n";
		time += time_span.count();
		EXPECT_EQ(res, origin);
		lit1.deleteNumber();
		lit2.deleteNumber();
		origin.deleteNumber();
		res.deleteNumber();
		std::cout << i << " ";
	}
	resDiff.close();
	std::cout << " " << time << std::endl;
}


TEST(BETestSuite, SimpleSumCorrectnessTest)
{
	std::ofstream resSum;
	resSum.open("results_1/resultsBsum.txt");
	double time = 0;
	std::chrono::duration<double, std::milli> time_span;
	CReader sumReader("data/Sum.txt");
	for (int i = 0; i < 100; i++) {
		CBigIntBig lit1(sumReader.first_number[i]), lit2(sumReader.second_number[i]);
		CBigIntBig origin(sumReader.result_number[i]);
		CBigIntBig res;
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		res = lit1 + lit2;
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		time_span = t2 - t1;
		resSum << std::to_string(time_span.count()) << " " << std::to_string(lit1.getSize() > lit2.getSize() ? lit1.getSize() : lit2.getSize()) << "\n";
		time += time_span.count();
		EXPECT_EQ(res, origin);
		lit1.deleteNumber();
		lit2.deleteNumber();
		origin.deleteNumber();
		res.deleteNumber();
		std::cout << i << " ";
	}
	resSum.close();
	std::cout << " " << time << std::endl;
}

TEST(BETestSuite, SimpleDiffCorrectnessTest)
{
	std::ofstream resDiff;
	resDiff.open("results_1/resultsBdiff.txt");
	CReader diffReader("data/Diff.txt");
	double time = 0;
	std::chrono::duration<double, std::milli> time_span;
	for (int i = 0; i < 100; i++) {
		CBigIntBig lit1(diffReader.first_number[i]), lit2(diffReader.second_number[i]);
		CBigIntBig origin(diffReader.result_number[i]);
		CBigIntBig res;
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		res = lit1 - lit2;
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		time_span = t2 - t1;
		resDiff << std::to_string(time_span.count()) << " " << std::to_string(lit1.getSize() > lit2.getSize() ? lit1.getSize() : lit2.getSize()) << "\n";
		time += time_span.count();
		EXPECT_EQ(res, origin);
		lit1.deleteNumber();
		lit2.deleteNumber();
		origin.deleteNumber();
		res.deleteNumber();
		std::cout << i << " ";
	}
	resDiff.close();
	std::cout << " " << time << std::endl;
}

/*TEST(BETestSuite, SimpleMultCorrectnessTest)
{
	std::ofstream resMult;
	resMult.open("results_1/resultsBmult.txt");
	CReader multReader("data/Mult.txt");
	double time = 0;
	std::chrono::duration<double, std::milli> time_span;
	for (int i = 0; i < 100; i++) {
		CBigIntBig lit1(multReader.first_number[i]), lit2(multReader.second_number[i]);
		CBigIntBig origin(multReader.result_number[i]);
		CBigIntBig res;
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		res = lit1 * lit2;
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		time_span = t2 - t1;
		resMult << std::to_string(time_span.count()) << " " << std::to_string(lit1.getSize() + lit2.getSize()) << "\n";
		time += time_span.count();
		EXPECT_EQ(res, origin);
		lit1.deleteNumber();
		lit2.deleteNumber();
		origin.deleteNumber();
		res.deleteNumber();
		std::cout << i << " ";
	}
	resMult.close();
	std::cout << " " << time << std::endl;
}

TEST(LETestSuite, SimpleMultCorrectnessTest)
{
	std::ofstream resMult;
	resMult.open("results_1/resultsLmult.txt");
	CReader multReader("data/Mult.txt");
	double time = 0;
	std::chrono::duration<double, std::milli> time_span;
	for (int i = 0; i < 100; i++) {
		CBigIntLittle lit1(multReader.first_number[i]), lit2(multReader.second_number[i]);
		CBigIntLittle origin(multReader.result_number[i]);
		CBigIntLittle res;
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		res = lit1 * lit2;
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		time_span = t2 - t1;
		resMult << std::to_string(time_span.count()) << " " << std::to_string(lit1.getSize() + lit2.getSize()) << "\n";
		time += time_span.count();
		EXPECT_EQ(res, origin);
		lit1.deleteNumber();
		lit2.deleteNumber();
		origin.deleteNumber();
		res.deleteNumber();
		std::cout << i << " ";
	}
	resMult.close();
	std::cout << " " << time << std::endl;
}

TEST(BETestSuite, SimpleDivCorrectnessTest)
{
	std::ofstream resDiv;
	resDiv.open("results_1/resultsBdiv.txt");
	CReader divReader("data/Div.txt");
	double time = 0;
	std::chrono::duration<double, std::milli> time_span;
	for (int i = 0; i < 100; i++) {
		CBigIntBig lit1(divReader.first_number[i]), lit2(divReader.second_number[i]);
		CBigIntBig origin(divReader.result_number[i]);
		CBigIntBig res;
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		res = lit1 / lit2;
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		time_span = t2 - t1;
		resDiv << std::to_string(time_span.count()) << " " << std::to_string(lit1 > lit2 ? lit1.getSize() - lit2.getSize() : 0) << "\n";
		time += time_span.count();
		EXPECT_EQ(res, origin);
		lit1.deleteNumber();
		lit2.deleteNumber();
		origin.deleteNumber();
		res.deleteNumber();
		std::cout << i << " ";
	}
	resDiv.close();
	std::cout << " " << time << std::endl;
}

TEST(LETestSuite, SimpleDivCorrectnessTest)
{
	std::ofstream resDiv;
	resDiv.open("results_1/resultsLdiv.txt");
	CReader divReader("data/Div.txt");
	double time = 0;
	std::chrono::duration<double, std::milli> time_span;
	for (int i = 0; i < 100; i++) {
		CBigIntLittle lit1(divReader.first_number[i]), lit2(divReader.second_number[i]);
		CBigIntLittle origin(divReader.result_number[i]);
		CBigIntLittle res;
		std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
		res = lit1 / lit2;
		std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
		time_span = t2 - t1;
		resDiv << std::to_string(time_span.count()) << " " << std::to_string(lit1 > lit2 ? lit1.getSize() - lit2.getSize() : 0) << "\n";
		time += time_span.count();
		EXPECT_EQ(res, origin);
		lit1.deleteNumber();
		lit2.deleteNumber();
		origin.deleteNumber();
		res.deleteNumber();
		std::cout << i << " ";
	}
	resDiv.close();
	std::cout << " " << time << std::endl;
}*/