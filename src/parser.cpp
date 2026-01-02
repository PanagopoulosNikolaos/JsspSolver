#include "parser.hpp"

/**
 * Parses a JSSP instance from file.
 *
 * Args:
 *   filename: Path to input file.
 *
 * Returns:
 *   Parsed problem instance.
 */
std::shared_ptr<ProblemInstance> Parser::parseFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    std::shared_ptr<ProblemInstance> problem = std::make_shared<ProblemInstance>();
    
    int numJobs, numMachines;
    if (!(file >> numJobs >> numMachines)) {
        throw std::runtime_error("No valid operations found in file");
    }
    
    if (numJobs <= 0 || numMachines <= 0) {
        throw std::runtime_error("Invalid number of jobs or machines");
    }
    
    problem->createJobs(numJobs);
    problem->createMachines(numMachines);
    
    // Read operations
    int jobId, machineId, processingTime;
    int operationCount = 0;
    
    while (file >> jobId >> machineId >> processingTime) {
        if (jobId < 0 || jobId >= numJobs || 
            machineId < 0 || machineId >= numMachines || 
            processingTime <= 0) {
            std::cerr << "Warning: Invalid operation data (Job: " << jobId 
                     << ", Machine: " << machineId 
                     << ", Time: " << processingTime << ")" << std::endl;
            continue;
        }
        
        auto operation = std::make_shared<Operation>(jobId, machineId, processingTime, operationCount);
        auto job = problem->getJob(jobId);
        if (job) {
            job->addOperation(operation);
        }
        operationCount++;
    }
    
    file.close();
    
    if (operationCount == 0) {
        throw std::runtime_error("No valid operations found in file");
    }
    
    std::cout << "Parsed problem: " << numJobs << " jobs, " << numMachines 
              << " machines, " << operationCount << " operations" << std::endl;
    
    return problem;
}

/**
 * Parses a problem instance from string format.
 *
 * Args:
 *   data: String containing problem data.
 *
 * Returns:
 *   Parsed problem instance.
 */
std::shared_ptr<ProblemInstance> Parser::parseString(const std::string& data) {
    std::istringstream iss(data);
    std::shared_ptr<ProblemInstance> problem = std::make_shared<ProblemInstance>();
    
    int numJobs, numMachines;
    iss >> numJobs >> numMachines;
    
    if (numJobs <= 0 || numMachines <= 0) {
        throw std::runtime_error("Invalid number of jobs or machines");
    }
    
    problem->createJobs(numJobs);
    problem->createMachines(numMachines);
    
    int jobId, machineId, processingTime;
    int operationCount = 0;
    
    while (iss >> jobId >> machineId >> processingTime) {
        auto operation = std::make_shared<Operation>(jobId, machineId, processingTime, operationCount);
        auto job = problem->getJob(jobId);
        if (job) {
            job->addOperation(operation);
        }
        operationCount++;
    }
    
    return problem;
}

/**
 * Saves a problem instance to file for debugging/testing.
 *
 * Args:
 *   problem: Problem instance to save.
 *   filename: Output file path.
 */
void Parser::saveToFile(const std::shared_ptr<ProblemInstance>& problem, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not create file: " + filename);
    }
    
    file << problem->numJobs << " " << problem->numMachines << std::endl;
    
    for (const auto& job : problem->jobs) {
        for (const auto& operation : job->operations) {
            file << job->jobId << " " << operation->machineId << " " << operation->processingTime << std::endl;
        }
    }
    
    file.close();
}

/**
 * Generates a simple test problem programmatically.
 *
 * Returns:
 *   Generated problem instance.
 */
std::shared_ptr<ProblemInstance> Parser::generateSimpleProblem() {
    auto problem = std::make_shared<ProblemInstance>();
    problem->createJobs(3);
    problem->createMachines(3);
    
    // Job 0: M0(2), M1(3), M2(1)
    problem->getJob(0)->addOperation(std::make_shared<Operation>(0, 0, 2, 0));
    problem->getJob(0)->addOperation(std::make_shared<Operation>(0, 1, 3, 1));
    problem->getJob(0)->addOperation(std::make_shared<Operation>(0, 2, 1, 2));
    
    // Job 1: M1(1), M2(2), M0(3)
    problem->getJob(1)->addOperation(std::make_shared<Operation>(1, 1, 1, 3));
    problem->getJob(1)->addOperation(std::make_shared<Operation>(1, 2, 2, 4));
    problem->getJob(1)->addOperation(std::make_shared<Operation>(1, 0, 3, 5));
    
    // Job 2: M2(3), M0(1), M1(2)
    problem->getJob(2)->addOperation(std::make_shared<Operation>(2, 2, 3, 6));
    problem->getJob(2)->addOperation(std::make_shared<Operation>(2, 0, 1, 7));
    problem->getJob(2)->addOperation(std::make_shared<Operation>(2, 1, 2, 8));
    
    return problem;
}

