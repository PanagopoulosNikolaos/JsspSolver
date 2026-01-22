# JSSP Solver Test Documentation: Solver Tests

## Overview

The `test_solver.cpp` file contains comprehensive unit tests for the solver module of the Job Shop Scheduling Problem (JSSP) solver. These tests verify the correct implementation of scheduling algorithms including FIFO (First-In-First-Out), SPT (Shortest Processing Time), and LPT (Longest Processing Time). The solver module is responsible for generating schedules that optimize different objectives and calculating performance metrics.

Key responsibilities of the solver include:
- Implementing multiple scheduling algorithms (FIFO, SPT, LPT)
- Creating solver instances with different algorithms
- Solving JSSP problems and generating schedules
- Calculating performance metrics (makespan, total completion time, average flow time)
- Comparing solutions from different algorithms
- Handling edge cases and error conditions

## Test Structure

All tests are organized within the `SolverTest` class which inherits from `::testing::Test`. This provides a common setup mechanism for all solver-related tests.

### Setup Method

```cpp
void SetUp() override {
    // Create a simple test problem
    problem = Parser::generateSimpleProblem();
}
```

The setup method creates a standard test problem that can be used across multiple test cases.

## FIFO Algorithm Tests

### Solve With FIFO Test

**Test Name**: `SolveWithFIFO`

This test verifies that the FIFO algorithm correctly solves a problem and produces valid results:

```cpp
TEST_F(SolverTest, SolveWithFIFO) {
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::FIFO);
    auto result = solver->solve(problem);
    
    ASSERT_NE(result, nullptr);
    EXPECT_GT(result->makespan, 0);
    EXPECT_GT(result->totalCompletionTime, 0);
    EXPECT_GT(result->avgFlowTime, 0);
    
    // Verify all operations are scheduled
    for (const auto& job : result->problem.jobs) {
        for (const auto& operation : job->operations) {
            EXPECT_TRUE(operation->isScheduled());
            EXPECT_GE(operation->startTime, 0);
            EXPECT_GT(operation->endTime, operation->startTime);
        }
    }
    
    // Verify makespan calculation
    int maxCompletion = 0;
    for (const auto& job : result->problem.jobs) {
        int jobCompletion = 0;
        for (const auto& operation : job->operations) {
            jobCompletion = std::max(jobCompletion, operation->endTime);
        }
        maxCompletion = std::max(maxCompletion, jobCompletion);
    }
    EXPECT_EQ(result->makespan, maxCompletion);
}
```

The test confirms that:
- The solver returns a valid result
- All metrics are positive values
- All operations are scheduled with valid start and end times
- The makespan is correctly calculated as the maximum job completion time

## SPT Algorithm Tests

### Solve With SPT Test

**Test Name**: `SolveWithSPT`

This test verifies that the SPT algorithm correctly solves a problem and produces valid results:

```cpp
TEST_F(SolverTest, SolveWithSPT) {
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::SPT);
    auto result = solver->solve(problem);
    
    ASSERT_NE(result, nullptr);
    EXPECT_GT(result->makespan, 0);
    EXPECT_GT(result->totalCompletionTime, 0);
    EXPECT_GT(result->avgFlowTime, 0);
    
    // Verify all operations are scheduled
    for (const auto& job : result->problem.jobs) {
        for (const auto& operation : job->operations) {
            EXPECT_TRUE(operation->isScheduled());
            EXPECT_GE(operation->startTime, 0);
            EXPECT_GT(operation->endTime, operation->startTime);
        }
    }
}
```

The test ensures that the SPT algorithm produces valid schedules with all operations properly scheduled.

## LPT Algorithm Tests

### Solve With LPT Test

**Test Name**: `SolveWithLPT`

This test verifies that the LPT algorithm correctly solves a problem and produces valid results:

```cpp
TEST_F(SolverTest, SolveWithLPT) {
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::LPT);
    auto result = solver->solve(problem);
    
    ASSERT_NE(result, nullptr);
    EXPECT_GT(result->makespan, 0);
    EXPECT_GT(result->totalCompletionTime, 0);
    EXPECT_GT(result->avgFlowTime, 0);
    
    // Verify all operations are scheduled
    for (const auto& job : result->problem.jobs) {
        for (const auto& operation : job->operations) {
            EXPECT_TRUE(operation->isScheduled());
            EXPECT_GE(operation->startTime, 0);
            EXPECT_GT(operation->endTime, operation->startTime);
        }
    }
}
```

The test ensures that the LPT algorithm produces valid schedules with all operations properly scheduled.

## Algorithm Factory Tests

### Create FIFO Solver Test

