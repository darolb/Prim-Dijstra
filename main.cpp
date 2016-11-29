#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <time.h>

#define TRUE 1
#define FALSE 0
#define INT_MAX 2147483647
#define LCHILD(x) 2 * x
#define RCHILD(x) 2 * x + 1

//�� �ڵ�
//���� ���(vertex)
typedef struct vertex {
	int index;
	int value;
} vertex;
//�� �迭
typedef struct HeapType {
	vertex *heap;
	int heap_size;
} HeapType;
//�� �ʱ�ȭ �Լ�
void init(HeapType *h)
{
	h->heap_size =0;
}
//�ڸ� �ٲٱ�(heapify���� ���)
void swap(vertex *n1, vertex *n2) {
    vertex temp = *n1 ;
    *n1 = *n2 ;
    *n2 = temp ;
}
//�� ���� �Լ�
void insert_min_heap(HeapType *h, vertex item)
{
	int i=++(h->heap_size); //�� �ε����� ����Ű�� ����
	h->heap[i] = item;
	while (i > 1) { //�ε����� ��Ʈ��忡 �� ������ �ݺ�
		if (h->heap[i].value < h->heap[i/2].value) { //�ڽ��� �θ𺸴� ũ��
			swap(&h->heap[i], &h->heap[i/2]); //�ڽİ� �θ��� ��ġ�� �ٲ۴�.
			i /= 2;
		}
		else break;
	}
}

