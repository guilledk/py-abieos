# syntax=docker/dockerfile:1.4

ARG PYTHON_VERSION=3.11.9
ARG JOB_COUNT=2

FROM python:${PYTHON_VERSION}-bookworm AS build-stage

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get -y install build-essential cmake

WORKDIR /root/target

COPY . .

WORKDIR /root/target/build

RUN cmake .. && make -j${JOB_COUNT}

FROM scratch AS export-stage
COPY --from=build-stage /root/target/build/_abieos*.so src/_abieos.so
