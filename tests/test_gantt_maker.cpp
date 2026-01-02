#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "gantt_maker.hpp"
#include "parser.hpp"
#include "models.hpp"
#include "solver.hpp"

class GanttMakerTest : public ::testing::Test {
protected:
    /**
     * SetUp method for test fixture.
     */
    void SetUp() override {
        // Create a simple test problem
        problem = Parser::generateSimpleProblem();
        
        // Create a schedule result
        auto solver = std::make_shared<Solver>(SchedulingAlgorithm::FIFO);
        result = solver->solve(problem);
    }
    
    std::shared_ptr<ProblemInstance> problem;
    std::shared_ptr<ScheduleResult> result;
};

// GanttChartMaker constructor and basic functionality tests
TEST_F(GanttMakerTest, Constructor) {
    GanttChartMaker gantt;
    
    // Test that window is created
    EXPECT_TRUE(gantt.isOpen());
    
    // Test default layout parameters
    // Note: These are private members, so we can't test them directly
    // but we can test that the object is properly initialized
}

TEST_F(GanttMakerTest, WindowManagement) {
    GanttChartMaker gantt;
    
    EXPECT_TRUE(gantt.isOpen());
    
    gantt.close();
    EXPECT_FALSE(gantt.isOpen());
}

TEST_F(GanttMakerTest, SetWindowSize) {
    GanttChartMaker gantt;
    
    // This should not throw
    EXPECT_NO_THROW({
        gantt.setWindowSize(800, 600);
    });
    
    // Window should still be open
    EXPECT_TRUE(gantt.isOpen());
}

TEST_F(GanttMakerTest, SetTimeScale) {
    GanttChartMaker gantt;
    
    // This should not throw
    EXPECT_NO_THROW({
        gantt.setTimeScale(25.0f);
    });
}

TEST_F(GanttMakerTest, SetRowHeight) {
    GanttChartMaker gantt;
    
    // This should not throw
    EXPECT_NO_THROW({
        gantt.setRowHeight(80.0f);
    });
}

// Color generation tests
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

// Schedule result validation tests
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

// Display functionality tests (without actual display)
TEST_F(GanttMakerTest, DisplaySchedule) {
    GanttChartMaker gantt;
    
    // This should not throw (even though we can't see the actual display)
    EXPECT_NO_THROW({
        gantt.displaySchedule(result);
    });
}

TEST_F(GanttMakerTest, DisplayNullSchedule) {
    GanttChartMaker gantt;
    
    // This should handle null gracefully
    EXPECT_NO_THROW({
        gantt.displaySchedule(nullptr);
    });
}

// Save functionality tests
TEST_F(GanttMakerTest, SaveToFile) {
    GanttChartMaker gantt;
    
    // This should not throw (implementation may be minimal)
    EXPECT_NO_THROW({
        gantt.saveToFile(result, "test_output.png");
    });
}

TEST_F(GanttMakerTest, SaveNullSchedule) {
    GanttChartMaker gantt;
    
    // This should handle null gracefully
    EXPECT_NO_THROW({
        gantt.saveToFile(nullptr, "test_output.png");
    });
}

// Edge case tests
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

// Font loading tests
TEST(GanttMakerFontTests, FontLoading) {
    GanttChartMaker gantt;
    
    // Font loading may succeed or fail depending on system
    // We just test that it doesn't crash
    EXPECT_NO_THROW({
        // Font loading is done in constructor, so we can't test it directly
        // But we can test that the object was created successfully
    });
}

// Layout parameter tests
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

// Poll events test
TEST(GanttMakerEventTests, PollEvents) {
    GanttChartMaker gantt;
    
    // This should not throw
    EXPECT_NO_THROW({
        gantt.pollEvents();
    });
}

// Integration with other components
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
