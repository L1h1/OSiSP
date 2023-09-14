#include "Minesweeper.h"
#include<cstdlib>
#include <iostream>

using namespace std;

MineSweeper::MineSweeper() {
	srand(time(0));
	CreateField();
	GenerateFieldData();
}

MineSweeper::~MineSweeper() {
	MemClear();
}


void MineSweeper::ResetField() {
	MemClear();
	CreateField();
	GenerateFieldData();
}

void MineSweeper::MemClear() {
	for (int i = 0; i < fieldSize; i++) {
		delete[] field[i];
		delete[] fieldStatus[i];
		delete[] markStatus[i];
	}
	delete[] field;
	delete[] fieldStatus;
	delete[] markStatus;
}

void MineSweeper::CreateField() {

	unrevealedCount = fieldSize * fieldSize;


	field = new int* [fieldSize];
	for (int i = 0; i < fieldSize; i++) {
		field[i] = new int[fieldSize];
		for (int j = 0; j < fieldSize; j++) {
			field[i][j] = 0;
		}
	}

	fieldStatus = new bool* [fieldSize];
	for (int i = 0; i < fieldSize; i++) {
		fieldStatus[i] = new bool[fieldSize];
		for (int j = 0; j < fieldSize; j++) {
			fieldStatus[i][j] = 0;
		}
	}
	markStatus = new bool* [fieldSize];
	for (int i = 0; i < fieldSize; i++) {
		markStatus[i] = new bool[fieldSize];
		for (int j = 0; j < fieldSize; j++) {
			markStatus[i][j] = 0;
		}
	}
}

void MineSweeper::IncreaseNeighbours(int i,int j) {
	for (int k = i - 1; k <= i + 1; k++) {
		if (k < 0 || k==fieldSize) continue;
		for (int p = j - 1; p <= j + 1; p++) {
			if (p < 0 || p == fieldSize) continue;
			if (k == i && j == p) continue;
			if (field[k][p] != -1) {
				field[k][p]++;
			}
		}
	}
}


void MineSweeper::GenerateFieldData() {
	int totalMines = 0;

	while (totalMines < mineCount) {
		int i = rand() % fieldSize, j = rand() % fieldSize;
		if (field[i][j] == -1) {
			continue;
		}
		else {
			field[i][j] = -1;
			totalMines++;
			IncreaseNeighbours(i, j);
		}
	}

}

int MineSweeper::GetFieldSize() {
	return fieldSize;
}
int MineSweeper::GetMineCount() {
	return mineCount;
}

