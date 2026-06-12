# assignment9

# 숫자 야구(UE5) - 구현 과정

## 1. 프로젝트 구조 개요

이 프로젝트는 **Unreal Engine 5 기반의 멀티플레이 숫자 야구 게임**이며, **서버 권한 구조(Authoritative Server)** 를 기반으로 동작한다.


시작하기 전 반드시 PIE 설정을 2명의 플레이어로 설정하고 하나는 Listen+Client로 해야한다.

### 주요 클래스 구성

| 클래스 | 역할 |
|----------|----------|
| GameMode | 게임 규칙 및 서버 로직 관리 |
| GameState | 전역 상태 및 UI 동기화 데이터 관리 |
| PlayerController | 클라이언트 입력 → 서버 전송 |
| PlayerState | 플레이어 상태(기회, 이름 등) 관리 |

---

## 2. 핵심 게임 로직 (GameMode)

GameMode는 모든 게임 규칙을 서버에서 처리한다.

### 2.1 랜덤 숫자 생성

```cpp
void ABaseballGameMode::GenerateRandomNumbers()
```

- 1~9 사이 중복 없는 3자리 숫자 생성
- 게임 시작 및 리셋 시 호출

### 2.2 입력 검증

```cpp
bool ValidateInput(const FString& Input)
```

#### 검증 조건

- 길이 3
- 숫자만 입력
- 중복 숫자 금지

### 2.3 정답 판정 로직

```cpp
FString CheckAnswer(const FString& Input)
```

- **Strike** : 위치 + 숫자 모두 일치
- **Ball** : 숫자만 존재, 위치 다름
- **OUT** : 완전히 불일치

### 2.4 승리 처리

```cpp
HandleWin()
```

- GameState에 승자 정보 저장
- 모든 클라이언트에 메시지 전파
- 5초 후 `ResetGame()` 호출

### 2.5 Draw 처리

```cpp
CheckDraw()
```

- 모든 플레이어가 기회를 소진했는지 확인
- TRUE일 경우 DRAW 메시지 브로드캐스트

### 2.6 게임 리셋

```cpp
ResetGame()
```

- 새로운 숫자 생성
- 모든 플레이어 Try 초기화
- 게임 상태 초기화 메시지 전송

---

## 3. 플레이어 입력 흐름 (PlayerController)

클라이언트 입력은 반드시 서버 RPC를 통해 처리된다.

### 3.1 입력 전송

```cpp
void ABaseBallPlayerController::Guess(const FString& Input)
```

↓

Server RPC 호출

### 3.2 서버 처리

```cpp
ServerSubmitAnswer_Implementation()
```

#### 처리 순서

1. GameMode 획득
2. 입력 검증
3. 기회 여부 확인
4. Try 증가
5. 정답 판정
6. GameMode로 결과 전달

---

## 4. PlayerState 역할

각 플레이어의 상태를 저장한다.

### 저장 데이터

- DisplayName
- CurrentTry
- MaxTry

### 기회 소진 여부

```cpp
IsOutOfChance()
```

### Try 증가

```cpp
PS->IncreaseTry();
```

---

## 5. GameState 역할 (네트워크 동기화)

GameState는 모든 클라이언트가 공유하는 상태를 담당한다.

### 동기화 변수

```cpp
UPROPERTY(Replicated)
bool bGameEnded;

UPROPERTY(Replicated)
FString WinnerName;
```

### 5.1 메시지 브로드캐스트

```cpp
MulticastShowMessage()
```

- 모든 클라이언트 UI 출력
- 디버그 메시지 표시



---

## 7. 네트워크 구조 요약

```text
클라이언트
    ↓
PlayerController (RPC)
    ↓
GameMode (Server Only)
    ↓
GameState (Replication / Multicast)
    ↓
UI (Widget Binding)
```

---

## 8. 현재 구조 특징

- 서버 권한 기반 구조 (Cheat 방지)
- GameMode에서 모든 게임 규칙 처리
- GameState를 통한 UI 동기화
- PlayerState로 개인 상태 관리
- RPC + Replication 혼합 구조
