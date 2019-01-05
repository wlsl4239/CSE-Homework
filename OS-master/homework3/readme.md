# virtual memory

Designing a Virtual memory manager  
  Demand paging, address translation, page fault, page replacement(FIFO)  


- read a file containing virtual address  
- translate virtual addresses to corresponding physical addresses  
	-> using paging(page table, frame table)  
- size of virtual address, physical address = 16-bit (65536)  
	16bits are devided into 8-bit page number, 8-bit page offset  
- 256 entries in page table, frame table  
- 32 entries in TLB  

## 프로그램 동작  
- addresses.txt를 읽어서 virtual address를 VA 벡터에 저장한다.  
- 각각의 virtual address에 대해 다음 과정을 수행한다.  
  - 현재 virtual address값을 VA[i]라고 가정.  
- page number = VA[i] / 256 (상위 8bit)  
- page offset = VA[i] % 256 (하위 8bit)  

- page number가 TLB에 저장되어 있다면  
  - hit를 1증가시킨다.  
  - inTLB를 true로 설정한다.  
  - TLB를 참고하여 physical.txt에 physical address를 출력한다.  

- page number가 TLB에 없다면 (inTLB == false)  
  - frame table과 page table을 갱신한다.  
  - TLB에 page number와 frame number을 추가한다.   
  - frameNum을 1 증가시킨다.  
  - TLBpointer를 1증가시킨 후 값이32라면 0으로 만든다.  
      -> FIFO  
  - physical.txt에 physical address를 출력한다.  

- 모든 virtual address에 위 과정이 완료된 후 frame_table.txt에 frame table을 출력한다.  
- TLB.txt에 TLB 배열의 값을 출력한다.  
- hit ratio를 출력한다.  
