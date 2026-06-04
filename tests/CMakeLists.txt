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

# Сценарий CSMA/CD коллизия
add_executable(scenario_csma_collision scenarios/scenario_csma_collision.cpp)
target_link_libraries(scenario_csma_collision PRIVATE Catch2::Catch2WithMain NetworkSimulatorLib)
target_include_directories(scenario_csma_collision PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Сценарий IP маршрутизация
add_executable(scenario_ip_routing scenarios/scenario_ip_routing.cpp)
target_link_libraries(scenario_ip_routing PRIVATE Catch2::Catch2WithMain NetworkSimulatorLib)
target_include_directories(scenario_ip_routing PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Сценарий Сохранение и загрузка
add_executable(scenario_save_load scenarios/scenario_save_load.cpp)
target_link_libraries(scenario_save_load PRIVATE Catch2::Catch2WithMain NetworkSimulatorLib)
target_include_directories(scenario_save_load PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Сценарий Статистика
add_executable(scenario_statistics scenarios/scenario_statistics.cpp)
target_link_libraries(scenario_statistics PRIVATE Catch2::Catch2WithMain NetworkSimulatorLib)
target_include_directories(scenario_statistics PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Сценарий Headless режина 200 узлов
add_executable(scenario_headless_200 scenarios/scenario_headless_200.cpp)
target_link_libraries(scenario_headless_200 PRIVATE Catch2::Catch2WithMain NetworkSimulatorLib)
target_include_directories(scenario_headless_200 PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Регистрируем тесты для CTest
include(Catch)
catch_discover_tests(runUnitTests)
catch_discover_tests(scenario_csma_collision)
catch_discover_tests(scenario_ip_routing)
catch_discover_tests(scenario_save_load)
catch_discover_tests(scenario_statistics)
catch_discover_tests(scenario_headless_200)