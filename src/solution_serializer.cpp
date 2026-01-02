#include "solution_serializer.hpp"

/**
 * Exports a ScheduleResult to a file in the specified format.
 *
 * Args:
 *   result: Schedule result to export.
 *   filename: Output file path.
 *   format: Export format.
 */
void SolutionSerializer::exportSolution(const std::shared_ptr<ScheduleResult>& result,
                                       const std::string& filename,
                                       ExportFormat format) {
    if (!result) {
        throw std::runtime_error("Cannot export null solution");
    }
    
    switch (format) {
        case ExportFormat::TEXT:
            exportText(result, filename);
            break;
        case ExportFormat::JSON:
            exportJSON(result, filename);
            break;
        case ExportFormat::XML:
            exportXML(result, filename);
            break;
    }
}

/**
 * Exports a ScheduleResult to text format.
 *
 * Args:
 *   result: Schedule result to export.
 *   filename: Output file path.
 */
void SolutionSerializer::exportText(const std::shared_ptr<ScheduleResult>& result,
                                   const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not create file: " + filename);
    }
    
    // Header
    file << "JSSP SOLUTION EXPORT\n";
    file << "===================\n\n";
    
    // Problem metadata
    file << "PROBLEM METADATA:\n";
    file << "Jobs: " << result->problem.numJobs << "\n";
    file << "Machines: " << result->problem.numMachines << "\n";
    file << "Total Operations: " << result->problem.getTotalOperations() << "\n\n";
    
    // Scheduling results
    file << "SCHEDULING RESULTS:\n";
    file << "===================\n\n";
    
    for (const auto& job : result->problem.jobs) {
        file << "Job " << job->jobId << ":\n";
        for (const auto& operation : job->operations) {
            if (operation->isScheduled()) {
                file << "  Operation " << operation->operationId 
                     << ": Machine " << operation->machineId 
                     << " [" << operation->startTime << "-" << operation->endTime << "]\n";
            }
        }
        file << "\n";
    }
    
    // Machine schedules
    file << "MACHINE SCHEDULES:\n";
    file << "==================\n\n";
    
    for (const auto& machine : result->problem.machines) {
        file << "Machine " << machine->machineId << ":\n";
        for (const auto& operation : machine->scheduledOperations) {
            file << "  Job " << operation->jobId 
                 << " Operation " << operation->operationId 
                 << " [" << operation->startTime << "-" << operation->endTime << "]\n";
        }
        file << "\n";
    }
    
    // Performance metrics
    file << "PERFORMANCE METRICS:\n";
    file << "====================\n";
    file << "Makespan: " << result->makespan << "\n";
    file << "Total Completion Time: " << result->totalCompletionTime << "\n";
    file << "Average Flow Time: " << result->avgFlowTime << "\n\n";
    
    file.close();
}

/**
 * Exports a ScheduleResult to JSON format.
 *
 * Args:
 *   result: Schedule result to export.
 *   filename: Output file path.
 */
void SolutionSerializer::exportJSON(const std::shared_ptr<ScheduleResult>& result,
                                   const std::string& filename) {
    json j;
    
    // Problem metadata
    j["problem"]["numJobs"] = result->problem.numJobs;
    j["problem"]["numMachines"] = result->problem.numMachines;
    j["problem"]["totalOperations"] = result->problem.getTotalOperations();
    
    // Operations data
    json operations = json::array();
    for (const auto& job : result->problem.jobs) {
        for (const auto& operation : job->operations) {
            json op;
            op["jobId"] = operation->jobId;
            op["machineId"] = operation->machineId;
            op["processingTime"] = operation->processingTime;
            op["operationId"] = operation->operationId;
            op["startTime"] = operation->startTime;
            op["endTime"] = operation->endTime;
            op["scheduled"] = operation->isScheduled();
            operations.push_back(op);
        }
    }
    j["operations"] = operations;
    
    // Machine schedules
    json machines = json::array();
    for (const auto& machine : result->problem.machines) {
        json m;
        m["machineId"] = machine->machineId;
        m["availableTime"] = machine->availableTime;
        
        json scheduledOps = json::array();
        for (const auto& operation : machine->scheduledOperations) {
            json op;
            op["jobId"] = operation->jobId;
            op["operationId"] = operation->operationId;
            op["startTime"] = operation->startTime;
            op["endTime"] = operation->endTime;
            scheduledOps.push_back(op);
        }
        m["scheduledOperations"] = scheduledOps;
        machines.push_back(m);
    }
    j["machines"] = machines;
    
    // Performance metrics
    j["metrics"]["makespan"] = result->makespan;
    j["metrics"]["totalCompletionTime"] = result->totalCompletionTime;
    j["metrics"]["averageFlowTime"] = result->avgFlowTime;
    
    // Write to file
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not create file: " + filename);
    }
    file << std::setw(4) << j << std::endl;
    file.close();
}