**Test Name**: `CreateFIFOSolver`

This test verifies the factory method for creating FIFO solvers:

```cpp
TEST_F(SolverTest, CreateFIFOSolver) {
    auto solver = Solver::createFIFOSolver();
    ASSERT_NE(solver, nullptr);
    EXPECT_EQ(solver->getAlgorithm(), SchedulingAlgorithm::FIFO);
}
```

The test confirms that the factory method returns a valid solver configured with the FIFO algorithm.

### Create SPT Solver Test

**Test Name**: `CreateSPTSolver`

This test verifies the factory method for creating SPT solvers:

```cpp
TEST_F(SolverTest, CreateSPTSolver) {
    auto solver = Solver::createSPTSolver();
    ASSERT_NE(solver, nullptr);
    EXPECT_EQ(solver->getAlgorithm(), SchedulingAlgorithm::SPT);
}
```

The test confirms that the factory method returns a valid solver configured with the SPT algorithm.

### Create LPT Solver Test

**Test Name**: `CreateLPTSolver`

This test verifies the factory method for creating LPT solvers:

```cpp
TEST_F(SolverTest, CreateLPTSolver) {
    auto solver = Solver::createLPTSolver();
    ASSERT_NE(solver, nullptr);
    EXPECT_EQ(solver->getAlgorithm(), SchedulingAlgorithm::LPT);
}
```

The test confirms that the factory method returns a valid solver configured with the LPT algorithm.

## Algorithm Name Tests

### Get Algorithm Name Test

**Test Name**: `GetAlgorithmName`

This test verifies that algorithm names are returned correctly:

```cpp
TEST_F(SolverTest, GetAlgorithmName) {
    EXPECT_EQ(Solver::getAlgorithmName(SchedulingAlgorithm::FIFO), "FIFO (First-In-First-Out)");
    EXPECT_EQ(Solver::getAlgorithmName(SchedulingAlgorithm::SPT), "SPT (Shortest Processing Time)");
    EXPECT_EQ(Solver::getAlgorithmName(SchedulingAlgorithm::LPT), "LPT (Longest Processing Time)");
}
```

The test ensures that each algorithm has the correct descriptive name.

### Get Current Algorithm Name Test

**Test Name**: `GetCurrentAlgorithmName`

This test verifies that the current algorithm name is returned correctly:

```cpp
TEST_F(SolverTest, GetCurrentAlgorithmName) {
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::SPT);
    EXPECT_EQ(solver->getCurrentAlgorithmName(), "SPT (Shortest Processing Time)");
}
```

The test confirms that the solver instance can return the name of its currently configured algorithm.

## Algorithm Switching Tests

### Set Algorithm Test

**Test Name**: `SetAlgorithm`

This test verifies that algorithms can be changed dynamically:

```cpp
TEST_F(SolverTest, SetAlgorithm) {
    auto solver = std::make_shared<Solver>();
    solver->setAlgorithm(SchedulingAlgorithm::LPT);
    EXPECT_EQ(solver->getAlgorithm(), SchedulingAlgorithm::LPT);
}
```

The test ensures that algorithms can be switched after solver creation.

## Comparison Tests

### Compare Solutions Test

**Test Name**: `CompareSolutions`

This test verifies that solution comparison works correctly:

```cpp
TEST_F(SolverTest, CompareSolutions) {
    auto solver1 = std::make_shared<Solver>(SchedulingAlgorithm::FIFO);
    auto solver2 = std::make_shared<Solver>(SchedulingAlgorithm::SPT);
    
    auto result1 = solver1->solve(problem);
    auto result2 = solver2->solve(problem);
    
    ASSERT_NE(result1, nullptr);
    ASSERT_NE(result2, nullptr);
    
    // Test comparison function (should not throw)
    EXPECT_NO_THROW({
        Solver::compareSolutions(result1, result2, "FIFO", "SPT");
    });
}
```

The test ensures that solution comparison can be performed without throwing exceptions.

## Edge Case Tests

### Solve Null Problem Test

**Test Name**: `SolveNullProblem`

This test handles the edge case of solving a null problem:

```cpp
TEST(SolverEdgeCases, SolveNullProblem) {
    auto solver = std::make_shared<Solver>();
    
    EXPECT_THROW({
        try {
            solver->solve(nullptr);
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ("Problem instance is null", e.what());
            throw;
        }
    }, std::runtime_error);
}
```

The test confirms that appropriate error handling occurs when trying to solve a null problem.

### Solve Empty Problem Test

**Test Name**: `SolveEmptyProblem`

This test handles solving an empty problem:

