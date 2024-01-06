// Copyright 2022 Graham Stelzer

#ifndef STRINGSOUND_HPP_
#define STRINGSOUND_HPP_

#include <iostream>
#include <vector>
#include <memory>

#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "CircularBuffer.hpp"

class StringSound {
 public:
    // Create a guitar string sound of the given
    // frequency using a sampling rate of 44,100
    explicit StringSound(double frequency);

    // Create a guitar string with size and initial values given by the vector
    explicit StringSound(std::vector<sf::Int16> init);

    // No copy constructor
    StringSound(const StringSound& obj) = delete;

    // Pluck the guitar string by replacing the buffer with
    // random values, representing the white noise
    void pluck();

    // Advance the simulation one time step
    void tick();

    // Return the current sample
    sf::Int16 sample() const;

    // Return the number of times tick was called so far
    size_t time() const;

 private:
    std::unique_ptr<CircularBuffer<sf::Int16>> _buffer;
    size_t _time;
    size_t _size;
};
#endif  // STRINGSOUND_HPP_
