#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include "io/CsvWriter.h"
#include "signal/DataGenerator.h"
#include "voting/Voting.h"

using namespace std;

// Zapisuje jedna kolumne wartosci jako CSV: naglowek "lp,<nazwa>", potem numer probki i wartosc
static void saveColumn(const string& file, const string& header, const vector<double>& values) {
    vector<vector<string>> rows = {{"lp", header}};
    for (size_t i = 0; i < values.size(); i++) {
        rows.push_back({to_string(i), to_string(values[i])});
    }
    generateCSV(file, rows);
}

int main() {
    // Trzy kanaly: ten sam sygnal piloksztaltny z niezaleznym szumem +-ror
    double freq = 2, dur = 1, sr = 20, ror = 5;
    vector<vector<double>> channels;
    for (int k = 0; k < 3; k++) {
        channels.push_back(utils::DataGenerator::generateRandomSawtoothWave(freq, dur, sr, ror));
        for (double v : channels[k]) cout << v << ' ';
        cout << endl;
        saveColumn("data_" + to_string(k + 1) + ".csv", "generated", channels[k]);
    }

    double prog = 1;  // Prog dla glosowania z niedokladnymi danymi
    vector<double> wagi = {0.3, 0.5, 0.2};

    struct Voter {
        string label, file;
        function<double(const vector<double>&)> vote;
    };
    vector<Voter> voters = {
            {"Nieprecyzyjne glosowanie wiekszosciowe", "wiekszosciowe.csv",
             [&](const vector<double>& v) { return votingAlgorithm(v, prog, true); }},
            {"Nieprecyzyjne glosowanie pluralistyczne", "pluralistyczne.csv",
             [&](const vector<double>& v) { return votingAlgorithm(v, prog, false); }},
            {"Weighted Average Voting", "weighted.csv",
             [&](const vector<double>& v) { return weightedAverageVoting(v, wagi); }},
            {"Median Voting", "median.csv", [](const vector<double>& v) { return medianVoting(v); }},
            {"Linear Voting", "linear.csv", [](const vector<double>& v) { return linearVoting(v); }},
            {"First-Order Voting", "first_order.csv", [](const vector<double>& v) { return firstOrderVoting(v); }},
    };

    for (const Voter& voter : voters) {
        cout << voter.label << ": ";
        vector<double> results;
        for (size_t i = 0; i < channels[0].size(); i++) {
            vector<double> glosy = {channels[0][i], channels[1][i], channels[2][i]};
            results.push_back(voter.vote(glosy));
            cout << results.back() << " ";
        }
        cout << endl;
        saveColumn(voter.file, "selected", results);
    }
    return 0;
}
