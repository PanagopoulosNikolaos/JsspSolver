# Source Files - JSSP Solver Implementation

This directory contains the implementation files (.cpp) for all core components of the Job Shop Scheduling Problem (JSSP) solver application.

## Overview

Each source file implements the corresponding header file from the `include/` directory, providing concrete implementations of classes, methods, and algorithms. This traditional C++ library structure separates interface (headers) from implementation (source files), providing:

- **Faster compilation** - Changes to implementation don't require recompiling all dependent files
- **Better encapsulation** - Implementation details are hidden from users
- **Library potential** - Can create precompiled libraries for distribution
- **Maintainability** - Clear separation of interface and implementation

The implementations follow C++17 standards with emphasis on performance, memory safety, and maintainability.

## Core Implementations

### main.cpp
**Purpose**: Application entry point and main execution flow.

**Key Functions**:
- `main()`: Initializes the application and starts the UI
- Exception handling for fatal errors
- Application lifecycle management

**Features**:
- Clean startup and shutdown procedures
- Global exception catching
- Integration with SFML UI framework

### models.cpp
**Purpose**: Implementation of core data structures and their methods.

**Key Implementations**:
- **`Operation` methods**: Scheduling logic and state management
- **`Job` methods**: Operation management and sequencing
- **`Machine` methods**: Scheduling operations and availability tracking
- **`ProblemInstance` methods**: Job and machine creation, access control
- **`ScheduleResult` methods**: Performance metric calculations

**Features**:
- Smart pointer memory management
- Efficient data structures for large problem instances
- Validation and error checking

### solver.cpp
**Purpose**: Implementation of scheduling algorithms and solver factory.

**Key Implementations**:
- **`Solver` base class**: Abstract interface for all algorithms
- **Algorithm implementations**: FIFO, SPT, LPT concrete classes
- **`Solver::createSolver()`**: Factory method for algorithm instantiation

**Algorithms**:
- **FIFO (First In, First Out)**: Processes operations in arrival order
- **SPT (Shortest Processing Time)**: Prioritizes shortest operations first
- **LPT (Longest Processing Time)**: Prioritizes longest operations first

**Performance Characteristics**:
- Time complexity: O(n log n) for sorting operations
- Space complexity: O(n) for operation storage
- Efficient for problems up to 1000+ operations

### parser.cpp
**Purpose**: Implementation of file parsing and data validation.

**Key Implementations**:
- **`Parser::parseFile()`**: File I/O and format validation
- **`Parser::parseString()`**: String-based parsing for testing
- Error handling and recovery mechanisms

**Supported Features**:
- Standard .jssp file format parsing
- Data validation and constraint checking
- Error reporting with detailed messages
- Memory-efficient parsing for large files

### gantt_maker.cpp
**Purpose**: Implementation of Gantt chart visualization and export.

**Key Implementations**:
- **`GanttMaker` constructor**: Chart layout and scaling
- **Color generation**: Job-specific color assignment
- **`draw()` methods**: Chart rendering logic
- **`saveToFile()`**: PNG export functionality

**Visualization Features**:
- Scalable chart rendering
- Job color coding
- Time axis scaling
- Interactive display capabilities

### solution_serializer.cpp
**Purpose**: Implementation of solution export in multiple formats.

**Key Implementations**:
- **`SolutionSerializer` methods**: Format-specific serialization
- **Text export**: Human-readable schedule summaries
- **JSON export**: Structured data for programmatic use
- **XML export**: Alternative structured format

**Features**:
- Multiple output formats
- Comprehensive solution data export
- File system integration
- Error handling for I/O operations

## Architecture and Design

### Code Organization
- **Single Responsibility**: Each file implements one primary component
- **Dependency Injection**: Clear separation between interface and implementation
- **Error Handling**: Comprehensive exception handling and validation

### Performance Considerations
- **Algorithm Efficiency**: Optimized scheduling algorithms
- **Memory Usage**: Smart pointers prevent leaks
- **I/O Optimization**: Efficient file parsing and export
- **Scalability**: Handles varying problem sizes

### Thread Safety
- **Single-threaded Design**: Current implementation is not thread-safe
- **Future Extensions**: Designed for potential multi-threading support

## Dependencies and Linking

### Internal Dependencies
- Each .cpp file includes its corresponding .hpp file
- Cross-component dependencies managed through headers
- Minimal circular dependencies

### External Libraries
- **SFML**: Linked for graphics and UI (used in main.cpp indirectly)
- **nlohmann/json**: Used in solution_serializer.cpp for JSON output
- **Standard Library**: C++17 features extensively used

### Build Integration
Files are compiled and linked via CMake:
```cmake
add_executable(JSPSolver
    src/main.cpp
    src/models.cpp
    src/parser.cpp
    src/solver.cpp
    src/gantt_maker.cpp
    src/solution_serializer.cpp
    ui/base_ui.cpp
)
```

## Testing and Validation

### Unit Testing
Each implementation is thoroughly tested in `tests/`:
- `test_models.cpp`: Data structure validation
- `test_solver.cpp`: Algorithm correctness
- `test_parser.cpp`: File parsing accuracy
- `test_gantt_maker.cpp`: Visualization components

### Integration Testing
- End-to-end workflows tested in `test_integration.cpp`
- Performance regression monitoring
- Memory leak detection

## File Structure

```
src/
├── README.md                 # This documentation
├── main.cpp                 # Application entry point
├── models.cpp               # Data structure implementations
├── solver.cpp               # Algorithm implementations
├── parser.cpp               # File parsing logic
├── gantt_maker.cpp          # Visualization components
└── solution_serializer.cpp  # Export functionality
```

## Development Guidelines

### Code Style
- **C++17 Standards**: Modern language features used appropriately
- **RAII Pattern**: Resource management through constructors/destructors
- **Const Correctness**: Proper use of const qualifiers
- **Documentation**: Inline comments for complex logic

### Error Handling
- **Exceptions**: Used for recoverable errors
- **Assertions**: Used for programming errors (debug builds)
- **Validation**: Input validation at component boundaries

### Performance Optimization
- **Algorithm Selection**: Appropriate data structures chosen
- **Memory Efficiency**: Minimal allocations in hot paths
- **Cache Awareness**: Data locality considerations

## Extension Points

### Adding New Algorithms
1. Implement new solver class in `solver.cpp`
2. Add factory case in `Solver::createSolver()`
3. Update header declarations
4. Add corresponding tests

### Adding New Export Formats
1. Extend `SolutionSerializer` in `solution_serializer.cpp`
2. Implement format-specific logic
3. Update header interface
4. Add UI integration if needed

### Performance Improvements
1. Profile existing code to identify bottlenecks
2. Optimize data structures and algorithms
3. Consider parallel processing for large problems
4. Update tests to maintain performance baselines

## Maintenance Notes

### Version Compatibility
- Code designed for C++17 compatibility
- SFML version dependencies clearly specified
- JSON library version requirements documented

### Debugging Support
- Comprehensive logging in debug builds
- Exception messages with context
- Test framework integration for regression detection

### Future Enhancements
- Potential for multi-threading in solver algorithms
- GPU acceleration for visualization
- Network capabilities for distributed solving
