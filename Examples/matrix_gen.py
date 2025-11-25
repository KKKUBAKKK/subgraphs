import random

def generate_multigraph_matrices(n, m, filename):
    """
    Generuje dwie macierze sąsiedztwa dla multigrafów skierowanych
    i zapisuje je do pliku tekstowego.

    Args:
        n (int): Liczba wierzchołków dla pierwszej macierzy.
        m (int): Liczba wierzchołków dla drugiej macierzy.
        filename (str): Nazwa pliku, do którego zostaną zapisane macierze.
    """
    with open(filename, 'w') as f:
        # Pierwsza macierz (n x n)
        f.write(str(n) + '\n')
        for i in range(n):
            row = []
            for j in range(n):
                if i == j:
                    row.append(0)
                else:
                    row.append(random.randint(0, 20))
            f.write(' '.join(map(str, row)) + '\n')

        # Druga macierz (m x m)
        f.write(str(m) + '\n')
        for i in range(m):
            row = []
            for j in range(m):
                if i == j:
                    row.append(0)
                else:
                    row.append(random.randint(0, 20))
            f.write(' '.join(map(str, row)) + '\n')

if __name__ == '__main__':
    #generate_multigraph_matrices(5, 7, "approx1.txt")
    generate_multigraph_matrices(10, 20, "approx3.txt")
    generate_multigraph_matrices(35, 40, "approx4.txt")
    print("Wygenerowano pliki")
