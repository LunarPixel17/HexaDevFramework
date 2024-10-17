#include "Window.h"
#include "XAMLParser.h"
#include "Renderer.h"
#include <dwmapi.h>
#include <gdiplus.h>

#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

void EnableBlurBehind(HWND hwnd) {
    DWM_BLURBEHIND bb = { 0 };
    bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
    bb.fEnable = TRUE;
    bb.hRgnBlur = CreateRectRgn(0, 0, -1, -1);
    DwmEnableBlurBehindWindow(hwnd, &bb);
    DeleteObject(bb.hRgnBlur);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {  // Corrected signature
    switch (uMsg) {
    case WM_CREATE:
        EnableBlurBehind(hwnd);
        // Initialize GDI+ and parse XAML here if needed
        ParseXAML(L"window.xaml");
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RenderUI(hdc);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam); // Correctly passing all arguments
}

HWND CreateMainWindow(HINSTANCE hInstance, int nShowCmd) {
    const wchar_t CLASS_NAME[] = L"Custom XAML Window";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Custom XAML Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return NULL;
    }

    ShowWindow(hwnd, nShowCmd);
    return hwnd;
}
