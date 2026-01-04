# JSSP Solver Tests

This directory contains comprehensive tests for the Job Shop Scheduling Problem (JSSP) solver project.

## Test Structure

The test suite is organized into the following categories:

### 1. Unit Tests (`test_*.cpp`)

- **`test_models.cpp`** - Tests for core data models (Operation, Job, Machine, ProblemInstance, ScheduleResult)
- **`test_parser.cpp`** - Tests for file parsing, string parsing, and data validation
- **`test_solver.cpp`** - Tests for scheduling algorithms (FIFO, SPT, LPT)
- **`test_gantt_maker.cpp`** - Tests for visualization components
- **`test_integration.cpp`** - End-to-end workflow tests

## Architecture Integration

The tests validate the separation of interface and implementation in our traditional C++ library structure:

- **Header files** (`include/`) define interfaces that are tested through public methods
- **Implementation files** (`src/`) contain the actual logic being tested
- **Test files** verify that the interface contracts are properly implemented

This separation allows for:
- **Faster compilation** - Test changes don't require rebuilding all dependent components
- **Better encapsulation** - Implementation details are hidden from test code
- **Easier mocking** - Interfaces can be easily mocked for unit testing
- **Library testing** - Headers can be tested independently of implementations

## Running Tests

### Using the test runner script (Recommended)
```bash
chmod +x run_tests.sh
./run_tests.sh
```

### Test Categories

The tests cover the following areas:

#### Models Tests
- Operation creation and scheduling
- Job operations management
- Machine scheduling and state management
- Problem instance setup and validation
- Schedule result metrics calculation
- Edge cases and error conditions

#### Parser Tests
- File parsing with valid and invalid data
- String parsing functionality
- Error handling for malformed files
- File I/O operations
- Data validation and sanitization

#### Solver Tests
- All three scheduling algorithms (FIFO, SPT, LPT)
- Algorithm factory methods
- Algorithm comparison functionality
- Edge cases (empty problems, single operations)
- Performance with larger problems
- Metrics calculation accuracy

#### Gantt Chart Tests
- Visualization component functionality
- Color generation for different jobs
- Layout parameter configuration
- Error handling for invalid schedules
- Integration with other components

#### Integration Tests
- End-to-end workflows
- File I/O pipelines
- Algorithm comparison in real scenarios
- Performance testing with larger problems
- Memory management validation
- Real-world scheduling scenarios

## Test Coverage

The test suite provides comprehensive coverage of:

- ✔️ All core data structures and their methods
- ✔️ File parsing with various edge cases
- ✔️ All scheduling algorithms and their behavior
- ✔️ Error handling and validation
- ✔️ Integration between components
- ✔️ Performance characteristics
- ✔️ Memory management with smart pointers

## Writing New Tests

When adding new functionality, follow these guidelines:

1. **Add unit tests** for new classes or methods in the appropriate `test_*.cpp` file
2. **Add integration tests** for new workflows in `test_integration.cpp`
3. **Use descriptive test names** that explain what is being tested
4. **Test both success and failure cases**
5. **Clean up test data** in `TearDown()` methods
6. **Use appropriate assertions** (`ASSERT_*` for critical conditions, `EXPECT_*` for non-critical)

### Example Test Structure

```cpp
class MyComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Initialize test data
    }
    
    void TearDown() override {
        // Clean up test data
    }
};

TEST_F(MyComponentTest, TestName) {
    // Test implementation
    EXPECT_EQ(result, expected);
}
```

## Continuous Integration

The tests are designed to be run in CI/CD environments. They:
- Have no external dependencies beyond gtest and SFML
- Clean up after themselves automatically
- Provide clear pass/fail feedback
- Include performance regression detection

## Troubleshooting

### Common Issues

1. **Script not executable**: Make sure to run `chmod +x run_tests.sh` first
2. **Dependencies missing**: The script will check for required dependencies
3. **Tests failing**: Check the script output for detailed error messages
4. **Permission issues**: Ensure you have write permissions in the tests directory

## Performance Notes

- Tests are designed to run quickly (typically under 10 seconds)
- Large problem tests are limited to reasonable sizes
- Memory usage is monitored to prevent leaks
- Tests can be run in parallel with CTest
