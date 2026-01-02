#ifndef PARSER_HPP
#define PARSER_HPP

#include "models.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Parser {
public:
    /**
     * Parses a JSSP instance from file.
     *
     * Format:
     * First line: num_jobs num_machines
     * Following lines: job_id machine_id processing_time (one line per operation)
     *
     * Args:
     *   filename: Path to input file.
     *
     * Returns:
     *   Parsed problem instance.
     */
    static std::shared_ptr<ProblemInstance> parseFile(const std::string& filename);

    /**
     * Parses a problem instance from string format.
     *
     * Args:
     *   data: String containing problem data.
     *
     * Returns:
     *   Parsed problem instance.
     */
    static std::shared_ptr<ProblemInstance> parseString(const std::string& data);

    /**
     * Saves a problem instance to file for debugging/testing.
     *
     * Args:
     *   problem: Problem instance to save.
     *   filename: Output file path.
     */
    static void saveToFile(const std::shared_ptr<ProblemInstance>& problem, const std::string& filename);

    /**
     * Generates a simple test problem programmatically.
     *
     * Returns:
     *   Generated problem instance.
     */
    static std::shared_ptr<ProblemInstance> generateSimpleProblem();

    /**
     * Loads a solution from a file. Supports TEXT, JSON, and XML formats.
     *
     * Args:
     *   filename: Path to solution file.
     *
     * Returns:
     *   Loaded schedule result.
     */
    static std::shared_ptr<ScheduleResult> loadSolution(const std::string& filename);

    /**
     * Loads a solution from TEXT format.
     *
     * Args:
     *   filename: Path to TEXT solution file.
     *
     * Returns:
     *   Loaded schedule result.
     */
    static std::shared_ptr<ScheduleResult> loadTextSolution(const std::string& filename);

    /**
     * Loads a solution from JSON format.
     *
     * Args:
     *   filename: Path to JSON solution file.
     *
     * Returns:
     *   Loaded schedule result.
     */
    static std::shared_ptr<ScheduleResult> loadJSONSolution(const std::string& filename);

    /**
     * Loads a solution from XML format.
     *
     * Args:
     *   filename: Path to XML solution file.
     *
     * Returns:
     *   Loaded schedule result.
     */
    static std::shared_ptr<ScheduleResult> loadXMLSolution(const std::string& filename);

private:
    /**
     * Helper function to extract integer from XML tag.
     *
     * Args:
     *   content: XML content string.
     *   tag: XML tag name.
     *
     * Returns:
     *   Extracted integer value.
     */
    static int extractIntFromXML(const std::string& content, const std::string& tag);

    /**
     * Helper function to extract double from XML tag.
     *
     * Args:
     *   content: XML content string.
     *   tag: XML tag name.
     *
     * Returns:
     *   Extracted double value.
     */
    static double extractDoubleFromXML(const std::string& content, const std::string& tag);
};

#endif // PARSER_HPP
