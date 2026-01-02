#include "base_ui.hpp"
#include <iostream>

/**
 * Main entry point for the JSSP application.
 *
 * Returns:
 *   0 on success, 1 on error.
 */
int main() {
    try {
        std::cout << "Starting JSSP Solver..." << std::endl;
        BaseUI ui;
        ui.run();
        std::cout << "JSSP Solver closed." << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
