/*
Author: Anton Goretsky
Course: CSCI-135
Instructor: Professor Marayash
Assignment: Project 2

Input: Word
Output: Pronounciation, 
Words with an extra phoneme, 
words with one less phoneme, 
and words with one substituted phoneme.
*/

#include <iostream>
#include <fstream>
#include <string>

//to not use using namespace std
using std::cout;
using std::cin;
using std::string;
using std::ifstream;
using std::endl;

//pre declarations
int valid(string s);
void splitOnSpace(string s, string &before, string &after);
int add_phoneme(string &or_phonemes, string &cur_phonemes);
int remove_phoneme(string &or_phonemes, string &cur_phonemes);
int replace_phoneme(string &or_phonemes, string &cur_phonemes);
int countChar(string line, char c);
string own_substring(const string& s, int start, int end);
int countPhonemes(const string& phonemes);

//main method here
int main() {
    //pull word from cin
    string word = "";
    cin >> word;

    //check word for non-alpha characters and non-apostraphe characters.
    if (!valid(word)) {
        cout << "non valid in non alpha" << endl;
        cout << "Not found" << endl;
        return 0;
    }

    //upper the word
    string upperword = "";
    for (int i = 0; i < (int) word.size(); i++) {
        upperword += std::toupper(word[i]);
    }
    word = upperword;

    //opening file
    string line = "";
    string first_word = "";
    string phonemes = "";
    ifstream file("cmudict.0.7a");
    int found = 0;
    
    //check if word exists in dictionary.
    while (getline(file, line)) {
        //Skip improperly formatted lines
        if (!isalpha(line[0]) && line[0] != '\'') {
            continue;
        }
        splitOnSpace(line, first_word, phonemes);
        //also skip invalid words in dict
        if (valid(first_word)) {
            if (upperword.compare(first_word) == 0) {
                found = 1;
                break;
            }
        }
    }
    if (!found) {
        cout << "Not found" << endl;
        return 0;
    }

    //reset file pointer to the beginning
    file.seekg(0, std::ios::beg);

    cout << "Pronunciation    :" << phonemes << endl;

    string identical = "";
    string add_phoneme_list = "";
    string remove_phoneme_list = "";
    string replace_phoneme_list = "";
    string cur_phonemes = "";

    //do all the functions
    while (getline(file, line)) {
        //skip improperly formatted lines
        if (!isalpha(line[0]) && line[0] != '\'') {
            continue;
        }
        splitOnSpace(line, first_word, cur_phonemes);
        //also skip invalid words in dict
        if (!valid(first_word)) {
            continue;
        }
        //cout << "made it past validity checking" << endl;
        //check if entry had identitcal pronunciation
        if (phonemes.compare(cur_phonemes) == 0 && word != first_word) {
            //cout << "identitcal comparison ran" << endl;
            identical += first_word + " ";
        }
        //check if entry is an add_phoneme possibility
        if (add_phoneme(phonemes, cur_phonemes)) {
            //cout << "add method ran" << endl;
            add_phoneme_list += first_word + " ";
        }
        //Check if entry is a remove_phoneme
        if (remove_phoneme(phonemes, cur_phonemes)) {
            remove_phoneme_list += first_word + " ";
        }
        //Check if entry is a replace phoneme possibility
        if (replace_phoneme(phonemes, cur_phonemes)) {
            replace_phoneme_list += first_word + " ";
        }
    }

    cout << endl;
    cout << "Identical        : " << identical << endl;
    cout << "Add phoneme      : " << add_phoneme_list << endl;
    cout << "Remove phoneme   : " << remove_phoneme_list << endl;
    cout << "Replace phoneme  : " << replace_phoneme_list << endl;
    return 0;
}

//checks until mismatch, then checks if remains match without the extra phoneme
int add_phoneme(string &or_phonemes, string &cur_phonemes) {
    //cout << "add phoneme is running" << endl;
    //Check amount of phonemes in current entry is one greater than original.
    if (countChar(or_phonemes, ' ') + 1 != countChar(cur_phonemes, ' ')) {
        return 0;
    }
    
    //Compare phoneme by phoneme until mismatch
    //Remove mismatched and check if remains works.
    //cout << "got after count char comparison" << endl;
    int last_space = 0;
    for (int i = 0; i < (int) or_phonemes.size(); i++) {
        //cout << "i: " << i << endl;
        //update last space to actual last space
        if (or_phonemes[i] == ' ') {
            last_space = i;
        }
        //mismatch is hit
        if (or_phonemes[i] != cur_phonemes[i]) {
            int ctr = i;
            do (ctr++);
                while (cur_phonemes[ctr] != ' ');
            
            //compare remains.
            int result = or_phonemes.substr(last_space, or_phonemes.size()).compare(
            cur_phonemes.substr(ctr, cur_phonemes.size()));
            if (result == 0)
                return 1;
            else
                return 0;
            
        }
    }
    return 1;
}

