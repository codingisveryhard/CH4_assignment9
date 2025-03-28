![image](https://github.com/user-attachments/assets/f68c821e-4c0a-472b-aea5-50d91f30cfb8)# CH4_ChattingGame
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
### 1.1 닉네임 설정
![image](https://github.com/user-attachments/assets/991bb427-38fa-47a2-9923-d6d8afd91562)
+ 처음 실행하게 되면 만나게 되는 화면입니다.
+ 위젯을 통해 텍스트 박스, 버튼, 텍스트를 이용해 간단히 구현하였습니다.
+ 원하는 닉네임을 입력하게 되면 채팅 시 닉네임이 반영됩니다.

### 1.2 채팅 기능
![image](https://github.com/user-attachments/assets/e7682fe1-f54b-4c0d-aa8e-e324432826f1)
+ 게임을 시작하지 않은 상태에서는 채팅 서버로도 사용할 수 있습니다.
+ 입력한 닉네임을 통해 자신과 상대가 누구인지 구별할 수 있습니다.
+ 채팅이 길어지게 될 경우 스크롤 바를 통해 이전의 채팅도 확인할 수 있습니다.

### 1.3 게임 시작 버튼
![image](https://github.com/user-attachments/assets/e3929ae5-3940-411d-b827-d7a2e3db880d)
+ 게임 시작 버튼은 호스트만 사용할 수 있습니다.
+ 버튼을 클릭하게 되면 숫자 야구 게임을 시작할 수 있습니다.

### 1.3 게임 시작 버튼
### 1.3 게임 시작 버튼
### 1.3 게임 시작 버튼
### 1.3 게임 시작 버튼

주어진 시간이 지날 시 자신의 턴을 상실하고 시도횟수를 차감하게 됩니다.
![image](https://github.com/user-attachments/assets/8437e869-4b60-4580-9b6e-741dd4a9535a)

