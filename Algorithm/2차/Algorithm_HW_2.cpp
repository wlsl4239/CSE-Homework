#include <iostream>
#include <vector>
#include <stack>
#include <string>
#include <fstream>

using namespace std;

typedef long long ll;

char filename[30];		//���ϸ�
int _num;				//ȸ����ȣ
char _name[25];			//ȸ���̸�
char _phoneNum[15];		//����ó

int scc[5001];			//ȸ���� ���Ե� �׷��� leader�� �����ϴ� �迭
bool visited[5001];		//������ dfs�� visit ���θ� �����ϴ� �迭
bool rvisited[5001];	//������ dfs�� visit ���θ� �����ϴ� �迭
pair< int, pair<string, string> > people[5001]; //ȸ�������� �����ϴ� �迭. first : ȸ����ȣ, second.first : �̸�, second.second : ����ó
vector<int> digra[5001];	//������ �׷���. ���� �׷��� ����
vector<int> rdigra[5001];	//������ �׷���. SCC�� ���Ҷ� ����Ѵ�.
stack<int> stac;			//������ �׷����� DFS�� ������ �� ������ �Ϸ�� vertex���� �����ϴ� stack.
int numberOfSCC;		//SCC�� ���� �����ϴ� ����. 
int n, m;				//����, ������ ��


void dfs(int now)						//������ dfs
{
	if (visited[now])					//�湮�� vertex���
		return;							//�Լ� ����
	visited[now] = true;				//���� vertex�� �湮ó��
	for (int i = 0;i<digra[now].size();i++)	
		dfs(digra[now][i]);				//����� vertex�� ���� DFS

	stac.push(now);						//�Ϸ�� vertex���� stack�� �����Ѵ�.
}

void rdfs(int now, int leader)			//������ dfs
{
	if (rvisited[now])					//�湮�� vertex���
		return;							//�Լ� ����
	rvisited[now] = true;				//���� vertex�� �湮ó��.
	for (int i = 0;i<rdigra[now].size();i++)
		rdfs(rdigra[now][i], leader);	//����� vertex�� ���� ������ DFS

	scc[now] = leader;					//���� vertex�� leader�� �������ش�.
}


void func()									//SCC�� ���ϴ� �Լ�.
{
	numberOfSCC = 0;					//SCC�� ���� 0���� �ʱ�ȭ�Ѵ�.
	for (int i = 0;i<n;i++)
	{
		visited[i] = false;					//visited �迭��
		rvisited[i] = false;				//rvisited �迭�� �ʱ�ȭ�Ѵ�.
	}

	for (int i = 0;i<n;i++)
		if (!visited[i])
			dfs(i);

	for (int i = 0;i<n;i++)	
	{
		if (!rvisited[stac.top()]) {		//������ top�� �湮���� �ʾҴٸ�
			rdfs(stac.top(), stac.top());	//�ش� vertex���� ������ DFS�� �����Ѵ�. �� �� leader�� �� vertex�̴�.
			numberOfSCC++;					//SCC�� ���� ������Ų��.
		}
												//�湮�Ǿ��ٸ� �ƹ��ൿ�� ���� �ʰ�
		stac.pop();							//pop������ �����Ѵ�. 
	}
}



int main()
{
//	freopen("query_1000.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);			//���������

	printf("�Է������� �̸��� �Է��ϼ��� : ");
	scanf("%s", filename);						//���� �̸��� �Է¹޴´�.
	ifstream fi;								//ifstream ����
	fi.open(filename);							//�Է¹��� ������ �����Ѵ�.

	fi >> n >> m;								//������ ������ ������ �Է¹޴´�.

	for (int i = 0;i<n;i++)						//���� ������ �Է¹޴´�.
		fi >> people[i].first >> people[i].second.first >> people[i].second.second;	//��ȣ,�̸�,����ó
	for (int i = 0;i<m;i++)						//���� ������ �Է¹޴´�.
	{
		int a, b;
		fi >> a >> b;
		digra[a].push_back(b);					//a������ b������ ���� ���� ����.
		rdigra[b].push_back(a);					//������ �׷��� ����.
	}

	func();										//�Է¹��� �׷����� SCC�� ���Ѵ�.

	char query;
	while (1)
	{
		scanf("%c", &query);					//������ �Է¹޴´�.

		switch (query)
		{
		case 'T':								//'T' ����. ���� : T S(ȸ����ȣ)
		{
			int __num;							//�Է¹��� ȸ���� ȸ����ȣ
			int index;							//�Է¹��� ȸ���� leader�� people�� ���° index�� �ִ��� �����ϴ� ����.
			cin >> __num;						//ȸ����ȣ�� �Է¹޴´�.
			
			for (int i = 0;i < 5001;i++)
				if (people[i].first == scc[__num]) {		//�Է¹��� ȸ���� leader�� people���� ã�� index�� ����Ѵ�.
					index = i;
					break;
				}

			cout << people[index].first << " " << people[index].second.first << " " << people[index].second.second << "\n";
												//�Է¹��� ȸ���� leader�� ������ ����Ѵ�.

			break;
		}

		case 'A':								//'A' ����. ���� : A E(ȸ����ȣ A) D(ȸ����ȣ B)
		{
			int e, d;							//ȸ����ȣ A�� B
			bool flag = false;					//A->B���� ������ �ִ��� Ȯ���ϴ� ����.
			int index = 0;						//������ ���� �� B�� digra[A]�� ���°�� �ִ��� �����ϴ� ����.
			int rindex = 0;						//������ ���� �� A�� rdigra[B]�� ���°�� �ִ��� �����ϴ� ����.
			cin >> e >> d;

			for (int i = 0;i<digra[e].size();i++)
				if (digra[e][i] == d)			//A->B���� ������ �ִٸ�
				{
					flag = true;				//flag�� true�� �ϰ�
					index = i;					//index�� �������ش�.
					break;
				}
			if (flag) {							//A->B���� ������ �ִٸ�
				for (int i = 0;i < rdigra[d].size();i++)
					if (rdigra[d][i] == e)
					{
						rindex = i;				//rindex�� �������ش�.
						break;
					}
			}

			if (flag)							//A->B���� ������ �ִٸ�
			{
				for (int i = index;i<digra[e].size() - 1;i++)
					digra[e][i] = digra[e][i + 1];		
				digra[e].pop_back();			//������ �׷������� ������ �������ش�.
				for (int i = rindex;i < rdigra[d].size() - 1;i++)
					rdigra[d][i] = rdigra[d][i + 1];
				rdigra[d].pop_back();			//������ �׷������� ������ �������ش�.

			}

			else							//A->B���� ������ ���ٸ�
			{
				digra[e].push_back(d);		//������ �׷������� ������ �������ش�.
				rdigra[d].push_back(e);		//������ �׷������� ������ �������ش�.
			}

			func();							//SCC�� �����ش�.

			cout << scc[e] << "\n";			//ȸ����ȣ A�� leader�� ����Ѵ�.
			break;
		}
		case '0':
		{
			cout << numberOfSCC << "\n";	//SCC�� ���� ����Ѵ�.
			return 0;						//���α׷� ����
		}
		}
	}
}