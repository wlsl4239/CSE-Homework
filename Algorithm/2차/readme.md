# Homework 02 - Strongly Connected Component 구현  

파일에 저장 된 회원 정보와 Follow 정보를 읽어 유향 그래프를 인접 리스트 방식(adjacency list representation)으로 모델링하고  
강결합 요소(Strongly Connected Components)를 사용하여 회원들 간의 Follow 정보를 관리하는 프로그램을 구현한다.  

- 질의 ‘T’ : 입력받은 회원이 포함된 그룹의 리더 정보를 출력한다.
- 질의 ‘A’ : 첫 번째 회원으로부터 두 번째 회원으로 가는 Follow 상태를 변경한다.  
Follow 상태가 아니라면 첫 번째 회원은 두 번째 회원을 Follow 하게 되고, Follow 상태였다면 Unfollow를 하게 된다.
- 질의 ‘0’ : 현재 그래프에서의 SCC 개수를 출력한 뒤 프로그램을 종료한다.
