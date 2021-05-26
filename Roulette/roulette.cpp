#include "roulette.h"
#include <bit>
#include <bitset>
#include <cstdint>
#include <initializer_list>
#include <vector>
#include <algorithm>
#include <iostream>

#ifdef _MSC_VER
#  include <intrin.h>
#  define __builtin_popcount __popcnt
#endif

Roulette::Roulette(std::vector<bool>& cards, int blackCount, int whiteCount)
{
	

	this->validPosition = true;
	this->cards = cards;
	this->blackCountIntrigue = blackCount;
	this->whiteCountIntrigue = whiteCount;
	this->lvls.resize(cards.size() + 1);
	this->needBlackCards = 0;
	this->needWhiteCards = 0;
	for (int i = 0; i < this->cards.size(); ++i) {
		if (this->cards[i]) {
			this->needWhiteCards++;
		}
		else {
			this->needBlackCards++;
		}
	}

	if (blackCount > this->needBlackCards || whiteCount > this->needWhiteCards) {
		this->validPosition = false;
	}
}

int Roulette::GetCountGoodPositions() {
	return this->goodPositionsCount;
}
int Roulette::GetTotalPositions() {
	return this->totalPositions;
}

int Roulette::GetIntFromPositions(const std::vector<bool>& pos) {
	int res = 0;
	for (const auto i : pos) {
		res *= 2;
		res += i ? 1 : 0;
	}

	return res;
}

std::vector<bool> Roulette::GetPositionFromInt(int n) {
	std::vector<bool> res(this->cards.size(), false);
	int i = res.size() - 1;
	while (n) {
		res[i] = n & 1 ? true : false;
		n /= 2;
		--i;
	}
	return res;
}


bool Roulette::IsValidPosition() {
	return this->validPosition;
}

bool Roulette::IsBadPosition(int nowBlack, int needBlack, int nowWhite, int needWhite) {
	return (nowWhite == needWhite + 1 && nowBlack < needBlack) || (nowBlack == needBlack + 1 && nowWhite < needWhite);
}

bool Roulette::IsGoodPosition(int nowBlack, int needBlack, int nowWhite, int needWhite) {
	return nowWhite == needWhite && nowBlack == needBlack;
}

bool Roulette::IsNormalPosition(int nowBlack, int needBlack, int nowWhite, int needWhite) {
	return nowBlack <= needBlack && nowWhite <= needWhite;
}

void Roulette::MakePositions() {
	

	for (int i = 0; i < (1 << this->cards.size()); ++i) {
		std::vector<bool> pos = this->GetPositionFromInt(i);
		int countWhiteCards = 0, countBlackCards = 0;
		for (int j = 0; j < pos.size(); ++j) {
			if (pos[j]) {
				if (this->cards[j]) {
					countWhiteCards++;
				}
				else {
					countBlackCards++;
				}
			}
		}
		if (this->IsBadPosition(countBlackCards, this->needBlackCards, countWhiteCards, this->needWhiteCards)) {
			this->badPositions.push_back(i);
		}
		else if (this->IsGoodPosition(countBlackCards, this->needBlackCards, countWhiteCards, this->needWhiteCards)) {
			this->goodPositions.push_back(i);
		}
	}
}

void Roulette::CalcLvls(int lvl) {
	for (auto pos : this->lvls[lvl]) {
		std::vector<bool> position = this->GetPositionFromInt(pos);
		int leftOnes = 0;
		for (int i = position.size() - 1; i > 0; --i) {
			if (position[i]) {
				leftOnes++;
			}
			else {
				break;
			}
		}
		for (int i = 0; i < position.size(); ++i) {
			if (position[i]) {
				leftOnes++;
			}
			else {
				std::vector<bool> newPosition = position;
				newPosition[i] = true;
				int newPositionInt = this->GetIntFromPositions(newPosition);
				this->countOfPositions[newPositionInt] += (leftOnes + 1) * this->countOfPositions[pos];
				leftOnes = 0;
			}
		}
	}
}

void Roulette::CalcPositions() {
	this->needBlackCards = 0;
	this->needWhiteCards = 0;
	if (!this->validPosition) {
		this->totalPositions = -1;
		this->badPositionsCount = -1;
		this->goodPositionsCount = -1;
		return;
	}

	for (int i = 0; i < this->cards.size(); ++i) {
		if (this->cards[i]) {
			this->needWhiteCards++;
		}
		else {
			this->needBlackCards++;
		}
	}
	this->needBlackCards -= this->blackCountIntrigue;
	this->needWhiteCards -= this->whiteCountIntrigue;

	for (int i = 0; i < (1 << this->cards.size()); ++i) {
		std::vector<bool> pos = this->GetPositionFromInt(i);
		int countWhiteCards = 0, countBlackCards = 0;
		for (int j = 0; j < pos.size(); ++j) {
			if (pos[j]) {
				if (this->cards[j]) {
					countWhiteCards++;
				}
				else {
					countBlackCards++;
				}
			}
		}
		if (this->IsNormalPosition(countBlackCards, this->needBlackCards, countWhiteCards, this->needWhiteCards)) {
			this->lvls[__builtin_popcount(static_cast<unsigned int> (i))].push_back(i);
		}
	}
	this->countOfPositions.clear();
	this->countOfPositions[0] = 1;
	for (int i = 0; i < this->lvls.size(); ++i) {
		this->CalcLvls(i);
	}
}

double Roulette::Calc()
{

	

	this->CalcPositions();
	
	if (!this->IsValidPosition()) {
		return -1;
	}

	{
		this->MakePositions();
		this->totalPositions = 0;
		this->goodPositionsCount = 0;
		this->badPositionsCount = 0;
		for (auto goodPos : this->goodPositions) {
			this->goodPositionsCount += this->countOfPositions[goodPos];
		}
		for (auto badPos : this->badPositions) {
			this->badPositionsCount += this->countOfPositions[badPos];
		}
		this->totalPositions = this->goodPositionsCount + this->badPositionsCount;
		return static_cast<double>(this->goodPositionsCount) / static_cast<double>(this->totalPositions);
		/*std::cout << "On cards [";
		for (auto card : this->cards) {
			std::cout << card ? "w" : "b";
		}
		std::cout << "] we have this parametrs:" << std::endl;
		std::cout << "Need leave " << this->whiteCountIntrigue << " and " << this->blackCountIntrigue << " cards";
		std::cout << "Total positions = " << this->totalPositions << std::endl;
		std::cout << "Positions with intrigue = " << this->goodPositionsCount << std::endl;
		*/
	}
}
