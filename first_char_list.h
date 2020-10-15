//File: first_char_list.h
#ifndef FIRST_CHAR_LIST_H
#define FIRST_CHAR_LIST_H
#include <string>
#include <vector>

using namespace std;

void first_char_list( const vector<string> & wordlist,
                      int nchar,
                      vector<string> &char_vec,
                      vector<int> &counter_vec
                    )
{

    int n_words = wordlist.size();

    string old = "";

    for(int i = 0; i < n_words; ++i){

        string word = wordlist[i];
        string first_chars = word.substr(0, nchar);

        if(first_chars != old){
            char_vec.push_back (first_chars);
            counter_vec.push_back (i);
        }

        old = first_chars;
    }
}

#endif
