#include "base_ui.hpp"
#include "solution_serializer.hpp"
#include "gantt_maker.hpp"
#include <iostream>
#include <filesystem>
#include <cstdio>
#include <memory>
#include <string>
#include <array>
#include <cmath>

// BaseUI constructor: Initializes the UI with default settings, loads font, sets up layout, and logs welcome messages.
BaseUI::BaseUI() : currentView(ViewMode::Output), selectedAlgo(SchedulingAlgorithm::FIFO), fileScrollOffset(0), dropdownOpen(false) {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(sf::VideoMode(1280, 950), "JSSP Dashboard", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    
    loadFont();
    initLayout();
    logToConsole("Welcome to JSSP Solver Dashboard.");
    logToConsole("Select a file and algorithm from the sidebar, then click 'Solve'.");
}

// BaseUI destructor: Default cleanup.
BaseUI::~BaseUI() {}

// Load font from common system paths. Returns true if successful.
bool BaseUI::loadFont() {
    // List of potential font paths across OSes
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

// Create a button and add to container. Handles styling and text centering.
void BaseUI::createButton(std::vector<Button>& container, const std::string& label, sf::Vector2f pos, sf::Vector2f size, std::function<void()> action, bool isAction) {
    Button b;
    b.shape.setSize(size);
    b.shape.setPosition(pos);
    b.isAction = isAction;
    
    // Style based on action type
    if (isAction) {
        b.shape.setFillColor(colorAccent);
        b.shape.setOutlineColor(sf::Color(255, 255, 255, 50));
    } else {
        b.shape.setFillColor(sf::Color(45, 45, 48));
        b.shape.setOutlineColor(sf::Color(60, 60, 60));
    }
    b.shape.setOutlineThickness(1);
    
    if (fontLoaded) {
        b.text.setFont(font);
        b.text.setString(label);
        b.text.setCharacterSize(13);
        b.text.setFillColor(colorTextMain);
        
        // Center text in button
        sf::FloatRect textRect = b.text.getLocalBounds();
        b.text.setOrigin(std::floor(textRect.left + textRect.width/2.0f), std::floor(textRect.top + textRect.height/2.0f));
        b.text.setPosition(std::floor(pos.x + size.x/2.0f), std::floor(pos.y + size.y/2.0f));
    }
    
    b.action = action;
    container.push_back(b);
}

// Initialize UI layout: Load files, create dropdown, buttons for algos and actions.
void BaseUI::initLayout() {
    fileButtons.clear();
    algoButtons.clear();
    navButtons.clear();
    dropdownItems.clear();
    availableFiles.clear();
    
    float startY = headerHeight + 35;
    float btnHeight = 32;
    float btnSpacing = 8;
    float sectionSpacing = 30;
    
    // Load .jssp files from data directory
    std::string dataPath = "data";
    if (!std::filesystem::exists(dataPath)) {
        dataPath = "../data";
    }
    
    try {
        if (std::filesystem::exists(dataPath)) {
            for (const auto& entry : std::filesystem::directory_iterator(dataPath)) {
                if (entry.is_regular_file() && entry.path().extension() == ".jssp") {
                    availableFiles.push_back(entry.path().filename().string());
                }
            }
            std::sort(availableFiles.begin(), availableFiles.end());
            
            if (!availableFiles.empty()) {
                logToConsole("Found " + std::to_string(availableFiles.size()) + " problem files in " + dataPath);
            }
        }
    } catch (const std::exception& e) {
        logToConsole("Error reading data directory: " + std::string(e.what()));
    }
    
    // Fallback if no files
    if (availableFiles.empty()) {
        logToConsole("No problem files found in data directory.");
    }
    
    // Create dropdown button for file selection
    dropdownButton.shape.setSize({sidebarWidth - 20, btnHeight});
    dropdownButton.shape.setPosition(15, startY);
    dropdownButton.shape.setFillColor(sf::Color(45, 45, 48));
    dropdownButton.shape.setOutlineThickness(1);
    dropdownButton.shape.setOutlineColor(sf::Color(60, 60, 60));
    
    if (fontLoaded) {
        std::string label = selectedFile.empty() ? "Select Problem File ->" : selectedFile + " *";
        dropdownButton.text.setFont(font);
        dropdownButton.text.setString(label);
        dropdownButton.text.setCharacterSize(13);
        dropdownButton.text.setFillColor(colorTextMain);
        sf::FloatRect textRect = dropdownButton.text.getLocalBounds();
        dropdownButton.text.setOrigin(std::floor(textRect.left + textRect.width/2.0f), std::floor(textRect.top + textRect.height/2.0f));
        dropdownButton.text.setPosition(std::floor(15 + (sidebarWidth - 30)/2.0f), std::floor(startY + btnHeight/2.0f));
    }
    
    dropdownButton.action = [this]() {
        dropdownOpen = !dropdownOpen;
    };
    
    // Create dropdown items for each file
    float dropdownY = startY + btnHeight;
    for (const auto& f : availableFiles) {
        Button item;
        item.shape.setSize({sidebarWidth - 30, btnHeight});
        item.shape.setPosition(15, dropdownY);
        item.shape.setFillColor(sf::Color(35, 35, 38));
        item.shape.setOutlineThickness(1);
        item.shape.setOutlineColor(sf::Color(50, 50, 50));
        
        if (fontLoaded) {
            item.text.setFont(font);
            item.text.setString(f);
            item.text.setCharacterSize(13);
            item.text.setFillColor(colorTextMain);
            sf::FloatRect textRect = item.text.getLocalBounds();
            item.text.setOrigin(std::floor(textRect.left + textRect.width/2.0f), std::floor(textRect.top + textRect.height/2.0f));
            item.text.setPosition(std::floor(15 + (sidebarWidth - 30)/2.0f), std::floor(dropdownY + btnHeight/2.0f));
        }
        
        item.action = [this, f, btnHeight]() {
            loadFile("data/" + f);
            dropdownOpen = false;
            
            // Update dropdown label
            if (fontLoaded) {
                dropdownButton.text.setString(f + " [v]");
                sf::FloatRect textRect = dropdownButton.text.getLocalBounds();
                dropdownButton.text.setOrigin(std::floor(textRect.left + textRect.width/2.0f), std::floor(textRect.top + textRect.height/2.0f));
                dropdownButton.text.setPosition(std::floor(15 + (sidebarWidth - 30)/2.0f), std::floor(dropdownButton.shape.getPosition().y + btnHeight/2.0f));
            }
        };
        
        dropdownItems.push_back(item);
        dropdownY += btnHeight;
    }
    
    // Add custom load button
    Button loadCustom;
    loadCustom.shape.setSize({sidebarWidth - 30, btnHeight});
    loadCustom.shape.setPosition(15, dropdownY);
    loadCustom.shape.setFillColor(colorAccent);
    loadCustom.shape.setOutlineThickness(1);
    loadCustom.shape.setOutlineColor(sf::Color(255, 255, 255, 50));
    loadCustom.isAction = true;
    
    if (fontLoaded) {
        loadCustom.text.setFont(font);
        loadCustom.text.setString("Load Problem...");
        loadCustom.text.setCharacterSize(13);
        loadCustom.text.setFillColor(colorTextMain);
        sf::FloatRect textRect = loadCustom.text.getLocalBounds();
        loadCustom.text.setOrigin(std::floor(textRect.left + textRect.width/2.0f), std::floor(textRect.top + textRect.height/2.0f));
        loadCustom.text.setPosition(std::floor(15 + (sidebarWidth - 30)/2.0f), std::floor(dropdownY + btnHeight/2.0f));
    }
    
    loadCustom.action = [this]() {
        browseForFile();
        dropdownOpen = false;
    };
    
    dropdownItems.push_back(loadCustom);
    
    float currentY = startY;
    
    // Algorithms section at fixed bottom
    float bottomSectionY = 500;
    
    std::vector<std::pair<std::string, SchedulingAlgorithm>> algos = {
        {"FIFO", SchedulingAlgorithm::FIFO},
        {"SPT", SchedulingAlgorithm::SPT},
        {"LPT", SchedulingAlgorithm::LPT}
    };
    
    float algoY = bottomSectionY;
    for (const auto& a : algos) {
        createButton(algoButtons, a.first, {15, algoY}, {sidebarWidth - 30, btnHeight}, [this, a]() {
            selectedAlgo = a.second;
            logToConsole("Selected Algorithm: " + a.first);
        }, false);
        algoY += btnHeight + btnSpacing;
    }
    
    // Action buttons
    float actionY = algoY + sectionSpacing;
    
    createButton(navButtons, "SOLVE", {15, actionY}, {sidebarWidth - 30, 40}, [this]() {
        solve();
    }, true);
    
    actionY += 40 + sectionSpacing;
    
    createButton(navButtons, "Export Gantt", {15, actionY}, {sidebarWidth - 30, btnHeight}, [this]() {
        exportGanttChartInteractive();
    }, false);
    actionY += btnHeight + btnSpacing;
    
    createButton(navButtons, "Export Solution", {15, actionY}, {sidebarWidth - 30, btnHeight}, [this]() {
        exportSolutionInteractive();
    }, false);
    actionY += btnHeight + btnSpacing;
    
    createButton(navButtons, "Load Solution", {15, actionY}, {sidebarWidth - 30, btnHeight}, [this]() {
        loadSolutionInteractive();
    }, false);
    
    // View switchers
    actionY += btnHeight + sectionSpacing;
    float halfWidth = (sidebarWidth - 35) / 2;
    createButton(navButtons, "Console", {15, actionY}, {halfWidth, btnHeight}, [this]() {
        currentView = ViewMode::Output;
    }, false);
    createButton(navButtons, "Gantt", {15 + halfWidth + 5, actionY}, {halfWidth, btnHeight}, [this]() {
        currentView = ViewMode::GanttChart;
    }, false);
}

// Handle user input: Events like clicks, scrolls, resizes.
void BaseUI::handleInput() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
            
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                
                // Handle dropdown button
                if (dropdownButton.shape.getGlobalBounds().contains(mousePos)) {
                    dropdownButton.action();
                    continue;
                }
                
                // Handle dropdown items if open
                if (dropdownOpen) {
                    bool clickedItem = false;
                    for (auto& item : dropdownItems) {
                        if (item.shape.getGlobalBounds().contains(mousePos)) {
                            item.action();
                            clickedItem = true;
                            break;
                        }
                    }
                    if (clickedItem) continue;
                    
                    // Close dropdown if clicked outside
                    if (mousePos.x < sidebarWidth && mousePos.y > headerHeight) {
                        dropdownOpen = false;
                    }
                }
                
                // Check other buttons
                auto check = [&](std::vector<Button>& btns) {
                    for (auto& b : btns) {
                        if (b.shape.getGlobalBounds().contains(mousePos)) {
                            b.action();
                            return true;
                        }
                    }
                    return false;
                };
                
                if (check(algoButtons)) {}
                else if (check(navButtons)) {}
            }
        }
        
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.x >= 0 && event.mouseWheelScroll.x <= sidebarWidth) {
                // Scroll file list if in top section
                if (sf::Mouse::getPosition(window).y < 500) {
                    fileScrollOffset -= static_cast<int>(event.mouseWheelScroll.delta) * 30;
                    
                    int contentHeight = static_cast<int>(fileButtons.size()) * 40; // approx
                    int viewHeight = 500 - headerHeight;
                    int maxScroll = contentHeight - viewHeight;
                    
                    if (fileScrollOffset < 0) fileScrollOffset = 0;
                    if (maxScroll < 0) maxScroll = 0;
                    if (fileScrollOffset > maxScroll) fileScrollOffset = maxScroll;
                }
            }
        }
        
        // Handle window resize
        if (event.type == sf::Event::Resized) {
            sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            window.setView(sf::View(visibleArea));
        }
    }
}

