#include <Rcpp.h>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>
#include "first_char_list.h"

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::export]]

DataFrame read_ngram(std::string filename,
                     std::vector<std::string> wordlist
                    )
{

    // test if file exists
    std::ifstream ifile;
    ifile.open(filename);
    if(ifile) {
    } else {
        throw std::range_error("File does not exist");
    }


    // number of words in list
    int n_words = wordlist.size();


    // get std::vector of first 2 chars (speeds up searching)
    std::vector<std::string> char_vec;
    std::vector<int> counter_vec;
    int nchar = 2;

    first_char_list(wordlist,
                    nchar,
                    char_vec,
                    counter_vec );


    // read in ngram data
    // ****************************************************
    int global_counter = 0;

    // output vectors
    int file_guess = 50000000;

    std::vector<std::string> word_vec(file_guess);
    Rcpp::NumericVector year_vec(file_guess);
    Rcpp::NumericVector wordcount_vec(file_guess);

    // read in ngram
    std::string ngram;
    std::ifstream in(filename);

    while (getline(in, ngram)) {

        // find first tab
        std::string tab = "\t";
        size_t id_tab = ngram.find(tab);

        // get word
        if( id_tab != std::string::npos ){

            std::string word = ngram.substr(0, id_tab);

            // convert word to lower case
            boost::to_lower(word);


            // test if first 2 chars in character list, and get index
            std::string char_2 = word.substr(0, nchar);


            auto it = find(char_vec.begin(), char_vec.end(), char_2);


            if (it != char_vec.end()) {

                int index_start = distance(char_vec.begin(), it);
                int index_end = index_start + 1;

                int search_start = counter_vec[index_start];
                int search_end = counter_vec[index_end];


                // if last entry in countervec
                if( index_start == counter_vec.size() - 1 ){
                    search_end = wordlist.size() - 1;
                }

                // search for word
                int test = 1;
                int i = search_start;

                while(i <= search_end && test != 0){

                    test = word.compare( wordlist[i] );
                    i++;

                 }

                // if found
                if(test == 0){


                    // erase word from ngram
                    ngram.erase(0, id_tab + tab.length());

                    // split on tab
                    std::string delimiter = "\t";
                    size_t pos = 0;
                    std::string subdata;

                    while ((pos = ngram.find(delimiter)) != std::string::npos) {

                        // word
                        word_vec[global_counter] = word;

                        // get year and word count
                        subdata = ngram.substr(0, pos);

                        // year
                        size_t comma1 = subdata.find(",");
                        std::string year = subdata.substr(0, comma1);
                        year_vec[global_counter] = std::stoi(year);

                        // word count
                        subdata.erase(0, comma1 + 1);
                        size_t comma2 = subdata.find(",");
                        std::string wordcount = subdata.substr(0, comma2);
                        wordcount_vec[global_counter] = std::stoi(wordcount);

                        // erase tab
                        ngram.erase(0, pos + delimiter.length());

                        // advance global counter
                        global_counter++;

                    }

                    // get last entry
                    word_vec[global_counter] = word;

                    size_t comma1 = ngram.find(",");
                    std::string year = ngram.substr(0, comma1);
                    year_vec[global_counter] = std::stoi(year);

                    ngram.erase(0, comma1 + 1);
                    size_t comma2 = ngram.find(",");
                    std::string wordcount = ngram.substr(0, comma2);
                    wordcount_vec[global_counter] = std::stoi(wordcount);

                    // advance global counter
                    global_counter++;

                }
            }
        }
    }


    // erase zeros
    word_vec.erase (word_vec.begin() + global_counter, word_vec.end() );
    year_vec = year_vec[year_vec != 0 ];
    wordcount_vec = wordcount_vec[wordcount_vec != 0 ];

    // bind output in dataframe
    DataFrame output = DataFrame::create(
        Named("word") =      word_vec,
        Named("year") =      year_vec,
        Named("wordcount") = wordcount_vec
    );

    return output;

}
