#include<stdio.h>

typedef struct ListNode List;

struct ListNode
{
	int element;
	struct ListNode *next;
};

typedef struct HashTb1 HashTable;

struct HashTb1
{
	int tableSize;
	List *theList;
};

//函数声明
HashTable init(int size);
HashTable create(int element, HashTable H);
void print(HashTable H);
void freeHashTable(HashTable H);
void find(int x, HashTable H);
HashTable insert(int x, HashTable H);
HashTable  _delete(int x, HashTable H);

HashTable init(int size)
{
	HashTable H;


	H.tableSize = size;

	H.theList = (List*)malloc(sizeof(List)*size);

	if (H.theList == NULL)
	{
		printf("内存申请失败");
		exit(1);
	}
	for (int i = 0; i < size; i++)//初始化散列表的表头
	{
		H.theList[i].next = NULL;
		H.theList[i].element = i;//将表头元素定义为此列表的模值
	}
	return H;
}

HashTable create(int element, HashTable H)
{
	int temp;

	temp = element % 10;//通过模运算找到合适的位置

	List *L = (List*)malloc(sizeof(List));//动态创建List结构体

	for (int i = 0; i < H.tableSize; i++)
	{
		if (H.theList[i].next == NULL)
		{
			if (temp == H.theList[i].element)
			{
				H.theList[i].next = L;
				L->element = element;
				L->next = NULL;
				return H;
			}
		}
		else
		{
			if (temp == H.theList[i].element)
			{
				L->next = H.theList[i].next;//新元素插到前面
				{
					L->next = H.theList[i].next;
					H.theList[i].next = L;
					L->element = element;
					return H;
				}
			}
		}
	}
}

void print(HashTable H)
{
	List *L;

	for (int i = 0; i < H.tableSize; i++)
	{
		printf("%d", H.theList[i].element);
		if (H.theList[i].next != NULL)
		{
			L = H.theList[i].next;
			do
			{
				printf("%d", L->element);
				L = L->next;
			} while (L != NULL);
		}
		printf("\n");
	}
}

void freeHashTable(HashTable H)
{
	List *l;
	List *p;

	for (int i = 0; i < H.tableSize; i++)
	{
		if (H.theList[i].next != NULL)
		{
			l = H.theList[i].next;
			p = H.theList[i].next;

			do
			{
				free(l);
				l = p->next;
				p = l;
			} while (l != NULL);
		}
		free(H.theList);
	}
}

void find(int x, HashTable H)
{
	int tag = 0;
	List *l;

	for (int i = 0; H.theList[i].next; i++)
	{
		if (H.theList[i].next != NULL)
		{
			l = H.theList[i].next;
			while (l != NULL)
			{
				if (l->element == x)
				{
					printf("%d在%d行，位置是%d", x, H.theList[i].element, l);
				}
				l = l->next;
			}
		}
	}
	printf("没有找到");
}

HashTable insert(int x, HashTable H)
{
	int temp;

	temp = x % 10;//通过模运算找到合适的位置

	List *l = (List*)malloc(sizeof(List));

	for (int i = 0; i < H.theList[i].element;i++)
	{
		if (temp == H.theList[i].element)
		{
			l->next = H.theList[i].next;
			H.theList[i].next = l;
			l->element = x;
			return H;
		}
	}
}

HashTable _delete(int x, HashTable H)
{
	List *l;
	List *temp;

	for (int i = 0; i < H.tableSize; i++)
	{
		l = H.theList[i].next;
		temp = H.theList;

		if (H.theList[i].next != NULL)
		{
			do
			{
				if (l->element == x)
				{
					temp->next = l->next;
					free(l);
					return H;
				}
				temp = l;
				l = l->next;
			}while(l != NULL);
		}
	}
	printf("没有找到要删除的元素");
	return H;
}

int main()
{
	HashTable H;
	int i;
	int h[10] = { 1,0,4,16,25,64,81,9,49,36 };
	H = init(10);

	for (int i = 0; i < 10; i++)
	{
		H = create(h[i], H);
	}

	print(H);
	find(16, H);
	_delete(64, H);
	print(H);
	insert(23, H);
	print(H);
	free(H);

	return 0;

}
