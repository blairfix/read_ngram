#include <Rcpp.h>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>

using namespace std;
using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::export]]



DataFrame read_ngram(string filename,
                     const vector<string> & wordlist
                    )
{

    // number of words in list
    int n_words = wordlist.size();



    // get vector of first 2 chars (speeds up searching)
    //****************************************************
    StringVector char_vec;
    NumericVector counter_vec;
    StringVector wordlist_sub;

    string old = "";

    for(int i = 0; i < n_words; ++i){

        string word = wordlist[i];
        string first_chars = word.substr(0, 2);

        if(first_chars != old){
            char_vec.push_back (first_chars);
            counter_vec.push_back (i);
        }

        old = first_chars;
    }



    // read in data
    // ****************************************************
    int global_counter = 0;

    // output vectors
    vector<string> word_vec(50000000);
    NumericVector year_vec(50000000);
    NumericVector wordcount_vec(50000000);

    // read in ngram
    ifstream in(filename);

    string ngram;
    while (getline(in, ngram)) {

        // find first tab
        size_t found = ngram.find("\t");

        // get word
        if( found != std::string::npos ){

            string word = ngram.substr(0, found);

            // convert word to lower case
            boost::to_lower(word);


            // test if first 2 chars in character list, and get index
            string char_2 = word.substr(0, 2);

            auto it = find(char_vec.begin(), char_vec.end(), char_2);

            if (it != char_vec.end()) {

                int index = distance(char_vec.begin(), it);

                int search_start = counter_vec[index];
                int search_end = counter_vec[index + 1];

                // test if end of counter_vec
                if (index == counter_vec.size() - 1){
                    search_end = counter_vec.size() - 1;
                }

                // test if word in word sublist
                if (std::find(wordlist.begin() + search_start, wordlist.begin() + search_end, word) != wordlist.begin() + search_end ){

                    // get year/frequency data
                    string data = ngram.substr(found + 1);

                    // split on tab
                    std::string delimiter = "\t";
                    size_t pos = 0;
                    std::string subdata;

                    while ((pos = data.find(delimiter)) != std::string::npos) {

                        // get subdata
                        subdata = data.substr(0, pos);

                        // word
                        word_vec[global_counter] = word;

                        // year
                        size_t comma1 = subdata.find(",");
                        string year = subdata.substr(0, comma1);
                        int year_int = std::stoi(year);
                        year_vec[global_counter] = year_int;

                        // word count
                        subdata.erase(0, comma1 + 1);
                        size_t comma2 = subdata.find(",");
                        string wordcount = subdata.substr(0, comma2);

                        int wordcount_int = std::stoi(wordcount);
                        wordcount_vec[global_counter] = wordcount_int;

                        // erase tab
                        data.erase(0, pos + delimiter.length());

                        // advance global counter
                        global_counter++;

                    }
                }
            }
        }
    }


    // erase zeros
    word_vec.erase (word_vec.begin() + global_counter, word_vec.end() );
    year_vec = year_vec[year_vec != 0 ];
    wordcount_vec = wordcount_vec[wordcount_vec != 0 ];

    // bind output in dataframe
    DataFrame output = DataFrame::create( Named("word") =      word_vec,
                                          Named("year") =      year_vec,
                                          Named("wordcount") = wordcount_vec
                                    );

    return output;

}
