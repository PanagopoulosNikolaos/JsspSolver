#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "solver.hpp"
#include "parser.hpp"
#include "models.hpp"


class SolverTest : public ::testing::Test {
protected:
    /**
     * SetUp method for test fixture.
     */
    void SetUp() override {
        // Create a simple test problem
        problem = Parser::generateSimpleProblem();
    }
    
    std::shared_ptr<ProblemInstance> problem;
};

// FIFO algorithm tests
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

// Algorithm factory tests
TEST_F(SolverTest, CreateFIFOSolver) {
    auto solver = Solver::createFIFOSolver();
    ASSERT_NE(solver, nullptr);
    EXPECT_EQ(solver->getAlgorithm(), SchedulingAlgorithm::FIFO);
}

TEST_F(SolverTest, CreateSPTSolver) {
    auto solver = Solver::createSPTSolver();
    ASSERT_NE(solver, nullptr);
    EXPECT_EQ(solver->getAlgorithm(), SchedulingAlgorithm::SPT);
}

TEST_F(SolverTest, CreateLPTSolver) {
    auto solver = Solver::createLPTSolver();
    ASSERT_NE(solver, nullptr);
    EXPECT_EQ(solver->getAlgorithm(), SchedulingAlgorithm::LPT);
}

// Algorithm name tests
TEST_F(SolverTest, GetAlgorithmName) {
    EXPECT_EQ(Solver::getAlgorithmName(SchedulingAlgorithm::FIFO), "FIFO (First-In-First-Out)");
    EXPECT_EQ(Solver::getAlgorithmName(SchedulingAlgorithm::SPT), "SPT (Shortest Processing Time)");
    EXPECT_EQ(Solver::getAlgorithmName(SchedulingAlgorithm::LPT), "LPT (Longest Processing Time)");
}

TEST_F(SolverTest, GetCurrentAlgorithmName) {
    auto solver = std::make_shared<Solver>(SchedulingAlgorithm::SPT);
    EXPECT_EQ(solver->getCurrentAlgorithmName(), "SPT (Shortest Processing Time)");
}

// Algorithm switching tests
TEST_F(SolverTest, SetAlgorithm) {
    auto solver = std::make_shared<Solver>();
    solver->setAlgorithm(SchedulingAlgorithm::LPT);
    EXPECT_EQ(solver->getAlgorithm(), SchedulingAlgorithm::LPT);
}

// Comparison tests
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

// Edge case tests
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

// Algorithm-specific behavior tests
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

// Metrics calculation tests
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
