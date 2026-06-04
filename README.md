# NetworkSimulator

Симулятор компьютерной сети (C++) — учебное десктопное/CLI приложение для моделирования передачи пакетов, протокола CSMA/CD и IP-маршрутизации.

## Требования
- Компилятор с поддержкой C++17
- CMake (>= 3.10)
- Git (для загрузки Catch2)

## Инструкция по сборке

### Локальная сборка
1. Сконфигурируйте проект с помощью CMake:
   ```bash
   cmake -S . -B build
   ```
2. Соберите проект:
   ```bash
   cmake --build build
   ```
3. Запустите приложение:
   ```bash
   cd build && ./NetworkSimulator
   ```

### Тестирование
Для запуска unit-тестов и тестовых сценариев (Catch2):
```bash
cd build && ctest --output-on-failure
```

### Сборка и запуск через Docker
Проект также поддерживает многостадийную сборку через Docker:

1. Сборка Docker-образа (автоматически запускает внутренние тесты на стадии tester):
   ```bash
   docker build -t network-simulator .
   ```
2. Запуск приложения в контейнере:
   ```bash
   docker run --rm network-simulator
   ```

## Архитектура
В проекте реализованы базовые классы: `Node`, `Host`, `Router`, `Link` и симуляция протоколов `CSMACDMedium`. Реализованы шаблоны Observer, Strategy, Singleton и Factory Method. Узлы загружаются из конфигурационного файла, генерируют трафик и собирают статистику через `StatisticsCollector`.
