/* 12131469 김경수 OS Homework3 - virtual memory */
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <queue>
#include <fstream>

using namespace std;


pair<int, bool> pages[256];         //page table. frame number와 valid-invalid bit로 구성되어있다.
int frame[256];                     //frame table. frame에 해당하는 page number로 구성되어있다.
pair<int,int> TLB[32];              //TLB. page number와 그에 대응하는 frame number로 구성되어있다.
vector <int> VA;                    //Virtual Addresses. argument로 입력받은 파일에서 virtual address를 저장한다.
double hit;                         //hit ratio를 계산하기위한 변수. TLB에서 값을 찾으면 1씩 증가한다.

int main(int argc, char* argv[])
{
    ofstream phyos("Physical.txt");         /**/
    ofstream frameos("Frame_table.txt");    /**/
    ofstream tlbos("TLB.txt");              /**/
    ifstream fi(argv[1]);                   /* 파일 입출력 */
    
    for(int i=0;i<32;i++)               //TLB 배열 초기화
        TLB[i].first = -1;
    for(int i=0;i<256;i++)              //frame table 초기화
        frame[i] = -1;
    
    while (fi.good())
    {
        int a;
        fi >> a;                        //argument로 입력받은 파일로부터 값을 읽어와서 VA 벡터에 저장.
        VA.push_back(a);
    }
    
    int TLBpointer = 0;                 //TLB배열의 index를 가리키는 번호.
    int frameNum = 0;                   //frame배열의 index를 가리키는 번호.
    
    
    for(int i=0;i<VA.size();i++)
    {
        bool inTLB = false;             //현재 page number가 TLB에 있는지 확인하는 변수
        int pageNum = VA[i] / 256;      //16bit 중 앞 8bit : page number
        int offset = VA[i] % 256;       //16bit 중 뒷 8bit : offset
        
        for(int i=0;i<32;i++)
            if(TLB[i].first == pageNum){    //현재 virtual address의 page number가 TLB에 저장되있는지 확인한다.
                hit++;                      //저장되어있다면 hit를 1증가시켜주고
                inTLB = true;               //inTLB를 true로 설정한다.
                phyos << TLB[i].second * 256 + offset<<" "; //TLB를 참고하여 physical address를 출력한다.
                break;
            }
                                        //있으면 바로 physical address를 출력한다.
        if(!inTLB){                     //TLB에 없다면
            frame[frameNum] = pageNum;          //frame number와 page number를 mapping한다.
            pages[pageNum].first = frameNum;    //frame number와 page number를 mapping한다.
            pages[pageNum].second = true;       //valid상태로 만든다.

            TLB[TLBpointer].first = pageNum;    //TLB에 추가한다.
            TLB[TLBpointer].second = frameNum;  //TLB에 추가한다.
            
            frameNum++;
            TLBpointer++;       //FIFO -> TLBpointer를 다음순서로 설정.
            TLBpointer%=32;     //31번 다음은 0번
            phyos << pages[pageNum].first * 256 + offset << " ";        //physical address를 출력.

        }
    }
    
    for(int i=0;i<256;i++)      //frame table을 출력
    {
        if(frame[i] != -1)
            frameos << i << " "<< pages[frame[i]].second << " " << frame[i]<<"\n";      //frame번호, valid, mapping된 page번호
        else
            frameos << i << " 0 -1\n";  //invalid 라면 page번호는 -1
        
    }
    
    for(int i=0;i<32;i++){      //TLB 출력
        if(TLB[i].first == -1)
            break;
        tlbos<<TLB[i].first<<" "<<TLB[i].second<<"\n";      //page# , frame#
    }
    cout<<"Hit ratio : "<<hit/(double)VA.size()<<"\n";     //hit ratio 출력
    
    return 0;
}
