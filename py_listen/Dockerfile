################################################################################
# configure tahu by regenrating protobuf
################################################################################
FROM ubuntu:24.04 AS tahu
RUN apt update && apt install -y git protobuf-compiler

# tahu
WORKDIR /var/lib/docker
RUN git clone https://github.com/eclipse/tahu.git --branch v1.0.11
RUN protoc --proto_path=tahu/sparkplug_b/ \
            --python_out=tahu/python/core/ \
              tahu/sparkplug_b/sparkplug_b.proto

################################################################################
# final
################################################################################
FROM python:3.13
RUN pip install paho-mqtt==1.6.1 protobuf

COPY --from=tahu /var/lib/docker/tahu/python/core ./listen/

ADD listen.py listen/
CMD ["python3","-u", "listen/listen.py"]