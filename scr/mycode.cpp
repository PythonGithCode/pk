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
    }

    // Release the console (for DLLs)
    FreeConsole();
}



