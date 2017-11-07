#include "HaNoiTower.h"

int main()
{
	CHaNoiTower obj(4, CHaNoiTower::RANDOM);
	size_t disk;
	string col;

	//cout << obj.status();

	do
	{
		system("cls");
		cout << obj.status();
		cout << obj.getHistory();
		cout << "Disk: ";	cin >> disk;
		cout << "To col: ";	cin >> col;
		obj.move(disk, col);
	} while (!obj.isSolve());

	system("cls");
	cout << obj.status();
	cout << obj.getHistory();
	cout << "Solved with " << obj.getNumMoving() << " moves" << endl;

	system("pause");
	return 0;
}