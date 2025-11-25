#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
#include <iomanip> // Para formatação

using namespace std;

const long long INF = 1e18;

struct edge {
    int v, cap, flow, cost, rev;
};
vector<vector<edge>> graph;
vector<long long> dist;
vector<int> p_node;
vector<int> p_edge; 

// Função auxiliar para imprimir o estado atual das distâncias
void printDistances(int V_size) {
    cout << "   [Distâncias]: ";
    for(int i=1; i<=V_size; i++) {
        if(dist[i] == INF) cout << "INF ";
        else cout << dist[i] << " ";
    }
    cout << endl;
}

bool bellman_ford(int s, int t, int V_size) {
    cout << "\n--- Iniciando Bellman-Ford (Busca do Menor Caminho no Grafo Residual) ---" << endl;
    dist.assign(V_size + 1, INF);
    p_node.assign(V_size + 1, -1);
    p_edge.assign(V_size + 1, -1);    
    dist[s] = 0;
    
    cout << "   Estado Inicial: Distância da origem " << s << " é 0. As demais são INF." << endl;

    // Loop de relaxamento (V-1 vezes)
    for (int i = 1; i <= V_size - 1; i++) {
        bool relaxed = false;
        // Tenta relaxar todas as arestas
        for (int u = 1; u <= V_size; u++) {
            if (dist[u] == INF) continue; // Não podemos relaxar a partir de um nó inalcançável

            for (int j = 0; j < graph[u].size(); j++) {
                edge &e = graph[u][j];                
                // Só considera arestas com capacidade residual
                if (e.cap - e.flow > 0) {
                    if (dist[e.v] > dist[u] + e.cost) {
                        cout << "   Relaxando aresta " << u << " -> " << e.v 
                             << " (Custo: " << e.cost << ", Residual: " << e.cap - e.flow << ")"
                             << ". Nova dist para " << e.v << ": " << dist[u] + e.cost << endl;
                        
                        dist[e.v] = dist[u] + e.cost;
                        p_node[e.v] = u;
                        p_edge[e.v] = j;
                        relaxed = true;
                    }
                }
            }
        }
        if (!relaxed) {
            cout << "   Nenhuma aresta relaxada na iteracao " << i << ". Bellman-Ford convergiu cedo." << endl;
            break; 
        }
    }
    
    if (dist[t] != INF) 
        cout << "   Caminho encontrado para o destino " << t << " com custo total: " << dist[t] << endl;
    else 
        cout << "   Nao foi possivel chegar ao destino " << t << " no grafo residual." << endl;

    return dist[t] != INF;
}

void addEdge(int u, int v, int cap, int cost) {
    // Aresta direta: custo positivo, capacidade K
    edge a = {v, cap, 0, cost, (int)graph[v].size()};
    // Aresta reversa: custo negativo, capacidade 0 inicial
    edge b = {u, 0, 0, -cost, (int)graph[u].size()};
    
    graph[u].push_back(a);
    graph[v].push_back(b);
}

int main() {    
    int V, E; 
    int t = 1;

    // Para testar, você pode rodar com a entrada do exemplo ou redirecionar um arquivo
    while (cin >> V >> E) {
        cout << "========================================" << endl;
        cout << "PROCESSANDO INSTANCIA " << t << endl;
        cout << "========================================" << endl;

        vector<tuple<int, int, int>> temp_edges;
        for (int i = 0; i < E; i++) {
            int u, v, c;
            cin >> u >> v >> c;
            temp_edges.emplace_back(u, v, c);
        }
        int D, K;
        cin >> D >> K;
        cout << "Amigos (Demanda): " << D << ", Assentos por voo (Capacidade): " << K << endl;

        graph.assign(V + 1, vector<edge>());        
        for (auto [u, v, c] : temp_edges) {
            addEdge(u, v, K, c);
            addEdge(v, u, K, c);
        }
        long long total_cost = 0;
        int maxFlow = 0;         

        int iteracao_fluxo = 1;
        // Enquanto houver caminho de aumento e a demanda não for atendida
        while (maxFlow < D && bellman_ford(1, V, V)) {
            cout << "\n--- Aumentando Fluxo (Iteracao " << iteracao_fluxo++ << ") ---" << endl;
            
            int push = D - maxFlow;
            int curr = V;
            
            // Backtracking para encontrar o gargalo do caminho
            cout << "   Caminho reverso encontrado: " << V;
            vector<int> path_nodes;
            path_nodes.push_back(V);
            
            int temp_curr = V;
            while (temp_curr != 1) {
                int prev = p_node[temp_curr];
                int idx = p_edge[temp_curr];
                push = min(push, graph[prev][idx].cap - graph[prev][idx].flow);
                temp_curr = prev;
                path_nodes.push_back(prev);
            }
            
            // Imprime o caminho na ordem correta
            cout << " <- ";
            for(int i = path_nodes.size()-2; i >= 0; i--) {
                cout << path_nodes[i] << (i > 0 ? " <- " : "");
            }
            cout << endl;

            cout << "   Gargalo (Flow a enviar): " << push << endl;

            // Aplicando o fluxo
            curr = V;
            while (curr != 1) {
                int prev = p_node[curr];
                int idx = p_edge[curr];
                
                graph[prev][idx].flow += push;
                int rev_idx = graph[prev][idx].rev;
                graph[curr][rev_idx].flow -= push; // Reduz capacidade na volta (aumenta potencial de cancelamento)

                total_cost += (long long)push * graph[prev][idx].cost;
                
                cout << "      Atualizando aresta " << prev << "->" << curr 
                     << ": Flow agora e " << graph[prev][idx].flow << "/" << graph[prev][idx].cap 
                     << ". Custo acumulado parcial: " << total_cost << endl;

                curr = prev;
            }
            maxFlow += push;
            cout << "   Fluxo Total Atual: " << maxFlow << "/" << D << endl;
        }

        cout << "\nRESULTADO FINAL INSTANCIA " << t++ << endl;
        if (maxFlow == D) {
            cout << "Custo Minimo Total: " << total_cost << endl;
        } else {
            cout << "impossivel (Conseguiu apenas " << maxFlow << " de " << D << ")" << endl;
        }
        cout << endl;
    }
    return 0;
}