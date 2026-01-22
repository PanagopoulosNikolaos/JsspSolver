# Models Documentation

## Overview
The models.hpp file defines the core data structures used throughout the JSSP (Job Shop Scheduling Problem) solver. These classes represent the fundamental entities involved in job shop scheduling: operations, jobs, machines, problem instances, and schedule results.

## Key Components
- Operation: Represents a single task that needs to be processed on a specific machine
- Job: Contains a sequence of operations that must be processed in order
- Machine: Represents a resource that can process operations with time constraints
- ProblemInstance: Contains the complete problem definition with jobs and machines
- ScheduleResult: Represents a solution to a problem instance with performance metrics

## Dependencies
```cpp
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
```

## Classes

### Operation
Represents a single operation in the job shop scheduling problem.

#### Members
- `jobId`: ID of the job this operation belongs to
- `machineId`: ID of the machine required for this operation
- `processingTime`: Time needed to process this operation
- `operationId`: Unique identifier for this operation
- `startTime`: Scheduled start time (initially 0)
- `endTime`: Scheduled end time (initially 0)

#### Methods
- `Operation(jobId, machineId, processingTime, operationId)`: Constructor
- `getDuration()`: Returns the processing time
- `setScheduled(start, end)`: Sets the scheduled start and end times
- `isScheduled()`: Checks if the operation has been scheduled

### Job
Represents a job consisting of multiple operations that must be processed in sequence.

#### Members
- `jobId`: Unique identifier for this job
- `operations`: Vector of shared pointers to Operation objects

#### Methods
- `Job(jobId)`: Constructor
- `addOperation(op)`: Adds an operation to the job
- `getOperationCount()`: Returns the number of operations in the job
- `getOperation(index)`: Gets an operation by index

### Machine
Represents a machine that can process operations with time constraints.

#### Members
- `machineId`: Unique identifier for this machine
- `scheduledOperations`: Vector of scheduled operations on this machine
- `availableTime`: The earliest time this machine will be available

#### Methods
- `Machine(machineId)`: Constructor
- `scheduleOperation(op, startTime)`: Schedules an operation on this machine
- `reset()`: Resets the machine to its initial state
- `isAvailable()`: Checks if the machine is currently available

### ProblemInstance
Represents the entire job shop scheduling problem instance.

#### Members
- `jobs`: Vector of shared pointers to Job objects
- `machines`: Vector of shared pointers to Machine objects
- `numJobs`: Number of jobs in the problem
- `numMachines`: Number of machines in the problem

#### Methods
- `ProblemInstance()`: Constructor
- `createJobs(count)`: Creates a specified number of jobs
- `createMachines(count)`: Creates a specified number of machines
- `getJob(jobId)`: Gets a job by ID
- `getMachine(machineId)`: Gets a machine by ID
- `clear()`: Clears all operations and resets machines
- `getTotalOperations()`: Gets the total number of operations

### ScheduleResult
Represents the result of scheduling a problem instance.

#### Members
- `problem`: The original problem instance
- `scheduledJobs`: Vector of scheduled jobs
- `makespan`: The total time to complete all jobs
- `totalCompletionTime`: Sum of completion times for all jobs
- `avgFlowTime`: Average flow time of all jobs

#### Methods
- `ScheduleResult()`: Constructor
- `calculateMetrics()`: Calculates scheduling metrics like makespan and flow time

## Usage Example
```cpp
// Create a problem instance
auto problem = std::make_shared<ProblemInstance>();
problem->createJobs(3);  // Create 3 jobs
problem->createMachines(2);  // Create 2 machines

// Create operations for job 0
auto job0 = problem->getJob(0);
auto op1 = std::make_shared<Operation>(0, 0, 5, 0);  // Job 0, Machine 0, Time 5
auto op2 = std::make_shared<Operation>(0, 1, 3, 1);  // Job 0, Machine 1, Time 3
job0->addOperation(op1);
job0->addOperation(op2);

// Process and schedule the operations
// ... (scheduling logic)

// Calculate results
ScheduleResult result;
result.problem = *problem;
result.calculateMetrics();
```

These model classes form the foundation of the JSSP solver and are used by other components like the solver, parser, and Gantt chart generator.