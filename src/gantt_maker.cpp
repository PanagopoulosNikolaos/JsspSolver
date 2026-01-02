#include "gantt_maker.hpp"

/**
 * Constructor for GanttChartMaker.
 */
GanttChartMaker::GanttChartMaker()
    : marginLeft(100), marginTop(50), marginRight(50), marginBottom(50),
      rowHeight(60), timeScale(20), machineLabelWidth(80),
      fontLoaded(false) {
    
    // Initialize window
    window.create(sf::VideoMode(1200, 800), "JSSP Gantt Chart", sf::Style::Close);
    window.setFramerateLimit(60);
    
    // Initialize colors for jobs
    jobColors = {
        sf::Color(255, 99, 71),    // Tomato
        sf::Color(70, 130, 180),   // SteelBlue
        sf::Color(60, 179, 113),   // MediumSeaGreen
        sf::Color(255, 215, 0),    // Gold
        sf::Color(147, 112, 219),  // MediumPurple
        sf::Color(255, 105, 180),  // HotPink
        sf::Color(255, 140, 0),    // DarkOrange
        sf::Color(64, 224, 208),   // Turquoise
        sf::Color(220, 20, 60),    // Crimson
        sf::Color(0, 206, 209)     // DarkTurquoise
    };
    
    loadFont();
}

/**
 * Destructor for GanttChartMaker.
 */
GanttChartMaker::~GanttChartMaker() {
    if (window.isOpen()) {
        window.close();
    }
}

/**
 * Loads the font for text rendering.
 *
 * Returns:
 *   True if font loaded successfully.
 */
bool GanttChartMaker::loadFont() {
    // Try to load a default font (you may need to add a font file to your project)
    // For now, we'll use system fonts
    if (font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf")) {
        fontLoaded = true;
        return true;
    } else if (font.loadFromFile("/usr/share/fonts/TTF/arial.ttf")) {
        fontLoaded = true;
        return true;
    } else {
        std::cerr << "Warning: Could not load font. Text labels will not be displayed." << std::endl;
        fontLoaded = false;
        return false;
    }
}

/**
 * Gets the color for a specific job.
 *
 * Args:
 *   jobId: Job identifier.
 *
 * Returns:
 *   Color for the job.
 */
sf::Color GanttChartMaker::getJobColor(int jobId) {
    if (jobId < static_cast<int>(jobColors.size())) {
        return jobColors[jobId];
    }
    // Generate a color for jobs beyond our predefined set
    int colorIndex = jobId % static_cast<int>(jobColors.size());
    return jobColors[colorIndex];
}

/**
 * Draws the grid for the Gantt chart.
 *
 * Args:
 *   startX: Starting X position.
 *   startY: Starting Y position.
 *   maxTime: Maximum time value.
 *   numMachines: Number of machines.
 */
void GanttChartMaker::drawGrid(float startX, float startY, int maxTime, int numMachines) {
    sf::RectangleShape line(sf::Vector2f(maxTime * timeScale, 1));
    line.setFillColor(sf::Color(200, 200, 200));
    
    // Horizontal lines (machine separators)
    for (int i = 0; i <= numMachines; i++) {
        float y = startY + i * rowHeight;
        line.setPosition(startX, y);
        line.setSize(sf::Vector2f(maxTime * timeScale, 1));
        window.draw(line);
    }
    
    // Vertical lines (time separators)
    sf::RectangleShape vline(sf::Vector2f(1, numMachines * rowHeight));
    vline.setFillColor(sf::Color(200, 200, 200));
    
    for (int t = 0; t <= maxTime; t += 5) {
        float x = startX + t * timeScale;
        vline.setPosition(x, startY);
        vline.setSize(sf::Vector2f(1, numMachines * rowHeight));
        window.draw(vline);
    }
}

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
void GanttChartMaker::drawGridToTexture(sf::RenderTexture& target, float startX, float startY, int maxTime, int numMachines) {
    sf::RectangleShape line(sf::Vector2f(maxTime * timeScale, 1));
    line.setFillColor(sf::Color(200, 200, 200));
    
    // Horizontal lines (machine separators)
    for (int i = 0; i <= numMachines; i++) {
        float y = startY + i * rowHeight;
        line.setPosition(startX, y);
        line.setSize(sf::Vector2f(maxTime * timeScale, 1));
        target.draw(line);
    }
    
    // Vertical lines (time separators)
    sf::RectangleShape vline(sf::Vector2f(1, numMachines * rowHeight));
    vline.setFillColor(sf::Color(200, 200, 200));
    
    for (int t = 0; t <= maxTime; t += 5) {
        float x = startX + t * timeScale;
        vline.setPosition(x, startY);
        vline.setSize(sf::Vector2f(1, numMachines * rowHeight));
        target.draw(vline);
    }
}

