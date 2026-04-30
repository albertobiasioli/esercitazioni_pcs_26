#include <iostream>
#include <vector>
#include <string>

#include "randfiller.h"
#include "timecounter.h"
#include "sorts.hpp"

int main() {
    
    randfiller rf;

    std::vector<int> sizes = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
    int sizes_count = sizes.size();
    

    // Benchmark int
    std::cout << "\n-- Benchmark Tipo: INTERI (int) --\n";
    for (int i = 0; i < sizes_count; i++) {
        std::vector<int> base_vector(sizes[i]);
        rf.fill(base_vector, -10000, 10000);

        std::vector<int> vec_bubble = base_vector;
        std::vector<int> vec_insertion = base_vector;
        std::vector<int> vec_selection = base_vector;
        std::vector<int> vec_merge = base_vector;
        std::vector<int> vec_quick = base_vector;
        std::vector<int> vec_std = base_vector;

        timecounter tc;

        tc.tic(); 
        bubble_sort(vec_bubble); 
        double time_bubble = tc.toc();
        
        tc.tic(); 
        insertion_sort(vec_insertion); 
        double time_insertion = tc.toc();
        
        tc.tic(); 
        selection_sort(vec_selection); 
        double time_selection = tc.toc();
        
        tc.tic(); 
        merge_sort(vec_merge,0,vec_merge.size()-1); 
        double time_merge = tc.toc();

        tc.tic(); 
        quick_sort(vec_quick,0,vec_quick.size()-1); 
        double time_quick = tc.toc();
        
        tc.tic(); 
        std::sort(vec_std.begin(), vec_std.end()); 
        double time_std = tc.toc();

        std::cout << "Size: " << sizes[i]
                  << " | Bubble: " << time_bubble
                  << "s | Insertion: " << time_insertion
                  << "s | Selection: " << time_selection
                  << "s | Merge: " << time_merge
                  << "s | Quick: " << time_quick
                  << "s | std::sort: " << time_std << "s\n";
    }

    // Benchmark double
    std::cout << "\n--- Benchmark Tipo: DOUBLE ---\n";
    for (int i = 0; i < sizes_count; i++) {
        std::vector<double> base_vector(sizes[i]);
        rf.fill(base_vector, -10000.0, 10000.0);

        std::vector<double> vec_bubble = base_vector;
        std::vector<double> vec_insertion = base_vector;
        std::vector<double> vec_selection = base_vector;
        std::vector<double> vec_std = base_vector;
        std::vector<double> vec_merge = base_vector;
        std::vector<double> vec_quick = base_vector;

        timecounter tc;

        tc.tic(); 
        bubble_sort(vec_bubble); 
        double time_bubble = tc.toc();
        
        tc.tic(); 
        insertion_sort(vec_insertion); 
        double time_insertion = tc.toc();
        
        tc.tic(); 
        selection_sort(vec_selection); 
        double time_selection = tc.toc();

        tc.tic(); 
        merge_sort(vec_merge,0,vec_merge.size()-1); 
        double time_merge = tc.toc();

        tc.tic(); 
        quick_sort(vec_quick,0,vec_quick.size()-1); 
        double time_quick = tc.toc();

        tc.tic(); 
        std::sort(vec_std.begin(), vec_std.end()); 
        double time_std = tc.toc();

        std::cout << "Size: " << sizes[i]
                  << " | Bubble: " << time_bubble
                  << "s | Insertion: " << time_insertion
                  << "s | Selection: " << time_selection 
                  << "s | Merge: " << time_merge
                  << "s | Quick: " << time_quick
                  << "s | std::sort: " << time_std << "s\n";
    }

    std::cout << endl;

    return 0;
}