//own substring method due to exceptions from standard
string own_substring(const string& s, int start, int end)
{
    string res = "";
    for (int i = start; i <= end; ++i)
        res += s[i];
    return res;
}

//exactly the same as add_phoneme just with or_phoneme and cur_phoneme switching places
int remove_phoneme(string &or_phonemes, string &cur_phonemes) {
    //cout << "entering remove phoneme" << endl;
     //Check amount of phonemes in original entry is one greater than current.
    if (countChar(or_phonemes, ' ') != countChar(cur_phonemes, ' ') + 1) {
        return 0;
    } 

    //Compare phoneme by phoneme until mismatch
    //Remove mismatched and check if remains works.
    int last_space = 0;
    for (int i = 0; i < (int) cur_phonemes.size(); i++) {
        //update last space to actual last space
        if (cur_phonemes[i] == ' ') {
            last_space = i;
        }
        //mismatch is hit
        if (cur_phonemes[i] != or_phonemes[i]) {
            int ctr = i;
            do (ctr++);
                while (or_phonemes[ctr] != ' ');
            
            //compare remains.
            int result = cur_phonemes.substr(last_space, cur_phonemes.size()).compare(
            or_phonemes.substr(ctr, or_phonemes.size()));
            if (result == 0)
                return 1;
            else
                return 0;
        }
    }
    return 1;
}

//works same as add and replace, just different comparison, and run through both to check remains after replaced
int replace_phoneme(string &or_phonemes, string &cur_phonemes) {
    //cout << "entering replace phoneme" << endl;
    //Check amount of phonemes in current entry is the same as original.
    if (countChar(or_phonemes, ' ') != countChar(cur_phonemes, ' ')) {
        return 0;
    }
    //cout << "got after comparison" << endl;
    //Compare phoneme by phoneme until mismatch
    //Remove mismatched and check if remains works.

    //Set iter to size of small string
    int iter = or_phonemes.size();
    if (or_phonemes.size() < cur_phonemes.size()) {
        iter = or_phonemes.size();
    }
    else {
        iter = cur_phonemes.size();
    }
    //cout << "size in comparison: " << iter << endl;
    int last_space = 0;
    for (int i = 0; i < iter; i++) {
        //update last space to actual last space
        if (or_phonemes[i] == ' ') {
            last_space = i;
        }
        //mismatch is hit
        if (or_phonemes[i] != cur_phonemes[i]) {
            int or_ctr = i;
            int cur_ctr = i;
            do (cur_ctr++);
                while (cur_phonemes[cur_ctr] != ' ');
            do (or_ctr++);
                while (or_phonemes[or_ctr] != ' ');
            
            //compare remains.
            return (own_substring(or_phonemes, or_ctr, or_phonemes.size()) ==
                    own_substring(cur_phonemes, cur_ctr, cur_phonemes.size()));
            /*
            
            int result = or_phonemes.substr(or_ctr, or_phonemes.size()).compare(
            cur_phonemes.substr(cur_ctr, cur_phonemes.size()));
            if (result == 0)
                return 1;
            else
                return 0;
            */
        }
    }
    return 0;
}

//count char method
int countChar(string line, char c) {
    //cout << "entering count char" << endl;
    int ctr = 0;
    for (int i = 0; i < (int) line.size(); i++) {
        if (line[i] == c) {
            ctr++;
        }
    }
    //cout << "exiting count char" << endl;
    return ctr;
}

//validity checking
int valid(string word) {
    for (int i = 0; i < (int) word.size(); i++) {
        if (!isalpha(word[i]) && word[i] != '\'') {
            return 0;
        }
    }
    return 1;
}

//split on space provided method
void splitOnSpace(string s, string &before, string &after) {
    before = "";
    after = "";
    int i = 0;
    while (i < s.size() && !isspace(s[i])) {
        before += s[i];
        i++;
    }
    i++;
    while (i < s.size()) {
        after += s[i];
        i++;
    }
}

