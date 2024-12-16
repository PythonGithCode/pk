// #include <ctime>
#include <atomic>
#include <iostream>
#include <thread>
#include <string>
#include <mutex>

#include <math.h> 
#include <stdio.h>
#include <windows.h>

using namespace std;

#ifdef _WIN32
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL
#endif

// LARGE_INTEGER frequency;
// LARGE_INTEGER lastTime;

bool declareds = false;
// int frameCount = 0;

// const time_t current_time = time(NULL);


// double fps = 0;

// ::QueryPerformanceFrequency(&frequency);
// ::QueryPerformanceCounter(&lastTime);




// Global variables for shared state
std::atomic<bool> runConsole2(true); // Control flag to stop console thread
std::atomic<int> rectX2(100);    // X position of the rectangle
std::atomic<int> rectY2(100);    // Y position of the rectangle
std::atomic<int> moveSpeed2(5);    // Y position of the rectangle

std::mutex logMutex2; // To synchronize logging




// Console thread function
extern "C" __declspec(dllexport) void ConsoleThread2() {
    std::string input2;
    while (runConsole2) {
        // Log to console
        {
            std::lock_guard<std::mutex> lock(logMutex2);
            std::cout << "Enter new position for the shape (x y): ";
        }

        // Get user input (new position)
        int newX2, newY2;
        std::cin >> newX2 >> newY2;

        // Update shared variables
        rectX2.store(newX2);
        rectY2.store(newY2);

        // Log the updated position
        {
            std::lock_guard<std::mutex> lock(logMutex2);
            std::cout << "Updated position to: (" << newX2 << ", " << newY2 << ")\n";
        }
    }
}

