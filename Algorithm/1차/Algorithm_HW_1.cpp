/* Algorithm_HW_01 - Red-Black Tree */
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

typedef long long ll;

char filename[30];																			//입력 파일명
int _num;																					//입력 회원 번호
char _name[25];																				//입력 회원 이름
char _phoneNum[15];																			//입력 회원 전화번호
int _x;																						//입력 회원이 처음에 갖는 땅의 x값
int _y;																						//입력 회원이 처음에 갖는 땅의 y값
int _grade;																					//입력 회원 등급
int _nowCost;																				//입력 회원의 현재 금액

pair<int, int> ground[1010][1010];															//x,y에 해당하는 땅주인의 회원번호, 현재 땅 가격
pair<int, int> rich5[5];																	//금액이 가장 많은 5명.  현재 금액, 회원 번호
																							//index 0이 가장 금액이 많은 회원이며
																							//index 1~4까지 2번째, 3번째, 4번째, 5번째 순서이다.

void swap(pair<int, int> *a, pair<int, int>* b)												//swap 연산
{
	pair<int, int>* temp = a;
	a = b;
	b = temp;
}

int minimum(int a, int b)																	//두 값중 더 작은 값을 반환하는 함수
{
	return a<b ? a : b;
}

int maximum(int a, int b)																	//두 값중 더 큰 값을 반환하는 함수
{
	return a>b ? a : b;
}

void rich(int cost,int num)																	//금액이 가장 많은 5명을 구하는 함수.
{
	if (cost > rich5[0].first || (cost == rich5[0].first && num < rich5[0].second))			// 금액이 가장 높거나, 최대금액과 같은데 회원번호가 더 낮은경우
	{
		rich5[4] = rich5[3];
		rich5[3] = rich5[2];
		rich5[2] = rich5[1];		
		rich5[1] = rich5[0];																//	index 0~3번 값을 뒤로 한칸씩 미루고
		rich5[0].first = cost;																//	0번 인덱스에 현재 값을 넣는다.
		rich5[0].second = num;
	}
	else if (cost > rich5[1].first || (cost == rich5[1].first && num < rich5[1].second))	// 금액이 2번째로 높거나, 2번째 금액과 같은데 회원번호가 더 낮은경우
	{
		rich5[4] = rich5[3];
		rich5[3] = rich5[2];
		rich5[2] = rich5[1];																//	index 1~3번 값을 뒤로 한칸씩 미루고
		rich5[1].first = cost;																//	0번 인덱스에 현재 값을 넣는다.
		rich5[1].second = num;
	}
	else if (cost > rich5[2].first || (cost == rich5[2].first && num < rich5[2].second))	// 금액이 3번째로 높거나, 3번째 금액과 같은데 회원번호가 더 낮은경우
	{
		rich5[4] = rich5[3];
		rich5[3] = rich5[2];																//	index 2~3번 값을 뒤로 한칸씩 미루고
		rich5[2].first = cost;																//	0번 인덱스에 현재 값을 넣는다.
		rich5[2].second = num;
	}
	else if (cost > rich5[3].first || (cost == rich5[3].first && num < rich5[3].second))	// 금액이 4번째로 높거나, 5번째 금액과 같은데 회원번호가 더 낮은경우
	{
		rich5[4] = rich5[3];																//	index 3번 값을 뒤로 한칸 미루고
		rich5[3].first = cost;																//	3번 인덱스에 현재 값을 넣는다.
		rich5[3].second = num;
	}
	else if (cost > rich5[4].first || (cost == rich5[4].first && num < rich5[4].second))	// 금액이 5번째로 높거나, 5번째 금액과 같은데 회원번호가 더 낮은경우
	{
		rich5[4].first = cost;																//	4번 인덱스에 현재 값을 넣는다.
		rich5[4].second = num;
	}
}


class node {																				//red-black tree의 노드
public:
	int num;																				//회원 번호
	string name;																			//회원 이름
	string phoneNum;																		//회원 전화번호
	pair<int, int> xy;																		//회원이 처음에 갖는 땅의 x,y값
	int grade;																				//회원 등급
	int nowCost;																			//회원의 현재 금액
	vector< pair<bool, int> > history;														//변동내역. +- , changed cost
	bool color;																				//0 : red, 1 : black

