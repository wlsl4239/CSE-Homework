#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>

using namespace std;

typedef long long ll;

char filename[30];		//파일명
int _num;				//회원번호
char _name[25];			//회원이름
char _phoneNum[15];		//연락처

int scc[5001];			//회원이 가입된 그룹의 leader를 저장하는 배열
bool visited[5001];		//정방향 dfs의 visit 여부를 저장하는 배열
bool rvisited[5001];	//역방향 dfs의 visit 여부를 저장하는 배열
pair< int, pair<string, string> > people[5001]; //회원정보를 저장하는 배열. first : 회원번호, second.first : 이름, second.second : 연락처
vector<int> digra[5001];	//정방향 그래프. 실제 그래프 정보
vector<int> rdigra[5001];	//역방향 그래프. SCC를 구할때 사용한다.
stack<int> stac;			//정방향 그래프의 DFS를 수행한 후 수행이 완료된 vertex부터 저장하는 stack.
int numberOfSCC;		//SCC의 수를 저장하는 변수. 
int n, m;				//정점, 간선의 수


void dfs(int now)						//정방향 dfs
{
	if (visited[now])					//방문된 vertex라면
		return;							//함수 종료
	visited[now] = true;				//현재 vertex를 방문처리
	for (int i = 0;i<digra[now].size();i++)	
		dfs(digra[now][i]);				//연결된 vertex에 대해 DFS

	stac.push(now);						//완료된 vertex부터 stack에 저장한다.
}

void rdfs(int now, int leader)			//역방향 dfs
{
	if (rvisited[now])					//방문된 vertex라면
		return;							//함수 종료
	rvisited[now] = true;				//현재 vertex를 방문처리.
	for (int i = 0;i<rdigra[now].size();i++)
		rdfs(rdigra[now][i], leader);	//연결된 vertex에 대해 역방향 DFS

	scc[now] = leader;					//현재 vertex의 leader를 설정해준다.
}


void func()									//SCC를 구하는 함수.
{
	numberOfSCC = 0;					//SCC의 수를 0으로 초기화한다.
	for (int i = 0;i<n;i++)
	{
		visited[i] = false;					//visited 배열과
		rvisited[i] = false;				//rvisited 배열을 초기화한다.
	}

	for (int i = 0;i<n;i++)
		if (!visited[i])
			dfs(i);

	for (int i = 0;i<n;i++)	
	{
		if (!rvisited[stac.top()]) {		//스택의 top이 방문되지 않았다면
			rdfs(stac.top(), stac.top());	//해당 vertex부터 역방향 DFS를 수행한다. 이 때 leader는 이 vertex이다.
			numberOfSCC++;					//SCC의 수를 증가시킨다.
		}
												//방문되었다면 아무행동도 하지 않고
		stac.pop();							//pop연산을 수행한다. 
	}
}



int main()
{
//	freopen("query_1000.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);			//파일입출력

	printf("입력파일의 이름을 입력하세요 : ");
	scanf("%s", filename);						//파일 이름을 입력받는다.
	ifstream fi;								//ifstream 선언
	fi.open(filename);							//입력받은 파일을 실행한다.

	fi >> n >> m;								//정점과 간선의 갯수를 입력받는다.

	for (int i = 0;i<n;i++)						//정점 정보를 입력받는다.
		fi >> people[i].first >> people[i].second.first >> people[i].second.second;	//번호,이름,연락처
	for (int i = 0;i<m;i++)						//간선 정보를 입력받는다.
	{
		int a, b;
		fi >> a >> b;
		digra[a].push_back(b);					//a점에서 b점으로 가는 간선 생성.
		rdigra[b].push_back(a);					//역방향 그래프 생성.
	}

	func();										//입력받은 그래프로 SCC를 구한다.

	char query;
	while (1)
	{
		scanf("%c", &query);					//쿼리를 입력받는다.

		switch (query)
		{
		case 'T':								//'T' 질의. 형식 : T S(회원번호)
		{
			int __num;							//입력받은 회원의 회원번호
			int index;							//입력받은 회원의 leader가 people의 몇번째 index에 있는지 저장하는 변수.
			cin >> __num;						//회원번호를 입력받는다.
			
			for (int i = 0;i < 5001;i++)
				if (people[i].first == scc[__num]) {		//입력받은 회원의 leader를 people에서 찾아 index를 기록한다.
					index = i;
					break;
				}

			cout << people[index].first << " " << people[index].second.first << " " << people[index].second.second << "\n";
												//입력받은 회원의 leader의 정보를 출력한다.

			break;
		}

		case 'A':								//'A' 질의. 형식 : A E(회원번호 A) D(회원번호 B)
		{
			int e, d;							//회원번호 A와 B
			bool flag = false;					//A->B로의 간선이 있는지 확인하는 변수.
			int index = 0;						//간선이 있을 때 B가 digra[A]의 몇번째에 있는지 저장하는 변수.
			int rindex = 0;						//간선이 있을 때 A가 rdigra[B]의 몇번째에 있는지 저장하는 변수.
			cin >> e >> d;

			for (int i = 0;i<digra[e].size();i++)
				if (digra[e][i] == d)			//A->B로의 간선이 있다면
				{
					flag = true;				//flag를 true로 하고
					index = i;					//index를 설정해준다.
					break;
				}
			if (flag) {							//A->B로의 간선이 있다면
				for (int i = 0;i < rdigra[d].size();i++)
					if (rdigra[d][i] == e)
					{
						rindex = i;				//rindex를 설정해준다.
						break;
					}
			}

			if (flag)							//A->B로의 간선이 있다면
			{
				for (int i = index;i<digra[e].size() - 1;i++)
					digra[e][i] = digra[e][i + 1];		
				digra[e].pop_back();			//정방향 그래프에서 간선을 제거해준다.
				for (int i = rindex;i < rdigra[d].size() - 1;i++)
					rdigra[d][i] = rdigra[d][i + 1];
				rdigra[d].pop_back();			//역방향 그래프에서 간선을 제거해준다.

			}

			else							//A->B로의 간선이 없다면
			{
				digra[e].push_back(d);		//정방향 그래프에서 간선을 생성해준다.
				rdigra[d].push_back(e);		//역방향 그래프에서 간선을 생성해준다.
			}

			func();							//SCC를 구해준다.

			cout << scc[e] << "\n";			//회원번호 A의 leader를 출력한다.
			break;
		}
		case '0':
		{
			cout << numberOfSCC << "\n";	//SCC의 수를 출력한다.
			return 0;						//프로그램 종료
		}
		}
	}
}