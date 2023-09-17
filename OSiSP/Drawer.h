#pragma once
#include <windows.h>
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