// Update button states based on mouse position.
void BaseUI::update(sf::Vector2f mousePos) {
    auto updateBtn = [&](std::vector<Button>& btns, bool applyScroll = false) {
        for (auto& b : btns) {
            bool hovered = false;
            if (applyScroll) {
                if (mousePos.y > headerHeight && mousePos.y < 500 && mousePos.x < sidebarWidth) {
                    float virtualY = mousePos.y + fileScrollOffset;
                    if (b.shape.getGlobalBounds().contains(mousePos.x, virtualY)) {
                        hovered = true;
                    }
                }
            } else {
                if (b.shape.getGlobalBounds().contains(mousePos)) {
                    hovered = true;
                }
            }

            if (hovered) {
                if (b.isAction) b.shape.setFillColor(sf::Color(0, 140, 255)); // Lighter blue
                else b.shape.setFillColor(sf::Color(60, 60, 63));
            } else {
                if (b.isSelected) {
                    b.shape.setFillColor(sf::Color(50, 50, 50));
                    b.shape.setOutlineColor(colorAccent);
                    b.shape.setOutlineThickness(2);
                } else {
                    if (b.isAction) b.shape.setFillColor(colorAccent);
                    else b.shape.setFillColor(sf::Color(45, 45, 48));
                    b.shape.setOutlineThickness(1);
                    b.shape.setOutlineColor(sf::Color(60, 60, 60));
                }
            }
        }
    };
    updateBtn(fileButtons, true);
    updateBtn(algoButtons);
    updateBtn(navButtons);
}

