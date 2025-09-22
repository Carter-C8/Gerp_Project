/*
*  Indexer.cpp
*  Carter Ng-Yu and Leventis Qiu
*  4/21/25
*
*  CS 15 Proj04 Gerp
*
*  Implements the Indexer.h interface
*/

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <exception>
#include <stack>

#include "Indexer.h"
#include "FSTree.h"
#include "DirNode.h"

using namespace std;

/*
* name:      indexDirectory ( )
* purpose:   Indexes every file from the provided directory and its
*            subdirectories, storing every word in the HASHMAP wordMap.
* arguments: A string reference to the name of the directory to index.
* returns:   none
* effects:   Opens and reads all files in the directory, storing every word in
*            wordMap.
*/
void Indexer::indexDirectory(string &directory) {
    //Initialize FSTree and DirNode Stack
    FSTree file(directory);
    DirNode *curr(file.getRoot());
    stack<DirNode*> s;
    s.push(curr);
    //Traverse all Nodes
    while (not s.empty()) {
        curr = s.top();
        s.pop();
        //Get the filepath of the current Node.
        string filePath = "";
        if (curr->hasFiles()) {
            DirNode* looper(curr);
            while (looper != file.getRoot()) {
                string currPath = looper->getName() + "/";
                filePath.insert(0, currPath);
                looper = looper->getParent();
            }
            filePath.insert(0, file.getRoot()->getName() + "/");
          
            //Index all Files and Push on SubDirectories
            for (int i = 0; i < curr->numFiles(); i++) {
                string filename = curr->getFile(i);
                indexFile(filename, filePath);
            }
        }
        for (int i = 0; i < curr->numSubDirs(); i++) {
            s.push(curr->getSubDir(i));
        }
    }
}

/*
* name:      indexFile ( )
* purpose:   Reads through a provided file, indexing and storing every word
*            into wordMap.
* arguments: A string reference to the name of the provided file and a string
*            reference to the path of the provided file.
* returns:   none
* effects:   Opens and reads through the provided file, updating wordMap.
*/
void Indexer::indexFile(string &filename, string &filepath) {
    ifstream instream;
    openFile(instream, filename, filepath);
    string currWord = "";
    int currLine = 1;
    int thisFileIndex = getFileIndex(filepath, filename);
    string tempStr;
    vector<string> tempData;
    while(getline(instream, tempStr)) {
        tempData.push_back(tempStr);
        istringstream lineStream(tempStr);
        while (lineStream >> currWord) {
            Word newData;
            newData.fileIndex = thisFileIndex;
            newData.lineNumber = currLine;
            newData.insensitive = false;
            currWord = stripNonAlphaNum(currWord);
            if (currWord == allLowercase(currWord)) { //Strip == Original Word
                addWord(currWord, newData);
            } else { //Else if stripped version is different, add both versions
                addWord(currWord, newData);
                newData.insensitive = true;
                currWord = allLowercase(currWord);
                addWord(currWord, newData);
            }
        }
        ++currLine;
    }
    instream.close();
    fileData.push_back(std::move(tempData));
}
  

/*
* name:      addWord ( )
* purpose:   Adds a provided word to the wordMap, including the word's data,
*            such as filename, linenumber, and if it is stripped.
* arguments: A string reference to the provided word, a Word struct reference
*            to the information of the provided word, and a Value struct
*            reference which holds the vector used for collisions and another
*            HMAP ensure no duplicate words are added from the same line.
* returns:   none
* effects:   Updates HASHMAP wordMap by indexing/adding the provided word and
*            its data.
*/
void Indexer::addWord(string &word, Word &data) {
    //If word exists in hashmap and is on the same line, do not add it to map
    if ((wordMap.search(word)) and (checkLine(data, word))) {
        return;
    }

    //If the word already exists within the hashmap, add the word to the word
    //array (linked list node)
    if ((wordMap.search(word))) {
        Value &curr = wordMap.get(word);
        curr.wordArray.push_back(data);
        //update the lineCheck Hmap to show this word exists on this line
        curr.lineCheck.insert(data.lineNumber, data.fileIndex);
    } else { //The word does not exist in the Hasmap yet
        Value newValue;
        //Add word to hashmap (vector slot)
        wordMap.insert(word, newValue);
        Value &curr = wordMap.get(word);
        curr.wordArray.push_back(data);
        curr.lineCheck.insert(data.lineNumber, data.fileIndex);
    }
}


