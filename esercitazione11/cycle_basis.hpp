#pragma once

#include "graph.hpp"      
#include "algorithms.hpp" 
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using BinaryVector = std::vector<bool>;

struct OrientedEdge {
    size_t edge_id; 
    int sign;      
};

using OrientedCycle = std::vector<OrientedEdge>;

// Nodo virtuale per la costruzione del rivestimento doppio del grafo
template <typename T>
struct VirtualNode {
    T id;            // ID del nodo originale del grafo
    char sheet_sign; // Identifica il foglio parallelo: '+' (positivo) o '-' (negativo)

    // Operatori di confronto necessari per l'ordinamento nei std::set e std::map
    bool operator<(const VirtualNode& other) const {
        if (id != other.id) return id < other.id;
        return sheet_sign < other.sheet_sign;
    }
    bool operator>(const VirtualNode& other) const { return other < *this; }
    bool operator==(const VirtualNode& other) const { return id == other.id && sheet_sign == other.sheet_sign; }
    bool operator!=(const VirtualNode& other) const { return !(*this == other); }
};

// Prodotto scalare nello spazio vettoriale su F2 (operatore AND seguito da XOR)
inline int dot_product_f2(const BinaryVector& a, const BinaryVector& b) {
    int risultato = 0;
    for (size_t i = 0; i < a.size(); i++) {
        risultato ^= (a[i] && b[i]); 
    }
    return risultato;
}

// Somma tra vettori binari su F2 (equivalente all'operatore bitwise XOR)
inline BinaryVector add_f2(const BinaryVector& a, const BinaryVector& b) {
    BinaryVector risultato(a.size());
    for (size_t i = 0; i < a.size(); i++) {
        risultato[i] = a[i] ^ b[i]; 
    }
    return risultato;
}

// Decomposizione del grafo G=(V,E) nell'albero di copertura T e nel relativo coalbero
template <typename T>
bool trova_albero_e_coalbero(const undirected_graph<T>& grafo, 
                             undirected_graph<T>& albero_copertura, 
                             std::vector<undirected_edge<T>>& archi_coalbero,
                             std::vector<BinaryVector>& vettori_supporto) {
    
    auto nodi = grafo.all_nodes();
    if (nodi.empty()) return false;

    // Estraggo l'albero di copertura aciclico tramite la DFS ricorsiva
    albero_copertura = dfs_ricorsiva(grafo, nodi.front());

    // Se l'albero non tocca tutti i nodi, il grafo di partenza non è connesso
    if (albero_copertura.all_nodes().size() != nodi.size()) {
        return false; 
    }

    // Il coalbero è definito dalla differenza tra il grafo totale e l'albero
    undirected_graph<T> grafo_coalbero = grafo - albero_copertura;
    
    for (const auto& arco : grafo_coalbero.all_edges()) {
        archi_coalbero.push_back(arco);
    }

    // Inizializzo i vettori di supporto (base ortogonale su F2 per l'algoritmo di De Pina)
    vettori_supporto.assign(archi_coalbero.size(), BinaryVector(grafo.all_edges().size(), false));
    
    // Ogni vettore della base associa inizialmente un bit attivo a una e una sola corda del coalbero
    for (size_t i = 0; i < archi_coalbero.size(); i++) {
        vettori_supporto[i][grafo.edge_number(archi_coalbero[i])] = true;
    }
    return true;
}

