#include <iostream>
#include <Eigen/Dense>

#include "gradient.hpp"

int test_gradiente_coniugato(int n)
{
    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(n, n);
    for (int i = 0; i < n; i++)
    {
        A(i, i) = 2.0;
        if (i > 0) {
             A(i, i-1) = -1.0;  
        }
        if (i < n-1) {
            A(i, i+1) = -1.0;
        } 
    }

    Eigen::VectorXd x_esatta(n);
    for (int i = 0; i < n; i++) {
         x_esatta(i) = i + 1;

    }

    Eigen::VectorXd b = A * x_esatta;
    Eigen::VectorXd x0 = Eigen::VectorXd::Zero(n);
    Eigen::VectorXd x = gradiente_coniugato(A, b, x0);

    double errore = (x - x_esatta).norm() / x_esatta.norm();
    std::cout << "n=" << n << "  errore relativo: " << errore << "\n";

    if (errore > 1e-6) {
    return 1;
    }

    return 0;
}

int main()
{
    int dimensioni[] = {5, 10, 50, 100, 500};
    for (int n : dimensioni)
    {
        if (test_gradiente_coniugato(n) != 0)
            return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}