SparkPlugB Demo

- [1.description](#1-description)
  - [1.1 runtime architecture](#11-runtime-architecture)
  - [1.2 deployment architecture](#12-deployment-architecture)
- [2.use](#2-use)
  - [2.1 use with docker compose](#21-use-with-docker-compose)
  - [2.2 use with local applications](#22-use-with-local-applications)
    - [2.2.1 tahu setup](#221-tahu-setup)
    - [2.2.2 een](#222-een)
    - [2.2.3sub](#223-sub)
- [8. developer resources](#8-developer-resources)
- [9. TODO](#9-todo)

# 1. description
## 1.1 runtime architecture
```mermaid
flowchart LR
  subgraph embedded box
    een[een <br>embedded edge node]
    data_serial --data--> een
    data_serial --config*--> een
    een --cmd--> data_serial

    data_can --data--> een
    data_can --config*--> een
    een --cmd--> data_can

    style data_serial fill:black,stroke:red,stroke-width:2px,color:red
    style data_can fill:black,stroke:orange,stroke-width:2px,color:orange
    style een fill:black,stroke:yellow,stroke-width:2px,color:yellow
  end

  subgraph server
    mqtt <--> HostApplication
    een  <--sparkPlugB--> mqtt[(mqtt)] <--> sub

    style mqtt fill:#660066,stroke:#fff,stroke-width:2px,color:#fff
    style sub fill:black,stroke:green,stroke-width:2px,color:green

    style HostApplication fill:black,stroke:blue,stroke-width:2px,color:blue,stroke-dasharray: 5 5
  end
```
*secondary config stream could be replaced by a single comprehensive data-stream with full configuration.  However, timestamps would have to indicate if attribute  values are fresh, and bandwith size will increase linearly with the number of attributes monitored.  Protobuff buffers could be use to minimize data size.

## 1.2  deployment architecture
![](./images/deployment_architecture.png)
A) data_serial/release.py
   1. commits tag of code (if safe).  This is important to automate to enure that image tags are traceable to code tags.
   2. build + tag + push docker image to container registry

B) een/release.py (same as above)

C) deploy.py
   1. create temp deployment from existing manifest, adding secrets that must not be kept in version-control that are stored as environment-variables


# 2. local use
NOTE : inter-module communication is limited to Azure IoT Edge routes, which are unavailable without an Azure managed deployment.  This can be optionally replaced to use a local MQTT server, but this is not yet fully implemented.

## 2.1 use with docker compose
```
sudo docker compose build een sub
sudo docker compose --profile een --profile sub up
```

## 2.2. use with local applications
### 2.2.1 tahu setup
tahu must be built and installed
```
cd ..
git clone https://github.com/eclipse/tahu.git --branch v1.0.11
cd tahu

# disable SPARKPLUG_DEBUG printing
sed -i "s|#define SPARKPLUG_DEBUG 1|//#define SPARKPLUG_DEBUG 1 |g" c/core/include/tahu.h

# navigate to tahu repo
cd c/core
make
cp include/* /usr/local/include
cp lib/libtahu.a /usr/lib/
```
note that protobuf files can be rebuit as shown in `een`/`sub` Dockerfiles, but the stock tahu files seem to be okay (for now...)

### 2.2.2 een
```
cd een
mkdir -p build && cd build
cmake .. && make
een/een
```
### 2.2.3 sub
```
cd sub
mkdir -p build && cd build
cmake .. && make
sub/sub
```

# 8 developer resources
1. https://docs.docker.com/guides/cpp/containerize/
2. https://medium.com/codex/a-practical-guide-to-containerize-your-c-application-with-docker-50abb197f6d4
    - https://github.com/mostsignificant/simplehttpserver
3. https://cedalo.com/blog/mosquitto-docker-configuration-ultimate-guide/


# 9 TODO
- reconnection attempt behavior