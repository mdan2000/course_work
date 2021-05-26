#pragma once
#include <vector>
#include <map>


class Roulette 
{
private:
	std::vector<bool> cards;
	std::map<int, long long> countOfPositions;
	std::vector<int> goodPositions;
	long long goodPositionsCount;
	std::vector<int> badPositions;
	long long badPositionsCount;
	std::vector<int> startPositions;
	std::vector<std::vector<int>> lvls;
	int blackCountIntrigue;
	int whiteCountIntrigue;
	int needBlackCards;
	int needWhiteCards;
	long long totalPositions;
	bool validPosition;
	bool IsBadPosition(int nowBlack, int needBlack, int nowWhite, int needWhite);
	bool IsGoodPosition(int nowBlack, int needBlack, int nowWhite, int needWhite);
	bool IsNormalPosition(int nowBlack, int needBlack, int nowWhite, int needWhite);
	void CalcLvls(int lvl);
	void CalcPositions();
	void MakePositions();

public:
	
	Roulette(std::vector<bool>& cards, int black_n, int white_n);
	int GetCountGoodPositions();
	int GetTotalPositions();
	bool IsValidPosition();

	std::vector<bool> GetPositionFromInt(int n);
	int GetIntFromPositions(const std::vector<bool>& pos);
	
	double Calc();



};
