#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include<format>
#include "Minesweeper.h"
#include <utility>
#include <commctrl.h>

MineSweeper game = MineSweeper();
int mineCount = game.GetMineCount();
int width, height;

//т.к. кнопки по умолчанию не обрабатывают нажатие ПКМ, переопределение процедуры
LRESULT CALLBACK SubclassWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
    switch (uMsg) {
    case WM_NCDESTROY:
        RemoveWindowSubclass(hWnd, SubclassWindowProc, uIdSubclass);
        break;
    
    case WM_RBUTTONDOWN:

        int bId = GetDlgCtrlID(hWnd);
        std::pair<int,int>coords = game.ExtractCoordinates(bId);
        if (game.markStatus[coords.first][coords.second]) {
            SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)(L" "));
            game.markStatus[coords.first][coords.second] = false;
        }
        else {
            SendMessage(hWnd, WM_SETTEXT, 0, (LPARAM)(L"*"));
            game.markStatus[coords.first][coords.second] = true;
        }
        
        break;
    }

    return DefSubclassProc(hWnd, uMsg, wParam, lParam);
}

bool UncoverTiles(HWND hwnd, int iX, int iY);
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//точка входа
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) 
{

    const wchar_t CLASS_NAME[] = L"Sample Window Class";
    width = game.GetFieldSize() * 30 + 16;
    height = game.GetFieldSize() * 30 + 140;



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
        CW_USEDEFAULT, CW_USEDEFAULT, width, height,

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
    {
        //Создание внутренностей окна
        int size = game.GetFieldSize();
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                
                
                HWND hButton = CreateWindow(
                    L"BUTTON",
                    reinterpret_cast<LPCWSTR>(L" "),
                    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON |BS_NOTIFY,
                    // x  y   w   h
                    j*30, 100+i*30, 30, 30, hwnd, reinterpret_cast<HMENU>(i*size+j), 0, 0
                );
                SetWindowSubclass(hButton, SubclassWindowProc, 1, 0);
            }
        }

    }

    return 0;



    case WM_COMMAND:
    {
        if (HIWORD(wParam) == WM_RBUTTONUP) {
            SendMessage(GetDlgItem(hwnd, LOWORD(wParam)), WM_SETTEXT, 0, LPARAM(L"*"));
        }
        else {
            pair<int, int> data = game.ExtractCoordinates(LOWORD(wParam)); //координаты игровом на поле | first = i | second = j
            UncoverTiles(hwnd, data.first, data.second);
        }


    }
    return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        SetBkColor(hdc, RGB(0, 255, 0));
        EndPaint(hwnd, &ps);
    }
    return 0;


    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void CALLBACK UncoverTile(HWND hwnd, int id,int iX,int iY,bool isZero=TRUE) {
    //открытие клетки
    game.fieldStatus[iX][iY] = true;
    if (game.field[iX][iY] == -1) {
        MessageBox(hwnd, L"Конец", L"Вы взорвались",MB_OK);
        PostQuitMessage(0);
    }
    game.unrevealedCount--;
    HWND bHWnd = GetDlgItem(hwnd,id);
    


    if (isZero) {
        SendMessage(bHWnd, WM_SETTEXT, 0, (LPARAM)(L"#"));
    }
    else {
        std::string tempstr = std::format("{}", game.field[iX][iY]);
        std::wstring temp = std::wstring(tempstr.begin(), tempstr.end());
        SendMessage(bHWnd, WM_SETTEXT, 0, (LPARAM)(temp.c_str()));
    }

    //сделать кнопку вскрытой клетки поля неактивной
    SendMessage(bHWnd, WM_ENABLE, 0, 0);    

    //конец игры?
    if (game.unrevealedCount == mineCount) {
        MessageBox(hwnd, L"Победа", L"Вы победили", MB_OK);
        PostQuitMessage(0);
    }
}


bool CALLBACK UncoverTiles(HWND hwnd,int iX, int iY) {
    //если уже открыта - уйти
    if (game.fieldStatus[iX][iY])
        return true;

    int fieldSize = game.GetFieldSize();

    //если клетка "пограничная" - вскрыть только ее
    if (game.field[iX][iY] != 0) {
        UncoverTile(hwnd, iX * fieldSize + iY, iX, iY,FALSE);
        return false;
    }


    UncoverTile(hwnd, iX * fieldSize + iY, iX, iY);
    //открытие соседних клеток
    for (int k = iX - 1; k <= iX + 1; k++) {
        if (k < 0 || k == fieldSize) continue;
        for (int p = iY - 1; p <= iY + 1; p++) {
            if (p < 0 || p == fieldSize) continue;
            if (k == iX && iY == p) continue;
            UncoverTiles(hwnd, k, p);
        }
    }
    return true;
}