#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
using namespace std;

class CHaNoiTower
{
public:
	enum TYPE {
		NORMAL = 1, // create data without ramdom disk on column
		RANDOM = 2	// create data with random disk on column
	};

	// constructor
	CHaNoiTower();
	CHaNoiTower(size_t numDisk = 3, TYPE type = NORMAL);
	~CHaNoiTower();

	// method
	string getHistory();
	size_t getNumMoving();
	bool move(size_t disk, string toCol);
	bool isSolve();
	string status();

protected:
	void move(size_t toCol, size_t fromCol);
	void updateHistory(size_t disk, size_t fromCol, size_t toCol);
	string nameOfCol(size_t numOfCol);
	size_t numOfCol(string nameOfCol);
	void init(size_t numDisk);
private:
	string History;	// store history move disk
	size_t numDisk = 0;
	vector<int> col[3];
	size_t num[3];
	size_t numMoving = 0;
};