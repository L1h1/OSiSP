#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include <utility>
#include <commctrl.h>
#include <string>
#include <vector>
#include "Data.h"


using namespace std;


int screenWidth = 900, screenHeight = 700;
//x выступает в роли метки (номер месяца и т. п.)
//у - основное значение
vector<pair<int, int>> analysisData = {
                                        make_pair(1,1),
                                        make_pair(2,2),
                                        make_pair(3,3),
                                        make_pair(4,4),
                                        make_pair(5,5)
};




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

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        RECT rt;
        GetClientRect(hwnd, &rt);
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);    //контекст




        Rectangle(hdc, rt.left + 10, rt.top + 10, rt.right - 10, rt.bottom - 10);

        //координатные оси
        DrawLine(hdc, rt.left + 50, rt.bottom - 50, rt.right - 50, rt.bottom - 50, RGB(0, 0, 0), 3);
        DrawLine(hdc, rt.left + 50, rt.bottom - 50, rt.left + 50, rt.top + 50, RGB(0, 0, 0), 3);








        ////Разметка
        //int dataCount = analysisData.size();    //число узловых точек
        //
        ////цена деления
        //int ySpan = (rt.bottom - rt.top-100) / dataCount;
        //int xSpan = (rt.right - rt.left-100) / dataCount;

        //for (int i = 1; i < dataCount; i++) {
        //    DrawLine(hdc, (rt.left + 50) + i * xSpan, rt.bottom - 45, (rt.left + 50) + i * xSpan, rt.bottom - 55,RGB(0,0,0),2);
        //    DrawLine(hdc, rt.left + 45, (rt.bottom - 50)-i*ySpan, rt.left + 55, (rt.bottom - 50) - i * ySpan, RGB(0, 0, 0), 2);
        //}





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