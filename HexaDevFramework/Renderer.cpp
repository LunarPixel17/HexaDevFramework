#include "Renderer.h"
#include "XAMLParser.h"
#include <windows.h>
#include <gdiplus.h>

using namespace Gdiplus;

// GDI+ initialization token
ULONG_PTR gdiplusToken;

// Function declarations
void InitGDIPlus();
void ShutdownGDIPlus();
void RenderUI(HDC hdc);

#pragma comment(lib, "gdiplus.lib")

// GDI+ initialization
void InitGDIPlus() {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
}

// GDI+ shutdown
void ShutdownGDIPlus() {
    GdiplusShutdown(gdiplusToken);
}

// Render function to draw UI elements
void RenderUI(HDC hdc) {
    Graphics graphics(hdc);
    const auto& elements = GetUIElements(); // Assuming this function is defined elsewhere

    for (const auto& element : elements) {
        if (element.type == L"Image") {
            Image image(element.imagePath.c_str());
            graphics.DrawImage(&image, element.x, element.y, element.width, element.height);
        }
        else if (element.type == L"Button") {
            graphics.FillRectangle(&SolidBrush(Color(255, 200, 200, 200)), element.x, element.y, element.width, element.height);
            graphics.DrawString(element.text.c_str(), -1, &Font(L"Arial", 12), PointF(static_cast<float>(element.x + 10), static_cast<float>(element.y + 10)), &SolidBrush(Color(255, 0, 0, 0)));
        }
    }
}

// Example of a Win32 Window Procedure
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        RenderUI(hdc); // Call your render function here
        EndPaint(hwnd, &ps);
    } break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}

// Main function
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize GDI+
    InitGDIPlus();

    // Your window creation code goes here
    // Example window class registration and creation

    // Enter the message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Shutdown GDI+
    ShutdownGDIPlus();

    return static_cast<int>(msg.wParam);
}
