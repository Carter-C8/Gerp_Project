/*
 *  hashtable.h
 *  Leventis Qui cqui02 and Carter Ng-Yu cngyu01
 *  4/21/25
 *
 *  CS 15 Proj04 Gerp
 * 
 * The purpose of this file is to hold the function declarations 
 * (and definitions due to templating) to implement the hashtable class. This
 * class acts as an implementation of a hash table that can use keys to store
 * values, and allow for easy access to this data by using the same key. The
 * hashtable has the ability to dynamically resize, allowing for the storage of
 * any amount of data. Public functions that might be used are the insert 
 * function which uses a provided key to insert a provided value into the hash
 * table. The search function can be used to check if a specific bucket or 
 * index already exists inside the hash table. The get function is used to
 * access a value in the hash table through a provided key. 
 * 
 * (There is no .cpp for this class because everything must be defined in the
 *  .h file due to the use of templates)
 * 
 */

#ifndef HASH_TABLE_LEV_CARTER
#define HASH_TABLE_LEV_CARTER

#include <string>
#include <vector>
#include <list>
#include <functional>
#include <stdexcept>
#include <iostream>

using namespace std;

template <class Key, class Value> 
class HMap {
   public:

    /*
    * name:      HMap default constructor
    * purpose:   initialize an empty HashTable
    * arguments: none
    * returns:   none
    * effects:   loadFactor is set to 0.7, capacity will be set to 20
    */
    HMap() {
        loadFactor = 0.7;
        size = 0;
        capacity = 0;
        rehash();
    }

    /*
    * name:      insert ()
    * purpose:   To insert a Value into the HashTable using a provided key.
    * arguments: A constant reference to a key, a constant reference to a 
    *            value.
    * returns:   none.
    * effects:   Adds the new Value into the HashTable (into its associated
    *            bucket/index slot).
    */
    void insert(const Key &key, const Value &value) {
        //Rehash if the loadFactor is over 0.7.
        if (currLoadFactor() > loadFactor) {
            rehash();
        }
        //Get the bucket/Index for our key.
        size_t currIndex = getIndex(key);
        //for every key-value pair, if there is the same one in it, update the 
        //value (if the bucket is empty, add the key-value pair)
        for (pair<Key, Value> &kvpair : buckets[currIndex]) {
            if (kvpair.first == key) {
                kvpair.second = value;
                return;
            }
        }
        //If the bucket(list) is already exists, add our new key-value 
        //pair (word + line#) to the list
        buckets[currIndex].push_back({key, value});
        size++;
    }

    /*
    * name:      get ()
    * purpose:   To access an associated Value using the provided key.
    * arguments: A constant reference to a key.
    * returns:   A reference to the associated Value of the provided key.
    * effects:   none.
    */
    Value &get(const Key &key) {
        //Get the bucket for our provided key value
        size_t index = getIndex(key);
        //Return the Value(list) that is found in the associated 
        //bucket(vector slot)
        for (pair<Key, Value> &kvpair : buckets[index]) {
            if (kvpair.first == key) {
                return kvpair.second;
            }
        }
        //If we cannot find the bucket or get the bucket, throw a runtime error 
        throw runtime_error("get error");
    }

    /*
    * name:      search ()
    * purpose:   To use a provided key to search for the associated bucket, and
    *            check if that bucket is filled or not..
    * arguments: A constant reference to a key.
    * returns:   a bool, true if there is a value stored in the associated.
    *            bucket, false if the associated bucket is empty (nothing has
    *            been stored/used yet).
    * effects:   none.
    */
    bool search(const Key &key) {
        size_t index = getIndex(key);
        //Check the bucket for a Value that matches with the associated key
        for (pair<Key, Value> &kvpair : buckets[index]) {
            if (kvpair.first == key) {
                return true;
            }
        }
        return false;
    }

    /*
    * name:      sizeE ()
    * purpose:   To get the size of the HashTable (# filled buckets, not size 
    *            of list in the buckets)
    * arguments: none.
    * returns:   a size_t that represents the # of filled buckets in the 
    *            HashTable
    * effects:   none.
    */
    size_t sizeE() {
        return size;
    }

    /*
    * name:      currLoadFactor ()
    * purpose:   To get the the current Load Factor of the HashTable
    * arguments: none.
    * returns:   A float that represents the current Load Factor.
    * effects:   none.
    */
    float currLoadFactor() {
        //have to cast it to float to make sure the result is float.
        if (capacity == 0) {
            return 0;
        }
        return (static_cast<float>(size) / capacity);
    }

   private:
    //Private Member Variables:
    //HashTable implementation
    vector<list<pair<Key,Value>>> buckets;
    //size is vector.size
    size_t size;
    size_t capacity;
    float loadFactor;

    //Private Functions:

    /*
    * name:      getIndex ()
    * purpose:   To get the index number (slot/bucket) from a provided key.
    * arguments: A constant reference to a key.
    * returns:   A size_t that represents the index that the provided key is
    *            associated with.
    * effects:   none.
    */
    size_t getIndex(const Key &key) {
        hash<Key> tempHash;
        return tempHash(key) % capacity;
    }

    /*
    * name:      rehash ()
    * purpose:   Create a new and larger HashTable(vector) to store more data.
    * arguments: A HashTable to copy and increase capacity
    * returns:   none.
    * effects:   Previous HashTable is deconstructed, all elements are copied 
    *            over to the new and larger HashTable(vector).
    */
    void rehash() {
        //Create en empty HashTable with a new/larger capacity
        int newCapacity = capacity * 2 + 20;
        vector<list<pair<Key,Value>>> newBuckets(newCapacity);

        //Copy and Rehash everything from original HashTable to the new empty
        //HashTable
        for (list<pair<Key,Value>> &bucket : buckets) {
            for (pair<Key,Value> &kvPair : bucket) {
                hash<Key> tempHash;
                size_t newIndex = tempHash(kvPair.first) % newCapacity;
                newBuckets[newIndex].push_back(
                        std::make_pair(kvPair.first, std::move(kvPair.second)));

            }
        }

        //Set private member variables to their new values
        buckets = std::move(newBuckets);
        capacity = newCapacity;
    }

};

#endif