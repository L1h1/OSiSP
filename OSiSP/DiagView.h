#pragma once
#include<windows.h>
#include <vector>
#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
using namespace std;

namespace diag {
	RECT rt;
	int circleCenterX, circleCenterY,circleRadius=150;

	//Для диаграмм - метка + значение
	vector<pair<string, int>> diagramAnalysisData = { {"1",1},{"2",2},{"3",1} };
	int dataCount;
	double _coef;
	vector<double> calculatedCoefs;//в РАДИАНАХ!
	vector<pair<int, int>> calculatedCoords;



	void CalcCoefs() {
		int sum = 0;
		for (int i = 0; i < dataCount; i++) {
			sum += diagramAnalysisData[i].second;
		}
		_coef = 2*M_PI / sum;

		for (int i = 0; i < dataCount; i++) {
			calculatedCoefs.push_back(_coef * diagramAnalysisData[i].second);
		}
		

		for (int i = 1; i < dataCount; i++) {
			calculatedCoefs[i] += calculatedCoefs[i - 1];
		}


	}
	void CalcCoords() {

		for (int i = 0; i < dataCount; i++) {

			int x = circleCenterX + circleRadius * cos(calculatedCoefs[i]);
			int y = circleCenterY + circleRadius * sin(calculatedCoefs[i]);

			calculatedCoords.push_back(make_pair(x, y));

		}
	}

	void PrepData() {
		circleCenterX = rt.right / 2-150;
		circleCenterY = rt.bottom / 2-150;
		dataCount = diagramAnalysisData.size();
		CalcCoefs();
		CalcCoords();
	}





	void Paint(HWND hwnd) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		//рамка
		Rectangle(hdc, rt.left + 10, rt.top + 10, rt.right - 10, rt.bottom - 10);

		//рамка под легенду
		Rectangle(hdc, circleCenterX+circleRadius+100, rt.top + 10, rt.right - 10, rt.bottom - 10);

		//скелет диаграммы
		DrawCircle(hdc, circleCenterX, circleCenterY, circleRadius, RGB(0, 0, 0), 4);

		//DrawLine(hdc, circleCenterX, circleCenterY, circleCenterX, circleCenterY-circleRadius, RGB(0, 0, 0), 4);

		for (int i = 0; i < dataCount; i++) {
			DrawLine(hdc, circleCenterX, circleCenterY, calculatedCoords[i].first, calculatedCoords[i].second, RGB(0, 0, 0), 4);
		}






		EndPaint(hwnd, &ps);
	}

}