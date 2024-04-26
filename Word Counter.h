// Wordlist.h
#pragma once

// Student Information 
// Name : Anureet Grewal 
// St.# : 301554326 
// Email: anureet_grewal@sfu.ca

// Statement of Originality
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.


// References
// used https://cplusplus.com to search up specific built in c++ functions 
// used https://www.geeksforgeeks.org/bubble-sort/ to help recall bubble sort 


#include "Wordlist_base.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <string>
using namespace std; 

class Wordlist : public Wordlist_base {

    struct Node {
        string word;
        Node *next;
        Node *prev;
    };

    // information to keep track of 
    Node* starting = nullptr; 
    Node* ending = nullptr; 
    bool bool_frozen = 0; 
    int int_length = 0; 

public: 
    // default constructor 
    Wordlist() 
    {}

    // copy constructor 
    Wordlist(const Wordlist& other) {

        // edge cass
        if (other.length() == 0) Wordlist();  

        // loop case 
        for (int i = 0; i < other.length(); i++) {
            add_word (other.get_word(i)); 
        }

        // same frozen 
        bool_frozen = other.bool_frozen; 
    }

    // file constructor 
    Wordlist(const string& filename) {

        // open file 
        ifstream file (filename);
        if (file.fail()) throw runtime_error("unknown file name in constructor"); 

        // read in data 
        string word; 
        while (true) {

            // add words using add_word function; 
            file >> word; 
            add_word (word); 

            // check for end of data 
            if (file.fail())  break; 
        }

        file.close();
    }

    // deconstructor 
    ~Wordlist() {

        // edge case 
        if (length() == 0) return; 
        else if (length() == 1) delete starting; 

        // loop case, end to start 
        else {
            for (Node* p = ending -> prev; p != nullptr; p = p -> prev) {
                delete p -> next; 
            } 
            delete starting; 
        }

        // remove dangling pointers 
        starting = nullptr; 
        ending = nullptr; 
    }

    bool is_frozen() const {
        return bool_frozen; 
    }

    bool contains (const string &w) const {

        // edge case 
        if (length() == 0) return false; 

        // loop case, start to end 
        for (Node* p = starting; p != nullptr; p = p -> next) {
            if (p -> word == w) return true; 
        }
        return false; 
    }
    
    int length() const {
        return int_length; 
    }

    string get_word(int index) const {

        // edge cases 
        if (!(0 <= index)) throw std::runtime_error("called out of bounds index"); 
        if (!(index < length())) throw std::runtime_error("called out of bounds index");

        // loop case, start to end 
        Node* p = starting; 
        for (int i = 0; i < index; i++) p = p -> next; 
        return p -> word; 
    }

    void add_word(const string &w) {

        // edge cases 
        if (is_frozen()) throw std::runtime_error("cannot change frozen list");
        if (contains(w)) return;  

        // if empty  
        if (length() == 0) {
            Node* p = new Node(); 
            p -> word = w; 
            p -> prev = nullptr; 
            p -> next = nullptr; 
            int_length++;
            starting = p; 
            ending = p; 
        }

        // if not empty 
        else {
            Node* p = ending; 
            p -> next = new Node(); 
            p -> next -> word = w; 
            p -> next -> next = nullptr; 
            p -> next -> prev = p; 
            int_length++; 
            ending = p -> next; 
        }
    }

    void remove_word(const string &w) {

        // edge cases
        if (is_frozen()) throw std::runtime_error("cannot change frozen list");
        if (length() == 0) return; 

        // check if in Wordlist 
        if (!contains(w)) return;  

        // if only one word 
        if (length() == 1) {
            delete starting; 
            starting = nullptr; 
            ending = nullptr; 
            int_length = 0; 
            return; 
        }   

        // else find word, start to end 
        Node* p = starting; 
        while (p -> word != w) {
            p = p -> next; 
        }

        // remove word according to position 
        if (p == starting) {      
            Node* next_node = p -> next;
            next_node -> prev = nullptr; 
            delete p; 
            starting = next_node; 
            int_length--; 
        }
        else if (p == ending) {
            Node* prev_node = p -> prev; 
            prev_node -> next = nullptr;  
            delete p; 
            ending = prev_node;
            int_length--; 
        }
        else {
            Node* prev_node = p -> prev; 
            Node* next_node = p -> next; 
            prev_node -> next = next_node; 
            next_node -> prev = prev_node; 
            delete p; 
            int_length--; 
        }
    }

    // helper function for get_sorted_index()
    // checks if string a is alphabetically infront of string b 
    bool alphabetical_order (string a, string b) {
        for (int i = 0; i < a.length() && i < b.length(); i++) {
            if (a[i] < b[i]) return true; 
            if (a[i] > b[i]) return false;  
        }
        
        if (a.length() > b.length()) return false; 
        else return true; 
    }

    vector<string*> get_sorted_index() {

        // set up 
        bool_frozen = 1;
        vector<string*> sorted; 

        // edge case 
        if (length() == 0) return sorted; 
        
        // collect all pointers in vector, not alphabetically
        for (Node* p = starting; p != nullptr; p = p -> next) {
            sorted.push_back(&(p->word)); 
        }

        // bubble sort from start to end indicies
        // reference https://www.geeksforgeeks.org/bubble-sort/
        // sorts till - 1 since bubble sort compares index to the one directly next to it 
        for (int i = 0; i < sorted.size()-1; i++) {
            bool order = true; 
            
            // sort from start to indicies left to sort 
            for (int j = 0; j < sorted.size()-i- 1; j++) {

                // if in order continue to new next index 
                if (alphabetical_order(*sorted[j], *sorted[j+1])) 
                    continue;

                // if not in order swap indecies 
                else {
                    swap (sorted[j], sorted[j+1]); 
                    order = false; 
                }
            }

            // the alphabetically last value is swapped to the back of the list after the j loop
            // thus the next j-loop can ignore that value and continue on bubble sorting 
            // so that the alphabetically second last value is swapped into place 
            // bubble sort continues swapping until all values are in sorted order 
            
            // break case when the entire list is in order 
            if (order) break; 
        }
        return sorted; 
    }
};