# Parser Documentation

## Overview
The Parser class is responsible for parsing JSSP (Job Shop Scheduling Problem) problem instances from various input formats and saving them. It supports multiple input formats including text files and string data, and can also generate simple test problems programmatically. Additionally, it handles loading of solutions from different formats (TEXT, JSON, XML).

## Key Features
- Parse JSSP problems from file or string
- Support for multiple input formats
- Generate simple test problems programmatically
- Load solutions from different formats (TEXT, JSON, XML)
- Save problem instances for debugging/testing

## Dependencies
```cpp
#include "models.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
```

## Class Methods

### Public Methods

#### `parseFile(filename)`
Parses a JSSP instance from a file.
- **Parameters**: `filename` - Path to input file
- **Returns**: Parsed problem instance
- **Format**: First line contains "num_jobs num_machines", followed by lines with "job_id machine_id processing_time" for each operation

#### `parseString(data)`
Parses a problem instance from string format.
- **Parameters**: `data` - String containing problem data
- **Returns**: Parsed problem instance

#### `saveToFile(problem, filename)`
Saves a problem instance to file for debugging/testing.
- **Parameters**: 
  - `problem` - Problem instance to save
  - `filename` - Output file path

#### `generateSimpleProblem()`
Generates a simple test problem programmatically.
- **Returns**: Generated problem instance

#### `loadSolution(filename)`
Loads a solution from a file. Supports TEXT, JSON, and XML formats.
- **Parameters**: `filename` - Path to solution file
- **Returns**: Loaded schedule result

#### `loadTextSolution(filename)`
Loads a solution from TEXT format.
- **Parameters**: `filename` - Path to TEXT solution file
- **Returns**: Loaded schedule result

#### `loadJSONSolution(filename)`
Loads a solution from JSON format.
- **Parameters**: `filename` - Path to JSON solution file
- **Returns**: Loaded schedule result

#### `loadXMLSolution(filename)`
Loads a solution from XML format.
- **Parameters**: `filename` - Path to XML solution file
- **Returns**: Loaded schedule result

### Private Methods

#### `extractIntFromXML(content, tag)`
Helper function to extract integer from XML tag.
- **Parameters**: 
  - `content` - XML content string
  - `tag` - XML tag name
- **Returns**: Extracted integer value

#### `extractDoubleFromXML(content, tag)`
Helper function to extract double from XML tag.
- **Parameters**: 
  - `content` - XML content string
  - `tag` - XML tag name
- **Returns**: Extracted double value

## Input File Format
The parser expects JSSP problem files in the following format:
- First line: `num_jobs num_machines`
- Following lines: `job_id machine_id processing_time` (one line per operation)

## Usage Example
```cpp
// Parse a problem from file
auto problem = Parser::parseFile("problem.jssp");

// Parse a problem from string
std::string problemData = "2 2\n0 0 3\n0 1 2\n1 1 4\n1 0 3";
auto problemFromString = Parser::parseString(problemData);

// Generate a simple test problem
auto simpleProblem = Parser::generateSimpleProblem();

// Load a solution from file
auto solution = Parser::loadSolution("solution.json");

// Save a problem to file
Parser::saveToFile(problem, "saved_problem.jssp");
```

The Parser class serves as the primary interface for input/output operations in the JSSP solver system, allowing for flexible problem definition and solution handling.