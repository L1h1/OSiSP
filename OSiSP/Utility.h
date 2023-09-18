#pragma once
#include <windows.h>


using namespace std;
OPENFILENAME ofn;
wstring filename;

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
    MoveToEx(hdc, x, y, NULL); //сделать текущими координаты x,y

    HPEN newPen = CreatePen(PS_SOLID, width, color);
    HGDIOBJ oldPen = SelectObject(hdc, newPen);

    BOOL result = LineTo(hdc, x + 1, y + 1);
    SelectObject(hdc, oldPen);
    DeleteObject(newPen);

    return result;
}

//x,y- центр окружности x1,y1 - начальная точка дуги, x2,y2-конечная
BOOL DrawSector(HDC hdc, int x, int y, int radius, int x1, int y1, int x2, int y2, COLORREF color, int width) {
    
    HPEN newPen = CreatePen(PS_SOLID, width, RGB(0,0,0));
    HGDIOBJ oldPen = SelectObject(hdc, newPen);

    HBRUSH newBrush = CreateSolidBrush(color);
    HGDIOBJ oldBrush = SelectObject(hdc, newBrush);

    //Отрисовка сектора
    BOOL result = Pie(hdc, x - radius, y - radius, x + radius, y + radius, x1, y1, x2, y2);


    SelectObject(hdc, oldPen);
    DeleteObject(newPen);

    SelectObject(hdc, oldBrush);
    DeleteObject(newBrush);


    return result;

}



COLORREF RandomizeColor() {
    return RGB((BYTE)rand() % 255, (BYTE)rand() % 255, (BYTE)rand() % 255);
}


bool OpenFile(HWND hwnd) {

    const std::wstring title = L"Select a File";
    std::wstring src(MAX_PATH, L'\0');

    OPENFILENAMEW ofn = { };
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = TEXT("*.txt\0");
    ofn.lpstrFile = &src[0];  // use the std::wstring buffer directly
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = title.c_str();
    ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn))
    {
        filename = src;    //<----------Save filepath in global variable
        return true;
    }
    return false;
}