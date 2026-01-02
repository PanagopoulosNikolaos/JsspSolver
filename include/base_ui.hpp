#ifndef BASE_UI_HPP
#define BASE_UI_HPP

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

/**
 * Enumeration for different view modes in the UI.
 */
enum class ViewMode {
    Output,
    GanttChart
};

/**
 * Base class for the GUI application handling user interface and interactions.
 */
class BaseUI {
private:
    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded;
    
    // Theme Colors
    const sf::Color colorBg = sf::Color(18, 18, 18);
    const sf::Color colorSidebar = sf::Color(25, 25, 25);
    const sf::Color colorHeader = sf::Color(33, 33, 33);
    const sf::Color colorAccent = sf::Color(0, 120, 215);
    const sf::Color colorTextMain = sf::Color(240, 240, 240);
    const sf::Color colorTextDim = sf::Color(150, 150, 150);
    
    // UI State
    ViewMode currentView;
    std::string selectedFile;
    SchedulingAlgorithm selectedAlgo;
    std::vector<std::string> consoleLines;
    
    // Problem data
    std::shared_ptr<ProblemInstance> currentProblem;
    std::shared_ptr<ScheduleResult> currentResult;
    
    // Layout constants
    const float headerHeight = 70.f;
    const float sidebarWidth = 260.f;
    
    // UI Elements
    struct Button {
        sf::RectangleShape shape;
        sf::Text text;
        std::function<void()> action;
        bool isSelected = false;
        bool isAction = false;
    };
    
    std::vector<Button> fileButtons;
    std::vector<Button> algoButtons;
    std::vector<Button> navButtons;
    
    // Dropdown state
    bool dropdownOpen;
    Button dropdownButton;
    std::vector<Button> dropdownItems;
    std::vector<std::string> availableFiles;
    
    // Scrolling state
    int fileScrollOffset;
    
    // Helper methods
    /**
     * Loads the font for UI elements.
     *
     * Returns:
     *   True if font loaded successfully.
     */
    bool loadFont();

    /**
     * Initializes the UI layout.
     */
    void initLayout();

    /**
     * Handles user input events.
     */
    void handleInput();

    /**
     * Updates UI state.
     *
     * Args:
     *   mousePos: Current mouse position.
     */
    void update(sf::Vector2f mousePos);

    /**
     * Renders the UI.
     */
    void draw();

    /**
     * Draws the header section.
     */
    void drawHeader();

    /**
     * Draws the sidebar.
     */
    void drawSidebar();

    /**
     * Draws the main area.
     */
    void drawMainArea();

    /**
     * Draws the console output.
     */
    void drawConsole();

    /**
     * Draws the Gantt chart in the main area.
     */
    void drawGanttInMain();

    /**
     * Logs a message to the console.
     *
     * Args:
     *   message: Message to log.
     */
    void logToConsole(const std::string& message);

    /**
     * Creates a button and adds it to a container.
     *
     * Args:
     *   container: Button container to add to.
     *   label: Button text.
     *   pos: Button position.
     *   size: Button size.
     *   action: Callback function.
     *   isAction: Whether this is an action button (primary style).
     */
    void createButton(std::vector<Button>& container, const std::string& label, sf::Vector2f pos, sf::Vector2f size, std::function<void()> action, bool isAction = false);

public:
    /**
     * Constructor for BaseUI.
     */
    BaseUI();

    /**
     * Destructor for BaseUI.
     */
    ~BaseUI();

    /**
     * Runs the main UI loop.
     */
    void run();

    /**
     * Loads a problem file.
     *
     * Args:
     *   filename: Path to the input file.
     */
    void loadFile(const std::string& filename);

    /**
     * Solves the loaded problem using the selected algorithm.
     */
    void solve();

    /**
     * Displays a message dialog.
     *
     * Args:
     *   title: Dialog title.
     *   message: Dialog message.
     */
    void showMessage(const std::string& title, const std::string& message);


    /**
     * Opens an interactive dialog to export Gantt chart.
     */
    void exportGanttChartInteractive();

    /**
     * Opens an interactive dialog to export solution.
     */
    void exportSolutionInteractive();

    /**
     * Opens an interactive dialog to load a solution.
     */
    void loadSolutionInteractive();

    /**
     * Loads a solution from a file.
     *
     * Args:
     *   filename: Path to the solution file.
     */
    void loadSolutionFromFile(const std::string& filename);

    /**
     * Opens a file browser dialog to select a JSSP file.
     */
    void browseForFile();
};

#endif // BASE_UI_HPP
