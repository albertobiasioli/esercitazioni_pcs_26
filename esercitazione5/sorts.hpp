#pragma once

#include <iostream>
#include <optional>
#include <vector>
#include <math.h>
#include <string.h>

using namespace std;

template<typename T>
void bubble_sort(vector<T>& vec)
{
    int N = vec.size();
    for (int i = 0; i < N-1; i++)
    {
        for (int j = 0; j < N-1-i; j++)
        {
            if (vec[j] > vec[j + 1]) {
                T key = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = key;
            }
        }
    }
    return;
}

template<typename T>
void merge(vector<T>& vec, int p, int q, int r)
{
    int n1 = q-p+1;
    int n2 = r-q;
    vector<T> L (n1+1);
    vector<T> R (n2+1);
    for (int i = 0; i < n1; i++)
    {
        L[i] = vec[p+i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = vec[q+j+1];
    }
    L[n1] = 1000000;
    R[n2] = 1000000;
    int i = 0;
    int j = 0;
    for (int k = p; k <= r; k++)
    {
        if (L[i] <= R[j])
        {
            vec[k] = L[i];
            i++;
        }
        else 
        {
            vec[k] = R[j];
            j++;
        }
    }
}

//aiuto di Gemini sulla gestione di un template specifico per le stringhe

template<>
void merge<std::string>(vector<std::string>& vec, int p, int q, int r)
{
    int n1 = q-p+1;
    int n2 = r-q;
    vector<std::string> L (n1+1);
    vector<std::string> R (n2+1);
    for (int i = 0; i < n1; i++)
    {
        L[i] = vec[p+i];
    }
    for (int j = 0; j < n2; j++)
    {
        R[j] = vec[q+j+1];
    }
    L[n1] = "}}}}}}}}";
    R[n2] = "}}}}}}}}";
    int i = 0;
    int j = 0;
    for (int k = p; k <= r; k++)
    {
        if (L[i] <= R[j])
        {
            vec[k] = L[i];
            i++;
        }
        else 
        {
            vec[k] = R[j];
            j++;
        }
    }
}

//fine aiuto Gemini

template<typename T>
void merge_sort(vector<T>& vec, int p, int r)
{
    if ( p<r )
    {
        int q = floor((p+r)/2);
        merge_sort(vec,p,q);
        merge_sort(vec,q+1,r);
        merge(vec,p,q,r);
    }
}

template<typename T>
int partition(vector<T>& vec, int p, int r)
{
    T x = vec[r];
    int i = p-1;
    for (int j = p; j <= r-1; j++)
    {
        if (vec[j] <= x)
        {
            i++;
            T dummy = vec[i];
            vec[i] = vec[j];
            vec[j] = dummy;
        }
    }
    T dummy = vec[i+1];
    vec[i+1] = vec[r];
    vec[r] = dummy;
    return i+1;
}

template<typename T>
void quick_sort(vector<T>& vec, int p, int r)
{   
    if (p < r)
    {
        int q = partition(vec,p,r);
        quick_sort(vec,p,q-1);
        quick_sort(vec,q+1,r);
    }
}

template<typename T>
void insertion_sort(std::vector<T>& vec)
{
    int N = vec.size();
    for (int j = 1; j < N; j++)
    {
        T key = vec[j];
        int i = j-1;
        while (i>=0 && vec[i] > key) 
        {
            vec[i+1] = vec[i];
            i = i-1;
        }
        vec[i+1] = key;
    }
    return;
}

template<typename T>
void quick_sort_v2(vector<T>& vec, int p, int r, int key_dim)
{
    if (vec.size() < key_dim)
    {
        insertion_sort(vec);
    }
    else
   {
        quick_sort(vec, p, r);
   }
}



template<typename T>
void selection_sort(std::vector<T>& vec)
{
    int N = vec.size();
    for (int i = 0; i < N; i++)
    {
        int min = i;
        for (int j = i+1; j < N; j++)
        {
            if (vec[j] < vec[min])
            {
                min = j;
            }
        }
        T key = vec[i];
        vec[i] = vec[min];
        vec[min] = key;
    }
    return;
}

template<typename T>
int is_sorted(const vector<T>& vec)
{
    int N = vec.size();
    for (int i=0; i < N-1; i++)
    {
        if( vec[i] > vec[i+1])
        {
            return 1;
        }
    }
    return 0;
}