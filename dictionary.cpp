#include <cstdint>
#include <iostream>
#include <evhttp.h>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <math.h>
#include "dictionary.h"
using namespace std;
using namespace chrono;

/**************************************************************
* @brief   Init the dictionary database (unordered_multimap). *
*          Open input file and insert each word to the map.   *
*          Key is the sortted word and the value is the word  *
*          itself. unordered_multimap allow multiple values   *
*          for the same key.                                  *
* @param                                                      * 
* @return  Return TRUE if init succeeded. FALSE otherwise.    *
**************************************************************/ 
bool Dictionary::init_dictionary_db()
{
    string word, sort_word;
    ifstream file(INPUT_FILE);
    if (file.is_open()) {
        while (file >> word) {
            sort_word = word;
            sort(sort_word.begin(), sort_word.end());
            ummap.insert({sort_word, word});
        }
        file.close();
    } else {
        cerr << "Not able to open input file: " << INPUT_FILE << endl;
        return false;
    }
    return true;
}

/**************************************************************
* @brief   Create a json format string with all similar words *
*          (permutations of word).                            *
*          Look for all entries in map with the same key -    *
*          the sortted word.                                  *
*          Function also calculate the number of total        *
*          Requests and avarage request processing time       *
* @param   String (word)                                      *
* @return  String in json format contains all words           *
*          permutations.                                      *
* @note    Return permutations of word even if word is not in *
*          dictionary.                                        * 
**************************************************************/
string Dictionary::get_permutations(string word)
{
    int process_time;
    string result, sort_word = word;
    /* Get start time */
    auto start = high_resolution_clock::now();
    total_requests++;
    /* Key is the sortted word */ 
    sort(sort_word.begin(), sort_word.end());

    result += "{\"similar\":[";
    /* Find all vaules with the same key */
    auto its = ummap.equal_range(sort_word);
    for (auto it = its.first; it != its.second; it++) {
        if(word != it->second) {
            result += "\""+it->second+"\",";
        }
    }
    /* Remove ending comma */
    if (result[result.size()-1] ==',') {
        result.pop_back();
    }
    result += "]}";

    /* Get finish time */
    auto finish = high_resolution_clock::now();
    process_time = duration_cast<std::chrono::nanoseconds>(finish-start).count();
    /* Calculate avarage process time in a way that will prevent integer overflow */ 
    avg_time = ((avg_time/sqrt(total_requests)) * ((total_requests-1)/sqrt(total_requests)) + (process_time/total_requests));

    return result;
}

/**************************************************************
* @brief   Create a json format string with the following     *
*          information: Total requests number, Total word in  *
*          dictionary and avarage request process time.       *
* @param                                                      *
* @return  String in json format contains all the above info. *
**************************************************************/
string Dictionary::get_dictionary_state()
{
        string result =
        "{\"totalWords\":" + to_string(ummap.size()) +
        ",\"totalRequests\":" + to_string(total_requests) +
        ",\"avgProcessingTimeNs\":" + to_string(avg_time) + "}";
        return result;
}
