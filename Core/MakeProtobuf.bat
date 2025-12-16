pushd %~dp0
protoc.exe -I=./ --cpp_out=./Protocol ./Protocol/Protobuf/Enum.proto
protoc.exe -I=./ --cpp_out=./Protocol ./Protocol/Protobuf/Struct.proto
protoc.exe -I=./ --cpp_out=./Protocol ./Protocol/Protobuf/Protocol.proto
