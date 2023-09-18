#pragma once
#define _USE_MATH_DEFINES
#include<windows.h>
#include <vector>
#include <fstream>
#include<string>
#include <utility>
#include <iostream>
#include <format>
#include <math.h>
using namespace std;

namespace diag {
	RECT rt;
	int circleCenterX, circleCenterY,circleRadius=150;

	//Для диаграмм - метка + значение
	//Шаблонные данные в случае ошибки чтения пользовательских
	vector<pair<string, int>> diagramAnalysisData = { {"First",1},{"Second",8},{"Third",3},{"Fourth",5},{"Fifth",2},{"Sixth",7} };
	int dataCount;
	int sum=0;
	double _coef;
	vector<double> calculatedCoefs;//в РАДИАНАХ!
	vector<pair<int,int>> calculatedSubs;
	vector<pair<int, int>> calculatedCoords;
	vector<COLORREF> colors;



	void ReadDataFromFile() {

		fstream new_file;
		new_file.open(filename, ios::in);

		vector<pair<string, int>> temp;
		string input;
		bool isMark = true;
		int tvalue;
		string tmark;
		try {
			while (getline(new_file, input)) {
				if (isMark) {
					isMark = false;
					tmark = input;
				}
				else {
					isMark = true;
					tvalue = stoi(input);
					temp.push_back(make_pair(tmark, tvalue));
				}
			}
		}
		catch (exception ex) {

		}
		

		if(temp.size()>1)
		diagramAnalysisData = temp;

		new_file.close();
	}




	//углы поворота
	void CalcCoefs() {

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

			x = circleCenterX + circleRadius*1.2 * cos(calculatedCoefs[i]*0.9);
			y = circleCenterY + circleRadius*1.2 * sin(calculatedCoefs[i]*0.9);
			calculatedSubs.push_back(make_pair(x, y));

		}
	}
	
	void RandColors() {
		for (int i = 0; i < dataCount; i++) {
			colors.push_back(RandomizeColor());
		}
	}

	void PrepData() {
		ReadDataFromFile();
		srand(time(0));
		circleCenterX = rt.right / 2-150;
		circleCenterY = rt.bottom / 2-150;
		dataCount = diagramAnalysisData.size();
		CalcCoefs();
		CalcCoords();
		RandColors();
	}





	void Paint(HWND hwnd) {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);

		//рамка
		Rectangle(hdc, rt.left + 10, rt.top + 10, rt.right - 10, rt.bottom - 10);

		//рамка под легенду
		Rectangle(hdc, circleCenterX+circleRadius+100, rt.top + 10, rt.right - 10, rt.bottom - 10);


		string tempstr;
		wstring temp;
		TextOut(hdc, circleCenterX + circleRadius + 150, rt.top + 50, L"Легенда", 7);



		//скелет диаграммы + легенда
		//DrawCircle(hdc, circleCenterX, circleCenterY, circleRadius, RGB(0, 0, 0), 4);
		DrawSector(hdc, circleCenterX, circleCenterY, circleRadius,
			calculatedCoords[0].first, calculatedCoords[0].second, calculatedCoords[dataCount-1].first, calculatedCoords[dataCount-1].second, colors[0], 2);

		for (int i = 0; i < dataCount; i++) {

			if (i > 0) {
				DrawSector(hdc, circleCenterX, circleCenterY, circleRadius,
					calculatedCoords[i].first, calculatedCoords[i].second, calculatedCoords[i - 1].first, calculatedCoords[i - 1].second, colors[i], 2);
			}
			
			//собственно легенда
			HBRUSH newBrush = CreateSolidBrush(colors[i]);
			HGDIOBJ oldBrush = SelectObject(hdc, newBrush);
			Rectangle(hdc, circleCenterX + circleRadius + 150, (rt.top + 50) + (i + 1) * 50, circleCenterX + circleRadius + 170, (rt.top + 50) + (i + 1) * 50 + 20);
			tempstr = diagramAnalysisData[i].first+format(" - {}%",1.0 * diagramAnalysisData[i].second / sum * 100);
			temp = wstring(tempstr.begin(), tempstr.end());
			TextOut(hdc, circleCenterX + circleRadius + 175, (rt.top + 50) + (i + 1) * 50, temp.c_str(), temp.length());



			SelectObject(hdc, oldBrush);
			DeleteObject(newBrush);
		}
		




		EndPaint(hwnd, &ps);
	}

}