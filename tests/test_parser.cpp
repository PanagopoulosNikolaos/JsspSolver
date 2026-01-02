#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <memory>
#include "parser.hpp"
#include "models.hpp"

class ParserTest : public ::testing::Test {
protected:
    /**
     * SetUp method for test fixture.
     */
    void SetUp() override {
        // Create temporary test files
        createTestFiles();
    }

    /**
     * TearDown method for test fixture.
     */
    void TearDown() override {
        // Clean up test files
        cleanupTestFiles();
    }

    /**
     * Creates test data files.
     */
    void createTestFiles() {
        // Simple test file (3x3)
        std::ofstream simple("test_simple.jssp");
        simple << "3 3\n";
        simple << "0 0 2\n";
        simple << "0 1 3\n";
        simple << "0 2 1\n";
        simple << "1 1 1\n";
        simple << "1 2 2\n";
        simple << "1 0 3\n";
        simple << "2 2 3\n";
        simple << "2 0 1\n";
        simple << "2 1 2\n";
        simple.close();
        
        // Medium test file (2x2)
        std::ofstream medium("test_medium.jssp");
        medium << "2 2\n";
        medium << "0 0 5\n";
        medium << "0 1 3\n";
        medium << "1 1 2\n";
        medium << "1 0 4\n";
        medium.close();
        
        // Invalid test file (negative processing time)
        std::ofstream invalid("test_invalid.jssp");
        invalid << "2 2\n";
        invalid << "0 0 -1\n";  // Invalid negative time
        invalid << "1 1 2\n";
        invalid.close();
        
        // Empty test file
        std::ofstream empty("test_empty.jssp");
        empty.close();
    }

    /**
     * Cleans up test data files.
     */
    void cleanupTestFiles() {
        remove("test_simple.jssp");
        remove("test_medium.jssp");
        remove("test_invalid.jssp");
        remove("test_empty.jssp");
        remove("test_output.jssp");
    }
};

// File parsing tests
TEST_F(ParserTest, ParseSimpleFile) {
    auto problem = Parser::parseFile("test_simple.jssp");
    
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 3);
    EXPECT_EQ(problem->numMachines, 3);
    EXPECT_EQ(problem->getTotalOperations(), 9);
    
    // Check job 0 operations
    auto job0 = problem->getJob(0);
    ASSERT_NE(job0, nullptr);
    EXPECT_EQ(job0->getOperationCount(), 3);
    EXPECT_EQ(job0->getOperation(0)->machineId, 0);
    EXPECT_EQ(job0->getOperation(0)->processingTime, 2);
    EXPECT_EQ(job0->getOperation(1)->machineId, 1);
    EXPECT_EQ(job0->getOperation(1)->processingTime, 3);
    EXPECT_EQ(job0->getOperation(2)->machineId, 2);
    EXPECT_EQ(job0->getOperation(2)->processingTime, 1);
    
    // Check job 1 operations
    auto job1 = problem->getJob(1);
    ASSERT_NE(job1, nullptr);
    EXPECT_EQ(job1->getOperationCount(), 3);
    EXPECT_EQ(job1->getOperation(0)->machineId, 1);
    EXPECT_EQ(job1->getOperation(0)->processingTime, 1);
    EXPECT_EQ(job1->getOperation(1)->machineId, 2);
    EXPECT_EQ(job1->getOperation(1)->processingTime, 2);
    EXPECT_EQ(job1->getOperation(2)->machineId, 0);
    EXPECT_EQ(job1->getOperation(2)->processingTime, 3);
}

TEST_F(ParserTest, ParseMediumFile) {
    auto problem = Parser::parseFile("test_medium.jssp");
    
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 2);
    EXPECT_EQ(problem->numMachines, 2);
    EXPECT_EQ(problem->getTotalOperations(), 4);
    
    auto job0 = problem->getJob(0);
    auto job1 = problem->getJob(1);
    
    ASSERT_NE(job0, nullptr);
    ASSERT_NE(job1, nullptr);
    
    EXPECT_EQ(job0->getOperation(0)->machineId, 0);
    EXPECT_EQ(job0->getOperation(0)->processingTime, 5);
    EXPECT_EQ(job0->getOperation(1)->machineId, 1);
    EXPECT_EQ(job0->getOperation(1)->processingTime, 3);
    
    EXPECT_EQ(job1->getOperation(0)->machineId, 1);
    EXPECT_EQ(job1->getOperation(0)->processingTime, 2);
    EXPECT_EQ(job1->getOperation(1)->machineId, 0);
    EXPECT_EQ(job1->getOperation(1)->processingTime, 4);
}

TEST_F(ParserTest, ParseFileNotFound) {
    EXPECT_THROW({
        try {
            Parser::parseFile("nonexistent_file.jssp");
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ("Could not open file: nonexistent_file.jssp", e.what());
            throw;
        }
    }, std::runtime_error);
}

TEST_F(ParserTest, ParseInvalidDimensions) {
    // Create a file with invalid dimensions
    std::ofstream bad("test_bad.jssp");
    bad << "-1 3\n";  // Invalid negative jobs
    bad.close();
    
    EXPECT_THROW({
        try {
            Parser::parseFile("test_bad.jssp");
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ("Invalid number of jobs or machines", e.what());
            throw;
        }
    }, std::runtime_error);
    
    remove("test_bad.jssp");
}

