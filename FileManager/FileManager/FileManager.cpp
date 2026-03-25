#include "FileManager.h"

bool HelpCommand::Execute() {
    if (!args_.empty()) {
    }

    cout << TerminalColor::Bold() << TerminalColor::SetColor(TerminalColor::Color::WHITE)
        << "\n═══════════════════════════════════════════\n"
        << "        AVAILABLE COMMANDS\n"
        << "═══════════════════════════════════════════\n\n"
        << TerminalColor::Reset();

    cout << TerminalColor::SetColor(TerminalColor::Color::GREEN) << "help"
        << TerminalColor::Reset() << "     - " << GetDescription() << "\n"
        << TerminalColor::SetColor(TerminalColor::Color::GREEN) << "pwd"
        << TerminalColor::Reset() << "      - " << PwdCommand(currentPath_).GetDescription() << "\n"
        << TerminalColor::SetColor(TerminalColor::Color::GREEN) << "cd <dir>"
        << TerminalColor::Reset() << "   - " << CdCommand(currentPath_).GetDescription() << "\n"
        << TerminalColor::SetColor(TerminalColor::Color::GREEN) << "ls"
        << TerminalColor::Reset() << "       - " << ListCommand(currentPath_).GetDescription() << "\n"
        << TerminalColor::SetColor(TerminalColor::Color::GREEN) << "mkdir <name>"
        << TerminalColor::Reset() << " - " << MkdirCommand(currentPath_).GetDescription() << "\n"
        << TerminalColor::SetColor(TerminalColor::Color::GREEN) << "touch <file>"
        << TerminalColor::Reset() << "  - " << TouchCommand(currentPath_).GetDescription() << "\n"
        << TerminalColor::SetColor(TerminalColor::Color::GREEN) << "rm <name>"
        << TerminalColor::Reset() << "     - " << RmCommand(currentPath_).GetDescription() << "\n"
        << TerminalColor::SetColor(TerminalColor::Color::GREEN) << "cat <file>"
        << TerminalColor::Reset() << "     - " << CatCommand(currentPath_).GetDescription() << "\n"
        << TerminalColor::SetColor(TerminalColor::Color::GREEN) << "exit"
        << TerminalColor::Reset() << "     - " << ExitCommand(currentPath_).GetDescription() << "\n"
        << endl;

    return true;
}

bool PwdCommand::Execute() {
    cout << TerminalColor::Info()
        << currentPath_.string()
        << TerminalColor::Reset() << endl;
    return true;
}

bool CdCommand::Execute() {
    if (!ValidateArgCount(1, 1)) return false;

    string target = GetArg(0);

    try {
        if (target == "..") {
            if (currentPath_.has_parent_path()) {
                currentPath_ = currentPath_.parent_path();
            }
        }
        else if (target == "/" || target == "\\") {
            currentPath_ = currentPath_.root_path();
        }
        else {
            fs::path newPath = currentPath_ / target;
            newPath = fs::weakly_canonical(newPath);

            if (fs::exists(newPath) && fs::is_directory(newPath)) {
                currentPath_ = newPath;
            }
            else {
                cout << TerminalColor::Error()
                    << "Directory not found: " << target
                    << TerminalColor::Reset() << endl;
                return false;
            }
        }
        return true;
    }
    catch (const fs::filesystem_error& e) {
        cout << TerminalColor::Error()
            << "Error changing directory: " << e.what()
            << TerminalColor::Reset() << endl;
        return false;
    }
}

bool ListCommand::Execute() {
    try {
        cout << TerminalColor::Bold() << TerminalColor::SetColor(TerminalColor::Color::WHITE)
            << "\nContents of: " << currentPath_.string() << "\n"
            << "─────────────────────────────────────\n"
            << TerminalColor::Reset();

        bool hasItems = false;

        for (const auto& entry : fs::directory_iterator(currentPath_)) {
            hasItems = true;

            if (entry.is_directory()) {
                cout << TerminalColor::SetColor(TerminalColor::Color::GREEN)
                    << "[DIR]  " << entry.path().filename().string()
                    << TerminalColor::Reset() << endl;
            }
            else if (entry.is_regular_file()) {
                auto size = fs::file_size(entry);
                cout << TerminalColor::SetColor(TerminalColor::Color::CYAN)
                    << "[FILE] " << entry.path().filename().string()
                    << TerminalColor::Reset()
                    << " (" << size << " bytes)" << endl;
            }
            else {
                cout << TerminalColor::SetColor(TerminalColor::Color::YELLOW)
                    << "[LINK] " << entry.path().filename().string()
                    << TerminalColor::Reset() << endl;
            }
        }

        if (!hasItems) {
            cout << TerminalColor::Warning()
                << "Directory is empty"
                << TerminalColor::Reset() << endl;
        }

        return true;
    }
    catch (const fs::filesystem_error& e) {
        cout << TerminalColor::Error()
            << "Error reading directory: " << e.what()
            << TerminalColor::Reset() << endl;
        return false;
    }
}

