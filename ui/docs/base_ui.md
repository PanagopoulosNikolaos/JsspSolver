# BaseUI Class Documentation

## Overview

The BaseUI class is the core graphical user interface component of the JSSP (Job Shop Scheduling Problem) Solver application. It implements a modern, dark-themed dashboard that allows users to load problem files, select scheduling algorithms, solve problems, and visualize results through Gantt charts. Built using SFML (Simple and Fast Multimedia Library), the interface features a sidebar navigation system, console output display, and interactive visualization capabilities.

The UI follows a clean, organized layout with three main sections:
- Header: Displays application title and status information
- Sidebar: Contains file selection, algorithm selection, and action buttons
- Main Area: Shows either console output or Gantt chart visualization

## Dependencies

The BaseUI class depends on several other components of the JSSP Solver:
- `base_ui.hpp`: Main header file defining the class interface
- `solution_serializer.hpp`: For exporting solution files
- `gantt_maker.hpp`: For generating Gantt chart visualizations
- Standard C++ libraries: `<iostream>`, `<filesystem>`, `<cstdio>`, `<memory>`, `<string>`, `<array>`, `<cmath>`
- SFML graphics and window libraries

## Class Structure

### Private Member Variables

```cpp
sf::RenderWindow window;          // Main SFML rendering window
sf::Font font;                   // Font used for all text elements
bool fontLoaded;                 // Flag indicating if font was successfully loaded

// Theme Colors
const sf::Color colorBg = sf::Color(18, 18, 18);        // Dark background
const sf::Color colorSidebar = sf::Color(25, 25, 25);   // Sidebar background
const sf::Color colorHeader = sf::Color(33, 33, 33);    // Header background
const sf::Color colorAccent = sf::Color(0, 120, 215);   // Accent blue color
const sf::Color colorTextMain = sf::Color(240, 240, 240); // Primary text color
const sf::Color colorTextDim = sf::Color(150, 150, 150); // Secondary text color

// UI State
ViewMode currentView;             // Current display mode (console or Gantt)
std::string selectedFile;         // Currently selected problem file
SchedulingAlgorithm selectedAlgo; // Currently selected scheduling algorithm
std::vector<std::string> consoleLines; // Console output history

// Problem Data
std::shared_ptr<ProblemInstance> currentProblem; // Loaded problem instance
std::shared_ptr<ScheduleResult> currentResult;   // Computed schedule result

// Layout Constants
const float headerHeight = 70.f;  // Height of the header section
const float sidebarWidth = 260.f; // Width of the sidebar section

// UI Elements
struct Button {                   // Button structure definition
    sf::RectangleShape shape;     // Visual rectangle for the button
    sf::Text text;               // Text displayed on the button
    std::function<void()> action; // Callback function when clicked
    bool isSelected = false;     // Selection state indicator
    bool isAction = false;       // Action button style flag
};

std::vector<Button> fileButtons;   // Collection of file-related buttons
std::vector<Button> algoButtons;   // Collection of algorithm selection buttons
std::vector<Button> navButtons;    // Collection of navigation/action buttons

// Dropdown State
bool dropdownOpen;                 // Visibility state of file selection dropdown
Button dropdownButton;             // Main dropdown trigger button
std::vector<Button> dropdownItems; // Individual dropdown items
std::vector<std::string> availableFiles; // Available problem files in data directory

// Scrolling State
int fileScrollOffset;              // Vertical scroll offset for file list
```

### Enumerations

```cpp
enum class ViewMode {
    Output,      // Console output view
    GanttChart   // Gantt chart visualization view
};
```

## Constructor and Destructor

### BaseUI()

Initializes the UI with default settings, creates the main window, loads fonts, sets up the layout, and displays welcome messages.

```cpp
BaseUI::BaseUI() : currentView(ViewMode::Output), 
                   selectedAlgo(SchedulingAlgorithm::FIFO), 
                   fileScrollOffset(0), 
                   dropdownOpen(false) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(1280, 950), "JSSP Dashboard", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    
    loadFont();
    initLayout();
    logToConsole("Welcome to JSSP Solver Dashboard.");
    logToConsole("Select a file and algorithm from the sidebar, then click 'Solve'.");
}
```

