import random

def generate_multigraph_matrices(n, m, sparse_n=False, sparse_m=False, density=0.1):
    """
    Generuje dwie macierze sąsiedztwa dla multigrafów skierowanych.
    Pozwala określić, czy macierze mają być rzadkie.

    Args:
        n (int): Liczba wierzchołków dla pierwszej macierzy.
        m (int): Liczba wierzchołków dla drugiej macierzy.
        filename (str): Nazwa pliku wyjściowego.
        sparse_n (bool): Czy pierwsza macierz ma być rzadka?
        sparse_m (bool): Czy druga macierz ma być rzadka?
        density (float): Gęstość wypełnienia dla macierzy rzadkich (0.0 - 1.0). 
    """

    filename = f'Examples/graph_{n}_{m}_{sparse_n}_{sparse_m}.txt'
    
    # Funkcja pomocnicza do losowania wartości
    def get_value(is_sparse):
        if is_sparse:
            if random.random() < density:
                return random.randint(1, 20)
            else:
                return 0
        else:
            return random.randint(0, 20)


    with open(filename, 'w') as f:
        # --- Pierwsza macierz (n x n) ---
        f.write(str(n) + '\n')
        for i in range(n):
            row = []
            for j in range(n):
                if i == j:
                    row.append(0)
                else:
                    row.append(get_value(sparse_n))
            f.write(' '.join(map(str, row)) + '\n')

        # --- Druga macierz (m x m) ---
        f.write(str(m) + '\n')
        for i in range(m):
            row = []
            for j in range(m):
                if i == j:
                    row.append(0)
                else:
                    row.append(get_value(sparse_m))
            f.write(' '.join(map(str, row)) + '\n')


if __name__ == '__main__':
    generate_multigraph_matrices(10, 20, True, False)
    print("Wygenerowano pliki")