// Windows Procedure function
extern "C" __declspec(dllexport) LRESULT CALLBACK WindowProc2(HWND hwnd2, UINT uMsg2, WPARAM wParam2, LPARAM lParam2) {
    switch (uMsg2) {
        case WM_PAINT: {
            PAINTSTRUCT ps2;
            HDC hdc2 = BeginPaint(hwnd2, &ps2);

            // Draw a rectangle using the updated values from the console
            Rectangle(hdc2, rectX2.load(), rectY2.load(), rectX2.load() + 100, rectY2.load() + 100);

            EndPaint(hwnd2, &ps2);
            return 0;
        }
        case WM_KEYDOWN: {
            // Handle movement based on WASD keys

            if (GetAsyncKeyState('W') & 0x8000) { // Move up
                rectY2.store(rectY2.load() - moveSpeed2.load());
                // redraw = true;
            }
            if (GetAsyncKeyState('S') & 0x8000) { // Move down
                rectY2.store(rectY2.load() + moveSpeed2.load());
                // redraw = true;
            }
            if (GetAsyncKeyState('A') & 0x8000) { // Move left
                rectX2.store( rectX2.load() - moveSpeed2.load() );
                // redraw = true;
            }
            if (GetAsyncKeyState('D') & 0x8000) { // Move right
                rectX2.store(rectX2.load() + moveSpeed2.load());
                // redraw = true;
            }

            if (GetAsyncKeyState('J') & 0x8000) { // More speed
                moveSpeed2.store(moveSpeed2.load() + 1);
                // redraw = true;
            }
            if (GetAsyncKeyState('K') & 0x8000) { // Less speed
                moveSpeed2.store(moveSpeed2.load() - 1);
                // redraw = true;
            }

            if (GetAsyncKeyState('L') & 0x8000) { // start over stuff
                moveSpeed2.store(5); // Movement speed
                rectX2.store(50);
                rectY2.store(50);
                
                // redraw = true;
            }
            
            // Redraw the window if any key was pressed
            InvalidateRect(hwnd2, NULL, TRUE);

            return 0;
        }
        case WM_DESTROY: {
            runConsole2 = false; // Signal console thread to stop
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd2, uMsg2, wParam2, lParam2);
}

extern "C" __declspec(dllexport) int WINAPI WinMain2(HINSTANCE hInstance2, HINSTANCE hPrevInstance2, PSTR lpCmdLine2, int nCmdShow2) {
    // Create the console window
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    freopen("CONIN$", "r", stdin);

    // Create and run the console thread
    std::thread consoleThread2(ConsoleThread2);

    // Initialize and create a window
    WNDCLASS wc2 = { };
    wc2.lpfnWndProc = WindowProc2;
    wc2.hInstance = hInstance2;
    wc2.lpszClassName = "GraphicsWindowClass";
    RegisterClass(&wc2);

    HWND hwnd2 = CreateWindowEx(0, "GraphicsWindowClass", "Graphics Window", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, nullptr, nullptr, hInstance2, nullptr);

    if (hwnd2 == nullptr) return 0;

    ShowWindow(hwnd2, nCmdShow2);

    // Main message loop
    MSG msg2 = { };
    while (GetMessage(&msg2, nullptr, 0, 0)) {
        TranslateMessage(&msg2);
        DispatchMessage(&msg2);
    }

    // Clean up
    runConsole2 = false;
    consoleThread2.join();

    return 0;
}



// Global variables
int g_variable = 42;
HWND g_textBox;     // Handle for the text box
HWND g_staticText;  // Handle for the static text

// Forward declaration of window procedure
extern "C" __declspec(dllexport) LRESULT CALLBACK WindowProc3(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// Function to update the static text label
extern "C" __declspec(dllexport) void UpdateStaticText() {
    std::string text = "Current Value: " + std::to_string(g_variable);
    SetWindowText(g_staticText, text.c_str());
}

// Function that handles the button click and logs to the console
extern "C" __declspec(dllexport) void LogToConsole() {
    std::cout << "Button Clicked! Current value of g_variable: " << g_variable << std::endl;
}

// Function to handle text input and update the variable
extern "C" __declspec(dllexport) void UpdateVariableFromTextBox() {
    char buffer[256];
    GetWindowText(g_textBox, buffer, sizeof(buffer));
    g_variable = atoi(buffer);  // Convert text to int
    UpdateStaticText();
}

// Entry point of the Windows application
extern "C" __declspec(dllexport) int WINAPI WinMain3(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nCmdShow) {
    // Allocate console window for logging
    AllocConsole();
    freopen("CONOUT$", "w", stdout);
    std::cout << "Console window initialized!" << std::endl;

    // Register the window class
    const char* CLASS_NAME = "Sample Window Class";

    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc3;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles
        CLASS_NAME,                     // Window class
        "GUI Window",                   // Window title
        WS_OVERLAPPEDWINDOW,            // Window style
        CW_USEDEFAULT, CW_USEDEFAULT,   // Window position
        300, 200,                       // Window size
        NULL,                           // Parent window
        NULL,                           // Menu
        hInstance,                      // Instance handle
        NULL                            // Additional application data
    );

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

// Window procedure to handle window messages
extern "C" __declspec(dllexport) LRESULT CALLBACK WindowProc3(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE:
            // Create a button
            CreateWindow(
                "BUTTON", "Log to Console",
                WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
                50, 50, 150, 30, hwnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            // Create a static text to display variable
            g_staticText = CreateWindow(
                "STATIC", "Current Value: 42",
                WS_VISIBLE | WS_CHILD,
                50, 100, 150, 30, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            // Create a text box for variable input
            g_textBox = CreateWindow(
                "EDIT", "42",
                WS_VISIBLE | WS_CHILD | WS_BORDER | ES_NUMBER,
                50, 150, 150, 20, hwnd, NULL, (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE), NULL);

            break;

        case WM_COMMAND:
            if (LOWORD(wParam) == 1) {
                // Button clicked: Log to the console
                LogToConsole();
            }
            break;

        case WM_KEYDOWN:
            if (wParam == VK_RETURN) {
                // Enter key pressed: Update variable from text box
                UpdateVariableFromTextBox();
            }
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;

        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return 0;
}




LARGE_INTEGER frequency;       // Holds ticks per second
LARGE_INTEGER startTime;       // Start time of the current frame
LARGE_INTEGER endTime;         // End time of the current frame
int frameCount = 0;            // Number of frames counted in the current second
float fps = 0.0f;              // FPS value to display
float elapsedTime = 0.0f;      // Elapsed time for current frame
float fpsUpdateInterval = 1.0f;// Interval to update the FPS display (1 second)




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


// Global variables for rectangle position
bool constGravity = false;
bool isGravity = false;
bool isJump = false;

short typeOfShape = 0;
short timeFalling = 0;

int moveSpeed = 5; // Movement speed
int rectX = 50;
int rectY = 50;
int rectWidth = 150;
int rectHeight = 150;
// int windowWidth;
// int windowHeight;
double d_rectY = rectY;



HINSTANCE hInst; // Current instance

// Function declarations
extern "C" __declspec(dllexport) LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern "C" __declspec(dllexport) void ShowGraphics();

// Entry point for the DLL
extern "C" __declspec(dllexport) BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) {
    if (fdwReason == DLL_PROCESS_ATTACH) {
        hInst = hinstDLL; // Save instance handle
    }
    return TRUE;
}

// Function to show graphics
extern "C" __declspec(dllexport) void ShowGraphicsDLL() {
    ShowGraphics(); // Create a window and draw
}

extern "C" __declspec(dllexport) void ShowGraphics() {
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


    
    // RECT windowRect;
    // GetClientRect(hwnd, &windowRect);
    
    // int windowWidth = windowRect.right - windowRect.left;
    // int windowHeight = windowRect.bottom - windowRect.top;
    // declareds = true;

    // Main message loop
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    
        // Get the current time
        QueryPerformanceCounter(&endTime);
    
        // Calculate the elapsed time in seconds
        elapsedTime = static_cast<float>(endTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
    
        // Increment frame count
        frameCount++;
    
        // If one second has passed, calculate the FPS
        if (elapsedTime >= fpsUpdateInterval) {
            fps = frameCount / elapsedTime;  // Frames per second
            frameCount = 0;                  // Reset frame count
            QueryPerformanceCounter(&startTime);  // Reset start time
        }
    
        // Use the fps value for display
        // You can display the fps in your WM_PAINT section using TextOut function
    }
}

// Handle the window messages, including keyboard input
extern "C" __declspec(dllexport) LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    
    
    switch (uMsg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

    
    
        // Display FPS as text in the window
        // fps = frameCount / (timeNow - current_time);
        
        // char fpsText[64];
        
        // sprintf(fpsText, "FPS: %.2f", fps);


        // draw shapes
        if ( typeOfShape == 0 ) {
            // Draw the rectangle at the current position
            Rectangle(hdc, rectX, rectY, rectX + rectWidth, rectY + rectHeight);
        } else if ( typeOfShape == 1 ) {
            Ellipse(hdc, rectX, rectY, rectX + rectWidth, rectY + rectHeight);
        } else {
            Rectangle(hdc, rectX, rectY, rectX + rectWidth, rectY + rectHeight);
        }

        // text
        // char* text = "words";
        short posTextX = 10, posTextY = 30;
        
        char fpsText[64];
        
        //text coulr
        SetTextColor(hdc, RGB(255,255,255));
        SetBkColor(hdc, RGB(0,0,0));
        
        // draw text
        // TextOut(hdc, posTextX, posTextY, text, strlen(text));
        // TextOut(hdc, posTextX, posTextY, fpsText, strlen(fpsText));
        // Draw FPS
        sprintf(fpsText, "FPS: %.2f", fps);  // Convert fps to string
        TextOut(hdc, 10, 30, fpsText, strlen(fpsText));


        
        EndPaint(hwnd, &ps);
    }
    return 0;

    case WM_KEYDOWN: {
        // Handle movement based on WASD keys
        // bool redraw = false;
        RECT windowRect;
        GetClientRect(hwnd, &windowRect);
        int windowHeight = windowRect.bottom - windowRect.top;   
        // int windowWidth = windowRect.right - windowRect.left;
        

        if (GetAsyncKeyState('W') & 0x8000) { // Move up
            rectY -= moveSpeed;
            // redraw = true;
        }
        if (GetAsyncKeyState('S') & 0x8000) { // Move down
            rectY += moveSpeed;
            // redraw = true;
        }
        if (GetAsyncKeyState('A') & 0x8000) { // Move left
            rectX -= moveSpeed;
            // redraw = true;
        }
        if (GetAsyncKeyState('D') & 0x8000) { // Move right
            rectX += moveSpeed;
            // redraw = true;
        }
        
        if (GetAsyncKeyState('Q') & 0x8000) { // Move shape
            typeOfShape += 1;
            // redraw = true;
        }
        if (GetAsyncKeyState('E') & 0x8000) { // less shape
            typeOfShape -= 1;
            // redraw = true;
        }

         if (GetAsyncKeyState('J') & 0x8000) { // More speed
            moveSpeed += 1;
            // redraw = true;
        }
        if (GetAsyncKeyState('K') & 0x8000) { // Less speed
            moveSpeed -= 1;
            // redraw = true;
        }

        if (GetAsyncKeyState('L') & 0x8000) { // start over stuff
            isJump = false;
            isGravity = false;
            typeOfShape = 0;
            timeFalling = 0;
            moveSpeed = 5; // Movement speed
            rectX = 50;
            rectY = 50;
            d_rectY = rectY;
            constGravity = false;
            
            // redraw = true;
        }
        
        if (GetAsyncKeyState('G') & 0x8000) { // Is gravity
            isGravity = !isGravity;
            // redraw = true;
        }

        if (GetAsyncKeyState('V') & 0x8000) { // Is gravity const
            constGravity = !constGravity;
            // redraw = true;
        }
        
        if (GetAsyncKeyState('C') & 0x8000) { // Is gravity
            isJump = true;
            timeFalling = 0;
            rectY -= 7 * moveSpeed;
            // redraw = true;
        }


        // Gravity
        if (isGravity & !constGravity) {
            timeFalling++;
            d_rectY = rectY; // this might work
            d_rectY += 0.075 * ( timeFalling * timeFalling );
            rectY = (int) round(d_rectY);

            RECT windowRect;
            GetClientRect(hwnd, &windowRect);
            int windowHeight = windowRect.bottom - windowRect.top;   
            // int windowWidth = windowRect.right - windowRect.left;
            
            // IS it at the bottom
            if ( rectY + 5 > ( windowHeight - rectHeight ) ) { // offset
                rectY = windowHeight - rectHeight; // offset
                timeFalling = 0;
                isJump = false;
            }
            // redraw = true;+
        }
        
        // Redraw the window if any key was pressed
        InvalidateRect(hwnd, NULL, TRUE);
    }
    return 0;

    case WM_CREATE:
        QueryPerformanceFrequency(&frequency);

        // Get the starting time
        QueryPerformanceCounter(&startTime);

        return 0;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        if (isGravity & constGravity) {
            timeFalling++;
            d_rectY = rectY; // this might work
            d_rectY += (0.00075) * ( timeFalling * timeFalling );
            rectY = (int) round(d_rectY);

            RECT windowRect;
            GetClientRect(hwnd, &windowRect);
            int windowHeight = windowRect.bottom - windowRect.top;   
            // int windowWidth = windowRect.right - windowRect.left;
            
            // IS it at the bottom
            if ( rectY + 5 > ( windowHeight - rectHeight ) ) { // offset
                rectY = windowHeight - rectHeight; // offset
                timeFalling = 0;
                isJump = false;
            }
            // Redraw the window if any key was pressed
            InvalidateRect(hwnd, NULL, TRUE);
            // redraw = true;+
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
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
        std::cout << "Thank You!" << std::endl;
        SetConsoleTextAttribute(hConsole, 0);
    } else {
        std::cout << endl << "Bye" << endl;
    }


    // end
    std::cout << "Exit Console." << endl;
    std::getline(std::cin, text);  // Get input from the user 
    
    // Release the console (for DLLs)
    FreeConsole();
}




extern "C" __declspec(dllexport) void LaunchExeIndirectly()
{
    // Path to the executable you want to run
    LPCSTR exePath = "C:\\path\\to\\your\\program.exe";  // Change to your desired .exe

    // Use ShellExecute to launch the executable
    HINSTANCE result = ShellExecute(
        NULL,           // Parent window (NULL if not applicable)
        "open",         // Operation to perform (open the EXE)
        exePath,        // Path to the executable
        NULL,           // Parameters (if any)
        NULL,           // Directory (use default)
        SW_SHOWNORMAL   // How to show the window (normal)
    );

    if ((int)result <= 32) 
    {
        // An error occurred (ShellExecute returns a value <= 32 on failure)
        MessageBox(NULL, "Failed to launch EXE", "Error", MB_OK);
    }
}

extern "C" __declspec(dllexport) void CALLBACK LaunchExeIndirectly2(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
    // Allocate a console for input/output (for DLLs running via rundll32)
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirect stdout to the console
    freopen("CONIN$", "r", stdin);    // Redirect stdin to the console

    // Ask the user for the executable path
    std::string exePaths;
    std::cout << "Please enter the full path of the executable you want to launch: ";
    getline(std::cin, exePaths);  // Get input from the user

    // Set up process startup info
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // Attempt to launch the executable
    // Path to the executable you want to run
    LPCSTR exePath = exePaths.c_str();  // Change to your desired .exe

    // Use ShellExecute to launch the executable
    HINSTANCE result = ShellExecute(
        NULL,           // Parent window (NULL if not applicable)
        "open",         // Operation to perform (open the EXE)
        exePath,        // Path to the executable
        NULL,           // Parameters (if any)
        NULL,           // Directory (use default)
        SW_SHOWNORMAL   // How to show the window (normal)
    );

       if ((int)result <= 32) 
    {
        // An error occurred, get the error code
        DWORD errorCode = GetLastError();  // Get the last error code

        // You can convert this error code to a more readable message
        std::string errorMessage = "Failed to launch EXE. Error Code: " + std::to_string(errorCode);
        MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK);
    }
    else
    {
        // Success (the .exe was launched)
        MessageBoxA(NULL, "EXE Launched Successfully", "Success", MB_OK);
    }

    // Release the console (for DLLs)
    FreeConsole();
}

#include <windows.h>
#include <string>
#include <iostream>

extern "C" __declspec(dllexport) void LaunchExeUsingShellExecuteEx(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
     // Allocate a console for input/output (for DLLs running via rundll32)
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirect stdout to the console
    freopen("CONIN$", "r", stdin);    // Redirect stdin to the console

    // Ask the user for the executable path
    std::string exePath;
    std::cout << "Please enter the full path of the executable you want to launch: ";
    getline(std::cin, exePath);  // Get input from the user

    // Set up process startup info
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    // std::string exePath = "C:\\path\\to\\your\\program.exe";  // Replace with your executable path

    SHELLEXECUTEINFO shExecInfo = {0};
    shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
    shExecInfo.hwnd = NULL;
    shExecInfo.lpVerb = "open";            // Operation (open the EXE)
    shExecInfo.lpFile = exePath.c_str();  // Path to the executable
    shExecInfo.lpParameters = NULL;       // Parameters (if any)
    shExecInfo.lpDirectory = NULL;        // Current directory
    shExecInfo.nShow = SW_SHOWNORMAL;     // Show window normally

    if (!ShellExecuteEx(&shExecInfo)) 
    {
        DWORD errorCode = GetLastError();
        std::string errorMessage = "Failed to launch EXE. Error Code: " + std::to_string(errorCode);
        MessageBoxA(NULL, errorMessage.c_str(), "Error", MB_OK);
    }
    else 
    {
        MessageBoxA(NULL, "EXE Launched Successfully", "Success", MB_OK);
    }
    // Release the console (for DLLs)
    FreeConsole();
}

// #include <windows.h>
// #include <taskschd.h>
// #include <comdef.h>
// #include <iostream>

// #pragma comment(lib, "taskschd.lib")
// #pragma comment(lib, "comsupp.lib")

// extern "C" __declspec(dllexport) void CreateTaskToRunExe(const std::wstring& exePath)
// {
//     // Initialize COM
//     HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
//     if (FAILED(hr)) {
//         std::cout << "COM initialization failed" << std::endl;
//         return;
//     }

//     ITaskService* pService = NULL;
//     ITaskFolder* pRootFolder = NULL;
//     ITaskDefinition* pTask = NULL;
//     IRegistrationInfo* pRegInfo = NULL;
//     ITaskActionCollection* pActionCollection = NULL;
//     IExecAction* pExecAction = NULL;

//     // Connect to the task scheduler service
//     hr = CoCreateInstance(CLSID_TaskScheduler, NULL, CLSCTX_INPROC_SERVER, IID_ITaskService, (void**)&pService);
//     if (FAILED(hr)) {
//         std::cout << "Failed to connect to Task Scheduler" << std::endl;
//         CoUninitialize();
//         return;
//     }

//     hr = pService->Connect(_variant_t(), _variant_t(), _variant_t(), _variant_t());
//     if (FAILED(hr)) {
//         std::cout << "Failed to connect to Task Scheduler service" << std::endl;
//         pService->Release();
//         CoUninitialize();
//         return;
//     }

//     // Get the root folder
//     hr = pService->GetFolder(_bstr_t("\\"), &pRootFolder);
//     if (FAILED(hr)) {
//         std::cout << "Failed to get root folder" << std::endl;
//         pService->Release();
//         CoUninitialize();
//         return;
//     }

//     // Create the task definition
//     hr = pService->NewTask(0, &pTask);
//     if (FAILED(hr)) {
//         std::cout << "Failed to create new task" << std::endl;
//         pRootFolder->Release();
//         pService->Release();
//         CoUninitialize();
//         return;
//     }

//     // Set registration info
//     hr = pTask->get_RegistrationInfo(&pRegInfo);
//     if (FAILED(hr)) {
//         std::cout << "Failed to get task registration info" << std::endl;
//         pTask->Release();
//         pRootFolder->Release();
//         pService->Release();
//         CoUninitialize();
//         return;
//     }
//     pRegInfo->put_Author(_bstr_t("System"));

//     // Create an action for the task to run an executable
//     hr = pTask->get_Actions(&pActionCollection);
//     if (FAILED(hr)) {
//         std::cout << "Failed to get action collection" << std::endl;
//         pTask->Release();
//         pRootFolder->Release();
//         pService->Release();
//         CoUninitialize();
//         return;
//     }

//     hr = pActionCollection->Create(TASK_ACTION_EXEC, &pExecAction);
//     if (FAILED(hr)) {
//         std::cout << "Failed to create exec action" << std::endl;
//         pActionCollection->Release();
//         pTask->Release();
//         pRootFolder->Release();
//         pService->Release();
//         CoUninitialize();
//         return;
//     }

//     // Set the executable path
//     pExecAction->put_Path(_bstr_t(exePath.c_str()));

//     // Register the task (run immediately)
//     hr = pRootFolder->RegisterTaskDefinition(
//         _bstr_t("MyTask"),
//         pTask,
//         TASK_CREATE_OR_UPDATE,
//         _variant_t(),
//         _variant_t(),
//         TASK_LOGON_INTERACTIVE,
//         _variant_t(),
//         NULL);
//     if (FAILED(hr)) {
//         std::cout << "Failed to register task" << std::endl;
//         pExecAction->Release();
//         pActionCollection->Release();
//         pTask->Release();
//         pRootFolder->Release();
//         pService->Release();
//         CoUninitialize();
//         return;
//     }

//     std::cout << "Task scheduled to run the EXE." << std::endl;

//     // Clean up
//     pExecAction->Release();
//     pActionCollection->Release();
//     pTask->Release();
//     pRootFolder->Release();
//     pService->Release();
//     CoUninitialize();
// }

// extern "C" __declspec(dllexport) void RunExeViaTaskScheduler(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)

// {
//      // Allocate a console for input/output (for DLLs running via rundll32)
//     AllocConsole();
//     freopen("CONOUT$", "w", stdout);  // Redirect stdout to the console
//     freopen("CONIN$", "r", stdin);    // Redirect stdin to the console

//     // Ask the user for the executable path
//     std::wstring exePath;
//     std::cout << "Please enter the full path of the executable you want to launch: ";
//     getline(std::cin, exePath);  // Get input from the user

//     // Set up process startup info
//     STARTUPINFO si = { sizeof(si) };
//     PROCESS_INFORMATION pi;
    
//     // std::wstring exePath = L"C:\\path\\to\\your\\program.exe";  // Path to your EXE
//     // Release the console (for DLLs)
//     FreeConsole();
//     CreateTaskToRunExe(exePath);
    
// }


#include <windows.h>
#include <string>
#include <iostream>

// Function to run the EXE via a system process
extern "C" __declspec(dllexport) void RunExeIndirectly(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow)
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirect stdout to the console
    freopen("CONIN$", "r", stdin);    // Redirect stdin to the console

    // Ask the user for the executable path
    std::string exePath;
    std::cout << "Please enter the full path of the executable you want to launch: ";
    getline(std::cin, exePath);  // Get input from the user

    // Set up process startup info
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    // std::wstring exePath = L"C:\\path\\to\\your\\program.exe";  // Path to your EXE
    // Release the console (for DLLs)
    FreeConsole();
    // Path to the executable you want to run
    // std::string exePath = "C:\\path\\to\\your_program.exe";  // Change this to the path of your .exe

    // Create a command string for cmd.exe to run the EXE
    std::string command = "cmd.exe /C start " + exePath;

    // Call CreateProcess to invoke cmd.exe with the desired command
     si = {0};
     pi = {0};

    // Initialize STARTUPINFO structure
    si.cb = sizeof(STARTUPINFO);

    if (!CreateProcess(
        "C:\\Windows\\System32\\cmd.exe",  // Path to cmd.exe
        const_cast<char*>(command.c_str()),  // Command to run (start your EXE)
        NULL,  // Process handle not inheritable
        NULL,  // Thread handle not inheritable
        FALSE, // Do not inherit handles
        CREATE_NEW_CONSOLE, // Create a new console window
        NULL,  // Use the environment of the parent process
        NULL,  // Current directory (NULL for default)
        &si,   // Pointer to STARTUPINFO structure
        &pi    // Pointer to PROCESS_INFORMATION structure
    )) {
        DWORD errorCode = GetLastError();
        std::cout << "CreateProcess failed with error code " << errorCode << std::endl;
    } else {
        std::cout << "Executable launched successfully via cmd.exe." << std::endl;
    }
}