The constructor sets up:
- An 8x antialiasing level for smooth graphics
- A window resolution of 1280x950 pixels
- A 60 FPS frame rate limit for consistent performance
- Default view mode to Output
- FIFO as the default scheduling algorithm
- Initial scroll offset to 0
- Dropdown closed by default

### ~BaseUI()

Default destructor with no specific cleanup required since SFML handles resource management automatically.

## Core Methods

### loadFont()

Attempts to load a font from common system paths across different operating systems. Returns true if successful, false otherwise.

```cpp
bool BaseUI::loadFont() {
    std::vector<std::string> fontPaths = {
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/TTF/arial.ttf",
        "/usr/share/fonts/liberation/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Helvetica.ttc", // macOS
        "C:\\Windows\\Fonts\\arial.ttf" // Windows
    };

    for (const auto& path : fontPaths) {
        if (font.loadFromFile(path)) {
            fontLoaded = true;
            return true;
        }
    }
    
    fontLoaded = false;
    std::cerr << "Warning: No suitable font found. Text will not be displayed." << std::endl;
    return false;
}
```

This method implements cross-platform font loading by checking common font locations on Linux, macOS, and Windows systems.

### createButton()

Creates and configures a button element with specified properties and adds it to the provided container.

```cpp
void BaseUI::createButton(std::vector<Button>& container, 
                         const std::string& label, 
                         sf::Vector2f pos, 
                         sf::Vector2f size, 
                         std::function<void()> action, 
                         bool isAction) {
    // Implementation creates button with appropriate styling and positioning
}
```

Parameters:
- `container`: Vector to store the created button
- `label`: Text to display on the button
- `pos`: Position coordinates for the button
- `size`: Size dimensions for the button
- `action`: Function to call when button is clicked
- `isAction`: Whether this is a primary action button (affects styling)

### initLayout()

Initializes the complete UI layout by creating all necessary buttons, loading available files, and setting up the visual structure.

Key responsibilities:
- Clears existing UI elements
- Discovers available .jssp files in the data directory
- Creates file selection dropdown
- Sets up algorithm selection buttons (FIFO, SPT, LPT)
- Creates action buttons (Solve, Export Gantt, Export Solution, Load Solution)
- Sets up view switching buttons (Console, Gantt)

### handleInput()

Processes all user input events including mouse clicks, keyboard input, and window events.

Handles:
- Window close events
- Mouse button presses for UI interaction
- Mouse wheel scrolling for file lists
- Window resizing events

The method implements sophisticated hit detection to determine which UI elements are being interacted with, including special handling for the dropdown menu system.

### update(sf::Vector2f mousePos)

Updates the visual state of UI elements based on mouse position, implementing hover effects and selection indicators.

```cpp
void BaseUI::update(sf::Vector2f mousePos) {
    // Updates button appearance based on mouse position
    // Implements hover effects for all button collections
    // Handles file list scrolling in the sidebar
}
```

### draw()

Renders the complete UI by drawing all visual elements in the correct order.

```cpp
void BaseUI::draw() {
    window.clear(colorBg);
    drawMainArea(); // Main content
    drawSidebar();  // Sidebar
    drawHeader();   // Header
    window.display();
}
```

### drawHeader()

Draws the top header section containing the application title and status information.

Features:
- Application title ("JSSP Solver")
- Theme information ("Dark Aqua Theme")
- Current file status display
- Themed background and borders

### drawSidebar()

Draws the left sidebar containing file selection, algorithm selection, and action buttons.

Organized into sections:
- File selection dropdown with available .jssp files
- Algorithm selection buttons (FIFO, SPT, LPT)
- Action buttons (Solve, Export, Load)
- View switching buttons (Console, Gantt)

### drawMainArea()

Conditionally draws the main content area based on the current view mode.

