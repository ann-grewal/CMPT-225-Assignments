// Stringlist.h

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

#pragma once
#include <cassert>
#include <iostream>
#include <string>
using namespace std;

// linked undo list 
class undo_list {

    struct step {

        // information on steps to undo
        string step_method = ""; 
        string step_value = ""; 
        int step_index = -1; 
        int step_repeated = 0; 
        int step_cap = 0;

        // double linked 
        step* prev = nullptr; 
        step* next = nullptr; 
    }; 

    step* starting = nullptr; 
    step* ending = nullptr; 
    int size = 0; 
    
public:

    // default constructor
    undo_list () {}

    // add step to top of stack (end)
    void add (string method, string value, int index, int repeated, int cap) {

        // set up step 
        step* p = new step();
        p->step_method = method;
        p->step_value = value;
        p->step_index = index; 
        p->step_repeated = repeated;
        p->step_cap = cap;


        // edge case 
        if (size == 0) {
            starting = p; 
            ending = p; 
        }

        // regular case 
        else {
            p->prev = ending; 
            ending->next = p; 
            ending = p; 
        }

        // update size
        size++; 
    }

    // remove step from top of stack (end)
    void remove_step () {

        // edge cases
        if (size == 0) return; 
        else if (size == 1) {
            delete ending; 
            starting = nullptr; 
            ending = nullptr; 
        }
        // regular case 
        else {
            step* p = ending->prev; 
            p->next = nullptr;    
            delete ending;    
            ending = p; 
        }

        // update size 
        size--; 
    }

    // return information from the top of the stack
    string undo_method() {
        return (ending -> step_method); 
    }  
    string undo_value() {
        return (ending -> step_value); 
    }
    int undo_index() {
        return (ending -> step_index); 
    }
    int undo_repeated(){
        return (ending -> step_repeated); 
    } 
    int undo_cap(){
        return (ending -> step_cap); 
    } 

    // checks if empty 
    bool empty() {
        if (size == 0) return true; 
        else return false; 
    }

    // destructor 
    ~undo_list() {
        for (int i = size; i > 0; i--)  remove_step(); 
    }
}; 

class Stringlist {

    int cap;     
    string *arr; 
    int sz;
    undo_list undo_stack; 

    // bounds checking 
    void bounds_error(const string &s) const {
        throw out_of_range("Stringlist::" + s + " index out of bounds");
    }
    void check_bounds(const string &s, int i) const {
        if (i < 0 || i >= sz) {
            bounds_error(s);
        }
    }

    // copy array 
    void copy(const string *other) {
        for (int i = 0; i < sz; i++) arr[i] = other[i];
    }

    // checking capacity, doubles size of the underlying array if necessary.
    void check_capacity() {
        if (sz == cap) {
            cap *= 2;
            string *temp = new string[cap];
            for (int i = 0; i < sz; i++) temp[i] = arr[i];
            delete[] arr;
            arr = temp;
        }
    }

public:

    // default constructor 
    Stringlist()
    : cap(10), arr(new string[cap]), sz(0), undo_stack()
    {}

    // copy constructor, does not copy any undo information
    Stringlist(const Stringlist &other)
    : cap(other.cap), arr(new string[cap]), sz(other.sz) { 
        copy(other.arr); 
    }

    // destructor 
    ~Stringlist() {
        delete[] arr;
    }

    // assignment operator, makes a copy of the given StringList undoable
    // undo stack not copied, lst1 retains its stack but adds assignement 
    Stringlist &operator=(const Stringlist &other) {
        
        // check for self assignment 
        if (this != &other) {

            // save information 
            int sz_original = sz; 
            int cap_orignal = cap; 

            // remove information 
            for (int i = 0; i < sz_original; i++) remove_at(0); 
            delete[] arr;

            // copy information 
            cap = other.capacity();
            arr = new string[cap];
            sz = other.size();
            copy(other.arr);

            // add to undo_list 
            undo_stack.add("ASSIGN", "", 0, sz_original, cap_orignal); 
        }
        return *this;
    }

    // call information, constant 
    int size() const { return sz; }
    bool empty() const { return size() == 0; }
    int capacity() const { return cap; }

    string get(int index) const {
        check_bounds("get", index);
        return arr[index];
    }

