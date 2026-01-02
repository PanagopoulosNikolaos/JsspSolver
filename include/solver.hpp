#ifndef SOLVER_HPP
#define SOLVER_HPP

#include "models.hpp"
#include <queue>
#include <algorithm>
#include <functional>
#include <iostream>

/**
 * Enumeration for scheduling algorithms.
 */
enum class SchedulingAlgorithm {
    FIFO,
    SPT, // Shortest Processing Time
    LPT  // Longest Processing Time
};

/**
 * Class for solving job shop scheduling problems using various algorithms.
 */
class Solver {
private:
    SchedulingAlgorithm algorithm;
    
    // Helper methods for different algorithms
    /**
     * Schedules operations using FIFO (First In, First Out) algorithm.
     *
     * Args:
     *   problem: Problem instance to schedule.
     */
    void scheduleFIFO(std::shared_ptr<ProblemInstance> problem);

    /**
     * Schedules operations using SPT (Shortest Processing Time) algorithm.
     *
     * Args:
     *   problem: Problem instance to schedule.
     */
    void scheduleSPT(std::shared_ptr<ProblemInstance> problem);

    /**
     * Schedules operations using LPT (Longest Processing Time) algorithm.
     *
     * Args:
     *   problem: Problem instance to schedule.
     */
    void scheduleLPT(std::shared_ptr<ProblemInstance> problem);

    // Generic scheduling helper
    /**
     * Schedules operations using a custom priority comparison.
     *
     * Args:
     *   problem: Problem instance to schedule.
     *   compare: Comparison function for operation priority.
     */
    void scheduleWithPriority(std::shared_ptr<ProblemInstance> problem,
                             std::function<bool(const std::shared_ptr<Operation>&,
                                               const std::shared_ptr<Operation>&)> compare);
    
public:
    /**
     * Constructor for Solver.
     *
     * Args:
     *   algo: Scheduling algorithm to use.
     */
    Solver(SchedulingAlgorithm algo = SchedulingAlgorithm::FIFO);

    /**
     * Sets the scheduling algorithm.
     *
     * Args:
     *   algo: Algorithm to set.
     */
    void setAlgorithm(SchedulingAlgorithm algo);

    /**
     * Gets the current scheduling algorithm.
     *
     * Returns:
     *   Current algorithm.
     */
    SchedulingAlgorithm getAlgorithm() const;

    /**
     * Solves the problem instance using the current algorithm.
     *
     * Args:
     *   problem: Problem instance to solve.
     *
     * Returns:
     *   Schedule result.
     */
    std::shared_ptr<ScheduleResult> solve(std::shared_ptr<ProblemInstance> problem);

    // Static factory methods for creating solvers with specific algorithms
    /**
     * Creates a FIFO solver.
     *
     * Returns:
     *   FIFO solver instance.
     */
    static std::shared_ptr<Solver> createFIFOSolver();

    /**
     * Creates a SPT solver.
     *
     * Returns:
     *   SPT solver instance.
     */
    static std::shared_ptr<Solver> createSPTSolver();

    /**
     * Creates a LPT solver.
     *
     * Returns:
     *   LPT solver instance.
     */
    static std::shared_ptr<Solver> createLPTSolver();

    // Get algorithm name
    /**
     * Gets the name of the algorithm.
     *
     * Args:
     *   algo: Algorithm type.
     *
     * Returns:
     *   Algorithm name string.
     */
    static std::string getAlgorithmName(SchedulingAlgorithm algo);

    /**
     * Gets the name of the current algorithm.
     *
     * Returns:
     *   Current algorithm name.
     */
    std::string getCurrentAlgorithmName() const;

    // Compare solutions from different algorithms
    /**
     * Compares solutions from different algorithms.
     *
     * Args:
     *   result1: First schedule result.
     *   result2: Second schedule result.
     *   name1: Name for first algorithm.
     *   name2: Name for second algorithm.
     */
    static void compareSolutions(const std::shared_ptr<ScheduleResult>& result1,
                                const std::shared_ptr<ScheduleResult>& result2,
                                const std::string& name1 = "Algorithm 1",
                                const std::string& name2 = "Algorithm 2");
};

#endif // SOLVER_HPP
