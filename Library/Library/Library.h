#pragma once

#include "Book.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

class Library {
private:
    std::vector<Book> m_books;

public:
    Library() = default;

    void addBook(const Book& book) {
        auto it = std::find_if(m_books.begin(), m_books.end(),
            [&book](const Book& b) { return b.getIsbn() == book.getIsbn(); });

        if (it != m_books.end()) {
            throw std::runtime_error("Book with this ISBN already exists");
        }

        m_books.push_back(book);
        std::cout << "Book \"" << book.getTitle() << "\" added to library!" << std::endl;
    }

    bool removeBook(const std::string& isbn) {
        if (isbn.empty()) {
            std::cout << "Error: ISBN cannot be empty" << std::endl;
            return false;
        }

        auto it = std::find_if(m_books.begin(), m_books.end(),
            [&isbn](const Book& b) { return b.getIsbn() == isbn; });

        if (it != m_books.end()) {
            std::cout << "Book \"" << it->getTitle() << "\" removed from library!" << std::endl;
            m_books.erase(it);
            return true;
        }

        std::cout << "Book with ISBN " << isbn << " not found" << std::endl;
        return false;
    }

    Book* findBookByTitle(const std::string& title) {
        if (title.empty()) {
            std::cout << "Error: Title cannot be empty" << std::endl;
            return nullptr;
        }

        auto it = std::find_if(m_books.begin(), m_books.end(),
            [&title](const Book& b) { return b.getTitle() == title; });

        if (it != m_books.end()) {
            return &(*it);
        }

        std::cout << "Book \"" << title << "\" not found" << std::endl;
        return nullptr;
    }

    void displayAllBooks() const {
        if (m_books.empty()) {
            std::cout << "Library is empty!" << std::endl;
            return;
        }

        std::cout << "\n=== ALL BOOKS IN LIBRARY ===" << std::endl;
        std::cout << "Total books: " << m_books.size() << std::endl;
        std::cout << "------------------------" << std::endl;

        for (const auto& book : m_books) {
            book.displayInfo();
        }
    }

    int getBookCount() const {
        return static_cast<int>(m_books.size());
    }

    void clearLibrary() {
        m_books.clear();
        std::cout << "Library cleared!" << std::endl;
    }

    Book& getBookAtIndex(int index) {
        if (index < 0 || index >= static_cast<int>(m_books.size())) {
            throw std::out_of_range("Index out of range");
        }
        return m_books[index];
    }

    bool isEmpty() const {
        return m_books.empty();
    }
};