pushd %~dp0

REM PacketGenerator 폴더로 이동
cd /d ".\PacketGenerator" || goto :ERR

REM python 실행 (python이 PATH에 있어야 함)
python PacketGeneratorCore.py