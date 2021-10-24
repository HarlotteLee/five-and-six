#include <stdio.h>
#include <stdlib.h>
#define INFINITY 32767


typedef int Item;				
typedef struct node
{
	Item item;
	struct node * left;		//	左子树
	struct node * nextsibling;	//	左兄弟
} Node;
typedef Node * SubTree;
typedef struct binqueue
{
	SubTree * forest;
	int size;
	int current;
} *BinQueue;

int InitializeBinQueue(BinQueue * const pbq, const int size);//初始化二项队列
int BinQueueIsEmpty(const BinQueue bq);//判断二项队列是否为空
BinQueue Merge(BinQueue bq1, BinQueue bq2);//合并两个二项队列
BinQueue Insert(BinQueue bq, const Item item);//向二项队列中插入元素
Item DeleteMin(BinQueue bq);//删除并返回一个二项队列的最小元素
void Release(const BinQueue bq);//释放一个二项队列所占用的空间

/*	局部函数声明	*/

static SubTree Combine_Trees(SubTree t1, SubTree t2);
static Node * Make_Node(const Item item);




int InitializeBinQueue(BinQueue * const pbq, const int size)//初始化二项队列
{
	int i;

	if (size <= 0)
		return 0;
	*pbq = (struct binqueue *) malloc(sizeof(struct binqueue));
	if (NULL == *pbq)
		return 0;
	(*pbq)->forest = (SubTree *)malloc(sizeof(SubTree) * size);
	if (NULL == (*pbq)->forest)
	{
		free(*pbq);
		return 0;
	}
	for (i = 0; i < size; i++)
		(*pbq)->forest[i] = NULL;
	(*pbq)->size = size;
	(*pbq)->current = 0;

	return 1;
}

int BinQueueIsEmpty(const BinQueue bq)//判断二项队列是否为空
{
	return 0 == bq->current;
}

BinQueue Merge(BinQueue bq1, BinQueue bq2)//合并两个二项队列
{
	BinQueue temp;
	SubTree t1, t2, carry = NULL;
	int i, j, ct, index = 0;

	if (bq1 == bq2)
		return bq1;
	if (bq1->current + bq2->current > bq1->size)
		return bq1;
	bq1->current += bq2->current;
	//	保障 bq1 -> current > bq2 -> current								
	if (bq1->current < bq2->current)
	{
		temp = bq1;
		bq1 = bq2;
		bq2 = temp;
	}
	//	这样就可以通过下面的 bq2 所有子树都已被合并过的条件检测有
	//	效地避免当 bq2 -> current 很小的时候将造成的太多次不必要的条件检测	
	for (i = 0, j = 1, ct = 0; j <= bq1->current; i++, j *= 2)
	{
		t1 = bq1->forest[i];
		//	以下判断是为了应对Insert的情况, 因为我在Insert时只分配了1棵子树的空间
		if (1 == index)
			t2 = NULL;
		else if (1 == bq2->size)
		{
			index = 1;
			t2 = bq2->forest[i];
		}
		else
			t2 = bq2->forest[i];
		switch (!!t1 + 2 * !!t2 + 4 * !!carry)
		{
			//	no trees
		case 0:	break;
			//	only bq1
		case 1:	if (ct == bq2->size)
		{
			Release(bq2);
			return bq1;
		}
				break;
				//	only bq2
		case 2:	bq1->forest[i] = t2;
			bq2->forest[i] = NULL;
			ct++;
			break;
			//	only carry
		case 4:	bq1->forest[i] = carry;
			carry = NULL;
			break;
			//	bq1 and bq2
		case 3:	carry = Combine_Trees(t1, t2);
			bq1->forest[i] = bq2->forest[i] = NULL;
			ct++;
			break;
			//	bq1 and carry
		case 5:	carry = Combine_Trees(t1, carry);
			bq1->forest[i] = NULL;
			break;
			//	bq2 and carry
		case 6:	carry = Combine_Trees(t2, carry);
			bq2->forest[i] = NULL;
			ct++;
			break;
			//	all there
		case 7:	bq1->forest[i] = carry;
			carry = Combine_Trees(t1, t2);
			bq2->forest[i] = NULL;
			break;
		}
	}

	return bq1;
}

BinQueue Insert(BinQueue bq, const Item item)//向二项队列中插入元素
{
	BinQueue temp;
	int size = 1;

	if (bq->size == bq->current)
		return bq;
	if (!InitializeBinQueue(&temp, size))
	{
		free(temp->forest);
		free(temp);
		return bq;
	}
	temp->forest[0] = Make_Node(item);
	if (NULL == temp->forest[0])	
	{
		free(temp->forest[0]);
		free(temp);
		return bq;
	}
	temp->current = 1;
	bq = Merge(bq, temp);

	return bq;
}

Item DeleteMin(BinQueue bq) // 删除并返回一个二项队列的最小元素
{
	BinQueue deleted_queue;
	SubTree deleted_tree, old_root;
	Item min;
	int i, j, size, min_index;

	if (BinQueueIsEmpty(bq))
		return -INFINITY;
	min = INFINITY;
	for (i = 0, size = bq->size; i < size; i++)
	{
		if (bq->forest[i] && bq->forest[i]->item < min)
		{
			min = bq->forest[i]->item;
			min_index = i;
		}
	}
	if (INFINITY == min)
		return INFINITY;
	deleted_tree = bq->forest[min_index];
	old_root = deleted_tree;
	deleted_tree = deleted_tree->left;
	free(old_root);
	InitializeBinQueue(&deleted_queue, (1 << min_index) - 1);
	for (j = min_index - 1; j >= 0; j--)
	{
		deleted_queue->forest[j] = deleted_tree;
		deleted_tree = deleted_tree->nextsibling;
		deleted_queue->forest[j]->nextsibling = NULL;
	}
	bq->forest[min_index] = NULL;
	bq->current -= deleted_queue->current + 1;

	bq = Merge(bq, deleted_queue);

	return min;
}

void Release(const BinQueue bq)//释放一个二项队列所占用的空间
{
	int i, size;

	for (i = 0, size = bq->size; i < size; i++)
	{
		if (bq->forest[i])
			free(bq->forest[i]);
	}
	free(bq);
}

static SubTree Combine_Trees(SubTree t1, SubTree t2)
{
	if (t1->item > t2->item)
		return Combine_Trees(t2, t1);	
	t2->nextsibling = t1->left;
	t1->left = t2;

	return t1;
}

static Node * Make_Node(const Item item)
{
	Node * new_node;

	new_node = (Node *)malloc(sizeof(Node));
	if (NULL == new_node)
		return NULL;
	new_node->item = item;
	new_node->left = new_node->nextsibling = NULL;

	return new_node;
}
