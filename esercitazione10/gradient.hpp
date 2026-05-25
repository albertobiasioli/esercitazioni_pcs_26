#pragma once

#include <iostream>
#include <Eigen/Dense>

Eigen::VectorXd gradiente_coniugato(const Eigen::MatrixXd& A, const Eigen::VectorXd& b, const Eigen::VectorXd& x0, double tol = 1.0e-12, unsigned int max_iter = 10000)
{
    const int n = b.size();

    if (A.rows() != n || A.cols() != n) {
        std::cerr << "A e b incompatibili (dimensioni)";
        return x0;
    }
        
    Eigen::VectorXd x = x0; 
    Eigen::VectorXd r = b - A * x;                
    Eigen::VectorXd p = r;                     

    const double r_norm_0 = r.norm();
    
    if (r_norm_0 == 0.0) {
        return x;
    }

    unsigned int it = 0;

    while (it < max_iter && r.norm() > tol * r_norm_0)
    {

        const Eigen::VectorXd Ap = A * p;
        const double pTAp = p.dot(Ap);            

        const double alpha_k = p.dot(r) / pTAp;  

        x = x + alpha_k * p;                    
        r = r - alpha_k * Ap;               

        const double beta_k = p.dot(A * r) / pTAp; 
        p = r - beta_k * p;                        

        ++it;
    }

    return x;
}