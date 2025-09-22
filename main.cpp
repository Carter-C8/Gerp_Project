/*
 *  main.cpp
 *  Carter Ng-Yu cngyu01 and Leventis Qui cqui02
 *  4/21/25
 *
 *  CS 15 Proj04 Gerp
 * 
 * The purpose of the Gerp project is to create a program that indexes a 
 * directory and allows for the client to search for words within this index.
 * Gerp will be called from the command line with a provided directory to index
 * and a provided output file to print to. The user can search for any word,
 * and Gerp will output all of the lines from the indexed files where the word
 * appears. If the user asks for an insensitive search, Gerp will search for
 * words that are not exact matches, but might have different capitlization or
 * non-alphanumeric characters attached to it. The user can also switch the
 * output file if they want to.
 * 
 */


#include "Searcher.h"

using namespace std;

void abort();

/*
 * name:      main( )
 * purpose:   Check the command line for the right format, printing an error to
 *            cerr if the format is wrong. Gets the name of input directory and
 *            the name of the output file and creates a Searcher object,
 *            starting the code.
 * arguments: an integer argc representing the amount of command line
 *            arguements, an array of character pointers to each element on the
 *            command line.
 * returns:   the integer 0
 * effects:   Creates a Searcher object, indexing the input directory and 
 *            printing any output to the output file.
 */
int main(int argc, char *argv[]) {
    //Checking if the command line is correct
    if ((not (argc == 3))) {
        abort();
    }
    string inputDirectory = argv[1];
    string outputDirectory = argv[2];
    Searcher gerp(inputDirectory, outputDirectory);
    return 0;
}

/*
* name:      abort( )
* purpose:   Print an error message and terminate the program
* arguments: none
* returns:   none
* effects:   Prints the error message and terminates the program
*/
void abort() {
    cerr << "Usage: ./gerp inputDirectory outputFile" << endl;
    exit(EXIT_FAILURE);
 }
 