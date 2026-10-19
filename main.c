#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_VERTICES 50
#define INF 999999

// 간선 구조체
typedef struct {
    int u;
    int v;
    int weight;
} Edge;

// 인접 리스트 노드
typedef struct Node {
    int vertex;
    int weight;
    struct Node* next;
} Node;

// 인접 리스트 그래프
typedef struct {
    int numVertices;
    int numEdges;
    Node* head[MAX_VERTICES];
} AdjListGraph;

// 인접 행렬 그래프
typedef struct {
    int numVertices;
    int numEdges;
    int matrix[MAX_VERTICES][MAX_VERTICES];
} AdjMatrixGraph;

// DFS 방문 탐색
void dfsCheck(int v, int tempMatrix[MAX_VERTICES][MAX_VERTICES], int visited[MAX_VERTICES]) {
    visited[v] = 1;
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (tempMatrix[v][i] > 0 && !visited[i]) {
            dfsCheck(i, tempMatrix, visited);
        }
    }
}

// 연결 그래프 여부 확인
int isConnected(int tempMatrix[MAX_VERTICES][MAX_VERTICES]) {
    int visited[MAX_VERTICES] = { 0 };
    dfsCheck(0, tempMatrix, visited);
    for (int i = 0; i < MAX_VERTICES; i++) {
        if (!visited[i]) return 0;
    }
    return 1;
}

// 랜덤 연결 그래프 생성
void generateRandomConnectedGraph(Edge edges[], int targetEdges) {
    int tempMatrix[MAX_VERTICES][MAX_VERTICES];

    while (1) {
        for (int i = 0; i < MAX_VERTICES; i++) {
            for (int j = 0; j < MAX_VERTICES; j++) {
                tempMatrix[i][j] = 0;
            }
        }

        int count = 0;
        while (count < targetEdges) {
            int u = rand() % MAX_VERTICES;
            int v = rand() % MAX_VERTICES;

            if (u == v || tempMatrix[u][v] != 0) continue; // 자기 루프 및 중복 간선 제외

            int weight = (rand() % 100) + 1;
            tempMatrix[u][v] = weight;
            tempMatrix[v][u] = weight;

            edges[count].u = (u < v) ? u : v;
            edges[count].v = (u < v) ? v : u;
            edges[count].weight = weight;
            count++;
        }

        if (isConnected(tempMatrix)) break;
    }


}

// 인접 리스트 생성
void buildAdjList(AdjListGraph* g, Edge edges[], int numEdges) {
    g->numVertices = MAX_VERTICES;
    g->numEdges = numEdges;
    for (int i = 0; i < MAX_VERTICES; i++) g->head[i] = NULL;

    for (int i = 0; i < numEdges; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;

        Node* n1 = (Node*)malloc(sizeof(Node));
        n1->vertex = v;
        n1->weight = w;
        n1->next = g->head[u];
        g->head[u] = n1;

        Node* n2 = (Node*)malloc(sizeof(Node));
        n2->vertex = u;
        n2->weight = w;
        n2->next = g->head[v];
        g->head[v] = n2;
    }


}

// 인접 리스트 메모리 해제
void freeAdjList(AdjListGraph* g) {
    for (int i = 0; i < g->numVertices; i++) {
        Node* curr = g->head[i];
        while (curr != NULL) {
            Node* temp = curr;
            curr = curr->next;
            free(temp);
        }
        g->head[i] = NULL;
    }
}

// 인접 행렬 생성
void buildAdjMatrix(AdjMatrixGraph* g, Edge edges[], int numEdges) {
    g->numVertices = MAX_VERTICES;
    g->numEdges = numEdges;

    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            g->matrix[i][j] = (i == j) ? 0 : INF;
        }
    }

    for (int i = 0; i < numEdges; i++) {
        int u = edges[i].u;
        int v = edges[i].v;
        int w = edges[i].weight;
        g->matrix[u][v] = w;
        g->matrix[v][u] = w;
    }


}

