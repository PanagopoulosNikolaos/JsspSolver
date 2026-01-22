# GanttChartMaker Documentation

## Overview
The GanttChartMaker class is responsible for creating and displaying Gantt charts, which are visual representations of job shop scheduling solutions. It uses SFML for rendering and provides both interactive display and file export capabilities for the generated charts.

## Key Features
- Interactive Gantt chart display
- Configurable layout parameters
- Export functionality to image files
- Automatic color assignment for different jobs
- Scrollable and zoomable interface
- Grid and time axis rendering

## Dependencies
```cpp
#include "models.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
```

## Data Structures
### GanttOperation
A struct representing a single operation in the Gantt chart:
- `operation`: Shared pointer to the Operation object
- `rectangle`: SFML rectangle shape for visual representation
- `label`: SFML text for operation labeling
- `color`: Color assigned to the operation

## Class Members

### Private Members
- `window`: SFML render window for the chart
- `view`: SFML view for camera control
- `marginLeft`, `marginTop`, `marginRight`, `marginBottom`: Layout margins
- `rowHeight`: Height of each machine row
- `timeScale`: Scale factor for time axis
- `machineLabelWidth`: Width allocated for machine labels
- `font`: Font used for text elements
- `fontLoaded`: Boolean indicating if font was loaded successfully
- `jobColors`: Vector of colors for different jobs

### Public Methods
- `GanttChartMaker()`: Constructor initializes the chart maker
- `~GanttChartMaker()`: Destructor cleans up resources
- `displaySchedule(result)`: Display the Gantt chart for a schedule result
- `saveToFile(result, filename)`: Save the Gantt chart to a file
- `setWindowSize(width, height)`: Set the window size
- `setTimeScale(scale)`: Set the time scale for the chart
- `setRowHeight(height)`: Set the row height for machines
- `getJobColor(jobId)`: Get the color for a specific job
- `isOpen()`: Check if the window is open
- `pollEvents()`: Poll for window events
- `close()`: Close the window

### Private Helper Methods
- `drawTimeAxis(startX, startY, maxTime)`: Draw the time axis
- `drawMachineLabels(startX, startY, problem)`: Draw machine labels
- `drawOperations(startX, startY, result)`: Draw operations on the chart
- `loadFont()`: Load the font
- `drawGrid(startX, startY, maxTime, numMachines)`: Draw the grid
- `drawGridToTexture(target, startX, startY, maxTime, numMachines)`: Draw grid to texture
- `drawTimeAxisToTexture(target, startX, startY, maxTime)`: Draw time axis to texture
- `drawMachineLabelsToTexture(target, startX, startY, problem)`: Draw machine labels to texture
- `drawOperationsToTexture(target, startX, startY, result)`: Draw operations to texture

## Usage Example
```cpp
GanttChartMaker ganttMaker;
std::shared_ptr<ScheduleResult> result = /* ... */;
ganttMaker.displaySchedule(result);  // Display the chart
ganttMaker.saveToFile(result, "gantt_chart.png");  // Save to file
```

The GanttChartMaker class works with the models defined in models.hpp to visualize scheduling solutions in an intuitive format.