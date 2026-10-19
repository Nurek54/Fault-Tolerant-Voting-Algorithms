// Ocena algorytmow glosowania wzgledem prawdziwego sygnalu.
// Trzy kanaly to ten sam sygnal piloksztaltny (0..255) z niezaleznym szumem jednostajnym +-noise;
// opcjonalnie jeden kanal jest uszkodzony. Prawdziwy sygnal to ten sam generator bez szumu.
//
// Uzycie:
//   experiment                         krotkie podsumowanie w konsoli
//   experiment csv                     pelna tabela: scenariusz, parametr, glosujacy, RMSE, blad maks., % decyzji
//   experiment trace [usterka] [kanal] przebieg probka po probce, usterka od polowy (domyslnie stuck 1)
#include "signal/DataGenerator.h"
#include "voting/Voting.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <string>
#include <vector>

using namespace std;

struct Voter {
    string name;
    function<double(const vector<double>&)> vote;
};

static vector<Voter> voters(double eps) {
    return {
            {"majority", [eps](const vector<double>& v) { return votingAlgorithm(v, eps, true); }},
            {"plurality", [eps](const vector<double>& v) { return votingAlgorithm(v, eps, false); }},
            {"weighted", [](const vector<double>& v) { return weightedAverageVoting(v, {0.3, 0.5, 0.2}); }},
            {"median", [](const vector<double>& v) { return medianVoting(v); }},
            {"linear", [](const vector<double>& v) { return linearVoting(v); }},
            {"first_order", [](const vector<double>& v) { return firstOrderVoting(v); }},
    };
}

// Usterka kanalu: none, stuck (stale 0), offset (+60), spikes (20% probek +-100)
static void injectFault(vector<double>& ch, const string& fault, size_t from, unsigned& state) {
    for (size_t i = from; i < ch.size(); ++i) {
        if (fault == "stuck") ch[i] = 0;
        else if (fault == "offset") ch[i] += 60;
        else if (fault == "spikes") {
            state = state * 1103515245u + 12345u;  // prosty LCG, zeby bylo powtarzalnie
            if ((state >> 16) % 5 == 0) ch[i] += ((state >> 8) & 1) ? 100 : -100;
        }
    }
}

struct Stats {
    double se = 0, maxErr = 0;
    long decided = 0, total = 0;
};

struct Row {
    string voter;
    double rmse, maxErr, decidedPct;
};

static vector<Row> evaluate(double noise, double eps, const string& fault, int faultyChannel, int reps) {
    const double freq = 2, dur = 50, sr = 20;  // 1000 probek na powtorzenie
    vector<Voter> vs = voters(eps);
    vector<Stats> st(vs.size());
    unsigned lcg = 12345;
    for (int r = 0; r < reps; ++r) {
        vector<double> truth = utils::DataGenerator::generateRandomSawtoothWave(freq, dur, sr, 0);
        vector<vector<double>> ch(3);
        for (auto& c : ch) c = utils::DataGenerator::generateRandomSawtoothWave(freq, dur, sr, noise);
        if (fault != "none") injectFault(ch[faultyChannel], fault, 0, lcg);
        for (size_t i = 0; i < truth.size(); ++i) {
            vector<double> v = {ch[0][i], ch[1][i], ch[2][i]};
            for (size_t k = 0; k < vs.size(); ++k) {
                double out = vs[k].vote(v);
                st[k].total++;
                if (out == NO_MAJORITY) continue;
                double e = fabs(out - truth[i]);
                st[k].decided++;
                st[k].se += e * e;
                st[k].maxErr = max(st[k].maxErr, e);
            }
        }
    }
    vector<Row> rows;
    for (size_t k = 0; k < vs.size(); ++k) {
        double rmse = st[k].decided ? sqrt(st[k].se / st[k].decided) : NAN;
        rows.push_back({vs[k].name, rmse, st[k].maxErr, 100.0 * st[k].decided / st[k].total});
    }
    return rows;
}

static void printCsv(const string& scenario, const string& param, const vector<Row>& rows) {
    for (const Row& r : rows)
        printf("%s,%s,%s,%.4f,%.4f,%.2f\n", scenario.c_str(), param.c_str(), r.voter.c_str(), r.rmse, r.maxErr,
               r.decidedPct);
}

