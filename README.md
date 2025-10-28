# subgraphs
Algorithm for checking if a graph $G_s$ is a subgraph of $G_b$ in $O(k^n)$. If it isn't, then program finds minimal extension of graph $G_b$ that makes $G_s$ its subgraph also in $O(k^n)$.
There is also a version of this algorithm that uses approximation, to get the final results in $O(n^j)$ time.

# TODO:
1. Decide on graph representation
2. Decide on the metric to determine how much do the graphs resemble each other
3. Checking if $G_s$ is a subgraph of $G_b$
4. Finding minimal extension of $G_b$ so that $G_s$ will be a subgraph of the extended graph
5. Approximation for points 3 and 4
