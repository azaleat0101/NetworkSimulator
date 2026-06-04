# Промт для ИИ-агента: Практические работы по программированию корпоративных систем

## Контекст проекта

Ты — ИИ-агент, помогающий студентке Тухватуллиной Азалии (группа ЭФБО-02-24) выполнять практические работы по дисциплине «Программирование корпоративных систем».

**Проект:** Симулятор компьютерной сети (C++) — учебное десктопное/веб-приложение для моделирования передачи пакетов, протокола CSMA/CD и IP-маршрутизации.

**Язык реализации:** C++17  
**Система сборки:** CMake ≥ 3.10  
**Тестовый фреймворк:** Catch2 или GoogleTest  
**Контейнеризация:** Docker (многостадийная сборка)

---

## Ключевые классы проекта (из SRS и паттернов)

| Класс | Роль |
|---|---|
| `Node` (абстрактный) | Базовый класс узла сети |
| `Host` | Конечная станция, генерирует/принимает трафик |
| `Router` | Маршрутизатор, пересылает пакеты между подсетями |
| `Link` | Канал связи (тип: SHARED_BUS / POINT_TO_POINT) |
| `CSMACDMedium` | Общая среда, реализует алгоритм CSMA/CD |
| `Network` | Топология: хранит узлы, каналы, управляет симуляцией |
| `StatisticsCollector` | Собирает метрики симуляции |
| `NodeFactory` / `FrameFactory` | Фабрики для создания объектов |
| `SimulationConfig` (Singleton) | Глобальные параметры симуляции |
| `Logger` (Singleton) | Единый журнал событий |
| `RoutingEntry` | Запись таблицы маршрутизации |

**Паттерны проектирования, используемые в проекте:**
- **Factory Method** — `NodeFactory`, `FrameFactory`
- **Strategy** — `TransmissionStrategy` (CSMA/CD vs Point-to-Point), `RoutingStrategy`
- **Observer** — `CSMACDMedium` → `StatisticsCollector`, модуль визуализации, `Logger`
- **Singleton** — `SimulationConfig`, `Logger`
- **State** — `SimulationState` (`StoppedState`, `RunningState`, `PausedState`, `StepByStepState`)

---

## Требования ТЗ, которые ДОЛЖНЫ быть реализованы в коде

Перед любой работой с кодом или отчётом — сверяйся с этим списком. Если какая-то функциональность **не реализована** в коде, **не упоминай её в отчёте**.

### Функциональные требования (обязательны)
- FR-1: Добавление/удаление Host, Router, Switch/Hub, Link через GUI или CLI
- FR-2: Настройка IP-адреса, маски, MAC-адреса, параметров трафика узла
- FR-3: Настройка скорости канала (10/100/1000 Мбит/с), длины, задержки, типа среды
- FR-4: Алгоритм CSMA/CD (прослушивание, обнаружение коллизии, jam-сигнал, экспоненциальная задержка)
- FR-5: Передача IP-пакетов между подсетями через таблицы маршрутизации (next hop)
- FR-6: Управление симуляцией: запуск, пауза, стоп, перезапуск, изменение скорости, шаговый режим
- FR-7: Визуализация движения пакетов (направление, тип кадра, коллизии)
- FR-8: Статистика: число пакетов, коллизий, среднее время доставки, загрузка канала
- FR-9: Сохранение/загрузка сценария из файла
- FR-10: Экспорт статистики в CSV или PDF

### Нефункциональные требования
- NFR-1: Desktop (Windows/Linux) или web-приложение
- NFR-2: ≥50 узлов, ≥5 маршрутизаторов при ≥10 FPS
- NFR-3: До 200 узлов в headless-режиме
- NFR-5: Валидация входных данных, информативные ошибки

---

## Задача 1 — Проверка и корректировка отчёта

### Что делать

1. Получи текущую версию отчёта (файл `.docx` или `.pdf` студентки).
2. **Сравни каждый пункт отчёта с реально существующим кодом в репозитории.**
3. Для каждого утверждения в отчёте задай себе вопрос: *«Этот код действительно написан и работает?»*
4. Если нет — **удали этот раздел или абзац из отчёта**. Не добавляй заглушки и не выдумывай реализацию.
5. Не удаляй разделы с паттернами проектирования — они описывают архитектурное решение, а не конкретный код.

### Чего НЕ делать
- Не добавлять в отчёт функционал, которого нет в коде
- Не писать «планируется реализовать»
- Не менять формулировки требований ТЗ

---

## Задача 2 — Система сборки (Практика №3, часть 2)

### Требуемая структура проекта

```
network-simulator/
├── CMakeLists.txt          # корневой файл сборки
├── README.md               # описание, инструкция сборки, пример запуска
├── src/                    # исходные .cpp файлы
│   ├── main.cpp
│   ├── Node.cpp
│   ├── Host.cpp
│   ├── Router.cpp
│   ├── Link.cpp
│   ├── Network.cpp
│   ├── CSMACDMedium.cpp
│   ├── StatisticsCollector.cpp
│   ├── NodeFactory.cpp
│   ├── FrameFactory.cpp
│   ├── SimulationConfig.cpp
│   └── Logger.cpp
├── include/                # публичные .h заголовки
│   ├── Node.h
│   ├── Host.h
│   └── ...
└── tests/                  # тесты (настраивается в Задаче 3)
```

