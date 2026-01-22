# JSSP Solver Test Documentation: Integration Tests

## Overview

The `test_integration.cpp` file contains comprehensive integration tests for the Job Shop Scheduling Problem (JSSP) solver. These tests verify the correct interaction between different modules of the system, ensuring that the parser, solver, and Gantt chart maker work together seamlessly. Unlike unit tests that focus on individual components, integration tests validate the complete workflow from problem specification to solution visualization.

Key responsibilities of the integration tests include:
- Validating end-to-end workflows across multiple modules
- Testing file I/O pipelines with different problem sizes
- Verifying error handling across module boundaries
- Ensuring memory management works correctly with complex interactions
- Comparing algorithm performance in realistic scenarios
- Testing real-world scheduling scenarios

## Test Structure

All tests are organized within the `IntegrationTest` class which inherits from `::testing::Test`. This provides a common setup and teardown mechanism for all integration-related tests. The fixture includes helper methods for creating and cleaning up temporary test files.

### Setup Method

```cpp
void SetUp() override {
    // Create test data files
    createTestFiles();
}
```

The setup method calls `createTestFiles()` to prepare various test data files with different characteristics.

### Teardown Method

```cpp
void TearDown() override {
    // Clean up test files
    cleanupTestFiles();
}
```

The teardown method ensures that all temporary test files are removed after each test run.

### Helper Methods

The test fixture includes helper methods for managing test data:

```cpp
void createTestFiles() {
    // Creates test data files with various characteristics
    // Simple test file (3x3)
    std::ofstream simple("integration_simple.jssp");
    simple << "3 3\n";
    simple << "0 0 2\n";
    simple << "0 1 3\n";
    simple << "0 2 1\n";
    simple << "1 1 1\n";
    simple << "1 2 2\n";
    simple << "1 0 3\n";
    simple << "2 2 3\n";
    simple << "2 0 1\n";
    simple << "2 1 2\n";
    simple.close();
    
    // Medium test file (2x2)
    std::ofstream medium("integration_medium.jssp");
    medium << "2 2\n";
    medium << "0 0 5\n";
    medium << "0 1 3\n";
    medium << "1 1 2\n";
    medium << "1 0 4\n";
    medium.close();
    
    // Complex test file (4x3)
    std::ofstream complex("integration_complex.jssp");
    complex << "4 3\n";
    complex << "0 0 3\n";
    complex << "0 1 2\n";
    complex << "0 2 4\n";
    complex << "1 1 1\n";
    complex << "1 2 3\n";
    complex << "1 0 2\n";
    complex << "2 2 2\n";
    complex << "2 0 1\n";
    complex << "2 1 3\n";
    complex << "3 0 4\n";
    complex << "3 2 1\n";
    complex << "3 1 2\n";
    complex.close();
}

void cleanupTestFiles() {
    remove("integration_simple.jssp");
    remove("integration_medium.jssp");
    remove("integration_complex.jssp");
    remove("integration_output.jssp");
}
```

## End-to-End Workflow Tests

### Complete Workflow Simple Test

**Test Name**: `CompleteWorkflowSimple`

This test verifies the complete workflow with a simple problem:

```cpp
TEST_F(IntegrationTest, CompleteWorkflowSimple) {
    // 1. Parse file
    auto problem = Parser::parseFile("integration_simple.jssp");
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 3);
    EXPECT_EQ(problem->numMachines, 3);
    EXPECT_EQ(problem->getTotalOperations(), 9);
    
    // 2. Solve with different algorithms
    auto fifoSolver = Solver::createFIFOSolver();
    auto sptSolver = Solver::createSPTSolver();
    auto lptSolver = Solver::createLPTSolver();
    
    auto fifoResult = fifoSolver->solve(problem);
    auto sptResult = sptSolver->solve(problem);
    auto lptResult = lptSolver->solve(problem);
    
    // 3. Verify results
    ASSERT_NE(fifoResult, nullptr);
    ASSERT_NE(sptResult, nullptr);
    ASSERT_NE(lptResult, nullptr);
    
    EXPECT_GT(fifoResult->makespan, 0);
    EXPECT_GT(sptResult->makespan, 0);
    EXPECT_GT(lptResult->makespan, 0);
    
    // 4. Verify all operations are scheduled
    for (const auto& result : {fifoResult, sptResult, lptResult}) {
        for (const auto& job : result->problem.jobs) {
            for (const auto& operation : job->operations) {
                EXPECT_TRUE(operation->isScheduled());
                EXPECT_GE(operation->startTime, 0);
                EXPECT_GT(operation->endTime, operation->startTime);
            }
        }
    }
    
    // 5. Test Gantt chart creation (without display)
    GanttChartMaker gantt;
    EXPECT_NO_THROW({
        gantt.displaySchedule(fifoResult);
    });
    EXPECT_NO_THROW({
        gantt.displaySchedule(sptResult);
    });
    EXPECT_NO_THROW({
        gantt.displaySchedule(lptResult);
    });
}
```

