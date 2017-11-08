#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>

#define GIVEUP	0
#define PLAYING 1
#define SOLVED	2

using namespace std;

enum TYPE {
	NORMAL = 1, // create data without ramdom disk on column
	RANDOM = 2	// create data with random disk on column
};

class CHaNoiTower
{
public:
	// constructor
	CHaNoiTower();
	CHaNoiTower(size_t numDisk, TYPE type = NORMAL);
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

class CGame
{
public:
	CGame();
	CGame(size_t numOfPlayer, size_t numDisk, TYPE type = NORMAL);

	bool isReady();
	string getHistory(size_t player);
	string getStatus(size_t player);
	bool move(size_t player, size_t disk, string toCol);
	bool isEndGame();
	size_t getWinner();
private:
	vector<CHaNoiTower> tower;
	vector<int>status;
	size_t m_numOfPlayer;
};