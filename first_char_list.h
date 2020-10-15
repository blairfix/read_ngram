//File: first_char_list.h
#ifndef FIRST_CHAR_LIST_H
#define FIRST_CHAR_LIST_H
#include <string>
#include <vector>


void first_char_list( const std::vector<std::string> & wordlist,
                      int nchar,
                      std::vector<std::string> &char_vec,
                      std::vector<int> &counter_vec
                    )
{

    int n_words = wordlist.size();

    std::string old = "";

    for(int i = 0; i < n_words; ++i){

        std::string word = wordlist[i];
        std::string first_chars = word.substr(0, nchar);

        if(first_chars != old){
            char_vec.push_back (first_chars);
            counter_vec.push_back (i);
        }

        old = first_chars;
    }
}

#endif
