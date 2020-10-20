#include <Rcpp.h>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>
#include <boost/algorithm/string/replace.hpp>

using namespace Rcpp;

// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::export]]

std::vector<std::string> get_ngrams(std::string filename)
{
    // hyphen replace strings
    std::string hyphen = " - ";
    std::string hyphen_replace = "-";

    // test if file exists
    std::ifstream ifile;
    ifile.open(filename);
    if(ifile) {
    } else {
        throw std::range_error("File does not exist");
    }


    // read in ngram
    std::string ngram;
    std::ifstream in(filename);

    std::vector<std::string> output(5000000);
    int i = 0;

    while (getline(in, ngram)) {

        // find first tab
        std::string tab = "\t";
        size_t id_tab = ngram.find(tab);

        // get word
        if( id_tab != std::string::npos ){

            std::string word = ngram.substr(0, id_tab);

            // convert word to lower case
            boost::to_lower(word);

            // remove spaces around hyphen
            boost::replace_all(word, hyphen, hyphen_replace);


            output[i] = word;
            i++;

            if(i == output.size()){
                std::vector<std::string> B(5000000);
                output.insert(output.end(), B.begin(), B.end());
            }
        }
    }

    output.erase (output.begin() + i, output.end() );
    return output;

}
