#pragma once
#include <list>

// Classe che implementa una Coda (FIFO: First In, First Out)
template <typename T>
class fifo {
    std::list<T> c; // Lista interna che fa da contenitore per i dati
public:
    fifo() = default; // Costruttore di default standard
    
    // Inserisce un elemento in fondo alla coda
    void put(const T& val) { c.push_back(val); }
    
    // Legge il primo elemento della coda, lo rimuove dalla lista e lo ritorna
    T get() { 
        T val = c.front();
        c.pop_front();     
        return val;        
    }
    
    // Controlla se la coda è vuota
    bool empty() const { return c.empty(); }
};

// Classe che implementa una Pila o Stack (LIFO: Last In, First Out)
template <typename T>
class lifo {
    std::list<T> c; // Lista interna che fa da contenitore per i dati
public:
    lifo() = default; // Costruttore di default standard
    
    // Inserisce un elemento in testa alla pila
    void put(const T& val) { c.push_front(val); }
    
    // Legge l'ultimo elemento inserito, lo rimuove e lo ritorna
    T get() { 
        T val = c.front(); 
        c.pop_front();     
        return val;         
    }
    
    // Controlla se la pila è vuota
    bool empty() const { return c.empty(); }
};