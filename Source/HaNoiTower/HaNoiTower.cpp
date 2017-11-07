#include "HaNoiTower.h"
using namespace std;

CHaNoiTower::CHaNoiTower()
{
	History = string();
	numDisk = 3;
	for (size_t i = 0; i < 3; i++)
	{
		col[i].resize(numDisk);
		num[i] = 0;
	}

	for (num[0] = 0; num[0] < numDisk; num[0]++)
		col[0][num[0]] = numDisk - num[0];
}

CHaNoiTower::CHaNoiTower(TYPE type, size_t numDisk)
{
	srand(time(NULL));
	switch (type)
	{
	case NORMAL:
		CHaNoiTower();
		break;
	case RANDOM:
		size_t i;
		for (i = 0; i < 3; i++)
		{
			col[i].resize(numDisk);
			num[i] = 0;
		}

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

bool CHaNoiTower::move(size_t disk, string toCol)
{
	if (disk > numDisk)
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

	if (num[posCol] == 0)
		move(posCol, posDisk);
	else if (disk > col[posCol][num[posCol] - 1])
		return false;
	else
		move(posCol, posDisk);

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
	ss << endl;

	ss << "B: ";
	for (i = 0; i < num[1]; i++)
		ss << col[1][i] << " ";
	ss << endl;

	ss << "C: ";
	for (i = 0; i < num[2]; i++)
		ss << col[2][i] << " ";
	ss << endl;

	return ss.str();
}

void CHaNoiTower::move(size_t fromCol, size_t toCol)
{
	col[toCol][num[toCol]++] = col[fromCol][num[fromCol]--];
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