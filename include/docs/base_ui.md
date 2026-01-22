# BaseUI Documentation

## Overview
The BaseUI class is the core GUI application class that handles user interface and interactions for the JSSP (Job Shop Scheduling Problem) solver. It provides a visual interface for loading problem files, selecting algorithms, solving problems, and displaying results including Gantt charts.

## Key Features
- Interactive GUI using SFML for graphics
- File browsing and loading functionality
- Support for multiple scheduling algorithms
- Console output display
- Gantt chart visualization
- Solution export/import capabilities

## Dependencies
```cpp
#include "models.hpp"
#include "solver.hpp"
#include "gantt_maker.hpp"
#include "parser.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <memory>
#include <string>
#include <functional>
```

## Enumerations
### ViewMode
Controls the display mode of the UI:
- `Output`: Display console output
- `GanttChart`: Display Gantt chart visualization

## Class Members

### Private Members
- `window`: SFML render window for the UI
- `font`: Font used for UI elements
- `fontLoaded`: Boolean indicating if font was loaded successfully
- `colorBg`, `colorSidebar`, `colorHeader`, `colorAccent`, `colorTextMain`, `colorTextDim`: Theme colors for UI
- `currentView`: Current view mode (output or Gantt chart)
- `selectedFile`: Currently selected problem file
- `selectedAlgo`: Selected scheduling algorithm
- `consoleLines`: Vector storing console output lines
- `currentProblem`: Shared pointer to the current problem instance
- `currentResult`: Shared pointer to the current schedule result
- `headerHeight`, `sidebarWidth`: Layout constants
- `fileButtons`, `algoButtons`, `navButtons`: Collections of UI buttons
- `dropdownOpen`, `dropdownButton`, `dropdownItems`, `availableFiles`: Dropdown menu state
- `fileScrollOffset`: Scroll offset for file list

### Public Methods
- `BaseUI()`: Constructor initializes the UI
- `~BaseUI()`: Destructor cleans up resources
- `run()`: Main UI loop
- `loadFile(filename)`: Load a problem file
- `solve()`: Solve the loaded problem
- `showMessage(title, message)`: Display a message dialog
- `exportGanttChartInteractive()`: Export Gantt chart interactively
- `exportSolutionInteractive()`: Export solution interactively
- `loadSolutionInteractive()`: Load solution interactively
- `loadSolutionFromFile(filename)`: Load solution from file
- `browseForFile()`: Open file browser dialog

### Private Helper Methods
- `loadFont()`: Load font for UI elements
- `initLayout()`: Initialize UI layout
- `handleInput()`: Handle user input events
- `update(mousePos)`: Update UI state
- `draw()`: Render the UI
- `drawHeader()`: Draw header section
- `drawSidebar()`: Draw sidebar
- `drawMainArea()`: Draw main area
- `drawConsole()`: Draw console output
- `drawGanttInMain()`: Draw Gantt chart in main area
- `logToConsole(message)`: Log message to console
- `createButton(container, label, pos, size, action, isAction)`: Create UI button

## Usage Example
```cpp
BaseUI ui;
ui.run();  // Starts the main UI loop
```

The BaseUI class integrates with other components of the JSSP solver to provide a complete user experience for solving job shop scheduling problems.