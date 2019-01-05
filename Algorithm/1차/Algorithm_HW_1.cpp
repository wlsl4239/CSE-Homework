/* Algorithm_HW_01 - Red-Black Tree */
#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

typedef long long ll;

char filename[30];																			//�Է� ���ϸ�
int _num;																					//�Է� ȸ�� ��ȣ
char _name[25];																				//�Է� ȸ�� �̸�
char _phoneNum[15];																			//�Է� ȸ�� ��ȭ��ȣ
int _x;																						//�Է� ȸ���� ó���� ���� ���� x��
int _y;																						//�Է� ȸ���� ó���� ���� ���� y��
int _grade;																					//�Է� ȸ�� ���
int _nowCost;																				//�Է� ȸ���� ���� �ݾ�

pair<int, int> ground[1010][1010];															//x,y�� �ش��ϴ� �������� ȸ����ȣ, ���� �� ����
pair<int, int> rich5[5];																	//�ݾ��� ���� ���� 5��.  ���� �ݾ�, ȸ�� ��ȣ
																							//index 0�� ���� �ݾ��� ���� ȸ���̸�
																							//index 1~4���� 2��°, 3��°, 4��°, 5��° �����̴�.

void swap(pair<int, int> *a, pair<int, int>* b)												//swap ����
{
	pair<int, int>* temp = a;
	a = b;
	b = temp;
}

int minimum(int a, int b)																	//�� ���� �� ���� ���� ��ȯ�ϴ� �Լ�
{
	return a<b ? a : b;
}

int maximum(int a, int b)																	//�� ���� �� ū ���� ��ȯ�ϴ� �Լ�
{
	return a>b ? a : b;
}

void rich(int cost,int num)																	//�ݾ��� ���� ���� 5���� ���ϴ� �Լ�.
{
	if (cost > rich5[0].first || (cost == rich5[0].first && num < rich5[0].second))			// �ݾ��� ���� ���ų�, �ִ�ݾװ� ������ ȸ����ȣ�� �� �������
	{
		rich5[4] = rich5[3];
		rich5[3] = rich5[2];
		rich5[2] = rich5[1];		
		rich5[1] = rich5[0];																//	index 0~3�� ���� �ڷ� ��ĭ�� �̷��
		rich5[0].first = cost;																//	0�� �ε����� ���� ���� �ִ´�.
		rich5[0].second = num;
	}
	else if (cost > rich5[1].first || (cost == rich5[1].first && num < rich5[1].second))	// �ݾ��� 2��°�� ���ų�, 2��° �ݾװ� ������ ȸ����ȣ�� �� �������
	{
		rich5[4] = rich5[3];
		rich5[3] = rich5[2];
		rich5[2] = rich5[1];																//	index 1~3�� ���� �ڷ� ��ĭ�� �̷��
		rich5[1].first = cost;																//	0�� �ε����� ���� ���� �ִ´�.
		rich5[1].second = num;
	}
	else if (cost > rich5[2].first || (cost == rich5[2].first && num < rich5[2].second))	// �ݾ��� 3��°�� ���ų�, 3��° �ݾװ� ������ ȸ����ȣ�� �� �������
	{
		rich5[4] = rich5[3];
		rich5[3] = rich5[2];																//	index 2~3�� ���� �ڷ� ��ĭ�� �̷��
		rich5[2].first = cost;																//	0�� �ε����� ���� ���� �ִ´�.
		rich5[2].second = num;
	}
	else if (cost > rich5[3].first || (cost == rich5[3].first && num < rich5[3].second))	// �ݾ��� 4��°�� ���ų�, 5��° �ݾװ� ������ ȸ����ȣ�� �� �������
	{
		rich5[4] = rich5[3];																//	index 3�� ���� �ڷ� ��ĭ �̷��
		rich5[3].first = cost;																//	3�� �ε����� ���� ���� �ִ´�.
		rich5[3].second = num;
	}
	else if (cost > rich5[4].first || (cost == rich5[4].first && num < rich5[4].second))	// �ݾ��� 5��°�� ���ų�, 5��° �ݾװ� ������ ȸ����ȣ�� �� �������
	{
		rich5[4].first = cost;																//	4�� �ε����� ���� ���� �ִ´�.
		rich5[4].second = num;
	}
}