vertex delete_min_heap(HeapType *h){
	int parent, child;
	vertex item, temp;
	item = h->heap[1];
	temp = h->heap[(h->heap_size)--]; //�� ������ ���
	parent = 1;
	child = 2;
	while( child <= h->heap_size ) {// ���� ����� �ڽĳ���� �� ���� �ڽĳ�带 ã�´�.
		if( ( child < h->heap_size ) && (h->heap[child].value) > h->heap[child+1].value) //������ �ڽ��� �����ڽĺ��� ������
			child++; //������ �ڽ��� ����Ŵ
		if( temp.value <= h->heap[child].value ) break; //�����尡 child�� ������ ����
		// �Ѵܰ� �Ʒ��� �̵�
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

void heapify(HeapType *h, int i){
	int smallest;

	if(LCHILD(i)<=h->heap_size && h->heap[LCHILD(i)].value < h->heap[i].value)//���� �ڽ��� �θ𺸴� ������ �����ڽ� ����
		smallest = LCHILD(i);
	else
		smallest = i; //�ƴϸ� �θ� ����
	if(RCHILD(i)<=h->heap_size && h->heap[RCHILD(i)].value < h->heap[smallest].value) //������ �ڽ��� �θ𺸴� ������ ����
		smallest = RCHILD(i);
	if(smallest != i){//���� ������ �θ� �ƴѰ�� �ڸ��� �ٲٰ� ���ȣ��
		 swap(&(h->heap[i]), &(h->heap[smallest])) ;
		 heapify(h, smallest) ;
	}
}

//dijkstra �ڵ�
void shortest_spanning_key_value(int **graph, HeapType *node_heap, int start,int vertice_number)
{
	int i,j, *visit_check, heap_index, *previous, *weight, **result;
	vertex node, *present; //node�� �ʱ�ȭ/������Ʈ �� vertex, present�� ���� ����Ű�� �ִ� vertex
	vertex *adj; //�̿� ��带 �����ϴ� �迭
	
	adj=(vertex *)calloc(vertice_number,sizeof(vertex));
	visit_check=(int *)malloc(sizeof(int)*vertice_number); //�湮 üũ �迭
	previous = (int *)malloc(sizeof(int)*vertice_number); //���� ��带 �����ϴ� �迭
	weight = (int *)calloc(vertice_number,sizeof(int)); //MST�� ���Ը� �����ϴ� �迭
	previous[start-1]=0; //ù��° ���� ������尡 ����.
	weight[start-1]=0; // ���Ե� ��������

	result = (int **)calloc(vertice_number, sizeof(int *)); //��� �׷��� �迭 �����Ҵ�
	for(i=0 ; i<vertice_number ; i++)
	{
		result[i] = (int *)calloc(vertice_number,sizeof(int));
	}
	for(i=0 ; i<vertice_number ; i++) //�湮 �������� 1
		visit_check[i]=1;
	printf("<���ͽ�Ʈ�� �˰����Դϴ�>\n");

	//���� vertex�� ����ִ´�.
	for(i=1 ; i<=vertice_number ; i++)
	{
		node.index = i;
		//vertex�� Ű������ ��������
		node.value=INT_MAX;
		insert_min_heap(node_heap, node);
	}
	
	printf("\n");
	//���۳�� ����, value 0���� ����, �������� null
	node_heap->heap[start].index = start;
	node_heap->heap[start].value = 0;

	present = &node_heap->heap[start];

	//build heap
	for(i=vertice_number/2 ; i>0 ; i--)
		heapify(node_heap, i); //���� �ٲ�����Ƿ� heapfiy

	while(node_heap->heap_size!=0){ //��size�� 0�̸� ���� �� ���̹Ƿ� �ݺ��� ����
		present=&delete_min_heap(node_heap);
		visit_check[present->index-1]=0; //�湮 üũ, �湮�� ���� 0���� �ٲ���
		//������� ã�Ƽ� ����
		for(i=0 ; i<vertice_number ; i++)
		{
			if(present->index!=i+1 && graph[present->index-1][i]!=0 && visit_check[i]==1)
			{//�ڱ��ڽ��� �ƴϰ�, 0�� �ƴϸ鼭, üũ���� ���� ���� �������
				node.index = i+1;
				node.value = present->value+graph[present->index-1][i];
				adj[i]=node;
			}
		}
		//���� ���� �Ÿ� ����
		for(i=0 ; i<vertice_number ; i++)
		{
			for(j=0 ; j<node_heap->heap_size ; j++){ //������ �ε����� ������ ã�Ƴ���.
				if(node_heap->heap[j+1].index == adj[i].index){
					heap_index=j+1; //�ε����� ������ ����
				}
			}
			if(adj[i].value!=0 && adj[i].value < node_heap->heap[heap_index].value)//���� ��尡 �� ������ ������ ����
			{	
				for(j=0 ; j<vertice_number ; j++)
					result[i][j]=result[present->index-1][j];
				result[i][present->index-1]=graph[i][present->index-1];//��� ��� üũ
				node_heap->heap[heap_index].value=adj[i].value;//��������� ���� ���� ���� ����

				previous[i]=present->index; //������� ���
				weight[i]=adj[i].value; //min weight ���
			}
		}
		//���� �ٲ�����Ƿ� ���� ���� ������
		//build heap
		for(i=node_heap->heap_size/2 ; i>0 ; i--){
			heapify(node_heap, i); //���� �ٲ�����Ƿ� heapfiy
		}
		//������� �ʱ�ȭ
		for(i=0; i<vertice_number; i++)
		{
			node.index=0;
			node.value=0;
			adj[i]=node;
		}
	}

	//��� ���
	printf("<previous node> \n");
	printf("------------------------------------------------------------------------\n");
	for(i=0 ; i<vertice_number ; i++)
	{
		printf("	%d",i+1);
	}
	printf("\n");
	for(i=0 ; i<vertice_number ; i++)
	{
		printf("	%d",previous[i]);
	}
	printf("\n");
	printf("------------------------------------------------------------------------\n");
	printf("<node weight> \n");
	for(i=0 ; i<vertice_number ; i++)
	{
		printf("	%d",i+1);
	}
	printf("\n");
	for(i=0 ; i<vertice_number ; i++)
	{
		printf("	%d",weight[i]);
	}
	printf("\n");
	printf("------------------------------------------------------------------------\n");
	printf("<result set>\n");

	//��� �׷��� ���
	for(i=0 ; i<vertice_number ; i++)
		printf("	%d",i+1);
	for(i=0 ; i<vertice_number ; i++){
		printf("\n%d",i+1);
		for(j=0 ; j<vertice_number ; j++){
			printf("	%d",result[i][j]);
		}
	}
	printf("\n");
	printf("------------------------------------------------------------------------\n");
	//�����Ҵ� ��ȯ
	free(visit_check);
	free(previous);
	free(weight);
	free(adj);
	for(i=0 ; i<vertice_number ; i++)
		free(result[i]);
	free(result);
}

//prim �ڵ�
void min_spaning_key_value(int **graph, HeapType *node_heap, int start,int vertice_number)
{
	int i,j, *visit_check, heap_index, *previous, *weight, **result;
	vertex node, *present; //node�� �ʱ�ȭ/������Ʈ �� vertex, present�� ���� ����Ű�� �ִ� vertex
	vertex *adj; //�̿� ��带 �����ϴ� �迭
	
	adj=(vertex *)calloc(vertice_number,sizeof(vertex));
	visit_check=(int *)malloc(sizeof(int)*vertice_number); //�湮 üũ �迭
	previous = (int *)malloc(sizeof(int)*vertice_number); //���� ��带 �����ϴ� �迭
	weight = (int *)malloc(sizeof(int)*vertice_number); //MST�� ���Ը� �����ϴ� �迭
	previous[start-1]=0; //ù��° ���� ������尡 ����.
	weight[start-1]=0; // ���Ե� ��������

	result = (int **)calloc(vertice_number, sizeof(int *)); //��� ���� �迭
	for(i=0 ; i<vertice_number ; i++)
	{
		result[i] = (int *)calloc(vertice_number,sizeof(int));
	}
	for(i=0 ; i<vertice_number ; i++) //�湮 �������� 1
		visit_check[i]=1;

	printf("<���� �˰����Դϴ�>\n");

	//���� vertex�� ����ִ´�.
	for(i=1 ; i<=vertice_number ; i++)
	{
		node.index = i;
		//vertex�� Ű������ ��������
		node.value=INT_MAX;
		insert_min_heap(node_heap, node);
	}
	
	printf("\n");
	//���۳�� ����, value 0���� ����
	node_heap->heap[start].index = start;
	node_heap->heap[start].value = 0;
	
	present = &node_heap->heap[start];

	//build heap
	for(i=vertice_number/2 ; i>0 ; i--)
		heapify(node_heap, i); //���� �ٲ�����Ƿ� heapfiy
	
	while(node_heap->heap_size!=0){ //��size�� 0�̸� ���� �� ���̹Ƿ� �ݺ��� ����
		present=&delete_min_heap(node_heap);
		visit_check[present->index-1]=0;//�湮 üũ, �湮�� ���� 0���� �ٲ���
		//������� ã�Ƽ� ����
		for(i=0 ; i<vertice_number ; i++)
		{
			if(present->index!=i+1 && graph[present->index-1][i]!=0 && visit_check[i]==1)
			{//�ڱ��ڽ��� �ƴϰ�, 0�� �ƴϸ鼭, üũ���� ���� ���� �������
				node.index = i+1;
				node.value = graph[present->index-1][i];
				adj[i]=node;
			}
		}
		//�׷��� ���� ������ ���� �ٲ�
		for(i=0 ; i<vertice_number ; i++)
		{
			for(j=0 ; j<node_heap->heap_size ; j++){ //������ �ε����� ������ ã�Ƴ���.
				if(node_heap->heap[j+1].index == adj[i].index){
					heap_index=j+1;  //�ε����� ������ ����
				}
			}
			if(adj[i].value!=0 && adj[i].value < node_heap->heap[heap_index].value) //���� ��尡 �� ������ ������ ����
			{	
				node_heap->heap[heap_index].value=adj[i].value; //���� ������ ������� ������ �ٲ۴�.

				previous[i]=present->index; //���� ��� ���
				weight[i]=graph[i][present->index-1]; //min weight ���
			}
		}
		//���� �ٲ�����Ƿ� ���� ���� ������
		//build heap
		for(i=node_heap->heap_size/2 ; i>0 ; i--){
			heapify(node_heap, i); //���� �ٲ�����Ƿ� heapfiy
		}

		//������� �ʱ�ȭ
		for(i=0; i<vertice_number; i++)
		{
			node.index=0;
			node.value=0;
			adj[i]=node;
		}

	}
	//��� ���
	printf("<previous node> \n");
	printf("------------------------------------------------------------------------\n");
	for(i=0 ; i<vertice_number ; i++)
	{
		printf("	%d",i+1);
	}
	printf("\n");
	for(i=0 ; i<vertice_number ; i++)
	{
		printf("	%d",previous[i]);
	}
	printf("\n");
	printf("------------------------------------------------------------------------\n");
	printf("<node weight> \n");
	for(i=0 ; i<vertice_number ; i++)
	{
		printf("	%d",i+1);
	}
	printf("\n");
	for(i=0 ; i<vertice_number ; i++)
	{
		printf("	%d",weight[i]);
	}
	printf("\n");
	printf("------------------------------------------------------------------------\n");
	printf("<result set>\n");
	//��� �׷��� �����
	for(i=0; i<vertice_number; i++){
		if(previous[i]!=0)//start������ previous[-1]������ �����ϴ°��� ����
			result[i][previous[i]-1]=weight[i];
	}

	for(i=0 ; i<vertice_number ; i++)
		printf("	%d",i+1);
	for(i=0 ; i<vertice_number ; i++){
		printf("\n%d",i+1);
		for(j=0 ; j<vertice_number ; j++){
			printf("	%d",result[i][j]);
		}
	}
	printf("\n");
	printf("------------------------------------------------------------------------\n");
	//�����Ҵ� ��ȯ
	free(visit_check);
	free(previous);
	free(weight);
	free(adj);
	for(i=0 ; i<vertice_number ; i++)
		free(result[i]);
	free(result);
}

//Ʈ�� �ڵ�
void spanning_tree(int **graph, HeapType *node_heap, int vertice_number){
	int togle=0, start=1;
	
	while(1){
		printf("<���ϴ� �˰����� �����ϼ���>\n 1. �����˰��� \n 2. ���ͽ�Ʈ�� �˰���\n 3. ������\n�Է� : ");
		scanf("%d",&togle);
		printf("\n");

		switch(togle){
		case 1:
			printf("���������� �Է����ּ��� :");
			scanf("%d",&start);
			printf("\n");
			if(start<1 || start>vertice_number){
				printf("�߸��� ���� �Է��ϼ̽��ϴ�.\n");
				break;
			}
			printf("------------------------------------------------------------------------\n");
			min_spaning_key_value(graph, node_heap, start,vertice_number);
			break;
		case 2:
			printf("���������� �Է����ּ��� :");
			scanf("%d",&start);
			printf("\n");
			if(start<1 || start>vertice_number){
				printf("�߸��� ���� �Է��ϼ̽��ϴ�.\n");
				break;
			}
			printf("------------------------------------------------------------------------\n");
			shortest_spanning_key_value(graph, node_heap, start,vertice_number);
			break;
		case 3:
			printf("�ȳ�~ byebye ^^/");
			return;
		default:
			printf("�߸� �Է��ϼ̽��ϴ�. �ٽ� �Է����ּ���\n");
			printf("------------------------------------------------------------------------\n");
			break;
		}
	}
}

int main()
{
	int i, j, **graph, *node_heap, edge_num=0, nongraph_check_x=0, nongraph_check_y=0;
	int vertice_number=0;
	HeapType heap_graph;

	//���Ͽ��� ����� �޴� �κ��Դϴ�.
	FILE *f;
	f=fopen("indata3.dat", "r");

	fscanf(f, "%d",&vertice_number);
	
	//����ó��
	if(vertice_number<0)
		printf("�߸��� �Է��Դϴ�.");
	else if(vertice_number>20000)
		printf("�ʹ� �׷����� Ů�ϴ�. 20000���Ϸ� �Է��ϼ���.");

	heap_graph.heap = (vertex *)malloc(sizeof(vertex)*(vertice_number+1));

	node_heap=(int *)calloc(vertice_number,sizeof(int));
	graph = (int **)calloc(vertice_number,sizeof(int *));
	for(i=0 ; i<vertice_number ; i++)
		graph[i] = (int *)calloc(vertice_number,sizeof(int));

	for (i = 0; i < vertice_number; i++)
		for (j = 0; j < vertice_number; j++) 
			fscanf(f, "%d", &graph[i][j]);

	//�Է±׷��� ���
	for(i=0 ; i<vertice_number ; i++)
		printf("	%d",i+1);
	for(i=0 ; i<vertice_number ; i++){
		printf("\n%d",i+1);
		for(j=0 ; j<vertice_number ; j++){
			printf("	%d",graph[i][j]);
		}
	}
	printf("\n");
	printf("------------------------------------------------------------------------\n");

	//�׷��� üũ
	for(i=0; i<vertice_number; i++)
	{
		for(j=0; j<vertice_number; j++){
			nongraph_check_x+=graph[i][j];
			nongraph_check_y+=graph[j][i];
		}
		if(nongraph_check_x==0 || nongraph_check_y==0){
			printf("�߸��� �׷����Դϴ�. �ٸ� �׷����� �־��ּ���..\n");
			return -1;
		}
	}

	//�� �ʱ�ȭ
	init(&heap_graph);
	//�Լ� ȣ��
	spanning_tree(graph,&heap_graph, vertice_number);
	printf("\n");
	//�����Ҵ� ��ȯ
	free(node_heap);
	for(i=0 ; i<vertice_number ; i++)
		free(graph[i]);
	free(graph);
	free(heap_graph.heap);
	fclose(f);

	return 0;
}