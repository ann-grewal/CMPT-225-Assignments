// a3.cpp

// Student Info
// ------------
//
// Name : Anureet Grewal 
// St.# : 301554326
// Email: anureet_grewal@sfu.ca
//
// Statement of Originality
// ------------------------
//
// All the code and comments below are my own original work. For any non-
// original work, I have provided citations in the comments with enough
// detail so that someone can see the exact source and extent of the
// borrowed work.
//
// In addition, I have not shared this work with anyone else, and I have
// not seen solutions from other students, tutors, websites, books,
// etc.

#include "Announcement.h"
#include "JingleNet_announcer.h"
#include "Queue_base.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string> 
using namespace std;

// based upon queues.cpp from week 4 of cmpt 225 
// still a template class
template <typename T>
class Queue : public Queue_base<T> {
    struct node {
        T info;
        node *next;
        node *prev; 
    };

    node *starting = nullptr; 
    node *ending = nullptr; 
    int int_size = 0;

public:

    void enqueue(const T &x) {
        if (int_size == 0) {
            node* p = new node{x, nullptr, nullptr};
            starting = p; 
            ending = p; 
        }
        else {
            node* p = new node{x, nullptr, ending};
            ending->next = p;
            ending = p;      
        }
        int_size++;
    }

    void dequeue() {
        if (int_size == 0) throw runtime_error("dequeue: queue is empty"); 
        if (int_size == 1) {
            delete starting; 
            starting = nullptr; 
            ending = nullptr; 
            int_size--; 
        }
        // dequeue from front 
        else {
            node* p = starting -> next; 
            delete starting; 
            p -> prev = nullptr; 
            starting = p;       
            int_size--; 
        }   
    }

    const T &front() const {
        if (int_size == 0) throw runtime_error("front: queue is empty"); 
        return starting->info;
    }
    int size() const { 
        return int_size;
    }


    // additional methods 
    T front_pop() {
        T popped = front(); 
        dequeue();
        return popped; 
    }

    // reach anywhere 
    T index_transverse(int index) {
        node* p = starting; 
        for (int i = 0; i < index; i++) {
            p = p -> next; 
        }
        return p -> info; 
    }

    // delete anywhere 
    void index_delete(int index){
        
        node* p = starting; 
        for (int i = 0; i < index; i++) {
            p = p -> next; 
        }
        if (p == starting) dequeue(); 
        else if (p == ending) {
            p = p -> prev; 
            delete ending; 
            int_size--;  
            ending = p; 
        }
        else {
            node* q = p -> next; 
            node* r = p -> prev;
            q -> prev = r;  
            r -> next = q;  
            delete p; 
            int_size--; 
        } 
    }

    ~Queue() {
        while (int_size > 0) dequeue();
    }
}; 

class JingleNet  {
private:
    // store information in strings 
    Queue<string> santa_queue; 
    Queue<string> reindeer_queue; 
    Queue<string> elf2_queue; 
    Queue<string> elf1_queue; 
    Queue<string> snowman_queue; 

public: 
    ~JingleNet(){} 

    // get first word from string 
    string get_first(string s) {
        size_t pos = s.find(' ');
        s = s.substr(0, pos);
        return s; 
    }

    void jingle_announce (int n) {
        
        Queue<string> to_announce; 
        
        // compare to size 
        int total_size = santa_queue.size() + reindeer_queue.size()
                      + elf2_queue.size() + elf1_queue.size() + snowman_queue.size(); 

        if (n < total_size) {
            total_size = total_size - n;    
        }
        else {
            n = total_size; 
            total_size = 0; 
        }

        // clear queue by queue 
        for (int i = 0; i < n; i++){
            string s; 
            string rank; 

            if (santa_queue.size() > 0) {
                s = santa_queue.front_pop(); 
                rank = " santa";
            }
            else if (reindeer_queue.size() > 0) {
                s = reindeer_queue.front_pop(); 
                rank = " reindeer"; 
            }
            else if (elf2_queue.size() > 0) {
                s = elf2_queue.front_pop(); 
                rank = " elf2"; 
            } 
            else if (elf1_queue.size() > 0) {
                s = elf1_queue.front_pop(); 
                rank = " elf1"; 
            }
            else if (snowman_queue.size() > 0) {
                s = snowman_queue.front_pop(); 
                rank = " snowman"; 
            }

            // format to fit Announcement construcor 
            int pos = s.find(' ');
            s.insert(pos, rank);

            // add to temporary queue
            to_announce.enqueue(s);
        }
        

        // flush queue -> construct Announcments -> send to announce 
        while (to_announce.size() > 0) {
            string s = to_announce.front_pop(); 
            Announcement next(s);
            jnet.announce(next); 
        }
    }

