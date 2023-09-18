#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include <windows.h>
#include "FuncView.h"
#include "DiagView.h"
#include "Utility.h"
#include "resource.h"

using namespace std;




int screenWidth = 800, screenHeight = 800;
int ID;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);
//точка входа
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, (DLGPROC)DlgProc, 0);
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
        
        if (ID != IDCANCEL) {
            GetClientRect(hwnd, &func::rt);
            func::PrepData();
        }
        else {
            GetClientRect(hwnd, &diag::rt);
            diag::PrepData();
        }
        
        



        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {   
        if (ID != IDCANCEL) {
            func::Paint(hwnd);
        }
        else {
            diag::Paint(hwnd);
        }

    }
    return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}




BOOL CALLBACK DlgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_INITDIALOG:
        break;
    case WM_COMMAND:
        ID = LOWORD(wParam);
        
        OpenFile(hwnd);
        
        EndDialog(hwnd, 0);
        break;
    case WM_CLOSE:
        EndDialog(hwnd, 0);
        return 0;
    }
    return 0;
}




