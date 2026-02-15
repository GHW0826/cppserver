#!/usr/bin/env bash

# 스크립트 디렉토리로 이동(출력 숨김)
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
pushd "$SCRIPT_DIR" > /dev/null

# protoc 경로: 프로젝트 CommonTools 우선, 없으면 PATH의 protoc 사용
PROTOC="./CommonTools/protoc"
if [[ ! -x "$PROTOC" ]]; then
  PROTOC="protoc"
fi

INCLUDE_DIR="./Core/Protocol/Protobuf"
OUT_DIR="./Core/Protocol/Protobuf"

"$PROTOC" -I="$INCLUDE_DIR" --cpp_out="$OUT_DIR" "$INCLUDE_DIR/Enum.proto"
"$PROTOC" -I="$INCLUDE_DIR" --cpp_out="$OUT_DIR" "$INCLUDE_DIR/Struct.proto"
"$PROTOC" -I="$INCLUDE_DIR" --cpp_out="$OUT_DIR" "$INCLUDE_DIR/Protocol.proto"

# popd > /dev/null : 원래 디렉토리로 복귀(출력 숨김)
popd > /dev/null