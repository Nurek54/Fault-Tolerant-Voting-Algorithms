Chętnie wyjaśnię, jak działa każda część tego programu krok po kroku:

### 1. Funkcja `znajdzLubUtworzGrupe`
Ta funkcja ma na celu znalezienie istniejącej grupy dla danego głosu, gdzie grupa to zbiór głosów odległych od siebie o nie więcej niż określony próg (`prog`). Jeśli taka grupa nie istnieje, funkcja tworzy nową grupę z tym głosem.
- **Parametry**:
  - `grupy`: wektor grup, gdzie każda grupa to wektor głosów.
  - `glos`: głos, który chcemy zaklasyfikować do grupy.
  - `prog`: maksymalna różnica między głosami, aby mogły należeć do tej samej grupy.
- **Działanie**:
  - Iteruje przez listę istniejących grup.
  - Jeśli znajdzie grupę, do której pasuje głos (różnica między głosami w grupie a bieżącym głosem jest mniejsza lub równa progu), zwraca indeks tej grupy.
  - Jeśli nie znajdzie pasującej grupy, tworzy nową grupę z tym głosem i dodaje ją do listy grup.
  - Zwraca indeks nowej lub znalezionej grupy.

### 2. Funkcja `votingAlgorithm`
Funkcja obsługuje głosowanie większościowe i pluralistyczne z możliwością niedokładnych danych.
- **Parametry**:
  - `glosy`: wektor zawierający wszystkie głosy.
  - `prog`: próg różnicy między głosami do zgrupowania w jednej grupie.
  - `czyWiekszosc`: flaga określająca, czy stosujemy głosowanie większościowe (true) czy pluralistyczne (false).
- **Działanie**:
  - Tworzy puste grupy głosów.
  - Dla każdego głosu używa `znajdzLubUtworzGrupe`, aby dodać głos do odpowiedniej grupy.
  - Analizuje wszystkie grupy, aby znaleźć grupę z największą liczbą głosów.
  - W przypadku głosowania większościowego sprawdza, czy grupa zawiera więcej niż połowę wszystkich głosów. Jeśli tak, zwraca reprezentanta tej grupy; jeśli nie, zwraca `NO_MAJORITY` (-99).
  - W przypadku głosowania pluralistycznego zwraca reprezentanta największej grupy bez względu na jej wielkość.

### 3. Funkcja `weightedAverageVoting`
Oblicza średnią ważoną głosów.
- **Parametry**:
  - `glosy`: wektor głosów.
  - `wagi`: wektor wag odpowiadających każdemu głosowi.
- **Działanie**:
  - Sumuje iloczyny głosów i ich wag.
  - Sumuje wszystkie wagi.
  - Dzieli sumę ważonych głosów przez sumę wag, zwracając średnią ważoną.

### 4. Funkcja `medianVoting`
Oblicza medianę z listy głosów.
- **Działanie**:
  - Sortuje listę głosów.
  - Jeśli liczba głosów jest parzysta, zwraca średnią dwóch środkowych wartości.
  - Jeśli liczba głosów jest nieparzysta, zwraca środkowy element listy.

### 5. Funkcja `linearVoting`
Oblicza średnią arytmetyczną głosów.
- **Działanie**:
  - Sumuje wszystkie głosy.
  - Dzieli sumę przez liczbę głosów, zwracając średnią arytmetyczną.

### 6. Funkcja `firstOrderVoting`
Zwraca pierwszy głos z listy, reprezentujący "pierwszorzędowe" głosowanie.
- **Działanie**:
  - Zwraca pierwszy element z wektora

 głosów.

### 7. Funkcja `main`
Główna funkcja programu, która demonstruje użycie powyższych funkcji.
- **Działanie**:
  - Definiuje wektor głosów i wagi.
  - Wywołuje wszystkie funkcje głosowania i wyświetla ich wyniki.

