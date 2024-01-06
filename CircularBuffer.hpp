// Copyright 2022 Graham Stelzer
#ifndef CIRCULARBUFFER_HPP_
#define CIRCULARBUFFER_HPP_

#include <iostream>

template <typename T>
class CircularBuffer {
 public:
    explicit CircularBuffer(size_t capacity);  // Create an empty ring buffer
                                               // with given max capacity
    size_t size() const;
    bool isEmpty() const;
    bool isFull() const;
    void enqueue(T item);

    // delete and return item from the front
    T dequeue();

    T peek() const;

    void print() const;

    explicit CircularBuffer(const CircularBuffer& other);

    CircularBuffer<T>& operator=(const CircularBuffer<T>& other);
    ~CircularBuffer() {
        delete[] _data;
    }

 private:
    T* _data;
    size_t _capacity;
    int _head;
    int _tail;
    size_t _numItems;  // must be type size_t so that it can be compared to _capacity
};

template <typename T>
CircularBuffer<T>::CircularBuffer(const CircularBuffer& other) {
    _capacity = other._capacity;
    _head = other._head;
    _tail = other._tail;
    _numItems = other._numItems;
    _data = new T[_capacity];
    for (size_t i = 0; i < _capacity; i++) {
        _data[i] = other._data[i];
    }
}

template <typename T>
CircularBuffer<T>& CircularBuffer<T>::operator=
                    (const CircularBuffer<T>& other) {
    if (this == &other) return *this;

    _capacity = other._capacity;
    _head = other._head;
    _tail = other._tail;
    _numItems = other._numItems;
    _data = new T[_capacity];
    for (size_t i = 0; i < _capacity; i++) {
        _data[i] = other._data[i];
    }

    return *this;
}

template <typename T>
CircularBuffer<T>::CircularBuffer(size_t capacity) {
    if (capacity < 1) {
        throw std::invalid_argument("cannot have size < 1 buffer");
    }
    _head = 0;
    _tail = 0;
    _numItems = 0;
    _capacity = capacity;
    _data = new T[capacity];
}

template <typename T>
size_t CircularBuffer<T>::size() const {
    return _numItems;
}

template <typename T>
bool CircularBuffer<T>::isEmpty() const {
    if (_numItems == 0) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
bool CircularBuffer<T>::isFull() const {
    if (_numItems == _capacity) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
void CircularBuffer<T>::enqueue(T item) {
    if (isFull()) {
        throw std::runtime_error("buffer is full");
    } else {
        _numItems++;
        _data[_tail] = item;
        _tail = (_tail + 1) % _capacity;
    }
}

template <typename T>
T CircularBuffer<T>::dequeue() {
    if (isEmpty()) {
        throw std::out_of_range("deq: buffer is empty");
    }
    T temp = _data[_head];
    _numItems--;
    _head = (_head + 1) % _capacity;

    return temp;
}

template <typename T>
T CircularBuffer<T>::peek() const {
    if (isEmpty()) {
        std::cout << "buffer is empty" << std::endl;
        throw std::out_of_range("peek: buffer is empty");
    } else {
        return _data[_head];
    }
}

template <typename T>
void CircularBuffer<T>::print() const {
    std::cout << "buffer: " << std::endl;
    for (size_t i = 0; i < _capacity; i++) {
        std::cout << _data[i] << " ";
    }
    std::cout << std::endl;
}

#endif  // CIRCULARBUFFER_HPP_
