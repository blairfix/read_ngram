# read_ngram

`read_ngram` provides R functions for parsing data from the [Google ngram 2020 corpus](http://storage.googleapis.com/books/ngrams/books/datasetsv3.html). To use it, first download the Gzip folders from the Google repository. `read_ngram` provides two functions for parsing this data efficiently:

* `get_words`
* `get_data`

### Background

The 2012 ngram corpus provides data in a standard tab-separated format, as shown below:

```
ngram           year   match_count   volume_count
circumvallate   1978   335           91
circumvallate   1979   261           91
```

This data is easily parsed by R functions like `fread()` and `read_tsv()`. The 2020 ngram corpus, however, is provided in a non-standard format, as shown below. Each ngram (a word or phrase) gets one line:

```
ngram        year,match_count,volume_count  year,match_count,volume_count   ...
Breadberry	 1785,1,1                       1795,1,1                        ...
```

Because ngrams aren't repeated, this format saves space. But it's difficult to parse using standard R functions. That's where `read_ngram` comes in. It provides two function that are designed to be used together. `get_words` tells you the words (or phrases) that are in an ngram file. I assume you're interested only in a subset of these words. After identifying which words/phrases you want, `get_data` parses the ngram file and returns the data. 


### `get_words`

`get_words` tells you what words (or phrases) are in the ngram file. It takes a `filename` as input, and returns a vector of words/phrases in the ngram file


### `get_data`

I'm assuming you're interested in only a small subset of the words/phrases in the ngram file. That's where `get_data` comes in. Once you've used `get_words`, identify the line number of the words for which you want data. Then run `get_data` with these inputs:

*  `filename`: the ngram file you want to parse
* `ids`: the line numbers you want to read (see example below)

`get_words` returns a long-form dataframe containing the word/phrase, year and match_count:


```
word        year    match_count
Breadberry  1785    1
Breadberry  1795    1
```


### Example

Suppose I've downloaded the ngram file `1-00016-of-00024` and put it in the directory `~/Downloads/ngram_2020`. The code below will parse the file and extract data for words that are in the `grady_augmented` word list:

```R
library(Rcpp)

# source functions
sourceCpp("get_words.cpp")
sourceCpp("get_data.cpp")

# get words in ngram file
setwd("~/Downloads/ngram_2020")
words = get_words("1-00016-of-00024")

# get data for words in grady_augmented wordlist
dictionary = lexicon::grady_augmented
ids = which(words %in% dictionary)
ngram_data = get_data("1-00016-of-00024", ids)
```


### Performance

Because the ngram database is large (the 1-ngram data is about 46 GB), it's important to be able to parse it quickly. Assuming you're interested in only a subset of the data, `get_words` and `get_data` parse ngram files much faster than the fastest R parser, `readLines`. The code below extracts data for words in the `grady_augmented` list, and compares it to reading the whole database into R using `readLines`:


```R
microbenchmark(
  words = get_words("1-00016-of-00024"),
  ngram_data = get_data("1-00016-of-00024", ids),
  RreadLines = readLines("1-00016-of-00024"),
  times = 1
)


Unit: seconds
       expr       min        lq      mean    median        uq       max neval
      words  2.412359  2.412359  2.412359  2.412359  2.412359  2.412359     1
 ngram_data  4.393013  4.393013  4.393013  4.393013  4.393013  4.393013     1
 RreadLines 21.406342 21.406342 21.406342 21.406342 21.406342 21.406342     1

```

### Installation

To use `read_ngram`, install [Rcpp](https://cran.r-project.org/web/packages/Rcpp/index.html). Put the source code (`get_words.cpp` and `get_data.cpp`) in the directory of your R script. The source it with the commands:

```R
sourceCpp("get_words.cpp")
sourceCpp("get_data.cpp")
```