/**
 * Exports a ScheduleResult to XML format.
 *
 * Args:
 *   result: Schedule result to export.
 *   filename: Output file path.
 */
void SolutionSerializer::exportXML(const std::shared_ptr<ScheduleResult>& result,
                                  const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not create file: " + filename);
    }
    
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<jssp_solution>\n";
    
    // Problem metadata
    file << "  <problem>\n";
    file << "    <numJobs>" << result->problem.numJobs << "</numJobs>\n";
    file << "    <numMachines>" << result->problem.numMachines << "</numMachines>\n";
    file << "    <totalOperations>" << result->problem.getTotalOperations() << "</totalOperations>\n";
    file << "  </problem>\n\n";
    
    // Operations
    file << "  <operations>\n";
    for (const auto& job : result->problem.jobs) {
        for (const auto& operation : job->operations) {
            file << "    <operation>\n";
            file << "      <jobId>" << operation->jobId << "</jobId>\n";
            file << "      <machineId>" << operation->machineId << "</machineId>\n";
            file << "      <processingTime>" << operation->processingTime << "</processingTime>\n";
            file << "      <operationId>" << operation->operationId << "</operationId>\n";
            file << "      <startTime>" << operation->startTime << "</startTime>\n";
            file << "      <endTime>" << operation->endTime << "</endTime>\n";
            file << "      <scheduled>" << (operation->isScheduled() ? "true" : "false") << "</scheduled>\n";
            file << "    </operation>\n";
        }
    }
    file << "  </operations>\n\n";
    
    // Machines
    file << "  <machines>\n";
    for (const auto& machine : result->problem.machines) {
        file << "    <machine>\n";
        file << "      <machineId>" << machine->machineId << "</machineId>\n";
        file << "      <availableTime>" << machine->availableTime << "</availableTime>\n";
        file << "      <scheduledOperations>\n";
        for (const auto& operation : machine->scheduledOperations) {
            file << "        <scheduledOperation>\n";
            file << "          <jobId>" << operation->jobId << "</jobId>\n";
            file << "          <operationId>" << operation->operationId << "</operationId>\n";
            file << "          <startTime>" << operation->startTime << "</startTime>\n";
            file << "          <endTime>" << operation->endTime << "</endTime>\n";
            file << "        </scheduledOperation>\n";
        }
        file << "      </scheduledOperations>\n";
        file << "    </machine>\n";
    }
    file << "  </machines>\n\n";
    
    // Metrics
    file << "  <metrics>\n";
    file << "    <makespan>" << result->makespan << "</makespan>\n";
    file << "    <totalCompletionTime>" << result->totalCompletionTime << "</totalCompletionTime>\n";
    file << "    <averageFlowTime>" << result->avgFlowTime << "</averageFlowTime>\n";
    file << "  </metrics>\n";
    
    file << "</jssp_solution>\n";
    file.close();
}

/**
 * Detects format from filename extension.
 *
 * Args:
 *   filename: File path.
 *
 * Returns:
 *   Detected export format.
 */
ExportFormat SolutionSerializer::detectFormat(const std::string& filename) {
    std::string ext = filename.substr(filename.find_last_of('.') + 1);
    if (ext == "json" || ext == "JSON") {
        return ExportFormat::JSON;
    } else if (ext == "xml" || ext == "XML") {
        return ExportFormat::XML;
    } else {
        return ExportFormat::TEXT; // Default for .txt or any other extension
    }
}

/**
 * Gets format name for display.
 *
 * Args:
 *   format: Export format.
 *
 * Returns:
 *   Format name string.
 */
std::string SolutionSerializer::getFormatName(ExportFormat format) {
    switch (format) {
        case ExportFormat::TEXT: return "Text (.txt)";
        case ExportFormat::JSON: return "JSON (.json)";
        case ExportFormat::XML: return "XML (.xml)";
        default: return "Unknown";
    }
}
