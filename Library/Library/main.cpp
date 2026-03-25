#include <iostream>
#include <limits>
#include <string>
#include "Library.h"

using namespace std;

void clearScreen() {
    system("cls");
}

void waitForEnter() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

int getNumberInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;

        if (cin >> value) {
            cin.ignore();
            return value;
        }

        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Error: Please enter a valid number" << endl;
    }
}

string getStringInput(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

void displayMenu() {
    clearScreen();
    cout << "========================================" << endl;
    cout << "     LIBRARY MANAGEMENT SYSTEM" << endl;
    cout << "========================================" << endl;
    cout << "1. Add book" << endl;
    cout << "2. Remove book by ISBN" << endl;
    cout << "3. Find book by title" << endl;
    cout << "4. Display all books" << endl;
    cout << "5. Show book count" << endl;
    cout << "6. Clear library" << endl;
    cout << "7. Test exception handling" << endl;
    cout << "0. Exit" << endl;
    cout << "========================================" << endl;
    cout << "Choose action: ";
}

void addBookMenu(Library& library) {
    cout << "\n--- ADD NEW BOOK ---" << endl;

    string title = getStringInput("Enter title: ");
    if (title.empty()) {
        cout << "Title cannot be empty" << endl;
        return;
    }

    string author = getStringInput("Enter author: ");
    if (author.empty()) {
        cout << "Author cannot be empty" << endl;
        return;
    }

    int year = getNumberInput("Enter year: ");

    string isbn = getStringInput("Enter ISBN: ");
    if (isbn.empty()) {
        cout << "ISBN cannot be empty" << endl;
        return;
    }

    try {
        Book newBook(title, author, year, isbn);
        library.addBook(newBook);
    }
    catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void removeBookMenu(Library& library) {
    cout << "\n--- REMOVE BOOK ---" << endl;
    string isbn = getStringInput("Enter ISBN of book to remove: ");

    if (!isbn.empty()) {
        library.removeBook(isbn);
    }
}

void findBookMenu(Library& library) {
    cout << "\n--- FIND BOOK ---" << endl;
    string title = getStringInput("Enter book title: ");

    if (!title.empty()) {
        Book* found = library.findBookByTitle(title);
        if (found != nullptr) {
            cout << "\nBook found!" << endl;
            found->displayInfo();
        }
    }
}

void testExceptions(Library& library) {
    cout << "\n--- TESTING EXCEPTION HANDLING ---" << endl;
    cout << "1. Empty title" << endl;
    cout << "2. Invalid year" << endl;
    cout << "3. Duplicate ISBN" << endl;
    cout << "4. Out of range access" << endl;
    cout << "Choose test: ";

    int test = getNumberInput("");

    try {
        switch (test) {
        case 1: {
            cout << "Creating book with empty title..." << endl;
            Book("", "Test Author", 2020, "TEST001");
            break;
        }

        case 2: {
            cout << "Creating book with invalid year..." << endl;
            Book("Test Title", "Test Author", -5, "TEST002");
            break;
        }

        case 3: {
            cout << "Adding duplicate ISBN..." << endl;
            Book book1("Book 1", "Author 1", 2020, "DUPLICATE");
            Book book2("Book 2", "Author 2", 2021, "DUPLICATE");
            library.addBook(book1);
            library.addBook(book2);
            break;
        }

        case 4: {
            cout << "Accessing out of range index..." << endl;
            library.getBookAtIndex(999);
            break;
        }

        default:
            cout << "Invalid test choice" << endl;
        }
    }
    catch (const exception& e) {
        cout << "Exception caught: " << e.what() << endl;
    }
}

int main() {
    Library library;
    int choice = -1;

    while (choice != 0) {
        displayMenu();

        try {
            cin >> choice;
            cin.ignore();

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                throw invalid_argument("Please enter a valid number");
            }

            switch (choice) {
            case 1:
                addBookMenu(library);
                break;

            case 2:
                removeBookMenu(library);
                break;

            case 3:
                findBookMenu(library);
                break;

            case 4:
                library.displayAllBooks();
                break;

            case 5:
                cout << "\nNumber of books in library: " << library.getBookCount() << endl;
                break;

            case 6:
                library.clearLibrary();
                break;

            case 7:
                testExceptions(library);
                break;

            case 0:
                cout << "\nGoodbye!" << endl;
                break;

            default:
                cout << "Error: Please select 0-7" << endl;
            }

            if (choice != 0) {
                waitForEnter();
            }
        }
        catch (const exception& e) {
            cout << "Error: " << e.what() << endl;
            waitForEnter();
        }
    }

    return 0;
}