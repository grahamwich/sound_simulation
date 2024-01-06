// Copyright 2022 Graham Stelzer

#include <math.h>
#include <iostream>

#include <vector>
#include <random>
#include <chrono>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "CircularBuffer.hpp"
#include "StringSound.hpp"

// Create a guitar string sound of the given frequency
// using a sampling rate of 44,100
StringSound::StringSound(double frequency) {
    _size = ceil(44100 / frequency);
    _buffer = std::unique_ptr<CircularBuffer<int16_t>>
        (new CircularBuffer<int16_t>(_size));
    _time = 0;    
}

// Create a guitar string with size and initial values given by the vector
StringSound::StringSound(std::vector<sf::Int16> init) {
    _size = ceil(44100 / init.size());
    _buffer = std::unique_ptr<CircularBuffer<int16_t>>
        (new CircularBuffer<int16_t>(_size));
    for (size_t i = 0; i < init.size(); i++) {
        _buffer->enqueue(init[i]);
    }
    _time = 0;
}

// Return the current sample
sf::Int16 StringSound::sample() const {
    return _buffer->peek();
}

// tick, 3a, average of first 2 * decay rate
// track how many times tick was called, variable++
// algorithm
// .996 * 1/2 of first 2 values
void StringSound::tick() {
    int16_t temp = 0.996 * ((_buffer->dequeue()
                           + _buffer->peek()) / 2);
                           // no values to dequeue
    _buffer->enqueue(temp);
    _time++;
}

// Pluck the guitar string by replacing the buffer with
// random values, representing the white noise
void StringSound::pluck() {
    for (size_t i = 0; i < _buffer->size(); i++) {
        _buffer->dequeue();
    }
    unsigned int seed =
    std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 g2(seed);  // Mersenne Twister
    std::uniform_int_distribution<int> dist(-32767, 32767);
    for (size_t i = 0; i < _size; i++) {
        _buffer->enqueue(dist(g2) % 32767);
    }
}

size_t StringSound::time() const {
    return _time;
}
