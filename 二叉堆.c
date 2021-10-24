#include<stdio.h>

int heap[10000];
int size = 0;

int getLeft(int parent)//获取左节点
{
	return 2 * parent + 1;
}

int getRight(int parent)//获取右节点
{
	return 2 * parent + 1;
}

int getparent(int son)//获取父节点
{
	if (son <= 0)
		return -1;
	return (son - 1) / 2;
}

void init(int n)//n是堆的大小，初始化堆元素
{
	int index = n;

	while (index--)//获取最后一个非叶子节点
		if (getLeft(index) < n)
			break;
	
	while(index >= 0)
	{
		int parent = index;
		int temp = heap[parent];

		while (1)
		{
			int left = getLeft(parent);
			int right = getRight(parent);

			if (left >= n)//没有左节点
				break;
			int minSon = right >= n ? left : heap[left] < heap[right] ? left : right;

			if (temp <= heap[minSon])
				parent = minSon;

		}
		heap[parent] = temp;
		index--;
	}
}

void creatHeap(int n) //创建堆
{
	for (int i = 0; i < n; i++)
		scanf(heap[i]);
	size = n;
	initHeap(size);
}
void insert(int value) //添加元素
{
	int son = size;
	heap[size++] = value; //先将插入的数据放在最后
	while (1)
	{
		int parent = getParent(son); //得到它的父节点
		if (parent < 0)              //son已经是根节点了，结束
			break;
		if (heap[parent] <= value) //父节点小于等于插入的值，结束
			break;
		heap[son] = heap[parent];
		//然后让父节点变成下一轮的子节点，向上继续比较
		son = parent;
	}
	heap[son] = value;
}
void pop() //删除元素
{
	if (size <= 0)
		return;
	heap[0] = heap[--size];
	int parent = 0, temp = heap[parent];
	while (1)
	{
		int left = getLeft(parent), right = getRight(parent);
		if (left >= size) //parent没有子节点了
			break;
		int minSon = right >= size ? left : heap[left] < heap[right] ? left
			: right;
		if (temp <= heap[minSon]) //父节点小于等于最小子节点
			break;
		heap[parent] = heap[minSon];
		parent = minSon;
	}
	heap[parent] = temp;
}
void printHeap() //打印堆
{
	for (int i = 0; i < size; i++)
		printf("%d", heap[i]);
	printf("\n");
}
void heapSort() //堆排序
{
	if (size <= 0)
		return;
	int n = size;
	while (n--)
	{
		swap(heap[0], heap[n]);
		initHeap(n);
	}
}
