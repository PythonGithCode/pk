#include <iostream>
#include <windows.h>
#include <string>

#ifdef _WIN32
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL
#endif

// A simple function to export
extern "C" EXPORT_SYMBOL void hello() {
    std::cout << "Hello from DLL!" << std::endl;
}

extern "C" __declspec(dllexport) void CALLBACK hellos(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
    MessageBox(NULL, "Hello, World!", "My DLL", MB_OK);
}

extern "C" __declspec(dllexport) void CALLBACK hello2(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
    MessageBox(NULL, hwnd, "My DLL", MB_OK);
}


extern "C" __declspec(dllexport) void CALLBACK hello3(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow, String text) {
    MessageBox(NULL, text, "My DLL", MB_OK);
}
