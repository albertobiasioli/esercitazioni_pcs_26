#include <iostream>
#include <cmath>

int main()
{
    static const int N = 10;
    double arr[N] = {3.5, 7.8, 9.2, 1.7, 9.5, 7.6, 3.2, 1.6, 9.7, 11.0};
    int count = 1;

    while (count != 0)
    {
        count = 0;
        for (int i = 0; i < N-1; i++)
        {
            if (arr[i] > arr[i+1])
            {
                double dummy = arr[i+1];
                arr[i+1] = arr[i];
                arr[i] = dummy;
                count++;
            }
        }
    }

    std::cout << "Ordinato: ";

    for (int i = 0; i < N; i++)
    {
        std::cout << arr[i] << " ";
    }
    
    std::cout  << "\n";
}