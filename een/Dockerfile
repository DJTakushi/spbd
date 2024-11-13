################################################################################
# build stage
################################################################################

FROM alpine:3.20.3 AS build

RUN apk update && apk add --no-cache build-base
RUN apk add cmake
RUN apk add git
RUN apk add mosquitto-dev
RUN apk add bash
RUN apk add --no-cache protobuf-dev
RUN apk add python3
RUN apk add py3-pip
RUN python3 -m venv /my_venv
RUN /my_venv/bin/python3 -m pip install --upgrade protobuf grpcio-tools
RUN apk add openssl-dev
RUN apk add curl-dev
RUN apk add util-linux-dev


# tahu
WORKDIR /
RUN git clone https://github.com/eclipse/tahu.git --branch v1.0.11

### disable SPARKPLUG_DEBUG in tahu.h
RUN  sed -i "s|#define SPARKPLUG_DEBUG 1|//#define SPARKPLUG_DEBUG 1 |g" \
    /tahu/c/core/include/tahu.h

### get nanopb
WORKDIR /
RUN git clone https://github.com/nanopb/nanopb.git --branch nanopb-0.4.1
WORKDIR /nanopb/generator
CMD ["python3"  \
"nanopb_generator.py", \
"/tahu/sparkplug_b/sparkplug_b.proto", \
"-f", \
"/tahu/sparkplug_b/c/core/tahu.options", \
"/tahu/sparkplug_b/c/core/src/tahu.pb"]

RUN /my_venv/bin/python3 nanopb_generator.py \
    -I /tahu/sparkplug_b/sparkplug_b.proto \
    -f /tahu/c/core/tahu.options \
    /tahu/c/core/src/tahu.pb

RUN rm /tahu/c/core/include/pb_common.h
RUN cp /nanopb/pb_common.h /tahu/c/core/include/

RUN rm /tahu/c/core/include/pb_decode.h
RUN cp /nanopb/pb_decode.h /tahu/c/core/include/

RUN rm /tahu/c/core/include/pb_encode.h
RUN cp /nanopb/pb_encode.h /tahu/c/core/include/

# tahu requires its stock version with PB_ENABLE_MALLOC
# RUN rm /tahu/c/core/include/pb.h 
# RUN cp /nanopb/pb.h /tahu/c/core/include/

RUN rm /tahu/c/core/include/tahu.pb.h
RUN cp /nanopb/generator/tahu.pb.h /tahu/c/core/include/

RUN rm /tahu/c/core/src/pb_common.c
RUN cp /nanopb/pb_common.c /tahu/c/core/src/

RUN rm /tahu/c/core/src/pb_decode.c
RUN cp /nanopb/pb_decode.c /tahu/c/core/src/

RUN rm /tahu/c/core/src/pb_encode.c
RUN cp /nanopb/pb_encode.c /tahu/c/core/src/

RUN rm /tahu/c/core/src/tahu.pb.c
RUN cp /nanopb/generator/tahu.pb.c /tahu/c/core/src/

WORKDIR /tahu/c/core
RUN make
RUN cp include/* /usr/include/
RUN cp lib/libtahu.a /usr/lib/

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

# een build
COPY CMakeLists.txt /een/
COPY /een/* /een/een/
WORKDIR /een/build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .

################################################################################
# final image
################################################################################

FROM alpine:3.20.3

RUN apk update && apk add --no-cache libstdc++
RUN apk add mosquitto
RUN apk add openssl-dev
RUN apk add curl-dev
RUN apk add util-linux-dev

RUN addgroup -S shs && adduser -S shs -G shs
USER shs

COPY --chown=shs:shs --from=build /een/build/een/een ./een

ENTRYPOINT [ "./een" ]