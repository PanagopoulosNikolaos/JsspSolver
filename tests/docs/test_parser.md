# JSSP Solver Test Documentation: Parser Tests

## Overview

The `test_parser.cpp` file contains comprehensive unit tests for the parser module of the Job Shop Scheduling Problem (JSSP) solver. These tests verify the correct implementation of file parsing, string parsing, and data validation functionalities. The parser module is responsible for reading JSSP problem instances from files and converting them into internal data structures that can be processed by the solver.

Key responsibilities of the parser include:
- Reading JSSP problem files in the standard format
- Validating problem dimensions (number of jobs and machines)
- Parsing operation specifications (job ID, machine ID, processing time)
- Handling various edge cases and error conditions
- Generating sample problems for testing purposes
- Saving problem instances back to files

## Test Structure

All tests are organized within the `ParserTest` class which inherits from `::testing::Test`. This provides a common setup and teardown mechanism for all parser-related tests. The fixture includes helper methods for creating and cleaning up temporary test files.

### Setup Method

```cpp
void SetUp() override {
    // Create temporary test files
    createTestFiles();
}
```

The setup method calls `createTestFiles()` to prepare various test data files with different characteristics.

### Teardown Method

```cpp
void TearDown() override {
    // Clean up test files
    cleanupTestFiles();
}
```

The teardown method ensures that all temporary test files are removed after each test run.

### Helper Methods

The test fixture includes helper methods for managing test data:

```cpp
void createTestFiles() {
    // Creates test data files with various characteristics
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
    
    // Additional files for different test scenarios
    // ... (other file creations)
}

void cleanupTestFiles() {
    remove("test_simple.jssp");
    remove("test_medium.jssp");
    remove("test_invalid.jssp");
    remove("test_empty.jssp");
    remove("test_output.jssp");
}
```

## File Parsing Tests

### Parse Simple File Test

**Test Name**: `ParseSimpleFile`

This test verifies that the parser correctly reads and interprets a simple 3x3 JSSP problem file:

```cpp
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
    // ... additional operation checks
}
```

The test confirms that:
- The parser returns a valid problem instance pointer
- The number of jobs and machines is correctly parsed
- All operations are loaded (9 operations for a 3x3 problem)
- Each operation has the correct machine ID and processing time
- Operations are properly assigned to their respective jobs

### Parse Medium File Test

**Test Name**: `ParseMediumFile`

This test validates parsing of a medium-sized 2x2 problem file:

```cpp
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
    
    // Verify operation assignments and properties
    EXPECT_EQ(job0->getOperation(0)->machineId, 0);
    EXPECT_EQ(job0->getOperation(0)->processingTime, 5);
    // ... additional checks
}
```

The test ensures similar validation for a different problem size.

### Parse File Not Found Test

**Test Name**: `ParseFileNotFound`

This test verifies proper error handling when attempting to parse a non-existent file:

```cpp
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
```

The test confirms that:
- An exception is thrown when the file doesn't exist
- The exception message contains the correct error information
- The error message includes the filename that couldn't be opened

### Parse Invalid Dimensions Test

**Test Name**: `ParseInvalidDimensions`

This test validates error handling for files with invalid dimension specifications:

```cpp
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
```

The test ensures that:
- Negative or invalid dimension values trigger appropriate exceptions
- The error message clearly indicates the nature of the problem
- Invalid files are handled gracefully without crashing

### Parse Invalid Operations Test

**Test Name**: `ParseInvalidOperations`

This test checks how the parser handles files with invalid operation data:

```cpp
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
```

The test verifies that:
- Operations with invalid job IDs are rejected
- Operations with invalid machine IDs are rejected
- Operations with negative processing times are rejected
- Valid operations are still processed despite invalid ones in the same file

### Parse Empty File Test

**Test Name**: `ParseEmptyFile`

This test ensures proper handling of empty files:

```cpp
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
```

The test confirms that empty files trigger an appropriate exception with a descriptive message.

## String Parsing Tests

### Parse String Test

**Test Name**: `ParseString`

This test verifies that the parser can correctly parse problem data from a string:

```cpp
TEST_F(ParserTest, ParseString) {
    std::string data = "2 2\n0 0 5\n0 1 3\n1 1 2\n1 0 4\n";
    auto problem = Parser::parseString(data);
    
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 2);
    EXPECT_EQ(problem->numMachines, 2);
    EXPECT_EQ(problem->getTotalOperations(), 4);
}
```

The test ensures that:
- String parsing produces the same results as file parsing
- All operations are correctly extracted from the string
- Problem dimensions are correctly interpreted

### Parse String Invalid Dimensions Test

**Test Name**: `ParseStringInvalidDimensions`

This test validates error handling for strings with invalid dimension specifications:

```cpp
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
```

The test confirms that string parsing applies the same validation as file parsing.

## Save to File Tests

### Save to File Test

**Test Name**: `SaveToFile`

This test verifies the ability to save a problem instance to a file and reload it:

```cpp
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
```

The test ensures that:
- The save operation doesn't throw exceptions
- Saved files can be successfully parsed back
- The loaded problem maintains the same properties as the original

### Save to File Invalid Path Test

**Test Name**: `SaveToFileInvalidPath`

This test validates error handling when saving to an invalid path:

```cpp
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
```

The test confirms that invalid save paths trigger appropriate exceptions.

## Generate Simple Problem Tests

### Generate Simple Problem Test

**Test Name**: `GenerateSimpleProblem`

This test validates the functionality for generating a simple problem programmatically:

```cpp
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
    // ... additional verification
}
```

The test ensures that generated problems have the expected structure and content.

## Edge Case Tests

### Parse String Empty Test

**Test Name**: `ParseStringEmpty`

This test handles the edge case of parsing an empty string:

```cpp
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
```

### Parse String Only Dimensions Test

**Test Name**: `ParseStringOnlyDimensions`

This test handles parsing strings that contain only dimension information:

```cpp
TEST_F(ParserTest, ParseStringOnlyDimensions) {
    std::string data = "2 2\n";
    auto problem = Parser::parseString(data);
    
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 2);
    EXPECT_EQ(problem->numMachines, 2);
    EXPECT_EQ(problem->getTotalOperations(), 0);
}
```

The test confirms that problems with no operations are handled correctly.

### Parse String Extra Whitespace Test

**Test Name**: `ParseStringExtraWhitespace`

This test verifies robustness against extra whitespace in input:

```cpp
TEST_F(ParserTest, ParseStringExtraWhitespace) {
    std::string data = "  2  2  \n  0  0  5 \n  1  1  3  \n  ";
    auto problem = Parser::parseString(data);
    
    ASSERT_NE(problem, nullptr);
    EXPECT_EQ(problem->numJobs, 2);
    EXPECT_EQ(problem->numMachines, 2);
    EXPECT_EQ(problem->getTotalOperations(), 2);
}
```

The test ensures that the parser can handle irregular spacing in the input data.

## Conclusion

The parser tests provide comprehensive coverage of the file and string parsing functionality in the JSSP solver. They verify correct parsing of valid inputs, proper error handling for various invalid inputs, and the ability to save and restore problem instances. These tests ensure that the parser module reliably converts external JSSP problem specifications into internal data structures, which is crucial for the overall functionality of the solver system.