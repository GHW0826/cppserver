pushd %~dp0
.\CommonTools\protoc.exe -I=.\Core\Protocol\Protobuf --cpp_out=.\Core\Protocol\Protobuf .\Core\Protocol\Protobuf\Enum.proto
.\CommonTools\protoc.exe -I=.\Core\Protocol\Protobuf --cpp_out=.\Core\Protocol\Protobuf .\Core\Protocol\Protobuf\Struct.proto
.\CommonTools\protoc.exe -I=.\Core\Protocol\Protobuf --cpp_out=.\Core\Protocol\Protobuf .\Core\Protocol\Protobuf\Protocol.proto