class node {																				//red-black tree�� ���
public:
	int num;																				//ȸ�� ��ȣ
	string name;																			//ȸ�� �̸�
	string phoneNum;																		//ȸ�� ��ȭ��ȣ
	pair<int, int> xy;																		//ȸ���� ó���� ���� ���� x,y��
	int grade;																				//ȸ�� ���
	int nowCost;																			//ȸ���� ���� �ݾ�
	vector< pair<bool, int> > history;														//��������. +- , changed cost
	bool color;																				//0 : red, 1 : black

	node *left;																				//���� �ڽĳ��
	node *right;																			//������ �ڽĳ��
	node *parent;																			//�θ���

	node() : num(0), xy(make_pair(NULL, NULL)), grade(0), nowCost(0), parent(NULL), left(NULL), right(NULL){}		//initializer
	node(int _num, string _name, string _phoneNum, pair<int, int> _xy, int _grade, int _nowCost, bool _color) : num(_num),name(_name),phoneNum(_phoneNum),xy(_xy),grade(_grade),nowCost(_nowCost),color(_color){}		//initializer
	void setval(int _num, string _name, string _phoneNum, pair<int, int> _xy, int _grade, int _nowCost, bool _color) {	//���� ����� �������� �����ϴ� �Լ�.
		this->num = _num;
		this->name = _name; 
		this->phoneNum = _phoneNum;
		this->xy = _xy; 
		this->grade = _grade;
		this->nowCost = _nowCost;
		this->color = _color;
	}

	int depth() {																			//����� RBT�������� depth. root���� �ö󰡸鼭 depth�� �ľ��Ѵ�. RBT�� balanced binary tree�̹Ƿ� O(logn)
		node* p = this;
		int cnt = 0;
		while (p->parent != NULL) {															//�θ� NULL�� �ƴ϶�� (���� ��尡 root�� �ƴ϶��. / �θ� NULL : root�� �ǹ���.)
			p = p->parent;																	//�θ�� �̵��ذ��鼭
			cnt++;																			//cnt���� 1�� �÷�����.
		}
		return cnt;																			//cnt���� ��ȯ.
	}

	void makeExternalNode() {																//external node�� ����� �Լ�.
		left = new node();																	//���� �ڽĳ�� ����
		right = new node();																	//������ �ڽĳ�� ����
		left->parent = this;																//�θ�
		right->parent = this;																//���� ���� �������ְ�
		left->color = 1;																	//color��
		right->color = 1;																	//Black���� �������ش�.
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
		if (n->color == 0 && n->parent->color == 0)											//������� �θ��尡 ��� red�� ���.
			return true;																	// true�� ��ȯ
		else																				//�׷��� �ʴٸ�
			return false;																	//false�� ��ȯ.
	}
	node * sibling(node * n) {																//sibling ��� : ���� �θ��� �ٸ����. �������.
		if (n->parent->left == n)															//�ڽ��� �����ڽ��̶��
			return n->parent->right;														//������ �ڽ��� ��ȯ.
		else																				//������ �ڽ��̶��
			return n->parent->left;															//���� �ڽ��� ��ȯ.
	}
	bool isBlack(node * n) {																//�ڽ��� color�� black�ΰ�?
		return n->color;																	//0 = false = red , 1 = true = black
	}
	bool isExternal(node * n) {																//�ڽ��� external node�ΰ�?
		if (n->left == NULL && n->right == NULL)											//�����ڽ�, �������ڽ��� ��� NULL�̸� external node�̹Ƿ�
			return true;																	//true�� ��ȯ, �׷����ʴٸ�
		return false;																		//false�� ��ȯ
	}

	node* search(int _num) {																//_num�� ȸ�� ��ȣ�� ���� node, ���ٸ� ������ ��ġ�� external node�� ã�� �Լ�. �ִ� depth��ŭ �����ϹǷ� O(logn)
		node * n = root;																	//root���� �����ؼ�
		while (n->num != _num && !isExternal(n)) {											//���� ã�ų� external node�϶����� �ݺ��Ѵ�.
			if (n->num < _num)																//���� ũ�ٸ�
				n = n->right;																//����������
			else																			//���� �۴ٸ�
				n = n->left;																//�������� �̵��Ѵ�.
		}
		return n;																			//ã�� node�� ��ȯ
	}

