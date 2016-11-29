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

//힙 코드
//힙의 노드(vertex)
typedef struct vertex {
	int index;
	int value;
} vertex;
//힙 배열
typedef struct HeapType {
	vertex *heap;
	int heap_size;
} HeapType;
//힙 초기화 함수
void init(HeapType *h)
{
	h->heap_size =0;
}
//자리 바꾸기(heapify에서 사용)
void swap(vertex *n1, vertex *n2) {
    vertex temp = *n1 ;
    *n1 = *n2 ;
    *n2 = temp ;
}
//힙 삽입 함수
void insert_min_heap(HeapType *h, vertex item)
{
	int i=++(h->heap_size); //힙 인덱스를 가리키는 변수
	h->heap[i] = item;
	while (i > 1) { //인덱스가 루트노드에 갈 때까지 반복
		if (h->heap[i].value < h->heap[i/2].value) { //자식이 부모보다 크면
			swap(&h->heap[i], &h->heap[i/2]); //자식과 부모의 위치를 바꾼다.
			i /= 2;
		}
		else break;
	}
}

vertex delete_min_heap(HeapType *h){
	int parent, child;
	vertex item, temp;
	item = h->heap[1];
	temp = h->heap[(h->heap_size)--]; //맨 마지막 노드
	parent = 1;
	child = 2;
	while( child <= h->heap_size ) {// 현재 노드의 자식노드중 더 작은 자식노드를 찾는다.
		if( ( child < h->heap_size ) && (h->heap[child].value) > h->heap[child+1].value) //오른쪽 자식이 왼쪽자식보다 작으면
			child++; //오른쪽 자식을 가리킴
		if( temp.value <= h->heap[child].value ) break; //현재노드가 child가 작으면 종료
		// 한단계 아래로 이동
		h->heap[parent] = h->heap[child];
		parent = child;
		child *= 2;
	}
	h->heap[parent] = temp;
	return item;
}

void heapify(HeapType *h, int i){
	int smallest;

	if(LCHILD(i)<=h->heap_size && h->heap[LCHILD(i)].value < h->heap[i].value)//왼쪽 자식이 부모보다 작으면 왼쪽자식 저장
		smallest = LCHILD(i);
	else
		smallest = i; //아니면 부모 저장
	if(RCHILD(i)<=h->heap_size && h->heap[RCHILD(i)].value < h->heap[smallest].value) //오른쪽 자식이 부모보다 작으면 저장
		smallest = RCHILD(i);
	if(smallest != i){//제일 작은게 부모가 아닌경우 자리를 바꾸고 재귀호출
		 swap(&(h->heap[i]), &(h->heap[smallest])) ;
		 heapify(h, smallest) ;
	}
}

