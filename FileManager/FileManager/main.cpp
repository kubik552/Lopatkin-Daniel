#include "FileManager.h"

int main() {
    cout << "Starting File Manager..." << endl;

    try {
        FileManager manager;
        manager.Run();
    }
    catch (const exception& e) {
        cout << TerminalColor::Error()
            << "Critical error: " << e.what()
            << TerminalColor::Reset() << endl;
        return 1;
    }

    cout << "Program terminated successfully!" << endl;
    return 0;
}