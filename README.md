# C++ Web Socket

### Prerequisites

- [Boost 1.82.0](https://www.boost.org)
- [CMake](https://cmake.org/download/)

### Boost Installation

- Update Ubuntu Linux

```sh
sudo apt update && sudo apt upgrade
```

- Install Developer's tool

```sh
sudo apt-get install build-essential g++ python3-dev autotools-dev libicu-dev libbz2-dev libboost-all-dev
```

- Download Boost `1.82.0` from https://www.boost.org and unzip with `tar xvf` command.

- Setup Boost's bootstrap

```sh
cd boost_1_82_0
./bootstrap.h --prefix=/usr/
```

- Install Boost

```sh
sudo ./b2 install
```

### Build with CMake

```sh
cd boost-lib-websocket
cmake .
make -j 4
```

### Run web socket server

```sh
./WSChatServer/WSChatServer
```

### Run web socket client

```sh
./WSChatClient/WSChatClient
```

### Communication & broadcast

- After a client joined the server, notifications will be received as below:

```sh
Client has joined the server: 127.0.0.1:33134
```

- Clients can now communicate with each other by giving inputs.

- Messages sent by client will be broadcasted by the server to every connected clients.

- To disconnected properly, client may enter `\q` command.

- To shut down the server, enter `CTRL + C`.
