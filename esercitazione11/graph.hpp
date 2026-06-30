#pragma once

#include <iostream>
#include <map>
#include <set>
#include <list>

using namespace std; 

template <typename T>
class undirected_edge {
    T nodo_from; 
    T nodo_to;
public:
    // Il costruttore ordina subito i nodi in modo lessicografico (il più piccolo va sempre in nodo_from)
    // Questo ci serve per fare in modo che l'arco (1,2) sia identico all'arco (2,1)
    undirected_edge(const T& n1, const T& n2) {
        if (n1 < n2) {
            nodo_from = n1;
            nodo_to = n2;
        } else {
            nodo_from = n2;
            nodo_to = n1;
        }
    }

    // Metodi per leggere i nodi di partenza e arrivo dell'arco
    const T& from() const { return nodo_from; } 
    const T& to() const { return nodo_to; } 

    // Due archi sono uguali se hanno gli stessi identici nodi
    bool operator==(const undirected_edge& other) const {
        if (nodo_from == other.nodo_from && nodo_to == other.nodo_to) {
            return true;
        } else {
            return false;
        }
    }

    // Operatore minore ordinato: serve a C++ per poter inserire gli archi dentro i std::set o come chiavi nei std::map
    bool operator<(const undirected_edge& other) const {
        if (nodo_from < other.nodo_from) {
            return true;
        } else if (nodo_from > other.nodo_from) {
            return false;
        } else {
            // Se i nodi di partenza sono uguali, decido l'ordine in base al nodo di arrivo
            if (nodo_to < other.nodo_to) {
                return true;
            } else {
                return false;
            }
        }
    }
};

// Operatore per stampare l'arco a schermo nel formato (nodo1--nodo2)
template <typename T>
std::ostream& operator<<(std::ostream & output, const undirected_edge<T>& e) {
    output << "(" << e.from() << "--" << e.to() << ")";
    return output;
}

template <typename T>
class undirected_graph {

    std::map<T, std::set<T>> lista_adiacenza;       // Mappa che associa a ogni nodo l'insieme dei suoi vicini
    std::map<undirected_edge<T>, std::size_t> arco_to_id; // Associa a ogni arco un ID numerico univoco (serve per le matrici)
    std::map<std::size_t, undirected_edge<T>> id_to_arco; // Mappa inversa: dall'ID numerico risalgo all'arco
    std::size_t prossimo_id;                             // Contatore progressivo per assegnare gli ID agli archi
       
public:
    // Il costruttore inizializza il contatore degli ID a zero
    undirected_graph() {
        prossimo_id = 0;
    }

    // Costruttore di copia di default standard
    undirected_graph(const undirected_graph& other) = default; 

    // Restituisce l'insieme dei nodi vicini a un nodo dato
    std::set<T> neighbours(const T& nodo) const {
        auto indice_nodo = lista_adiacenza.find(nodo); 
        // Se il nodo non esiste nel grafo, creo un insieme vuoto e lo ritorno per sicurezza
        if (indice_nodo == lista_adiacenza.end()) {
            std::set<T> insieme_vuoto;
            return insieme_vuoto;
        }
        return lista_adiacenza.at(nodo);
    }

    // Inserisce un arco nel grafo e aggiorna tutte le strutture collegate
    bool add_edge(const undirected_edge<T>& arco) {
    
        // Provo a inserire l'arco nella mappa. Se l'arco esisteva già, l'operazione non fa nulla
        auto risultato_inserimento = arco_to_id.insert({arco, prossimo_id});
        
        // Se l'inserimento è andato a buon fine (quindi l'arco è nuovo e non un duplicato)
        if (risultato_inserimento.second) {
            
            // Aggiorno la mappa inversa ID -> Arco
            id_to_arco.insert({prossimo_id, arco});
            
            // Aggiorno la lista di adiacenza in entrambi i sensi perché il grafo non è orientato
            lista_adiacenza[arco.from()].insert(arco.to());
            lista_adiacenza[arco.to()].insert(arco.from());
            
            // Incremento il contatore per il prossimo arco che verrà inserito
            prossimo_id++;
            return true;
        }
        return false;
    }

    // Estrae tutti gli archi presenti nel grafo inserendoli in una lista
    std::list<undirected_edge<T>> all_edges() const {
        std::list<undirected_edge<T>> lista_archi;
        for (const auto& pair : arco_to_id) {
            lista_archi.push_back(pair.first); // pair.first è l'arco memorizzato come chiave nella mappa
        }
        return lista_archi;
    }

    // Estrae tutti i nodi registrati nel grafo
    std::list<T> all_nodes() const {
        std::list<T> lista_nodi;
        for (const auto& pair : lista_adiacenza) {
            lista_nodi.push_back(pair.first); // Le chiavi della lista di adiacenza sono i nodi stessi
        }
        return lista_nodi;
    }

    // Sato un arco, restituisce il suo numero di riga nella matrice
    std::size_t edge_number(const undirected_edge<T>& arco) const {
        return arco_to_id.at(arco);
    }

    // Funzione inversa: dato un numero di riga, risale all'arco geometrico corrispondente
    undirected_edge<T> edge_at(std::size_t numero) const {
        return id_to_arco.at(numero);
    }

    // Operatore di sottrazione tra grafi: serve per calcolare il coalbero (Grafo_Totale - Albero_DFS)
    undirected_graph<T> operator-(const undirected_graph<T>& other) const {
        undirected_graph<T> grafo_finale;

        // Scorro tutti gli archi del grafo corrente
        for (const auto& pair : arco_to_id) {
            undirected_edge<T> arco_corrente = pair.first;
            
            // Se l'arco corrente NON è presente nel secondo grafo (other), lo aggiungo al grafo differenza
            if (other.arco_to_id.find(arco_corrente) == other.arco_to_id.end()) {
                grafo_finale.add_edge(arco_corrente);
            }
        }
        
        return grafo_finale;
    }
};