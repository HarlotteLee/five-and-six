//开放定址法
#include<stdio.h>

typedef struct element Element;

struct element
{
	int key;
	int data;
};

typedef struct table Table;

struct table
{
	Element **elm;
	int count;//记录表中的数据个数
	int size;//哈希表的大小50
};

int hash(int key)
{
	return key % 13;
}

Table *init()
{
	Table *t = (Table*)malloc(sizeof(Table));
	t->size = 50;
	t->count = 0;
	t->elm = (Element**)malloc(sizeof(Element*));

	for (int i = 0; i < t->size; i++)
	{
		t->elm[i] = NULL;//将数组的每个域赋空
	}
	return t;
}

void insert(Table *t, Element *k)//将key值插入到哈希表中
{
	int position;
	position = hash(k->key);

	for (int i = 1; t->elm[position] = NULL && i < t->size; i++)
	{
		position = (hash(position) + i) % t->size;
	}
	t->elm[position] = k;
	t->count += 1;
}

int serch(Table *t, Element *k)//查找value并返回其所在的地址
{
	int position = hash(k->key);
	while (t->elm[position]->key != k->key)
	{
		position = hash(position + 1);
		if (t->elm[position] == NULL || position == hash(k->key))
			return -1;
	}
	return position;
}

void print(Table *t)
{
	for (int i = 0; i < 13; i++)
	{
		if (t->elm[i])
			printf("[%d %d]", t->elm[i]->key, t->elm[i]->data);
		else
			printf("NULL");
	}
	printf("\n");
}

int mian()
{
	Table *t = init();

	Element a[] = { { 12,19 }, { 14,98 }, { 26,87 }, { 14,77 }, { 15,100 }, { 10,59 } };
	int length = sizeof(a) / sizeof(Element);

	for (int i = 0; i < length; i++)
	{
		insert(t, &a[i]);
	}
	print(t);
	printf("a[3] is locat %d\n", serch(t, &a[3]));
	free(t);

	return 0;

}
