################################################################################
# build stage
################################################################################

FROM alpine:3.20.3 AS build

RUN apk update && apk add --no-cache build-base
RUN apk add cmake
RUN apk add git
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

# data_b build
COPY CMakeLists.txt /filter_sample/
COPY /filter_sample/* /filter_sample/filter_sample/
WORKDIR /filter_sample/build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .

################################################################################
# final image
################################################################################

FROM alpine:3.20.3

RUN apk update && apk add --no-cache libstdc++
RUN apk add openssl-dev
RUN apk add curl-dev
RUN apk add util-linux-dev

RUN addgroup -S shs && adduser -S shs -G shs
USER shs

COPY --chown=shs:shs --from=build /filter_sample/build/filter_sample/filter_sample ./filter_sample

ENTRYPOINT [ "./filter_sample" ]