```cpp
TEST(SolverEdgeCases, SolveEmptyProblem) {
    auto emptyProblem = std::make_shared<ProblemInstance>();
    auto solver = std::make_shared<Solver>();
    
    // Should handle empty problem gracefully
    auto result = solver->solve(emptyProblem);
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->makespan, 0);
    EXPECT_EQ(result->totalCompletionTime, 0);
    EXPECT_DOUBLE_EQ(result->avgFlowTime, 0.0);
}
```

The test ensures that empty problems are handled gracefully with appropriate default metric values.

### Solve Single Operation Test

**Test Name**: `SolveSingleOperation`

This test validates solving a problem with a single operation:

```cpp
TEST(SolverEdgeCases, SolveSingleOperation) {
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(1);
    problem->createMachines(1);
    
    auto op = std::make_shared<Operation>(0, 0, 5, 0);
    problem->getJob(0)->addOperation(op);
    
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::FIFO);
    auto result = solver->solve(problem);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->makespan, 5);
    EXPECT_EQ(result->totalCompletionTime, 5);
    EXPECT_DOUBLE_EQ(result->avgFlowTime, 5.0);
    
    // Verify operation is scheduled correctly
    EXPECT_TRUE(op->isScheduled());
    EXPECT_EQ(op->startTime, 0);
    EXPECT_EQ(op->endTime, 5);
}
```

The test confirms that single-operation problems are solved correctly with proper scheduling and metrics.

### Solve Single Job Multiple Machines Test

**Test Name**: `SolveSingleJobMultipleMachines`

This test validates solving a problem with one job that has operations on multiple machines:

```cpp
TEST(SolverEdgeCases, SolveSingleJobMultipleMachines) {
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(1);
    problem->createMachines(3);
    
    auto op1 = std::make_shared<Operation>(0, 0, 2, 0);
    auto op2 = std::make_shared<Operation>(0, 1, 3, 1);
    auto op3 = std::make_shared<Operation>(0, 2, 1, 2);
    
    problem->getJob(0)->addOperation(op1);
    problem->getJob(0)->addOperation(op2);
    problem->getJob(0)->addOperation(op3);
    
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::FIFO);
    auto result = solver->solve(problem);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->makespan, 6); // 2 + 3 + 1
    EXPECT_EQ(result->totalCompletionTime, 6);
    EXPECT_DOUBLE_EQ(result->avgFlowTime, 6.0);
    
    // Verify operations are scheduled in sequence
    EXPECT_TRUE(op1->isScheduled());
    EXPECT_TRUE(op2->isScheduled());
    EXPECT_TRUE(op3->isScheduled());
    
    EXPECT_EQ(op1->startTime, 0);
    EXPECT_EQ(op1->endTime, 2);
    EXPECT_EQ(op2->startTime, 2);
    EXPECT_EQ(op2->endTime, 5);
    EXPECT_EQ(op3->startTime, 5);
    EXPECT_EQ(op3->endTime, 6);
}
```

The test ensures that operations for a single job are scheduled in sequence across multiple machines.

### Solve Multiple Jobs Single Machine Test

**Test Name**: `SolveMultipleJobsSingleMachine`

This test validates solving a problem with multiple jobs on a single machine:

```cpp
TEST(SolverEdgeCases, SolveMultipleJobsSingleMachine) {
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(3);
    problem->createMachines(1);
    
    auto op1 = std::make_shared<Operation>(0, 0, 2, 0);
    auto op2 = std::make_shared<Operation>(1, 0, 3, 1);
    auto op3 = std::make_shared<Operation>(2, 0, 1, 2);
    
    problem->getJob(0)->addOperation(op1);
    problem->getJob(1)->addOperation(op2);
    problem->getJob(2)->addOperation(op3);
    
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::FIFO);
    auto result = solver->solve(problem);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->makespan, 6); // 2 + 3 + 1
    EXPECT_EQ(result->totalCompletionTime, 13); // 2 + 5 + 6
    EXPECT_DOUBLE_EQ(result->avgFlowTime, 13.0/3.0);
    
    // Verify operations are scheduled in sequence
    EXPECT_TRUE(op1->isScheduled());
    EXPECT_TRUE(op2->isScheduled());
    EXPECT_TRUE(op3->isScheduled());
    
    EXPECT_EQ(op1->startTime, 0);
    EXPECT_EQ(op1->endTime, 2);
    EXPECT_EQ(op2->startTime, 2);
    EXPECT_EQ(op2->endTime, 5);
    EXPECT_EQ(op3->startTime, 5);
    EXPECT_EQ(op3->endTime, 6);
}
```

