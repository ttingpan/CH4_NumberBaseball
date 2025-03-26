# CH4_NumberBaseball
## [9번 과제] 숫자 야구 게임 
---

## **게임 진행 흐름**

  ![image](https://github.com/user-attachments/assets/c02363bd-1e6c-49ff-86bd-29137b017255)

---

## **C++ 라이브러리 분리**
  - `판정(S/B 계산)`과`난수 생성` 로직은 각각 `UBlueprintFunctionLibrary`를 상속받은 클래스에 구현

### **3자리 난수 생성 로직 (중복 없는 1~9 숫자)**
  - 게임 시작 시 서버에서 무작위 n자리 숫자 생성 (n은 `GameMode`에서 설정, 에디터에도 노출)
  - 숫자는 0을 제외한 3 ~ 9자리까지 생성 가능

  <details open=true>
  <summary>RandomNumberLib.cpp</summary>

  ```cpp
  #include "RandomNumberLib.h"

  // #include "Runtime/Core/Tests/Containers/TestUtils.h"

  FString URandomNumberLib::GenerateRandomNumber(const int32 Length)
  {
    TArray<int32> Numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    FString Result = "";
  
    // 내부적으로 Fisher-Yates 알고리즘을 사용
    // Test::Shuffle(Numbers);
  
    // 참고하여 사용(입력받은 길이 만큼 조합)
    for (int32 i = 0; i < Length; i++)
    {
      const int32 j = FMath::RandRange(i, Numbers.Num() - 1); 
      if (i != j)
      {
        Numbers.Swap(i, j);
      }
  
      Result.Append(FString::FromInt(Numbers[i]));
    }
    
    return Result;
  }

  ```
    
  </details>

### **판정 로직 (S/B/OUT 계산)**
  - 서버가 생성한 숫자와 플레이어가 입력한 숫자를 비교하여 스트라이크(S), 볼(B), 아웃(OUT)을 계산
  - 자리수와 값이 같으면 S, 자리수는 다르지만 값이 존재하면 B, 둘 다 아니면 OUT

  <details open=true>
  <summary>ComparingNumbersLib.cpp</summary>
    
  ```cpp
    
    #include "ComparingNumbersLib.h"

    void UComparingNumbersLib::ComparingNumbers(const FString& InputNumber, const FString& TargetNumber,
                                                int32& OutStrikeCount, int32& OutBallCount)
    {
      for (int32 i = 0; i < InputNumber.Len(); i++)
      {
        TCHAR InputChar = InputNumber[i];
    
        // 해당 문자가 TargetNumber에 존재하는지 확인
        if (TargetNumber.Contains(FString(1, &InputChar)))
        {
          // 해당 문자가 TargetNumber에 존재하고, 위치도 같은지 확인
          if (InputChar == TargetNumber[i])
          {
            OutStrikeCount++;
          }
          // TargetNumber에 존재하지만, 위치가 다른 경우
          else
          {
            OutBallCount++;
          }
        }
      }
    }

  ```
  </details>

---

## **시도 횟수 및 상태 관리와 턴(Turn) 제어 기능**
  - `TurnManager`를 만들어서 `턴`에 관한 기능들을 수행
  - `GameMode`에서 생성 저장 하고 `GameState`에 복사하여 사용
  - `턴 시작`, `시간 업데이트`, `턴 종료`, `턴 즉시 종료`등의 Delegate를 만들고 다른 객체에서 바인딩하여 해당 이벤트에 필요한 액션을 수행 할 수 있도록 함
  - 플레이어의 입력 1번당 1턴으로 계산하고 턴이 넘어 갈때 마다 시도 횟수가 줄어드는 형태로 작성
  - 플레이어의 입력 혹은 시간 초과로 턴이 넘어 가도록 작성

### **턴 진행 흐름**

  ![image](https://github.com/user-attachments/assets/ffd38598-76bd-4ef8-b7aa-cecff3489ec3)

---

## **게임 승리 조건**
  - 각 턴에서 `HIT(3Strike)`를 한 플레이어가 승점을 획득
  - 승점이 정해진 `승리 승점(GameMode에서 설정)`에 도달하면 해당 플레이어가 게임 승리
  - 해당 라운드에서 모든 턴을 소모해도 아무도 승점을 획득 하지 못했다면 다음 라운드 진행 -> 무승부
    > 현재는 플레이어중 승리자가 나올 때까지 무한히 라운드를 진행함 -> 최대 라운드를 정하고 현재까지 최대 승점을 얻은 플레이어가 승리하도록 수정 필요

---

## **UI 에시**

### **게임 참가 UI**

  ![image](https://github.com/user-attachments/assets/ab2ade71-6e77-454f-8d8c-fe6e9c2dd2ed)

### **플레이 UI**

  ![image](https://github.com/user-attachments/assets/c3714a76-9fda-4918-9643-7f476adb25f9)

### **게임 승리 및 패배**

  ![image](https://github.com/user-attachments/assets/bdc249bc-361f-46cf-b9e5-042ad8a8b453)

  ![image](https://github.com/user-attachments/assets/986f4dcf-cdaa-42bf-b94b-06d42ed19b18)

---

## **게임 플레이 예시**

https://github.com/user-attachments/assets/2a5097eb-1ff8-4efc-b7bf-63ff7cb24723








