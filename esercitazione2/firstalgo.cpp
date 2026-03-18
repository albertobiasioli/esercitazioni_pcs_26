#include <iostream>
#include <cmath>

int main()
{
    static const int N = 10;
    double arr[N] = {1.9, 2.8, 3.7, 4.6, 5.5 ,6.4, 7.3, 8.2, 9.1, 10.0};

    // minimo 

    double min = arr[0];

    for (int i = 1; i < N; i++) 
    {
        min = std::min(min, arr[i]);
    }
    
    // massimo

    double max = arr[0];

     for (int i = 1; i < N; i++) 
    {
        max = std::max(min, arr[i]);
    }

    // media 

    double sum = 0;

    for (int i = 0; i < N; i++) 
    {
        sum = sum + arr[i];
    }

    double mean = sum/N;

    // deviazione standard

    double devianza = 0;
    
    for (int i = 0; i < N; i++) 
    {
        devianza = devianza + (arr[i] - mean)*(arr[i] - mean);
    }

    double devstand = std::sqrt(devianza/(N-1));

    std::cout << "Min: " << min << "\n";
    std::cout << "Max: " << max << "\n";
    std::cout << "Mean: " << mean << "\n";
    std::cout << "Standard Deviation: " << devstand << "\n";
}