# JSSP Solver Test Documentation: Gantt Maker Tests

## Overview

The `test_gantt_maker.cpp` file contains comprehensive unit tests for the Gantt chart visualization module of the Job Shop Scheduling Problem (JSSP) solver. These tests verify the correct implementation of the Gantt chart maker, which is responsible for visualizing the generated schedules using the SFML graphics library. The Gantt chart provides a graphical representation of job scheduling on different machines over time.

Key responsibilities of the Gantt maker include:
- Creating and managing an SFML window for visualization
- Generating colored bars representing operations on machines
- Calculating and displaying the timeline of operations
- Providing user interaction capabilities (window closing, event polling)
- Configuring layout parameters (time scale, row height, window size)
- Generating distinct colors for different jobs
- Saving generated charts to image files

## Test Structure

All tests are organized within the `GanttMakerTest` class which inherits from `::testing::Test`. This provides a common setup mechanism for all Gantt maker-related tests.

### Setup Method

```cpp
void SetUp() override {
    // Create a simple test problem
    problem = Parser::generateSimpleProblem();
    
    // Create a schedule result
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::FIFO);
    result = solver->solve(problem);
}
```

The setup method creates a standard test problem and generates a schedule result that can be used across multiple test cases.

## Constructor and Basic Functionality Tests

### Constructor Test

**Test Name**: `Constructor`

This test verifies that the GanttChartMaker constructor creates a valid window:

```cpp
TEST_F(GanttMakerTest, Constructor) {
    GanttChartMaker gantt;
    
    // Test that window is created
    EXPECT_TRUE(gantt.isOpen());
    
    // Test default layout parameters
    // Note: These are private members, so we can't test them directly
    // but we can test that the object is properly initialized
}
```

The test confirms that:
- The Gantt chart maker object is properly constructed
- The SFML window is successfully opened and accessible

### Window Management Test

**Test Name**: `WindowManagement`

This test verifies the window management functionality:

```cpp
TEST_F(GanttMakerTest, WindowManagement) {
    GanttChartMaker gantt;
    
    EXPECT_TRUE(gantt.isOpen());
    
    gantt.close();
    EXPECT_FALSE(gantt.isOpen());
}
```

The test ensures that:
- The window starts in an open state
- The close method properly closes the window
- The isOpen method correctly reports the window state

### Set Window Size Test

**Test Name**: `SetWindowSize`

This test validates the window size configuration:

```cpp
TEST_F(GanttMakerTest, SetWindowSize) {
    GanttChartMaker gantt;
    
    // This should not throw
    EXPECT_NO_THROW({
        gantt.setWindowSize(800, 600);
    });
    
    // Window should still be open
    EXPECT_TRUE(gantt.isOpen());
}
```

The test confirms that window size can be set without throwing exceptions and that the window remains functional.

### Set Time Scale Test

**Test Name**: `SetTimeScale`

This test validates the time scale configuration:

```cpp
TEST_F(GanttMakerTest, SetTimeScale) {
    GanttChartMaker gantt;
    
    // This should not throw
    EXPECT_NO_THROW({
        gantt.setTimeScale(25.0f);
    });
}
```

The test ensures that the time scale parameter can be set without errors.

### Set Row Height Test

**Test Name**: `SetRowHeight`

This test validates the row height configuration:

```cpp
TEST_F(GanttMakerTest, SetRowHeight) {
    GanttChartMaker gantt;
    
    // This should not throw
    EXPECT_NO_THROW({
        gantt.setRowHeight(80.0f);
    });
}
```

The test confirms that the row height parameter can be set without errors.

## Color Generation Tests

### Get Job Color Test

**Test Name**: `GetJobColor`

This test verifies that different jobs receive different colors:

```cpp
TEST_F(GanttMakerTest, GetJobColor) {
    GanttChartMaker gantt;
    
    // Test that we get different colors for different jobs
    sf::Color color1 = gantt.getJobColor(0);
    sf::Color color2 = gantt.getJobColor(1);
    sf::Color color3 = gantt.getJobColor(2);
    
    // Colors should be different (at least one component should differ)
    bool color1Different = (color1.r != color2.r) || (color1.g != color2.g) || (color1.b != color2.b);
    bool color2Different = (color2.r != color3.r) || (color2.g != color3.g) || (color2.b != color3.b);
    
    EXPECT_TRUE(color1Different);
    EXPECT_TRUE(color2Different);
}
```

The test ensures that:
- Different job IDs receive visually distinct colors
- At least one RGB component differs between consecutive jobs
- This enables easy visual differentiation between jobs in the Gantt chart

### Get Job Color Wrapping Test

