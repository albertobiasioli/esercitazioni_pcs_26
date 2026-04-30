#include <iostream>
#include <vector>
#include <string>

#include "randfiller.h"
#include "timecounter.h"
#include "sorts.hpp"

int main() {

    randfiller rf;

    int num_sizes = 100;
    std::vector<int> sizes(num_sizes);
    for (int i = 0; i < num_sizes; i++) {
        sizes[i] = i + 1;
    }
    
    int sizes_count = sizes.size();
    int vec_per_size = 100; 

    std::vector<double> mean_bubble(sizes_count);
    std::vector<double> mean_insertion(sizes_count);
    std::vector<double> mean_selection(sizes_count);
    std::vector<double> mean_merge(sizes_count);
    std::vector<double> mean_quick(sizes_count);
    std::vector<double> mean_std(sizes_count);

    bool dimension_found = false;

    int key_dim = -1;


    for (int i = 0; i < sizes_count; i++) 
    {
        std::vector<std::vector<int>> base_vectors(vec_per_size, std::vector<int>(sizes[i]));
        for (int j = 0; j < vec_per_size; j++) {
            rf.fill(base_vectors[j], -10000, 10000);
        }

        std::vector<std::vector<int>> vecs_bubble = base_vectors;
        std::vector<std::vector<int>> vecs_insertion = base_vectors;
        std::vector<std::vector<int>> vecs_selection = base_vectors;
        std::vector<std::vector<int>> vecs_merge = base_vectors;
        std::vector<std::vector<int>> vecs_quick = base_vectors;
        std::vector<std::vector<int>> vecs_std = base_vectors;

        timecounter tc;

        tc.tic();
        for (int j = 0; j < vec_per_size; j++){
            bubble_sort(vecs_bubble[j]);
        }
        double total_time_bubble = tc.toc();

        tc.tic();
        for (int j = 0; j < vec_per_size; j++) {
            insertion_sort(vecs_insertion[j]);
        } 
        double total_time_insertion = tc.toc();

        tc.tic();
        for (int j = 0; j < vec_per_size; j++) {
            selection_sort(vecs_selection[j]);
        }
        double total_time_selection = tc.toc();

        tc.tic();
        for (int j = 0; j < vec_per_size; j++) {
            merge_sort(vecs_merge[j], 0, sizes[i] - 1);
        }
        double total_time_merge = tc.toc();

        tc.tic();
        for (int j = 0; j < vec_per_size; j++) { 
            quick_sort(vecs_quick[j], 0, sizes[i] - 1);
        }
        double total_time_quick = tc.toc();

        tc.tic();
        for (int j = 0; j < vec_per_size; j++) {
            std::sort(vecs_std[j].begin(), vecs_std[j].end());
        }
        double total_time_std = tc.toc();

        mean_bubble[i] = total_time_bubble / vec_per_size;
        mean_insertion[i] = total_time_insertion / vec_per_size;
        mean_selection[i] = total_time_selection / vec_per_size;
        mean_merge[i] = total_time_merge / vec_per_size;
        mean_quick[i] = total_time_quick / vec_per_size;
        mean_std[i] = total_time_std / vec_per_size;

        double best_quad = std::max({mean_bubble[i], mean_insertion[i], mean_selection[i]});
        double worst_log = std::max({mean_merge[i], mean_quick[i], mean_std[i]});

        //std::cout << "N = " << sizes[i] 
        //          << " | Best Quadratic: " << best_quad 
        //          << "s | Worst Logaritmic: " << worst_log << "s\n";

        if (!dimension_found && (worst_log < best_quad)){
            {
                key_dim = sizes[i];
                dimension_found = true; 
            }
        }

    }

    std::cout << "\n" << "--------------------------------" << "\n"
        << "Key Dimension Found: " << key_dim << "\n" 
        << "--------------------------------" << "\n" << "\n";

    std::cout << "New QuickSort with Key Dimension Test..." << "\n" << "\n";

    std::vector<int> sizes2 = {4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
    int sizes_count2 = sizes2.size();
    
    for (int i = 0; i < sizes_count2; i++) {
        std::vector<int> base_vector(sizes[i]);
        rf.fill(base_vector, -10000, 10000);

        std::vector<int> vec_quick = base_vector;
        std::vector<int> vec_quick_v2 = base_vector;

        timecounter tc;

        tc.tic(); 
        quick_sort(vec_quick,0,vec_quick.size()-1); 
        double time_quick = tc.toc();

        tc.tic(); 
        quick_sort_v2(vec_quick_v2,0,vec_quick.size()-1, key_dim); 
        double time_quick_v2 = tc.toc();
  
        std::cout << "Size: " << sizes2[i]
                  << " | Quick: " << time_quick
                  << "s | New Quick: " << time_quick_v2 
                  << "s | Delta " << time_quick - time_quick_v2 << "\n";
    }

    std::cout << "\n";

}


