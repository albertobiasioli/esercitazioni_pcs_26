#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

#include "timecounter.h"
#include "randfiller.h"
#include "sorts.hpp"
using namespace std;


template<typename T>
int test_sort(int dimensione, T val_min, T val_max){
	randfiller rf;
	vector<T> v(dimensione);
	rf.fill(v, val_min, val_max);
	
	selection_sort(v);
	int c = is_sorted(v);
	return c;
}

int main(){
	randfiller rf;
	vector<int> dimensioni(100);
	rf.fill(dimensioni, 1, 8000);
	for(int i=0;i<dimensioni.size();i++){
		int t_1 = test_sort<int>(dimensioni[i],-10000, 10000);
		if(t_1 != 0){
			return EXIT_FAILURE;
		}
		int t_2 = test_sort<double>(dimensioni[i],-10000, 10000);
		if(t_2 != 0){
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
};