TEST_F(ParserTest, ParseInvalidOperations) {
    // Create a file with invalid operation data
    std::ofstream bad("test_bad_ops.jssp");
    bad << "2 2\n";
    bad << "0 0 5\n";
    bad << "3 1 2\n";  // Invalid job ID
    bad << "1 2 3\n";  // Invalid machine ID
    bad << "1 1 -2\n"; // Invalid negative time
    bad.close();
    
    // Should parse but warn about invalid operations
    auto problem = Parser::parseFile("test_bad_ops.jssp");
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->getTotalOperations(), 1); // Only first operation should be valid
    
    remove("test_bad_ops.jssp");
}

TEST_F(ParserTest, ParseEmptyFile) {
    EXPECT_THROW({
        try {
            Parser::parseFile("test_empty.jssp");
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ("No valid operations found in file", e.what());
            throw;
        }
    }, std::runtime_error);
}

// String parsing tests
TEST_F(ParserTest, ParseString) {
    std::string data = "2 2\n0 0 5\n0 1 3\n1 1 2\n1 0 4\n";
    auto problem = Parser::parseString(data);
    
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 2);
    EXPECT_EQ(problem->numMachines, 2);
    EXPECT_EQ(problem->getTotalOperations(), 4);
}

TEST_F(ParserTest, ParseStringInvalidDimensions) {
    std::string data = "-1 2\n0 0 5\n";
    
    EXPECT_THROW({
        try {
            Parser::parseString(data);
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ("Invalid number of jobs or machines", e.what());
            throw;
        }
    }, std::runtime_error);
}

// Save to file tests
TEST_F(ParserTest, SaveToFile) {
    auto problem = Parser::generateSimpleProblem();
    
    EXPECT_NO_THROW({
        Parser::saveToFile(problem, "test_output.jssp");
    });
    
    // Verify file was created and can be parsed back
    auto loaded = Parser::parseFile("test_output.jssp");
    ASSERT_NE(loaded, nullptr);
    EXPECT_EQ(loaded->numJobs, 3);
    EXPECT_EQ(loaded->numMachines, 3);
    EXPECT_EQ(loaded->getTotalOperations(), 9);
}

TEST_F(ParserTest, SaveToFileInvalidPath) {
    auto problem = Parser::generateSimpleProblem();
    
    EXPECT_THROW({
        try {
            Parser::saveToFile(problem, "/invalid/path/test.jssp");
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ("Could not create file: /invalid/path/test.jssp", e.what());
            throw;
        }
    }, std::runtime_error);
}

// Generate simple problem tests
TEST_F(ParserTest, GenerateSimpleProblem) {
    auto problem = Parser::generateSimpleProblem();
    
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 3);
    EXPECT_EQ(problem->numMachines, 3);
    EXPECT_EQ(problem->getTotalOperations(), 9);
    
    // Verify specific operations
    auto job0 = problem->getJob(0);
    auto job1 = problem->getJob(1);
    auto job2 = problem->getJob(2);
    
    ASSERT_NE(job0, nullptr);
    ASSERT_NE(job1, nullptr);
    ASSERT_NE(job2, nullptr);
    
    // Job 0: M0(2), M1(3), M2(1)
    EXPECT_EQ(job0->getOperation(0)->machineId, 0);
    EXPECT_EQ(job0->getOperation(0)->processingTime, 2);
    EXPECT_EQ(job0->getOperation(1)->machineId, 1);
    EXPECT_EQ(job0->getOperation(1)->processingTime, 3);
    EXPECT_EQ(job0->getOperation(2)->machineId, 2);
    EXPECT_EQ(job0->getOperation(2)->processingTime, 1);
    
    // Job 1: M1(1), M2(2), M0(3)
    EXPECT_EQ(job1->getOperation(0)->machineId, 1);
    EXPECT_EQ(job1->getOperation(0)->processingTime, 1);
    EXPECT_EQ(job1->getOperation(1)->machineId, 2);
    EXPECT_EQ(job1->getOperation(1)->processingTime, 2);
    EXPECT_EQ(job1->getOperation(2)->machineId, 0);
    EXPECT_EQ(job1->getOperation(2)->processingTime, 3);
    
    // Job 2: M2(3), M0(1), M1(2)
    EXPECT_EQ(job2->getOperation(0)->machineId, 2);
    EXPECT_EQ(job2->getOperation(0)->processingTime, 3);
    EXPECT_EQ(job2->getOperation(1)->machineId, 0);
    EXPECT_EQ(job2->getOperation(1)->processingTime, 1);
    EXPECT_EQ(job2->getOperation(2)->machineId, 1);
    EXPECT_EQ(job2->getOperation(2)->processingTime, 2);
}

// Edge case tests
TEST_F(ParserTest, ParseStringEmpty) {
    std::string empty = "";
    
    EXPECT_THROW({
        try {
            Parser::parseString(empty);
        } catch (const std::runtime_error& e) {
            EXPECT_STREQ("Invalid number of jobs or machines", e.what());
            throw;
        }
    }, std::runtime_error);
}

TEST_F(ParserTest, ParseStringOnlyDimensions) {
    std::string data = "2 2\n";
    auto problem = Parser::parseString(data);
    
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 2);
    EXPECT_EQ(problem->numMachines, 2);
    EXPECT_EQ(problem->getTotalOperations(), 0);
}

TEST_F(ParserTest, ParseStringExtraWhitespace) {
    std::string data = "  2  2  \n  0  0  5  \n  1  1  3  \n  ";
    auto problem = Parser::parseString(data);
    
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 2);
    EXPECT_EQ(problem->numMachines, 2);
    EXPECT_EQ(problem->getTotalOperations(), 2);
}
