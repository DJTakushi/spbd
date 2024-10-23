# spbd
SparkPlugB Demo

# build image
sudo docker build . -t spbd/spbd

# run
sudo docker run spbd/spbd

# description
1. generate random data [TODO]
2. publish data to mosquito [TODO]

# developer resources
1. https://docs.docker.com/guides/cpp/containerize/
2. https://medium.com/codex/a-practical-guide-to-containerize-your-c-application-with-docker-50abb197f6d4
  - https://github.com/mostsignificant/simplehttpserver
3. https://cedalo.com/blog/mosquitto-docker-configuration-ultimate-guide/

# Local Tahu
## C
```
# navigate to tahu repo
cd c/core
# undefine SPARKPLUG_DEBUG in tahu.h
make
cp include/* /usr/local/include
cp lib/libtahu.a /usr/lib/

```
## python
haha...