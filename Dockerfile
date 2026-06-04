# --- Stage 1: Build ---
FROM ubuntu:22.04 AS builder

# Исключаем интерактивные диалоги apt-get
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    catch2 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build --parallel

# --- Stage 2: Test ---
FROM builder AS tester
WORKDIR /app/build
# Запускаем все тесты
RUN ctest --output-on-failure

# --- Stage 3: Runtime ---
FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

# Устанавливаем только библиотеки выполнения, если нужны (пока нет дополнительных)
RUN apt-get update && apt-get install -y \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
# Копируем собранный проект из стадии builder
COPY --from=builder /app/build/NetworkSimulator .

ENTRYPOINT ["./NetworkSimulator"]
CMD []
