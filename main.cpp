// Copyright 2022 Graham Stelzer
// main for ps3a
#include <math.h>
#include <limits.h>
#include <iostream>

#include <string>
#include <exception>
#include <stdexcept>
#include <vector>
#include <map>
#include <utility>  // used for pairs

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Window.hpp>

#include "CircularBuffer.hpp"
#include "StringSound.hpp"

std::vector<sf::Int16> makeSamples(StringSound& gs) {
    std::vector<sf::Int16> samples;
    gs.pluck();

    int duration = 8;  // seconds
    int i;
    for (i= 0; i < 44100 * duration; i++) {
        gs.tick();
        samples.push_back(gs.sample());
    }
    return samples;
}

int main() {
    // string of all the keys
    std::string keys = "q2we4r5ty7u8i9op-[=zxdcfvgbnjmk,.;/' ";

    // create an array of 37 frequencies for the 37 keys
    double freqs[37] {0};
    for (int i = 0; i < 37; i++) {
        freqs[i] = 440 * pow(2, (i - 24) / 12.0);
    }

    // create map of <keyboard key, frequency>
    std::map<char, double> keyMap;
    std::pair<char, double> temp;

    // fill map with key and frequency
    for (int i = 0; i < 37; i++) {
        temp = std::make_pair(keys[i], freqs[i]);
        keyMap.insert(temp);
    }

    // initiate necessary sound variables and window
    char key;
    sf::Sound sound1;
    sf::SoundBuffer buf1;
    std::vector<sf::Int16> samples;
    sf::RenderWindow window(sf::VideoMode(300, 200),
                            "SFML Plucked String Sound Lite");
    sf::Event event;

    // sound via SFML sounds
    sf::Sound sound2;
    sf::SoundBuffer buf2;
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("sound.wav"))
        throw std::runtime_error
        ("sf::SoundBuffer: failed to load from file");
    sound2.setBuffer(buffer);

    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            // close the window if needed
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
            default:
                break;
            }

            // if a key is pressed, play the sound
            if (event.type == sf::Event::TextEntered) {
                key = static_cast<char>(event.text.unicode);
                if ( keyMap.find(key) != keyMap.end() ) {
                // ABOVE IF DOES NOT DETERMINE KEY
                // ONLY DETERMINES IF KEY IS IN MAP
                    double tempKey = keyMap[key];
                    StringSound gs1(tempKey);

                    samples = makeSamples(gs1);

                    if (!buf1.loadFromSamples(&samples[0],
                                            samples.size(),
                                            2,
                                            44100))
                        throw std::runtime_error(
                            "sf::SoundBuffer: failed to load from samples.");
                    sound1.setBuffer(buf1);
                    sound1.play();
                } else {
                sound2.play();
                }
            }
            window.clear();
            window.display();
        }
    }
    return 0;
}
