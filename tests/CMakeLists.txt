# Загружаем Catch2
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.4.0
)
FetchContent_MakeAvailable(Catch2)

# Список тестовых исходников
set(UNIT_TESTS
    test_logger.cpp
    test_statistics.cpp
    test_node.cpp
    test_host.cpp
    test_router.cpp
    test_hub.cpp
    test_link.cpp
    test_network.cpp
    test_csmacd.cpp
    test_iprouting.cpp
    test_configloader.cpp
)

# Исполняемый файл для unit-тестов
add_executable(runUnitTests ${UNIT_TESTS})
target_link_libraries(runUnitTests PRIVATE Catch2::Catch2WithMain NetworkSimulatorLib)
target_include_directories(runUnitTests PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Сценарий 1: передача между хостами
add_executable(scenario1 scenario1.cpp)
target_link_libraries(scenario1 PRIVATE Catch2::Catch2WithMain NetworkSimulatorLib)
target_include_directories(scenario1 PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Сценарий 2: работа с конфигурацией
add_executable(scenario2 scenario2.cpp)
target_link_libraries(scenario2 PRIVATE Catch2::Catch2WithMain NetworkSimulatorLib)
target_include_directories(scenario2 PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Регистрируем тесты для CTest
include(Catch)
catch_discover_tests(runUnitTests)
catch_discover_tests(scenario1)
catch_discover_tests(scenario2)