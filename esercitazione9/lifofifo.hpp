
#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <vector>
#include <set>
#include "unidirected_graph.hpp"


// classe FIFO

class fifo {
    std::queue<int> q;
public:
    fifo() = default;
    void put(int x) { 
        q.push(x); 
    }
    int get() {
        int x = q.front(); 
        q.pop(); 
        return x; 
    }
    bool empty() const { 
        return q.empty(); 
    }
};

// classe LIFO

class lifo {
    std::stack<int> s;
public:
    lifo() = default;
    void put(int x) { 
        s.push(x); 
    }
    int get() { 
        int x = s.top(); 
        s.pop(); 
        return x; 
    }
    bool empty() const { 
        return s.empty(); 
    }
};

// funzione template graph_visit

template<typename container>
unidirected_graph graph_visit(const unidirected_graph& G, int source, container& c)
{
    unidirected_graph tree;
    std::set<int> visited;

    c.put(source);
    visited.insert(source);

    while (!c.empty()) {
        int dummy = c.get();
        for (int v : G.neighbours(dummy)) {
            if (visited.find(v) == visited.end()) {
                visited.insert(v);
                tree.add_edge(dummy,v);
                c.put(v);
            }
        }
    }
    return tree;
};

// funzione recursive_dfs()

void dfs_before(const unidirected_graph& G, int u, std::set<int>& visited, unidirected_graph& tree)
{
    visited.insert(u);
    for (int v : G.neighbours(u)) {
        if (visited.find(v) == visited.end()) {
            tree.add_edge(u, v);
            dfs_before(G, v, visited, tree);
        }
    }
}

unidirected_graph recursive_dfs(const unidirected_graph& G, int source) {
    unidirected_graph tree;
    std::set<int> visited;
    dfs_before(G, source, visited, tree);
    return tree;
}