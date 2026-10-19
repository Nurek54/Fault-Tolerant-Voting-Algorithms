//
// Created by fzmyslony on 02.05.2024.
//

#include "signal/DataGenerator.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <random>

namespace utils {

    std::vector<double>
    DataGenerator::generateRandomSawtoothWave(double frequency, double duration, double samplingRate,
                                              double randomOffsetRange) {
        double period = 1 / frequency;

        std::vector<double> t(duration * samplingRate);
        std::vector<double> sawtoothWave(t.size());

        for (size_t i = 0; i < t.size(); ++i) {
            double adjustedTime = (i / samplingRate) / period;
            t[i] = adjustedTime - std::floor(adjustedTime);
        }

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(-randomOffsetRange, randomOffsetRange);

        for (size_t i = 0; i < sawtoothWave.size(); ++i) {
            sawtoothWave[i] = std::floor(t[i]) + (t[i] - std::floor(t[i])) * 255 + dis(gen);
        }

        return sawtoothWave;
    }
} // utils