# ⚾개요
+ 이 프로젝트는 숫자 야구 게임을 기반으로 하여 언리얼 엔진에서 리슨 서버를 활용한 멀티 플레이 게임으로 구현한 것입니다.
+ 이 게임의 원제는 *Bulls and Cows* 이며, 프로젝트에서는 야구라는 명칭에 맞게 야구와 관련된 용어를 사용합니다.

## 🕹규칙
+ 임의로 주어지는 3자리의 숫자가 주어집니다.
+ 플레이어는 주어진 각 턴마다 자신의 차례에 숫자를 입력할 수 있습니다.
+ 각 플레이어가 채팅으로 3자리 숫자를 입력하여 입력된 값과 주어진 값을 비교합니다.
+ 위치와 숫자가 일치할 시 스트라이크(S), 숫자만 일치할 시 볼(B), 일치하지 않을 시 아웃(O)이 됩니다.
+ 입력한 숫자가 3S 가 되면 승리, 3O 이 되면 패배하게 됩니다.
+ 플레이어에게는 최대 턴이 존재하며 그 턴까지 3S 를 하지 못한다면 무승부가 됩니다.
+ 플레이어가 입력하지 않은 채 시간을 초과할 시 자신의 턴을 소모하게 됩니다.
+ 잘못된 입력(중복된 값, 숫자가 아닌 값, 3자리 이상의 숫자)을 하게 될 시 3O 으로 처리되어 패배합니다.
+ 플레이어는 스코어 보드를 통해 자신의 점수를 확인할 수 있습니다.
+ 3인 이상의 인원으로도 플레이 할 수 있습니다.

---
# 📋기능
## 1. 구현된 기능
---
### 1. 닉네임 설정
![image](https://github.com/user-attachments/assets/991bb427-38fa-47a2-9923-d6d8afd91562)
+ 처음 실행하게 되면 만나게 되는 화면입니다.
+ 위젯을 통해 텍스트 박스, 버튼, 텍스트를 이용해 간단히 구현하였습니다.
+ 원하는 닉네임을 입력하게 되면 채팅 시 닉네임이 반영됩니다.
---
### 2. 채팅 기능
![image](https://github.com/user-attachments/assets/e7682fe1-f54b-4c0d-aa8e-e324432826f1)
+ 게임을 시작하지 않은 상태에서는 채팅 서버로도 사용할 수 있습니다.
+ 입력한 닉네임을 통해 자신과 상대가 누구인지 구별할 수 있습니다.
+ 채팅이 길어지게 될 경우 스크롤 바를 통해 이전의 채팅도 확인할 수 있습니다.
---
### 3. 게임 시작 버튼
![image](https://github.com/user-attachments/assets/e3929ae5-3940-411d-b827-d7a2e3db880d)
+ 게임 시작 버튼은 호스트만 사용할 수 있습니다.
+ 버튼을 클릭하게 되면 숫자 야구 게임을 시작할 수 있습니다.
+ 게임을 시작한 상태에서 다시 누르더라도 작동하지 않으며 게임이 종료된 상태에서만 실행됩니다.
---
### 4. 턴제 기능
![image](https://github.com/user-attachments/assets/974d5d83-2b6c-42bd-9d56-6aa5043d5ef1)
+ 게임이 시작되면 먼저 호스트부터 턴이 주어집니다.
![image](https://github.com/user-attachments/assets/3a18fd68-2d74-4b2e-9ec6-56a22fbe29ff)
+ 주어진 타이머 동안 입력하지 못할 시 기회가 하나 소진되며 상대에게 턴이 넘어갑니다.
![image](https://github.com/user-attachments/assets/4dd90f66-af4f-4212-a364-5337835e040c)
+ 자신의 턴이 아닌 경우 입력한 값은 일반 채팅으로 처리됩니다.
---
### 5. 정답 판별 기능
![image](https://github.com/user-attachments/assets/fe37cdcc-466f-4b32-9494-2d7562dbce4a)
+ 입력된 값을 정답과 비교하여 스트라이크, 볼, 아웃을 판별하고 정답일 시 승리합니다.
![image](https://github.com/user-attachments/assets/183deced-1474-4940-a7fd-133b5f72ab0b)
+ 잘못된 값(중복된 숫자, 3자리 숫자가 아닌경우, 일반 문자)를 입력하게 될 경우 즉시 패배하여 상대가 승리하게됩니다.
---
### 6. 3인 이상 플레이 기능
![image](https://github.com/user-attachments/assets/4a6f5304-3159-421f-a205-3db228cbc63c)
+ 4인까지도 플레이 할 수 있습니다.
![image](https://github.com/user-attachments/assets/beccf19b-3eb1-47d7-bfdb-c58155d5edf4)
+ 턴의 진행은 입장한 순서대로 진행되고 마지막까지 가게되면 다시 호스트부터 시작합니다.

