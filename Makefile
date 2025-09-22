###
### Makefile for Gerp Project
###
### Author:  Carter Ng-Yu and Leventis Qui

CXX      = clang++
CXXFLAGS = -g3 -Wall -Wextra -Wpedantic -Wshadow
LDFLAGS  = -g3 

## This is the linking command for gerp
gerp : main.o Searcher.o Indexer.o hashtable.o FSTree.o DirNode.o
	${CXX} ${LDFLAGS} -o gerp $^
	
## This rule builds main.o
main.o : main.cpp Searcher.h hashtable.h FSTree.h DirNode.h Indexer.h
	${CXX} ${CXXFLAGS} -c main.cpp

## This rule builds Searcher.o
Searcher.o : Searcher.cpp Searcher.h hashtable.h FSTree.h DirNode.h Indexer.h
	${CXX} ${CXXFLAGS} -c Searcher.cpp

## This rule builds Indexer.o
Indexer.o: Indexer.cpp Indexer.h hashtable.h FSTree.h DirNode.h
	${CXX} ${CXXFLAGS} -c Indexer.cpp

## This rule builds hashtable.o
hashtable.o: hashtable.cpp hashtable.h
	${CXX} ${CXXFLAGS} -c hashtable.cpp

## This rule builds unit_test
unit_test: unit_test_driver.o hashtable.o Indexer.o FSTree.o DirNode.o
	${CXX} ${CXXFLAGS} $^
##
## Here is a special rule that removes all .o files besides the provided ones 
## (DirNode.o and FSTree.o), all temporary files (ending with ~), and 
## a.out produced by running unit_test. First, we find all .o files 
## that are not provided files, then we run rm -f on each found file 
## using the -exec find flag. Second, we delete the temporary files
## and a.out. @ is used to suppress stdout.
## 
## You do not need to modify or further comment this rule!
##
clean:
	@find . -type f \( \
		-name '*.o' ! -name 'FSTree.o' ! -name 'DirNode.o' \
		\) -exec rm -f {} \;
	@rm -f *~ a.out