bool MkdirCommand::Execute() {
    if (!ValidateArgCount(1, 1)) return false;

    string dirName = GetArg(0);
    fs::path newDir = currentPath_ / dirName;

    try {
        if (fs::create_directory(newDir)) {
            cout << TerminalColor::Success()
                << "✓ Directory created: " << dirName
                << TerminalColor::Reset() << endl;
            return true;
        }
        else {
            cout << TerminalColor::Error()
                << "Failed to create directory (may already exist)"
                << TerminalColor::Reset() << endl;
            return false;
        }
    }
    catch (const fs::filesystem_error& e) {
        cout << TerminalColor::Error()
            << "Error creating directory: " << e.what()
            << TerminalColor::Reset() << endl;
        return false;
    }
}

bool TouchCommand::Execute() {
    if (!ValidateArgCount(1, 1)) return false;

    string fileName = GetArg(0);
    fs::path newFile = currentPath_ / fileName;

    try {
        ofstream file(newFile);
        if (file) {
            cout << TerminalColor::Success()
                << "✓ File created: " << fileName
                << TerminalColor::Reset() << endl;
            return true;
        }
        else {
            cout << TerminalColor::Error()
                << "Failed to create file"
                << TerminalColor::Reset() << endl;
            return false;
        }
    }
    catch (const exception& e) {
        cout << TerminalColor::Error()
            << "Error: " << e.what()
            << TerminalColor::Reset() << endl;
        return false;
    }
}

bool RmCommand::Execute() {
    if (!ValidateArgCount(1, 1)) return false;

    string target = GetArg(0);
    fs::path path = currentPath_ / target;

    try {
        if (!fs::exists(path)) {
            cout << TerminalColor::Error()
                << "Item not found: " << target
                << TerminalColor::Reset() << endl;
            return false;
        }

        uintmax_t removedCount = fs::remove_all(path);
        cout << TerminalColor::Success()
            << "✓ Removed items: " << removedCount
            << TerminalColor::Reset() << endl;
        return true;
    }
    catch (const fs::filesystem_error& e) {
        cout << TerminalColor::Error()
            << "Error removing: " << e.what()
            << TerminalColor::Reset() << endl;
        return false;
    }
}

bool CatCommand::Execute() {
    if (!ValidateArgCount(1, 1)) return false;

    string fileName = GetArg(0);
    fs::path filePath = currentPath_ / fileName;

    try {
        if (!fs::exists(filePath) || !fs::is_regular_file(filePath)) {
            cout << TerminalColor::Error()
                << "File not found: " << fileName
                << TerminalColor::Reset() << endl;
            return false;
        }

        ifstream file(filePath);
        if (!file.is_open()) {
            cout << TerminalColor::Error()
                << "Failed to open file"
                << TerminalColor::Reset() << endl;
            return false;
        }

        cout << TerminalColor::Bold() << TerminalColor::SetColor(TerminalColor::Color::WHITE)
            << "\n─── " << fileName << " ───\n"
            << TerminalColor::Reset();

        string line;
        int lineNum = 1;
        while (getline(file, line)) {
            cout << TerminalColor::SetColor(TerminalColor::Color::CYAN)
                << setw(4) << lineNum++ << " │ "
                << TerminalColor::Reset() << line << endl;
        }

        file.close();
        return true;
    }
    catch (const exception& e) {
        cout << TerminalColor::Error()
            << "Error: " << e.what()
            << TerminalColor::Reset() << endl;
        return false;
    }
}

bool ExitCommand::Execute() {
    cout << TerminalColor::Warning()
        << "Shutting down..."
        << TerminalColor::Reset() << endl;
    return false;
}

CommandRegistry::CommandRegistry(fs::path& path) : currentPath_(path) {
    RegisterCommand<HelpCommand>();
    RegisterCommand<PwdCommand>();
    RegisterCommand<CdCommand>();
    RegisterCommand<ListCommand>();
    RegisterCommand<MkdirCommand>();
    RegisterCommand<TouchCommand>();
    RegisterCommand<RmCommand>();
    RegisterCommand<CatCommand>();
    RegisterCommand<ExitCommand>();
}

ICommand* CommandRegistry::GetCommand(const string& name) {
    auto it = commands_.find(name);
    if (it != commands_.end()) {
        return it->second.get();
    }
    return nullptr;
}

void CommandRegistry::ShowHelp(const string& commandName) {
    if (commandName.empty()) {
        auto helpCmd = GetCommand("help");
        if (helpCmd) helpCmd->Execute();
    }
    else {
        auto cmd = GetCommand(commandName);
        if (cmd) {
            cout << TerminalColor::Info()
                << "\nCommand: " << TerminalColor::Bold() << cmd->GetName()
                << TerminalColor::Reset() << endl;
            cout << "Description: " << cmd->GetDescription() << endl;
            cout << "Usage: " << cmd->GetUsage() << endl;
        }
        else {
            cout << TerminalColor::Error()
                << "Command not found: " << commandName
                << TerminalColor::Reset() << endl;
        }
    }
}