/*
* name:      checkLine ( )
* purpose:   Checks whether a line in the file has 2 of the same word in it.
* arguments: A Word struct reference with the word's data
*            (including lineNumber), and a string reference to the word.
* returns:   True bool if there ARE two of the provided word in the same line
*            in the SAME file. False bool if there are NOT two of the provided 
*            word in the same line.
* effects:   none
*/
bool Indexer::checkLine(Word &data, string &word) {
    //If there is a (value) entry for that lineNumber (key)
    if (wordMap.get(word).lineCheck.search(data.lineNumber)) {
        //Return the true if the lineNumber key returns a file index that
        //matches the current word's file index
        return 
        wordMap.get(word).lineCheck.get(data.lineNumber) == data.fileIndex;
    }
    //If there is no entry return false
    return false;
}


/*
* name:      stripNonAlphaNum ( )
* purpose:   Strips all the non-Alphabetic-Numerical characters from a string.
* arguments: A string reference of the input to be stripped.
* returns:   A stripped string.
* effects:   none
*/
string Indexer::stripNonAlphaNum(string &input) {
    size_t head = 0;
    size_t tail = input.length();
    //define head/tail outside for loop so that we can use the value afterwards
    for(;head < input.size() and not std::isalnum(input[head]); head++);
    for(;tail > head and not std::isalnum(input[tail-1]); tail--);
    return input.substr(head, tail - head);
}


/*
* name:      openFile ( )
* purpose:   Opens a provided file and throws a runtime_error if the file is
*            unable to be opened.
* arguments: A stream to the provided file, and a string reference of the name
*            of the file, and a string reference of the filepath.
* returns:   none.
* effects:   Opens the file or throws a runtime_error.
*/
template<typename streamtype>
void Indexer::openFile(streamtype &stream, string &fileName, string &filepath) {
    stream.open(filepath + fileName);
    if (not stream.is_open()) {
        throw runtime_error("Unable to open file " + fileName);
    }
}

/*
* name:      allLowercase ( )
* purpose:   turns a striped string into lowercase
* arguments: A constant string reference.
* returns:   none
* effects:   none
*/
string Indexer::allLowercase(const string &input) {
    string result = input;
    for (char &c : result) {
        c = tolower(c);
    }
    return result;
}

/*
* name:      getFilePath ( )
* purpose:   Gets the file path of a provided index.
* arguments: A constant integer reference of the desired index.
* returns:   A string representing the file path.
* effects:   none.
*/
string Indexer::getFilePath(const int &index) {
    return filePathMap[index];
}

/*
* name:      getFileIndex ( )
* purpose:   Gets the index of a file using the file path.
* arguments: A constant string reference of the file's path, a constant string
             reference of the file's name.
* returns:   An integer that represents the file's index.
* effects:   Adds the file's path to the filePathMap.
*/
int Indexer::getFileIndex(const string &path, const string &filename) {
    size_t newIndex = filePathMap.size();
    string fullPath = path + filename;
    filePathMap.push_back(fullPath);
    return newIndex;
}

/*
* name:      getWordMap ( )
* purpose:   Gets the index (wordMap).
* arguments: none.
* returns:   A reference to wordMap.
* effects:   none.
*/
HMap<string, Indexer::Value>& Indexer::getWordMap() {
    return wordMap;
}

/*
* name:      getData ( )
* purpose:   Gets the fileData vector.
* arguments: none.
* returns:   A reference to fileData.
* effects:   none.
*/
vector<vector<string>> &Indexer::getData() {
    return fileData;
}