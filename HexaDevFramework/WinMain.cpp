/************************************************************************************
 *  Project: HexaDev Framework
 *  Author: X Bitlet
 *
 *  Copyright (c) 2024 X Bitlet. All rights reserved.
 *
 *  This code is part of the HexaDev Framework project.
 *  You are permitted to use, modify, and distribute this code
 *  under the terms of the license provided with the project.
 *
 *  Any unauthorized use of this code outside the terms of the
 *  license is strictly prohibited and may result in legal action.
 *
 *  For more information or to obtain a copy of the license, please
 *  contact X Bitlet at: contact@xbitlet.com
 ************************************************************************************/

#include <windows.h>
#include <dwmapi.h>
#include <gdiplus.h>
#include <string>

#pragma comment(lib, "Dwmapi.lib")
#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// GDI+ initialization
void InitGDIPlus(ULONG_PTR* gdiplusToken) {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(gdiplusToken, &gdiplusStartupInput, NULL);
}

void ShutdownGDIPlus(ULONG_PTR* gdiplusToken) {
    GdiplusShutdown(*gdiplusToken);
}

void EnableBlurBehind(HWND hwnd) {
    DWM_BLURBEHIND bb = { 0 };
    bb.dwFlags = DWM_BB_ENABLE | DWM_BB_BLURREGION;
    bb.fEnable = TRUE;
    bb.hRgnBlur = CreateRectRgn(0, 0, -1, -1); // Use a negative region to cover the whole window
    DwmEnableBlurBehindWindow(hwnd, &bb);
    DeleteObject(bb.hRgnBlur); // Clean up the created region
}

// Render the image onto the window
void RenderImage(HDC hdc, HWND hwnd, const std::wstring& imagePath) {
    Graphics graphics(hdc);
    Image image(imagePath.c_str());

    RECT rect;
    GetClientRect(hwnd, &rect);
    int width = rect.right - rect.left;
    int height = rect.bottom - rect.top;

    // Draw the image stretched to fit the window
    graphics.DrawImage(&image, 0, 0, width, height);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    static std::wstring imagePath = L"/mnt/data/image.png";  // Path to your uploaded image
    static ULONG_PTR gdiplusToken;

    switch (uMsg) {
    case WM_CREATE:
        EnableBlurBehind(hwnd);
        InitGDIPlus(&gdiplusToken);
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RenderImage(hdc, hwnd, imagePath);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        ShutdownGDIPlus(&gdiplusToken);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
    const wchar_t CLASS_NAME[] = L"Blurred Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Blurred Window with Image Background",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nShowCmd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

int main() {
    // Call the WinMain function to show the window
    return WinMain(GetModuleHandle(NULL), NULL, GetCommandLineA(), SW_SHOWNORMAL);
}
