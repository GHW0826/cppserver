pushd %~dp0
..\CommonTools\protoc.exe -I=.\Protocol\Protobuf --cpp_out=.\Protocol\Protobuf .\Protocol\Protobuf\Enum.proto
..\CommonTools\protoc.exe -I=.\Protocol\Protobuf --cpp_out=.\Protocol\Protobuf .\Protocol\Protobuf\Struct.proto
..\CommonTools\protoc.exe -I=.\Protocol\Protobuf --cpp_out=.\Protocol\Protobuf .\Protocol\Protobuf\Protocol.proto
