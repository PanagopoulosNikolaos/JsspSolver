#ifndef GANTT_MAKER_HPP
#define GANTT_MAKER_HPP

#include "models.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <map>
#include <memory>
#include <iostream>

/**
 * Struct representing a single operation in the Gantt chart.
 */
struct GanttOperation {
    std::shared_ptr<Operation> operation;
    sf::RectangleShape rectangle;
    sf::Text label;
    sf::Color color;
};

/**
 * Class for creating and displaying Gantt charts.
 */
class GanttChartMaker {
private:
    sf::RenderWindow window;
    sf::View view;
    
    // Layout parameters
    float marginLeft;
    float marginTop;
    float marginRight;
    float marginBottom;
    float rowHeight;
    float timeScale;
    float machineLabelWidth;
    
    // Fonts and text
    sf::Font font;
    bool fontLoaded;
    
    // Colors for different jobs
    std::vector<sf::Color> jobColors;
    
    // Helper methods
    /**
     * Draws the time axis.
     *
     * Args:
     *   startX: Starting X position.
     *   startY: Starting Y position.
     *   maxTime: Maximum time value.
     */
    void drawTimeAxis(float startX, float startY, int maxTime);

    /**
     * Draws machine labels.
     *
     * Args:
     *   startX: Starting X position.
     *   startY: Starting Y position.
     *   problem: Problem instance.
     */
    void drawMachineLabels(float startX, float startY, std::shared_ptr<ProblemInstance> problem);

    /**
     * Draws operations on the chart.
     *
     * Args:
     *   startX: Starting X position.
     *   startY: Starting Y position.
     *   result: Schedule result.
     */
    void drawOperations(float startX, float startY, std::shared_ptr<ScheduleResult> result);

    /**
     * Loads the font.
     *
     * Returns:
     *   True if loaded successfully.
     */
    bool loadFont();

    /**
     * Draws the grid.
     *
     * Args:
     *   startX: Starting X position.
     *   startY: Starting Y position.
     *   maxTime: Maximum time.
     *   numMachines: Number of machines.
     */
    void drawGrid(float startX, float startY, int maxTime, int numMachines);
    
    /**
     * Draws the grid to a render texture.
     *
     * Args:
     *   target: Render texture to draw to.
     *   startX: Starting X position.
     *   startY: Starting Y position.
     *   maxTime: Maximum time.
     *   numMachines: Number of machines.
     */
    void drawGridToTexture(sf::RenderTexture& target, float startX, float startY, int maxTime, int numMachines);
    
    /**
     * Draws the time axis to a render texture.
     *
     * Args:
     *   target: Render texture to draw to.
     *   startX: Starting X position.
     *   startY: Starting Y position.
     *   maxTime: Maximum time value.
     */
    void drawTimeAxisToTexture(sf::RenderTexture& target, float startX, float startY, int maxTime);
    
    /**
     * Draws machine labels to a render texture.
     *
     * Args:
     *   target: Render texture to draw to.
     *   startX: Starting X position.
     *   startY: Starting Y position.
     *   problem: Problem instance.
     */
    void drawMachineLabelsToTexture(sf::RenderTexture& target, float startX, float startY, std::shared_ptr<ProblemInstance> problem);
    
    /**
     * Draws operations to a render texture.
     *
     * Args:
     *   target: Render texture to draw to.
     *   startX: Starting X position.
     *   startY: Starting Y position.
     *   result: Schedule result.
     */
    void drawOperationsToTexture(sf::RenderTexture& target, float startX, float startY, std::shared_ptr<ScheduleResult> result);
    
public:
    /**
     * Constructor for GanttChartMaker.
     */
    GanttChartMaker();

    /**
     * Destructor for GanttChartMaker.
     */
    ~GanttChartMaker();

    // Main display function
    /**
     * Displays the Gantt chart for a schedule result.
     *
     * Args:
     *   result: The schedule result to display.
     */
    void displaySchedule(std::shared_ptr<ScheduleResult> result);

    // Save to file functionality
    /**
     * Saves the Gantt chart to a file.
     *
     * Args:
     *   result: The schedule result to save.
     *   filename: Output file path.
     */
    void saveToFile(std::shared_ptr<ScheduleResult> result, const std::string& filename);

    // Configuration
    /**
     * Sets the window size.
     *
     * Args:
     *   width: Window width.
     *   height: Window height.
     */
    void setWindowSize(unsigned int width, unsigned int height);

    /**
     * Sets the time scale for the chart.
     *
     * Args:
     *   scale: Time scale factor.
     */
    void setTimeScale(float scale);

    /**
     * Sets the row height for machines.
     *
     * Args:
     *   height: Row height.
     */
    void setRowHeight(float height);

    // Color generation
    /**
     * Gets the color for a specific job.
     *
     * Args:
     *   jobId: Job identifier.
     *
     * Returns:
     *   Color for the job.
     */
    sf::Color getJobColor(int jobId);

    // Window management
    /**
     * Checks if the window is open.
     *
     * Returns:
     *   True if window is open.
     */
    bool isOpen() const;

    /**
     * Polls for window events.
     */
    void pollEvents();

    /**
     * Closes the window.
     */
    void close();
};

#endif // GANTT_MAKER_HPP