	node *left;																				//왼쪽 자식노드
	node *right;																			//오른쪽 자식노드
	node *parent;																			//부모노드

	node() : num(0), xy(make_pair(NULL, NULL)), grade(0), nowCost(0), parent(NULL), left(NULL), right(NULL){}		//initializer
	node(int _num, string _name, string _phoneNum, pair<int, int> _xy, int _grade, int _nowCost, bool _color) : num(_num),name(_name),phoneNum(_phoneNum),xy(_xy),grade(_grade),nowCost(_nowCost),color(_color){}		//initializer
	void setval(int _num, string _name, string _phoneNum, pair<int, int> _xy, int _grade, int _nowCost, bool _color) {	//현재 노드의 변수값을 설정하는 함수.
		this->num = _num;
		this->name = _name; 
		this->phoneNum = _phoneNum;
		this->xy = _xy; 
		this->grade = _grade;
		this->nowCost = _nowCost;
		this->color = _color;
	}

	int depth() {																			//노드의 RBT내에서의 depth. root까지 올라가면서 depth를 파악한다. RBT는 balanced binary tree이므로 O(logn)
		node* p = this;
		int cnt = 0;
		while (p->parent != NULL) {															//부모가 NULL이 아니라면 (현재 노드가 root가 아니라면. / 부모가 NULL : root를 의미함.)
			p = p->parent;																	//부모로 이동해가면서
			cnt++;																			//cnt값을 1씩 늘려간다.
		}
		return cnt;																			//cnt값을 반환.
	}

	void makeExternalNode() {																//external node를 만드는 함수.
		left = new node();																	//왼쪽 자식노드 생성
		right = new node();																	//오른쪽 자식노드 생성
		left->parent = this;																//부모를
		right->parent = this;																//현재 노드로 설정해주고
		left->color = 1;																	//color를
		right->color = 1;																	//Black으로 설정해준다.
	}

};

class RBT
{
public:
	node* root;		

	RBT() {																					//Initializer
		root = NULL;
	}
	bool doubleRed(node* n) {																//doubleRed
		if (n->color == 0 && n->parent->color == 0)											//현재노드와 부모노드가 모두 red인 경우.
			return true;																	// true를 반환
		else																				//그렇지 않다면
			return false;																	//false를 반환.
	}
	node * sibling(node * n) {																//sibling 노드 : 같은 부모의 다른노드. 형제노드.
		if (n->parent->left == n)															//자신이 왼쪽자식이라면
			return n->parent->right;														//오른쪽 자식을 반환.
		else																				//오른쪽 자식이라면
			return n->parent->left;															//왼쪽 자식을 반환.
	}
	bool isBlack(node * n) {																//자신의 color가 black인가?
		return n->color;																	//0 = false = red , 1 = true = black
	}
	bool isExternal(node * n) {																//자신이 external node인가?
		if (n->left == NULL && n->right == NULL)											//왼쪽자식, 오른쪽자식이 모두 NULL이면 external node이므로
			return true;																	//true를 반환, 그렇지않다면
		return false;																		//false를 반환
	}

	node* search(int _num) {																//_num을 회원 번호로 갖는 node, 없다면 적합한 위치의 external node를 찾는 함수. 최대 depth만큼 수행하므로 O(logn)
		node * n = root;																	//root부터 시작해서
		while (n->num != _num && !isExternal(n)) {											//값을 찾거나 external node일때까지 반복한다.
			if (n->num < _num)																//값이 크다면
				n = n->right;																//오른쪽으로
			else																			//값이 작다면
				n = n->left;																//왼쪽으로 이동한다.
		}
		return n;																			//찾은 node를 반환
	}

