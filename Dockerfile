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

# tahu
WORKDIR /
RUN git clone https://github.com/eclipse/tahu.git --branch v1.0.11

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

RUN /my_venv/bin/python3 nanopb_generator.py -I /tahu/sparkplug_b/sparkplug_b.proto -f /tahu/c/core/tahu.options /tahu/c/core/src/tahu.pb

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

WORKDIR /spbd

COPY spbd/ ./spbd/
COPY CMakeLists.txt .

WORKDIR /spbd/build

RUN cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .

################################################################################
# final image
################################################################################

FROM alpine:3.20.3

RUN apk update && apk add --no-cache libstdc++
RUN apk add mosquitto

RUN addgroup -S shs && adduser -S shs -G shs
USER shs

COPY --chown=shs:shs --from=build ./spbd/build/spbd/spbd ./spbd

ENTRYPOINT [ "./spbd" ]