# JSSP Solver User Interface

This directory contains the graphical user interface implementation for the Job Shop Scheduling Problem (JSSP) solver application.

## Overview

The UI is built using SFML (Simple and Fast Multimedia Library) and provides an interactive dashboard for:

- Selecting problem instances from available .jssp files
- Choosing scheduling algorithms (FIFO, SPT, LPT)
- Visualizing schedules with interactive Gantt charts
- Exporting solutions in various formats
- Loading and saving solution data with native file dialogs

## Architecture Integration

The UI follows the traditional C++ library structure where:
- **Header files** (`include/`) define the UI interfaces and class declarations
- **UI implementation** (`ui/`) contains the SFML-based UI logic
- **Core logic** (`src/`) contains business logic implementations

This separation provides:
- **Clean interface definition** - UI behavior is clearly defined in headers
- **Implementation flexibility** - UI logic can be modified without affecting other components
- **Easier testing** - UI interfaces can be mocked for unit testing
- **Better maintainability** - Clear separation between UI and business logic

## Architecture

### Core Components

- **`base_ui.hpp`** (in `include/`) - Main UI class definition with SFML rendering and event handling
- **`base_ui.cpp`** (in `ui/`) - Implementation of the UI logic, layout management, and user interactions

### Key Classes

#### BaseUI
The main UI controller that manages:
- Window creation and rendering loop
- Event handling (mouse clicks, keyboard input)
- Layout management (header, sidebar, main area)
- State management (current view, selected files/algorithms)
- Integration with solver and visualization components

#### UI Elements
- **Buttons**: Interactive elements for file selection, algorithm choice, and actions
- **Console**: Text output area for logging messages and results
- **Gantt Chart Display**: Interactive visualization of the scheduling solution
- **Header Bar**: Application title and navigation
- **Sidebar**: Control panel for selecting options

### View Modes

1. **Output View**: Displays console output, problem details, and solution metrics
2. **Gantt Chart View**: Shows the interactive Gantt chart visualization

## Dependencies

- SFML 2.5+ (Graphics, Window, System modules)
- C++17 standard library
- Integration with core application components:
  - `models.hpp` - Data structures
  - `solver.hpp` - Scheduling algorithms
  - `gantt_maker.hpp` - Visualization
  - `parser.hpp` - File parsing

## Building and Running

The UI is automatically included when building the main application:

```bash
mkdir build
cd build
cmake ..
make
./JSPSolver
```

### Prerequisites

Ensure SFML development libraries are installed:

```bash
# Ubuntu/Debian
sudo apt-get install libsfml-dev

# CentOS/RHEL
sudo yum install sfml-devel

# macOS
brew install sfml
```

## Usage

### Basic Workflow

1. **Launch Application**: Run `./JSPSolver`
2. **Select Problem**: Click on a .jssp file from the sidebar
3. **Choose Algorithm**: Select FIFO, SPT, or LPT from the algorithm buttons
4. **Solve**: Click the "Solve" button to generate the schedule
5. **View Results**: Switch between Output and Gantt Chart views
6. **Export**: Use export buttons to save solutions or images

### Interactive Features

- **File Selection**: Browse available problem files in the data directory
- **Algorithm Comparison**: Quickly switch between different scheduling strategies
- **Gantt Chart Interaction**: Zoom and pan the schedule visualization
- **File Dialogs**: Native system file dialogs for loading solution files
- **Export Options**:
  - PNG images of Gantt charts
  - Text summaries of solutions
  - JSON/XML structured data

### File Loading

The application now uses native system file dialogs for loading exported solutions:

- **Load Solution Button**: Opens a file selection dialog
- **Supported Formats**: .txt, .json, .xml files are filtered by default
- **Cross-Platform**: Uses system-native file dialogs (Zenity on Linux, native dialogs on Windows/macOS)
- **User Choice**: Users can select any solution file from any location, eliminating hard-coded paths

### Keyboard Shortcuts

- **Escape**: Close the application
- **F11**: Toggle fullscreen (if supported)
- Mouse wheel: Zoom in Gantt chart view

## Customization

### Layout Constants

Key dimensions are defined in `base_ui.hpp`:
- Header height: 60 pixels
- Sidebar width: 220 pixels
- Button sizes and spacing: Configurable in `initLayout()`

### Styling

- Font loading: Attempts to load system fonts (Arial, sans-serif fallback)
- Colors: Hard-coded for buttons, backgrounds, and text
- Layout: Responsive to window resizing

## Extension Points

### Adding New Algorithms

1. Add algorithm to `SchedulingAlgorithm` enum in `models.hpp`
2. Implement solver logic in `solver.cpp`
3. Add UI button in `BaseUI::initLayout()`
4. Update button click handlers

### Adding New Views

1. Extend `ViewMode` enum
2. Add view rendering method (e.g., `drawNewView()`)
3. Add navigation button in `initLayout()`
4. Implement view switching logic

### Custom Export Formats

1. Add export method to `BaseUI` class
2. Implement serialization in `solution_serializer.cpp`
3. Add UI button with click handler
4. Update file dialog for new format

## Troubleshooting

### Common Issues

1. **Font not loading**: Ensure system fonts are available
2. **SFML window not opening**: Check graphics drivers and SFML installation
3. **Buttons not responding**: Verify mouse position calculations
4. **Gantt chart not rendering**: Check problem data validity

### Debug Output

Enable console logging by checking the console output area in the UI for error messages and debug information.

## File Structure

```
ui/
└── base_ui.cpp         # Main UI implementation

include/
└── base_ui.hpp         # UI class declarations
```

## Integration

The UI integrates with all core components:
- **Parser**: Loads problem files selected by user
- **Solver**: Executes algorithms chosen by user
- **Gantt Maker**: Displays visualization of results
- **Models**: Uses data structures for state management
- **Solution Serializer**: Handles export functionality

## Performance Considerations

- SFML rendering is optimized for real-time interaction
- Gantt chart drawing scales with problem size
- Button hit detection uses efficient bounding box checks
- Memory management uses smart pointers to prevent leaks
