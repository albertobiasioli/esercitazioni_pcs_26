#pragma once

#include "graph.hpp"
#include <set>
#include <map>
#include <vector>

// Struttura di supporto per associare un nodo alla sua distanza temporanea
// Ci serve dentro Dijkstra per tenere ordinati gli elementi nel set
template <typename T>
struct nodo_distanza {
    double dist; 
    T nodo;      

    // Operatore minore ordinato: il set lo usa per mettere in testa il nodo con la distanza minore
    bool operator<(const nodo_distanza& altro) const {
        if (dist != altro.dist) {
            return dist < altro.dist; 
        }
        return nodo < altro.nodo; // Se la distanza è uguale, ordino in base all'ID del nodo
    }
};

// Funzione generica per visitare il grafo usando un container adapter (coda o pila)
// Se passiamo una fifo fa una BFS, se passiamo una lifo fa una DFS
template <typename T, typename Container>
undirected_graph<T> visita_grafo(const undirected_graph<T>& grafo, const T& sorgente, Container& c) {
    undirected_graph<T> albero;  
    std::set<T> visitati;       

    c.put(sorgente);          
    visitati.insert(sorgente); 

    while (!c.empty()) {
        T corrente = c.get(); // Estraggo il nodo corrente dal contenitore

        // Esploro tutti i vicini del nodo estratto
        for (const auto& vicino : grafo.neighbours(corrente)) {
            if (visitati.find(vicino) == visitati.end()) {
                visitati.insert(vicino); 
                albero.add_edge(undirected_edge<T>(corrente, vicino)); // Salvo l'arco nell'albero di copertura
                c.put(vicino); // Inserisco il vicino nel contenitore per visitarlo più avanti
            }
        }
    }
    return albero; 
}

// Funzione che serve per la ricorsiva della DFS
template <typename T>
void dfs_ricorsiva_helper(const undirected_graph<T>& grafo, const T& u, std::set<T>& visitati, undirected_graph<T>& albero) {
    
    // Scorro i vicini del nodo corrente uno alla volta
    for (const auto& v : grafo.neighbours(u)) {
        // Entro nel ramo solo se il vicino non è mai stato toccato da nessuna ramificazione precedente
        if (visitati.find(v) == visitati.end()) {
            visitati.insert(v); // Marco il nodo come visitato nel momento in cui ci entro
            albero.add_edge(undirected_edge<T>(u, v)); // Aggiungo l'arco all'albero DFS
            
            dfs_ricorsiva_helper(grafo, v, visitati, albero); 
        }
    }
}

// Funzione principale per far partire la DFS ricorsiva
template <typename T>
undirected_graph<T> dfs_ricorsiva(const undirected_graph<T>& grafo, const T& sorgente) {
    undirected_graph<T> albero; 
    std::set<T> visitati;      

    visitati.insert(sorgente); // La sorgente è il punto di partenza, la marco come visitata
    dfs_ricorsiva_helper(grafo, sorgente, visitati, albero); 

    return albero; 
}

// Algoritmo di Dijkstra per trovare i cammini minimi partendo da una sorgente
template <typename T>
std::map<T, double> dijkstra(const undirected_graph<T>& rete, 
                             const T& sorgente, 
                             std::map<T, T>& padre, 
                             const std::map<undirected_edge<T>, double>& pesi) {
    
    std::map<T, double> distanze; // Mappa che associa a ogni nodo la sua distanza minima dalla sorgente
    std::set<nodo_distanza<T>> coda_priorita; // Uso un set come coda di priorità (mantiene gli elementi ordinati)
    
    padre.clear(); 
    distanze[sorgente] = 0.0; // La distanza della sorgente da se stessa è ovviamente zero
    
    nodo_distanza<T> nodo_inizio;
    nodo_inizio.dist = 0.0;
    nodo_inizio.nodo = sorgente;
    coda_priorita.insert(nodo_inizio); 

    while (!coda_priorita.empty()) {
        // Prendo l'elemento in testa al set, che ha la distanza minore di tutti
        auto it = coda_priorita.begin();
        double dist_corrente = it->dist; 
        T u = it->nodo;                  
        coda_priorita.erase(it); // Rimuovo il nodo dalla coda perché lo sto analizzando ora

        // Se la distanza estratta è vecchia (superata da un cammino migliore trovato dopo), la ignoro
        if (dist_corrente <= distanze[u]) {
            // Scorro tutti i vicini del nodo corrente per aggiornare le loro distanze
            for (const auto& v : rete.neighbours(u)) {
                undirected_edge<T> arco(u, v); 
                
                // Estraggo il peso dell'arco. Se non è specificato nella mappa dei pesi, di default vale 1.0
                double peso = 1.0;
                auto it_peso = pesi.find(arco);
                if (it_peso != pesi.end()) {
                    peso = it_peso->second;
                }

                bool mai_visitato = (distanze.find(v) == distanze.end());
                
                // Se trovo un cammino più corto per raggiungere il nodo v, aggiorno i dati
                if (mai_visitato || distanze[u] + peso < distanze[v]) {
                    // Se il nodo era già nella coda, devo cancellare il vecchio record prima di inserire quello nuovo
                    if (!mai_visitato) {
                        nodo_distanza<T> vecchio_record;
                        vecchio_record.dist = distanze[v]; 
                        vecchio_record.nodo = v;
                        coda_priorita.erase(vecchio_record); 
                    }
                    
                    // Aggiorno distanza e puntatore al padre
                    distanze[v] = distanze[u] + peso; 
                    padre[v] = u; 
                    
                    // Inserisco il record aggiornato nella coda di priorità
                    nodo_distanza<T> nuovo_record;
                    nuovo_record.dist = distanze[v];
                    nuovo_record.nodo = v;
                    coda_priorita.insert(nuovo_record); 
                }
            }
        }
    }

    return distanze; 
}