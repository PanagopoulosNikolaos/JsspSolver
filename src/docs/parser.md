# Parser Documentation

## Overview
The Parser class is responsible for reading and writing JSSP (Job Shop Scheduling Problem) problem instances and solution files. It handles multiple file formats and provides functionality to parse problem data from various sources, including text files, strings, and generated test problems.

## Class Definition
```cpp
class Parser {
public:
    static std::shared_ptr<ProblemInstance> parseFile(const std::string& filename);
    static std::shared_ptr<ProblemInstance> parseString(const std::string& data);
    static void saveToFile(const std::shared_ptr<ProblemInstance>& problem, const std::string& filename);
    static std::shared_ptr<ProblemInstance> generateSimpleProblem();
    static std::shared_ptr<ScheduleResult> loadSolution(const std::string& filename);
    static std::shared_ptr<ScheduleResult> loadTextSolution(const std::string& filename);
    static std::shared_ptr<ScheduleResult> loadJSONSolution(const std::string& filename);
    static std::shared_ptr<ScheduleResult> loadXMLSolution(const std::string& filename);
    static int extractIntFromXML(const std::string& content, const std::string& tag);
    static double extractDoubleFromXML(const std::string& content, const std::string& tag);
};
```

## Key Functions

### File Parsing
- `parseFile()`: Reads a JSSP problem from a file in the standard format, validating the input and creating the appropriate data structures
- `parseString()`: Parses problem data from a string, useful for testing or programmatic generation

### Problem Generation
- `generateSimpleProblem()`: Creates a hardcoded simple test problem with 3 jobs and 3 machines for testing purposes
- `saveToFile()`: Saves a problem instance to a file for debugging or sharing

### Solution Loading
- `loadSolution()`: Detects the format of a solution file (TEXT, JSON, or XML) and calls the appropriate parser
- `loadTextSolution()`: Parses a solution from a human-readable text format
- `loadJSONSolution()`: Parses a solution from JSON format
- `loadXMLSolution()`: Parses a solution from XML format

### Helper Functions
- `extractIntFromXML()` and `extractDoubleFromXML()`: Helper functions for extracting values from XML content

## File Format Specifications

### JSSP Input Format
The parser expects files with the following format:
- First line: Number of jobs, number of machines
- Following lines: Job ID, Machine ID, Processing time for each operation

### Solution Output Formats
The parser supports three output formats for solutions:
1. Text format: Human-readable format with clear sections for problem metadata, scheduling results, machine schedules, and performance metrics
2. JSON format: Structured data format suitable for programmatic processing
3. XML format: Standard markup format for interoperability

## Error Handling
The parser includes comprehensive error handling for:
- Missing or invalid input files
- Invalid problem data (negative numbers, out-of-range IDs)
- Malformed solution files
- Unsupported file formats

## Usage
The parser is typically used at the beginning of the workflow to load problem instances, and at the end to save or load solution results. It provides a unified interface for handling different data formats while maintaining consistency in the internal data structures.