#include <windows.h>
#include <shellapi.h>
#include <iostream>

// Function to execute an executable using ShellExecuteEx
extern "C" __declspec(dllexport) void RunExeUsingShellExecute()
{
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirect stdout to the console
    freopen("CONIN$", "r", stdin);    // Redirect stdin to the console

    // Ask the user for the executable path
    std::string exePaths;
    std::cout << "Please enter the full path of the executable you want to launch: ";
    getline(std::cin, exePaths);  // Get input from the user

    // Set up process startup info
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;
    
    // std::wstring exePath = L"C:\\path\\to\\your\\program.exe";  // Path to your EXE
    // Release the console (for DLLs)
    
    // Path to the executable you want to run
    // const wchar_t* exePath = exePaths; // Update with your EXE path

    SHELLEXECUTEINFO shExecInfo = {0};
    shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
    shExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;  // Don't close the process immediately
    shExecInfo.hwnd = NULL;
    shExecInfo.lpVerb = "open";            // Operation (open the EXE)
    shExecInfo.lpFile = exePaths.c_str();  // Path to the executable
    shExecInfo.lpParameters = NULL;  // No additional parameters
    shExecInfo.lpDirectory = "C:/Program Files/";  // Default directory
    shExecInfo.nShow = SW_SHOW;  // Show the window
    shExecInfo.hInstApp = NULL;

    // Execute the program
    if (ShellExecuteEx(&shExecInfo)) {
        std::cout << "Executable launched successfully." << std::endl;
    } else {
        DWORD error = GetLastError();
        std::cout << "Error launching executable. Error code: " << error << std::endl;
    }
    FreeConsole();
}

