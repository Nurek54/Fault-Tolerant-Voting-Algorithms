#ifndef CSVWRITER_H
#define CSVWRITER_H

#include <string>
#include <vector>

// Zapisuje tabele (pierwszy wiersz to naglowek) do pliku CSV
void generateCSV(const std::string& filename, const std::vector<std::vector<std::string>>& data);

#endif // CSVWRITER_H
