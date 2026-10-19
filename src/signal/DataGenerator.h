//
// Created by fzmyslony on 02.05.2024.
//

#ifndef NIDUC2_PROJEKT_DATAGENERATOR_H
#define NIDUC2_PROJEKT_DATAGENERATOR_H

#include <vector>

namespace utils {
    /**
     * @class DataGenerator
     * @brief Data generation
     */
    class DataGenerator {
    public:
        /**
         * Generating sawtooth wave based on params provided to function.
         *
         * @param frequency Double, number of waves per second.
         * @param duration Double, duration of signal in seconds.
         * @param samplingRate Double, number of samples per second.
         * @param randomOffsetRange Double, offset for data generation, used to insert error and make course non linear.
         * @return vector of double, generated sawtooth wave.
         */
        static std::vector<double> generateRandomSawtoothWave(double frequency, double duration, double samplingRate, double randomOffsetRange);
    };
} // utils

#endif //NIDUC2_PROJEKT_DATAGENERATOR_H
