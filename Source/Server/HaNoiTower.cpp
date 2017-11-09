#include "stdafx.h"
#include "HaNoiTower.h"
using namespace std;

CHaNoiTower::CHaNoiTower()
{
	init(3);

	for (num[0] = 0; num[0] < numDisk; num[0]++)
		col[0][num[0]] = numDisk - num[0];
}

CHaNoiTower::CHaNoiTower(size_t numDisk, TYPE type)
{
	srand((unsigned int)time(NULL));
	switch (type)
	{
	case NORMAL:
		init(numDisk);
		for (num[0] = 0; num[0] < numDisk; num[0]++)
			col[0][num[0]] = numDisk - num[0];
		break;
	case RANDOM:
		init(numDisk);
		size_t R;

		for (size_t i = 0; i < numDisk; i++)
		{
			R = rand() % 3;
			col[R][num[R]++] = numDisk - i;
		}
		break;
	}
}

CHaNoiTower::~CHaNoiTower()
{
}

string CHaNoiTower::getHistory()
{
	return History;
}

size_t CHaNoiTower::getNumMoving()
{
	return numMoving;
}

bool CHaNoiTower::move(size_t disk, string toCol)
{
	if (disk > numDisk || disk == 0)
		return false;
	size_t posDisk = numDisk + 1;
	size_t posCol;
	for (size_t i = 0; i < 3; i++)
	{
		if (num[i] != 0 && col[i][num[i] - 1] == disk)
		{
			posDisk = i;
			break;
		}
	}

	// input is invalid
	if (posDisk == numDisk + 1)
		return false;

	posCol = numOfCol(toCol);

	if (num[posCol] == 0 || disk < col[posCol][num[posCol] - 1])
		move(posCol, posDisk);
	else
		return false;

	return true;
}

bool CHaNoiTower::isSolve()
{
	return num[1] == numDisk || num[2] == numDisk ? true : false;
}

string CHaNoiTower::status()
{
	stringstream ss;
	size_t i;

	ss << "A: ";
	for (i = 0; i < num[0]; i++)
		ss << col[0][i] << " ";
	ss << "\r\n";

	ss << "B: ";
	for (i = 0; i < num[1]; i++)
		ss << col[1][i] << " ";
	ss << "\r\n";

	ss << "C: ";
	for (i = 0; i < num[2]; i++)
		ss << col[2][i] << " ";
	ss << "\r\n";

	return ss.str();
}

void CHaNoiTower::move(size_t toCol, size_t fromCol)
{
	col[toCol][num[toCol]] = col[fromCol][num[fromCol] - 1];
	num[toCol]++;
	num[fromCol]--;
	numMoving++;
	updateHistory(col[toCol][num[toCol] - 1], fromCol, toCol);
}

void CHaNoiTower::updateHistory(size_t disk, size_t fromCol, size_t toCol)
{
	stringstream ss;

	ss << "Disk: " << disk << " from column " << nameOfCol(fromCol) << " to column " << nameOfCol(toCol);
	History += ss.str();
	History += "\r\n";
}

string CHaNoiTower::nameOfCol(size_t col)
{
	switch (col)
	{
	case 0:
		return "A";
	case 1:
		return "B";
	case 2:
		return "C";
	default:
		return string();
	}
}

size_t CHaNoiTower::numOfCol(string nameOfCol)
{
	if (nameOfCol == "A")
		return 0;
	if (nameOfCol == "B")
		return 1;
	else
		return 2;
}

void CHaNoiTower::init(size_t numDisk)
{
	History = string();
	this->numDisk = numDisk;
	for (size_t i = 0; i < 3; i++)
	{
		col[i].resize(numDisk);
		num[i] = 0;
	}
}

size_t CHaNoiTower::getNumOfDisk()
{
	return numDisk;
}

CGame::CGame()
{
	m_numOfPlayer = 0;
}

CGame::CGame(size_t numOfPlayer, size_t numDisk, TYPE type)
{
	m_numOfPlayer = numOfPlayer;
	tower.resize(m_numOfPlayer);
	status.resize(m_numOfPlayer);

	CHaNoiTower tmp(numDisk, type);

	for (size_t i = 0; i < m_numOfPlayer; i++)
		tower[i] = tmp;
}

bool CGame::startGame()
{
	if (isReady())
	{
		for (size_t i = 0; i < m_numOfPlayer; i++)
			status[i] = PLAYING;
		return true;
	}
	
	return false;
}

bool CGame::isReady()
{
	return m_numOfPlayer > 0 ? true : false;
}

bool CGame::giveup(size_t player)
{
	if (player >= m_numOfPlayer)
		return false;

	status[player] = GIVEUP;
	return true;
}

bool CGame::isGiveup(size_t player)
{
	return status[player] == GIVEUP;
}

size_t CGame::getNumberDisk()
{
	return tower[0].getNumOfDisk();
}

string CGame::getHistory(size_t player)
{
	return tower[player].getHistory();
}

string CGame::getStatus(size_t player)
{
	return tower[player].status();
}

bool CGame::move(size_t player, size_t disk, string toCol)
{
	bool st = tower[player].move(disk, toCol);
	if (tower[player].isSolve())
		status[player] = SOLVED;
	return st;
}

bool CGame::isEndGame()
{
	for (size_t player = 0; player < m_numOfPlayer; player++)
		if (status[player] == PLAYING)
			return false;
	return true;
}

size_t CGame::getWinner()
{
	size_t winner = size_t(-1);
	size_t score = size_t(-1);

	for (size_t player = 0; player < m_numOfPlayer; player++)
	{
		if (tower[player].getNumMoving() < score)
		{
			winner = player;
			score = tower[player].getNumMoving();
		}
	}
	return winner;
}