//dijkstra 코드
void shortest_spanning_key_value(int **graph, HeapType *node_heap, int start,int vertice_number)
{
	int i,j, *visit_check, heap_index, *previous, *weight, **result;
	vertex node, *present; //node는 초기화/업데이트 용 vertex, present는 현재 가리키고 있는 vertex
	vertex *adj; //이웃 노드를 저장하는 배열
	
	adj=(vertex *)calloc(vertice_number,sizeof(vertex));
	visit_check=(int *)malloc(sizeof(int)*vertice_number); //방문 체크 배열
	previous = (int *)malloc(sizeof(int)*vertice_number); //이전 노드를 저장하는 배열
	weight = (int *)calloc(vertice_number,sizeof(int)); //MST의 무게를 저장하는 배열
	previous[start-1]=0; //첫번째 노드는 이전노드가 없다.
	weight[start-1]=0; // 무게도 마찬가지

	result = (int **)calloc(vertice_number, sizeof(int *)); //결과 그래프 배열 동적할당
	for(i=0 ; i<vertice_number ; i++)
	{
		result[i] = (int *)calloc(vertice_number,sizeof(int));
	}
	for(i=0 ; i<vertice_number ; i++) //방문 안했으면 1
		visit_check[i]=1;
	printf("<다익스트라 알고리즘입니다>\n");

	//힙에 vertex를 집어넣는다.
	for(i=1 ; i<=vertice_number ; i++)
	{
		node.index = i;
		//vertex의 키값들을 무한으로
		node.value=INT_MAX;
		insert_min_heap(node_heap, node);
	}
	
	printf("\n");
	//시작노드 지정, value 0으로 지정, 이전노드는 null
	node_heap->heap[start].index = start;
	node_heap->heap[start].value = 0;

	present = &node_heap->heap[start];

	//build heap
	for(i=vertice_number/2 ; i>0 ; i--)
		heapify(node_heap, i); //값이 바뀌었으므로 heapfiy

	while(node_heap->heap_size!=0){ //힙size가 0이면 힙이 빈 것이므로 반복문 종료
		present=&delete_min_heap(node_heap);
		visit_check[present->index-1]=0; //방문 체크, 방문한 곳은 0으로 바꿔줌
		//인접노드 찾아서 저장
		for(i=0 ; i<vertice_number ; i++)
		{
			if(present->index!=i+1 && graph[present->index-1][i]!=0 && visit_check[i]==1)
			{//자기자신이 아니고, 0이 아니면서, 체크하지 않은 곳이 인접노드
				node.index = i+1;
				node.value = present->value+graph[present->index-1][i];
				adj[i]=node;
			}
		}
		//인접 노드와 거리 측정
		for(i=0 ; i<vertice_number ; i++)
		{
			for(j=0 ; j<node_heap->heap_size ; j++){ //힙에서 인덱스가 같은걸 찾아낸다.
				if(node_heap->heap[j+1].index == adj[i].index){
					heap_index=j+1; //인덱스를 변수에 저장
				}
			}
			if(adj[i].value!=0 && adj[i].value < node_heap->heap[heap_index].value)//인접 노드가 힙 값보다 작으면 실행
			{	
				for(j=0 ; j<vertice_number ; j++)
					result[i][j]=result[present->index-1][j];
				result[i][present->index-1]=graph[i][present->index-1];//결과 경로 체크
				node_heap->heap[heap_index].value=adj[i].value;//인접노드의 값을 힙의 값에 저장

				previous[i]=present->index; //이전노드 기록
				weight[i]=adj[i].value; //min weight 기록
			}
		}
		//값이 바뀌었으므로 힙을 유지 시켜줌
		//build heap
		for(i=node_heap->heap_size/2 ; i>0 ; i--){
			heapify(node_heap, i); //값이 바뀌었으므로 heapfiy
		}
		//인접노드 초기화
		for(i=0; i<vertice_number; i++)
		{
			node.index=0;
			node.value=0;
			adj[i]=node;
		}
	}

	//결과 출력
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

	//결과 그래프 출력
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
	//동적할당 반환
	free(visit_check);
	free(previous);
	free(weight);
	free(adj);
	for(i=0 ; i<vertice_number ; i++)
		free(result[i]);
	free(result);
}