// Load a solution from a file (supports TEXT, JSON, and XML formats)
std::shared_ptr<ScheduleResult> Parser::loadSolution(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    // Read first few lines to detect format
    std::string firstLine;
    std::getline(file, firstLine);
    file.close();
    
    // Detect format based on content
    if (firstLine.find("JSSP SOLUTION EXPORT") != std::string::npos) {
        return loadTextSolution(filename);
    } else if (firstLine.find("<?xml") != std::string::npos) {
        return loadXMLSolution(filename);
    } else if (firstLine.find("{") != std::string::npos || firstLine.find("\"problem\"") != std::string::npos) {
        return loadJSONSolution(filename);
    } else {
        throw std::runtime_error("Unknown solution file format");
    }
}

/**
 * Loads a solution from TEXT format.
 *
 * Args:
 *   filename: Path to TEXT solution file.
 *
 * Returns:
 *   Loaded schedule result.
 */
std::shared_ptr<ScheduleResult> Parser::loadTextSolution(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    auto result = std::make_shared<ScheduleResult>();
    std::string line;
    
    // Skip header
    std::getline(file, line); // "JSSP SOLUTION EXPORT"
    std::getline(file, line); // "==================="
    std::getline(file, line); // empty line
    
    // Parse problem metadata
    std::getline(file, line); // "PROBLEM METADATA:"
    std::getline(file, line); // "Jobs: X"
    try {
        result->problem.numJobs = std::stoi(line.substr(line.find(":") + 1));
    } catch (const std::exception&) { result->problem.numJobs = 0; }
    
    std::getline(file, line); // "Machines: X"
    try {
        result->problem.numMachines = std::stoi(line.substr(line.find(":") + 1));
    } catch (const std::exception&) { result->problem.numMachines = 0; }
    
    result->problem.createJobs(result->problem.numJobs);
    result->problem.createMachines(result->problem.numMachines);
    
    // Skip to scheduling results
    while (std::getline(file, line) && line != "SCHEDULING RESULTS:");
    
    // Parse operations
    while (std::getline(file, line) && line != "MACHINE SCHEDULES:") {
        if (line.find("Job ") == 0) {
            // Parse job line
            int jobId = 0;
            try {
                jobId = std::stoi(line.substr(4, line.find(":") - 4));
            } catch (const std::exception&) { continue; }
            
            auto job = result->problem.getJob(jobId);
            
            // Parse operations for this job
            while (std::getline(file, line) && line.find("  Operation ") == 0) {
                // Parse operation line: " Operation X: Machine Y [start-end]"
                size_t opStart = line.find("Operation ") + 10;
                size_t opEnd = line.find(":", opStart);
                int operationId = 0;
                try {
                    operationId = std::stoi(line.substr(opStart, opEnd - opStart));
                } catch (const std::exception&) { continue; }
                
                size_t machineStart = line.find("Machine ") + 8;
                size_t machineEnd = line.find(" ", machineStart);
                int machineId = 0;
                try {
                    machineId = std::stoi(line.substr(machineStart, machineEnd - machineStart));
                } catch (const std::exception&) { continue; }
                
                size_t timeStart = line.find("[") + 1;
                size_t timeEnd = line.find("]", timeStart);
                std::string timeStr = line.substr(timeStart, timeEnd - timeStart);
                size_t dashPos = timeStr.find("-");
                int startTime = 0, endTime = 0;
                try {
                    startTime = std::stoi(timeStr.substr(0, dashPos));
                    endTime = std::stoi(timeStr.substr(dashPos + 1));
                } catch (const std::exception&) { continue; }
                
                auto operation = std::make_shared<Operation>(jobId, machineId, endTime - startTime, operationId);
                operation->setScheduled(startTime, endTime);
                job->addOperation(operation);
            }
        }
    }
    
    // Parse machine schedules
    while (std::getline(file, line) && line != "PERFORMANCE METRICS:") {
        if (line.find("Machine ") == 0) {
            int machineId = 0;
            try {
                machineId = std::stoi(line.substr(8, line.find(":") - 8));
            } catch (const std::exception&) { continue; }
            
            auto machine = result->problem.getMachine(machineId);
            if (!machine) continue;
            
            while (std::getline(file, line) && line.find("  Job ") == 0) {
                // Parse operation line: " Job X Operation Y [start-end]"
                size_t jobStart = line.find("Job ") + 4;
                size_t jobEnd = line.find(" Operation", jobStart);
                int jobId = 0;
                try {
                    jobId = std::stoi(line.substr(jobStart, jobEnd - jobStart));
                } catch (const std::exception&) { continue; }
                
                size_t opStart = line.find("Operation ") + 10;
                size_t opEnd = line.find(" [", opStart);
                int operationId = 0;
                try {
                    operationId = std::stoi(line.substr(opStart, opEnd - opStart));
                } catch (const std::exception&) { continue; }
                
                size_t timeStart = line.find("[") + 1;
                size_t timeEnd = line.find("]", timeStart);
                std::string timeStr = line.substr(timeStart, timeEnd - timeStart);
                size_t dashPos = timeStr.find("-");
                int startTime = 0, endTime = 0;
                try {
                    startTime = std::stoi(timeStr.substr(0, dashPos));
                    endTime = std::stoi(timeStr.substr(dashPos + 1));
                } catch (const std::exception&) { continue; }
                
                auto operation = std::make_shared<Operation>(jobId, machineId, endTime - startTime, operationId);
                operation->setScheduled(startTime, endTime);
                machine->scheduledOperations.push_back(operation);
                machine->availableTime = endTime;
            }
        }
    }
    
    // Parse performance metrics
    while (std::getline(file, line)) {
        if (line.find("Makespan: ") == 0) {
            try {
                result->makespan = std::stoi(line.substr(10));
            } catch (const std::exception&) {
                result->makespan = 0;
            }
        } else if (line.find("Total Completion Time: ") == 0) {
            try {
                result->totalCompletionTime = std::stoi(line.substr(23));
            } catch (const std::exception&) {
                result->totalCompletionTime = 0;
            }
        } else if (line.find("Average Flow Time: ") == 0) {
            try {
                result->avgFlowTime = std::stod(line.substr(19));
            } catch (const std::exception&) {
                result->avgFlowTime = 0.0;
            }
        }
    }
    
    file.close();
    return result;
}

