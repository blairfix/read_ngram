#include <Rcpp.h>
#include <fstream>
#include <string>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string.hpp>
#include <vector>

using namespace Rcpp;

// [[Rcpp::depends(BH)]]
// [[Rcpp::plugins(cpp11)]]
// [[Rcpp::export]]


DataFrame get_data(
		    std::string filename,
		    Rcpp::IntegerVector ids
                   )
{

    // 1. test if file exists
    std::ifstream ifile;
    ifile.open(filename);
    if(ifile) {
    } else {
        throw std::range_error("File does not exist");
    }


    if( ids.size() == 0 ){
        throw std::range_error("ids vector is blank");
    }


    // 2. read in ngram data
    // -----------------------------------------------------

    // output vectors
    std::list<std::string> word_vec;
    std::list<double> year_vec;
    std::list<double> wordcount_vec;

    // read in ngram
    std::string ngram;
    std::ifstream in(filename);

    int line_number = 1;
    int id_counter = 0;

    // get number of words
    int nwords = ids.size();

    // read in file and get data with rows in id vector
    while (getline(in, ngram) ) {

        // test if line in id and counter is not over nwords
        if( line_number == ids[id_counter] && id_counter < nwords ){

	    // advance word counter
            id_counter++;

            // split line on tab
            std::vector<std::string> line_split;
            boost::split(line_split, ngram, boost::is_any_of("\t") );

            if( line_split.size() > 1 ){

                // make word lower case
                boost::to_lower( line_split[0] );

                // loop over data
                for(int i = 1; i < line_split.size(); i++){

                    // split data on comma
                    std::vector<std::string> data;
                    boost::split(data,  line_split[i], boost::is_any_of(",") );

                    // append data
                    word_vec.push_back( line_split[0] );
                    year_vec.push_back( std::stoi( data[0] ) );
                    wordcount_vec.push_back( std::stoi( data[1] ) );

                }
            }
        }

        line_number++;
    }


    // 3. bind output in dataframe
    // -----------------------------------------------------

    DataFrame output = DataFrame::create(
        Named("word") =      word_vec,
        Named("year") =      year_vec,
        Named("wordcount") = wordcount_vec
    );

    return output;

}