#include <windows.h>
#include <iostream>
#include <string>

extern "C" __declspec(dllexport) void RunExeUsingLoadLibrary()
{
    // Get user input for executable path
        AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirect stdout to the console
    freopen("CONIN$", "r", stdin);    // Redirect stdin to the console

    // Ask the user for the executable path
    std::string exePath;
    std::cout << "Please enter the full path of the executable you want to launch: ";
    getline(std::cin, exePath);  // Get input from the user

    // Set up process startup info
    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    if (exePath.empty()) {
        std::cout << L"Error: No path provided." << std::endl;
        return;
    }

    // Try loading the executable as a DLL (in case it's a DLL file, adjust this method as necessary)
    HMODULE hModule = LoadLibrary(exePath.c_str());

     if (hModule == NULL) {
        DWORD dwError = GetLastError();
        std::cout << "Failed to load the executable as a DLL. Error code: " << dwError << std::endl;
        // Print the system error message for the last error code
        LPVOID msgBuffer;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
            NULL, dwError, 0, (LPSTR)&msgBuffer, 0, NULL);
        std::cout << "Error Message: " << (LPSTR)msgBuffer << std::endl;
        LocalFree(msgBuffer);
    } else {
        std::cout << "Executable loaded successfully." << std::endl;
        std::cout << "Press enter to contiues";
        getline(std::cin, exePath);
        FreeLibrary(hModule);  // Unload the DLL after usage
    }
    
    std::cout << "Press enter to quitsda";
    getline(std::cin, exePath);
    FreeConsole();
}

