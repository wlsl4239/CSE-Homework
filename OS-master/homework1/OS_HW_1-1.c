/*OS homework1-1 Finding the highest value */

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#define INF 1000000

int arr[10][10];
int maxs[30];
int blocknum[10];
int result;

int x[9] = { 0, 0, 0, 3, 3, 3, 6, 6, 6};
int y[9] = { 0, 3, 6, 0, 3, 6, 0, 3, 6};    //arr[x[blocknum + i]][y[blocknum + j]]) (0<=i<3, 0<=j<3)로 블록 내부에 있는 9개의 수에 접근한다.

void* findmax(void* data)
{
    int maxx = -INF;
    int num = *(int*)data;
    
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
            if(maxx < arr[x[num] + i][y[num] + j])
                maxx = arr[x[num] + i][y[num] + j];    //update highest value of block #num
    pthread_exit((void*)maxx);
}

void* findmax2(void* data)
{
    int rst = -INF;
    for(int i=0;i<9;i++)
        if(rst<maxs[i])
            rst = maxs[i];       //update highest value
    pthread_exit((void*) rst);
}

int main()
{
    freopen("input.txt","r",stdin);
    freopen("output.txt","w",stdout);       //파일 입출력

    pthread_t t[10];
    
    for(int i=0;i<9;i++)
        blocknum[i] = i;    //blocknum setting
    
    for(int i=0;i<10;i++)
        maxs[i] = 0;        // 각 블록의 최댓값을 0으로 초기화
    
    for(int i=0;i<9;i++)
        for(int j=0;j<9;j++)
            scanf("%d",&arr[i][j]);
    
    for(int i=0;i<9;i++)
        pthread_create(&t[i],NULL,findmax,(void *)&blocknum[i]);    //thread create
    for(int i=0;i<9;i++)
        pthread_join(t[i],&maxs[i]);

    
    pthread_create(&t[9],NULL,findmax2,NULL);
    pthread_join(t[9],&result);
    
//    for(int i=0;i<9;i++)
  //      printf("Highest value of Block %d : %d\n",i,maxs[i]);
    printf("Highest number : %d\n",result);
    
}


