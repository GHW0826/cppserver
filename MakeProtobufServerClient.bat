pushd %~dp0
.\CommonTools\protoc.exe -I=.\Common\Protocol\Protobuf --cpp_out=.\Common\Protocol\Protobuf .\Common\Protocol\Protobuf\CSEnum.proto
.\CommonTools\protoc.exe -I=.\Common\Protocol\Protobuf --cpp_out=.\Common\Protocol\Protobuf .\Common\Protocol\Protobuf\CSStruct.proto
.\CommonTools\protoc.exe -I=.\Common\Protocol\Protobuf --cpp_out=.\Common\Protocol\Protobuf .\Common\Protocol\Protobuf\CSProtocol.proto
