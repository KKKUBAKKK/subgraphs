
namespace Subgraphs {

template <typename IndexType>
std::vector<std::vector<uint8_t>>
GraphLoader<IndexType>::loadAdjacencyMatrix(const std::filesystem::path& filePath) {
    std::ifstream infile(filePath);
    if (!infile.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath.string());
    }

    std::string line;
    int n = 0;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        if (iss >> n)
            break;
    }
    if (n <= 0)
        throw std::runtime_error("Invalid or missing matrix size in file: " + filePath.string());

    std::vector<std::vector<uint8_t>> matrix;
    for (int i = 0; i < n; ++i) {
        if (!std::getline(infile, line))
            throw std::runtime_error("Unexpected end of file while reading adjacency matrix: " +
                                     filePath.string());
        std::istringstream iss(line);
        std::vector<uint8_t> row;
        int val;
        for (int j = 0; j < n; ++j) {
            if (!(iss >> val))
                throw std::runtime_error("Malformed adjacency matrix row in file: " +
                                         filePath.string());
            row.push_back(static_cast<uint8_t>(val));
        }
        matrix.push_back(std::move(row));
    }
    return matrix;
}

template <typename IndexType>
std::pair<Multigraph<IndexType>, Multigraph<IndexType>>
GraphLoader<IndexType>::loadFromFile(const std::filesystem::path& filePath) {
    std::ifstream infile(filePath);
    if (!infile.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath.string());
    }

    std::string line;
    int n1 = 0;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        if (iss >> n1)
            break;
    }
    if (n1 <= 0)
        throw std::runtime_error("Invalid or missing first matrix size in file: " +
                                 filePath.string());

    std::vector<std::vector<uint8_t>> matrix1;
    for (int i = 0; i < n1; ++i) {
        if (!std::getline(infile, line))
            throw std::runtime_error("Unexpected end of file while reading first matrix: " +
                                     filePath.string());
        std::istringstream iss(line);
        std::vector<uint8_t> row;
        int val;
        for (int j = 0; j < n1; ++j) {
            if (!(iss >> val))
                throw std::runtime_error("Malformed first matrix row in file: " +
                                         filePath.string());
            row.push_back(static_cast<uint8_t>(val));
        }
        matrix1.push_back(std::move(row));
    }

    int n2 = 0;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        if (iss >> n2)
            break;
    }
    if (n2 <= 0)
        throw std::runtime_error("Invalid or missing second matrix size in file: " +
                                 filePath.string());

    std::vector<std::vector<uint8_t>> matrix2;
    for (int i = 0; i < n2; ++i) {
        if (!std::getline(infile, line))
            throw std::runtime_error("Unexpected end of file while reading second matrix: " +
                                     filePath.string());
        std::istringstream iss(line);
        std::vector<uint8_t> row;
        int val;
        for (int j = 0; j < n2; ++j) {
            if (!(iss >> val))
                throw std::runtime_error("Malformed second matrix row in file: " +
                                         filePath.string());
            row.push_back(static_cast<uint8_t>(val));
        }
        matrix2.push_back(std::move(row));
    }

    Multigraph<IndexType> g1(std::move(matrix1));
    Multigraph<IndexType> g2(std::move(matrix2));

    if (g1 < g2) {
        return {std::move(g1), std::move(g2)};
    }

    return {std::move(g2), std::move(g1)};
}

template <typename IndexType>
void GraphLoader<IndexType>::saveToFile(
    const Multigraph<IndexType>& g1, const Multigraph<IndexType>& g2,
    const std::vector<std::tuple<IndexType, IndexType, uint8_t>>& extension, int subgraphsCount,
    const std::filesystem::path& filePath) {
    std::ofstream outfile(filePath);
    if (!outfile.is_open()) {
        throw std::runtime_error("Could not open file: " + filePath.string());
    }

    auto n1 = g1.getVertexCount();
    outfile << n1 << "\n";
    for (IndexType i = 0; i < n1; ++i) {
        for (IndexType j = 0; j < n1; ++j) {
            outfile << static_cast<int>(g1.getEdges(i, j));
            if (j + 1 < n1)
                outfile << " ";
        }
        outfile << "\n";
    }

    auto n2 = g2.getVertexCount();
    outfile << n2 << "\n";
    for (IndexType i = 0; i < n2; ++i) {
        for (IndexType j = 0; j < n2; ++j) {
            outfile << static_cast<int>(g2.getEdges(i, j));
            if (j + 1 < n2)
                outfile << " ";
        }
        outfile << "\n";
    }
}

} // namespace Subgraphs
