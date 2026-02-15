#!/usr/bin/env bash
set -euo pipefail

# 스크립트가 있는 디렉토리로 이동 (pushd %~dp0 대응)
SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)"
pushd "$SCRIPT_DIR" > /dev/null

# PacketGenerator 폴더로 이동 (없으면 에러)
cd "./PacketGenerator"

# python 실행 (python3 우선, 없으면 python)
PY="python3"
command -v "$PY" >/dev/null 2>&1 || PY="python"

"$PY" PacketGeneratorCore.py

popd > /dev/null
