#include <Rcpp.h>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>


using namespace Rcpp;

// [[Rcpp::depends(BH)]]
// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::export]]

DataFrame get_data(std::string filename,
		   IntegerVector ids
                   )
{

    // test if file exists
    std::ifstream ifile;
    ifile.open(filename);
    if(ifile) {
    } else {
        throw std::range_error("File does not exist");
    }


    if( ids.size() == 0 ){
        throw std::range_error("ids vector is blank");
    }


    // read in ngram data
    // ****************************************************
    int global_counter = 0;

    // output vectors
    int file_guess = 50000000;

    std::vector<std::string> word_vec(file_guess);
    std::vector<double> year_vec(file_guess);
    std::vector<double> wordcount_vec(file_guess);


    // read in ngram
    std::string ngram;
    std::ifstream in(filename);

    int line_number = 1;
    int id_counter = 0;

    while (getline(in, ngram)) {


        // test if line in id
        if( line_number == ids[id_counter] ){
            id_counter++;

            // find first tab
            std::string tab = "\t";
            size_t id_tab = ngram.find(tab);

            // get word
            if( id_tab != std::string::npos ){

                std::string word = ngram.substr(0, id_tab);

                // convert word to lower case
                boost::to_lower(word);

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

	    // if at the end of output container, append more rows
            if(global_counter == word_vec.size()){

                std::vector<std::string> B(5000000);
                word_vec.insert(word_vec.end(), B.begin(), B.end());

                std::vector<double> C(5000000);
                year_vec.insert(year_vec.end(), C.begin(), C.end());
                wordcount_vec.insert(wordcount_vec.end(), C.begin(), C.end());

            }

        }

        line_number++;

    }

    // erase zeros at end of container
    word_vec.erase (word_vec.begin() + global_counter, word_vec.end() );
    year_vec.erase (year_vec.begin() + global_counter, year_vec.end() );
    wordcount_vec.erase (wordcount_vec.begin() + global_counter, wordcount_vec.end() );


    // bind output in dataframe
    DataFrame output = DataFrame::create(
        Named("word") =      word_vec,
        Named("year") =      year_vec,
        Named("wordcount") = wordcount_vec
    );

    return output;

}
