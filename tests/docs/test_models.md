# JSSP Solver Test Documentation: Models Tests

## Overview

The `test_models.cpp` file contains comprehensive unit tests for the core data models used in the Job Shop Scheduling Problem (JSSP) solver. These tests verify the correct implementation of fundamental classes including Operation, Job, Machine, ProblemInstance, and ScheduleResult. The tests ensure that each model component behaves as expected under various conditions and edge cases.

The models form the foundation of the entire JSSP solver system, representing the essential entities in job shop scheduling:
- **Operation**: Represents a single task that needs to be performed on a specific machine
- **Job**: A sequence of operations that must be processed in order
- **Machine**: A resource that can process operations sequentially
- **ProblemInstance**: The complete problem definition containing jobs and machines
- **ScheduleResult**: The result of a scheduling algorithm containing metrics

## Test Structure

All tests are organized within the `ModelsTest` class which inherits from `::testing::Test`. This provides a common setup and teardown mechanism for all model-related tests. The fixture creates shared instances of the model classes that can be used across multiple test cases.

### Setup Method

```cpp
void SetUp() override {
    // Create test data
    operation = std::make_shared<Operation>(0, 1, 5, 0);
    job = std::make_shared<Job>(0);
    machine = std::make_shared<Machine>(1);
    problem = std::make_shared<ProblemInstance>();
}
```

The setup method initializes shared model instances that are accessible to all test methods through member variables.

## Operation Tests

### Operation Creation Test

**Test Name**: `OperationCreation`

This test verifies that an Operation object is correctly initialized with the provided parameters. It checks that all properties are set as expected:

```cpp
TEST_F(ModelsTest, OperationCreation) {
    EXPECT_EQ(operation->jobId, 0);
    EXPECT_EQ(operation->machineId, 1);
    EXPECT_EQ(operation->processingTime, 5);
    EXPECT_EQ(operation->operationId, 0);
    EXPECT_EQ(operation->getDuration(), 5);
    EXPECT_FALSE(operation->isScheduled());
}
```

The test confirms that:
- The operation belongs to job ID 0
- The operation is assigned to machine ID 1
- The processing time is set to 5 units
- The operation ID is 0
- The duration getter returns the same value as processing time
- The operation starts in an unscheduled state

### Operation Scheduling Test

**Test Name**: `OperationScheduling`

This test validates the scheduling functionality of operations, specifically the `setScheduled` method:

```cpp
TEST_F(ModelsTest, OperationScheduling) {
    operation->setScheduled(10, 15);
    EXPECT_TRUE(operation->isScheduled());
    EXPECT_EQ(operation->startTime, 10);
    EXPECT_EQ(operation->endTime, 15);
}
```

The test verifies that when an operation is scheduled with a start time of 10 and end time of 15:
- The scheduling status is updated to true
- The start time property is correctly set
- The end time property is correctly set

## Job Tests

### Job Creation Test

**Test Name**: `JobCreation`

This test validates the basic functionality of the Job class during initialization:

```cpp
TEST_F(ModelsTest, JobCreation) {
    EXPECT_EQ(job->jobId, 0);
    EXPECT_EQ(job->getOperationCount(), 0);
    EXPECT_EQ(job->getOperation(0), nullptr);
}
```

The test confirms that:
- The job ID is correctly set to 0
- A new job has zero operations initially
- Attempting to access a non-existent operation returns null

### Job Operations Test

**Test Name**: `JobOperations`

This test verifies the operation management capabilities of the Job class:

```cpp
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
```

The test ensures that:
- Operations can be added to a job
- The operation count is correctly updated
- Operations can be retrieved by their index
- Accessing an out-of-bounds index returns null

## Machine Tests

### Machine Creation Test

**Test Name**: `MachineCreation`

This test validates the initial state of a Machine object:

```cpp
TEST_F(ModelsTest, MachineCreation) {
    EXPECT_EQ(machine->machineId, 1);
    EXPECT_TRUE(machine->isAvailable());
    EXPECT_EQ(machine->availableTime, 0);
    EXPECT_EQ(machine->scheduledOperations.size(), 0);
}
```

The test confirms that:
- The machine ID is correctly set to 1
- A new machine starts in an available state
- The available time is initially 0
- No operations are scheduled initially

### Machine Scheduling Test

**Test Name**: `MachineScheduling`

