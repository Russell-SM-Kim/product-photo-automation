# Product Photo Automation

상품 등록 업무를 위한 Windows 기반 **상품 촬영 자동화 시스템 시제품**입니다.

바코드 리더기, Bluetooth 전자저울, Sony 카메라를 연동하여 상품 촬영 과정에서 발생하는 중량 수집, 상품 식별, 이미지 파일명 변경 및 상품별 이미지 분류를 자동화합니다.

현재 버전은 로컬 촬영 및 데이터 저장을 중심으로 구현되어 있으며, 향후 상품 등록 시스템과의 API 연동을 준비중에 있습니다.

---

## Project Background

본 프로젝트는 **자기야닷컴 제공 상품의 상품 등록 업무 효율화를 목적으로, 상품 등록 및 수출 관리 업무를 수행하는 (주)이크레모스와의 업무 협의를 바탕으로 개발한 시제품**입니다.

관련 프로젝트의 컨설팅은 **(주)코너스톤엑스**에서 수행하고 있습니다.

### Related Parties

본 프로젝트는 상품 등록 및 촬영 업무 효율화를 목적으로 관련 업체들과 협의하며 진행 중인 시제품입니다.

현재 세부 업무 범위 및 역할은 협의 중이며, 아래 내용은 프로젝트 진행 과정에서의 대략적인 참여 영역을 나타냅니다.


| 구분 | 역할 |
|---|---|
| 자기야닷컴 | 상품 제공 |
| (주)이크레모스 | 상품 등록 및 수출 관리 |
| (주)코너스톤엑스 | 관련 프로젝트 컨설팅 |

※ 각 참여사의 세부 역할 및 업무 범위는 향후 협의에 따라 변경될 수 있습니다.

본 Repository는 상품 촬영 자동화 시스템의 기술 검토 및 관계자 간 협의를 위한 Prototype 소스코드를 관리합니다.

---

## Purpose

기존 상품 등록을 위한 촬영 과정에서는 다음과 같은 반복 작업이 발생합니다.

- 상품 바코드 확인
- 상품 중량 확인
- 상품 촬영
- 촬영 이미지 확인
- 이미지 파일명 변경
- 상품별 이미지 분류
- 상품 데이터 기록

본 시스템은 이러한 작업을 하나의 촬영 Workflow로 연결하여 반복적인 수작업을 줄이는 것을 목표로 합니다.

---

## Workflow

```text
상품 적재
    ↓
전자저울 중량 수신
    ↓
바코드 스캔
    ↓
Barcode / Weight 매칭
    ↓
Sony A7C II 촬영
    ↓
Sony Imaging Edge Remote
    ↓
Incoming Folder
    ↓
신규 이미지 자동 감지
    ↓
Barcode 기반 파일명 변경
    ↓
상품별 폴더 자동 분류
    ↓
로컬 상품 데이터 저장
```

---

## Current Features

### Barcode

- USB HID Barcode Reader 지원
- Barcode + Enter 자동 입력
- 신규 바코드 스캔 시 현재 상품 변경
- 현재 Barcode와 촬영 이미지 자동 연결

### Electronic Scale

- Bluetooth Serial 전자저울 연결
- Windows COM Port 기반 통신
- Qt SerialPort 사용
- COM Port 자동 검색 및 GUI 선택
- 실시간 중량 수신
- 저울 데이터 Parsing
- Barcode / Weight 매칭

### Camera

- Sony A7C II
- Sony Imaging Edge Remote 기반 PC Remote 촬영
- Incoming Folder 실시간 감시
- 신규 이미지 자동 감지
- Barcode 기반 이미지 Rename
- 상품별 이미지 폴더 자동 생성

### Local Storage

- Timestamp 저장
- Barcode 저장
- Weight 저장
- CSV 데이터 생성
- 상품별 이미지 저장

### Configuration

- COM Port 선택
- COM Port 새로고침
- Camera Incoming Folder 선택
- Workspace Folder 선택
- PC별 설정 자동 저장 및 복원

---

## Directory Structure

사용자가 Workspace를 지정하면 다음과 같은 구조로 상품 데이터를 관리합니다.

```text
Workspace/
│
├── Products/
│   │
│   ├── 8801056193010/
│   │   ├── 8801056193010_1.jpg
│   │   ├── 8801056193010_2.jpg
│   │   └── 8801056193010_3.jpg
│   │
│   └── 8809306544810/
│       ├── 8809306544810_1.jpg
│       └── 8809306544810_2.jpg
│
└── Data/
    └── product_data.csv
```

Sony Imaging Edge Remote의 Incoming Folder는 Workspace와 별도로 지정할 수 있습니다.

---

## CSV Data

현재 상품 데이터는 다음 형태로 저장됩니다.

```csv
timestamp,barcode,weight_kg
2026-09-10 02:30:15,8801056193010,0.31
```

---

## Software Architecture

```text
MainWindow
│
├── ScaleManager
│   └── Qt SerialPort
│
├── CameraManager
│   └── QFileSystemWatcher
│
└── FileManager
    └── CSV / Local File System
```

### MainWindow

Qt 기반 사용자 인터페이스 및 전체 작업 Workflow를 제어합니다.

### ScaleManager

전자저울 연결, Serial 데이터 수신 및 중량 Parsing을 담당합니다.

### CameraManager

Sony Imaging Edge Remote의 Incoming Folder를 감시하고 신규 촬영 이미지를 상품 Barcode와 연결하여 Rename 및 분류합니다.

### FileManager

Barcode, Weight, Timestamp 등의 상품 데이터를 로컬 파일로 저장합니다.

---

## Development Environment

- Windows
- C++17
- Qt 6
- Qt Widgets
- Qt SerialPort
- CMake
- MSVC

현재 개발 환경:

```text
Qt 6.11.2
MSVC 2022 64-bit Qt Build
Visual Studio Build Tools
VS Code
```

---

## Hardware

### Camera

Sony A7C II

PC Remote 촬영은 Sony Imaging Edge Remote를 사용합니다.

### Electronic Scale

Prototype 기준:

```text
Model: TCS
Capacity: 200 kg
Division: 50 g
Bluetooth: HC-06
Interface: Bluetooth SPP / Virtual COM Port
```

### Barcode Reader

USB HID 방식 Barcode Reader를 사용합니다.

---

## Future Development

현재 Repository는 로컬 촬영 자동화 시제품 단계입니다.

향후 다음 기능을 검토합니다.

- REST API 연동
- 상품 Metadata 전송
- 이미지 Upload
- Upload Queue
- 네트워크 장애 시 Retry
- 전송 상태 관리
- 촬영 수량 표시
- 장치 연결 상태 UI 개선
- 작업 로그 관리

향후 서버/API 인터페이스는 관련 시스템 담당자와 협의하여 정의할 예정입니다.

---

## Status

**Prototype / Technical Validation**

현재 구현 범위:

```text
Hardware
    ↓
Desktop Application
    ↓
Local Image / Data Storage
```

향후 확장:

```text
Local Storage
    ↓
Upload Queue
    ↓
REST API
    ↓
Product Registration System
```

---

## Notes

본 프로젝트는 현재 상품 촬영 및 상품 등록 Workflow의 기술 검토를 위한 시제품입니다.

실제 운영 환경 적용 시 장비 구성, 저장 정책, API Specification 및 운영 프로세스에 따라 추가 개발 및 검증이 필요합니다.