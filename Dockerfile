FROM ubuntu:24.04 AS builder

RUN apt-get update && \
    apt-get install -y build-essential cmake && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /src
COPY . .

RUN mkdir -p build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release && \
    cmake --build . -j $(nproc)

FROM ubuntu:24.04
WORKDIR /app

COPY --from=builder /src/build/image_processor .

RUN chmod +x ./image_processor

ENTRYPOINT ["./image_processor"]