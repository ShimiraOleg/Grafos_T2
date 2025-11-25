/*
Grupo: Quinaiers
Alunos: André Cidade Irie, Guilherme Licori, Mateus de Oliveira Lopes
*/
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
using namespace std;

const long long INF = 1e18;

struct edge {
    int v, cap, flow, cost, rev;
};
vector<vector<edge>> graph;
vector<long long> dist;
vector<int> p_node;
vector<int> p_edge; 

bool bellman_ford(int s, int t, int V_size) {
    dist.assign(V_size + 1, INF);
    p_node.assign(V_size + 1, -1);
    p_edge.assign(V_size + 1, -1);    
    dist[s] = 0;

    for (int i = 1; i <= V_size - 1; i++) {
        bool relaxed = false;
        for (int u = 1; u <= V_size; u++) {
            for (int j = 0; j < graph[u].size(); j++) {
                edge &e = graph[u][j];                
                if (e.cap - e.flow > 0) {
                    if (dist[u] != INF && dist[e.v] > dist[u] + e.cost) {
                        dist[e.v] = dist[u] + e.cost;
                        p_node[e.v] = u;
                        p_edge[e.v] = j;
                        relaxed = true;
                    }
                }
            }
        }
        if (!relaxed) break;
    }
    return dist[t] != INF;
}

void addEdge(int u, int v, int cap, int cost) {
    edge a = {v, cap, 0, cost, (int)graph[v].size()};
    edge b = {u, 0, 0, -cost, (int)graph[u].size()};
    
    graph[u].push_back(a);
    graph[v].push_back(b);
}

int main() {    
    int V, E; 
    int t = 1;

    while (cin >> V >> E) {
        vector<tuple<int, int, int>> temp_edges;
        for (int i = 0; i < E; i++) {
            int u, v, c;
            cin >> u >> v >> c;
            temp_edges.emplace_back(u, v, c);
        }
        int D, K;
        cin >> D >> K;

        graph.assign(V + 1, vector<edge>());        
        for (auto [u, v, c] : temp_edges) {
            addEdge(u, v, K, c);
            addEdge(v, u, K, c);
        }
        long long total_cost = 0;
        int maxFlow = 0;         

        while (maxFlow < D && bellman_ford(1, V, V)) {
            int push = D - maxFlow;
            int curr = V;
            while (curr != 1) {
                int prev = p_node[curr];
                int idx = p_edge[curr];
                push = min(push, graph[prev][idx].cap - graph[prev][idx].flow);
                curr = prev;
            }
            curr = V;
            while (curr != 1) {
                int prev = p_node[curr];
                int idx = p_edge[curr];
                
                graph[prev][idx].flow += push;
                int rev_idx = graph[prev][idx].rev;
                graph[curr][rev_idx].flow -= push;

                total_cost += (long long)push * graph[prev][idx].cost;
                curr = prev;
            }
            maxFlow += push;
        }
        cout << "Instancia " << t++ << endl;
        if (maxFlow == D) {
            cout << total_cost << endl;
        } else {
            cout << "impossivel" << endl;
        }
        cout << endl;
    }
    return 0;
}
