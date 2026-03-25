#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <stdexcept>
#include <initializer_list>

using namespace std;

template <typename T>
class Vector {
private:
    T* data;
    size_t size_;
    size_t capacity_;

    void reallocate(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = data[i];
        }
        delete[] data;
        data = new_data;
        capacity_ = new_capacity;
    }

public:
    Vector() : data(nullptr), size_(0), capacity_(0) {}

    explicit Vector(size_t initial_size)
        : size_(initial_size), capacity_(initial_size * 2) {
        data = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = T();
        }
    }

    Vector(size_t initial_size, const T& value)
        : size_(initial_size), capacity_(initial_size * 2) {
        data = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = value;
        }
    }

    Vector(initializer_list<T> init)
        : size_(init.size()), capacity_(init.size() * 2) {
        data = new T[capacity_];
        size_t i = 0;
        for (const T& elem : init) {
            data[i++] = elem;
        }
    }

    Vector(const Vector& other)
        : size_(other.size_), capacity_(other.capacity_) {
        data = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data[i] = other.data[i];
        }
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~Vector() {
        delete[] data;
    }

    T& at(size_t index) {
        if (index >= size_) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    const T& at(size_t index) const {
        if (index >= size_) {
            throw out_of_range("Index out of range");
        }
        return data[index];
    }

    T& operator[](size_t index) {
        return data[index];
    }

    const T& operator[](size_t index) const {
        return data[index];
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }
        data[size_++] = value;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
        }
    }

    void insert(size_t index, const T& value) {
        if (index > size_) {
            throw out_of_range("Index out of range");
        }

        if (size_ >= capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            reallocate(new_capacity);
        }

        for (size_t i = size_; i > index; --i) {
            data[i] = data[i - 1];
        }

        data[index] = value;
        ++size_;
    }

    void erase(size_t index) {
        if (index >= size_) {
            throw out_of_range("Index out of range");
        }

        for (size_t i = index; i < size_ - 1; ++i) {
            data[i] = data[i + 1];
        }

        --size_;
    }

    void clear() {
        size_ = 0;
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t size() const {
        return size_;
    }

    size_t capacity() const {
        return capacity_;
    }

    void resize(size_t new_size) {
        if (new_size > capacity_) {
            size_t new_capacity = new_size * 2;
            reallocate(new_capacity);
        }

        for (size_t i = size_; i < new_size; ++i) {
            data[i] = T();
        }

        size_ = new_size;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            reallocate(new_capacity);
        }
    }

    T& front() {
        if (size_ == 0) {
            throw out_of_range("Vector is empty");
        }
        return data[0];
    }

    const T& front() const {
        if (size_ == 0) {
            throw out_of_range("Vector is empty");
        }
        return data[0];
    }

    T& back() {
        if (size_ == 0) {
            throw out_of_range("Vector is empty");
        }
        return data[size_ - 1];
    }

    const T& back() const {
        if (size_ == 0) {
            throw out_of_range("Vector is empty");
        }
        return data[size_ - 1];
    }

    typedef T* iterator;
    typedef const T* const_iterator;

    iterator begin() {
        return data;
    }

    iterator end() {
        return data + size_;
    }

    const_iterator begin() const {
        return data;
    }

    const_iterator end() const {
        return data + size_;
    }
};

template <typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs) {
    if (lhs.size() != rhs.size()) return false;
    for (size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) return false;
    }
    return true;
}

template <typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs) {
    return !(lhs == rhs);
}

#endif