#include <iostream>
#include "unidirected_graph.hpp"

void print_nodes(const std::set<int>& nodes) {
    std::cout << "{ ";
    for (int n : nodes)
        std::cout << n << " ";
    std::cout << "}";
}

void print_edges(const std::set<unidirected_edge>& edges) {
    std::cout << "{ ";
    for (const auto& e : edges)
        std::cout << e << " ";
    std::cout << "}";
}

int main() {

    std::cout << "\n" << "\n";

    std::cout << "TEST 1: add_edge, all_nodes, all_edges\n";
    {
        std::cout << "\n";
        unidirected_graph g;
        g.add_edge(1, 2);
        g.add_edge(2, 3);
        g.add_edge(3, 4);

        std::cout << "Nodi: ";
        print_nodes(g.all_nodes());
        std::cout << "\n";

        std::cout << "Archi: ";
        print_edges(g.all_edges());
        std::cout << "\n";
    }

    std::cout << "\nTEST 2: arco duplicato\n";
    {
        std::cout << "\n";
        unidirected_graph g;
        g.add_edge(1, 2);
        g.add_edge(1, 2);
        g.add_edge(2, 1);

        std::cout << "Archi: ";
        print_edges(g.all_edges());
        std::cout << "\n";
    }

    std::cout << "\nTEST 3: neighbours \n";
    {
        std::cout << "\n";
        unidirected_graph g;
        g.add_edge(1, 2);
        g.add_edge(1, 3);
        g.add_edge(1, 4);

        std::cout << "Vicini di 1: ";
        print_nodes(g.neighbours(1));
        std::cout << "\n";

        std::cout << "Vicini di 2: ";
        print_nodes(g.neighbours(2));
        std::cout << "\n";

        std::cout << "Vicini di 99 (che non esiste): ";
        print_nodes(g.neighbours(99));
        std::cout << "\n";
    }

    std::cout << "\nTEST 4: edge_number e edge_at \n";
    {
        std::cout << "\n";
        unidirected_graph g;
        g.add_edge(1, 2);
        g.add_edge(3, 4);
        g.add_edge(5, 6);

        unidirected_edge e(1, 2);
        int id = g.edge_number(e);
        std::cout << "ID di (1,2): " << id << "\n";

        unidirected_edge recuperato = g.edge_at(id);
        std::cout << "Arco all'ID " << id << ": " << recuperato << "\n";

        unidirected_edge non_presente(10, 20);
        std::cout << "ID di (10,20) (non esistendo -1): " << g.edge_number(non_presente) << "\n";
    }

    std::cout << "\nTEST 5: operator- \n";
    {
        std::cout << "\n";
        unidirected_graph g1, g2;
        g1.add_edge(1, 2);
        g1.add_edge(2, 3);
        g1.add_edge(3, 4);
        g2.add_edge(2, 3);

        unidirected_graph diff = g1 - g2;

        std::cout << "G1 - G2: ";
        print_edges(diff.all_edges());
        std::cout << "\n";
    }

    std::cout << "\nTEST 6: costruttore di copia \n";
    {
        std::cout << "\n";
        unidirected_graph g1;
        g1.add_edge(1, 2);
        g1.add_edge(3, 4);

        unidirected_graph g2(g1);
        g1.add_edge(5, 6);

        std::cout << "Archi g1: ";
        print_edges(g1.all_edges());
        std::cout << "\n";

        std::cout << "Archi g2: ";
        print_edges(g2.all_edges());
        std::cout << "\n";
    }

    std::cout << "\n" << "\n";
}