/**
 * Draws the time axis to a render texture.
 *
 * Args:
 *   target: Render texture to draw to.
 *   startX: Starting X position.
 *   startY: Starting Y position.
 *   maxTime: Maximum time value.
 */
void GanttChartMaker::drawTimeAxisToTexture(sf::RenderTexture& target, float startX, float startY, int maxTime) {
    if (!fontLoaded) return;
    
    for (int t = 0; t <= maxTime; t += 5) {
        float x = startX + t * timeScale;
        
        // Draw tick
        sf::RectangleShape tick(sf::Vector2f(1, 10));
        tick.setFillColor(sf::Color(0, 0, 0));
        tick.setPosition(x, startY - 10);
        target.draw(tick);
        
        // Draw time label
        sf::Text timeText;
        timeText.setFont(font);
        timeText.setString(std::to_string(t));
        timeText.setCharacterSize(12);
        timeText.setFillColor(sf::Color(0, 0, 0));
        timeText.setPosition(x - 5, startY - 25);
        target.draw(timeText);
    }
}

/**
 * Draws machine labels to a render texture.
 *
 * Args:
 *   target: Render texture to draw to.
 *   startX: Starting X position.
 *   startY: Starting Y position.
 *   problem: Problem instance.
 */
void GanttChartMaker::drawMachineLabelsToTexture(sf::RenderTexture& target, float startX, float startY, std::shared_ptr<ProblemInstance> problem) {
    if (!fontLoaded) return;
    
    for (int i = 0; i < problem->numMachines; i++) {
        float y = startY + i * rowHeight + rowHeight / 2;
        
        sf::Text machineText;
        machineText.setFont(font);
        machineText.setString("M" + std::to_string(i));
        machineText.setCharacterSize(14);
        machineText.setFillColor(sf::Color(0, 0, 0));
        machineText.setPosition(startX - machineLabelWidth + 10, y - 7);
        target.draw(machineText);
    }
}

/**
 * Draws operations to a render texture.
 *
 * Args:
 *   target: Render texture to draw to.
 *   startX: Starting X position.
 *   startY: Starting Y position.
 *   result: Schedule result.
 */
void GanttChartMaker::drawOperationsToTexture(sf::RenderTexture& target, float startX, float startY, std::shared_ptr<ScheduleResult> result) {
    for (const auto& job : result->problem.jobs) {
        for (const auto& operation : job->operations) {
            if (operation->isScheduled()) {
                float x = startX + operation->startTime * timeScale;
                float y = startY + operation->machineId * rowHeight + 5;
                float width = operation->getDuration() * timeScale;
                float height = rowHeight - 10;
                
                // Draw operation rectangle
                sf::RectangleShape rect(sf::Vector2f(width, height));
                rect.setFillColor(getJobColor(operation->jobId));
                rect.setOutlineColor(sf::Color(0, 0, 0));
                rect.setOutlineThickness(1);
                rect.setPosition(x, y);
                target.draw(rect);
                
                // Draw operation label
                if (fontLoaded && width > 30) {
                    sf::Text opText;
                    opText.setFont(font);
                    opText.setString("J" + std::to_string(operation->jobId) + " Op" + std::to_string(operation->operationId));
                    opText.setCharacterSize(10);
                    opText.setFillColor(sf::Color(0, 0, 0));
                    opText.setPosition(x + 2, y + height / 2 - 5);
                    target.draw(opText);
                }
            }
        }
    }
}

/**
 * Draws the time axis.
 *
 * Args:
 *   startX: Starting X position.
 *   startY: Starting Y position.
 *   maxTime: Maximum time value.
 */
void GanttChartMaker::drawTimeAxis(float startX, float startY, int maxTime) {
    if (!fontLoaded) return;
    
    for (int t = 0; t <= maxTime; t += 5) {
        float x = startX + t * timeScale;
        
        // Draw tick
        sf::RectangleShape tick(sf::Vector2f(1, 10));
        tick.setFillColor(sf::Color(0, 0, 0));
        tick.setPosition(x, startY - 10);
        window.draw(tick);
        
        // Draw time label
        sf::Text timeText;
        timeText.setFont(font);
        timeText.setString(std::to_string(t));
        timeText.setCharacterSize(12);
        timeText.setFillColor(sf::Color(0, 0, 0));
        timeText.setPosition(x - 5, startY - 25);
        window.draw(timeText);
    }
}

