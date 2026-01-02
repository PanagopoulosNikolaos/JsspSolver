#ifndef SOLUTION_SERIALIZER_HPP
#define SOLUTION_SERIALIZER_HPP

#include "models.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/**
 * Enumeration for export formats.
 */
enum class ExportFormat {
    TEXT,
    JSON,
    XML
};

/**
 * Class for serializing schedule results to various formats.
 */
class SolutionSerializer {
public:
    /**
     * Exports a ScheduleResult to a file in the specified format.
     *
     * Args:
     *   result: Schedule result to export.
     *   filename: Output file path.
     *   format: Export format.
     */
    static void exportSolution(const std::shared_ptr<ScheduleResult>& result, 
                              const std::string& filename, 
                              ExportFormat format);
    
    /**
     * Exports a ScheduleResult to text format.
     *
     * Args:
     *   result: Schedule result to export.
     *   filename: Output file path.
     */
    static void exportText(const std::shared_ptr<ScheduleResult>& result, 
                          const std::string& filename);
    
    /**
     * Exports a ScheduleResult to JSON format.
     *
     * Args:
     *   result: Schedule result to export.
     *   filename: Output file path.
     */
    static void exportJSON(const std::shared_ptr<ScheduleResult>& result, 
                          const std::string& filename);
    
    /**
     * Exports a ScheduleResult to XML format.
     *
     * Args:
     *   result: Schedule result to export.
     *   filename: Output file path.
     */
    static void exportXML(const std::shared_ptr<ScheduleResult>& result, 
                         const std::string& filename);
    
    /**
     * Detects format from filename extension.
     *
     * Args:
     *   filename: File path.
     *
     * Returns:
     *   Detected export format.
     */
    static ExportFormat detectFormat(const std::string& filename);
    
    /**
     * Gets format name for display.
     *
     * Args:
     *   format: Export format.
     *
     * Returns:
     *   Format name string.
     */
    static std::string getFormatName(ExportFormat format);
};

#endif // SOLUTION_SERIALIZER_HPP
