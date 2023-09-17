#pragma once
#include <vector>
#include <windows.h>
#include "Drawer.h"
using namespace std;

namespace func {
    
    RECT rt;

    //смещение оси х, чтобы видеть отрицательные значения
    int moveConst = 400;

    //Для Графиков (x,y) точки
    vector<pair<int, int>> funcAnalysisData = { {4,-6}, {1,1},{5,2},{10,4} };
    int dataCount;

    //установка предельных значений (в случае с y еще учет отрицательных)
    int maxDataValueX;
    int maxDataValueY;
    void SetMAXValueFuncAnalysis(bool byX) {
        int max_val = INT_MIN;
        int min_val = INT_MAX;
        if (byX) {
            for (int i = 0; i < dataCount; i++) {
                if (max_val < funcAnalysisData[i].first) {
                    max_val = funcAnalysisData[i].first;
                }
                if (min_val > funcAnalysisData[i].first) {
                    min_val = funcAnalysisData[i].first;
                }
            }
            maxDataValueX = max_val;
        }
        else {
            for (int i = 0; i < dataCount; i++) {
                if (max_val < funcAnalysisData[i].second) {
                    max_val = funcAnalysisData[i].second;
                }
                if (min_val > funcAnalysisData[i].second) {
                    min_val = funcAnalysisData[i].second;
                }
            }
            if (min_val != INT_MAX) {
                maxDataValueY = abs(min_val) > max_val ? abs(min_val) * 2 : max_val * 2;
            }
            else {
                maxDataValueY = max_val;
            }

        }

    }

    //цена деления + координаты точек
    int xSpan, ySpan;
    vector<pair<int, int>> coords;
    void SetCoordsANDSpanData() {
        //цена деления
        ySpan = (rt.bottom - rt.top - 100) / maxDataValueY;
        xSpan = (rt.right - rt.left - 100) / maxDataValueX;

        //Разметка делений
        for (int i = 0; i < dataCount; i++) {
            int x = funcAnalysisData[i].first * xSpan;
            int y = funcAnalysisData[i].second * ySpan;
            coords.push_back({ x,y });
        }
    }

    //Сортировка, поиск предельных значений, цены деления и координат в пределах окна соответственно
    void PrepData() {
        dataCount = funcAnalysisData.size();
        sort(funcAnalysisData.begin(), funcAnalysisData.end(), [](auto& left, auto& right) {
            return left.first < right.first;
            });
        SetMAXValueFuncAnalysis(true);
        SetMAXValueFuncAnalysis(false);
        SetCoordsANDSpanData();
    }


     void CALLBACK Paint(HWND hwnd) {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps); 

        //рамка
        Rectangle(hdc, rt.left + 10, rt.top + 10, rt.right - 10, rt.bottom - 10);

        //координатные оси
        DrawLine(hdc, rt.left + 50, rt.bottom - moveConst, rt.right - 50, rt.bottom - moveConst, RGB(0, 0, 0), 3);
        DrawLine(hdc, rt.left + 50, rt.bottom - 50, rt.left + 50, rt.top + 50, RGB(0, 0, 0), 3);

        

        //Разметка делений
        for (int i = 0; i < dataCount; i++) {
            //ось X
            DrawLine(hdc, (rt.left + 50) + coords[i].first, rt.bottom - moveConst + 5, (rt.left + 50) + coords[i].first, rt.bottom - moveConst - 5, RGB(0, 0, 0), 2);
            string tempstr = format("{}", funcAnalysisData[i].first);
            wstring temp = std::wstring(tempstr.begin(), tempstr.end());
            TextOut(hdc, (rt.left + 47) + coords[i].first, rt.bottom - moveConst + 10, temp.c_str(), 3);
            //ось Y
            DrawLine(hdc, rt.left + 45, (rt.bottom - moveConst) - coords[i].second, rt.left + 55, (rt.bottom - moveConst) - coords[i].second, RGB(0, 0, 0), 2);
            tempstr = format("{}", funcAnalysisData[i].second);
            temp = std::wstring(tempstr.begin(), tempstr.end());
            TextOut(hdc, rt.left + 20, (rt.bottom - moveConst - 10) - coords[i].second, temp.c_str(), 3);

        }

        //точки
        for (int i = 0; i < dataCount; i++) {
            DrawDot(hdc, rt.left + 50 + coords[i].first, rt.bottom - moveConst - coords[i].second, RGB(255, 0, 0), 6);
        }

        //линии
        int prevX = rt.left + 50, prevY = rt.bottom - moveConst;   //начало оси
        for (int i = 0; i < dataCount; i++) {
            DrawLine(hdc, prevX, prevY, rt.left + 50 + coords[i].first, rt.bottom - moveConst - coords[i].second, RGB(0, 0, 255), 3);
            prevX = (rt.left + 50) + coords[i].first;
            prevY = (rt.bottom - moveConst) - coords[i].second;
        }
        EndPaint(hwnd, &ps);
    }
}