/**
 * Draws machine labels.
 *
 * Args:
 *   startX: Starting X position.
 *   startY: Starting Y position.
 *   problem: Problem instance.
 */
void GanttChartMaker::drawMachineLabels(float startX, float startY, std::shared_ptr<ProblemInstance> problem) {
    if (!fontLoaded) return;
    
    for (int i = 0; i < problem->numMachines; i++) {
        float y = startY + i * rowHeight + rowHeight / 2;
        
        sf::Text machineText;
        machineText.setFont(font);
        machineText.setString("M" + std::to_string(i));
        machineText.setCharacterSize(14);
        machineText.setFillColor(sf::Color(0, 0, 0));
        machineText.setPosition(startX - machineLabelWidth + 10, y - 7);
        window.draw(machineText);
    }
}

/**
 * Draws operations on the chart.
 *
 * Args:
 *   startX: Starting X position.
 *   startY: Starting Y position.
 *   result: Schedule result.
 */
void GanttChartMaker::drawOperations(float startX, float startY, std::shared_ptr<ScheduleResult> result) {
    for (const auto& job : result->problem.jobs) {
        for (const auto& operation : job->operations) {
            if (operation->isScheduled()) {
                float x = startX + operation->startTime * timeScale;
                float y = startY + operation->machineId * rowHeight + 5;
                float width = operation->getDuration() * timeScale;
                float height = rowHeight - 10;
                
                // Draw operation rectangle
                sf::RectangleShape rect(sf::Vector2f(width, height));
                rect.setFillColor(getJobColor(operation->jobId));
                rect.setOutlineColor(sf::Color(0, 0, 0));
                rect.setOutlineThickness(1);
                rect.setPosition(x, y);
                window.draw(rect);
                
                // Draw operation label
                if (fontLoaded && width > 30) {
                    sf::Text opText;
                    opText.setFont(font);
                    opText.setString("J" + std::to_string(operation->jobId) + " Op" + std::to_string(operation->operationId));
                    opText.setCharacterSize(10);
                    opText.setFillColor(sf::Color(0, 0, 0));
                    opText.setPosition(x + 2, y + height / 2 - 5);
                    window.draw(opText);
                }
            }
        }
    }
}

/**
 * Displays the Gantt chart for a schedule result.
 *
 * Args:
 *   result: Schedule result to display.
 */
void GanttChartMaker::displaySchedule(std::shared_ptr<ScheduleResult> result) {
    if (!result) {
        std::cerr << "Error: No schedule result provided" << std::endl;
        return;
    }
    
    // Calculate layout
    float startX = marginLeft + machineLabelWidth;
    float startY = marginTop + 50; // Space for time axis
    
    // Clear window
    window.clear(sf::Color(255, 255, 255));
    
    // Draw title
    if (fontLoaded) {
        sf::Text title;
        title.setFont(font);
        title.setString("JSSP Schedule - Makespan: " + std::to_string(result->makespan));
        title.setCharacterSize(20);
        title.setFillColor(sf::Color(0, 0, 0));
        title.setPosition(marginLeft, 10);
        window.draw(title);
    }
    
    // Draw grid
    drawGrid(startX, startY, result->makespan, result->problem.numMachines);
    
    // Draw time axis
    drawTimeAxis(startX, startY - 30, result->makespan);
    
    // Draw machine labels
    drawMachineLabels(startX, startY, std::make_shared<ProblemInstance>(result->problem));
    
    // Draw operations
    drawOperations(startX, startY, result);
    
    // Display legend
    if (fontLoaded && result->problem.numJobs <= static_cast<int>(jobColors.size())) {
        float legendY = window.getSize().y - marginBottom - 80;
        for (int i = 0; i < result->problem.numJobs; i++) {
            float legendX = marginLeft + i * 80;
            
            // Draw color box
            sf::RectangleShape colorBox(sf::Vector2f(15, 15));
            colorBox.setFillColor(getJobColor(i));
            colorBox.setOutlineColor(sf::Color(0, 0, 0));
            colorBox.setOutlineThickness(1);
            colorBox.setPosition(legendX, legendY);
            window.draw(colorBox);
            
            // Draw job label
            sf::Text jobText;
            jobText.setFont(font);
            jobText.setString("Job " + std::to_string(i));
            jobText.setCharacterSize(12);
            jobText.setFillColor(sf::Color(0, 0, 0));
            jobText.setPosition(legendX + 20, legendY - 2);
            window.draw(jobText);
        }
    }
    
    window.display();
}

