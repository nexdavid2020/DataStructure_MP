#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// 자료구조 세 번째(마지막) 과제
// 20200110 정태현
// 크루스칼 알고리즘을 min heap과 disjoint set을 이용해서
// 시간 복잡도가 O(e log n) 이 되도록 구현하기!!
// 그리고 이 신장트리가 제대로 다 연결되어있는지 꼭 체크하기!

// Min Heap
#define MAX_ELEMENTS 50000000
#define HEAP_FULL(n)  (n == MAX_ELEMENTS-1)
#define HEAP_EMPTY(n)  (!n)

typedef struct {
    int src, dest, weight;
} Edge;

Edge heap[MAX_ELEMENTS]; // Min Heap 구현하는데 쓰는 배열! 힙에 담길 애들은 간선이니 max가 50000000이어야
int n = 0; // Min heap에 얼마나 요소가 차 있는지 표현하는 변수

void insert_min_heap(Edge item, int* n);
Edge delete_min_heap(int* n);


// Disjoint Set
int simpleFind(int parent[], int i);
void simpleUnion(int parent[], int i, int j);


// 마지막에 연결되었는지 BFS를 사용할 것이기 때문에
// 그래프 2차원 배열로 필요함!
#define MAX_VERTICES 10000  // 2차원 인접 배열로 만드는 것은 그래프이니 최대 10000*10000
int graph[MAX_VERTICES][MAX_VERTICES]; // 인접 행렬을 사용한 그래프 표현
int visited[MAX_VERTICES];  // 방문한 노드를 표시
int graph_node; // 그래프의 노드 수

void bfs(int start, int graph_node);
int is_connected(int graph_node);


int main(int argc, char *argv[]) {

    // 프로그램 시작 시간!
    clock_t start = clock();
    
    if (argc != 2) {
        printf("usage: ./fp1 input_filename\n");
        return 1;
    }

    // Open the input file
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("The input file does not exist.\n");
        return 1;
    }


    // 파일에서 노드(V)와 간선(E) 개수 읽기
    int V, E;
    fscanf(file, "%d\n%d", &V, &E);
    
    graph_node = V; // 그래프의 노드 수 입력받은 것을 보내주면서 초기화!
    for(int i=0;i < graph_node; i++){
        for(int j=0; j < graph_node; j++){
            graph[i][j] = 0; // 0으로 초기화
        }
    }

    // Disjoint Set 생성
    int* parent = (int*)malloc(sizeof(int) * V);
    for(int i=0; i<V; i++){
        parent[i] = -1; // 처음에 부모 다 -1로 세팅
    }

    // Read edges
    Edge item;
    for (int i = 0; i < E; i++) {
        fscanf(file, "%d %d %d", &item.src, &item.dest, &item.weight);
        insert_min_heap(item, &n); // 방금 입력받은 간선 복사해서 min heap에 넣기!
    }
    fclose(file);


    // 크루스칼 알고리즘을 적용하기!
    int totalWeight = 0;
    int connectedComponents = 0; 
    FILE *outputFile = fopen("fp1_result.txt", "w"); // 결과 담을 file 열기!
    
    for (int i = 0; i < E; i++) {
        Edge e = delete_min_heap(&n); // min heap에서 가장 작은 weight을 가진 간선 하나 뽑기
        int u = simpleFind(parent, e.src); // 그 edge의 source와
        int v = simpleFind(parent, e.dest); // destination 비교

        if (u != v) { // 기존에 없었다는 것이니 
            simpleUnion(parent, u, v);
            fprintf(outputFile, "%d %d %d\n", e.src, e.dest, e.weight); // 이제 output파일에도 저장
            totalWeight += e.weight; // 총 weight 증가
            connectedComponents++; // connectedComponents 하나 줄이기!(매우 중요)
            graph[u][v] = 1;
            graph[v][u] = 1;
        }

        if(connectedComponents == V-1){ // 신장트리에 연결된 간선이 노드 - 1개 이면 break;
            break;
        }
    }

    // 총 weight과 연결 상태를 출력하기~ 
    // connectedComponents가 1이라는 것은 연결 되었다는 것이고, 1이 아니라면 연결 안 되었다는 것!
    fprintf(outputFile, "%d\n", totalWeight);

    if(is_connected(graph_node) == 1){
        fprintf(outputFile, "CONNECTED\n");
    }else{
        fprintf(outputFile, "DISCONNECTED\n");
    }

    fclose(outputFile);


    // disjoint set 해제~
    free(parent);

    // 프로그램 실행 시간 측정하고 출력하기!
    clock_t end = clock();
    double cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("output written to fp1_result.txt.\n");
    printf("running time: %.6f seconds\n", cpu_time_used); // 예시에도 소수점 6자리까지여서 동일하게

    return 0;
}

// min heap function
void insert_min_heap(Edge item, int* n){
    int i;
    if(HEAP_FULL(*n)){
        fprintf(stderr, "The heap is full\n");
        exit(1);
    }

    i = ++(*n);
    while((i != 1) && (item.weight < heap[i/2].weight)){ // weight 기준으로 자리 찾기
        heap[i] = heap[i/2];
        i /= 2;
    }
    heap[i] = item;
}

Edge delete_min_heap(int* n){
    int parent, child;
    Edge item, temp;
    if(HEAP_EMPTY(*n)){
        fprintf(stderr, "The heap is empty\n");
        exit(1);
    }

    item = heap[1];
    temp = heap[(*n)--];
    parent = 1;
    child = 2;
    while(child <= *n){
        if((child < *n) && (heap[child].weight > heap[child + 1].weight)) child++;
        if(temp.weight <= heap[child].weight) break;
        heap[parent] = heap[child];
        parent = child;
        child *= 2;
    }
    heap[parent] = temp;
    return item;
}

// disjoint set funtion
int simpleFind(int parent[], int i){
    for(; parent[i] >= 0; i = parent[i]){
    }

    return i;
}

void simpleUnion(int parent[], int i, int j){
    parent[i] = j;
}


// BFS function

void bfs(int start, int graph_node) {
    int queue[graph_node], front = 0, rear = 0;
    queue[rear++] = start;
    visited[start] = 1;
    
    while (front < rear) {
        int node = queue[front++];
        for (int i = 0; i < graph_node; ++i) {
            if (graph[node][i] && !visited[i]) {
                queue[rear++] = i;
                visited[i] = 1;
            }
        }
    }
}

int is_connected(int graph_node) {
    for (int i = 0; i < graph_node; ++i) {
        visited[i] = 0;
    }
    bfs(0, graph_node);
    for (int i = 0; i < graph_node; ++i) {
        if (!visited[i]) {
            return 0;  // 연결되지 않은 노드 발견
        }
    }
    return 1;  // 모든 노드가 연결됨
}
