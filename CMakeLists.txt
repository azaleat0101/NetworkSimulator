cmake_minimum_required(VERSION 3.10)
project(NetworkSimulator VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    src/main.cpp
    src/core/SimulationEngine.cpp
    src/devices/Node.cpp
    src/devices/Host.cpp
    src/devices/Router.cpp
    src/devices/Hub.cpp
    src/network/Network.cpp
    src/network/Link.cpp
    src/protocols/CSMACDStrategy.cpp
    src/protocols/PointToPointStrategy.cpp
    src/protocols/IPRouting.cpp
    src/utils/Logger.cpp
    src/utils/StatisticsCollector.cpp
    src/utils/ConfigLoader.cpp
    src/utils/ConsoleRenderer.cpp
    src/utils/ReportExporter.cpp
)

add_library(NetworkSimulatorLib STATIC ${SOURCES})
target_include_directories(NetworkSimulatorLib PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_executable(netsim src/main.cpp)
target_link_libraries(netsim PRIVATE NetworkSimulatorLib)

enable_testing()
add_subdirectory(tests)