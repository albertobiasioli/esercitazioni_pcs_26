#include <iostream>
#include <vector>
#include <string>

#include "sorts.hpp"
using namespace std;

int main(){
	vector<string> strings = {
		"zucchero", "albero", "dado", "casa", "barca", 
		"xilofono", "mela", "fuoco", "gatto", "erba"
	};
	
	// Test Bubble Sort
	vector<string> v_bubble = strings;
	bubble_sort(v_bubble);
	int t_1 = is_sorted(v_bubble);
	if(t_1 != 0){
		return EXIT_FAILURE;
	}

	// Test Insertion Sort
	vector<string> v_insertion = strings;
	insertion_sort(v_insertion);
	int t_2 = is_sorted(v_insertion);
	if(t_2 != 0){
		return EXIT_FAILURE;
	}

	// Test Selection Sort
	vector<string> v_selection = strings;
	selection_sort(v_selection);
	int t_3 = is_sorted(v_selection);
	if(t_3 != 0){
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
};