// Draw the entire UI.
void BaseUI::draw() {
    window.clear(colorBg);
    
    drawMainArea(); // Main content
    drawSidebar();  // Sidebar
    drawHeader();   // Header
    
    window.display();
}

// Draw the header bar with title and status.
void BaseUI::drawHeader() {
    sf::RectangleShape header({(float)window.getSize().x, headerHeight});
    header.setFillColor(colorHeader);
    window.draw(header);
    
    // Border
    sf::RectangleShape border({(float)window.getSize().x, 1});
    border.setPosition(0, headerHeight);
    border.setFillColor(sf::Color(50, 50, 50));
    window.draw(border);
    
    if (fontLoaded) {
        sf::Text title("JSSP Solver", font, 22);
        title.setStyle(sf::Text::Bold);
        title.setPosition(20, 20);
        title.setFillColor(colorTextMain);
        window.draw(title);
        
        sf::Text subtitle("Dark Aqua Theme", font, 12);
        subtitle.setPosition(22, 48);
        subtitle.setFillColor(colorAccent);
        window.draw(subtitle);
        
        std::string status = "File: " + (selectedFile.empty() ? "None" : selectedFile);
        sf::Text statusText(status, font, 14);
        // Right-align
        sf::FloatRect bounds = statusText.getLocalBounds();
        statusText.setPosition(window.getSize().x - bounds.width - 30, 25);
        statusText.setFillColor(colorTextDim);
        window.draw(statusText);
    }
}