template <typename T>
std::pair<BinaryVector, OrientedCycle> trova_ciclo_minimo(const undirected_graph<T>& grafo, 
                                                          const std::map<undirected_edge<T>, double>& pesi, 
                                                          const BinaryVector& vettore_firma) {
    
    undirected_graph<VirtualNode<T>> grafo_rivestimento;
    std::map<undirected_edge<VirtualNode<T>>, double> pesi_rivestimento;
    double somma_pesi = 0.0;

    // COSTRUZIONE DEL LIFTING DEL GRAFO
    // Sdoppio il grafo su due fogli paralleli (+ e -). L'obiettivo dalla teoria è forzare
    // Dijkstra a trovare un cammino inter-foglio, che corrisponde a un ciclo reale 
    // avente intersezione dispari (prodotto scalare = 1) con il vettore_firma.
    for (const auto& arco : grafo.all_edges()) {
        size_t id_arco = grafo.edge_number(arco);
        
        VirtualNode<T> u_pos{arco.from(), '+'};
        VirtualNode<T> u_neg{arco.from(), '-'};
        VirtualNode<T> v_pos{arco.to(), '+'};
        VirtualNode<T> v_neg{arco.to(), '-'};

        double w = 1.0;
        auto it = pesi.find(arco);
        if (it != pesi.end()) {
            w = it->second;
        }
        somma_pesi += w;

        // Se l'arco appartiene alla firma: creo i ponti incrociati tra i due fogli (+ a - e - a +)
        if (vettore_firma[id_arco] == true) {
            undirected_edge<VirtualNode<T>> e1(u_pos, v_neg);
            undirected_edge<VirtualNode<T>> e2(u_neg, v_pos);
            grafo_rivestimento.add_edge(e1); 
            grafo_rivestimento.add_edge(e2);
            pesi_rivestimento[e1] = w; 
            pesi_rivestimento[e2] = w;
        } 
        // Se l'arco non appartiene alla firma: mantengo le connessioni parallele sullo stesso foglio
        else {
            undirected_edge<VirtualNode<T>> e1(u_pos, v_pos);
            undirected_edge<VirtualNode<T>> e2(u_neg, v_neg);
            grafo_rivestimento.add_edge(e1); 
            grafo_rivestimento.add_edge(e2);
            pesi_rivestimento[e1] = w; 
            pesi_rivestimento[e2] = w;
        }
    }

    // Soglia iniziale superiore alla somma di tutti i pesi degli archi del grafo
    double peso_minimo_cammino = somma_pesi + 1.0; 
    std::map<VirtualNode<T>, VirtualNode<T>> padri_migliori; 
    VirtualNode<T> inizio_migliore, fine_migliore; 
    bool cammino_trovato = false; 

    // CHIAMATA ITERATIVA A DIJKSTRA: trovo il ciclo minimo che rispetta la firma corrente
    for (const auto& nodo : grafo.all_nodes()) { 
        VirtualNode<T> v_pos{nodo, '+'}; // Sorgente sul foglio positivo del rivestimento
        VirtualNode<T> v_neg{nodo, '-'}; // Traguardo sul foglio negativo dello stesso nodo
        
        std::map<VirtualNode<T>, VirtualNode<T>> padri_correnti; 
        auto distanze = dijkstra(grafo_rivestimento, v_pos, padri_correnti, pesi_rivestimento); 

        auto it_distanze = distanze.find(v_neg); 
        // Se esiste un cammino inter-foglio più corto del minimo globale attuale, lo salvo
        if (it_distanze != distanze.end() && it_distanze->second < peso_minimo_cammino) { 
            peso_minimo_cammino = it_distanze->second; 
            padri_migliori = padri_correnti;           
            inizio_migliore = v_pos;                   
            fine_migliore = v_neg;                     
            cammino_trovato = true;                    
        } 
    } 

    BinaryVector C(grafo.all_edges().size(), false); 
    OrientedCycle ciclo_orientato;                  

    // RICOSTRUZIONE DELLA MAGLIA: risalgo la catena dei padri dal traguardo '-' alla sorgente '+'
    if (cammino_trovato) { 
        std::vector<VirtualNode<T>> cammino; 
        VirtualNode<T> corrente = fine_migliore; 
        
        while (!(corrente == inizio_migliore)) { 
            cammino.push_back(corrente);             
            corrente = padri_migliori.at(corrente); 
        } 
        cammino.push_back(inizio_migliore); 
        
        std::reverse(cammino.begin(), cammino.end()); // Riordino la sequenza dal nodo di partenza a quello di arrivo

        // TRADUZIONE DEI PASSI VIRTUALI IN ARCHI REALI ORIENTATI
        for (size_t i = 0; i < cammino.size() - 1; i++) { 
            undirected_edge<T> arco_originale(cammino[i].id, cammino[i+1].id); 
            size_t id_arco = grafo.edge_number(arco_originale);               
            
            C[id_arco] = !C[id_arco]; // Proprietà di F2: il doppio passaggio cancella l'arco (XOR)

            // CONVENZIONE DEI SEGNI: confronto il verso del ciclo con la freccia dell'arco
            int segno = -1; 
            if (cammino[i].id < cammino[i+1].id) { 
                segno = 1; // Il ciclo attraversa l'arco nel da ID minore a ID maggiore
            } 
            ciclo_orientato.push_back({id_arco, segno}); 
        } 
    } 
    
    return {C, ciclo_orientato}; 
}

// Algoritmo principale di De Pina per estrarre la base delle maglie minime
template <typename T>
std::vector<OrientedCycle> de_pina(const undirected_graph<T>& grafo, 
                                   const std::map<undirected_edge<T>, double>& pesi, 
                                   std::vector<BinaryVector>& vettori_supporto) {
    
    std::vector<OrientedCycle> base_cicli;
    
    // Scorro i vettori di supporto: a ogni passo trovo un ciclo minimo e aggiorno i vettori 
    // successivi per mantenere rigorosamente l'indipendenza lineare della base su F2
    for (size_t i = 0; i < vettori_supporto.size(); i++) {
        auto risultato = trova_ciclo_minimo(grafo, pesi, vettori_supporto[i]);
        BinaryVector C_i = risultato.first;
        
        base_cicli.push_back(risultato.second); // Salvo la maglia orientata minima trovata

        // Fase di ortogonalizzazione: se il ciclo appena estratto interseca in modo dispari
        // un vettore di supporto successivo, sommo i vettori su F2 per azzerare il bit e ortogonalizzare
        for (size_t j = i + 1; j < vettori_supporto.size(); j++) {
            if (dot_product_f2(C_i, vettori_supporto[j]) == 1) {
                vettori_supporto[j] = add_f2(vettori_supporto[j], vettori_supporto[i]);
            }
        }
    }
    return base_cicli;
}