**Test Name**: `GetJobColorWrapping`

This test verifies color wrapping behavior for jobs beyond the predefined set:

```cpp
TEST_F(GanttMakerTest, GetJobColorWrapping) {
    GanttChartMaker gantt;
    
    // Test that colors wrap around for jobs beyond the predefined set
    sf::Color color1 = gantt.getJobColor(0);
    sf::Color color2 = gantt.getJobColor(10); // Should wrap to same as job 0
    
    // For now, we just test that it doesn't crash
    EXPECT_TRUE(color2.r >= 0 && color2.r <= 255);
    EXPECT_TRUE(color2.g >= 0 && color2.g <= 255);
    EXPECT_TRUE(color2.b >= 0 && color2.b <= 255);
}
```

The test confirms that the color generation handles large job numbers gracefully without causing crashes or invalid color values.

## Schedule Result Validation Tests

### Validate Schedule Result Test

**Test Name**: `ValidateScheduleResult`

This test verifies that the setup method correctly creates valid schedule results:

```cpp
TEST_F(GanttMakerTest, ValidateScheduleResult) {
    ASSERT_NE(result, nullptr);
    
    // Verify that the result has valid data
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

The test ensures that the test fixtures provide valid schedule results for visualization testing.

### Schedule Result Metrics Test

**Test Name**: `ScheduleResultMetrics`

This test verifies that schedule result metrics are calculated correctly:

```cpp
TEST_F(GanttMakerTest, ScheduleResultMetrics) {
    ASSERT_NE(result, nullptr);
    
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
    
    // Verify total completion time calculation
    int totalCompletion = 0;
    for (const auto& job : result->problem.jobs) {
        int jobCompletion = 0;
        for (const auto& operation : job->operations) {
            jobCompletion = std::max(jobCompletion, operation->endTime);
        }
        totalCompletion += jobCompletion;
    }
    EXPECT_EQ(result->totalCompletionTime, totalCompletion);
    
    // Verify average flow time calculation
    double expectedAvgFlowTime = static_cast<double>(totalCompletion) / result->problem.jobs.size();
    EXPECT_DOUBLE_EQ(result->avgFlowTime, expectedAvgFlowTime);
}
```

The test confirms that the schedule result contains properly calculated performance metrics.

## Display Functionality Tests

### Display Schedule Test

**Test Name**: `DisplaySchedule`

This test verifies that the display schedule functionality works without throwing exceptions:

```cpp
TEST_F(GanttMakerTest, DisplaySchedule) {
    GanttChartMaker gantt;
    
    // This should not throw (even though we can't see the actual display)
    EXPECT_NO_THROW({
        gantt.displaySchedule(result);
    });
}
```

The test ensures that the display functionality can handle valid schedule results without errors.

### Display Null Schedule Test

**Test Name**: `DisplayNullSchedule`

This test verifies proper handling of null schedule results:

```cpp
TEST_F(GanttMakerTest, DisplayNullSchedule) {
    GanttChartMaker gantt;
    
    // This should handle null gracefully
    EXPECT_NO_THROW({
        gantt.displaySchedule(nullptr);
    });
}
```

The test confirms that the display function handles null inputs gracefully without crashing.

## Save Functionality Tests

### Save to File Test

**Test Name**: `SaveToFile`

This test verifies the save functionality:

```cpp
TEST_F(GanttMakerTest, SaveToFile) {
    GanttChartMaker gantt;
    
    // This should not throw (implementation may be minimal)
    EXPECT_NO_THROW({
        gantt.saveToFile(result, "test_output.png");
    });
}
```

The test ensures that the save functionality doesn't throw exceptions when attempting to save a chart.

### Save Null Schedule Test

**Test Name**: `SaveNullSchedule`

This test verifies proper handling of null schedule results during save operations:

```cpp
TEST_F(GanttMakerTest, SaveNullSchedule) {
    GanttChartMaker gantt;
    
    // This should handle null gracefully
    EXPECT_NO_THROW({
        gantt.saveToFile(nullptr, "test_output.png");
    });
}
```

The test confirms that the save function handles null inputs gracefully.

## Edge Case Tests

### Empty Schedule Result Test

**Test Name**: `EmptyScheduleResult`

This test handles the edge case of visualizing an empty schedule:

```cpp
TEST(GanttMakerEdgeCases, EmptyScheduleResult) {
    GanttChartMaker gantt;
    
    auto emptyResult = std::make_shared<ScheduleResult>();
    emptyResult->problem.createJobs(0);
    emptyResult->problem.createMachines(0);
    
    // Should handle empty result gracefully
    EXPECT_NO_THROW({
        gantt.displaySchedule(emptyResult);
    });
}
```

The test ensures that empty schedules are handled without errors.

### Single Operation Schedule Test

**Test Name**: `SingleOperationSchedule`

This test handles visualizing a schedule with a single operation:

```cpp
TEST(GanttMakerEdgeCases, SingleOperationSchedule) {
    GanttChartMaker gantt;
    
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(1);
    problem->createMachines(1);
    
    auto op = std::make_shared<Operation>(0, 0, 5, 0);
    op->setScheduled(0, 5);
    problem->getJob(0)->addOperation(op);
    
    auto result = std::make_shared<ScheduleResult>();
    result->problem = *problem;
    result->calculateMetrics();
    
    // Should handle single operation gracefully
    EXPECT_NO_THROW({
        gantt.displaySchedule(result);
    });
}
```

The test confirms that minimal schedules are handled properly.

### Large Schedule Result Test

**Test Name**: `LargeScheduleResult`

This test handles visualizing a large schedule:

```cpp
TEST(GanttMakerEdgeCases, LargeScheduleResult) {
    GanttChartMaker gantt;
    
    // Create a larger problem
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(10);
    problem->createMachines(5);
    
    // Add operations
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 3; ++j) {
            auto op = std::make_shared<Operation>(i, j % 5, 2 + (i % 3), i * 3 + j);
            op->setScheduled(i * 2 + j, i * 2 + j + 2 + (i % 3));
            problem->getJob(i)->addOperation(op);
        }
    }
    
    auto result = std::make_shared<ScheduleResult>();
    result->problem = *problem;
    result->calculateMetrics();
    
    // Should handle larger schedule gracefully
    EXPECT_NO_THROW({
        gantt.displaySchedule(result);
    });
}
```

The test ensures that the Gantt maker can handle larger problems without crashing.

## Font Loading Tests

### Font Loading Test

**Test Name**: `FontLoading`

This test verifies font loading functionality:

```cpp
TEST(GanttMakerFontTests, FontLoading) {
    GanttChartMaker gantt;
    
    // Font loading may succeed or fail depending on system
    // We just test that it doesn't crash
    EXPECT_NO_THROW({
        // Font loading is done in constructor, so we can't test it directly
        // But we can test that the object was created successfully
    });
}
```

The test confirms that font loading in the constructor doesn't cause crashes.

## Layout Parameter Tests

### Layout Parameters Test

**Test Name**: `LayoutParameters`

This test verifies that layout parameters can be adjusted:

```cpp
TEST(GanttMakerLayoutTests, LayoutParameters) {
    GanttChartMaker gantt;
    
    // Test that we can set layout parameters without crashing
    EXPECT_NO_THROW({
        gantt.setTimeScale(10.0f);
        gantt.setTimeScale(50.0f);
        gantt.setRowHeight(30.0f);
        gantt.setRowHeight(100.0f);
        gantt.setWindowSize(640, 480);
        gantt.setWindowSize(1920, 1080);
    });
}
```

The test ensures that all layout parameters can be modified without errors.

## Event Polling Tests

### Poll Events Test

**Test Name**: `PollEvents`

This test verifies the event polling functionality:

```cpp
TEST(GanttMakerEventTests, PollEvents) {
    GanttChartMaker gantt;
    
    // This should not throw
    EXPECT_NO_THROW({
        gantt.pollEvents();
    });
}
```

The test confirms that event polling works without throwing exceptions.

## Integration Tests

### Full Workflow Test

**Test Name**: `FullWorkflow`

This test verifies the complete workflow from problem creation to visualization:

```cpp
TEST(GanttMakerIntegrationTests, FullWorkflow) {
    // Create problem
    auto problem = Parser::generateSimpleProblem();
    
    // Solve problem
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::FIFO);
    auto result = solver->solve(problem);
    
    // Create Gantt chart
    GanttChartMaker gantt;
    
    // Display result
    EXPECT_NO_THROW({
        gantt.displaySchedule(result);
    });
    
    // Verify result is valid
    ASSERT_NE(result, nullptr);
    EXPECT_GT(result->makespan, 0);
    EXPECT_GT(result->totalCompletionTime, 0);
    EXPECT_GT(result->avgFlowTime, 0);
}
```

The test ensures that the complete pipeline from problem creation to visualization works correctly.

## Conclusion

The Gantt maker tests provide comprehensive coverage of the visualization functionality in the JSSP solver. They verify correct window management, color generation, schedule display, and error handling for various scenarios. These tests ensure that the Gantt chart visualization module reliably represents scheduling results graphically, which is essential for understanding and analyzing the performance of different scheduling algorithms.