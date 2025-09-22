/*
 *  Indexer.h
 *  Carter Ng-Yu cngyu01 and Leventis Qiu cqui02
 *  4/21/25
 *
 *  CS 15 Proj04 Gerp
 * 
 * The purpose of this file is to hold the function declarations used to 
 * implement the Indexer class. This class acts the indexer for the provided
 * directory and files. It indexes every word from every file from a provided
 * directory, and stores it in a HashTable to allow for easy access to every
 * word by using a key. Important private member variables are wordMap which
 * represents the the index of all the words from the directory, and 
 * filePathMap which holds all the File Paths. Some important public functions 
 * are indexDirectory, which indexes every file in the provided directory. 
 * allLowercase turns a provided string into all-lowercase characters. 
 * stripNonAlphaNum strips all the Non-AlphaNumeric characters from a string.
 * 
 */

#ifndef _INDEXER
#define _INDEXER

#include <string>
#include "hashtable.h"
#include "FSTree.h"
#include "DirNode.h"

using namespace std;

class Indexer {
   public:
    struct Word {
        int lineNumber;
        bool insensitive;
        int fileIndex;
    };
    struct Value {
        HMap<int, int> lineCheck;
        vector<Word> wordArray;
    };

    //public functions:
    void indexDirectory(string &directory);
    string getFilePath(const int &index);
    HMap<string, Value> &getWordMap();
    vector<vector<string>> &getData();
    string allLowercase(const string &input);
    string stripNonAlphaNum(string &input);

   private:
    //private member variables:
    HMap<string, Value> wordMap;
    vector<string> filePathMap;
    vector<vector<string>> fileData;

    //private functions:
    void indexFile(string &filename, string &filepath);
    void addWord(string &word, Word &data);
    bool checkLine(Word &data, string &word);
    int getFileIndex(const string &name, const string &filename);
    template<typename streamtype>
    void openFile(streamtype &stream, string &fileName, string &filepath);
};

#endif