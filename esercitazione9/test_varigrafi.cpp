#include <iostream>
#include <fstream>
#include "lifofifo.hpp"
#include "dijkstra.hpp"

//aiuto di Claude per la scrittura della funzione di export del file .dot

void export_dot(const unidirected_graph& G, const std::string& filename)
{
    std::ofstream f(filename);
    for (const auto& e : G.all_edges())
        f << "  " << e.from() << " -- " << e.to() << ";\n";
    f << "}\n";
    std::cout << "Esportato: " << filename << "\n";
}

//fine aiuto Claude

int main()
{
    unidirected_graph G;
    G.add_edge(1, 2);
    G.add_edge(1, 3);
    G.add_edge(1, 4);
    G.add_edge(1, 6);   
    G.add_edge(2, 4);
    G.add_edge(2, 5);
    G.add_edge(2, 7);
    G.add_edge(3, 6);
    G.add_edge(4, 6);
    G.add_edge(4, 7);
    G.add_edge(5, 7);
    G.add_edge(6, 7);
    G.add_edge(6, 8);
    G.add_edge(7, 9);
    G.add_edge(8, 9);
    
    std::cout << "\n";

    std::cout << "Archi Grafo Iniziale:\n";
    std::cout << "\n";
    for (const auto& e : G.all_edges())
        std::cout <<  "  " << e << "\n";
    std::cout << "\n";

    fifo q;
    auto bfs = graph_visit(G, 1, q);
    std::cout << "BFS tree (con orgente 1) -> archi:\n";
    std::cout << "\n";
    for (const auto& e : bfs.all_edges())
        std::cout << "  " << e << "\n";
    std::cout << "\n";

    lifo s;
    auto dfs = graph_visit(G, 1, s);
    std::cout << "DFS iterativa tree (con sorgente 1) -> archi:\n";
    std::cout << "\n";
    for (const auto& e : dfs.all_edges())
        std::cout << "  " << e << "\n";
    std::cout << "\n";

    auto dfsRecursive = recursive_dfs(G, 1);
    std::cout << "DFS ricorsiva tree (con sorgente 1) -> archi:\n";
    std::cout << "\n";
    for (const auto& e : dfsRecursive.all_edges())
        std::cout << "  " << e << "\n";
    std::cout << "\n";

    export_dot(G,   "grafo.dot");
    export_dot(bfs,   "bfs_tree.dot");
    export_dot(dfs,   "dfs_tree.dot");
    export_dot(dfsRecursive,   "dfs_recursive_tree.dot");

    std::cout << "\n";

    std::cout << "Test Algoritmo di Dijkstra:\n\n";
    
    auto dijkstra_res = dijkstra(G, 1);
  
    std::cout << "Distanze minime dal nodo 1:\n";
    for (const auto& pair : dijkstra_res.dist) {
        if (pair.second < 1e9) { 
            std::cout << "  Nodo " << pair.first << " - Distanza: " << pair.second << "\n";
        }
    }
    std::cout << "\n";

    int target_node = 9;
    
    std::cout << "Cammino piu' breve dal nodo 1 al nodo " << target_node << ":\n  ";
    auto path = get_path(dijkstra_res, 1, target_node);
    
    if (path.empty()) {
        std::cout << "Nessun cammino trovato.";
    } else {
        for (size_t i = 0; i < path.size(); ++i) {
            std::cout << path[i] << (i < path.size() - 1 ? " -> " : "");
        }
    }
    std::cout << "\n\n";
}