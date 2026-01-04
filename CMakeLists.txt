cmake_minimum_required(VERSION 3.10)
project(JSSPSolver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find SFML
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Find nlohmann/json
find_package(nlohmann_json 3.2.0 REQUIRED)

# Create main executable
add_executable(JSPSolver
    src/main.cpp
    src/models.cpp
    src/parser.cpp
    src/solver.cpp
    src/gantt_maker.cpp
    src/solution_serializer.cpp
    ui/base_ui.cpp
)

# No additional compile definitions needed

# Include directories
target_include_directories(JSPSolver PRIVATE include)

# Link SFML libraries
target_link_libraries(JSPSolver PRIVATE sfml-graphics sfml-window sfml-system)

# Enable warnings
target_compile_options(JSPSolver PRIVATE -Wall -Wextra -Wpedantic)

# Add tests if BUILD_TESTS is enabled
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()
    
    # Find Google Test
    find_package(GTest REQUIRED)
    
    # Add test executable
    add_executable(JSSPTests
        tests/test_models.cpp
        tests/test_parser.cpp
        tests/test_solver.cpp
        tests/test_gantt_maker.cpp
        tests/test_integration.cpp
        
        src/models.cpp
        src/parser.cpp
        src/solver.cpp
        src/solution_serializer.cpp
        src/gantt_maker.cpp
        ui/base_ui.cpp
    )
    
    # Include directories for tests
    target_include_directories(JSSPTests PRIVATE include)
    target_include_directories(JSSPTests PRIVATE ${GTEST_INCLUDE_DIRS})
    
    # Link libraries for tests
    target_link_libraries(JSSPTests PRIVATE 
        ${GTEST_LIBRARIES} 
        ${GTEST_MAIN_LIBRARIES}
        sfml-graphics 
        sfml-window 
        sfml-system
    )
    
    # Enable warnings for tests
    target_compile_options(JSSPTests PRIVATE -Wall -Wextra -Wpedantic)
    
    # Add test executable to CTest
    include(GoogleTest)
    gtest_discover_tests(JSSPTests)
    
    # Add a convenience target to build and run tests
    add_custom_target(check
        COMMAND JSSPTests
        DEPENDS JSSPTests
        COMMENT "Running tests..."
        VERBATIM
    )
endif()
