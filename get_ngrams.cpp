#include <Rcpp.h>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>
#include <vector>


using namespace Rcpp;

// [[Rcpp::depends(BH)]]
// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::export]]

std::vector<std::string> get_ngrams(std::string filename)
{
    //1. open input file
    std::ifstream input_file(filename, std::ifstream::in);
    if (!input_file.is_open())
    {
        throw std::range_error("File does not exist");
    }

    //2.  read in ngram
    std::string ngram;
    std::list<std::string> output; //store the ouput in a std::list because it can handle insertions in O[1] time.
    while (getline(input_file, ngram)) 
    {
        //get only the ngram from each line, discard all the number information.
        std::string word = ngram.substr(0, ngram.find("\t"));

        if (word.length() > 0)
        {
            boost::to_lower(word);
            output.push_back(word);
        }
    }

    //3. convert output to vector and return
    std::vector<std::string> result(output.begin(), output.end());
    return result;

}
