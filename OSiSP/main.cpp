#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <utility>
#include <commctrl.h>
#include <string>
#include <vector>
#include <format>
#include <algorithm>
using namespace std;


int screenWidth = 800, screenHeight = 800;
int moveConst = 400;
RECT rt;


//Для диаграмм - метка + значение
vector<pair<string,int>> diagramAnalysisData = { {"1",1},{"2",2},{"3",3} };

//Для Графиков (x,y) точки
vector<pair<int,int>> funcAnalysisData = { {4,-6}, {1,1},{5,2},{10,4} };

//цена деления + координаты точек
void GetCoordsData();
int xSpan, ySpan;
vector<pair<int,int>> coords;


void GetMAXValueFunc(bool byX);
void GetMAXValueDiag();

int maxDataValueX;
int maxDataValueY;


//Отрисовка линии, работает от пера
BOOL DrawLine(HDC hdc, int xFrom, int yFrom, int xTo, int yTo, COLORREF color, int width);
BOOL DrawDot(HDC hdc, int x, int y, COLORREF color, int width);





LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//точка входа
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

    const wchar_t CLASS_NAME[] = L"Sample Window Class";


    //Регистрация окна

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;        //процедура окна
    wc.hInstance = hInstance;           //дескриптор приложения
    wc.lpszClassName = CLASS_NAME;      //класс окна

    RegisterClass(&wc);

    //Создание окна

    HWND hwnd = CreateWindowEx(
        0,                              //Доп. стили (прим. прозрачные окна)
        CLASS_NAME,                     //Имя класса
        L"Program",                     //Заголовок окна
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_CLIPCHILDREN,            //Стиль окна

        // Координаты, ширина, высота окна
        CW_USEDEFAULT, CW_USEDEFAULT, screenWidth, screenHeight,

        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // все работает на сообщениях
    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

//процедура для обработки тех самых сообщений
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_CREATE:
        GetClientRect(hwnd, &rt);
        sort(funcAnalysisData.begin(), funcAnalysisData.end(), [](auto& left, auto& right) {
            return left.first < right.first;
            });
        GetMAXValueFunc(true);
        GetMAXValueFunc(false);
        
        GetCoordsData();



        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {

        
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);    //контекст

        Rectangle(hdc, rt.left + 10, rt.top + 10, rt.right - 10, rt.bottom - 10);

        //координатные оси
        DrawLine(hdc, rt.left + 50, rt.bottom - moveConst, rt.right - 50, rt.bottom - moveConst, RGB(0, 0, 0), 3);
        DrawLine(hdc, rt.left + 50, rt.bottom - 50, rt.left + 50, rt.top + 50, RGB(0, 0, 0), 3);







        //число узловых точек
        int dataCount = funcAnalysisData.size();    
        


        //Разметка делений
        for (int i = 0; i < dataCount; i++) {
            //ось X
            DrawLine(hdc, (rt.left + 50) + coords[i].first, rt.bottom - moveConst+5, (rt.left + 50) + coords[i].first, rt.bottom - moveConst-5, RGB(0, 0, 0), 2);
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
            DrawDot(hdc, rt.left+50+coords[i].first, rt.bottom-moveConst-coords[i].second, RGB(255, 0, 0), 6);
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
    return 0;


    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL DrawLine(HDC hdc, int xFrom, int yFrom, int xTo, int yTo, COLORREF color, int width) {
    MoveToEx(hdc, xFrom, yFrom, NULL); //сделать текущими координаты xFrom, yFrom

    HPEN newPen = CreatePen(PS_SOLID, width, color);
    HGDIOBJ oldPen = SelectObject(hdc, newPen);

    BOOL result = LineTo(hdc, xTo, yTo);
    SelectObject(hdc, oldPen);
    DeleteObject(newPen);

    return result;
}

BOOL DrawDot(HDC hdc, int x, int y, COLORREF color, int width) {
    MoveToEx(hdc, x, y, NULL); //сделать текущими координаты xFrom, yFrom

    HPEN newPen = CreatePen(PS_SOLID, width, color);
    HGDIOBJ oldPen = SelectObject(hdc, newPen);

    BOOL result = LineTo(hdc, x + 1, y + 1);
    SelectObject(hdc, oldPen);
    DeleteObject(newPen);

    return result;
}




void GetMAXValueDiag() {
    int length = diagramAnalysisData.size();
    int max_val = INT_MIN;
    for (int i = 0; i < length; i++) {
        if (max_val < diagramAnalysisData[i].second) {
            max_val = diagramAnalysisData[i].second;
        }
    }
    maxDataValueY = max_val;
}
void GetMAXValueFunc(bool byX){
    int length = funcAnalysisData.size();
    int max_val = INT_MIN;
    int min_val = INT_MAX;
    if (byX) {
        for (int i = 0; i < length; i++) {
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
        for (int i = 0; i < length; i++) {
            if (max_val < funcAnalysisData[i].second) {
                max_val = funcAnalysisData[i].second;
            }
            if (min_val > funcAnalysisData[i].second) {
                min_val = funcAnalysisData[i].second;
            }
        }
        if (min_val != INT_MAX) {
            maxDataValueY=abs(min_val)>max_val?abs(min_val)*2:max_val*2;
        }
        else {
            maxDataValueY = max_val;
        }

    }

}
void GetCoordsData() {
    int dataCount = funcAnalysisData.size();

    //цена деления
    ySpan = (rt.bottom - rt.top - 100) / maxDataValueY;
    xSpan = (rt.right - rt.left - 100) / maxDataValueX;

    //Разметка делений
    for (int i = 0; i < dataCount; i++) {
        int x = funcAnalysisData[i].first * xSpan;
        int y = funcAnalysisData[i].second * ySpan;
        coords.push_back({x,y});
    }
}
