=== INSTRUKCJA OBSŁUGI PROGRAMU SUBGRAPHS ===

KOMPILACJA (JESLI NIE MA PLIKU WYKONYWALNEGO):
  cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
  cmake --build build --config Release

URUCHOMIENIE:
  <plik_wykonywalny> <plik_wejściowy> [liczba_podgrafów] [algorytm] [heurystyka]

ARGUMENTY:
  <plik_wykonywalny> - ścieżka do pliku wykonywalnego (wymagany)
  <plik_wejściowy>   - ścieżka do pliku z grafami (wymagany)
  [liczba_podgrafów] - liczba kopii grafu wzorca (domyślnie: 1)
  [algorytm]         - exact lub approx1|approx2 (domyślnie: exact)
  [heurystyka]       - degree|directed|directed_ignore|histogram|structure|greedy
                       (tylko dla approx2, domyślnie: degree)

ALGORYTMY:
  exact   - algorytm dokładny (najwolniejszy, optymalne rozwiązanie)
  approx1 - algorytm aproksymacyjny v1
  approx2 - algorytm aproksymacyjny v2 z różnymi heurystykami

HEURYSTYKI:
  degree          - różnica stopni
  directed        - różnica stopni z kierunkami
  directed_ignore - różnica stopni z kierunkami, zignorowanie nadmiaru
  histogram       - histogram sąsiedztwa
  structure       - struktura sąsiedztwa z trójkątami
  greedy          - wyznaczanie wag na podstawie chciwie przybliżonych wag sąsiadów

FORMAT PLIKU WEJŚCIOWEGO:
  Pierwsza liczba: liczba wierzchołków grafu wzorca
  Następne linie: macierz sąsiedztwa grafu wzorca
  Kolejna liczba: liczba wierzchołków grafu docelowego
  Następne linie: macierz sąsiedztwa grafu docelowego

PRZYKŁADY UŻYCIA (UNIX):
  # Algorytm dokładny, 1 podgraf
  ./build/bin/release/subgraphs Examples/dokladny1.txt

  # Algorytm dokładny, 3 podgrafy
  ./build/bin/release/subgraphs Examples/dokladny1.txt 3

  # Algorytm aproksymacyjny v1, 1 podgraf
  ./build/bin/release/subgraphs Examples/approx1.txt 1 approx1

  # Algorytm aproksymacyjny v2, 1 podgraf, heurystyka degree
  ./build/bin/release/subgraphs Examples/approx2.txt 1 approx2 degree

  # Algorytm aproksymacyjny v2, 2 podgrafy, heurystyka structure
  ./build/bin/release/subgraphs Examples/approx2.txt 2 approx2 directed

PRZYKŁADY UŻYCIA (WINDOWS):
  # Algorytm dokładny, 1 podgraf
  .\build\bin\release\subgraphs Examples\dokladny1.txt

  # Algorytm dokładny, 3 podgrafy
  .\build\bin\release\subgraphs Examples\dokladny1.txt 3

  # Algorytm aproksymacyjny v1, 1 podgraf
  .\build\bin\release\subgraphs Examples\approx1.txt 1 approx1

  # Algorytm aproksymacyjny v2, 1 podgraf, heurystyka degree
  .\build\bin\release\subgraphs Examples\approx2.txt 1 approx2 degree

  # Algorytm aproksymacyjny v2, 2 podgrafy, heurystyka structure
  .\build\bin\release\subgraphs Examples\approx3.txt 2 approx2 directed

WYJŚCIE PROGRAMU:
  Program wyświetli:
  - parametry uruchomienia
  - informacje o grafach (liczba wierzchołków i krawędzi)
  - macierze sąsiedztwa grafów
  - listę krawędzi do dodania (rozszerzenie)
  - zmodyfikowaną macierz grafu docelowego
  - czas wykonania