This test verifies the scheduling functionality of the Machine class:

```cpp
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
```

The test ensures that when an operation is scheduled:
- The machine becomes unavailable
- The machine's available time is updated to the operation's end time
- The operation is added to the machine's scheduled operations list
- The operation's start and end times are updated

### Machine Reset Test

**Test Name**: `MachineReset`

This test validates the reset functionality of the Machine class:

```cpp
TEST_F(ModelsTest, MachineReset) {
    auto op1 = std::make_shared<Operation>(0, 1, 5, 0);
    machine->scheduleOperation(op1, 10);
    
    machine->reset();
    EXPECT_TRUE(machine->isAvailable());
    EXPECT_EQ(machine->availableTime, 0);
    EXPECT_EQ(machine->scheduledOperations.size(), 0);
}
```

The test confirms that after resetting:
- The machine returns to an available state
- The available time is reset to 0
- All scheduled operations are cleared

## ProblemInstance Tests

### Problem Creation Test

**Test Name**: `ProblemCreation`

This test validates the initial state of a ProblemInstance object:

```cpp
TEST_F(ModelsTest, ProblemCreation) {
    EXPECT_EQ(problem->numJobs, 0);
    EXPECT_EQ(problem->numMachines, 0);
    EXPECT_EQ(problem->jobs.size(), 0);
    EXPECT_EQ(problem->machines.size(), 0);
}
```

The test confirms that a new problem instance starts with zero jobs and machines.

### Problem Setup Test

**Test Name**: `ProblemSetup`

This test verifies the job and machine creation functionality:

```cpp
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
```

The test ensures that:
- Jobs and machines can be created in specified quantities
- The counts are correctly maintained
- Each job and machine has the expected ID

### Problem Invalid Access Test

**Test Name**: `ProblemInvalidAccess`

This test verifies that accessing invalid indices returns null:

```cpp
TEST_F(ModelsTest, ProblemInvalidAccess) {
    problem->createJobs(2);
    problem->createMachines(2);
    
    EXPECT_EQ(problem->getJob(-1), nullptr);
    EXPECT_EQ(problem->getJob(2), nullptr);
    EXPECT_EQ(problem->getMachine(-1), nullptr);
    EXPECT_EQ(problem->getMachine(2), nullptr);
}
```

The test confirms that boundary checks are properly enforced.

### Problem Clear Test

**Test Name**: `ProblemClear`

This test validates the clearing functionality of the ProblemInstance class:

```cpp
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
```

The test ensures that clearing removes all scheduled operations and resets machine states.

### Problem Total Operations Test

**Test Name**: `ProblemTotalOperations`

This test verifies the total operations counting functionality:

```cpp
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
```

The test confirms that the total operation count includes operations from all jobs.

## ScheduleResult Tests

### ScheduleResult Creation Test

**Test Name**: `ScheduleResultCreation`

This test validates the initial state of a ScheduleResult object:

```cpp
TEST_F(ModelsTest, ScheduleResultCreation) {
    ScheduleResult result;
    EXPECT_EQ(result.makespan, 0);
    EXPECT_EQ(result.totalCompletionTime, 0);
    EXPECT_DOUBLE_EQ(result.avgFlowTime, 0.0);
}
```

The test confirms that metrics start at zero values.

### ScheduleResult Metrics Test

**Test Name**: `ScheduleResultMetrics`

This test verifies the calculation of scheduling metrics:

```cpp
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
```

The test verifies that metrics are calculated correctly based on scheduled operations.

## Edge Case Tests

### Operation Edge Cases Test

**Test Name**: `OperationEdgeCases`

This test handles unusual operation scenarios:

```cpp
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
```

The test covers zero-duration operations and negative time values.

### Job Edge Cases Test

**Test Name**: `JobEdgeCases`

This test handles unusual job scenarios:

```cpp
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
```

The test covers empty jobs, null operations, and valid operations.

### Machine Edge Cases Test

**Test Name**: `MachineEdgeCases`

This test handles unusual machine scenarios:

```cpp
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
```

The test covers null operation scheduling and negative start times.

## Conclusion

The models tests provide comprehensive coverage of the core data structures in the JSSP solver. They verify correct initialization, functionality, and edge case handling for all model components. These tests ensure that the foundational elements of the system behave predictably, which is crucial for the reliability of higher-level functionality like scheduling algorithms and visualization components.