//prim 코드
void min_spaning_key_value(int **graph, HeapType *node_heap, int start,int vertice_number)
{
	int i,j, *visit_check, heap_index, *previous, *weight, **result;
	vertex node, *present; //node는 초기화/업데이트 용 vertex, present는 현재 가리키고 있는 vertex
	vertex *adj; //이웃 노드를 저장하는 배열
	
	adj=(vertex *)calloc(vertice_number,sizeof(vertex));
	visit_check=(int *)malloc(sizeof(int)*vertice_number); //방문 체크 배열
	previous = (int *)malloc(sizeof(int)*vertice_number); //이전 노드를 저장하는 배열
	weight = (int *)malloc(sizeof(int)*vertice_number); //MST의 무게를 저장하는 배열
	previous[start-1]=0; //첫번째 노드는 이전노드가 없다.
	weight[start-1]=0; // 무게도 마찬가지

	result = (int **)calloc(vertice_number, sizeof(int *)); //결과 저장 배열
	for(i=0 ; i<vertice_number ; i++)
	{
		result[i] = (int *)calloc(vertice_number,sizeof(int));
	}
	for(i=0 ; i<vertice_number ; i++) //방문 안했으면 1
		visit_check[i]=1;

	printf("<프림 알고리즘입니다>\n");

	//힙에 vertex를 집어넣는다.
	for(i=1 ; i<=vertice_number ; i++)
	{
		node.index = i;
		//vertex의 키값들을 무한으로
		node.value=INT_MAX;
		insert_min_heap(node_heap, node);
	}
	
	printf("\n");
	//시작노드 지정, value 0으로 지정
	node_heap->heap[start].index = start;
	node_heap->heap[start].value = 0;
	
	present = &node_heap->heap[start];

	//build heap
	for(i=vertice_number/2 ; i>0 ; i--)
		heapify(node_heap, i); //값이 바뀌었으므로 heapfiy
	
	while(node_heap->heap_size!=0){ //힙size가 0이면 힙이 빈 것이므로 반복문 종료
		present=&delete_min_heap(node_heap);
		visit_check[present->index-1]=0;//방문 체크, 방문한 곳은 0으로 바꿔줌
		//인접노드 찾아서 저장
		for(i=0 ; i<vertice_number ; i++)
		{
			if(present->index!=i+1 && graph[present->index-1][i]!=0 && visit_check[i]==1)
			{//자기자신이 아니고, 0이 아니면서, 체크하지 않은 곳이 인접노드
				node.index = i+1;
				node.value = graph[present->index-1][i];
				adj[i]=node;
			}
		}
		//그래프 값이 작으면 값을 바꿈
		for(i=0 ; i<vertice_number ; i++)
		{
			for(j=0 ; j<node_heap->heap_size ; j++){ //힙에서 인덱스가 같은걸 찾아낸다.
				if(node_heap->heap[j+1].index == adj[i].index){
					heap_index=j+1;  //인덱스를 변수에 저장
				}
			}
			if(adj[i].value!=0 && adj[i].value < node_heap->heap[heap_index].value) //인접 노드가 힙 값보다 작으면 실행
			{	
				node_heap->heap[heap_index].value=adj[i].value; //값이 작으면 인접노드 값으로 바꾼다.

				previous[i]=present->index; //이전 노드 기록
				weight[i]=graph[i][present->index-1]; //min weight 기록
			}
		}
		//값이 바뀌었으므로 힙을 유지 시켜줌
		//build heap
		for(i=node_heap->heap_size/2 ; i>0 ; i--){
			heapify(node_heap, i); //값이 바뀌었으므로 heapfiy
		}

		//인접노드 초기화
		for(i=0; i<vertice_number; i++)
		{
			node.index=0;
			node.value=0;
			adj[i]=node;
		}

	}
	//결과 출력
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
	//결과 그래프 만들기
	for(i=0; i<vertice_number; i++){
		if(previous[i]!=0)//start지점은 previous[-1]지점에 도달하는것을 방지
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
	//동적할당 반환
	free(visit_check);
	free(previous);
	free(weight);
	free(adj);
	for(i=0 ; i<vertice_number ; i++)
		free(result[i]);
	free(result);
}

//트리 코드
void spanning_tree(int **graph, HeapType *node_heap, int vertice_number){
	int togle=0, start=1;
	
	while(1){
		printf("<원하는 알고리즘을 선택하세요>\n 1. 프림알고리즘 \n 2. 다익스트라 알고리즘\n 3. 나가기\n입력 : ");
		scanf("%d",&togle);
		printf("\n");

		switch(togle){
		case 1:
			printf("시작지점을 입력해주세요 :");
			scanf("%d",&start);
			printf("\n");
			if(start<1 || start>vertice_number){
				printf("잘못된 값을 입력하셨습니다.\n");
				break;
			}
			printf("------------------------------------------------------------------------\n");
			min_spaning_key_value(graph, node_heap, start,vertice_number);
			break;
		case 2:
			printf("시작지점을 입력해주세요 :");
			scanf("%d",&start);
			printf("\n");
			if(start<1 || start>vertice_number){
				printf("잘못된 값을 입력하셨습니다.\n");
				break;
			}
			printf("------------------------------------------------------------------------\n");
			shortest_spanning_key_value(graph, node_heap, start,vertice_number);
			break;
		case 3:
			printf("안녕~ byebye ^^/");
			return;
		default:
			printf("잘못 입력하셨습니다. 다시 입력해주세요\n");
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

	//파일에서 입출력 받는 부분입니다.
	FILE *f;
	f=fopen("indata3.dat", "r");

	fscanf(f, "%d",&vertice_number);
	
	//예외처리
	if(vertice_number<0)
		printf("잘못된 입력입니다.");
	else if(vertice_number>20000)
		printf("너무 그래프가 큽니다. 20000이하로 입력하세요.");

	heap_graph.heap = (vertex *)malloc(sizeof(vertex)*(vertice_number+1));

	node_heap=(int *)calloc(vertice_number,sizeof(int));
	graph = (int **)calloc(vertice_number,sizeof(int *));
	for(i=0 ; i<vertice_number ; i++)
		graph[i] = (int *)calloc(vertice_number,sizeof(int));

	for (i = 0; i < vertice_number; i++)
		for (j = 0; j < vertice_number; j++) 
			fscanf(f, "%d", &graph[i][j]);

	//입력그래프 출력
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

	//그래프 체크
	for(i=0; i<vertice_number; i++)
	{
		for(j=0; j<vertice_number; j++){
			nongraph_check_x+=graph[i][j];
			nongraph_check_y+=graph[j][i];
		}
		if(nongraph_check_x==0 || nongraph_check_y==0){
			printf("잘못된 그래프입니다. 다른 그래프를 넣어주세요..\n");
			return -1;
		}
	}

	//힙 초기화
	init(&heap_graph);
	//함수 호출
	spanning_tree(graph,&heap_graph, vertice_number);
	printf("\n");
	//동적할당 반환
	free(node_heap);
	for(i=0 ; i<vertice_number ; i++)
		free(graph[i]);
	free(graph);
	free(heap_graph.heap);
	fclose(f);

	return 0;
}