    // user manipulations remove all and promote 
    // always deletes any announcment from user from at least 1 queue 
    void jingle_user (string user, bool promote) {

        string current; 

        for (int i = 0; i < santa_queue.size(); i++){ 
            current = santa_queue.index_transverse(i); 
            string user_current = get_first(current);

            if (user_current == user) {
                // do not delete promoted events from top tier 

                if (!promote) {
                    santa_queue.index_delete(i); 
                    i--; 
                }
            } 
        }
        for (int i = 0; i < reindeer_queue.size(); i++){
            current = reindeer_queue.index_transverse(i); 
            string user_current = get_first(current); 

            if (user_current == user) {
                // if promote add to santa queue
                if (promote) santa_queue.enqueue(current); 

                reindeer_queue.index_delete(i); 
                i--; 
            } 
        }
        for (int i = 0; i < elf2_queue.size(); i++){
            current = elf2_queue.index_transverse(i); 
            string user_current = get_first(current); 

            if (user_current == user) {
                // if promote add to reindeer queue
                if (promote) reindeer_queue.enqueue(current);  

                elf2_queue.index_delete(i); 
                i--; 
            } 
        }
        for (int i = 0; i < elf1_queue.size(); i++){
            current = elf1_queue.index_transverse(i); 
            string user_current = get_first(current);

            if (user_current == user) {
                // if promote add to elf 2 queue
                if (promote) elf2_queue.enqueue(current);

                elf1_queue.index_delete(i); 
                i--; 
            } 
        }
        for (int i = 0; i < snowman_queue.size(); i++){
            current = snowman_queue.index_transverse(i); 
            string user_current = get_first(current); 

            if (user_current == user) {
                // if promote add to elf1 queue
                if (promote) elf1_queue.enqueue(current); 

                snowman_queue.index_delete(i); 
                i--; 
            } 
        }
    }
    
    // add information to specified queue
    // queue not saved as part of string 
    void jingle_send (string user, string rank, string text) {
        string item = user + " " + text; 
        if (rank == "santa") santa_queue.enqueue(item); 
        if (rank == "reindeer") reindeer_queue.enqueue(item); 
        if (rank == "elf2") elf2_queue.enqueue(item); 
        if (rank == "elf1") elf1_queue.enqueue(item); 
        if (rank == "snowman") snowman_queue.enqueue(item); 
    }

    void processing (string line) {

        // get command 
        size_t pos = line.find(' ');
        string command = line.substr(0, pos);
        line = line.substr(pos + 1);
       
        // rest of string is n 
        if (command == "ANNOUNCE") {    
            int n = stoi(line); 
            jingle_announce (n); 
            return; 
        }

        // get user 
        pos = line.find(' ');
        string user = line.substr(0, pos); 
        line = line.substr(pos + 1);

        // rest of string is empty 
        if (command == "REMOVE_ALL" ) { 
            jingle_user(user, false); 
            return; 
        }
        if (command == "PROMOTE_ANNOUNCEMENTS") { 
            jingle_user (user, true); 
            return;
        }

        // get rank  
        pos = line.find(' ');
        string rank = line.substr(0, pos); 
        line = line.substr(pos + 1);

        // rest of string is text 
        string text = line;
        
        if (command == "SEND") {
            jingle_send (user, rank, text); 
        } 
    }
}; 

// based off of getline_demo given 
int main(int argument_size, char* argument_programs[] ) {
    
    // set up queue 
    JingleNet christmas_queue; 

    // if one extra file is not given 
    if (argument_size != 2) {
        cout << "run " << argument_programs[0] <<  " with a text file\n"; 
        return 1; 
    }

    // get text file
    ifstream infile (argument_programs[1]); 
    if (infile.fail()) {
        cout << "file " << argument_programs[1] << "failed to open \n"; 
        return 1; 
    } 

    // get all information  
    string current; 
    while (!infile.fail()) {  

        // get word
        string s; 
        infile >> s;  

        // if reach a command keyword (new line)
        if (s == "ANNOUNCE" || s == "SEND" || s == "REMOVE_ALL" || s == "PROMOTE_ANNOUNCEMENTS") {
            if (current.empty()) {
                current = s; 
            } else {
                // flush last complete command in current 
                christmas_queue.processing(current);
                current = s; 
            }
        } else {
            // add to current command 
            current = current + " " + s; 
        }
    }

    // for last valid command 
    if (!current.empty()) {
        christmas_queue.processing(current);
    }

    // close file 
    infile.close(); 
}