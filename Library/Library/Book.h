#pragma once

#include <iostream>
#include <string>
#include <stdexcept>

class Book {
private:
    std::string m_title;
    std::string m_author;
    int m_year;
    std::string m_isbn;

public:
    Book() : m_title(""), m_author(""), m_year(0), m_isbn("") {}

    Book(const std::string& title, const std::string& author, int year, const std::string& isbn) {
        if (title.empty()) {
            throw std::invalid_argument("Title cannot be empty");
        }
        if (author.empty()) {
            throw std::invalid_argument("Author cannot be empty");
        }
        if (year <= 0 || year > 2026) {
            throw std::out_of_range("Year must be between 1 and 2026");
        }
        if (isbn.empty()) {
            throw std::invalid_argument("ISBN cannot be empty");
        }

        m_title = title;
        m_author = author;
        m_year = year;
        m_isbn = isbn;
    }

    // Getters
    std::string getTitle() const { return m_title; }
    std::string getAuthor() const { return m_author; }
    int getYear() const { return m_year; }
    std::string getIsbn() const { return m_isbn; }

    // Setters
    void setTitle(const std::string& title) {
        if (title.empty()) {
            throw std::invalid_argument("Title cannot be empty");
        }
        m_title = title;
    }

    void setAuthor(const std::string& author) {
        if (author.empty()) {
            throw std::invalid_argument("Author cannot be empty");
        }
        m_author = author;
    }

    void setYear(int year) {
        if (year <= 0 || year > 2026) {
            throw std::out_of_range("Year must be between 1 and 2026");
        }
        m_year = year;
    }

    void setIsbn(const std::string& isbn) {
        if (isbn.empty()) {
            throw std::invalid_argument("ISBN cannot be empty");
        }
        m_isbn = isbn;
    }

    void displayInfo() const {
        std::cout << "Title: " << m_title << std::endl;
        std::cout << "Author: " << m_author << std::endl;
        std::cout << "Year: " << m_year << std::endl;
        std::cout << "ISBN: " << m_isbn << std::endl;
        std::cout << "------------------------" << std::endl;
    }
};