// Draw the sidebar with sections, dropdown, buttons.
void BaseUI::drawSidebar() {
    sf::RectangleShape sidebar({sidebarWidth, (float)window.getSize().y - headerHeight});
    sidebar.setPosition(0, headerHeight);
    sidebar.setFillColor(colorSidebar);
    window.draw(sidebar);
    
    sf::RectangleShape border({1, (float)window.getSize().y - headerHeight});
    border.setPosition(sidebarWidth, headerHeight);
    border.setFillColor(sf::Color(50, 50, 50));
    window.draw(border);
    
    if (fontLoaded) {
        sf::Text t1("FILES", font, 11);
        t1.setStyle(sf::Text::Bold);
        t1.setPosition(15, headerHeight + 10);
        t1.setFillColor(colorAccent);
        window.draw(t1);
        
        sf::Text t2("ALGORITHMS", font, 11);
        t2.setStyle(sf::Text::Bold);
        t2.setPosition(15, 480);
        t2.setFillColor(colorAccent);
        window.draw(t2);
    }
    
    // Draw dropdown
    window.draw(dropdownButton.shape);
    window.draw(dropdownButton.text);
    
    if (dropdownOpen) {
        for (auto& item : dropdownItems) {
            window.draw(item.shape);
            window.draw(item.text);
        }
    }
    
    // Draw algo buttons with selection highlight
    for (auto& b : algoButtons) {
        b.isSelected = (selectedAlgo == SchedulingAlgorithm::FIFO && b.text.getString() == "FIFO") ||
                       (selectedAlgo == SchedulingAlgorithm::SPT && b.text.getString() == "SPT") ||
                       (selectedAlgo == SchedulingAlgorithm::LPT && b.text.getString() == "LPT");
        window.draw(b.shape);
        window.draw(b.text);
    }
    // Draw nav buttons with view highlight
    for (auto& b : navButtons) {
        if ((b.text.getString() == "Console" && currentView == ViewMode::Output) ||
            (b.text.getString() == "Gantt" && currentView == ViewMode::GanttChart)) {
             b.shape.setOutlineColor(colorAccent);
             b.shape.setOutlineThickness(2);
        } else if (!b.isAction) {
             b.shape.setOutlineThickness(1);
             b.shape.setOutlineColor(sf::Color(60, 60, 60));
        }
        window.draw(b.shape);
        window.draw(b.text);
    }
}

