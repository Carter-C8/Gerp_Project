/*
 *  Searcher.h
 *  Leventis Qui cqui02 and Carter Ng-Yu cngyu01
 *  4/21/25
 *
 *  CS 15 Proj04 Gerp
 * 
 * The purpose of this file is to hold the function declarations used to
 * implement the Searcher class. This class acts as the search engine for our
 * index. It prompts the user with a query, and allows them to perform either
 * an insensitive or sensitive search for a word, sending output to the
 * provided file. Public functions in this class are the constructor which
 * uses the INDEXER class to create an Index and then starts the query loop
 * with the query function. The query function allows the user to complete 
 * different types of searches and changes of output files through commands
 * from the commandline by the client.
 * 
 */

#ifndef SEARCHER_H
#define SEARCHER_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

#include "Indexer.h"

using namespace std;

class Searcher {
   public:
    Searcher(string &directory, string &outputFile);
    void query(string &outputFile);

   private:
    //private member variable:
    Indexer index;

    string searchAWord(string &word, bool sensitive);
};

#endif