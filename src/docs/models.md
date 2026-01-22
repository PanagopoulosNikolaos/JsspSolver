# Models Documentation

## Overview
The models.cpp file is part of the Job Shop Scheduling Problem (JSSP) solver and contains the core data structures used throughout the application. Although this file exists, the actual model implementations are defined inline in the corresponding header file (models.hpp) for performance and simplicity reasons.

## File Contents
```cpp
#include "models.hpp"

// Models implementation - Currently header-only design, but keeping file for consistency
// All model classes are implemented inline in the header file for simplicity and performance
```

## Design Pattern
The implementation follows a header-only design pattern where all class definitions and method implementations are contained within the header file (models.hpp). This approach provides several benefits:
- Improved performance through inlining of methods
- Simplified compilation process
- Better optimization opportunities for the compiler

## Core Model Classes
While the implementations are in the header file, the models likely include:

### ProblemInstance
Represents the entire JSSP problem with jobs, machines, and operations.

### Job
Represents a single job consisting of multiple operations that must be processed in sequence.

### Operation
Represents a single task that must be executed on a specific machine for a specified duration.

### Machine
Represents a machine that can process operations with capacity constraints.

### ScheduleResult
Contains the results of the scheduling algorithm including the makespan and other performance metrics.

## Reasoning for Header-Only Design
The comment indicates that the implementation is header-only for simplicity and performance. This design choice is common in applications where:
- Performance is critical
- Templates are heavily used
- The codebase is not too large to cause compilation time issues

## Dependencies
- models.hpp: Contains the actual implementations of the model classes