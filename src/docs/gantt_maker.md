# Gantt Maker Documentation

## Overview
The GanttChartMaker class is responsible for creating and displaying visual representations of job shop scheduling problems. It generates Gantt charts that visualize the scheduling of operations across multiple machines over time, providing a clear visual representation of how jobs are allocated to machines.

## Class Definition
```cpp
class GanttChartMaker {
private:
    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded;
    std::vector<sf::Color> jobColors;
    float marginLeft, marginTop, marginRight, marginBottom;
    float rowHeight, timeScale, machineLabelWidth;

public:
    GanttChartMaker();
    ~GanttChartMaker();
    bool loadFont();
    sf::Color getJobColor(int jobId);
    void drawGrid(float startX, float startY, int maxTime, int numMachines);
    void drawGridToTexture(sf::RenderTexture& target, float startX, float startY, int maxTime, int numMachines);
    void drawTimeAxisToTexture(sf::RenderTexture& target, float startX, float startY, int maxTime);
    void drawMachineLabelsToTexture(sf::RenderTexture& target, float startX, float startY, std::shared_ptr<ProblemInstance> problem);
    void drawOperationsToTexture(sf::RenderTexture& target, float startX, float startY, std::shared_ptr<ScheduleResult> result);
    void drawTimeAxis(float startX, float startY, int maxTime);
    void drawMachineLabels(float startX, float startY, std::shared_ptr<ProblemInstance> problem);
    void drawOperations(float startX, float startY, std::shared_ptr<ScheduleResult> result);
    void displaySchedule(std::shared_ptr<ScheduleResult> result);
    void saveToFile(std::shared_ptr<ScheduleResult> result, const std::string& filename);
    void setWindowSize(unsigned int width, unsigned int height);
    void setTimeScale(float scale);
    void setRowHeight(float height);
    bool isOpen() const;
    void pollEvents();
    void close();
};
```

## Key Functions

### Constructor and Destructor
The constructor initializes the SFML window, sets up default margins and dimensions, and defines a collection of distinct colors for representing different jobs. The destructor ensures proper cleanup of the window resource.

### Font Loading
The `loadFont()` function attempts to load a system font from common locations. It tries multiple potential font paths and returns a boolean indicating success or failure.

### Drawing Functions
The class provides various drawing functions for different elements of the Gantt chart:
- `drawGrid()`: Creates the grid lines separating machines and time intervals
- `drawTimeAxis()`: Adds time labels along the horizontal axis
- `drawMachineLabels()`: Labels each machine row
- `drawOperations()`: Renders the scheduled operations as colored rectangles

### Display and Export
- `displaySchedule()`: Shows the Gantt chart in the SFML window with all elements
- `saveToFile()`: Saves the chart as an image file, handling the rendering to a texture first

### Utility Functions
- `setWindowSize()`, `setTimeScale()`, `setRowHeight()`: Allow customization of the chart appearance
- `pollEvents()`: Handles window events like closing the window
- `getJobColor()`: Returns a specific color for each job to ensure visual distinction

## Usage
This class is typically used after solving a scheduling problem to visualize the results. It takes a ScheduleResult object containing the problem data and the computed schedule, then renders a visual representation showing when each operation is scheduled on each machine.