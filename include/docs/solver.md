# Solver Documentation

## Overview
The Solver class implements various algorithms for solving job shop scheduling problems. It provides different scheduling strategies including FIFO (First In, First Out), SPT (Shortest Processing Time), and LPT (Longest Processing Time). The class is designed to take a problem instance and produce an optimized schedule result based on the selected algorithm.

## Key Features
- Multiple scheduling algorithms (FIFO, SPT, LPT)
- Flexible algorithm selection
- Static factory methods for common algorithms
- Solution comparison capabilities
- Performance metric calculation

## Dependencies
```cpp
#include "models.hpp"
#include <queue>
#include <algorithm>
#include <functional>
#include <iostream>
```

## Enumerations
### SchedulingAlgorithm
Defines the supported scheduling algorithms:
- `FIFO`: First In, First Out - processes operations in the order they appear
- `SPT`: Shortest Processing Time - prioritizes operations with shorter processing times
- `LPT`: Longest Processing Time - prioritizes operations with longer processing times

## Class Members

### Private Members
- `algorithm`: The currently selected scheduling algorithm

### Public Methods

#### `Solver(algo)`
Constructor for the Solver class.
- **Parameters**: `algo` - Scheduling algorithm to use (defaults to FIFO)

#### `setAlgorithm(algo)`
Sets the scheduling algorithm.
- **Parameters**: `algo` - Algorithm to set

#### `getAlgorithm()`
Gets the current scheduling algorithm.
- **Returns**: Current algorithm

#### `solve(problem)`
Solves the problem instance using the current algorithm.
- **Parameters**: `problem` - Problem instance to solve
- **Returns**: Schedule result

#### `createFIFOSolver()`
Creates a FIFO solver.
- **Returns**: FIFO solver instance

#### `createSPTSolver()`
Creates a SPT solver.
- **Returns**: SPT solver instance

#### `createLPTSolver()`
Creates a LPT solver.
- **Returns**: LPT solver instance

#### `getAlgorithmName(algo)`
Gets the name of the algorithm.
- **Parameters**: `algo` - Algorithm type
- **Returns**: Algorithm name string

#### `getCurrentAlgorithmName()`
Gets the name of the current algorithm.
- **Returns**: Current algorithm name

#### `compareSolutions(result1, result2, name1, name2)`
Compares solutions from different algorithms.
- **Parameters**: 
  - `result1` - First schedule result
  - `result2` - Second schedule result
  - `name1` - Name for first algorithm (default: "Algorithm 1")
  - `name2` - Name for second algorithm (default: "Algorithm 2")

### Private Helper Methods

#### `scheduleFIFO(problem)`
Schedules operations using FIFO (First In, First Out) algorithm.
- **Parameters**: `problem` - Problem instance to schedule

#### `scheduleSPT(problem)`
Schedules operations using SPT (Shortest Processing Time) algorithm.
- **Parameters**: `problem` - Problem instance to schedule

#### `scheduleLPT(problem)`
Schedules operations using LPT (Longest Processing Time) algorithm.
- **Parameters**: `problem` - Problem instance to schedule

#### `scheduleWithPriority(problem, compare)`
Schedules operations using a custom priority comparison.
- **Parameters**: 
  - `problem` - Problem instance to schedule
  - `compare` - Comparison function for operation priority

## Algorithm Descriptions

### FIFO (First In, First Out)
Processes operations in the order they appear in the problem definition. This is the simplest scheduling strategy and often serves as a baseline for comparison with other algorithms.

### SPT (Shortest Processing Time)
Prioritizes operations with shorter processing times. This algorithm typically results in lower average flow times and is optimal for minimizing mean completion time.

### LPT (Longest Processing Time)
Prioritizes operations with longer processing times. This approach can be beneficial in certain scenarios where longer operations need to be started early to prevent bottlenecks.

## Usage Example
```cpp
// Create a solver with default FIFO algorithm
Solver solver;

// Or create with a specific algorithm
Solver sptSolver(SchedulingAlgorithm::SPT);

// Or use factory methods
auto fifoSolver = Solver::createFIFOSolver();
auto sptSolver = Solver::createSPTSolver();
auto lptSolver = Solver::createLPTSolver();

// Solve a problem
auto problem = /* ... */;
auto result = solver.solve(problem);

// Change algorithm
solver.setAlgorithm(SchedulingAlgorithm::SPT);
auto sptResult = solver.solve(problem);

// Compare solutions
auto fifoResult = fifoSolver->solve(problem);
auto sptResult = sptSolver->solve(problem);
Solver::compareSolutions(fifoResult, sptResult, "FIFO", "SPT");

// Get algorithm names
std::string algoName = Solver::getAlgorithmName(SchedulingAlgorithm::SPT);
```

The Solver class serves as the core computational component of the JSSP solver system, implementing different scheduling strategies to optimize various performance metrics.