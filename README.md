SparkPlugB Demo

- [1.description](#1-description)
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

    style data_serial fill:black,stroke:red,stroke-width:2px,color:red
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



```mermaid
---
title: een (embedded edge node) class diagram
---
classDiagram

class een{
  +string group_id
  +string edge_node_id
  -mosquitto* m_
  -LOCAL_MSG_HANDLE handle_
  +een(string config)
  +nbirth_send()
  +ndeath_send()
  +ncmd_rec()
  +dcmd_rec()
  +ndata_send()
  -rec_local_data_msg(string& msg)
  -rec_local_config_msg(string& msg)
}

class device_client{
  -string device_id_
  -string group_id_
  -string edge_node_id_
  -json config_
  -string topic_ddata_
  -set_topics()

  +device_client : (string group_id, string node_id, json& data_init)
  +dbirth_send(mosq* m)
  +ddeath_send(mosq* m)
  +ddata_send(mosq* m)
  +dcmd_pass(str cmd)
  +update(json& msg)
}
een *-- device_client : -map[string, device_client*] device_map_

class attribute_host{
  +mutex attribute_mutex
  -update_attribute(json& msg)
  +update_attributes_from_array(json& msg)
  +ddata_gen(org_eclipse_tahu_protobuf_Payload& payload)
}
device_client *-- attribute_host : -attribute_host attribute_host_

class attribute{
  -void* value_
  -uint64_t datatype_
  -timestamp time_recv
  -timestamp time_sent
  -uint64_t reported_epoch
  +attribute(uint64_t)
  +set_value(void*)
  +set_value_with_timetamp(jsong  j_val, stead_tp time)
  +void* get_value()
  +uint64_t get_type()
  +bool is_recently_published()
  +uint64_t reported_epoch_get()
  +reported_epoch_set(uint64_t epoch)
}
attribute_host *-- attribute : -map[name,attribute*] attributes_
```

# 2. use
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
- python containerized pub/sub containers
  - need fresh protocol files
    - `protoc --proto_path=sparkplug_b/ --python_out=python/core/ sparkplug_b/sparkplug_b.proto`
- reconnection attempt behavior