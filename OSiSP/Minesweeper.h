#pragma once
#include <utility>
using namespace std;
class MineSweeper {
	/*
	field - числовой массив, значение ячейки каждого - количество мин в соседних клетках
	-1 - обозначение бомбы
	fieldStatus - булевый массив, открыта ли ячейка в игровом окне
	*/
private:	
	int mineCount = 15;
	int fieldSize = 10;
	void CreateField();
	void GenerateFieldData();
	void MemClear();
	void IncreaseNeighbours(int i,int j);
public:
	int** field;
	bool** fieldStatus;
	bool** markStatus;
	int unrevealedCount;
	MineSweeper();
	void ResetField();
	int GetFieldSize();
	int GetMineCount();
	pair<int, int> ExtractCoordinates(int x) {
		
		int j = x % fieldSize;
		int i = (x - j) / fieldSize;
		return make_pair(i, j);

	}
	~MineSweeper();

	





};