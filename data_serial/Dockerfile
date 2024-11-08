################################################################################
# build stage
################################################################################
FROM alpine:3.20.3 AS build

RUN apk update && apk add --no-cache build-base
RUN apk add cmake
RUN apk add git
RUN apk add boost-dev
RUN apk add openssl-dev
RUN apk add curl-dev
RUN apk add util-linux-dev


# azure iot sdk
WORKDIR /
RUN git clone https://github.com/Azure/azure-iot-sdk-c.git --branch 1.14.0
RUN mkdir -p /azure-iot-sdk-c/cmake
WORKDIR /azure-iot-sdk-c/cmake
RUN git submodule init && git submodule update
RUN cmake -Duse_edge_modules=ON ..
RUN make install

# nlohmann
WORKDIR /
RUN git clone https://github.com/nlohmann/json.git --branch v3.11.3
RUN cp -r /json/include/* /usr/include/

# build
COPY CMakeLists.txt /data_serial/
COPY /data_serial/* /data_serial/data_serial/
WORKDIR /data_serial/build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .

################################################################################
# final image
################################################################################

FROM alpine:3.20.3

RUN apk update && apk add --no-cache libstdc++
RUN apk add boost-dev

RUN addgroup -S shs && adduser -S shs -G shs
USER shs

COPY --from=build \
    /data_serial/build/data_serial/data_serial ./data_serial

ENTRYPOINT [ "./data_serial" ]