#!/usr/bin/bash

### /home/user
#### ~ ���� ���� ###

# cd ~

############# CMake
CMAKE_VER="3.19.6"
CMAKE_TARBALL="cmake-${CMAKE_VER}.tar.gz"
CMAKE_SRC_DIR="cmake-${CMAKE_VER}"
CMAKE_URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VER}/${CMAKE_TARBALL}"

# remove package with config file CMake
sudo apt purge -y cmake
sudo apt autoremove -y

# (����) ���� �ҽ� ��ġ�� ���ν���: manifest�� ���� ����
if [[ -f "${CMAKE_SRC_DIR}/install_manifest.txt" ]]; then
  echo "[INFO] Uninstalling previous source-installed CMake using ${CMAKE_SRC_DIR}/install_manifest.txt"
  sudo xargs -d '\n' rm -f < "${CMAKE_SRC_DIR}/install_manifest.txt" || true
else
  echo "[INFO] No install_manifest.txt found at ${CMAKE_SRC_DIR}/install_manifest.txt (skip source uninstall)"
fi

sudo apt-get update
sudo apt-get install -y \
  wget ca-certificates \
  build-essential \
  libssl-dev \
  libncurses5-dev libncursesw5-dev \
  zlib1g-dev

# �ҽ� �ٿ�ε�/�������� (���� ������ ������ �浹 ����)
echo "[INFO] Downloading ${URL}"
rm -f "${CMAKE_TARBALL}"
rm -rf "${CMAKE_SRC_DIR}"

wget "${URL}"
tar -xvf "${CMAKE_TARBALL}"
rm -f "${CMAKE_TARBALL}"

cd "${CMAKE_SRC_DIR}"

./bootstrap --prefix=/usr/local
make -j"$(nproc)"
sudo make install

###  install_manifest.txt ������ ���� x
# [TODO] install_manifest.txt�� ���
# cd ..
# rm -rf cmake-3.19.6

echo
echo "[INFO] cmake resolution:"
which -a cmake || true
echo "[INFO] cmake version:"
cmake --version

##########################################################################

########## Protobuf
wget https://github.com/protocolbuffers/protobuf/releases/download/v21.9/protobuf-cpp-3.21.9.tar.gz
tar -zxf protobuf-cpp-3.21.9.tar.gz
cd ./protobuf-3.21.9
./autogen.sh
./configure # �뵒�뤃�듃濡� /usr/local �뿉 �꽕移섍�� �맂�떎. --prefix �뿉 �꽕移� 寃쎈줈瑜� 紐낆떆 �븷 �닔 �엳�떎
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
  sudo mv /usr/local/bin/protoc ./CommonTools/
else
  echo "protoc not found at /usr/local/bin/protoc"
fi

########## python
# Ȯ��
which -a python3 || true
python3 --version

which -a python  || true
python  --version || true



########## sh Auth Setting
chmod +x ./MakeProtobufCore.sh
sed -i 's/\r$//' ./MakeProtobufCore.sh

chmod +x ./MakeProtobufServerClient.sh
sed -i 's/\r$//' ./MakeProtobufServerClient.sh

chmod +x ./MakePacketHandlerCore.sh
sed -i 's/\r$//' ./MakePacketHandlerCore.sh

chmod +x ./MakePacketHandlerServerClient.sh
sed -i 's/\r$//' ./MakePacketHandlerServerClient.sh




sudo MakeProtobufCore.sh
sudo MakeProtobufServerClient.sh
sudo MakePacketHandlerCore.sh
sudo MakePacketHandlerServerClient.sh