// Union-Find: 루트 탐색
int findRoot(int parent[], int i) {
    if (parent[i] == i) return i;
    return parent[i] = findRoot(parent, parent[i]); // 경로 압축
}

// Union-Find: 집합 병합
void unionSets(int parent[], int rootA, int rootB) {
    if (rootA != rootB) parent[rootB] = rootA;
}

// 간선 오름차순 정렬 (선택 정렬)
void sortEdges(Edge edgeList[], int numEdges) {
    for (int i = 0; i < numEdges - 1; i++) {
        int minIdx = i;
        for (int j = i + 1; j < numEdges; j++) {
            if (edgeList[j].weight < edgeList[minIdx].weight) {
                minIdx = j;
            }
        }
        if (minIdx != i) {
            Edge temp = edgeList[i];
            edgeList[i] = edgeList[minIdx];
            edgeList[minIdx] = temp;
        }
    }
}

// Kruskal 알고리즘 수행
int runKruskal(AdjListGraph* g, Edge mstEdges[], int* mstCount) {
    Edge* edgeList = (Edge*)malloc(sizeof(Edge) * g->numEdges);
    int idx = 0;

    // 인접 리스트에서 간선 추출
    for (int u = 0; u < g->numVertices; u++) {
        Node* curr = g->head[u];
        while (curr != NULL) {
            if (u < curr->vertex) {
                edgeList[idx].u = u;
                edgeList[idx].v = curr->vertex;
                edgeList[idx].weight = curr->weight;
                idx++;
            }
            curr = curr->next;
        }
    }

    sortEdges(edgeList, g->numEdges);

    int parent[MAX_VERTICES];
    for (int i = 0; i < MAX_VERTICES; i++) parent[i] = i;

    int totalCost = 0;
    *mstCount = 0;

    // 사이클 검사 및 간선 선택
    for (int i = 0; i < g->numEdges; i++) {
        int rootU = findRoot(parent, edgeList[i].u);
        int rootV = findRoot(parent, edgeList[i].v);

        if (rootU != rootV) {
            mstEdges[*mstCount] = edgeList[i];
            (*mstCount)++;
            totalCost += edgeList[i].weight;
            unionSets(parent, rootU, rootV);

            if (*mstCount == g->numVertices - 1) break;
        }
    }

    free(edgeList);
    return totalCost;


}

// Prim 알고리즘 수행
int runPrim(AdjMatrixGraph* g, Edge mstEdges[], int* mstCount) {
    int visited[MAX_VERTICES] = { 0 };
    int minWeight[MAX_VERTICES];
    int parent[MAX_VERTICES];

    for (int i = 0; i < MAX_VERTICES; i++) {
        minWeight[i] = INF;
        parent[i] = -1;
    }

    minWeight[0] = 0; // 0번 정점 시작
    int totalCost = 0;
    *mstCount = 0;

    for (int step = 0; step < g->numVertices; step++) {
        int u = -1;
        int minVal = INF;

        // 최소 비용 미방문 정점 선택
        for (int v = 0; v < g->numVertices; v++) {
            if (!visited[v] && minWeight[v] < minVal) {
                minVal = minWeight[v];
                u = v;
            }
        }

        if (u == -1) break;
        visited[u] = 1;
        totalCost += minWeight[u];

        if (parent[u] != -1) {
            mstEdges[*mstCount].u = parent[u];
            mstEdges[*mstCount].v = u;
            mstEdges[*mstCount].weight = minWeight[u];
            (*mstCount)++;
        }

        // 인접 정점 최소 비용 갱신
        for (int v = 0; v < g->numVertices; v++) {
            if (g->matrix[u][v] != INF && !visited[v] && g->matrix[u][v] < minWeight[v]) {
                minWeight[v] = g->matrix[u][v];
                parent[v] = u;
            }
        }
    }

    return totalCost;


}