/**
 * Saves the Gantt chart to a file.
 *
 * Args:
 *   result: Schedule result to save.
 *   filename: Output file path.
 */
void GanttChartMaker::saveToFile(std::shared_ptr<ScheduleResult> result, const std::string& filename) {
    if (!result) {
        std::cerr << "Error: No schedule result provided for saving." << std::endl;
        return;
    }
    
    // Calculate required dimensions for the full chart
    float startX = marginLeft + machineLabelWidth;
    float startY = marginTop + 50; // Space for time axis
    
    // Calculate width based on makespan
    unsigned int chartWidth = static_cast<unsigned int>(startX + result->makespan * timeScale + marginRight);
    
    // Calculate height based on number of machines
    unsigned int chartHeight = static_cast<unsigned int>(startY + result->problem.numMachines * rowHeight + marginBottom + 100); // Extra for legend
    
    // Create a render texture with proper dimensions
    sf::RenderTexture renderTexture;
    if (!renderTexture.create(chartWidth, chartHeight)) {
        std::cerr << "Error: Failed to create render texture for saving." << std::endl;
        return;
    }
    
    // Clear the render texture
    renderTexture.clear(sf::Color(255, 255, 255));
    
    // Draw title
    if (fontLoaded) {
        sf::Text title;
        title.setFont(font);
        title.setString("JSSP Schedule - Makespan: " + std::to_string(result->makespan));
        title.setCharacterSize(20);
        title.setFillColor(sf::Color(0, 0, 0));
        title.setPosition(marginLeft, 10);
        renderTexture.draw(title);
    }
    
    // Draw grid
    drawGridToTexture(renderTexture, startX, startY, result->makespan, result->problem.numMachines);
    
    // Draw time axis
    drawTimeAxisToTexture(renderTexture, startX, startY - 30, result->makespan);
    
    // Draw machine labels
    drawMachineLabelsToTexture(renderTexture, startX, startY, std::make_shared<ProblemInstance>(result->problem));
    
    // Draw operations
    drawOperationsToTexture(renderTexture, startX, startY, result);
    
    // Display legend
    if (fontLoaded && result->problem.numJobs <= static_cast<int>(jobColors.size())) {
        float legendY = chartHeight - marginBottom - 80;
        for (int i = 0; i < result->problem.numJobs; i++) {
            float legendX = marginLeft + i * 80;
            
            // Draw color box
            sf::RectangleShape colorBox(sf::Vector2f(15, 15));
            colorBox.setFillColor(getJobColor(i));
            colorBox.setOutlineColor(sf::Color(0, 0, 0));
            colorBox.setOutlineThickness(1);
            colorBox.setPosition(legendX, legendY);
            renderTexture.draw(colorBox);
            
            // Draw job label
            sf::Text jobText;
            jobText.setFont(font);
            jobText.setString("Job " + std::to_string(i));
            jobText.setCharacterSize(12);
            jobText.setFillColor(sf::Color(0, 0, 0));
            jobText.setPosition(legendX + 20, legendY - 2);
            renderTexture.draw(jobText);
        }
    }
    
    renderTexture.display();
    
    // Save to file
    sf::Texture texture = renderTexture.getTexture();
    if (texture.copyToImage().saveToFile(filename)) {
        std::cout << "Gantt chart saved successfully to: " << filename << std::endl;
    } else {
        std::cerr << "Error: Failed to save Gantt chart to file: " << filename << std::endl;
    }
}

/**
 * Sets the window size.
 *
 * Args:
 *   width: Window width.
 *   height: Window height.
 */
void GanttChartMaker::setWindowSize(unsigned int width, unsigned int height) {
    sf::Vector2u newSize(width, height);
    window.setSize(newSize);
}

/**
 * Sets the time scale for the chart.
 *
 * Args:
 *   scale: Time scale factor.
 */
void GanttChartMaker::setTimeScale(float scale) {
    timeScale = scale;
}

/**
 * Sets the row height for machines.
 *
 * Args:
 *   height: Row height.
 */
void GanttChartMaker::setRowHeight(float height) {
    rowHeight = height;
}

/**
 * Checks if the window is open.
 *
 * Returns:
 *   True if window is open.
 */
bool GanttChartMaker::isOpen() const {
    return window.isOpen();
}

/**
 * Polls for window events.
 */
void GanttChartMaker::pollEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
    }
}

/**
 * Closes the window.
 */
void GanttChartMaker::close() {
    window.close();
}
