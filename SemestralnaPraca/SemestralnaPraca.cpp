#include <Windows.h>
#include <libds/heap_monitor.h>
#include "App.h"

int main()
{
    // Monitor haldy
    initHeapMonitor();

    // Nastavenie kodovania konzoly na Windows-1250
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    try
    {
        App app;
        app.start();
    }
	catch(std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
