#include <gtest/gtest.h>
#include <memory>
#include <vector>
#include "models.hpp"

class ModelsTest : public ::testing::Test {
protected:
    /**
     * SetUp method for test fixture.
     */
    void SetUp() override {
        // Create test data
        operation = std::make_shared<Operation>(0, 1, 5, 0);
        job = std::make_shared<Job>(0);
        machine = std::make_shared<Machine>(1);
        problem = std::make_shared<ProblemInstance>();
    }

    std::shared_ptr<Operation> operation;
    std::shared_ptr<Job> job;
    std::shared_ptr<Machine> machine;
    std::shared_ptr<ProblemInstance> problem;
};

// Operation tests
TEST_F(ModelsTest, OperationCreation) {
    EXPECT_EQ(operation->jobId, 0);
    EXPECT_EQ(operation->machineId, 1);
    EXPECT_EQ(operation->processingTime, 5);
    EXPECT_EQ(operation->operationId, 0);
    EXPECT_EQ(operation->getDuration(), 5);
    EXPECT_FALSE(operation->isScheduled());
}

TEST_F(ModelsTest, OperationScheduling) {
    operation->setScheduled(10, 15);
    EXPECT_TRUE(operation->isScheduled());
    EXPECT_EQ(operation->startTime, 10);
    EXPECT_EQ(operation->endTime, 15);
}

// Job tests
TEST_F(ModelsTest, JobCreation) {
    EXPECT_EQ(job->jobId, 0);
    EXPECT_EQ(job->getOperationCount(), 0);
    EXPECT_EQ(job->getOperation(0), nullptr);
}

TEST_F(ModelsTest, JobOperations) {
    auto op1 = std::make_shared<Operation>(0, 0, 2, 0);
    auto op2 = std::make_shared<Operation>(0, 1, 3, 1);
    
    job->addOperation(op1);
    job->addOperation(op2);
    
    EXPECT_EQ(job->getOperationCount(), 2);
    EXPECT_EQ(job->getOperation(0), op1);
    EXPECT_EQ(job->getOperation(1), op2);
    EXPECT_EQ(job->getOperation(2), nullptr);
}

// Machine tests
TEST_F(ModelsTest, MachineCreation) {
    EXPECT_EQ(machine->machineId, 1);
    EXPECT_TRUE(machine->isAvailable());
    EXPECT_EQ(machine->availableTime, 0);
    EXPECT_EQ(machine->scheduledOperations.size(), 0);
}

TEST_F(ModelsTest, MachineScheduling) {
    auto op1 = std::make_shared<Operation>(0, 1, 5, 0);
    machine->scheduleOperation(op1, 10);
    
    EXPECT_FALSE(machine->isAvailable());
    EXPECT_EQ(machine->availableTime, 15);
    EXPECT_EQ(machine->scheduledOperations.size(), 1);
    EXPECT_EQ(machine->scheduledOperations[0], op1);
    EXPECT_EQ(op1->startTime, 10);
    EXPECT_EQ(op1->endTime, 15);
}

TEST_F(ModelsTest, MachineReset) {
    auto op1 = std::make_shared<Operation>(0, 1, 5, 0);
    machine->scheduleOperation(op1, 10);
    
    machine->reset();
    EXPECT_TRUE(machine->isAvailable());
    EXPECT_EQ(machine->availableTime, 0);
    EXPECT_EQ(machine->scheduledOperations.size(), 0);
}

// ProblemInstance tests
TEST_F(ModelsTest, ProblemCreation) {
    EXPECT_EQ(problem->numJobs, 0);
    EXPECT_EQ(problem->numMachines, 0);
    EXPECT_EQ(problem->jobs.size(), 0);
    EXPECT_EQ(problem->machines.size(), 0);
}

TEST_F(ModelsTest, ProblemSetup) {
    problem->createJobs(3);
    problem->createMachines(2);
    
    EXPECT_EQ(problem->numJobs, 3);
    EXPECT_EQ(problem->numMachines, 2);
    EXPECT_EQ(problem->jobs.size(), 3);
    EXPECT_EQ(problem->machines.size(), 2);
    
    for (int i = 0; i < 3; ++i) {
        EXPECT_EQ(problem->getJob(i)->jobId, i);
    }
    
    for (int i = 0; i < 2; ++i) {
        EXPECT_EQ(problem->getMachine(i)->machineId, i);
    }
}

TEST_F(ModelsTest, ProblemInvalidAccess) {
    problem->createJobs(2);
    problem->createMachines(2);
    
    EXPECT_EQ(problem->getJob(-1), nullptr);
    EXPECT_EQ(problem->getJob(2), nullptr);
    EXPECT_EQ(problem->getMachine(-1), nullptr);
    EXPECT_EQ(problem->getMachine(2), nullptr);
}

