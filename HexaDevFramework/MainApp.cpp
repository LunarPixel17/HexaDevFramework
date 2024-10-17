#include <windows.h>
#include <gdiplus.h>
#include "Window.h" // Include the header with CreateMainWindow declaration

using namespace Gdiplus;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nShowCmd) {
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;

    // Initialize GDI+
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    HWND hwnd = CreateMainWindow(hInstance, nShowCmd); // Call with both parameters
    if (hwnd == NULL) {
        return 0;
    }

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Shutdown GDI+
    GdiplusShutdown(gdiplusToken);
    return 0;
}