The test confirms that operations from multiple jobs are scheduled sequentially on a single machine.

## Algorithm-Specific Behavior Tests

### SPT Prioritizes Short Operations Test

**Test Name**: `SPTPrioritizesShortOperations`

This test verifies that the SPT algorithm prioritizes shorter operations:

```cpp
TEST(SolverAlgorithmTests, SPTPrioritizesShortOperations) {
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(2);
    problem->createMachines(1);
    
    // Job 0: long operation
    auto op1 = std::make_shared<Operation>(0, 0, 10, 0);
    // Job 1: short operation
    auto op2 = std::make_shared<Operation>(1, 0, 2, 1);
    
    problem->getJob(0)->addOperation(op1);
    problem->getJob(1)->addOperation(op2);
    
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::SPT);
    auto result = solver->solve(problem);
    
    ASSERT_NE(result, nullptr);
    
    // SPT should schedule short operation first
    EXPECT_EQ(op2->startTime, 0);
    EXPECT_EQ(op1->startTime, 2);
}
```

The test confirms that the SPT algorithm schedules shorter operations before longer ones.

### LPT Prioritizes Long Operations Test

**Test Name**: `LPTPrioritizesLongOperations`

This test verifies that the LPT algorithm prioritizes longer operations:

```cpp
TEST(SolverAlgorithmTests, LPTPrioritizesLongOperations) {
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(2);
    problem->createMachines(1);
    
    // Job 0: long operation
    auto op1 = std::make_shared<Operation>(0, 0, 10, 0);
    // Job 1: short operation
    auto op2 = std::make_shared<Operation>(1, 0, 2, 1);
    
    problem->getJob(0)->addOperation(op1);
    problem->getJob(1)->addOperation(op2);
    
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::LPT);
    auto result = solver->solve(problem);
    
    ASSERT_NE(result, nullptr);
    
    // LPT should schedule long operation first
    EXPECT_EQ(op1->startTime, 0);
    EXPECT_EQ(op2->startTime, 10);
}
```

The test confirms that the LPT algorithm schedules longer operations before shorter ones.

## Metrics Calculation Tests

### Calculate Metrics Correctly Test

**Test Name**: `CalculateMetricsCorrectly`

This test verifies that performance metrics are calculated correctly:

```cpp
TEST(SolverMetricsTests, CalculateMetricsCorrectly) {
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(2);
    problem->createMachines(2);
    
    // Job 0: operations on machines 0 and 1 with total processing time 2+3=5
    auto op1 = std::make_shared<Operation>(0, 0, 2, 0);
    auto op2 = std::make_shared<Operation>(0, 1, 3, 1);
    
    // Job 1: operations on machines 2 and 3 with total processing time 1+3=4
    auto op3 = std::make_shared<Operation>(1, 0, 1, 2);
    auto op4 = std::make_shared<Operation>(1, 1, 3, 3);
    
    problem->getJob(0)->addOperation(op1);
    problem->getJob(0)->addOperation(op2);
    problem->getJob(1)->addOperation(op3);
    problem->getJob(1)->addOperation(op4);
    
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::FIFO);
    auto result = solver->solve(problem);
    
    ASSERT_NE(result, nullptr);
    

    EXPECT_GT(result->makespan, 0);
    EXPECT_GT(result->totalCompletionTime, 0);
    EXPECT_GT(result->avgFlowTime, 0);
    
    // Verify that makespan is the maximum job completion time
    int maxJobCompletion = 0;
    for (const auto& job : result->problem.jobs) {
        int jobCompletion = 0;
        for (const auto& operation : job->operations) {
            jobCompletion = std::max(jobCompletion, operation->endTime);
        }
        maxJobCompletion = std::max(maxJobCompletion, jobCompletion);
    }
    EXPECT_EQ(result->makespan, maxJobCompletion);
    
    // Verify that average flow time is calculated as total completion time divided by number of jobs
    double expectedAvgFlowTime = static_cast<double>(result->totalCompletionTime) / result->problem.jobs.size();
    EXPECT_DOUBLE_EQ(result->avgFlowTime, expectedAvgFlowTime);
}
```

The test ensures that all performance metrics (makespan, total completion time, average flow time) are calculated correctly according to their definitions.

## Conclusion

The solver tests provide comprehensive coverage of all scheduling algorithms and their functionality in the JSSP solver. They verify correct implementation of FIFO, SPT, and LPT algorithms, proper metric calculations, error handling for edge cases, and algorithm-specific behaviors. These tests ensure that the solver module reliably generates valid schedules and calculates performance metrics, which is essential for comparing different scheduling approaches and evaluating their effectiveness.