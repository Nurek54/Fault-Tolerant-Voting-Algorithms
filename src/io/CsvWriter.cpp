#include "io/CsvWriter.h"

#include <fstream>
#include <iostream>

void generateCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data) {
    std::ofstream outputFile(filename);

    if (!outputFile.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    // Komorki rozdzielone przecinkiem, bez separatora na koncu wiersza
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            outputFile << row[i] << (i + 1 < row.size() ? "," : "");
        }
        outputFile << "\n";
    }

    outputFile.close();
}