	void restructuring(node* n) {															//restructuring. doubleRed��, �θ��� sibling����� color�� black�̸� �����Ѵ�.
		node *p = n->parent;
		node *gp = p->parent;
		node *t1, *t2, *t3, *t4, *a, *b, *c;
																							//                          a            (black)		restructuring ���� �����̴�.
																							//                     b        c		 (red)			�̸� ���� node�� pointer�� 7�� ����.
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
		if (gp != root) {																	//gp�� root�� �ƴ϶��
			node* tmp = gp->parent;															//gp�� �θ�� ��������ش�.
			if (tmp->right == gp)															//gp�� �������ڽ��̿��ٸ�
				tmp->right = a;																//gp�� �θ��� �����ʿ� ����.
			else																			//���� �ڽ��̿��ٸ�
				tmp->left = a;																//gp�� �θ��� ���ʿ� �������ش�.
			a->parent = tmp;																//gp�� �θ�� a�� ���������ν� ���᳡.
		}
		else {
			root = a;																		//gp�� root���
			root->parent = NULL;															//a�� root���ǰ�, a�� �θ� NULL��.
		}
																							//�Ʒ��� ���� restructuring�����̴�.
		a->color = 1;																		//a : black
		b->color = 0;
		c->color = 0;																		//b,c : red
		a->left = b;																		
		a->right = c;																		//a�� b,c�� �����ϰ�
		b->left = t1;
		b->right = t2;
		c->left = t3;
		c->right = t4;																		//subtree���� �����Ѵ�.
		b->parent = a;																		
		c->parent = a;																		//b,c�� �θ� a�� �����ϰ�
		t1->parent = b;
		t2->parent = b;
		t3->parent = c;
		t4->parent = c;																		//subtree���� �θ� �������ش�.
	}

	node * recoloring(node * n) {															//recoloring. doubleRed��, �θ��� sibling����� color�� red�̸� �����Ѵ�.
		sibling(n->parent)->color = 1;														//�θ��� siblig����� color�� black����,
		n->parent->color = 1;																//�θ��� color�� black���� ����.

		if (n->parent->parent != root) {													//���θ� root�� �ƴ϶��
			n->parent->parent->color = 0;													//red�� �����ϰ�, root���
			return n->parent->parent;														//root���� black�̾���ϹǷ� ������� black���μ���.
		}
		return n;
	}

	void insertNode(int __num, string __name, string __phoneNum, int __x, int __y, int __grade, int __nowCost) {	//node ����.

		if (!root) {																					//root�� ���ٸ�
			root = new node(__num, __name, __phoneNum, make_pair(__x, __y), __grade, __nowCost, 1);		//root�� ������ְ�
			root->makeExternalNode();																	//externalnode�� �ٿ��ش�.
		}
		else																							//root�� �ִٸ�
		{
			node * n = search(__num);																	//��ġ�� ã�� ��
			if (isExternal(n)) {																		//external node��� (�߰��� __num���� ���� �ߺ�node�� �����ٸ�)
				n->setval(__num, __name, __phoneNum, make_pair(__x, __y), __grade, __nowCost, 0);		//�Է¹��� ���� node�� �������� ��
				n->makeExternalNode();																	//external node�� �ٿ��ش�.
				while (doubleRed(n)) {																	//�̶� doubleRed�� �߻��ߴٸ�
					if (isBlack(sibling(n->parent)) == 1) {												//�θ��� sibling����� color�� ����
						restructuring(n);																//restructuring��
						return;
					}
					else
						n = recoloring(n);																//recoloring�� �������ش�.
				}
			}
		}
	}
	void preorder(node* n) {																//�ݾ��� ���� ���� 5���� ���ϱ� ���� preorder traversal�� �����Ѵ�.
		if (n == NULL)																		//������ �������ٸ� 
			return;																			//return
		rich(n->nowCost, n->num);															//���� ���� ���� rich�Լ��� ����. rich5 �迭�� update ��Ų��.
		preorder(n->left);																	//�����ڽĺ���
		preorder(n->right);																	//������ �ڽ� ������ preorder�� �����Ѵ�.
	}
};