The test validates the complete pipeline:
- File parsing produces the expected problem structure
- Multiple algorithms can solve the same problem
- All solutions have valid metrics greater than zero
- All operations in all solutions are properly scheduled
- Gantt chart visualization works with all algorithm outputs

### Complete Workflow Medium Test

**Test Name**: `CompleteWorkflowMedium`

This test verifies the complete workflow with a medium-sized problem:

```cpp
TEST_F(IntegrationTest, CompleteWorkflowMedium) {
    // 1. Parse file
    auto problem = Parser::parseFile("integration_medium.jssp");
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 2);
    EXPECT_EQ(problem->numMachines, 2);
    EXPECT_EQ(problem->getTotalOperations(), 4);
    
    // 2. Solve with FIFO
    auto solver = Solver::createFIFOSolver();
    auto result = solver->solve(problem);
    
    // 3. Verify result
    ASSERT_NE(result, nullptr);
    EXPECT_GT(result->makespan, 0);
    EXPECT_GT(result->totalCompletionTime, 0);
    EXPECT_GT(result->avgFlowTime, 0);
    
    // 4. Test Gantt chart
    GanttChartMaker gantt;
    EXPECT_NO_THROW({
        gantt.displaySchedule(result);
    });
}
```

The test ensures that medium-sized problems work correctly through the entire pipeline.

### Complete Workflow Complex Test

**Test Name**: `CompleteWorkflowComplex`

This test verifies the complete workflow with a more complex problem:

```cpp
TEST_F(IntegrationTest, CompleteWorkflowComplex) {
    // 1. Parse file
    auto problem = Parser::parseFile("integration_complex.jssp");
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 4);
    EXPECT_EQ(problem->numMachines, 3);
    EXPECT_EQ(problem->getTotalOperations(), 12);
    
    // 2. Solve with different algorithms
    auto fifoResult = Solver::createFIFOSolver()->solve(problem);
    auto sptResult = Solver::createSPTSolver()->solve(problem);
    auto lptResult = Solver::createLPTSolver()->solve(problem);
    
    // 3. Verify results
    ASSERT_NE(fifoResult, nullptr);
    ASSERT_NE(sptResult, nullptr);
    ASSERT_NE(lptResult, nullptr);
    
    // 4. Compare algorithms
    EXPECT_NO_THROW({
        Solver::compareSolutions(fifoResult, sptResult, "FIFO", "SPT");
        Solver::compareSolutions(sptResult, lptResult, "SPT", "LPT");
        Solver::compareSolutions(fifoResult, lptResult, "FIFO", "LPT");
    });
    
    // 5. Test Gantt chart
    GanttChartMaker gantt;
    EXPECT_NO_THROW({
        gantt.displaySchedule(fifoResult);
        gantt.displaySchedule(sptResult);
        gantt.displaySchedule(lptResult);
    });
}
```

The test confirms that complex problems work correctly and algorithm comparison functions operate properly.

## Algorithm Comparison Tests

### Algorithm Comparison Test

**Test Name**: `AlgorithmComparison`

This test validates the comparison of different algorithms on the same problem:

```cpp
TEST_F(IntegrationTest, AlgorithmComparison) {
    auto problem = Parser::parseFile("integration_simple.jssp");
    
    auto fifoResult = Solver::createFIFOSolver()->solve(problem);
    auto sptResult = Solver::createSPTSolver()->solve(problem);
    auto lptResult = Solver::createLPTSolver()->solve(problem);
    
    // All should produce valid results
    ASSERT_NE(fifoResult, nullptr);
    ASSERT_NE(sptResult, nullptr);
    ASSERT_NE(lptResult, nullptr);
    
    // Results should be different (in most cases)
    // Note: They might be the same for very simple problems
    EXPECT_GT(fifoResult->makespan, 0);
    EXPECT_GT(sptResult->makespan, 0);
    EXPECT_GT(lptResult->makespan, 0);
    
    // Test comparison function
    EXPECT_NO_THROW({
        Solver::compareSolutions(fifoResult, sptResult);
    });
}
```