TEST_F(ModelsTest, ProblemClear) {
    problem->createJobs(2);
    problem->createMachines(2);
    
    auto op1 = std::make_shared<Operation>(0, 0, 2, 0);
    auto op2 = std::make_shared<Operation>(1, 1, 3, 1);
    problem->getJob(0)->addOperation(op1);
    problem->getJob(1)->addOperation(op2);
    
    problem->clear();
    
    for (const auto& job : problem->jobs) {
        EXPECT_EQ(job->operations.size(), 0);
    }
    
    for (const auto& machine : problem->machines) {
        EXPECT_EQ(machine->scheduledOperations.size(), 0);
        EXPECT_TRUE(machine->isAvailable());
    }
}

TEST_F(ModelsTest, ProblemTotalOperations) {
    problem->createJobs(2);
    problem->createMachines(2);
    
    auto op1 = std::make_shared<Operation>(0, 0, 2, 0);
    auto op2 = std::make_shared<Operation>(0, 1, 3, 1);
    auto op3 = std::make_shared<Operation>(1, 0, 1, 2);
    
    problem->getJob(0)->addOperation(op1);
    problem->getJob(0)->addOperation(op2);
    problem->getJob(1)->addOperation(op3);
    
    EXPECT_EQ(problem->getTotalOperations(), 3);
}

// ScheduleResult tests
TEST_F(ModelsTest, ScheduleResultCreation) {
    ScheduleResult result;
    EXPECT_EQ(result.makespan, 0);
    EXPECT_EQ(result.totalCompletionTime, 0);
    EXPECT_DOUBLE_EQ(result.avgFlowTime, 0.0);
}

TEST_F(ModelsTest, ScheduleResultMetrics) {
    auto result = std::make_shared<ScheduleResult>();
    result->problem.createJobs(2);
    result->problem.createMachines(2);
    
    // Job 0: operations at times 0-2, 2-5
    auto op1 = std::make_shared<Operation>(0, 0, 2, 0);
    auto op2 = std::make_shared<Operation>(0, 1, 3, 1);
    op1->setScheduled(0, 2);
    op2->setScheduled(2, 5);
    result->problem.getJob(0)->addOperation(op1);
    result->problem.getJob(0)->addOperation(op2);
    
    // Job 1: operations at times 0-1, 1-4
    auto op3 = std::make_shared<Operation>(1, 0, 1, 2);
    auto op4 = std::make_shared<Operation>(1, 1, 3, 3);
    op3->setScheduled(0, 1);
    op4->setScheduled(1, 4);
    result->problem.getJob(1)->addOperation(op3);
    result->problem.getJob(1)->addOperation(op4);
    
    result->calculateMetrics();
    
    EXPECT_EQ(result->makespan, 5); // max completion time
    EXPECT_EQ(result->totalCompletionTime, 9); // 5 + 4
    EXPECT_DOUBLE_EQ(result->avgFlowTime, 4.5); // 9 / 2
}

// Edge case tests
TEST(ModelsEdgeCases, OperationEdgeCases) {
    // Test with zero processing time
    auto op = std::make_shared<Operation>(0, 0, 0, 0);
    EXPECT_EQ(op->getDuration(), 0);
    
    // Test with negative times (should still work, though not typical)
    op->setScheduled(-5, -2);
    EXPECT_TRUE(op->isScheduled());
    EXPECT_EQ(op->startTime, -5);
    EXPECT_EQ(op->endTime, -2);
}

TEST(ModelsEdgeCases, JobEdgeCases) {
    auto job = std::make_shared<Job>(0);
    
    // Test empty job
    EXPECT_EQ(job->getOperationCount(), 0);
    EXPECT_EQ(job->getOperation(-1), nullptr);
    EXPECT_EQ(job->getOperation(0), nullptr);
    
    // Test adding null operation
    job->addOperation(nullptr);
    EXPECT_EQ(job->getOperationCount(), 0);
    
    // Test adding valid operation
    auto op = std::make_shared<Operation>(0, 0, 5, 0);
    job->addOperation(op);
    EXPECT_EQ(job->getOperationCount(), 1);
    EXPECT_EQ(job->getOperation(0), op);
    
    // Test adding another valid operation
    auto op2 = std::make_shared<Operation>(0, 1, 3, 1);
    job->addOperation(op2);
    EXPECT_EQ(job->getOperationCount(), 2);
    EXPECT_EQ(job->getOperation(1), op2);
}

TEST(ModelsEdgeCases, MachineEdgeCases) {
    auto machine = std::make_shared<Machine>(0);
    
    // Test scheduling null operation
    machine->scheduleOperation(nullptr, 0);
    EXPECT_EQ(machine->scheduledOperations.size(), 0);
    
    // Test scheduling with negative start time
    auto op = std::make_shared<Operation>(0, 0, 5, 0);
    machine->scheduleOperation(op, -10);
    EXPECT_EQ(op->startTime, -10);
    EXPECT_EQ(op->endTime, -5);
    EXPECT_EQ(machine->availableTime, -5);
}
