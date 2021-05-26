#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <string>
#include <thread>
#include <future>
#include "roulette.h"

#define tests {1, 1, 1, 1, 1, 1, 1, 1, 1}

using namespace std;

void TEST() {

	vector<bool> test = tests;

	if (test[0]) {
		vector<bool> t0 = { 1, 0, 0, 1 };
		Roulette r0(t0, 1, 1);
		r0.Calc();
		assert(r0.GetCountGoodPositions() == 10 && r0.GetTotalPositions() == 16);
		cout << "TEST 0 IS OK" << endl;
	}

	if (test[1]) {
		vector<bool> t1 = { 1, 0, 0, 0 };
		Roulette r1(t1, 1, 1);
		r1.Calc();
		assert(r1.GetCountGoodPositions() == 8 && r1.GetTotalPositions() == 13);
		cout << "TEST 1 IS OK" << endl;
	}

	if (test[2]) {
		vector<bool> t2 = { 1, 0, 0};
		Roulette r2(t2, 1, 1);
		r2.Calc();
		assert(r2.GetCountGoodPositions() == 2 && r2.GetTotalPositions() == 3);
		cout << "TEST 2 IS OK" << endl;
	}

	if (test[3]) {
		vector<bool> t3 = { 0, 1, 0 };
		Roulette r3(t3, 1, 1);
		r3.Calc();
		assert(r3.GetCountGoodPositions() == 2 && r3.GetTotalPositions() == 3);
		cout << "TEST 3 IS OK" << endl;
	}

	if (test[4]) {
		vector<bool> t4 = { 0, 0, 1 };
		Roulette r4(t4, 1, 1);
		r4.Calc();
		assert(r4.GetCountGoodPositions() == 2 && r4.GetTotalPositions() == 3);
		cout << "TEST 4 IS OK" << endl;
	}

	if (test[5]) {
		vector<bool> t5 = { 1, 0, 0 };
		Roulette r5(t5, 2, 1);
		r5.Calc();
		assert(r5.GetCountGoodPositions() == 1 && r5.GetTotalPositions() == 1);
		cout << "TEST 5 IS OK" << endl;
	}

	if (test[6]) {
		vector<bool> t6 = { 0, 1, 0 };
		Roulette r6(t6, 2, 1);
		r6.Calc();
		assert(r6.GetCountGoodPositions() == 1 && r6.GetTotalPositions() == 1);
		cout << "TEST 6 IS OK" << endl;
	}

	if (test[7]) {
		vector<bool> t7 = { 0, 0, 1 };
		Roulette r7(t7, 2, 1);
		r7.Calc();
		assert(r7.GetCountGoodPositions() == 1 && r7.GetTotalPositions() == 1);
		cout << "TEST 7 IS OK" << endl;
	}
}

double GetAns(Roulette r) {
	return r.Calc();
}


void GenTestBySize(int testSize) {

	string fileName = std::to_string(testSize) + ".csv";
	ofstream outf(fileName);
	outf << "mask_of_set";
	for (int whiteCount = 0; whiteCount <= testSize; ++whiteCount) {
		for (int blackCount = 0; blackCount + whiteCount <= testSize; ++blackCount) {
			outf << ',' << whiteCount << '_' << blackCount;
		}
	}
	outf << endl;

	for (int i = 0; i < (1 << testSize); ++i) {
		vector<bool> cards(testSize);
		int ci = 0;
		int mask = i;
		while (mask != 0) {
			cards[ci] = mask & 1;
			mask /= 2;
			++ci;
		}
		for (int i = 0; i < cards.size(); ++i) {
			if (cards[i]) {
				outf << 1;
			}
			else {
				outf << 0;
			}
		}
		vector<std::future<double>> p;
		for (int whiteCount = 0; whiteCount <= cards.size(); ++whiteCount) {
			for (int blackCount = 0; blackCount + whiteCount <= cards.size(); ++blackCount) {
				Roulette r(cards, blackCount, whiteCount);
				p.push_back(std::async(std::launch::async, GetAns, r));
			}

		}
		
		for (int i = 0; i < p.size(); ++i) {
			outf << ',' << p[i].get();
		}
		outf << endl;
	}
}


int main() {
	TEST();
	int minCards = 4;
	int maxCards = 16;
	vector<std::future<void>> t;
	for (int i = minCards; i <= maxCards; ++i) {
		//t.push_back(std::async(std::launch::async, GenTestBySize, i));
		GenTestBySize(i);
		cout << "Test size " << i << " is done\n";
	}
	
	/*
	for (int i = 0; i < t.size(); ++i) {
		t[i].get();
		cout << "Test size " << i + minCards << " is done\n";
	}
	*/
	return 0;
}