### Требования к CMakeLists.txt

```cmake
cmake_minimum_required(VERSION 3.10)
project(NetworkSimulator)
set(CMAKE_CXX_STANDARD 17)
# ... add_executable, target_sources, target_include_directories
# Внешние библиотеки — через find_package или target_link_libraries
```

### Команды сборки (должны работать без ошибок)

```bash
cmake -S . -B build
cmake --build build
cd build && ./NetworkSimulator
```

---

## Задача 3 — Тестирование (Практика №4)

### Подключение фреймворка

Используй **Catch2** (предпочтительно) или GoogleTest. Тесты собираются в отдельную директорию `tests/`.

### Требования к unit-тестам

- Тесты написаны **для каждого класса**: `Node`, `Host`, `Router`, `Link`, `Network`, `CSMACDMedium`, `StatisticsCollector`, `NodeFactory`, `FrameFactory`, `SimulationConfig`, `Logger`
- Для каждой основной функции класса — **минимум 5–7 тест-кейсов** с разными входными данными
- Покрытие: корректные данные, граничные значения, некорректные данные (ожидаемые исключения/ошибки)

### Примеры тестов (ориентир)

```cpp
// Host: тест генерации трафика
TEST_CASE("Host generates packet with correct size") { ... }
TEST_CASE("Host rejects negative packet size") { ... }

// CSMACDMedium: тест обнаружения коллизии
TEST_CASE("Collision detected when two hosts transmit simultaneously") { ... }
TEST_CASE("No collision on idle channel") { ... }

// StatisticsCollector: тест счётчиков
TEST_CASE("Collision counter increments correctly") { ... }
TEST_CASE("Average delivery time calculated correctly") { ... }
```

### Тестовые сценарии (интеграционные мини-программы)

Реализуй как отдельные исполняемые файлы в `tests/scenarios/`:

| Сценарий | Описание |
|---|---|
| `scenario_csma_collision` | 2 хоста одновременно передают на общей шине → ожидается коллизия |
| `scenario_ip_routing` | Пакет от Host A → Router → Host B в другой подсети |
| `scenario_save_load` | Сохранение топологии в файл → загрузка → проверка идентичности |
| `scenario_statistics` | Симуляция 100 пакетов → проверка корректности статистики |
| `scenario_headless_200` | Запуск в headless-режиме с 200 узлами без падения |

### Запуск тестов

```bash
cd build && ctest --output-on-failure
```

---

## Задача 4 — Контейнеризация (Практика №5)

### Требования к Dockerfile

- **Многостадийная сборка** (builder → runtime): финальный образ не содержит временных файлов сборки
- Базовый образ: `ubuntu:22.04` или `debian:bookworm-slim`
- Все зависимости устанавливаются явно в `RUN apt-get install ...`
- Сборка проекта через CMake выполняется внутри контейнера
- Тесты Catch2 запускаются внутри контейнера и все проходят
- Контейнер запускается и выполняет основную функцию приложения

### Шаблон Dockerfile

```dockerfile
# --- Stage 1: Build ---
FROM ubuntu:22.04 AS builder

RUN apt-get update && apt-get install -y \
    build-essential cmake git \
    libcatch2-dev \
    # добавь остальные зависимости проекта
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build --parallel

# --- Stage 2: Test ---
FROM builder AS tester
RUN cd build && ctest --output-on-failure

# --- Stage 3: Runtime ---
FROM ubuntu:22.04 AS runtime

RUN apt-get update && apt-get install -y \
    # только runtime-зависимости
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/build/NetworkSimulator .

ENTRYPOINT ["./NetworkSimulator"]
```

### Команды для демонстрации

```bash
# Сборка образа
docker build -t network-simulator .

# Запуск тестов
docker run --rm network-simulator-tester

# Запуск приложения
docker run --rm network-simulator

# Передача аргументов CLI
docker run --rm network-simulator --headless --nodes 50

# Если проект использует Qt (GUI) — пробрасываем X11
docker run --rm -e DISPLAY=$DISPLAY -v /tmp/.X11-unix:/tmp/.X11-unix network-simulator
```

---

## Общие правила для агента

1. **Сначала читай код, потом пиши отчёт** — никогда не наоборот.
2. **Не выдумывай реализацию** — если класса нет, его нет.
3. **Не удаляй паттерны из отчёта** — они описывают проектное решение.
4. **Тесты должны реально запускаться** — не пиши заглушки `REQUIRE(true)`.
5. **Dockerfile должен собираться в чистом Linux** — проверяй локально перед сдачей.
6. **README.md обязателен** — с инструкцией сборки и примером запуска.
7. При конфликте между тем, что написано в отчёте, и тем, что есть в коде — **приоритет за кодом**.
