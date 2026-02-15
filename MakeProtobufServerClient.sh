#!/usr/bin/env bash
set -euo pipefail

# 스크립트 위치로 이동 (pushd %~dp0 대응)
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
pushd "$SCRIPT_DIR" > /dev/null

# protoc 경로: 프로젝트 CommonTools 우선, 없으면 PATH의 protoc 사용
PROTOC="./CommonTools/protoc"
if [[ ! -x "$PROTOC" ]]; then
  PROTOC="protoc"
fi

INCLUDE_DIR="./Common/Protocol/Protobuf"
OUT_DIR="./Common/Protocol/Protobuf"

"$PROTOC" -I="$INCLUDE_DIR" --cpp_out="$OUT_DIR" "$INCLUDE_DIR/CSEnum.proto"
"$PROTOC" -I="$INCLUDE_DIR" --cpp_out="$OUT_DIR" "$INCLUDE_DIR/CSStruct.proto"
"$PROTOC" -I="$INCLUDE_DIR" --cpp_out="$OUT_DIR" "$INCLUDE_DIR/CSProtocol.proto"

popd > /dev/null