#ifndef VOTING_H
#define VOTING_H

#include <vector>

// Wartosc zwracana przez glosowanie wiekszosciowe, gdy zadna grupa nie ma wiekszosci
constexpr double NO_MAJORITY = -99;

// Dodaje glos do pierwszej grupy, ktorej pierwszy element rozni sie o co najwyzej prog,
// albo tworzy nowa grupe. Zwraca indeks grupy.
int znajdzLubUtworzGrupe(std::vector<std::vector<double>>& grupy, double glos, double prog);

// Nieprecyzyjne glosowanie: wiekszosciowe (czyWiekszosc = true) albo pluralistyczne (false).
// Zwraca pierwszy glos z najwiekszej grupy albo NO_MAJORITY.
double votingAlgorithm(const std::vector<double>& glosy, double prog, bool czyWiekszosc);

// Srednia wazona glosow
double weightedAverageVoting(const std::vector<double>& glosy, const std::vector<double>& wagi);

// Mediana glosow
double medianVoting(const std::vector<double>& glosy);

// Srednia arytmetyczna glosow
double linearVoting(const std::vector<double>& glosy);

// Pierwszy glos z listy
double firstOrderVoting(const std::vector<double>& glosy);

#endif // VOTING_H