    int index_of(const string &s) const {
        for (int i = 0; i < sz; i++) {
            if (arr[i] == s) { return i; }
        }
        return -1;
    }

    bool contains(const string &s) const {
        return index_of(s) != -1;
    }

    string to_string() const {
        string result = "{";
        for (int i = 0; i < size(); i++) {
            if (i > 0)  result += ", ";
            result += "\"" + get(i) + "\"";
        }
        return result + "}";
    }

    // sets the string at the given index
    // undoable by its self 
    void set(int index, string value) {

        check_bounds("set", index);
        string not_set = arr[index]; 
        arr[index] = value;

        // add to undo_list 
        undo_stack.add("SET", not_set, index, 0, 0); 
    }


    // insert s before index
    // undoable by remove_at
    void insert_before(int index, const string &s) {

        // allows insert at end, i == sz
        if (index < 0 || index > sz){ 
            bounds_error("insert_before");
        }

        // increase capacity if needed
        check_capacity();

        // insert s 
        for (int i = sz; i > index; i--) {
            arr[i] = arr[i - 1];
        }
        arr[index] = s;

        // change size 
        sz++;

        // add to undo_list
        undo_stack.add("INSERT", "", index, 0, 0); 
    }

    // insert at back
    // undoable by remove_at
    void insert_back(const string &s) {
        insert_before(size(), s);
    }

    // inserts s at front
    // undoable by remove_first 
    void insert_front(const string &s) {
        insert_before(0, s);
    }

    // removes the string at the given index, no capacity change
    // undoable by insert_before 
    void remove_at(int index) {

        check_bounds("remove_at", index);
        string remove_value = arr[index]; 
        for (int i = index; i < sz - 1; i++) {
            arr[i] = arr[i + 1];
        }

        // change size 
        sz--;

        // add to undo_list
        undo_stack.add("REMOVE", remove_value, index, 0, 0); 
    }

    // removes all strings from the list, no capacity change
    void remove_all() {
        int sz_original = sz; 
        while (sz > 0) remove_at(sz - 1);
        undo_stack.add("REMOVE_ALL", "", 0, sz_original, 0);
    }

    // removes the first occurrence of s in the list
    // returns true if s removed else if no s return false 
    bool remove_first(const string &s) {
        int index = index_of(s);
        if (index == -1) return false;
        remove_at(index);
        return true;
    }

    // undoes the last operation that modified the list
    // returns true if change undone else return false (nothing to change)
    bool undo() {

        // nothing on stack 
        if (undo_stack.empty()) {
            return false; 
        }

        // get top of stack 
        string method = undo_stack.undo_method(); 
        string value = undo_stack.undo_value(); 
        int index = undo_stack.undo_index(); 
        int repeated = undo_stack.undo_repeated(); 
        int capacity = undo_stack.undo_cap(); 
        undo_stack.remove_step(); 

        // find matching method undo
        if (method == "SET") {
            arr[index] = value;
        } 
        if (method =="INSERT") {
            remove_at(index); 
            undo_stack.remove_step();   
        }
        if (method == "REMOVE") {
            insert_before(index, value);
            undo_stack.remove_step(); 
        }
        if (method == "ASSIGN") {

            // delete old array, make new array (keep capacity)
            delete[] arr; 
            arr = new string[capacity];
            sz = 0; 

            // using logged remove statements 
            for (int i = 0; i < repeated; i++) undo(); 
        }
        if (method == "REMOVE_ALL") {
            // using logged remove statements 
            for (int i = 0; i < repeated; i++) undo();  
        } 
        return true; 
    }

}; 

// prints list to in the format {"a", "b", "c"}.
ostream &operator<<(ostream &os, const Stringlist &lst) {
    return os << lst.to_string();
}


// returns true if the two lists are equal, undo information not considered 
bool operator==(const Stringlist &a, const Stringlist &b) {
    if (a.size() != b.size()) return false;
    for (int i = 0; i < a.size(); i++) {
        if (a.get(i) != b.get(i)) return false;
    }
    return true;
}

// returns true if the two lists are not equal, undo information not considered 
bool operator!=(const Stringlist &a, const Stringlist &b) {
    return !(a == b);
}