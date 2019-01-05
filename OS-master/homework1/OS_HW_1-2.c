/*OS homework1-2 Finding the lowest value */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define INF 1000000

int arr[10];
int mins[5];
int result = INF;
int locknum[3] = {1,1,1};   //locked

typedef struct {
    int from_index;
    int to_index;
    int bnum;
}parameters;    //start,end,blocknum 3개의 인자를 넘겨주기 위한 구조체


void* findMin1(void* data)
{
    parameters *num = (parameters *)data;
    int startIdx, endIdx;
    startIdx = num->from_index;
    endIdx = num->to_index;
    
    for (int i = startIdx; i < endIdx; i++)
        if(mins[num->bnum] > arr[i])
            mins[num->bnum] = arr[i];
    locknum[num->bnum] = 0;   //unlock
    
    pthread_exit((void*)0);
}

void* findMin2(void* data)
{
    int rst = INF;
    while(locknum[0] || locknum[1] || locknum[2]);      //spinlock until all locknums are unlocked
    
    for(int i=0;i<3;i++)
        if(rst>mins[i])
            rst = mins[i];   //update Lowest value
    pthread_exit((void*) rst);
}

int main()
{
    
        freopen("input.txt","r",stdin);
        freopen("output.txt","w",stdout);       //파일 입출력
    
    pthread_t t[4];
    
    parameters *task1 = (parameters *)malloc(sizeof(parameters));
    parameters *task2 = (parameters *)malloc(sizeof(parameters));
    parameters *task3 = (parameters *)malloc(sizeof(parameters));
    
    task1->from_index = 0;
    task1->to_index = 3;
    task1->bnum = 0;        //0번블록 : 0~2
    task2->from_index = 3;
    task2->to_index = 6;
    task2->bnum = 1;        //1번블록 : 3~5
    task3->from_index = 6;
    task3->to_index = 10;
    task3->bnum = 2;        //2번블록 : 6~9
    
    for(int i=0;i<3;i++)
        mins[i] = INF;
    
    for(int i=0;i<10;i++)
        scanf("%d",&arr[i]);
    
    pthread_create(&t[0],NULL,findMin1,(void*)task1);
    pthread_create(&t[1],NULL,findMin1,(void*)task2);
    pthread_create(&t[2],NULL,findMin1,(void*)task3);
    pthread_create(&t[3],NULL,findMin2,NULL);
    
    for(int i=0;i<3;i++)
        pthread_join(t[i],NULL);
    pthread_join(t[3],&result);
    
    //    for(int i=0;i<3;i++)
    //        printf("Lowest num of block %d : %d\n",i,mins[i]);    //각 블록의 최솟값 출력
    printf("Lowest : %d\n",result);
    
}