/**
 * Loads a solution from JSON format.
 *
 * Args:
 *   filename: Path to JSON solution file.
 *
 * Returns:
 *   Loaded schedule result.
 */
std::shared_ptr<ScheduleResult> Parser::loadJSONSolution(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    json j;
    file >> j;
    file.close();
    
    auto result = std::make_shared<ScheduleResult>();
    
    // Parse problem metadata
    result->problem.numJobs = j["problem"]["numJobs"];
    result->problem.numMachines = j["problem"]["numMachines"];
    
    result->problem.createJobs(result->problem.numJobs);
    result->problem.createMachines(result->problem.numMachines);
    
    // Parse operations
    for (const auto& opJson : j["operations"]) {
        int jobId = opJson["jobId"];
        int machineId = opJson["machineId"];
        int processingTime = opJson["processingTime"];
        int operationId = opJson["operationId"];
        int startTime = opJson["startTime"];
        int endTime = opJson["endTime"];
        
        auto operation = std::make_shared<Operation>(jobId, machineId, processingTime, operationId);
        operation->setScheduled(startTime, endTime);
        
        auto job = result->problem.getJob(jobId);
        if (job) {
            job->addOperation(operation);
        }
    }
    
    // Parse machine schedules
    for (const auto& machineJson : j["machines"]) {
        int machineId = machineJson["machineId"];
        auto machine = result->problem.getMachine(machineId);
        if (machine) {
            machine->availableTime = machineJson["availableTime"];
            
            for (const auto& opJson : machineJson["scheduledOperations"]) {
                int jobId = opJson["jobId"];
                int operationId = opJson["operationId"];
                int startTime = opJson["startTime"];
                int endTime = opJson["endTime"];
                
                // Find the operation in the job
                auto job = result->problem.getJob(jobId);
                if (job) {
                    for (const auto& operation : job->operations) {
                        if (operation->operationId == operationId) {
                            operation->setScheduled(startTime, endTime);
                            machine->scheduledOperations.push_back(operation);
                            break;
                        }
                    }
                }
            }
        }
    }
    
    // Parse metrics
    result->makespan = j["metrics"]["makespan"];
    result->totalCompletionTime = j["metrics"]["totalCompletionTime"];
    result->avgFlowTime = j["metrics"]["averageFlowTime"];
    
    return result;
}

/**
 * Loads a solution from XML format.
 *
 * Args:
 *   filename: Path to XML solution file.
 *
 * Returns:
 *   Loaded schedule result.
 */