int main()
{

//	freopen("query_50k.txt", "r", stdin);
//	freopen("output.txt", "w", stdout);

	RBT tree;																				//red-black Tree ����.
	printf("�Է������� �̸��� �Է��ϼ��� : ");
	scanf("%s", filename);																	//input���ϸ��� �Է¹޴´�.
	ifstream fi;																			//input stream ���� ��
	fi.open(filename);																		//file�� open�Ѵ�.
	while (fi.good()) {																		//file���� �о�� �����Ͱ� �ִٸ� �޺�.
		fi >> _num >> _name >> _phoneNum >> _x >> _y >> _grade >> _nowCost;					//������ �о�� ��
		tree.insertNode(_num, _name, _phoneNum, _x, _y, _grade, _nowCost);					//tree�� node�� ������ �־��ش�.
		if(ground[_x][_y].first == 0)														//�������� ���ٸ�
			ground[_x][_y].first = _num;													//�������� �������ش�.
		
	}
	tree.preorder(tree.root);																//�ѹ� preorder�� ���Ƽ� input list�� ���� 
																							//�ݾ��� ���� ���� 5���� ���� ���´�.
	char query;
	while (1)
	{
		scanf("%c", &query);																//������ �Է¹޴´�.

		switch (query)
		{
		case 'I':																			//I : �ű� ȸ������
		{
			int __num;																		//ȸ�� ��ȣ
			char __name[25];																//ȸ�� �̸�
			char __phoneNum[15];															//ȸ�� ����ó
			int __x;																		//ȸ�� �� x��ǥ
			int __y;																		//ȸ�� �� y��ǥ
			scanf("%d %s %s %d %d",&__num,__name,__phoneNum,&__x,&__y);						//data���� �Է¹ް�

			if (!tree.isExternal(tree.search(__num)))										//������ ȸ����ȣ�� �����Ѵٸ�
				printf("%d 0\n", tree.search(__num)->depth());								//�� ����� ���̿� ����(0)�� ���

			else
			{
				tree.insertNode(__num, __name, __phoneNum, __x, __y, 0, 0);	
																							//�Է¹��� data�� node�� ������ �� tree�� �־��ش�.
																							//�� �� ȸ����ް� �ݾ��� 0������ �����Ѵ�.
				printf("%d 1\n", tree.search(__num)->depth());								//node�� ���̿� ����(1)�� ���.
				if (ground[__x][__y].first == 0)
					ground[__x][__y].first = __num;											//���� ������ ���ٸ� �������� ����.
			}

			break;
		}

		case 'P':																			//P : ȸ������ Ȯ��: Ư�� ȸ���� ���� ���
		{
			int __num;																		//ȸ�� ��ȣ
			scanf("%d", &__num);

			node * n = tree.search(__num);
			if (tree.isExternal(n))
				printf("Not found!\n");
			else
				printf("%s %s %d %d %d\n", n->name.c_str(), n->phoneNum.c_str(), n->grade, n->nowCost, n->depth());
			break;
		}

		case 'A':																			//A : ����: Ư�� ȸ���� ĳ�� ����ó��
		{
			int __num;																		//ȸ�� ��ȣ
			int __cash;																		//���� �ݾ�
			scanf("%d %d", &__num,&__cash);
			
			node * n = tree.search(__num);													//ȸ����ȣ�� ���� ��带 ã�Ƽ�
			if (tree.isExternal(n))															//�� ��尡 external�̶�� ȸ����ȣ�� ���� ��尡 ���ٴ� ���̹Ƿ�
				printf("Not found!\n");														//Not found�� ����Ѵ�.

			else																			//ȸ���� �ִٸ�
			{
				n->history.push_back(make_pair(1, __cash));									//���������� �������� �߰����ְ�
				n->nowCost += __cash;														//���� �ݾ׿� ��������ŭ �����ش�.
				if (n->nowCost >= 100000)
					n->grade = 3;
				else if (n->nowCost >= 50000)
					n->grade = 2;
				else if (n->nowCost >= 30000)
					n->grade = 1;
				else
					n->grade = 0;															//�ݾ׿� ���� ����� �缳�����ش�.


				bool inArray = false;														//���� ȸ���� rich5�迭�� �ִ��� üũ�ϴ� flag.
				for (int i = 0;i<5;i++)
					if (rich5[i].second == __num)											//rich5�� �ִٸ�
					{
						inArray = true;														//flag�� true��.
						rich5[i].first += __cash;											//�迭���� �ݾ��� �߰����� ��
						
						for (int j = i;j > 0;j--) {
							if (rich5[j].first > rich5[j - 1].first)
								swap(rich5[j], rich5[j - 1]);								//�´� ��ġ�� �̵���Ų��.
						}
						break;

					}
				if(!inArray)																//�迭�� ���ٸ�
					rich(n->nowCost, n->num);												//rich�Լ��� �����Ͽ� �迭�� update�Ѵ�.

				printf("%d %d\n", n->depth(), n->grade);									//n�� depth�� ����� ����Ѵ�.
			}
			break;
		}

		case 'F':																			//F : �˻�: ����ݾ��� ���� ���� ���� 5���� ���� ���
		{
			for (int i = 0;i < 5;i++) {
				if (rich5[i].second == 0 && rich5[i].first == 0)
					break;
				printf("%d %d\n", rich5[i].second, rich5[i].first);							//rich5 �迭�� ���.
			}

			break;
		}

		case 'R':																			//R : Ư�� ȸ���� �ֱ� �ݾ׺������� ��ȸ
		{
			int __num;																		//ȸ�� ��ȣ
			int __n;																		//��ȸ�� �������� ��
			scanf("%d %d", &__num, &__n);
			node * n = tree.search(__num);													//ȸ����ȣ�� ���� ��带 ã�Ƽ�


			if (tree.isExternal(n))															//�� ��尡 external�̶�� ȸ����ȣ�� ���� ��尡 ���ٴ� ���̹Ƿ�
				printf("Not found!\n");														//Not found�� ����Ѵ�.
			else {
				int size = n->history.size();
				if (__n > size)
					__n = size;																//���� �ݾ׺��������� ���� __n���� ������
																							//�����ϴ� �ݾ׺��������� ���ؼ��� ����Ѵ�.

				if (n->history.empty())														//�ݾ� ���������� ���ٸ�
					printf("0\n");															//0�� ���
				else {
					for (int i = size - 1; i >= size - __n;i--)								//�ݾ� �������� ���.
						printf("%d %d\n", n->history[i].first, n->history[i].second);
				}
			}
			break;

		}
		case 'B':																			//B : �� ����: Ư�� ȸ���� �� ����ó��
		{
			int __num;																		//ȸ�� ��ȣ
			int __x;																		//������ ���� x��ǥ
			int __y;																		//������ ���� y��ǥ
			int __cost;																		//�ŷ��ݾ�

			scanf("%d %d %d %d", &__num, &__x, &__y, &__cost);

			node * n = tree.search(__num);													//ȸ����ȣ�� ���� ��带 ã�Ƽ�
			
			if (ground[__x][__y].second > __cost) {											//�ŷ��ݾ��� �������� �۴ٸ� �ŷ��Ұ�.
				if (ground[__x][__y].first == 0)
					printf("0 %d -1\n", n->nowCost);										//�������� �����ٸ� ���Ž���(0), �������� �ܾ�, -1���
				else
					printf("0 %d %d\n", n->nowCost, ground[__x][__y].first);				//�ִٸ� ���Ž���(0), �������� �ܾ�, ���� ������ ���.
			}

			else if (n->nowCost < __cost)													//�������� ����ݾ��� �ŷ��ݾ׺��� �۴ٸ� �ŷ��Ұ�.
			{

				if (ground[__x][__y].first == 0)
					printf("0 %d -1\n", n->nowCost);										//�������� �����ٸ� ���Ž���(0), �������� �ܾ�, -1���
				else
					printf("0 %d %d\n", n->nowCost, ground[__x][__y].first);				//�ִٸ� ���Ž���(0), �������� �ܾ�, ���� ������ ���.
			}
			//�Ʒ��� ���Ű����� ���.
			else if (ground[__x][__y].first == 0)											//�� ������ ������ ���
			{
				n->nowCost -= __cost;														//�ŷ��ݾ׸�ŭ ����
				ground[__x][__y].first = __num;												//�������� �����ڷ� ����
				ground[__x][__y].second = __cost;											//������ �ŷ��ݾ����� ����
				n->history.push_back(make_pair(0, __cost));									//�ݾ� ���������� �߰�
				if (n->nowCost >= 100000)
					n->grade = 3;
				else if (n->nowCost >= 50000)
					n->grade = 2;
				else if (n->nowCost >= 30000)
					n->grade = 1;
				else
					n->grade = 0;															//�������� ����ݾ׿� ���� ��� ����.

				bool inArray = false;														//���� ȸ���� rich5�迭�� �ִ��� üũ�ϴ� flag.
				for(int i=0;i<5;i++)
					if (rich5[i].second == __num)											//rich5�� �ִٸ�
					{
						inArray = true;														//flag�� true��.
						break;																
					}

				if (inArray)																//rich5�� �ִٸ�
				{																			
					for (int i = 0;i < 5;i++)
						rich5[i] = make_pair(0, 0);											//rich5�迭�� �ʱ�ȭ�Ѵ�. �迭 ���� ���� ���ҵǾ� ��� ȸ���� �����ϱ� �����̴�.
					tree.preorder(tree.root);
				}
													
				printf("1 %d %d\n", n->nowCost,ground[__x][__y].first);
			}
			else																			//�������� �������
			{
				node * seller = tree.search(ground[__x][__y].first);						//�������� search.
				n->nowCost -= __cost;														//�������� ����ݾ��� ���ҽ�Ų��.
				seller->nowCost += __cost;													//�������� ����ݾ��� ������Ų��.
				n->history.push_back(make_pair(0, __cost));									//�������� �ݾ׺����׿��� �߰�
				seller->history.push_back(make_pair(1, __cost));							//�������� �ݾ׺����׿��� �߰�
				ground[__x][__y].first = __num;												//���� ������ �����ڷ� ����
				ground[__x][__y].second = __cost;											//������ �ŷ��ݾ����� ����.
				if (n->nowCost >= 100000)									
					n->grade = 3;											
				else if (n->nowCost >= 50000)								
					n->grade = 2;											
				else if (n->nowCost >= 30000)								
					n->grade = 1;											
				else														
					n->grade = 0;															//�������� ���� �ݾ׿����� ��޼���
																							
				if (seller->nowCost >= 100000)												
					seller->grade = 3;														
				else if (seller->nowCost >= 50000)											
					seller->grade = 2;														
				else if (seller->nowCost >= 30000)											
					seller->grade = 1;														
				else																		
					seller->grade = 0;														//�Ǹ����� ���� �ݾ׿����� ��޼���
																							
				bool sellerInArray = false;													//�Ǹ��ڰ� rich5�迭�� �ִ��� Ȯ���ϴ� flag
				for (int i = 0;i<5;i++)														
					if (rich5[i].second == seller->num)										//�迭�� �ִٸ�
					{																		
						sellerInArray = true;												//flag�� true�� ����
						rich5[i].first += __cost;											//���� �÷��ְ�
																							
						for (int j = i;j > 0;j--)											
							if (rich5[j].first > rich5[j - 1].first)						
								swap(rich5[j], rich5[j - 1]);								//�´� ��ġ�� �̵���Ų��.
						break;																
																							
					}																		
				if (!sellerInArray)															//�迭�� ���ٸ�
					rich(n->nowCost, n->num);												//rich�Լ��� �����Ͽ� �迭�� update�Ѵ�.
																							
				bool buyerInArray = false;													//�����ڰ� rich5�迭�� �ִ��� Ȯ���ϴ� flag
				for (int i = 0;i<5;i++)														
					if (rich5[i].second == __num)											//�迭�� �ִٸ�
					{																		
						buyerInArray = true;												//flag�� true�� ����
						break;																
					}																		
																							
				if (buyerInArray)															//�迭�� �ִٸ�
				{																			
					for (int i = 0;i < 5;i++)												
						rich5[i] = make_pair(0, 0);											//rich5�迭�� �ʱ�ȭ�Ѵ�. �迭 ���� ���� ���ҵǾ� ��� ȸ���� �����ϱ� �����̴�.
					tree.preorder(tree.root);												//root���� preorder traversal�� �����ϸ� �迭�� �ٽ� update�Ѵ�.
				}																			
																							
				if(seller->num == n->num)													//�ڽ��� ���� �����Ұ��
					printf("0 %d %d\n", n->nowCost, ground[__x][__y].first);				//���� ����
				else																		
					printf("1 %d %d\n", n->nowCost, ground[__x][__y].first);				//
																							
			}																				
			break;																			
																							
		}																					
		case 'Q':																			//Q : ���α׷� ����
		{																					
			return 0;																		// ����
		}
		}
	}
}