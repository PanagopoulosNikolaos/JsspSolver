# Solution Serializer Documentation

## Overview
The SolutionSerializer class provides functionality for exporting scheduling solutions in multiple formats (TEXT, JSON, and XML). It enables users to save the results of the JSSP solver in various formats for storage, sharing, or further processing.

## Class Definition
```cpp
enum class ExportFormat {
    TEXT,
    JSON,
    XML
};

class SolutionSerializer {
public:
    static void exportSolution(const std::shared_ptr<ScheduleResult>& result,
                              const std::string& filename,
                              ExportFormat format);
    static void exportText(const std::shared_ptr<ScheduleResult>& result,
                          const std::string& filename);
    static void exportJSON(const std::shared_ptr<ScheduleResult>& result,
                          const std::string& filename);
    static void exportXML(const std::shared_ptr<ScheduleResult>& result,
                         const std::string& filename);
    static ExportFormat detectFormat(const std::string& filename);
    static std::string getFormatName(ExportFormat format);
};
```

## Key Functions

### Main Export Function
- `exportSolution()`: The primary export function that takes a ScheduleResult, filename, and export format, then delegates to the appropriate format-specific function

### Format-Specific Export Functions
- `exportText()`: Exports the solution in a human-readable text format with clear sections for problem metadata, scheduling results, machine schedules, and performance metrics
- `exportJSON()`: Exports the solution in structured JSON format suitable for programmatic processing and integration with other systems
- `exportXML()`: Exports the solution in XML format for interoperability with other applications and systems

### Utility Functions
- `detectFormat()`: Determines the export format based on the file extension of the provided filename
- `getFormatName()`: Returns a user-friendly name for a given export format

## Export Format Details

### Text Format
The text format is designed for human readability with clearly labeled sections:
- Problem metadata (number of jobs, machines, total operations)
- Scheduling results organized by job
- Machine schedules showing operations assigned to each machine
- Performance metrics including makespan, total completion time, and average flow time

### JSON Format
The JSON format organizes data hierarchically:
- Problem metadata section
- Operations array with detailed information about each operation
- Machines array with scheduled operations for each machine
- Metrics section with performance indicators

### XML Format
The XML format provides structured markup:
- Root element containing all solution data
- Problem section with metadata
- Operations section with individual operation details
- Machines section with scheduling information
- Metrics section with performance data

## Error Handling
The serializer includes error handling for:
- Null result pointers
- Unwritable files
- Invalid file paths
- Format detection issues

## Usage
The SolutionSerializer is typically used after solving a scheduling problem to save the results in one or more formats for later analysis, sharing with others, or integration with other systems. The format detection feature allows for flexible usage with different file extensions.