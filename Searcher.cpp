/*
 *  Searcher.cpp
 *  Leventis Qui cqui02 and Carter Ng-Yu cngyu01
 *  4/21/25
 *
 *  CS 15 Proj04 Gerp
 * 
 * Implements the Searcher.h interface
 */

#include "Searcher.h"

using namespace std;

/*
* name:      Searcher constructor
* purpose:   initialize Searcher by creating an index and starting the query
             loop.
* arguments: A string reference of the name of a directory to index, a string
             reference of the name of the output file to print to.
* returns:   none.
* effects:   Fills the index private member variable and calls the query 
             function.
*/
Searcher::Searcher(string &inputDirectory, string &outputFile) {
    index.indexDirectory(inputDirectory);
    query(outputFile);
}

/*
* name:      query ( )
* purpose:   Start a query loop that prompts the client for commands to either
             complete a sensitive or insensitive search for words, change the
             output file, or quit the program.
* arguments: A string reference of the name of the output file to print to.
* returns:   none.
* effects:   Will write to the output file with the output from the searches.
*/
void Searcher::query(string &outputFile) {
    string line;
    ofstream out;
    out.open(outputFile);
    string cmd;
    bool status = true;
    while (status) {
        cout << "Query? ";
        if (not (cin >> cmd)) {
            status = false;
            continue;
        }
        if (cmd == "@q" or cmd == "@quit") {
            status = false;
        } else if (cmd == "@i" or cmd == "@insensitive") {
            //case insensitive
            string word;
            if(not (cin >> word)) {
                status = false;
            }
            word = index.allLowercase(index.stripNonAlphaNum(word));
            out << searchAWord(word, false);
        } else if (cmd == "@f") {
            string filename;
            if(not (cin >> filename)) {
                status = false;
            }
            out.close();
            out.open(filename);
        } else {
            //case sensitive
            cmd = index.stripNonAlphaNum(cmd);
            out << searchAWord(cmd, true);
        }
    }
    cout << "Goodbye! Thank you and have a nice day.\n";
    out.close();
}

/*
* name:      searchAWord ( )
* purpose:   Searches the index for the provided word, compiling all cases of
             that word and the text from those lines.
* arguments: A string reference of the the provided word and a bool, true for a
             sensitive search, and false for an insensitive search.
* returns:   A string compiled with all of the filepaths and lines of text that 
             have the provided word in it from the index.
* effects:   none.
*/
string Searcher::searchAWord(string &word, bool sensitive) {
    string result;
    if(index.getWordMap().search(word) and not word.empty()) {
        vector<Indexer::Word> *tempWordArr = 
        &index.getWordMap().get(word).wordArray;
        vector<vector<string>> tempData = index.getData();
        size_t size = tempWordArr->size();
        for(size_t i = 0; i < size; i++) {
            //skip stripped word if sensitive
            if(tempWordArr->at(i).insensitive and sensitive) continue;
            int fileIndex = tempWordArr->at(i).fileIndex;
            result += index.getFilePath(fileIndex);
            result += ":";
            int lineNum = tempWordArr->at(i).lineNumber;
            result += to_string(lineNum);
            result += ": ";
            result += tempData.at(fileIndex).at(lineNum - 1);
            result += "\n";
        }
        if(not result.empty()) return result;
    } 
    //case not found
    if(sensitive) {
        result += word;
        result += " Not Found. Try with @insensitive or @i.\n";
    } else {
        result += word;
        result += " Not Found.\n";
    }
    return result;
}
