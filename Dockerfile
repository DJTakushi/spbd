################################################################################
# build stage
################################################################################

FROM alpine:3.20.3 AS build

RUN apk update && apk add --no-cache build-base cmake

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

RUN addgroup -S shs && adduser -S shs -G shs
USER shs

COPY --chown=shs:shs --from=build ./spbd/build/spbd/spbd ./spbd

ENTRYPOINT [ "./spbd" ]