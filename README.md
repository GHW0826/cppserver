# cppserver
Engine, Fix Protocol, Game

# Protobuf
version: 33.1<br>
https://github.com/protocolbuffers/protobuf/releases/tag/v33.1

```
```

# WSL
```shell
# remove package with config file
sudo apt purge cmake

sudo apt-get update
sudo apt -y install wget
sudo apt -y install openssl
sudo apt -y install libssl-dev
sudo apt -y install build-essential

sudo wget https://github.com/Kitware/Cmake/releases/download/v3.19.6/cmake-3.19.6.tar.gz

tar -xvf cmake-3.19.6.tar.gz
cd cmake-3.19.6
./bootstrap --prefix=/usr
make
sudo make install

# CMake Build
cd build
cmake ..
cmake --build .

## [install] if error in ./bootstrap --prefix=/usr
# sudo apt install libssl-dev
```