// Draw the main area based on current view.
void BaseUI::drawMainArea() {
    if (currentView == ViewMode::Output) {
        drawConsole();
    } else {
        drawGanttInMain();
    }
}

// Draw the console output in main area.
void BaseUI::drawConsole() {
    float margin = 20;
    float x = sidebarWidth + margin;
    float y = headerHeight + margin;
    float w = window.getSize().x - sidebarWidth - (margin * 2);
    float h = window.getSize().y - headerHeight - (margin * 2);
    
    sf::RectangleShape bg({w, h});
    bg.setPosition(x, y);
    bg.setFillColor(sf::Color(10, 10, 10));
    bg.setOutlineColor(sf::Color(40, 40, 40));
    bg.setOutlineThickness(1);
    window.draw(bg);
    
    if (fontLoaded) {
        float textY = y + 10;
        float lineHeight = 20;
        int maxLines = h / lineHeight;
        
        size_t start = 0;
        if (consoleLines.size() > maxLines) {
            start = consoleLines.size() - maxLines;
        }
        
        for (size_t i = start; i < consoleLines.size(); ++i) {
            sf::Text line(consoleLines[i], font, 14);
            line.setPosition(x + 10, textY);
            
            // Color based on content
            std::string str = consoleLines[i];
            if (str.find("Error") != std::string::npos) line.setFillColor(sf::Color(255, 80, 80));
            else if (str.find("Solved!") != std::string::npos) line.setFillColor(sf::Color(80, 255, 80));
            else if (str.find(">") == 0) line.setFillColor(sf::Color(100, 200, 255));
            else line.setFillColor(sf::Color(200, 200, 200));
            
            window.draw(line);
            textY += lineHeight;
        }
    }
}

