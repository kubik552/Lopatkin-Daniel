#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <iomanip>

using namespace std;
namespace fs = std::filesystem;

class TerminalColor {
private:
    static constexpr const char* RESET = "\033[0m";
    static constexpr const char* BOLD = "\033[1m";

public:
    enum class Color {
        BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, CYAN, WHITE
    };

    static string SetColor(Color color) {
        switch (color) {
        case Color::BLACK:  return "\033[30m";
        case Color::RED:    return "\033[31m";
        case Color::GREEN:  return "\033[32m";
        case Color::YELLOW: return "\033[33m";
        case Color::BLUE:   return "\033[34m";
        case Color::PURPLE: return "\033[35m";
        case Color::CYAN:   return "\033[36m";
        case Color::WHITE:  return "\033[37m";
        default: return RESET;
        }
    }

    static string SetBackground(Color bg) {
        switch (bg) {
        case Color::BLACK:  return "\033[40m";
        case Color::RED:    return "\033[41m";
        case Color::GREEN:  return "\033[42m";
        case Color::YELLOW: return "\033[43m";
        case Color::BLUE:   return "\033[44m";
        case Color::PURPLE: return "\033[45m";
        case Color::CYAN:   return "\033[46m";
        case Color::WHITE:  return "\033[47m";
        default: return RESET;
        }
    }

    static string Bold() { return BOLD; }
    static string Reset() { return RESET; }
    static string Error() { return Bold() + SetColor(Color::RED); }
    static string Success() { return Bold() + SetColor(Color::GREEN); }
    static string Info() { return SetColor(Color::CYAN); }
    static string Warning() { return Bold() + SetColor(Color::YELLOW); }
    static string Prompt() { return SetColor(Color::GREEN); }
};

class ICommand {
protected:
    fs::path& currentPath_;
    vector<string> args_;

public:
    explicit ICommand(fs::path& path) : currentPath_(path) {}
    virtual ~ICommand() = default;

    void SetArgs(const vector<string>& args) {
        args_ = args;
    }

    virtual string GetName() const = 0;
    virtual string GetDescription() const = 0;
    virtual bool Execute() = 0;
    virtual string GetUsage() const = 0;

protected:
    bool ValidateArgCount(size_t expected, size_t max = 0) const {
        size_t argCount = args_.size();
        if (max == 0) max = expected;

        if (argCount < expected) {
            cout << TerminalColor::Error()
                << "Error: insufficient arguments. Expected " << expected
                << TerminalColor::Reset() << endl;
            return false;
        }

        if (argCount > max) {
            cout << TerminalColor::Error()
                << "Error: too many arguments. Maximum " << max
                << TerminalColor::Reset() << endl;
            return false;
        }

        return true;
    }

    string GetArg(size_t index) const {
        if (index < args_.size()) {
            return args_[index];
        }
        return "";
    }
};

class HelpCommand : public ICommand {
public:
    using ICommand::ICommand;
    string GetName() const override { return "help"; }
    string GetDescription() const override { return "Show help message"; }
    string GetUsage() const override { return "help [command]"; }
    bool Execute() override;
};

class PwdCommand : public ICommand {
public:
    using ICommand::ICommand;
    string GetName() const override { return "pwd"; }
    string GetDescription() const override { return "Show current working directory"; }
    string GetUsage() const override { return "pwd"; }
    bool Execute() override;
};

class CdCommand : public ICommand {
public:
    using ICommand::ICommand;
    string GetName() const override { return "cd"; }
    string GetDescription() const override { return "Change directory"; }
    string GetUsage() const override { return "cd <directory>"; }
    bool Execute() override;
};

class ListCommand : public ICommand {
public:
    using ICommand::ICommand;
    string GetName() const override { return "ls"; }
    string GetDescription() const override { return "List directory contents"; }
    string GetUsage() const override { return "ls"; }
    bool Execute() override;
};

class MkdirCommand : public ICommand {
public:
    using ICommand::ICommand;
    string GetName() const override { return "mkdir"; }
    string GetDescription() const override { return "Create a directory"; }
    string GetUsage() const override { return "mkdir <name>"; }
    bool Execute() override;
};

