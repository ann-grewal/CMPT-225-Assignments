// a4_sort_implementations.h

// Student Info
// ------------
//
// Name : Anureet Grewal 
// St.# : 301554326
// Email: anureet_grewal@sfu.ca

// Statement of Originality
// ------------------------
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.

#pragma once
#include "a4_base.h"
using namespace std;


// check is sorted for any vector, O(n) running time 
template <typename T>
bool is_sorted(vector<T> &v) {

    // base cases
    if (v.empty()) return true; 
    if (v.size() == 1) return true; 

    // regualar case 
    for (int i = 1; i < v.size(); i++) {
        if (v[i-1] > v[i]) return false;
    }
    return true; 
}

vector<int> rand_vec(int n, int min, int max) {

    // using time and cstdlib> and <ctime> from base h file can set up random
    // seed random variable sequence with time so updates so sequence changes each second 
    srand(time(0));

    vector <int> random_vector; 
    for (int i = 0; i < n; i++ ) {
        // rand() produces random integer from 0 to RAND_MAX(); 
        // (max - min + 1) + min scales the random integer to be in the bounds of max and min  
        int random = rand() % (max - min + 1) + min;

        // add to vector
        random_vector.push_back(random); 
    }

    return random_vector; 
}

// bubble sort as described in sort.cpp lecture, O(n^2) 
template <typename T>
Sort_stats bubble_sort(vector<T> &v) {

    // set up information 
    ulong comparisons = 0; 
    clock_t start = clock();
        
    // sort information 
    // outer loop 
    for (int i = 0; i < v.size(); i++) {
        // inner loop 
        for (int j = 0; j < v.size() - 1; j++) {
            // compare, use built in cstdlib swap funciton 
            comparisons++; 
            if (v[j] > v[j + 1]) swap(v[j], v[j + 1]); 
        }
    }

    // end information
    clock_t end = clock();
    double time = double(end-start) / CLOCKS_PER_SEC;

    return Sort_stats{"Bubble Sort", v.size(), comparisons, time}; 
}

// insertion sort described in lecture, O(n^2)
template <typename T>
Sort_stats insertion_sort(vector<T> &v) {

    // set up information 
    ulong comparisons = 0; 
    clock_t start = clock();

    // sort information
	for (int i = 1; i < v.size(); i++) {
        
        // to insert, 0th always sorted 
        T key = v[i]; 

		// j points to one position before the insertion point of key 
        // find first value less than or equal to key or start, search backwards 
        int j = i-1; 
        while (j >= 0) {
            // if greater than key, shift element 1 index up leaving hole behind 
            if (v[j] > key) v[j + 1] = v[j];
            else break; 
            j--; 
            comparisons++; 
        }
        // assign key value to empty spot or start (j = -1)
        v[j + 1] = key;  
    }
     
    // end information
    clock_t end = clock();
    double time = double(end-start) / CLOCKS_PER_SEC;

    return Sort_stats{"Insertion Sort", v.size(), comparisons, time};
}

// selection sort described in lecture, O(n^2)
template <typename T>
Sort_stats selection_sort(vector<T> &v) {

    // set up information 
    ulong comparisons = 0; 
    clock_t start = clock();

    // sort information
    // outer loop never reaches end 
    for(int i = 0; i < v.size()-1; i++) {
        int minimum = i;
      
        // inner loop, never reaches start 
        for(int j = i + 1; j < v.size(); j++) {
            // find minimum 
            comparisons++; 
            if (v[j] < v[minimum]) minimum = j; 
        }

        // swap minimum to front 
        if (minimum != i) swap(v[i], v[minimum]);

        // i = 0 finds 1st minimum, i = 2 finds second minimum 
    }
     
    // end information
    clock_t end = clock();
    double time = double(end-start) / CLOCKS_PER_SEC;

    return Sort_stats{"Selection Sort", v.size(), comparisons, time};
}

template <typename T>
Sort_stats shell_sort(vector<T> &v) {
    // set up information 
    ulong comparisons = 0; 
    clock_t start = clock();

    // sort information 
    for (int gap = v.size()/2; gap > 0; gap /= 2) {
        // gapped insertion sort 
	    for (int i = gap; i < v.size(); i++) {
        
            // value to inesert
            T key = v[i]; 
            
            // compare with all indexes gaps away
            // move everything forward until spot for key found  
            int j = i; 
            while (j >= gap && v[j - gap] > key) {
                comparisons++; 
                v[j] = v[j - gap]; 
                j = j - gap;
            }

            // key in correct position 
            v[j] = key; 
        }
    }

    // end information
    clock_t end = clock();
    double time = double(end-start) / CLOCKS_PER_SEC;
    return Sort_stats{"Shell Sort", v.size(), comparisons, time};
}


