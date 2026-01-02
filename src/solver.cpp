#include "solver.hpp"
#include <iomanip>

// FIFO (First-In-First-Out) Algorithm Implementation
void Solver::scheduleFIFO(std::shared_ptr<ProblemInstance> problem) {
    std::cout << "Scheduling with FIFO algorithm..." << std::endl;
    
    // Reset all machines
    for (auto& machine : problem->machines) {
        machine->reset();
    }
    
    // Clear all job operations (reset scheduling)
    for (auto& job : problem->jobs) {
        for (auto& operation : job->operations) {
            operation->startTime = 0;
            operation->endTime = 0;
        }
    }
    
    // FIFO: Process operations in order they were added to each job
    bool operationScheduled = true;
    int iteration = 0;
    
    while (operationScheduled && iteration < 1000) { // Safety limit
        operationScheduled = false;
        iteration++;
        
        // Try to schedule operations for each job
        for (auto& job : problem->jobs) {
            for (const auto& operation : job->operations) {
                if (!operation->isScheduled()) {
                    auto machine = problem->getMachine(operation->machineId);
                    if (machine) {
                        // Check if all previous operations in the job are completed
                        bool allPrevOpsCompleted = true;
                        for (const auto& prevOp : job->operations) {
                            if (prevOp->operationId < operation->operationId && !prevOp->isScheduled()) {
                                allPrevOpsCompleted = false;
                                break;
                            }
                        }
                        
                        if (allPrevOpsCompleted) {
                            int startTime = machine->availableTime;
                            // Also consider the completion time of the previous operation in the same job
                            int jobReadyTime = 0;
                            for (const auto& prevOp : job->operations) {
                                if (prevOp->operationId < operation->operationId) {
                                    jobReadyTime = std::max(jobReadyTime, prevOp->endTime);
                                }
                            }
                            startTime = std::max(startTime, jobReadyTime);
                            
                            machine->scheduleOperation(operation, startTime);
                            operationScheduled = true;
                            std::cout << "Scheduled Job " << operation->jobId 
                                     << " Operation " << operation->operationId 
                                     << " on Machine " << operation->machineId 
                                     << " [" << startTime << "-" << operation->endTime << "]" << std::endl;
                        }
                    }
                }
            }
        }
    }
    
    if (iteration >= 1000) {
        std::cerr << "Warning: Scheduling may not have completed properly" << std::endl;
    }
}

// SPT (Shortest Processing Time) Algorithm Implementation
void Solver::scheduleSPT(std::shared_ptr<ProblemInstance> problem) {
    std::cout << "Scheduling with SPT algorithm..." << std::endl;
    
    // Reset all machines
    for (auto& machine : problem->machines) {
        machine->reset();
    }
    
    // Clear all job operations
    for (auto& job : problem->jobs) {
        for (auto& operation : job->operations) {
            operation->startTime = 0;
            operation->endTime = 0;
        }
    }
    
    scheduleWithPriority(problem, [](const std::shared_ptr<Operation>& a, const std::shared_ptr<Operation>& b) {
        return a->processingTime < b->processingTime;
    });
}

// LPT (Longest Processing Time) Algorithm Implementation
void Solver::scheduleLPT(std::shared_ptr<ProblemInstance> problem) {
    std::cout << "Scheduling with LPT algorithm..." << std::endl;
    
    // Reset all machines
    for (auto& machine : problem->machines) {
        machine->reset();
    }
    
    // Clear all job operations
    for (auto& job : problem->jobs) {
        for (auto& operation : job->operations) {
            operation->startTime = 0;
            operation->endTime = 0;
        }
    }
    
    scheduleWithPriority(problem, [](const std::shared_ptr<Operation>& a, const std::shared_ptr<Operation>& b) {
        return a->processingTime > b->processingTime;
    });
}

// Generic priority-based scheduling
void Solver::scheduleWithPriority(std::shared_ptr<ProblemInstance> problem, 
                                 std::function<bool(const std::shared_ptr<Operation>&, 
                                                   const std::shared_ptr<Operation>&)> compare) {
    bool operationScheduled = true;
    int iteration = 0;
    
    while (operationScheduled && iteration < 1000) { // Safety limit
        operationScheduled = false;
        iteration++;
        
        // Collect all ready operations (previous operations completed)
        std::vector<std::shared_ptr<Operation>> readyOperations;
        
        for (auto& job : problem->jobs) {
            for (const auto& operation : job->operations) {
                if (!operation->isScheduled()) {
                    // Check if all previous operations in the job are completed
                    bool allPrevOpsCompleted = true;
                    for (const auto& prevOp : job->operations) {
                        if (prevOp->operationId < operation->operationId && !prevOp->isScheduled()) {
                            allPrevOpsCompleted = false;
                            break;
                        }
                    }
                    
                    if (allPrevOpsCompleted) {
                        readyOperations.push_back(operation);
                    }
                }
            }
        }
        
        // Sort ready operations by priority
        std::sort(readyOperations.begin(), readyOperations.end(), compare);
        
        // Schedule operations in priority order
        for (const auto& operation : readyOperations) {
            if (!operation->isScheduled()) {
                auto machine = problem->getMachine(operation->machineId);
                if (machine) {
                    int startTime = machine->availableTime;
                    // Also consider the completion time of the previous operation in the same job
                    int jobReadyTime = 0;
                    auto job = problem->getJob(operation->jobId);
                    if (job) {
                        for (const auto& prevOp : job->operations) {
                            if (prevOp->operationId < operation->operationId) {
                                jobReadyTime = std::max(jobReadyTime, prevOp->endTime);
                            }
                        }
                    }
                    startTime = std::max(startTime, jobReadyTime);
                    
                    machine->scheduleOperation(operation, startTime);
                    operationScheduled = true;
                    std::cout << "Scheduled Job " << operation->jobId 
                             << " Operation " << operation->operationId 
                             << " on Machine " << operation->machineId 
                             << " [" << startTime << "-" << operation->endTime << "]" << std::endl;
                }
            }
        }
    }
}

