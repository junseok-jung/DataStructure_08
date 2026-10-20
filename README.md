최소 비용 신장 트리(MST) 알고리즘 비교 및 분석

1. 실험 측정 결과

Case 1 : Sparse Graph - Adjacency List - Kruskal
Vertices      : 50
Edges         : 100
MST Edges     : 49
MST Cost      : 1142
Memory        : 5596 Bytes
Execution Time: 0.0180 ms

Case 2 : Dense Graph - Adjacency List - Kruskal
Vertices      : 50
Edges         : 600
MST Edges     : 49
MST Cost      : 285
Memory        : 27596 Bytes
Execution Time: 0.3420 ms

Case 3 : Sparse Graph - Adjacency Matrix - Prim
Vertices      : 50
Edges         : 100
MST Edges     : 49
MST Cost      : 1142
Memory        : 11196 Bytes
Execution Time: 0.0150 ms

Case 4 : Dense Graph - Adjacency Matrix - Prim
Vertices      : 50
Edges         : 600
MST Edges     : 49
MST Cost      : 285
Memory        : 11196 Bytes
Execution Time: 0.0160 ms

2. MST 전체 비용과 간선 수 확인

간선 수: 정점 50개인 연결 그래프이므로 4가지 경우 모두 MST 간선 수는 49개(정점 수 - 1)로 동일합니다.

전체 비용: 동일한 희소 그래프를 사용한 Case 1과 Case 3의 MST 비용(1142)이 일치하고, 동일한 밀집 그래프를 사용한 Case 2와 Case 4의 MST 비용(285)이 일치합니다. 밀집 그래프는 선택 가능한 저비용 간선 후보가 많아 희소 그래프보다 총비용이 작게 나타납니다.

3. 인접 리스트와 인접 행렬의 메모리 사용 차이

인접 리스트 + Kruskal (Case 1, 2): 실제 간선 수만큼 동적 노드와 간선 배열을 생성하므로 간선 수에 비례해 메모리가 증가합니다. 희소 그래프에서는 5,596 Bytes로 메모리를 적게 쓰지만, 밀집 그래프에서는 노드의 포인터 오버헤드가 누적되어 27,596 Bytes로 급증합니다.

인접 행렬 + Prim (Case 3, 4): 간선 수와 무관하게 50x50 크기의 고정 배열을 사용하므로 희소 그래프와 밀집 그래프 모두 11,196 Bytes로 메모리 사용량이 동일합니다.

4. Kruskal과 Prim 알고리즘의 실행 특성 및 시간 차이

Kruskal 알고리즘 (간선 중심): 모든 간선을 가중치 순으로 정렬한 뒤 Union-Find로 사이클을 검사합니다. 간선 정렬이 실행시간의 대부분을 차지하므로, 간선이 100개(Case 1)에서 600개(Case 2)로 늘어나면 정렬 연산량이 급증해 실행시간이 크게 증가합니다(0.0180 ms -> 0.3420 ms).

Prim 알고리즘 (정점 중심): 시작 정점에서 출발해 매 단계 정점 50개를 순회하며 최소 비용 정점을 선택하고 갱신합니다. 간선 수와 무관하게 정점 수의 제곱에 비례하는 일정한 횟수만 연산하므로, 희소 그래프(0.0150 ms)와 밀집 그래프(0.0160 ms)의 실행시간이 거의 같습니다.

5. 결론: 그래프 밀도가 성능에 미치는 영향

희소 그래프(간선 100개): 불필요한 빈 공간을 저장하지 않는 인접 리스트가 메모리를 절반으로 절약하고, 정렬할 간선 수가 적어 '인접 리스트 + Kruskal 알고리즘'이 효율적입니다.

밀집 그래프(간선 600개): 인접 리스트의 노드 메모리 증가와 Kruskal의 정렬 부하가 커지므로, 고정된 메모리와 일정한 실행 속도를 유지하는 '인접 행렬 + Prim 알고리즘'이 메모리와 실행시간 모두에서 훨씬 유리합니다.
