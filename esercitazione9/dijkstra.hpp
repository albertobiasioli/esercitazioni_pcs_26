#pragma once

#include <map>
#include <vector>
#include <queue> 
#include <algorithm>
#include "unidirected_graph.hpp"

struct dijkstraResult {
    std::map<int, double> dist;
    std::map<int, int>    prev;
};

dijkstraResult dijkstra(const unidirected_graph& G, int source, const std::map<unidirected_edge, double>& edge_weights = {}) {
    
    dijkstraResult result;
    const double INF = 1e9; 

    for (int v : G.all_nodes()) {
        result.dist[v] = INF;
        result.prev[v] = -1;     
    }
    
    result.dist[source] = 0.0;

    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;
    pq.push({0.0, source});

    while (!pq.empty()) {
        std::pair<double, int> u_pair = pq.top();
        pq.pop();

        double d_u = u_pair.first;
        int u = u_pair.second;

        if (d_u > result.dist[u]) continue;

        for (int v : G.neighbours(u)) {
            double w = 1.0;
            unidirected_edge edge(u, v);
            if (edge_weights.find(edge) != edge_weights.end()) {
                w = edge_weights.at(edge);
            }

            if (result.dist[u] + w < result.dist[v]) {
                result.dist[v] = result.dist[u] + w;  
                result.prev[v] = u;                  
                pq.push({result.dist[v], v});
            }
        }
    }

    return result;
}

std::vector<int> get_path(const dijkstraResult& r, int source, int target) {
    std::vector<int> path;

    if (r.prev.at(target) == -1 && target != source) {
        return path;
    }

    int current = target;
    while (current != source) {
        path.push_back(current);
        current = r.prev.at(current);
        
        if (current == -1) return std::vector<int>();
    }
    path.push_back(source);

    std::reverse(path.begin(), path.end());
    return path;
}