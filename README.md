# cppserver
C++ Server Engine

# 요약
```text
C++ 기반 서버 엔진, 네트워크 I/O 처리, 세션 관리, 패킷 파싱/핸들링, 스레드/잡 큐 기반 처리 구조

서버 서비스 구동 및 접속 수락: ServerService가 Listener를 통해 접속을 수락하고 세션을 생성.
IOCP 기반 네트워크 디스패치(Windows): IOCPCore가 완료 이벤트를 디스패치하고, Session이 이벤트를 처리.

패킷 단위 수신 처리: PacketSession이 바이트 스트림을 [size][id][data] 형식으로 파싱해 OnRecvPacket으로 전달.
프로토콜 버퍼 기반 패킷 핸들링: 패킷 ID별 핸들러 등록 및 직렬화/역직렬화를 수행.
멀티스레드 워커 루프: 네트워크 디스패치 + 잡 큐 실행을 워커 스레드가 반복 수행.

- 세션/서비스 추상화: Service가 세션 생성/관리, Session이 송수신 및 이벤트 처리 책임을 담당합니다.
- IOCP 이벤트 디스패치 구조: IOCPCore가 IOCP 완료 이벤트를 받아 IOCPObject의 Dispatch로 전달.
- Epoll : Linux 기반 환경에서 Epoll로 동작하도록 설정
- 패킷 파서 및 핸들러 자동 매핑 구조: PacketSession이 패킷 프레이밍, ClientPacketHandler/CorePacketHandler가 핸들러 테이블 구성 및 Protobuf 직렬화를 담당.
- 잡 큐 및 타이머 기반 비동기 작업 처리: JobQueue가 즉시 실행 및 지연 실행(타이머) 작업 수용.
- SendBuffer 풀 관리: SendBufferManager가 전송 버퍼를 재사용해 메모리 효율을 높임
```

# TODO
```
- Linux 환경 Epoll 적용 (~ing)
- UDP 서버
- 패킷 암호화
- SSL/TLS 적용
- DDoS 방어 기법
    - 연결 속도 제한
    - SYN Flood 방어
- HTTP/HTTPS 서버 구현
```


# Script
```

// Linux
InitWSLSetting.sh : WSL 설치시 환경 설정 스크립트
MakeCoreProtobuf.sh : Core Protobuf 생성 스크립트
MakeServerClientProtobuf.sh : Server, Test Client Protobuf 생성 스크립트
MakePacketHandlerCore.sh : Core Packet Handler 헤더 생성 스크립트
MakePacketHandlerServer.sh : Server, Test Client Packet Handler 헤더 생성 스크립트
MakeListCore.sh : CMake Core MakeList 생성 스크립트
MakeListServer.sh : CMake Server, Client MakeList 생성 스크립트

// Window
InitWindowSetting.bat : window 환경 설정 스크립트
MakeCoreProtobuf.bat : Core Protobuf 생성 스크립트
MakeServerClientProtobuf.bat : Server, Test Client Protobuf 생성 스크립트
MakePacketHandlerCore.bat : Core Packet Handler 헤더 생성 스크립트
MakePacketHandlerServer.bat : Server, Test Client Packet Handler 헤더 생성 스크립트



```

# Python
version: 3.12.10<br>

jinja2 (v3.1.6)<br>
pip (v25.0.1)
```python
pip install Jinja2
```
- 자동화 1. 프로토콜 .proto 파일을 protoc.exe를 이용해 Protocol Buffer 코드 생성
- 자동화 2. .proto 파일에 프로토콜이 추가될 때마다 PacketHandler에 매핑되는 핸들러를 등록하도록 하는 코드 생성

# Protobuf
- v22.x 부터 Protobuf 내부에서 Thrid-Party absl 사용하기 때문에 번거로워 짐<br>
- absl 안쓰는 버전으로 사용하기로 함.(Protocol Buffers v21.9)<br>
- libprotoc.lib는 CMake로 빌드 후 git에 저장함.
version: v21.9<br>
https://github.com/protocolbuffers/protobuf/releases/tag/v21.9