class TouchCommand : public ICommand {
public:
    using ICommand::ICommand;
    string GetName() const override { return "touch"; }
    string GetDescription() const override { return "Create an empty file"; }
    string GetUsage() const override { return "touch <file>"; }
    bool Execute() override;
};

class RmCommand : public ICommand {
public:
    using ICommand::ICommand;
    string GetName() const override { return "rm"; }
    string GetDescription() const override { return "Remove file or directory"; }
    string GetUsage() const override { return "rm <name>"; }
    bool Execute() override;
};

class CatCommand : public ICommand {
public:
    using ICommand::ICommand;
    string GetName() const override { return "cat"; }
    string GetDescription() const override { return "Display file contents"; }
    string GetUsage() const override { return "cat <file>"; }
    bool Execute() override;
};

class ExitCommand : public ICommand {
public:
    using ICommand::ICommand;
    string GetName() const override { return "exit"; }
    string GetDescription() const override { return "Exit the program"; }
    string GetUsage() const override { return "exit"; }
    bool Execute() override;
};

class CommandRegistry {
private:
    unordered_map<string, unique_ptr<ICommand>> commands_;
    fs::path& currentPath_;

public:
    explicit CommandRegistry(fs::path& path);

    template<typename T>
    void RegisterCommand() {
        auto cmd = make_unique<T>(currentPath_);
        commands_[cmd->GetName()] = move(cmd);
    }

    ICommand* GetCommand(const string& name);
    void ShowHelp(const string& commandName = "");
};

class CommandParser {
private:
    vector<string> ParseArguments(const string& input) {
        vector<string> args;
        stringstream ss(input);
        string arg;

        while (ss >> arg) {
            args.push_back(arg);
        }

        return args;
    }

public:
    pair<string, vector<string>> Parse(const string& input) {
        auto args = ParseArguments(input);

        if (args.empty()) {
            return { "", {} };
        }

        string command = args[0];
        args.erase(args.begin());

        return { command, args };
    }
};

class FileManager {
private:
    fs::path currentPath_;
    CommandRegistry registry_;
    CommandParser parser_;
    bool isRunning_;

    void ShowWelcomeMessage() {
        cout << TerminalColor::Bold() << TerminalColor::SetColor(TerminalColor::Color::WHITE)
            << "\n"
            << "╔══════════════════════════════════╗\n"
            << "║        FILE MANAGER v1.0         ║\n"
            << "║         Lopatkin Daniel          ║\n"
            << "╚══════════════════════════════════╝\n"
            << TerminalColor::Reset();

        cout << TerminalColor::Info()
            << "Current directory: " << currentPath_.string() << "\n"
            << "Type 'help' for command list\n"
            << TerminalColor::Reset() << endl;
    }

    void ShowPrompt() {
        cout << TerminalColor::Prompt()
            << currentPath_.filename().string()
            << TerminalColor::SetColor(TerminalColor::Color::WHITE)
            << " $ "
            << TerminalColor::Reset();
    }

public:
    FileManager() : currentPath_(fs::current_path()),
        registry_(currentPath_),
        isRunning_(true) {
#ifdef _WIN32
        system("chcp 65001 > nul");
#endif
    }

    void Run() {
        ShowWelcomeMessage();

        while (isRunning_) {
            ShowPrompt();

            string input;
            getline(cin, input);

            if (input.empty()) continue;

            auto [commandName, args] = parser_.Parse(input);

            if (commandName == "help") {
                if (args.empty()) {
                    registry_.ShowHelp();
                }
                else {
                    registry_.ShowHelp(args[0]);
                }
                continue;
            }

            auto command = registry_.GetCommand(commandName);

            if (command) {
                command->SetArgs(args);
                bool result = command->Execute();

                if (!result && commandName == "exit") {
                    isRunning_ = false;
                }
            }
            else if (!commandName.empty()) {
                cout << TerminalColor::Error()
                    << "Unknown command: " << commandName << "\n"
                    << TerminalColor::Reset();
                cout << TerminalColor::Info()
                    << "Type 'help' for available commands\n"
                    << TerminalColor::Reset();
            }
        }

        cout << TerminalColor::Success()
            << "Goodbye!"
            << TerminalColor::Reset() << endl;
    }
};

#endif