static string fmt(double x) {
    char b[32];
    snprintf(b, sizeof b, "%.1f", x);
    return b;
}

static void trace(const string& fault, int faultyChannel) {
    // 2 okresy po 40 probek; od probki 40 jeden kanal jest uszkodzony
    const double freq = 0.5, dur = 4, sr = 20, noise = 5, eps = 10;
    vector<double> truth = utils::DataGenerator::generateRandomSawtoothWave(freq, dur, sr, 0);
    vector<vector<double>> ch(3);
    for (auto& c : ch) c = utils::DataGenerator::generateRandomSawtoothWave(freq, dur, sr, noise);
    unsigned lcg = 1;
    injectFault(ch[faultyChannel], fault, truth.size() / 2, lcg);
    vector<Voter> vs = voters(eps);
    printf("i,truth,ch1,ch2,ch3");
    for (auto& v : vs) printf(",%s", v.name.c_str());
    printf("\n");
    for (size_t i = 0; i < truth.size(); ++i) {
        vector<double> v = {ch[0][i], ch[1][i], ch[2][i]};
        printf("%zu,%.4f,%.4f,%.4f,%.4f", i, truth[i], v[0], v[1], v[2]);
        for (auto& vt : vs) printf(",%.4f", vt.vote(v));
        printf("\n");
    }
}

int main(int argc, char** argv) {
    string mode = argc > 1 ? argv[1] : "summary";
    if (mode == "trace") {
        string fault = argc > 2 ? argv[2] : "stuck";
        int channel = argc > 3 ? atoi(argv[3]) : 1;
        trace(fault, channel - 1);
        return 0;
    }
    const int reps = 20;  // 20 x 1000 probek na kazdy punkt
    if (mode == "csv") {
        printf("scenario,parameter,voter,rmse,max_error,decided_pct\n");
        // 1) Prog grupowania przy szumie +-5 (w programie prog = 1)
        for (double eps : {0.5, 1.0, 2.0, 3.0, 5.0, 7.5, 10.0, 15.0, 20.0})
            printCsv("threshold", fmt(eps), evaluate(5, eps, "none", 0, reps));
        // 2) Poziom szumu przy progu 1
        for (double noise : {1.0, 2.0, 5.0, 10.0, 20.0})
            printCsv("noise", fmt(noise), evaluate(noise, 1, "none", 0, reps));
        // 3) Jeden uszkodzony kanal, szum +-5, prog 10
        for (string fault : {"stuck", "offset", "spikes"})
            for (int k = 0; k < 3; ++k)
                printCsv("fault_" + fault, "ch" + to_string(k + 1), evaluate(5, 10, fault, k, reps));
        return 0;
    }

    // Podsumowanie: RMSE wzgledem prawdziwego sygnalu, szum +-5, prog 10
    struct Case {
        string label, fault;
        int channel;
    };
    vector<Case> cases = {{"no fault", "none", 0},
                          {"ch1 stuck-at-0", "stuck", 0},
                          {"ch2 offset +60", "offset", 1},
                          {"ch3 spikes +-100", "spikes", 2}};
    printf("RMSE vs true signal (noise +-5, threshold 10, %d x 1000 samples)\n\n", reps);
    printf("%-18s", "scenario");
    for (const Voter& v : voters(10)) printf("%12s", v.name.c_str());
    printf("\n");
    for (const Case& c : cases) {
        printf("%-18s", c.label.c_str());
        for (const Row& r : evaluate(5, 10, c.fault, c.channel, reps)) printf("%12.1f", r.rmse);
        printf("\n");
    }
    printf("\nMajority vote decisions with no fault, noise +-5\n\n");
    for (double eps : {1.0, 2.0, 5.0}) {
        Row r = evaluate(5, eps, "none", 0, reps)[0];
        printf("  threshold %-4s  %5.1f%% decided, %5.1f%% returned NO_MAJORITY (-99)\n", fmt(eps).c_str(),
               r.decidedPct, 100 - r.decidedPct);
    }
    return 0;
}