// Draw Gantt chart in main area.
void BaseUI::drawGanttInMain() {
    if (!currentResult) {
        if (fontLoaded) {
            sf::Text msg("No results to display.", font, 24);
            sf::FloatRect bounds = msg.getLocalBounds();
            msg.setOrigin(bounds.width/2, bounds.height/2);
            msg.setPosition(sidebarWidth + (window.getSize().x - sidebarWidth)/2, window.getSize().y/2 - 20);
            msg.setFillColor(sf::Color(80, 80, 80));
            window.draw(msg);
            
            sf::Text sub("Select a file and algorithm, then click 'Solve'.", font, 16);
            bounds = sub.getLocalBounds();
            sub.setOrigin(bounds.width/2, bounds.height/2);
            sub.setPosition(sidebarWidth + (window.getSize().x - sidebarWidth)/2, window.getSize().y/2 + 20);
            sub.setFillColor(sf::Color(60, 60, 60));
            window.draw(sub);
        }
        return;
    }
    
    float margin = 30;
    float startX = sidebarWidth + margin + 40; // Space for labels
    float startY = headerHeight + margin + 40; // Space for axis
    float availableWidth = window.getSize().x - startX - margin;
    float availableHeight = window.getSize().y - startY - margin;
    
    int numMachines = currentResult->problem.numMachines;
    float machineHeight = std::min(50.0f, availableHeight / numMachines - 10);
    float gap = 10;
    
    // Time scale
    int maxTime = currentResult->makespan;
    float timeScale = availableWidth / (maxTime * 1.05f); // 5% padding
    
    // Time axis
    sf::RectangleShape axisLine({availableWidth, 1});
    axisLine.setPosition(startX, startY - 10);
    axisLine.setFillColor(sf::Color(100, 100, 100));
    window.draw(axisLine);
    
    // Grid and labels
    int timeStep = std::max(1, maxTime / 10);
    for (int t = 0; t <= maxTime; t += timeStep) {
        float x = startX + t * timeScale;
        
        sf::RectangleShape gridLine({1, availableHeight});
        gridLine.setPosition(x, startY - 10);
        gridLine.setFillColor(sf::Color(30, 30, 30));
        window.draw(gridLine);
        
        if (fontLoaded) {
            sf::Text label(std::to_string(t), font, 10);
            label.setOrigin(label.getLocalBounds().width/2, 0);
            label.setPosition(x, startY - 25);
            label.setFillColor(sf::Color(150, 150, 150));
            window.draw(label);
        }
    }
    
    // Machine tracks
    for (int i = 0; i < numMachines; ++i) {
        float y = startY + i * (machineHeight + gap);
        
        if (fontLoaded) {
            sf::Text mText("M" + std::to_string(i), font, 14);
            mText.setOrigin(mText.getLocalBounds().width, mText.getLocalBounds().height/2);
            mText.setPosition(startX - 15, y + machineHeight/2);
            mText.setFillColor(colorTextMain);
            window.draw(mText);
        }
        
        sf::RectangleShape track({availableWidth, machineHeight});
        track.setPosition(startX, y);
        track.setFillColor(sf::Color(25, 25, 28));
        track.setOutlineColor(sf::Color(40, 40, 40));
        track.setOutlineThickness(1);
        window.draw(track);
    }
    
    // Operations
    for (const auto& job : currentResult->problem.jobs) {
        // Pastel color per job
        float hue = (job->jobId * 137.508f);
        hue = fmod(hue, 360.0f);
        
        float s = 0.6f;
        float v = 0.85f;
        float c = v * s;
        float x = c * (1 - std::abs(fmod(hue / 60.0f, 2) - 1));
        float m = v - c;
        float r=0, g=0, b=0;
        
        if(hue < 60) { r=c; g=x; b=0; }
        else if(hue < 120) { r=x; g=c; b=0; }
        else if(hue < 180) { r=0; g=c; b=x; }
        else if(hue < 240) { r=0; g=x; b=c; }
        else if(hue < 300) { r=x; g=0; b=c; }
        else { r=c; g=0; b=x; }
        
        sf::Color jobColor((r+m)*255, (g+m)*255, (b+m)*255);
        
        for (const auto& op : job->operations) {
            if (op->isScheduled()) {
                float y = startY + op->machineId * (machineHeight + gap);
                float x = startX + op->startTime * timeScale;
                float w = op->processingTime * timeScale;
                
                sf::RectangleShape rect({w, machineHeight - 4});
                rect.setPosition(x, y + 2);
                rect.setFillColor(jobColor);
                rect.setOutlineThickness(1);
                rect.setOutlineColor(sf::Color(255, 255, 255, 100));
                
                window.draw(rect);
                
                // Job ID if space
                if (fontLoaded && w > 15) {
                    sf::Text idText(std::to_string(job->jobId), font, 10);
                    idText.setOrigin(idText.getLocalBounds().width/2, idText.getLocalBounds().height/2);
                    idText.setPosition(x + w/2, y + machineHeight/2);
                    idText.setFillColor(sf::Color::Black);
                    window.draw(idText);
                }
            }
        }
    }
    
    // Makespan info
    if (fontLoaded) {
        sf::Text info("Makespan: " + std::to_string(currentResult->makespan), font, 16);
        info.setPosition(startX, startY + numMachines * (machineHeight + gap) + 10);
        info.setFillColor(colorAccent);
        window.draw(info);
    }
}

