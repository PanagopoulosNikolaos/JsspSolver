# Solver Documentation

## Overview
The Solver class implements various scheduling algorithms for solving Job Shop Scheduling Problems (JSSP). It provides different approaches including FIFO (First-In-First-Out), SPT (Shortest Processing Time), and LPT (Longest Processing Time) to find optimal or near-optimal solutions for scheduling operations on machines.

## Class Definition
```cpp
enum class SchedulingAlgorithm {
    FIFO,
    SPT,
    LPT
};

class Solver {
private:
    SchedulingAlgorithm algorithm;
    void scheduleFIFO(std::shared_ptr<ProblemInstance> problem);
    void scheduleSPT(std::shared_ptr<ProblemInstance> problem);
    void scheduleLPT(std::shared_ptr<ProblemInstance> problem);
    void scheduleWithPriority(std::shared_ptr<ProblemInstance> problem, 
                             std::function<bool(const std::shared_ptr<Operation>&, 
                                               const std::shared_ptr<Operation>&)> compare);

public:
    Solver(SchedulingAlgorithm algo = SchedulingAlgorithm::FIFO);
    std::shared_ptr<ScheduleResult> solve(std::shared_ptr<ProblemInstance> problem);
    void setAlgorithm(SchedulingAlgorithm algo);
    SchedulingAlgorithm getAlgorithm() const;
    static std::shared_ptr<Solver> createFIFOSolver();
    static std::shared_ptr<Solver> createSPTSolver();
    static std::shared_ptr<Solver> createLPTSolver();
    std::string getCurrentAlgorithmName() const;
    static std::string getAlgorithmName(SchedulingAlgorithm algo);
    static void compareSolutions(const std::shared_ptr<ScheduleResult>& result1, 
                               const std::shared_ptr<ScheduleResult>& result2,
                               const std::string& name1, const std::string& name2);
};
```

## Key Functions

### Scheduling Algorithms
- `scheduleFIFO()`: Implements the First-In-First-Out algorithm, scheduling operations in the order they were added to each job
- `scheduleSPT()`: Implements the Shortest Processing Time algorithm, prioritizing operations with shorter processing times
- `scheduleLPT()`: Implements the Longest Processing Time algorithm, prioritizing operations with longer processing times
- `scheduleWithPriority()`: Generic function that schedules operations based on a custom comparison function

### Main Solve Function
- `solve()`: The primary function that applies the selected algorithm to solve the problem and calculate performance metrics

### Factory Methods
- `createFIFOSolver()`, `createSPTSolver()`, `createLPTSolver()`: Static factory methods for creating solvers with specific algorithms

### Utility Functions
- `setAlgorithm()`, `getAlgorithm()`: Manage the scheduling algorithm used by the solver
- `getAlgorithmName()`, `getCurrentAlgorithmName()`: Return human-readable names for algorithms
- `compareSolutions()`: Compares the results of two different scheduling algorithms

## Algorithm Details

### FIFO (First-In-First-Out)
This algorithm processes operations in the order they were added to each job. It respects the precedence constraints within each job, ensuring that earlier operations in a job are completed before later operations begin.

### SPT (Shortest Processing Time)
This algorithm prioritizes operations with shorter processing times. It aims to minimize the average completion time by completing shorter tasks first, which can lead to better resource utilization.

### LPT (Longest Processing Time)
This algorithm prioritizes operations with longer processing times. It can be useful in scenarios where longer tasks need to be started early to ensure they finish on time.

## Solution Comparison
The solver includes functionality to compare the results of different algorithms based on key performance metrics:
- Makespan: Total time to complete all jobs
- Total Completion Time: Sum of completion times for all jobs
- Average Flow Time: Average time jobs spend in the system

## Usage
The solver is typically instantiated with a specific algorithm, then applied to a ProblemInstance to generate a ScheduleResult. The solution can then be visualized or exported using other components of the system.