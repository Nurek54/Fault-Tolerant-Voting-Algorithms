#include "voting/Voting.h"

#include <algorithm>
#include <cmath>

using namespace std;

// Funkcja do znajdowania lub tworzenia grupy dla głosu
int znajdzLubUtworzGrupe(vector<vector<double>>& grupy, double glos, double prog) {
    for (size_t i = 0; i < grupy.size(); ++i) {
        if (!grupy[i].empty() && fabs(grupy[i][0] - glos) <= prog) {
            grupy[i].push_back(glos);
            return i;
        }
    }
    vector<double> nowaGrupa;
    nowaGrupa.push_back(glos);
    grupy.push_back(nowaGrupa);
    return grupy.size() - 1;
}

// Nieprecyzyjne głosowanie większościowe i pluralistyczne
double votingAlgorithm(const vector<double>& glosy, double prog, bool czyWiekszosc) {
    vector<vector<double>> grupy;
    for (double glos : glosy) {
        znajdzLubUtworzGrupe(grupy, glos, prog);
    }

    int indeksGrupyKandydata = -1;
    size_t maksRozmiarGrupy = 0;
    for (size_t i = 0; i < grupy.size(); ++i) {
        if (grupy[i].size() > maksRozmiarGrupy) {
            maksRozmiarGrupy = grupy[i].size();
            indeksGrupyKandydata = i;
        }
    }

    if (czyWiekszosc && maksRozmiarGrupy <= glosy.size() / 2) {
        return NO_MAJORITY; // Brak kandydata większościowego
    }
    return grupy[indeksGrupyKandydata][0]; // Pierwszy glos z najwiekszej grupy
}

// Weighted Average Voting
double weightedAverageVoting(const vector<double>& glosy, const vector<double>& wagi) {
    double suma = 0.0;
    double sumaWag = 0.0;
    for (size_t i = 0; i < glosy.size(); ++i) {
        suma += glosy[i] * wagi[i];
        sumaWag += wagi[i];
    }
    return suma / sumaWag;
}

// Median Voting
double medianVoting(const vector<double>& glosy) {
    vector<double> posortowaneGlosy = glosy;
    sort(posortowaneGlosy.begin(), posortowaneGlosy.end());
    size_t n = posortowaneGlosy.size();
    if (n % 2 == 0) {
        return (posortowaneGlosy[n / 2 - 1] + posortowaneGlosy[n / 2]) / 2.0;
    } else {
        return posortowaneGlosy[n / 2];
    }
}

// Linear Voting
double linearVoting(const vector<double>& glosy) {
    double suma = 0.0;
    for (double glos : glosy) {
        suma += glos;
    }
    return suma / glosy.size();
}

// First-Order Voting
double firstOrderVoting(const vector<double>& glosy) {
    return glosy.front();
}