	void restructuring(node* n) {															//restructuring. doubleRed고, 부모의 sibling노드의 color가 black이면 수행한다.
		node *p = n->parent;
		node *gp = p->parent;
		node *t1, *t2, *t3, *t4, *a, *b, *c;
																							//                          a            (black)		restructuring 후의 구조이다.
																							//                     b        c		 (red)			이를 위해 node형 pointer를 7개 선언.
																							//                   t1 t2    t3 t4
		int minn = minimum(n->num, minimum(p->num, gp->num));
		gp->color = 0;
		if (gp->num == minn) {
			b = gp;
			if (n->num > p->num) {															//        gp                         p
				t1 = gp->left;																//      t1    p         ->        gp    n
				t2 = p->left;																//          t2  n            t1 t2    t3 t4
				t3 = n->left;																//            t3 t4 
				t4 = n->right;
				a = p;
				c = n;
			}
			else {
				t1 = gp->left;																//        gp                        n
				t2 = n->left;																//     t1     p         ->       gp    p
				t3 = n->right;																//          n   t4            t1 t2    t3 t4
				t4 = p->right;																//		  t2 t3
				a = n;
				c = p;
			}
		}
		else {
			c = gp;
			if (n->num > p->num) {															//        gp                         n
				t1 = p->left;																//      p   t4           ->      p      gp
				t2 = n->left;																//    t1  n                   t1  t2   t3  t4
				t3 = n->right;																//      t2 t3
				t4 = gp->right;
				a = n;
				b = p;
			}
			else {
				t1 = n->left;																//        gp                         p
				t2 = n->right;																//      p   t4           ->      n       gp
				t3 = p->right;																//    n  t3                    t1 t2    t3 t4
				t4 = gp->right;																//  t1 t2
				a = p;
				b = n;
			}
		}
		if (gp != root) {																	//gp가 root가 아니라면
			node* tmp = gp->parent;															//gp의 부모와 연결시켜준다.
			if (tmp->right == gp)															//gp가 오른쪽자식이였다면
				tmp->right = a;																//gp의 부모의 오른쪽에 연결.
			else																			//왼쪽 자식이였다면
				tmp->left = a;																//gp의 부모의 왼쪽에 연결해준다.
			a->parent = tmp;																//gp의 부모와 a를 연결함으로써 연결끝.
		}
		else {
			root = a;																		//gp가 root라면
			root->parent = NULL;															//a가 root가되고, a의 부모를 NULL로.
		}
																							//아래는 실제 restructuring과정이다.
		a->color = 1;																		//a : black
		b->color = 0;
		c->color = 0;																		//b,c : red
		a->left = b;																		
		a->right = c;																		//a와 b,c를 연결하고
		b->left = t1;
		b->right = t2;
		c->left = t3;
		c->right = t4;																		//subtree들을 연결한다.
		b->parent = a;																		
		c->parent = a;																		//b,c의 부모를 a로 설정하고
		t1->parent = b;
		t2->parent = b;
		t3->parent = c;
		t4->parent = c;																		//subtree들의 부모를 설정해준다.
	}

	node * recoloring(node * n) {															//recoloring. doubleRed고, 부모의 sibling노드의 color가 red이면 수행한다.
		sibling(n->parent)->color = 1;														//부모의 siblig노드의 color를 black으로,
		n->parent->color = 1;																//부모의 color를 black으로 설정.

		if (n->parent->parent != root) {													//조부모가 root가 아니라면
			n->parent->parent->color = 0;													//red로 설정하고, root라면
			return n->parent->parent;														//root노드는 black이어야하므로 원래대로 black으로설정.
		}
		return n;
	}

	void insertNode(int __num, string __name, string __phoneNum, int __x, int __y, int __grade, int __nowCost) {	//node 삽입.

		if (!root) {																					//root가 없다면
			root = new node(__num, __name, __phoneNum, make_pair(__x, __y), __grade, __nowCost, 1);		//root를 만들어주고
			root->makeExternalNode();																	//externalnode를 붙여준다.
		}
		else																							//root가 있다면
		{
			node * n = search(__num);																	//위치를 찾은 후
			if (isExternal(n)) {																		//external node라면 (중간에 __num값을 갖는 중복node가 없었다면)
				n->setval(__num, __name, __phoneNum, make_pair(__x, __y), __grade, __nowCost, 0);		//입력받은 값을 node에 설정해준 후
				n->makeExternalNode();																	//external node를 붙여준다.
				while (doubleRed(n)) {																	//이때 doubleRed가 발생했다면
					if (isBlack(sibling(n->parent)) == 1) {												//부모의 sibling노드의 color에 따라
						restructuring(n);																//restructuring과
						return;
					}
					else
						n = recoloring(n);																//recoloring을 수행해준다.
				}
			}
		}
	}
	void preorder(node* n) {																//금액이 가장 많은 5명을 구하기 위해 preorder traversal을 수행한다.
		if (n == NULL)																		//끝까지 내려갔다면 
			return;																			//return
		rich(n->nowCost, n->num);															//현재 값에 따른 rich함수를 수행. rich5 배열을 update 시킨다.
		preorder(n->left);																	//왼쪽자식부터
		preorder(n->right);																	//오른쪽 자식 순으로 preorder를 수행한다.
	}
};