// Log message to console.
void BaseUI::logToConsole(const std::string& message) {
    consoleLines.push_back("> " + message);
    if (consoleLines.size() > 100) consoleLines.erase(consoleLines.begin());
}

// Load problem file.
void BaseUI::loadFile(const std::string& filename) {
    std::string path_to_load = filename;
    if (!std::filesystem::exists(path_to_load)) {
        if (path_to_load.rfind("data/", 0) == 0) {
             std::string parent_path = "../" + path_to_load;
             if (std::filesystem::exists(parent_path)) {
                 path_to_load = parent_path;
             }
        }
    }

    try {
        currentProblem = Parser::parseFile(path_to_load);
        selectedFile = filename;
        logToConsole("Loaded file: " + filename);
        logToConsole("Jobs: " + std::to_string(currentProblem->numJobs) + ", Machines: " + std::to_string(currentProblem->numMachines));
        currentResult = nullptr; // Reset result
    } catch (const std::exception& e) {
        logToConsole("Error loading file: " + std::string(e.what()));
    }
}

// Solve the current problem with selected algorithm.
void BaseUI::solve() {
    if (!currentProblem) {
        logToConsole("Error: No problem loaded.");
        return;
    }
    logToConsole("Solving with " + std::string(selectedAlgo == SchedulingAlgorithm::FIFO ? "FIFO" : 
                                              selectedAlgo == SchedulingAlgorithm::SPT ? "SPT" : 
                                              "LPT") + "...");
    
    // Force draw to show progress
    window.clear(colorBg);
    drawHeader();
    drawSidebar();
    drawConsole();
    window.display();
    
    auto solver = std::make_shared<Solver>(selectedAlgo);
    currentResult = solver->solve(currentProblem);
    logToConsole("Solved! Makespan: " + std::to_string(currentResult->makespan));
    currentView = ViewMode::GanttChart; // Switch to Gantt
}

// Main loop.
void BaseUI::run() {
    while (window.isOpen()) {
        handleInput();
        update(sf::Vector2f(sf::Mouse::getPosition(window)));
        draw();
    }
}

// Show message in console.
void BaseUI::showMessage(const std::string& title, const std::string& message) {
    logToConsole("[" + title + "] " + message);
}

// Export Gantt chart interactively.
void BaseUI::exportGanttChartInteractive() {
    if (!currentResult) {
        logToConsole("Error: No results to export.");
        return;
    }
    
    std::string homeDir = std::getenv("HOME");
    std::string ganttDir = homeDir + "/Pictures/GANTT_GRAPHS";
    std::filesystem::create_directories(ganttDir);
    
    // Generate filename with timestamp
    std::string filename = ganttDir + "/gantt_" + std::to_string(std::time(nullptr)) + ".png";
    
    logToConsole("Exporting Gantt chart to PNG...");
    
    try {
        // Use GanttChartMaker to save the chart
        auto ganttMaker = std::make_shared<GanttChartMaker>();
        ganttMaker->saveToFile(currentResult, filename);
        logToConsole("Gantt chart exported to: " + filename);
    } catch (const std::exception& e) {
        logToConsole("Error exporting Gantt chart: " + std::string(e.what()));
    }
}