// Trova l'unico cammino tra due nodi nell'albero
template <typename T>
bool cerca_cammino_albero(const undirected_graph<T>& albero_copertura, 
                          const T& corrente, 
                          const T& target, 
                          std::set<T>& visitati, 
                          std::vector<T>& cammino) {
    
    // Ipotizzo che il nodo 'corrente' faccia parte della strada giusta.
    // Lo inserisco immediatamente nei visitati per evitare di girare a vuoto e lo aggiungo al cammino.
    visitati.insert(corrente); 
    cammino.push_back(corrente); 

    // Se sono arrivato al nodo target, ho trovato la strada.
    if (corrente == target) return true; 

    // Scorro i vicini del nodo corrente muovendomi solo sui rami dell'albero
    for (const auto& vicino : albero_copertura.neighbours(corrente)) {
        
        // Se il vicino non è ancora stato esplorato in questo ramo, vado da lui
        if (visitati.find(vicino) == visitati.end()) {
            
            // Chiedo al vicino di cercare il target a partire da se stesso.
            // Se questa chiamata restituisce 'true', significa che il traguardo è stato trovato più in basso.
            if (cerca_cammino_albero(albero_copertura, vicino, target, visitati, cammino)) {
                return true; 
            }
        }
    }
    
    // Se il ciclo for finisce e non abbiamo ritornato 'true',
    // significa che tutti i vicini di questo nodo portano a strade interrotte. L'ipotesi iniziale era sbagliata.
    // Rimuovo il nodo dal vettore cammino per lasciare la strada pulita e ritorno false per avvisare il padre.
    cammino.pop_back(); 
    return false;
}

// Chiude le corde del coalbero per formare i cicli

template <typename T>
std::vector<OrientedCycle> cicli_fondamentali_dfs(const undirected_graph<T>& grafo, 
                                                  const undirected_graph<T>& albero_copertura, 
                                                  const std::vector<undirected_edge<T>>& archi_coalbero) {
    std::vector<OrientedCycle> base_cicli;

    // Ciascuna corda (arco rimasto fuori dall'albero) se reinserita chiude una e una sola maglia indipendente
    for (const auto& corda : archi_coalbero) {
        std::set<T> visitati;
        std::vector<T> cammino;
        
        // Sfrutto la proprietà degli alberi: trovo l'unico cammino nell'albero tra gli estremi della corda.
        cerca_cammino_albero(albero_copertura, corda.from(), corda.to(), visitati, cammino);

        OrientedCycle ciclo;
        
        // Scorro la sequenza di nodi per determinare il verso della legge di Kirchov
        for (size_t i = 0; i < cammino.size() - 1; i++) {
            undirected_edge<T> arco_albero(cammino[i], cammino[i+1]); 
            size_t id_arco = grafo.edge_number(arco_albero); // Recupero l'indice di riga del componente nelle matrici
            
            // La classe undirected_edge ordina i nodi in modo che l'arco punti sempre, per convenzione,
            //  dal nodo con ID minore a quello con ID maggiore.
            int segno = -1;
            // Se il nostro cammino si sta muovendo da un ID minore a uno maggiore, stiamo camminando
            // nel verso naturale dell'arco, quindi assegniamo segno positivo (+1).
            if (cammino[i] < cammino[i+1]) {
                segno = 1;
            }
            ciclo.push_back({id_arco, segno}); // Inserisco l'elemento orientato nella maglia corrente
        }

        // Il cammino nell'albero ci ha portati dal punto di partenza (corda.from()) al traguardo (corda.to()).
        // Manca l'ultimo passo per tornare indietro: da corda.to() a corda.from().
        size_t id_corda = grafo.edge_number(corda);
        
        int segno_corda = -1;
        // Applico la stessa regola: se per tornare indietro ci muoviamo da un ID minore a uno maggiore,
        // il movimento concorda con la freccia predefinita della corda, quindi il segno sarà +1.
        if (corda.to() < corda.from()) {
            segno_corda = 1;
        }
        ciclo.push_back({id_corda, segno_corda}); // Inserisco la corda per completare l'equazione di Kirchhoff

        base_cicli.push_back(ciclo); // Salvo la maglia fondamentale completa all'interno della base dei cicli
    }

    return base_cicli;
}