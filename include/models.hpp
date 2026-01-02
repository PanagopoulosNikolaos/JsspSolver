#ifndef MODELS_HPP
#define MODELS_HPP

#include <string>
#include <vector>
#include <memory>
#include <algorithm>

/**
 * Represents a single operation in the job shop scheduling problem.
 */
class Operation {
public:
    int jobId;
    int machineId;
    int processingTime;
    int operationId;
    int startTime;
    int endTime;

    /**
     * Constructor for Operation.
     *
     * Args:
     *   jobId: ID of the job this operation belongs to.
     *   machineId: ID of the machine required.
     *   processingTime: Time needed to process.
     *   operationId: Unique operation ID.
     */
    Operation(int jobId, int machineId, int processingTime, int operationId)
        : jobId(jobId), machineId(machineId), processingTime(processingTime),
          operationId(operationId), startTime(0), endTime(0) {}

    /**
     * Gets the processing duration.
     *
     * Returns:
     *   Processing time.
     */
    int getDuration() const { return processingTime; }

    /**
     * Sets the scheduled start and end times.
     *
     * Args:
     *   start: Start time.
     *   end: End time.
     */
    void setScheduled(int start, int end) { startTime = start; endTime = end; }

    /**
     * Checks if the operation is scheduled.
     *
     * Returns:
     *   True if scheduled.
     */
    bool isScheduled() const { return endTime > startTime; }
};

/**
 * Represents a job consisting of multiple operations.
 */
class Job {
public:
    int jobId;
    std::vector<std::shared_ptr<Operation>> operations;

    /**
     * Constructor for Job.
     *
     * Args:
     *   jobId: Unique job ID.
     */
    Job(int jobId) : jobId(jobId) {}

    /**
     * Adds an operation to the job.
     *
     * Args:
     *   op: Operation to add.
     */
    void addOperation(std::shared_ptr<Operation> op) {
        if (op) {
            operations.push_back(op);
        }
    }

    /**
     * Gets the number of operations.
     *
     * Returns:
     *   Operation count.
     */
    int getOperationCount() const { return operations.size(); }

    /**
     * Gets an operation by index.
     *
     * Args:
     *   index: Operation index.
     *
     * Returns:
     *   Operation pointer or nullptr.
     */
    std::shared_ptr<Operation> getOperation(int index) const {
        return index >= 0 && static_cast<size_t>(index) < operations.size() ? operations[index] : nullptr;
    }
};

/**
 * Represents a machine that can process operations.
 */
class Machine {
public:
    int machineId;
    std::vector<std::shared_ptr<Operation>> scheduledOperations;
    int availableTime;

    /**
     * Constructor for Machine.
     *
     * Args:
     *   machineId: Unique machine ID.
     */
    Machine(int machineId) : machineId(machineId), availableTime(0) {}

    /**
     * Schedules an operation on this machine.
     *
     * Args:
     *   op: Operation to schedule.
     *   startTime: Start time for operation.
     */
    void scheduleOperation(std::shared_ptr<Operation> op, int startTime) {
        if (op) {
            op->setScheduled(startTime, startTime + op->getDuration());
            scheduledOperations.push_back(op);
            availableTime = startTime + op->getDuration();
        }
    }

    /**
     * Resets the machine to initial state.
     */
    void reset() {
        scheduledOperations.clear();
        availableTime = 0;
    }

    /**
     * Checks if the machine is available.
     *
     * Returns:
     *   True if available.
     */
    bool isAvailable() const { return availableTime == 0; }
};

/**
 * Represents the entire job shop scheduling problem instance.
 */
class ProblemInstance {
public:
    std::vector<std::shared_ptr<Job>> jobs;
    std::vector<std::shared_ptr<Machine>> machines;
    int numJobs;
    int numMachines;

    /**
     * Constructor for ProblemInstance.
     */
    ProblemInstance() : numJobs(0), numMachines(0) {}

    /**
     * Creates a specified number of jobs.
     *
     * Args:
     *   count: Number of jobs to create.
     */
    void createJobs(int count) {
        numJobs = count;
        jobs.clear();
        for (int i = 0; i < count; ++i) {
            jobs.push_back(std::make_shared<Job>(i));
        }
    }

    /**
     * Creates a specified number of machines.
     *
     * Args:
     *   count: Number of machines to create.
     */
    void createMachines(int count) {
        numMachines = count;
        machines.clear();
        for (int i = 0; i < count; ++i) {
            machines.push_back(std::make_shared<Machine>(i));
        }
    }

    /**
     * Gets a job by ID.
     *
     * Args:
     *   jobId: Job ID.
     *
     * Returns:
     *   Job pointer or nullptr.
     */
    std::shared_ptr<Job> getJob(int jobId) const {
        return jobId >= 0 && static_cast<size_t>(jobId) < jobs.size() ? jobs[jobId] : nullptr;
    }

    /**
     * Gets a machine by ID.
     *
     * Args:
     *   machineId: Machine ID.
     *
     * Returns:
     *   Machine pointer or nullptr.
     */
    std::shared_ptr<Machine> getMachine(int machineId) const {
        return machineId >= 0 && static_cast<size_t>(machineId) < machines.size() ? machines[machineId] : nullptr;
    }

    /**
     * Clears all operations and resets machines.
     */
    void clear() {
        for (auto& job : jobs) {
            job->operations.clear();
        }
        for (auto& machine : machines) {
            machine->reset();
        }
    }

    /**
     * Gets the total number of operations.
     *
     * Returns:
     *   Total operations count.
     */
    int getTotalOperations() const {
        int total = 0;
        for (const auto& job : jobs) {
            total += job->operations.size();
        }
        return total;
    }
};

/**
 * Represents the result of scheduling a problem instance.
 */
class ScheduleResult {
public:
    ProblemInstance problem;
    std::vector<std::shared_ptr<Job>> scheduledJobs;
    int makespan;
    int totalCompletionTime;
    double avgFlowTime;

    /**
     * Constructor for ScheduleResult.
     */
    ScheduleResult() : makespan(0), totalCompletionTime(0), avgFlowTime(0.0) {}

    /**
     * Calculates scheduling metrics like makespan and flow time.
     */
    void calculateMetrics() {
        makespan = 0;
        totalCompletionTime = 0;

        for (const auto& job : problem.jobs) {
            int jobCompletionTime = 0;
            for (const auto& operation : job->operations) {
                if (operation->isScheduled()) {
                    jobCompletionTime = std::max(jobCompletionTime, operation->endTime);
                }
            }
            totalCompletionTime += jobCompletionTime;
            makespan = std::max(makespan, jobCompletionTime);
        }

        avgFlowTime = problem.jobs.empty() ? 0.0 : static_cast<double>(totalCompletionTime) / problem.jobs.size();
    }
};

#endif // MODELS_HPP
