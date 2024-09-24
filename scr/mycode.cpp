#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

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

extern "C" __declspec(dllexport) void CALLBACK launchExe(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
    // Allocate a console for input/output (for DLLs running via rundll32)
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirect stdout to the console
    freopen("CONIN$", "r", stdin);    // Redirect stdin to the console

    // Ask the user for the executable path
    std::string exePath;
    std::cout << "Please enter the full path of the executable you want to launch: ";
    std::getline(std::cin, exePath);  // Get input from the user

    // Set up process startup info
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // Attempt to launch the executable
    if (CreateProcess(
        exePath.c_str(),    // Path to the executable
        NULL,               // Command line arguments
        NULL,               // Process handle not inheritable
        NULL,               // Thread handle not inheritable
        FALSE,              // Set handle inheritance to FALSE
        0,                  // No creation flags
        NULL,               // Use parent's environment block
        NULL,               // Use parent's starting directory 
        &si,                // Pointer to STARTUPINFO structure
        &pi                 // Pointer to PROCESS_INFORMATION structure
    )) {
        std::cout << "Executable launched successfully!\n";

        // Wait for the process to finish
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close process and thread handles
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
    } else {
        std::cerr << "Failed to launch executable. Error code: " << GetLastError() << "\n";
        std::cout << "Failed to launch executable. Error code: " << GetLastError() << "\n";
        std::string exePatsh;
        std::cout << "Enter To Contiune";
        std::getline(std::cin, exePatsh);  // Get input from the user
    }

    // Release the console (for DLLs)
    FreeConsole();
}


// Global variables
HINSTANCE hInst; // Current instance

// Function declarations
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ShowGraphics();

// Entry point for the DLL
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        hInst = hinstDLL; // Save instance handle
    }
    return TRUE;
}

// Function to show graphics
extern "C" __declspec(dllexport) void ShowGraphicsDLL() {
    ShowGraphics(); // Create a window and draw
}

void ShowGraphics() {
    // Register the window class
    const char CLASS_NAME[] = "Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInst;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                          // Optional window styles.
        CLASS_NAME,                 // Window class
        "Graphics Window",          // Window text
        WS_OVERLAPPEDWINDOW,        // Window style
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,                       // Parent window
        NULL,                       // Menu
        hInst,                     // Instance handle
        NULL                        // Additional application data
    );

    if (hwnd == NULL) {
        return;
    }

    ShowWindow(hwnd, SW_SHOWNORMAL);
    UpdateWindow(hwnd);

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

// Window procedure to handle messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Draw a rectangle
        Rectangle(hdc, 50, 50, 200, 200);

        EndPaint(hwnd, &ps);
    }
    return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}



extern "C" __declspec(dllexport) void CALLBACK launchPotato(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
    // Allocate a console for input/output (for DLLs running via rundll32)
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirect stdout to the console
    freopen("CONIN$", "r", stdin);    // Redirect stdin to the console
    

    // Ask the user for the executable path
    std::string text;
    std::cout << "Please Press Enter?" << endl;
    std::getline(std::cin, text);  // Get input from the user

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // check for enter
    if (text.length() > 0) {
        SetConsoleTextAttribute(hConsole, 2);
        std::cout << "Thank You!";
        SetConsoleTextAttribute(hConsole, 0);
    } else {
        std::cout << endl << "Bye";
    }

    // Release the console (for DLLs)
    FreeConsole();
}


