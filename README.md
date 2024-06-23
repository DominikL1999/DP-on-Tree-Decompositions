# DP-on-Tree-Decompositions
Repository for Dynamic Programming Algorithms on Graph Tree Decompositions.

The problem to solve is the __MIN (WEIGHTED) VERTEX COVER__:

Given a graph G = (V, E) with treewidth < k and a function w : V -> Reals, find the subset X of V, such that every edge in E is incident to at least one vertex in X and such that the sum of all w(x) with x from X is minimal.

## Solution Description
For a description of the Dynamic Programming algorithm, see "solution_description.pdf" or "solution_description.tex".

## Input file format
The file format for the graphs and tree decompositions is the CSV file format described in https://github.com/ciaranm/glasgow-subgraph-solver?tab=readme-ov-file#file-formats.

## Installation
1. Clone the repository.
2. Open a shell and navigate to the root of the workspace.
3. Run `cmake. -B build`.
4. Navigate to the build folder and run `make`.

## Usage
The ./main executable parses a graph and its tree decomposition (2 separate files). All vertices in the graph input file and all nodes in the tree decomposition file are expected to have integer labels. Then it modifies the tree decomposition to be nice. Finally it solves the instance to optimality and outputs the found solution.

As a rule of thumb: If the width of the given tree decomposition is greater than 20, the program is not expected to terminate within a reasonable amount of time.

Example usage:

1. Navigate to build folder.
2. Call `./main ../test-instances/Treewidth-PACE-2017-Instances/ex001.gr.csv ../test-instances/Treewidth-PACE-2017-Instances/ex001.td.csv`.

## Testing
1. Navigate to the build folder.
2. Call `ctest`.

## TODO

- ✅ Write project summary in README.md.
- ✅ Create CMakeLists.txt.
- Create classes for ...
    1. ✅ Graphs
    2. ✅ Tree Decompositions
    3. ✅ Solver
- ✅ Come up with tiny problem instances.
- ✅ Parse tiny problem instances.
- ✅ Find/come up with (possibly large) instances with small treewidth to test solver on.
- ✅ Solve MINIMUM WEIGHTED VERTEX COVER on instances with small treewidth.
