# Twistlock Task

A small web service for printing similar words in the English language.
Two words w_1 and w_2 are considered similar if w_1 is a letter permutation of w_2 (e.g., "stressed" and "desserts"). 

The web service should listen on port 8000 and support the following two HTTP endpoints:

GET /api/v1/similar?word=stressed
Returns all words in the dictionary that has the same permutation as the word "stressed". The word in the query should not be returned. 

GET /api/v1/stats
Return general statistics about the program:
Total number of words in the dictionary
Total number of requests (not including "stats" requests)
Average time for request handling in nano seconds (not including "stats" requests)

# Main Algorithem
Server is written in C++ and uses libevent introduced in C++11.
Database is unordered multimap that allow us to find values in O(1).

## Flow:
1. Init dictionary database (unordered multimap that allow multiple values per one key):
    1. Open input file
    2. For each word:
        1. Sort word (key)
        2. Insert word (value) to map using key
2. Create http server to listen on localhost:8000 using libevent
3. For each request comming from socket:
    1. Get URI
    2. If URI is state request, call get_dictionary_state class function that returns json string with reply
    3. If URI is permutation request, call get_permutations class function:
        1. Sort word (key)
        2. For all values with the same key, add to reply string
    4. Send reply to client

## Dictionary class:


class Dictionary {
    
    private:
        
        /* Unordered multimap allow us to keep multiple values per key */
        
        /* Key is a SORTTED word and the values are the real words */
        
        unordered_multimap<string, string> ummap;
        
        /* Keep the total number of permutation requests */
        
        int total_requests = 0;
        
        /* Keep the avarage processing time for a request */
        
        int avg_time = 0;
    
    public:
        
        /* Init the map with input file */
        
        bool init_dictionary_db();
        
        string get_dictionary_state();
        
        string get_permutations(string word);

};


# Build
g++ http_server.cpp dictionary.cpp -levent

Run with ./a.out
