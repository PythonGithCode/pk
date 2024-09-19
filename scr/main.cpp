#include <iostream>

#ifdef _WIN32
#define EXPORT_SYMBOL __declspec(dllexport)
#else
#define EXPORT_SYMBOL
#endif

// A simple function to export
extern "C" EXPORT_SYMBOL void hello() {
    std::cout << "Hello from DLL!" << std::endl;
}