The test ensures that:
- All algorithms can solve the same problem
- All solutions have valid positive metrics
- Solution comparison functions work correctly

## File I/O Integration Tests

### File I/O Pipeline Test

**Test Name**: `FileIOPipeline`

This test validates the complete file I/O pipeline:

```cpp
TEST_F(IntegrationTest, FileIOPipeline) {
    // 1. Generate problem
    auto problem = Parser::generateSimpleProblem();
    ASSERT_NE(problem, nullptr);
    
    // 2. Save to file
    EXPECT_NO_THROW({
        Parser::saveToFile(problem, "integration_output.jssp");
    });
    
    // 3. Load from file
    auto loadedProblem = Parser::parseFile("integration_output.jssp");
    ASSERT_NE(loadedProblem, nullptr);
    
    // 4. Compare problems
    EXPECT_EQ(problem->numJobs, loadedProblem->numJobs);
    EXPECT_EQ(problem->numMachines, loadedProblem->numMachines);
    EXPECT_EQ(problem->getTotalOperations(), loadedProblem->getTotalOperations());
    
    // 5. Solve both and compare results
    auto originalResult = Solver::createFIFOSolver()->solve(problem);
    auto loadedResult = Solver::createFIFOSolver()->solve(loadedProblem);
    
    ASSERT_NE(originalResult, nullptr);
    ASSERT_NE(loadedResult, nullptr);
    
    // Results should be identical (same problem, same algorithm)
    EXPECT_EQ(originalResult->makespan, loadedResult->makespan);
    EXPECT_EQ(originalResult->totalCompletionTime, loadedResult->totalCompletionTime);
    EXPECT_DOUBLE_EQ(originalResult->avgFlowTime, loadedResult->avgFlowTime);
}
```

The test verifies that:
- Problems can be saved to files
- Problems can be loaded from files
- Loaded problems have the same structure as original problems
- Solutions to original and loaded problems are identical

## Error Handling Integration Tests

### Error Handling Pipeline Test

**Test Name**: `ErrorHandlingPipeline`

This test validates error handling across module boundaries:

```cpp
TEST_F(IntegrationTest, ErrorHandlingPipeline) {
    // Test with invalid file
    EXPECT_THROW({
        try {
            auto problem = Parser::parseFile("nonexistent.jssp");
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ("Could not open file: nonexistent.jssp", e.what());
            throw;
        }
    }, std::runtime_error);
    
    // Test with empty problem
    auto emptyProblem = std::make_shared<ProblemInstance>();
    auto solver = Solver::createFIFOSolver();
    auto result = solver->solve(emptyProblem);
    
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->makespan, 0);
    EXPECT_EQ(result->totalCompletionTime, 0);
    EXPECT_DOUBLE_EQ(result->avgFlowTime, 0.0);
}
```

The test ensures that:
- File parsing errors are properly propagated
- Error messages are informative and specific
- Empty problems are handled gracefully across modules

## Performance Integration Tests

### Performance Test

**Test Name**: `PerformanceTest`

This test evaluates the performance of the complete workflow with larger problems:

```cpp
TEST_F(IntegrationTest, PerformanceTest) {
    // Create a larger problem
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(20);
    problem->createMachines(10);
    
    // Add operations
    for (int i = 0; i < 20; ++i) {
        for (int j = 0; j < 5; ++j) {
            auto op = std::make_shared<Operation>(i, (i + j) % 10, 1 + (rand() % 10), i * 5 + j);
            problem->getJob(i)->addOperation(op);
        }
    }
    
    // Test solving performance
    auto start = std::chrono::high_resolution_clock::now();
    
    auto solver = Solver::createFIFOSolver();
    auto result = solver->solve(problem);
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Should complete in reasonable time (less than 1 second for this size)
    EXPECT_LT(duration.count(), 1000);
    
    // Verify result
    ASSERT_NE(result, nullptr);
    EXPECT_GT(result->makespan, 0);
    EXPECT_GT(result->totalCompletionTime, 0);
    EXPECT_GT(result->avgFlowTime, 0);
    
    // Verify all operations are scheduled
    for (const auto& job : result->problem.jobs) {
        for (const auto& operation : job->operations) {
            EXPECT_TRUE(operation->isScheduled());
        }
    }
}
```