int main()
{

//	freopen("query_50k.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);

	RBT tree;																				//red-black Tree 생성.
	printf("입력파일의 이름을 입력하세요 : ");
	scanf("%s", filename);																	//input파일명을 입력받는다.
	ifstream fi;																			//input stream 생성 후
	fi.open(filename);																		//file을 open한다.
	while (fi.good()) {																		//file에서 읽어올 데이터가 있다면 받복.
		fi >> _num >> _name >> _phoneNum >> _x >> _y >> _grade >> _nowCost;					//값들을 읽어온 후
		tree.insertNode(_num, _name, _phoneNum, _x, _y, _grade, _nowCost);					//tree에 node를 생성해 넣어준다.
		if(ground[_x][_y].first == 0)														//땅주인이 없다면
			ground[_x][_y].first = _num;													//땅주인을 설정해준다.
		
	}
	tree.preorder(tree.root);																//한번 preorder를 돌아서 input list에 따른 
																							//금액이 가장 많은 5명을 구해 놓는다.
	char query;
	while (1)
	{
		scanf("%c", &query);																//쿼리를 입력받는다.

		switch (query)
		{
		case 'I':																			//I : 신규 회원가입
		{
			int __num;																		//회원 번호
			char __name[25];																//회원 이름
			char __phoneNum[15];															//회원 연락처
			int __x;																		//회원 땅 x좌표
			int __y;																		//회원 땅 y좌표
			scanf("%d %s %s %d %d",&__num,__name,__phoneNum,&__x,&__y);						//data들을 입력받고

			if (!tree.isExternal(tree.search(__num)))										//동일한 회원번호가 존재한다면
				printf("%d 0\n", tree.search(__num)->depth());								//그 노드의 깊이와 거절(0)를 출력

			else
			{
				tree.insertNode(__num, __name, __phoneNum, __x, __y, 0, 0);	
																							//입력받은 data로 node를 생성한 후 tree에 넣어준다.
																							//이 때 회원등급과 금액은 0원으로 시작한다.
				printf("%d 1\n", tree.search(__num)->depth());								//node의 깊이와 승인(1)를 출력.
				if (ground[__x][__y].first == 0)
					ground[__x][__y].first = __num;											//땅의 주인이 없다면 주인으로 설정.
			}

			break;
		}

		case 'P':																			//P : 회원정보 확인: 특정 회원의 정보 출력
		{
			int __num;																		//회원 번호
			scanf("%d", &__num);

			node * n = tree.search(__num);
			if (tree.isExternal(n))
				printf("Not found!\n");
			else
				printf("%s %s %d %d %d\n", n->name.c_str(), n->phoneNum.c_str(), n->grade, n->nowCost, n->depth());
			break;
		}

		case 'A':																			//A : 충전: 특정 회원의 캐쉬 충전처리
		{
			int __num;																		//회원 번호
			int __cash;																		//충전 금액
			scanf("%d %d", &__num,&__cash);
			
			node * n = tree.search(__num);													//회원번호에 따른 노드를 찾아서
			if (tree.isExternal(n))															//그 노드가 external이라면 회원번호를 갖는 노드가 없다는 뜻이므로
				printf("Not found!\n");														//Not found를 출력한다.

			else																			//회원이 있다면
			{
				n->history.push_back(make_pair(1, __cash));									//변동내역에 충전값을 추가해주고
				n->nowCost += __cash;														//현재 금액에 충전값만큼 더해준다.
				if (n->nowCost >= 100000)
					n->grade = 3;
				else if (n->nowCost >= 50000)
					n->grade = 2;
				else if (n->nowCost >= 30000)
					n->grade = 1;
				else
					n->grade = 0;															//금액에 따른 등급을 재설정해준다.


				bool inArray = false;														//현재 회원이 rich5배열에 있는지 체크하는 flag.
				for (int i = 0;i<5;i++)
					if (rich5[i].second == __num)											//rich5에 있다면
					{
						inArray = true;														//flag를 true로.
						rich5[i].first += __cash;											//배열안의 금액을 추가해준 후
						
						for (int j = i;j > 0;j--) {
							if (rich5[j].first > rich5[j - 1].first)
								swap(rich5[j], rich5[j - 1]);								//맞는 위치로 이동시킨다.
						}
						break;

					}
				if(!inArray)																//배열에 없다면
					rich(n->nowCost, n->num);												//rich함수를 수행하여 배열을 update한다.

				printf("%d %d\n", n->depth(), n->grade);									//n의 depth와 등급을 출력한다.
			}
			break;
		}

		case 'F':																			//F : 검색: 현재금액이 가장 높은 상위 5명의 정보 출력
		{
			for (int i = 0;i < 5;i++) {
				if (rich5[i].second == 0 && rich5[i].first == 0)
					break;
				printf("%d %d\n", rich5[i].second, rich5[i].first);							//rich5 배열을 출력.
			}

			break;
		}

		case 'R':																			//R : 특정 회원의 최근 금액변동내역 조회
		{
			int __num;																		//회원 번호
			int __n;																		//조회할 변동내역 수
			scanf("%d %d", &__num, &__n);
			node * n = tree.search(__num);													//회원번호에 따른 노드를 찾아서


			if (tree.isExternal(n))															//그 노드가 external이라면 회원번호를 갖는 노드가 없다는 뜻이므로
				printf("Not found!\n");														//Not found를 출력한다.
			else {
				int size = n->history.size();
				if (__n > size)
					__n = size;																//만약 금액변동내역의 수가 __n보다 작으면
																							//존재하는 금액변동내역에 대해서만 출력한다.

				if (n->history.empty())														//금액 변동내역이 없다면
					printf("0\n");															//0을 출력
				else {
					for (int i = size - 1; i >= size - __n;i--)								//금액 변동내역 출력.
						printf("%d %d\n", n->history[i].first, n->history[i].second);
				}
			}
			break;

		}
		case 'B':																			//B : 땅 구매: 특정 회원의 땅 구매처리
		{
			int __num;																		//회원 번호
			int __x;																		//구매할 땅의 x좌표
			int __y;																		//구매할 땅의 y좌표
			int __cost;																		//거래금액

			scanf("%d %d %d %d", &__num, &__x, &__y, &__cost);

			node * n = tree.search(__num);													//회원번호에 따른 노드를 찾아서
			
			if (ground[__x][__y].second > __cost) {											//거래금액이 땅값보다 작다면 거래불가.
				if (ground[__x][__y].first == 0)
					printf("0 %d -1\n", n->nowCost);										//땅주인이 없었다면 구매실패(0), 구매자의 잔액, -1출력
				else
					printf("0 %d %d\n", n->nowCost, ground[__x][__y].first);				//있다면 구매실패(0), 구매자의 잔액, 원래 땅주인 출력.
			}

			else if (n->nowCost < __cost)													//구매자의 현재금액이 거래금액보다 작다면 거래불가.
			{

				if (ground[__x][__y].first == 0)
					printf("0 %d -1\n", n->nowCost);										//땅주인이 없었다면 구매실패(0), 구매자의 잔액, -1출력
				else
					printf("0 %d %d\n", n->nowCost, ground[__x][__y].first);				//있다면 구매실패(0), 구매자의 잔액, 원래 땅주인 출력.
			}
			//아래는 구매가능할 경우.
			else if (ground[__x][__y].first == 0)											//땅 주인이 없었을 경우
			{
				n->nowCost -= __cost;														//거래금액만큼 차감
				ground[__x][__y].first = __num;												//땅주인을 구매자로 설정
				ground[__x][__y].second = __cost;											//땅값을 거래금액으로 설정
				n->history.push_back(make_pair(0, __cost));									//금액 변동내역에 추가
				if (n->nowCost >= 100000)
					n->grade = 3;
				else if (n->nowCost >= 50000)
					n->grade = 2;
				else if (n->nowCost >= 30000)
					n->grade = 1;
				else
					n->grade = 0;															//구매자의 현재금액에 따른 등급 설정.

				bool inArray = false;														//현재 회원이 rich5배열에 있는지 체크하는 flag.
				for(int i=0;i<5;i++)
					if (rich5[i].second == __num)											//rich5에 있다면
					{
						inArray = true;														//flag를 true로.
						break;																
					}

				if (inArray)																//rich5에 있다면
				{																			
					for (int i = 0;i < 5;i++)
						rich5[i] = make_pair(0, 0);											//rich5배열을 초기화한다. 배열 내부 값이 감소되어 모든 회원을 봐야하기 때문이다.
					tree.preorder(tree.root);
				}
													
				printf("1 %d %d\n", n->nowCost,ground[__x][__y].first);
			}
			else																			//땅주인이 있을경우
			{
				node * seller = tree.search(ground[__x][__y].first);						//땅주인을 search.
				n->nowCost -= __cost;														//구매자의 현재금액을 감소시킨다.
				seller->nowCost += __cost;													//땅주인의 현재금액을 증가시킨다.
				n->history.push_back(make_pair(0, __cost));									//구매자의 금액변동네역에 추가
				seller->history.push_back(make_pair(1, __cost));							//땅주인의 금액변동네역에 추가
				ground[__x][__y].first = __num;												//땅의 주인을 구매자로 설정
				ground[__x][__y].second = __cost;											//땅값을 거래금액으로 설정.
				if (n->nowCost >= 100000)									
					n->grade = 3;											
				else if (n->nowCost >= 50000)								
					n->grade = 2;											
				else if (n->nowCost >= 30000)								
					n->grade = 1;											
				else														
					n->grade = 0;															//구매자의 현재 금액에따른 등급설정
																							
				if (seller->nowCost >= 100000)												
					seller->grade = 3;														
				else if (seller->nowCost >= 50000)											
					seller->grade = 2;														
				else if (seller->nowCost >= 30000)											
					seller->grade = 1;														
				else																		
					seller->grade = 0;														//판매자의 현재 금액에따른 등급설정
																							
				bool sellerInArray = false;													//판매자가 rich5배열에 있는지 확인하는 flag
				for (int i = 0;i<5;i++)														
					if (rich5[i].second == seller->num)										//배열에 있다면
					{																		
						sellerInArray = true;												//flag를 true로 설정
						rich5[i].first += __cost;											//값을 늘려주고
																							
						for (int j = i;j > 0;j--)											
							if (rich5[j].first > rich5[j - 1].first)						
								swap(rich5[j], rich5[j - 1]);								//맞는 위치로 이동시킨다.
						break;																
																							
					}																		
				if (!sellerInArray)															//배열에 없다면
					rich(n->nowCost, n->num);												//rich함수를 수행하여 배열을 update한다.
																							
				bool buyerInArray = false;													//구매자가 rich5배열에 있는지 확인하는 flag
				for (int i = 0;i<5;i++)														
					if (rich5[i].second == __num)											//배열에 있다면
					{																		
						buyerInArray = true;												//flag를 true로 설정
						break;																
					}																		
																							
				if (buyerInArray)															//배열에 있다면
				{																			
					for (int i = 0;i < 5;i++)												
						rich5[i] = make_pair(0, 0);											//rich5배열을 초기화한다. 배열 내부 값이 감소되어 모든 회원을 봐야하기 때문이다.
					tree.preorder(tree.root);												//root부터 preorder traversal을 수행하며 배열을 다시 update한다.
				}																			
																							
				if(seller->num == n->num)													//자신의 땅을 구매할경우
					printf("0 %d %d\n", n->nowCost, ground[__x][__y].first);				//구매 실패
				else																		
					printf("1 %d %d\n", n->nowCost, ground[__x][__y].first);				//
																							
			}																				
			break;																			
																							
		}																					
		case 'Q':																			//Q : 프로그램 종료
		{																					
			return 0;																		// 종료
		}
		}
	}
}