// mergesort as in sort.cpp lecture, O(nlogn)
// helper function quicksort to count comparisons and call recursively together 
template <typename T>
ulong merge_sort_implement(vector<T> &v) {
    // base case 1 element left 
    if (v.size() <= 1)  return 1;

    // recursively call mergesort on both sides of the split
    int mid = v.size() / 2;
    vector<T> v1(v.begin(), v.begin() + mid);
    vector<T> v2(v.begin() + mid, v.end());
    merge_sort(v1);
    merge_sort(v2);

    // merge back together 
    // note both v1 and v2 are already in order within themselves 
    v.clear(); 
    int i = 0;
    int j = 0;
    ulong comparisons = 0; 

    // while v1 or v2 not all added to vector yet 
    while (i < v1.size() && j < v2.size()) {
        comparisons++; 
        // if v1 value smaller, add value and increment v1 index
        if (v1[i] < v2[j]) {
            v.push_back(v1[i]);
            i++;
        }
        // if v2 value smaller or equal value add and increment v2 index
        else {
            v.push_back(v2[j]);
            j++;
        }
    }
    // if v2 all added, rest of v2 can be added (already in order)
    while (i < v1.size()) {
        comparisons++; 
        v.push_back(v1[i]);
        i++;
    }
    // if v1 all added, rest of v2 can be added (already in order)
    while (j < v2.size()) {
        comparisons++; 
        v.push_back(v2[j]);
        j++;
    }

    return comparisons; 
}

template <typename T>
Sort_stats merge_sort(vector<T> &v) {
    // set up information 
    ulong comparisons = 0; 
    clock_t start = clock();

    // sort information
    comparisons = merge_sort_implement(v);

    // end information 
    clock_t end = clock();
    double time = double(end-start) / CLOCKS_PER_SEC;

    return Sort_stats{"Merge Sort", v.size(), comparisons, time};
}


// quick sort as described in sort.cpp lecture, O(n^2) 
// helper function quicksort to count comparisons and call recursively together 
template <typename T>
ulong quicksort_implement (vector<T> &v, int begin, int end, bool isort) {
    
    ulong comparisons = 0; 
    
    // base case 1 element left
    if (begin >= end) return 1; 

    // set up iquicksort (selection sort)
    if (isort && v.size() == 10) {
        // sort information
        for (int i = 0; i < v.size(); i++) {
            
            // to insert, 0th always sorted 
            T key = v[i]; 

            // j points to one position before the insertion point of key 
            // find first value less than or equal to key or start, search backwards 
            int j = i -1; 
            while (j >= 0) {
                // if greater than key, shift element 1 index up leaving hole behind 
                comparisons++; 
                if (v[j] > key) v[j + 1] = v[j];
                else break; 
                j--; 
            }

            // assign key value to empty spot or start (j = -1)
            v[j + 1] = key;  
        }
        return comparisons; 
    }

    // set up partition 
    T pivot = v[end]; 
    int i = begin; 
    
    // compare each value to pivot value, swap all smaller to end of lesser side of partition 
    for (int j = begin; j < end; j++) {
        comparisons++; 
        if (v[j] < pivot) {
            swap(v[i], v[j]);
            i++;
        }
    }
    // add pivot value to end of lesser side, i index of pivot value
    swap(v[i], v[end]);
 
    // recursively call quicksort on both sides of the partition 
    comparisons += quicksort_implement(v, begin, i - 1, isort);
    comparisons += quicksort_implement(v, i + 1, end, isort);

    return comparisons; 
}

template <typename T>
Sort_stats quick_sort(vector<T> &v) {
    // set up information 
    ulong comparisons = 0; 
    clock_t start = clock();

    // sort information
    comparisons = quicksort_implement(v, 0, v.size()-1, false);

    // end information 
    clock_t end = clock();
    double time = double(end-start) / CLOCKS_PER_SEC;

    return Sort_stats{"Quick Sort", v.size(), comparisons, time};
}

// iquick sort as described in assignment
template <typename T>
Sort_stats iquick_sort(vector<T> &v) {
    // set up information 
    ulong comparisons = 0; 
    clock_t start = clock();

    // sort information
    comparisons = quicksort_implement(v, 0, v.size()-1, true);

    // end information 
    clock_t end = clock();
    double time = double(end-start) / CLOCKS_PER_SEC;

    return Sort_stats{"IQuick Sort", v.size(), comparisons, time};
}

template <typename T>
class Priority_queue_heap {
    vector<T> v;
    ulong var_comparisons = 0; 

public:

    Priority_queue_heap(vector<T> copy) {
        v = copy; 
        v.clear(); 
    }
    int size() const {
        return v.size();
    }
    ulong comparisons() {
        return var_comparisons; 
    }

    void insert(const T &x)  {
        v.push_back(x);
        int i = size() - 1;
        while (i > 0 && v[i] < v[(i - 1) / 2]) {
            var_comparisons++; 
            swap(v[i], v[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    const T &min() const {
        return v[0];
    }

    void remove_min() {
        v[0] = v.back();
        v.pop_back();
        int i = 0;
        while (2 * i + 1 < size()) {
            var_comparisons++; 
            int j = 2 * i + 1;
            if (j + 1 < size() && v[j + 1] < v[j]) j++; 
            if (v[i] <= v[j]) break;
            swap(v[i], v[j]);
            i = j;
        }
    }
}; 

template <typename T>
Sort_stats priority_queue_sort(vector<T> &v) {
    // set up information 
    ulong comparisons = 0; 
    clock_t start = clock();

    // sort information
    Priority_queue_heap heap_vector(v); 
    for (int i  = 0; i < v.size(); i++) {
        heap_vector.insert(v[i]); 
    }
    v.clear(); 
    while (heap_vector.size() > 0) {
        v.push_back(heap_vector.min());
        heap_vector.remove_min();  
    }
    comparisons = heap_vector.comparisons(); 
    
    // end information 
    clock_t end = clock();
    double time = double(end-start) / CLOCKS_PER_SEC;

    return Sort_stats{"Priority Queue Sort", v.size(), comparisons, time};
}