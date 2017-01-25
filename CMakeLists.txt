cmake_minimum_required(VERSION 2.8)
project(mc-pricer CXX)

# Prefix
if (NOT PREFIX)
    set(PREFIX ${CMAKE_CURRENT_BINARY_DIR})
endif (NOT PREFIX)
set(CMAKE_INSTALL_PREFIX ${PREFIX} CACHE STRING "Installation prefix." FORCE)

if (CXX)
    set(CMAKE_CXX_COMPILER ${CXX})
endif (CXX)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

# Release or Debug
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "Setting build type to 'Debug' as none was specified.")
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif ()


# Detect OMP
find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS ${CMAKE_C_CFLAGS} ${OpenMP_C_FLAGS})
    set (CMAKE_CXX_FLAGS ${CMAKE_CXX_CFLAGS} ${OpenMP_CXX_FLAGS})
else (OPENMP_FOUND)
    message(STATUS "OpenMP not found.")
endif (OPENMP_FOUND)

# Detect Boost
set(Boost_NO_BOOST_CMAKE ON)
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
message(STATUS "Boost include directory ${Boost_INCLUDE_DIR}")

# Detect PNL
find_package(Pnl REQUIRED)
set(LIBS ${LIBS} ${PNL_LIBRARIES})
include_directories(${PNL_INCLUDE_DIRS})
if(${CMAKE_BUILD_TYPE} STREQUAL "Release")
    add_definitions(-DPNL_RANGE_CHECK_OFF)
endif()

# Add math lib
set(LIBS ${LIBS} m)

# Testing Actigo
add_executable(actigo-test src/Actigo.cpp src/Utils.cpp src/BlackScholesModel.cpp src/MonteCarloPricer.cpp src/test_Actigo.cpp src/SimulationHedger.cpp src/Model.cpp)
target_link_libraries(actigo-test ${LIBS})

# Testing Vanilla Call
add_executable(call-test src/BasketOption.cpp src/Utils.cpp src/BlackScholesModel.cpp src/MonteCarloPricer.cpp src/test_Call.cpp src/SimulationHedger.cpp src/Model.cpp)
target_link_libraries(call-test ${LIBS})

# Testing Asian Option
add_executable(asian-test src/AsianOption.cpp src/Utils.cpp src/BlackScholesModel.cpp src/MonteCarloPricer.cpp src/test_AsianOption.cpp src/SimulationHedger.cpp src/Model.cpp)
target_link_libraries(asian-test ${LIBS})

# Testing Basket Option
add_executable(basket-test src/BasketOption.cpp src/Utils.cpp src/BlackScholesModel.cpp src/MonteCarloPricer.cpp src/test_BasketOption.cpp src/SimulationHedger.cpp src/Model.cpp)
target_link_libraries(basket-test ${LIBS})

# Testing Quanto Option
add_executable(quanto-test src/QuantoOption.cpp src/Utils.cpp src/BlackScholesModel.cpp src/MonteCarloPricer.cpp src/test_QuantoOption.cpp src/SimulationHedger.cpp src/Model.cpp)
target_link_libraries(quanto-test ${LIBS})

# Testing Performance Option
add_executable(performance-test src/PerformanceOption.cpp src/Utils.cpp src/BlackScholesModel.cpp src/MonteCarloPricer.cpp src/test_PerformanceOption.cpp src/SimulationHedger.cpp src/Model.cpp)
target_link_libraries(performance-test ${LIBS})

# Testing Utils
add_executable(utils-test src/Utils.cpp src/test_Utils.cpp)
target_link_libraries(utils-test ${LIBS})

# Testing Black Scholes Model
add_executable(blackscholes-test src/test_BlackScholesModel.cpp src/BlackScholesModel.cpp src/Model.cpp src/Utils.cpp)
target_link_libraries(blackscholes-test ${LIBS})


# Print compiler flags
get_directory_property(DirDefs COMPILE_DEFINITIONS)
message(STATUS "COMPILE_DEFINITIONS = ${DirDefs}")
if (${CMAKE_BUILD_TYPE} STREQUAL "Release")
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_LINKER_EXE_FLAGS}" )
else()
    message(STATUS "C++ Flags: ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} ${CMAKE_LINKER_EXE_FLAGS}" )
endif()
