# cppserver
Engine, Fix Protocol, Game


# CMake
```
cmake ..
make
```

# Python
version: 3.12.10<br>
```python
pip install Jinja2
```
- 자동화 1. 프로토콜 .proto 파일을 protoc.exe를 이용해 Protocol Buffer 코드 생성
- 자동화 2. .proto 파일에 프로토콜이 추가될 때마다 PacketHandler에 매핑되는 핸들러를 등록하도록 하는 코드 생성

# Protobuf
- v22.x 부터 Protobuf 내부에서 Thrid-Party absl 사용하기 때문에 번거로워 짐<br>
- absl 안쓰는 버전으로 사용하기로 함.(Protocol Buffers v21.9)<br>
- libprotoc.lib는 CMake로 빌드 후 git에 저장함.
version: v21.9<br>
https://github.com/protocolbuffers/protobuf/releases/tag/v21.9

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