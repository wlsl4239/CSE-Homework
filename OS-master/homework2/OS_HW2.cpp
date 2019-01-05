#include <iostream>
#include <cstdio>
#include <algorithm>
#include <queue>
#include <vector>

using namespace std;

#define NICE_0_LOAD 1024;

queue< pair<int,int> > realtime[101];       //index : priority / first : task number / second : timeslot
priority_queue< pair<pair<int,int> , pair<int,int> > > conventional;  // <<vruntime,delta_exec>,<priority, 남은횟수>>

int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);     //file IO
    
    while(1)
    {
        int a,b,c;
        
        if(scanf("%d %d %d",&a,&b,&c) == EOF)
            break;
        
        /*    scanf("%d %d %d",&a,&b,&c);
         if(a==0 && b==0 && c==0)
         break;
         */
        if(a>99){
            conventional.push(make_pair(make_pair(0,0),make_pair(-a,c)));    //priority queue는 내림차순으로 구현되있기 때문에 -값으로 push
        }
        else
            realtime[a].push(make_pair(b,c));
    }
    
    //real-time class : RoundRobin / timequantum : 2
    for(int i=0;i<100;i++)
    {
        if(realtime[i].empty())     // i : priority
            continue;
        else
        {
            while(!realtime[i].empty()) //현재 priority애 수행할 task가 있다면 수행.
            {
                printf("%d %d ",i,realtime[i].front().first);       //1번 수행
                int a = realtime[i].front().first;
                int b = realtime[i].front().second - 1;
                if(b>0) //1번 수행하고 끝나지 않았다면
                    printf("%d %d ",i,realtime[i].front().first);   //2번 수행
                b--;
                
                realtime[i].pop();
                if(b>0) //2번 수행하고 수행할게 남았다면 다시 push
                    realtime[i].push(make_pair(a,b));
            }
        }
        printf("\n");
    }
    
    //conventional class : CFS(completely Fair Scheduling)
    while(!conventional.empty())
    {
        printf("%d 1\n",-conventional.top().second.first);
        pair< pair<int,int> ,pair<int,int> > temp = conventional.top();
        conventional.pop();
        temp.second.second --;
        if(-temp.second.first<110){
            temp.first.first -= 12;       // 1/10 = 10/120
            temp.first.second --;
        }
        else if(-temp.second.first<120){
            temp.first.first -= 15;       // 1/8 = 15/120
            temp.first.second --;
        }
        else if(-temp.second.first<130){
            temp.first.first -= 20;       // 1/6 = 20/120
            temp.first.second --;
        }
        else if(-temp.second.first<140){
            temp.first.first -= 30;       // 1/4 = 30/120
            temp.first.second --;
        }                                 //내림차순이기 때문에 작은것부터 보기위해 -값으로 넣는다.
        
        if(temp.second.second > 0)  //수행횟수가 남았다면 queue에 다시 push한다.
            conventional.push(temp);
    }
}


