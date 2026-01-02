# JSSP Solver Header Files

This directory contains all header files (.hpp) for the Job Shop Scheduling Problem (JSSP) solver project. These headers define the interfaces and class declarations that are implemented in the corresponding source files.

## Overview

The header files establish the public API for the JSSP solver library, providing:

- **Class declarations** for all core components
- **Method signatures** defining the interface contracts
- **Type definitions** including enums and data structures
- **Documentation** for proper usage and extension

## Core Components

### models.hpp
**Purpose**: Core data structures and classes for representing JSSP problems and solutions.

**Key Classes**:
- **`Operation`**: Represents a single processing operation
- **`Job`**: Contains a sequence of operations for a job
- **`Machine`**: Manages scheduling of operations on a machine
- **`ProblemInstance`**: Complete problem definition with jobs and machines
- **`ScheduleResult`**: Contains the solution and performance metrics

### solver.hpp
**Purpose**: Scheduling algorithm interfaces and factory methods.

**Key Classes**:
- **`Solver`**: Main solver class with algorithm selection
- **`SchedulingAlgorithm` enum**: Defines available algorithms (FIFO, SPT, LPT)

**Factory Methods**:
- `createFIFOSolver()`, `createSPTSolver()`, `createLPTSolver()`
- `getAlgorithmName()` for display purposes

### parser.hpp
**Purpose**: File parsing and data import/export interfaces.

**Key Methods**:
- `parseFile()`: Load problem from .jssp file
- `parseString()`: Parse problem from string data
- `saveToFile()`: Export problem to file
- `generateSimpleProblem()`: Create sample problems
- `loadSolution()`: Import saved solutions in various formats

### gantt_maker.hpp
**Purpose**: Gantt chart visualization interfaces.

**Key Classes**:
- **`GanttChartMaker`**: Handles chart rendering and export
- **`GanttOperation` struct**: Chart-specific operation data

**Features**:
- Interactive chart display
- PNG export functionality
- Scalable time axis and job coloring

### solution_serializer.hpp
**Purpose**: Solution export in multiple formats.

**Key Classes**:
- **`SolutionSerializer`**: Static methods for different formats
- **`ExportFormat` enum**: Supported formats (TEXT, JSON, XML)

**Export Methods**:
- `exportText()`, `exportJSON()`, `exportXML()`
- `detectFormat()` based on file extension

### base_ui.hpp
**Purpose**: Graphical user interface framework.

**Key Classes**:
- **`BaseUI`**: Main UI controller with SFML integration
- **`ViewMode` enum**: UI view modes (Output, GanttChart)

**Features**:
- SFML-based rendering and event handling
- File dialogs for loading/saving
- Interactive buttons and console output

## Usage Examples

### Creating Problem Instances
```cpp
#include "models.hpp"

auto problem = std::make_shared<ProblemInstance>();
problem->createJobs(3);
problem->createMachines(3);
// Add operations to jobs...
```

### Using the Solver
```cpp
#include "solver.hpp"

auto solver = std::make_shared<Solver>(SchedulingAlgorithm::SPT);
auto result = solver->solve(problem);
```

### File Parsing
```cpp
#include "parser.hpp"

auto problem = Parser::parseFile("data/simple_3x3.jssp");
```

## File Structure

```
include/
├── README.md                 # This documentation
├── models.hpp               # Core data structures
├── solver.hpp               # Algorithm interfaces
├── parser.hpp               # File parsing
├── gantt_maker.hpp          # Visualization
├── solution_serializer.hpp  # Export functionality
└── base_ui.hpp              # UI framework
```

## Extension Guidelines

### Adding New Algorithms
1. Extend `SchedulingAlgorithm` enum in `solver.hpp`
2. Add implementation in `solver.cpp`
3. Create factory method in `Solver` class
4. Update UI integration in `base_ui.cpp`

### Adding New Data Fields
1. Modify appropriate model classes in `models.hpp`
2. Update dependent components
3. Maintain backward compatibility
4. Update serialization methods

### Adding New Export Formats
1. Extend `ExportFormat` enum in `solution_serializer.hpp`
2. Implement new format method in `solution_serializer.cpp`
3. Update UI integration if needed
4. Add file extension handling

## Testing Integration

All header-defined interfaces are thoroughly tested in the `tests/` directory:
- Unit tests for each class and method
- Integration tests for component interaction
- Performance tests for algorithm efficiency
- Edge case and error condition testing

## Dependencies

- **C++17 Standard Library**: Modern language features
- **SFML Headers**: Referenced in `base_ui.hpp` (linked at compile time)
- **nlohmann/json Headers**: Referenced in `solution_serializer.hpp`
- **Memory Management**: Uses `std::shared_ptr` for object lifetime