The test validates that:
- Larger problems can be processed within reasonable time limits
- The complete workflow maintains performance with increased problem size
- All operations in large problems are properly scheduled

## Memory Management Integration Tests

### Memory Management Test

**Test Name**: `MemoryManagement`

This test verifies proper memory management across multiple operations:

```cpp
TEST_F(IntegrationTest, MemoryManagement) {
    // Test that we can create and destroy many objects without issues
    for (int i = 0; i < 100; ++i) {
        auto problem = Parser::generateSimpleProblem();
        auto solver = Solver::createFIFOSolver();
        auto result = solver->solve(problem);
        
        // Verify result is valid
        ASSERT_NE(result, nullptr);
        EXPECT_GT(result->makespan, 0);
        
        // Objects will be automatically cleaned up by smart pointers
    }
}
```

The test ensures that:
- Multiple iterations of the complete workflow don't cause memory leaks
- Smart pointer management works correctly across all modules
- Resources are properly released after each iteration

## Real-World Scenario Tests

### Real-World Scenario Test

**Test Name**: `RealWorldScenario`

This test validates the system with a realistic manufacturing scenario:

```cpp
TEST_F(IntegrationTest, RealWorldScenario) {
    // Simulate a real-world scheduling scenario
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(5);
    problem->createMachines(3);
    
    // Job 0: Manufacturing process
    problem->getJob(0)->addOperation(std::make_shared<Operation>(0, 0, 10, 0)); // Cutting
    problem->getJob(0)->addOperation(std::make_shared<Operation>(0, 1, 15, 1)); // Shaping
    problem->getJob(0)->addOperation(std::make_shared<Operation>(0, 2, 5, 2));  // Finishing
    
    // Job 1: Assembly process
    problem->getJob(1)->addOperation(std::make_shared<Operation>(1, 1, 8, 3));  // Assembly
    problem->getJob(1)->addOperation(std::make_shared<Operation>(1, 2, 12, 4)); // Testing
    
    // Job 2: Packaging process
    problem->getJob(2)->addOperation(std::make_shared<Operation>(2, 2, 6, 5));  // Packaging
    problem->getJob(2)->addOperation(std::make_shared<Operation>(2, 0, 4, 6));  // Labeling
    
    // Job 3: Quality control
    problem->getJob(3)->addOperation(std::make_shared<Operation>(3, 1, 7, 7));  // Inspection
    problem->getJob(3)->addOperation(std::make_shared<Operation>(3, 2, 3, 8));  // Certification
    
    // Job 4: Shipping preparation
    problem->getJob(4)->addOperation(std::make_shared<Operation>(4, 0, 5, 9));  // Palletizing
    problem->getJob(4)->addOperation(std::make_shared<Operation>(4, 2, 8, 10)); // Loading
    
    // Solve with different algorithms
    auto fifoResult = Solver::createFIFOSolver()->solve(problem);
    auto sptResult = Solver::createSPTSolver()->solve(problem);
    auto lptResult = Solver::createLPTSolver()->solve(problem);
    
    // Verify all results
    ASSERT_NE(fifoResult, nullptr);
    ASSERT_NE(sptResult, nullptr);
    ASSERT_NE(lptResult, nullptr);
    
    // Test Gantt chart visualization
    GanttChartMaker gantt;
    EXPECT_NO_THROW({
        gantt.displaySchedule(fifoResult);
        gantt.displaySchedule(sptResult);
        gantt.displaySchedule(lptResult);
    });
    
    // Compare algorithms for this real-world scenario
    EXPECT_NO_THROW({
        Solver::compareSolutions(fifoResult, sptResult, "FIFO", "SPT");
        Solver::compareSolutions(sptResult, lptResult, "SPT", "LPT");
    });
}
```

The test validates that:
- Realistic problem structures work correctly
- Multi-stage processes with different operations per job are handled properly
- All modules work together with complex, real-world-like scenarios
- Algorithm comparison works with realistic problems

## Conclusion

The integration tests provide comprehensive coverage of the interactions between different modules in the JSSP solver. They verify that the complete workflow from problem specification to solution visualization works correctly, including error handling, performance considerations, and memory management. These tests ensure that the system functions reliably as a whole, not just as individual components, which is crucial for the practical usability of the solver in real-world applications.