// Export solution to file.
void BaseUI::exportSolutionInteractive() {
    if (!currentResult) {
        logToConsole("Error: No results to export.");
        return;
    }
    std::string homeDir = std::getenv("HOME");
    std::string solutionsDir = homeDir + "/Documents/JSSP_SOLUTIONS";
    std::filesystem::create_directories(solutionsDir);
    std::string baseFilename = solutionsDir + "/solution_" + std::to_string(std::time(nullptr));
    
    logToConsole("Exporting solution in multiple formats...");
    
    int successCount = 0;
    
    // Export as TEXT
    try {
        SolutionSerializer::exportSolution(currentResult, baseFilename + ".txt", ExportFormat::TEXT);
        logToConsole("[OK] Exported TEXT: " + baseFilename + ".txt");
        successCount++;
    } catch (const std::exception& e) {
        logToConsole("[FAIL] TEXT export failed: " + std::string(e.what()));
    }
    
    // Export as JSON
    try {
        SolutionSerializer::exportSolution(currentResult, baseFilename + ".json", ExportFormat::JSON);
        logToConsole("[OK] Exported JSON: " + baseFilename + ".json");
        successCount++;
    } catch (const std::exception& e) {
        logToConsole("[FAIL] JSON export failed: " + std::string(e.what()));
    }
    
    // Export as XML
    try {
        SolutionSerializer::exportSolution(currentResult, baseFilename + ".xml", ExportFormat::XML);
        logToConsole("[OK] Exported XML: " + baseFilename + ".xml");
        successCount++;
    } catch (const std::exception& e) {
        logToConsole("[FAIL] XML export failed: " + std::string(e.what()));
    }
    
    if (successCount > 0) {
        logToConsole("Successfully exported " + std::to_string(successCount) + "/3 formats to: " + solutionsDir);
    } else {
        logToConsole("Error: All exports failed.");
    }
}

// Open file browser for problem file.
void BaseUI::browseForFile() {
    std::string defaultPath = "../data";
    if (!std::filesystem::exists(defaultPath)) {
        defaultPath = ".";
    }
    
    std::string command = "zenity --file-selection --title=\"Select JSSP Problem File\" "
                         "--filename=\"" + defaultPath + "/\" "
                         "--file-filter=\"JSSP files (*.jssp) | *.jssp\" "
                         "--file-filter=\"All files | *\" 2>/dev/null";
    
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) {
        logToConsole("Error: Could not open file dialog.");
        return;
    }
    
    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }
    
    if (!result.empty() && result.back() == '\n') {
        result.pop_back();
    }
    
    if (!result.empty()) {
        loadFile(result);
    }
}

// Load solution from file.
void BaseUI::loadSolutionInteractive() {
    logToConsole("Please use the file dialog to load a solution.");
    std::string command = "zenity --file-selection --title=\"Load JSSP Solution\" --file-filter=\"*.txt *.json *.xml\" 2>/dev/null";
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) return;
    char buffer[128];
    std::string result = "";
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) result += buffer;
    if (!result.empty() && result.back() == '\n') result.pop_back();
    if (!result.empty()) {
        try {
            currentResult = Parser::loadSolution(result);
            if (currentResult) {
                logToConsole("Solution loaded. Makespan: " + std::to_string(currentResult->makespan));
                currentView = ViewMode::GanttChart;
            }
        } catch (const std::exception& e) {
            logToConsole("Error: " + std::string(e.what()));
        }
    }
}