```cpp
void BaseUI::drawMainArea() {
    if (currentView == ViewMode::Output) {
        drawConsole();
    } else {
        drawGanttInMain();
    }
}
```

### drawConsole()

Displays the console output area with scrollable message history.

Features:
- Dark-themed background for console display
- Automatic scrolling to show recent messages
- Color-coded messages (errors in red, success in green, etc.)
- Maximum line count limitation to prevent memory issues

### drawGanttInMain()

Visualizes the schedule results as a Gantt chart in the main area.

Implementation details:
- Dynamic scaling based on available space and makespan
- Color-coded operations by job ID
- Time axis with grid lines and labels
- Machine tracks with operation rectangles
- Makespan information display

The Gantt chart implementation includes sophisticated color generation using HSV color space to ensure visually distinct colors for different jobs.

## Public Interface Methods

### run()

Main application loop that continuously handles input, updates state, and renders the UI.

```cpp
void BaseUI::run() {
    while (window.isOpen()) {
        handleInput();
        update(sf::Vector2f(sf::Mouse::getPosition(window)));
        draw();
    }
}
```

### loadFile(const std::string& filename)

Loads a JSSP problem file and prepares it for solving.

Features:
- Cross-directory searching (checks both "data" and "../data")
- Error handling for invalid files
- Console logging of loaded file information
- Result reset when new file is loaded

### solve()

Executes the selected scheduling algorithm on the currently loaded problem.

Process:
- Validates that a problem is loaded
- Shows solving progress in console
- Creates appropriate solver instance
- Computes schedule and stores results
- Switches view to Gantt chart upon completion

### logToConsole(const std::string& message)

Adds a message to the console output with formatting.

```cpp
void BaseUI::logToConsole(const std::string& message) {
    consoleLines.push_back("> " + message);
    if (consoleLines.size() > 100) consoleLines.erase(consoleLines.begin());
}
```

Implements automatic history management by maintaining only the most recent 100 messages.

### exportGanttChartInteractive()

Provides an interactive mechanism to export the current Gantt chart as a PNG image.

Features:
- Automatic directory creation in user's Pictures folder
- Timestamp-based filenames to prevent conflicts
- Error handling for export operations
- Console feedback during export process

### exportSolutionInteractive()

Exports the current solution in multiple formats (TEXT, JSON, XML).

Process:
- Creates dedicated directory in user's Documents folder
- Exports in all three supported formats
- Provides individual success/failure feedback for each format
- Tracks overall export success rate

### browseForFile()

Opens a native file browser dialog for manual file selection.

Uses the Zenity utility on Linux systems to provide a native-looking file selection dialog. Falls back to current directory if the expected data directory doesn't exist.

### loadSolutionInteractive()

Provides an interactive mechanism to load previously computed solution files.

Features:
- File browser dialog for solution selection
- Support for multiple formats (TEXT, JSON, XML)
- Automatic result loading and view switching

## Design Patterns and Architecture

The BaseUI class follows several design principles:

### Model-View Separation
The UI layer is separated from the business logic, communicating through well-defined interfaces with the models, solver, and parser components.

### Event-Driven Architecture
User interactions are handled through an event-driven model where callbacks are registered and executed when UI elements are activated.

### Resource Management
Proper RAII (Resource Acquisition Is Initialization) principles are followed with smart pointers managing shared resources and SFML handling graphics resource lifecycle.

### Responsive Design
The interface adapts to window resizing and implements smooth scrolling for large file lists.

## Performance Considerations

The implementation includes several optimizations:
- Frame rate limiting to maintain consistent performance
- Efficient rendering with minimal redraw operations
- Memory management with bounded console history
- Lazy loading of UI elements only when needed
- Proper cleanup and resource management

## Error Handling

Comprehensive error handling is implemented throughout:
- Graceful degradation when fonts can't be loaded
- Safe file system operations with exception handling
- Validation of loaded problem instances
- User feedback for all error conditions
- Fallback mechanisms for missing files or directories