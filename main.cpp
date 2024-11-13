#include "Syscall.h"
#include <thread>
#include <chrono>

void ClearScreen() {
    COORD topLeft = {0, 0};
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, screen.wAttributes, screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

int main() {
    Syscall syscall;

    while (true) {
        ClearScreen();
        syscall.ListRunningProcesses();
        std::this_thread::sleep_for(std::chrono::seconds(5)); // Pause de 5 secondes avant la prochaine capture
    }

    return 0;
}
