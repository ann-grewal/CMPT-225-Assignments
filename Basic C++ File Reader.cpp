#include <cstdlib>  
#include <iostream>
#include <fstream>  
#include <string> 
#include <vector>

using namespace std; 
// check last 4 characters for .txt 
bool check_textfile (string outfile) {
    if (outfile.size() < 5) return false; 
    else {
        string target = ".txt"; 
        string end = outfile.substr(outfile.size()-4, 4); 
        if (end == target) return true; 
        else return false; 
    }

}

int main() {
    vector <string> words; 

    // open infile 
    string infile_name; 
    ifstream infile (infile_name); 
    while (infile.fail()) {
        cout << "enter valid name of text file to input from: "; 
        cin >> infile_name; 
        infile.open (infile_name); 
    }

    // open outfile 
    string outfile_name; 
    while (!check_textfile(outfile_name))  {
        cout << "enter valid name of text file to output to: "; 
        cin >> outfile_name; 
    }
    ofstream outfile(outfile_name); 
    if (outfile.fail()) cout << "no such file created"; 

    // process infile 
    string word; 
    while (!infile.fail()) {
        infile >> word; 
        words.push_back(word); 
    }
    words.pop_back(); 
    
    // close file 
    infile.close();
    outfile.close(); 
}