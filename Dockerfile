# Этап 1: Builder на Ubuntu с установкой CMake ≥ 3.30
FROM ubuntu:24.04 AS builder

RUN apt-get update && \
    apt-get install -y wget ca-certificates build-essential && \
    wget https://apt.kitware.com/kitware-archive.sh && \
    bash kitware-archive.sh && \
    apt-get update && \
    apt-get install -y cmake && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .
RUN mkdir -p build && cd build && \
    cmake .. && cmake --build .

# Этап 2: Runtime
FROM ubuntu:24.04
WORKDIR /app

RUN apt-get update

COPY --from=builder /src/build/image_processor .
ENTRYPOINT ["./image_processor"]
