# DP-on-Tree-Decompositions
Repository for Dynamic Programming Algorithms on Graph Tree Decompositions.

The first (and possibly only) problem to solve is the __MIN (WEIGHTED) VERTEX COVER__.

## Problem Description: MINIMUM WEIGHTED VERTEX COVER
Given a graph G = (V, E) with treewidth < k and a function w : V -> Reals, find the subset X of V, such that every edge in E is incident to at least one vertex in X and such that the sum of all w(x) with x from X is minimal.

## Input file format
The file format for the graphs is the CSV file format described in https://github.com/ciaranm/glasgow-subgraph-solver?tab=readme-ov-file#file-formats.

## TODO

- ✅ Write project summary in README.md.
- ✅ Create CMakeLists.txt.
- Create classes for ...
    1. Graphs. This class should cover all combinations of:
        - Labelled / Unlabelled vertices
        - Labelled / Unlabelled edges (for interoperability with other projects)
        - Directed / Undirected edges (for interoperability with other projects)\
        All labels may be assumed to be numbers.
    2. Tree Decompositions
    3. Solver
- Come up with tiny problem instances.
- Parse tiny problem instances.
- Find/come up with (possibly large) instances with small treewidth to test solver on.
- Solve MINIMUM WEIGHTED VERTEX COVER on instances with small treewidth.
- Write unit test to check correctness of solver by brute forcing all possible solutions for the tiny instances.