#include <windows.h>
#include <iostream>
#include <string>

typedef NTSTATUS(WINAPI* NtCreateProcessEx_t)(
    OUT PHANDLE ProcessHandle,
    IN ACCESS_MASK DesiredAccess,
    IN POBJECT_ATTRIBUTES ObjectAttributes,
    IN HANDLE ParentProcess,
    IN BOOLEAN InheritObjectTable,
    IN ULONG Flags,
    IN PVOID SectionHandle,
    IN PVOID DebugPort,
    IN PVOID ExceptionPort
);

extern "C" __declspec(dllexport) void LaunchExeIndirectly3()
{
    // Get user input for executable path
    AllocConsole();
    freopen("CONOUT$", "w", stdout);  // Redirect stdout to the console
    freopen("CONIN$", "r", stdin);    // Redirect stdin to the console

    // Get user input for executable path
    std::string exePath;
    std::cout << "Enter the path to the executable you want to run: ";
    std::getline(std::cin, exePath);

    if (exePath.empty()) {
        std::cout << "Error: No path provided." << std::endl;
        return;
    }

    // Load the NTDLL library (where NtCreateProcessEx is located)
    HMODULE hNtDll = LoadLibrary("ntdll.dll");
    if (!hNtDll) {
        std::cout << "Error: Failed to load ntdll.dll" << std::endl;
        return;
    }

    // Get the NtCreateProcessEx function address
    NtCreateProcessEx_t NtCreateProcessEx = (NtCreateProcessEx_t)GetProcAddress(hNtDll, "NtCreateProcessEx");

    if (!NtCreateProcessEx) {
        std::cout << "Error: Failed to get address of NtCreateProcessEx" << std::endl;
        return;
    }

    // Prepare the parameters for NtCreateProcessEx (this is a simplified example)
    HANDLE hProcess = NULL;
    OBJECT_ATTRIBUTES objAttributes = { 0 };
    UNICODE_STRING uniExePath;
    RtlInitUnicodeString(&uniExePath, exePath.c_str());

    // Call NtCreateProcessEx
    NTSTATUS status = NtCreateProcessEx(
        &hProcess, 
        PROCESS_ALL_ACCESS, 
        &objAttributes, 
        NULL,  // Parent process (NULL for no parent)
        FALSE, 
        0,     // Flags
        NULL,  // Section handle
        NULL,  // Debug port
        NULL   // Exception port
    );

    if (status == 0) { // STATUS_SUCCESS
        std::cout << "Executable launched successfully using NtCreateProcessEx!" << std::endl;
    } else {
        std::cout << "Error launching executable via NtCreateProcessEx!" << std::endl;
    }

    // Clean up
    FreeLibrary(hNtDll);
    std::cout << "Press enter to quitsda";
    getline(std::cin, exePath);
    FreeConsole();
}

// #include <Python.h>
// // Exported function callable via rundll32.exe
// extern "C" __declspec(dllexport) void CALLBACK RunPythonScript(HWND hwnd, HINSTANCE hinst, LPSTR lpszCmdLine, int nCmdShow) {
//     Py_Initialize();
//     if (lpszCmdLine && strlen(lpszCmdLine) > 0) {
//         PyRun_SimpleString(lpszCmdLine);
//     }
//     else {
//         // Fallback: Default message
//         PyRun_SimpleString("print('Hello from Python!')");
//     }
//     // Finalize Python runtime
//     Py_Finalize();
// }

