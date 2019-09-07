#ifndef DICTIONARY_H // include guard
#define DICTIONARY_H

#include<bits/stdc++.h>
#include <string>
using namespace std;

#define INPUT_FILE "words_clean.txt"

class Dictionary {
    private:
        unordered_multimap<string, string> ummap;
        int total_requests = 0;
        int avg_time = 0;
    public:
        bool init_dictionary_db();
        string get_dictionary_state();
        string get_permutations(string word);
};
#endif /* DICTIONARY_H */