// Main solve method
std::shared_ptr<ScheduleResult> Solver::solve(std::shared_ptr<ProblemInstance> problem) {
    if (!problem) {
        throw std::runtime_error("Problem instance is null");
    }
    
    auto result = std::make_shared<ScheduleResult>();
    
    // Schedule based on algorithm
    switch (algorithm) {
        case SchedulingAlgorithm::FIFO:
            scheduleFIFO(problem);
            break;
        case SchedulingAlgorithm::SPT:
            scheduleSPT(problem);
            break;
        case SchedulingAlgorithm::LPT:
            scheduleLPT(problem);
            break;
        default:
            throw std::runtime_error("Unknown algorithm");
    }
    
    result->problem = *problem; // Copy problem AFTER scheduling
    
    // Calculate metrics
    result->calculateMetrics();
    
    std::cout << "\nScheduling completed!" << std::endl;
    std::cout << "Algorithm: " << getCurrentAlgorithmName() << std::endl;
    std::cout << "Makespan: " << result->makespan << std::endl;
    std::cout << "Total Completion Time: " << result->totalCompletionTime << std::endl;
    std::cout << "Average Flow Time: " << result->avgFlowTime << std::endl;
    
    return result;
}

// Constructor
Solver::Solver(SchedulingAlgorithm algo) : algorithm(algo) {}

// Set algorithm
void Solver::setAlgorithm(SchedulingAlgorithm algo) { 
    algorithm = algo; 
}

// Get algorithm
SchedulingAlgorithm Solver::getAlgorithm() const { 
    return algorithm; 
}

// Static factory methods
std::shared_ptr<Solver> Solver::createFIFOSolver() {
    return std::make_shared<Solver>(SchedulingAlgorithm::FIFO);
}

std::shared_ptr<Solver> Solver::createSPTSolver() {
    return std::make_shared<Solver>(SchedulingAlgorithm::SPT);
}

std::shared_ptr<Solver> Solver::createLPTSolver() {
    return std::make_shared<Solver>(SchedulingAlgorithm::LPT);
}

// Get algorithm name
std::string Solver::getAlgorithmName(SchedulingAlgorithm algo) {
    switch (algo) {
        case SchedulingAlgorithm::FIFO: return "FIFO (First-In-First-Out)";
        case SchedulingAlgorithm::SPT: return "SPT (Shortest Processing Time)";
        case SchedulingAlgorithm::LPT: return "LPT (Longest Processing Time)";
        default: return "Unknown";
    }
}

// Get current algorithm name
std::string Solver::getCurrentAlgorithmName() const {
    return getAlgorithmName(algorithm);
}

// Compare solutions from different algorithms
void Solver::compareSolutions(const std::shared_ptr<ScheduleResult>& result1, 
                             const std::shared_ptr<ScheduleResult>& result2,
                             const std::string& name1, const std::string& name2) {
    std::cout << "\n=== Algorithm Comparison ===" << std::endl;
    std::cout << std::setw(20) << "Metric" << std::setw(15) << name1 << std::setw(15) << name2 << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << std::setw(20) << "Makespan" 
             << std::setw(15) << result1->makespan 
             << std::setw(15) << result2->makespan << std::endl;
    std::cout << std::setw(20) << "Total Completion Time" 
             << std::setw(15) << result1->totalCompletionTime 
             << std::setw(15) << result2->totalCompletionTime << std::endl;
    std::cout << std::setw(20) << "Average Flow Time" 
             << std::setw(15) << std::fixed << std::setprecision(2) << result1->avgFlowTime 
             << std::setw(15) << std::fixed << std::setprecision(2) << result2->avgFlowTime << std::endl;
    
    // Determine better solution
    std::cout << "\nBetter Solution: ";
    if (result1->makespan < result2->makespan) {
        std::cout << name1 << " (lower makespan)" << std::endl;
    } else if (result2->makespan < result1->makespan) {
        std::cout << name2 << " (lower makespan)" << std::endl;
    } else {
        std::cout << "Tie (equal makespan)" << std::endl;
    }
}
