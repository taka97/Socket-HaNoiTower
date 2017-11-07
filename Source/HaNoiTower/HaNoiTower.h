#pragma once
#include <iostream>
#include <vector>
#include <string>


class CHaNoiTower
{
public:
	enum TYPE {
		NORMAL = 1, // create data without ramdom disk on column
		RANDOM = 2	// create data with random disk on column
	};

	// constructor
	CHaNoiTower();
	CHaNoiTower(TYPE type, size_t num = 0);
	~CHaNoiTower();

	// method
	std::string getHistory();
	bool move(size_t disk, std::string toCol);
	bool isSolve();
	std::string status();
	
private:
	std::string History;	// store history move disk
	size_t numDisk = 0;
	std::vector<int> colA, colB, colC;
};