std::shared_ptr<ScheduleResult> Parser::loadXMLSolution(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filename);
    }
    
    // For simplicity, we'll use a basic string parsing approach
    // In a production environment, you might want to use a proper XML parser
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    
    auto result = std::make_shared<ScheduleResult>();
    
    // Parse basic XML structure using string operations
    // This is a simplified implementation - a real XML parser would be more robust
    
    // Extract problem metadata
    result->problem.numJobs = extractIntFromXML(content, "numJobs");
    result->problem.numMachines = extractIntFromXML(content, "numMachines");
    
    result->problem.createJobs(result->problem.numJobs);
    result->problem.createMachines(result->problem.numMachines);
    
    // Parse operations
    size_t opStart = 0;
    while ((opStart = content.find("<operation>", opStart)) != std::string::npos) {
        size_t opEnd = content.find("</operation>", opStart);
        if (opEnd == std::string::npos) break;
        std::string opContent = content.substr(opStart, opEnd - opStart + 12);
        
        int jobId = extractIntFromXML(opContent, "jobId");
        int machineId = extractIntFromXML(opContent, "machineId");
        int processingTime = extractIntFromXML(opContent, "processingTime");
        int operationId = extractIntFromXML(opContent, "operationId");
        int startTime = extractIntFromXML(opContent, "startTime");
        int endTime = extractIntFromXML(opContent, "endTime");
        
        auto operation = std::make_shared<Operation>(jobId, machineId, processingTime, operationId);
        operation->setScheduled(startTime, endTime);
        
        auto job = result->problem.getJob(jobId);
        if (job) {
            job->addOperation(operation);
        }
        
        opStart = opEnd;
    }
    
    // Parse machine schedules
    size_t machineStart = 0;
    while ((machineStart = content.find("<machine>", machineStart)) != std::string::npos) {
        size_t machineEnd = content.find("</machine>", machineStart);
        if (machineEnd == std::string::npos) break;
        std::string machineContent = content.substr(machineStart, machineEnd - machineStart + 10);
        
        int machineId = extractIntFromXML(machineContent, "machineId");
        auto machine = result->problem.getMachine(machineId);
        if (machine) {
            machine->availableTime = extractIntFromXML(machineContent, "availableTime");
            
            size_t opStart = 0;
            while ((opStart = machineContent.find("<scheduledOperation>", opStart)) != std::string::npos) {
                size_t opEnd = machineContent.find("</scheduledOperation>", opStart);
                if (opEnd == std::string::npos) break;
                std::string opContent = machineContent.substr(opStart, opEnd - opStart + 20);
                
                int jobId = extractIntFromXML(opContent, "jobId");
                int operationId = extractIntFromXML(opContent, "operationId");
                int startTime = extractIntFromXML(opContent, "startTime");
                int endTime = extractIntFromXML(opContent, "endTime");
                
                // Find the operation in the job
                auto job = result->problem.getJob(jobId);
                if (job) {
                    for (const auto& operation : job->operations) {
                        if (operation->operationId == operationId) {
                            operation->setScheduled(startTime, endTime);
                            machine->scheduledOperations.push_back(operation);
                            break;
                        }
                    }
                }
                
                opStart = opEnd;
            }
        }
        
        machineStart = machineEnd;
    }
    
    // Parse metrics
    result->makespan = extractIntFromXML(content, "makespan");
    result->totalCompletionTime = extractIntFromXML(content, "totalCompletionTime");
    result->avgFlowTime = extractDoubleFromXML(content, "averageFlowTime");
    
    return result;
}

// Helper function to extract integer from XML tag
int Parser::extractIntFromXML(const std::string& content, const std::string& tag) {
    std::string startTag = "<" + tag + ">";
    std::string endTag = "</" + tag + ">";
    
    size_t startPos = content.find(startTag);
    if (startPos == std::string::npos) return 0;
    
    startPos += startTag.length();
    size_t endPos = content.find(endTag, startPos);
    if (endPos == std::string::npos) return 0;
    
    std::string valueStr = content.substr(startPos, endPos - startPos);
    try {
        return std::stoi(valueStr);
    } catch (const std::exception&) {
        return 0;
    }
}

// Helper function to extract double from XML tag
double Parser::extractDoubleFromXML(const std::string& content, const std::string& tag) {
    std::string startTag = "<" + tag + ">";
    std::string endTag = "</" + tag + ">";
    
    size_t startPos = content.find(startTag);
    if (startPos == std::string::npos) return 0.0;
    
    startPos += startTag.length();
    size_t endPos = content.find(endTag, startPos);
    if (endPos == std::string::npos) return 0.0;
    
    std::string valueStr = content.substr(startPos, endPos - startPos);
    try {
        return std::stod(valueStr);
    } catch (const std::exception&) {
        return 0.0;
    }
}
