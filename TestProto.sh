#!/usr/bin/env bash


########## Protobuf
wget https://github.com/protocolbuffers/protobuf/releases/download/v21.9/protobuf-cpp-3.21.9.tar.gz
tar -zxf protobuf-cpp-3.21.9.tar.gz
cd protobuf-3.21.9
./autogen.sh
./configure # 디폴트로 /usr/local 에 설치가 된다. --prefix 에 설치 경로를 명시 할 수 있다
make -j 4
make check
sudo make install
sudo ldconfig
protoc --version

# After build mv bin
cd ..
rm protobuf-cpp-3.21.9.tar.gz
test -d ./CommonTools || mkdir -p ./CommonTools
if [ -x /usr/local/bin/protoc ]; then
  mv /usr/local/bin/protoc ./CommonTools/
else
  echo "protoc not found at /usr/local/bin/protoc"
fi
