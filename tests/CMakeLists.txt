cmake_minimum_required(VERSION 3.10)
project(JSSPTests)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Find Google Test
find_package(GTest REQUIRED)

# Create test executable
add_executable(JSSPTests
    test_models.cpp
    test_parser.cpp
    test_solver.cpp
    test_gantt_maker.cpp
    test_integration.cpp
    ../src/models.cpp
    ../src/parser.cpp
    ../src/solver.cpp
    ../src/gantt_maker.cpp
    ../src/solution_serializer.cpp
    ../ui/base_ui.cpp
)

# Include directories
target_include_directories(JSSPTests PRIVATE ../include)
target_include_directories(JSSPTests PRIVATE ${GTEST_INCLUDE_DIRS})

# Link libraries
target_link_libraries(JSSPTests PRIVATE 
    ${GTEST_LIBRARIES} 
    ${GTEST_MAIN_LIBRARIES}
    sfml-graphics 
    sfml-window 
    sfml-system
)

# Enable warnings
target_compile_options(JSSPTests PRIVATE -Wall -Wextra -Wpedantic)

# Add test executable to CTest
include(GoogleTest)
gtest_discover_tests(JSSPTests)