// Kruskal 메모리 계산
size_t calcKruskalMemory(int numEdges) {
    return sizeof(AdjListGraph)
        + (size_t)(2 * numEdges) * sizeof(Node)
        + (size_t)numEdges * sizeof(Edge)
        + (size_t)MAX_VERTICES * sizeof(int)
        + (size_t)(MAX_VERTICES - 1) * sizeof(Edge);
}

// Prim 메모리 계산
size_t calcPrimMemory(void) {
    return sizeof(AdjMatrixGraph)
        + (size_t)(3 * MAX_VERTICES) * sizeof(int)
        + (size_t)(MAX_VERTICES - 1) * sizeof(Edge);
}

// 결과 출력
void printResult(int caseNum, const char* title, int numEdges,
    Edge mstEdges[], int mstCount, int mstCost,
    size_t memBytes, double execTimeMs) {
    printf("Case %d : %s\n", caseNum, title);
    printf("Vertices      : %d\n", MAX_VERTICES);
    printf("Edges         : %d\n", numEdges);
    printf("MST Edges     : %d\n", mstCount);
    printf("MST Cost      : %d\n", mstCost);
    printf("Memory        : %zu Bytes\n", memBytes);
    printf("Execution Time: %.4f ms\n", execTimeMs);
    printf("MST Included Edges:\n");
    for (int i = 0; i < mstCount; i++) {
        printf("(%2d-%2d:%3d)%s", mstEdges[i].u, mstEdges[i].v, mstEdges[i].weight,
            ((i + 1) % 7 == 0 || i == mstCount - 1) ? "\n" : " ");
    }
    printf("\n");
}

int main(void) {
    srand((unsigned int)time(NULL));

    Edge sparseEdges[100];
    Edge denseEdges[600];

    generateRandomConnectedGraph(sparseEdges, 100);
    generateRandomConnectedGraph(denseEdges, 600);

    AdjListGraph sparseList, denseList;
    AdjMatrixGraph sparseMatrix, denseMatrix;

    buildAdjList(&sparseList, sparseEdges, 100);
    buildAdjList(&denseList, denseEdges, 600);
    buildAdjMatrix(&sparseMatrix, sparseEdges, 100);
    buildAdjMatrix(&denseMatrix, denseEdges, 600);

    Edge mstEdges[MAX_VERTICES - 1];
    int mstCount = 0;
    clock_t start, end;

    // Case 1: 희소 + 인접 리스트 + Kruskal
    start = clock();
    int cost1 = runKruskal(&sparseList, mstEdges, &mstCount);
    end = clock();
    printResult(1, "Sparse Graph - Adjacency List - Kruskal", 100,
        mstEdges, mstCount, cost1, calcKruskalMemory(100),
        ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC);

    // Case 2: 밀집 + 인접 리스트 + Kruskal
    start = clock();
    int cost2 = runKruskal(&denseList, mstEdges, &mstCount);
    end = clock();
    printResult(2, "Dense Graph - Adjacency List - Kruskal", 600,
        mstEdges, mstCount, cost2, calcKruskalMemory(600),
        ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC);

    // Case 3: 희소 + 인접 행렬 + Prim
    start = clock();
    int cost3 = runPrim(&sparseMatrix, mstEdges, &mstCount);
    end = clock();
    printResult(3, "Sparse Graph - Adjacency Matrix - Prim", 100,
        mstEdges, mstCount, cost3, calcPrimMemory(),
        ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC);

    // Case 4: 밀집 + 인접 행렬 + Prim
    start = clock();
    int cost4 = runPrim(&denseMatrix, mstEdges, &mstCount);
    end = clock();
    printResult(4, "Dense Graph - Adjacency Matrix - Prim", 600,
        mstEdges, mstCount, cost4, calcPrimMemory(),
        ((double)(end - start) * 1000.0) / CLOCKS_PER_SEC);

    freeAdjList(&sparseList);
    freeAdjList(&denseList);

    return 0;


}