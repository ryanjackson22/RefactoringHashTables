//
//  HashTable.h
//
//  This file defines a Hash Table class.
//  You SHOULD modify this file.
//
//  Copyright  2019 David Kopec
//  Edited     2023 Ryan Jackson
//  Refactored 2024 Ryan Jackson
//
//  Permission is hereby granted, free of charge, to any person
//  obtaining a copy of this software and associated documentation files
//  (the "Software"), to deal in the Software without restriction,
//  including without limitation the rights to use, copy, modify, merge,
//  publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so,
//  subject to the following conditions:
//
//  The above copyright notice and this permission notice
//  shall be included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
//  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//  OTHER DEALINGS IN THE SOFTWARE.

#ifndef hashtable_hpp
#define hashtable_hpp

#include <utility> // for pair
#include <functional> // for hash()
#include <list>
#include <optional>
#include <algorithm> // find_if(), remove_if()
#include <iostream>

#define DEFAULT_CAPACITY 10
#define MAX_LOAD_FACTOR 0.7
#define GROWTH_FACTOR 2

using namespace std;

namespace csi281 {
    template
    <typename K, typename V>
    class HashTable {
    public:
        HashTable(int capacity = DEFAULT_CAPACITY) {
            if (isInvalidCapacity(capacity))
                capacity = DEFAULT_CAPACITY;
            resizeHashTable(capacity);
        }

        ~HashTable() {
            delete[] backingStore;
        }

        void put(const K key, const V value) {
            if(keyExists(key)) {
                updateValue(key, value);
                return;
            }
            insertNewKey(key, value);
            total_elements++;

            if (atMAX_LOAD_FACTOR())
                resizeHashTable(array_slots * GROWTH_FACTOR);
        }

        void insertNewKey(const K key, const V value) {
            backingStore[findArraySlot(key)].push_back(make_pair(key, value));
        }

        bool keyExists(const K key) {
            for (pair<K, V>& element : backingStore[findArraySlot(key)]) {
                if (element.first == key) {
                    return true;
                }
            }
            return false;
        }

        void updateValue(const K key, const V value) {
            for (pair<K, V>& element : backingStore[findArraySlot(key)]) { // traversing the list
                if (element.first == key) { // if the key is found
                    element.second = value; // updating the value
                }
            }
        }

        // Get the item associated with a particular key
        // return an empty optional (nullopt) if the item is not found
        // and returns an optional with the value associated with key
        // if key is found
        // TIP: read the documentation on optional
        // https://en.cppreference.com/w/cpp/utility/optional
        // TIP: Be careful to get a reference to the list at each
        // location in the backing store, so you're modifying
        // the original and not a copy
        optional<V> get(const K &key) {
            for (pair<K, V>& element : backingStore[findArraySlot(key)]) {
                if (element.first == key) {
                    return element.second;
                }
            }
            // return an empty optional if the item is not found
            return nullopt;
        }
        
        // Remove a key and any associated value from the hash table
        // TIP: I suggest using remove_if()
        // https://en.cppreference.com/w/cpp/algorithm/remove
        // TIP: Be careful to get a reference to the list at each
        // location in the backing store, so you're modifying
        // the original and not a copy
        void remove(const K &key) {
//            size_t index = hashKey(key) % array_slots;
            for (pair<K, V>& element : backingStore[findArraySlot(key)]) { // traversing the list
                if (element.first == key) { // if the key is found:
                    backingStore[findArraySlot(key)].remove(element); // remove the pair
                    total_elements--; // decrease total_elements
                    return;
                }
            }
        }

        float getLoadFactor() { return ((float) total_elements) / ((float) array_slots); }
        
        int getTotalElements() { return total_elements; }
        
        int getArraySlots() { return array_slots; }

        bool atMAX_LOAD_FACTOR() { return getLoadFactor() >= MAX_LOAD_FACTOR; }

        bool isInvalidCapacity(int capacity) const { return capacity < 1; }

        size_t findArraySlot(const K key) { return (hashKey(key) % array_slots); }

        // Print out the contents of the hash table
        void debugPrint() {
            for (int i = 0; i < array_slots; i++) {
                cout << i << ":";
                for (pair<K, V>& element : backingStore[i]) {
                    cout << " -> (" << element.first << ", " << element.second << ")";
                }
                cout << endl;
            }
        }
        
    private:
        int array_slots = 0;
        int total_elements = 0;
        hash<K> key_hash;
        list<pair<K, V> > *backingStore = nullptr;
        
        // Shift all of the items in backingStore into a
        // new backing store of size capacity, or create
        // the backingStore for the first time
        void resizeHashTable(int capacity) {
            list<pair<K, V> > *backingStoreReplacement = new list<pair<K, V> >[capacity];
            for (int i = 0; i < capacity; i++) {
                backingStoreReplacement[i] = list<pair<K, V> >();
            }

            // get items from old backing store and move them over
            if (total_elements > 0) { // only if there are items to move
                for (int i = 0; i < array_slots; i++) {
                    for (pair<K, V> element : backingStore[i]) {
                        // find location in new array
                        size_t index = hashKey(element.first) % capacity;
                        // put it in
                        backingStoreReplacement[index].push_back(element);
                    }
                }
            }

            if (backingStore != nullptr) {
                delete[] backingStore;
            }

            backingStore = backingStoreReplacement;
            array_slots = capacity;
        }
        
        // hash anything into an integer appropriate for
        // the current array_slots
        // TIP: use the std::hash key_hash defined as a private variable
        size_t hashKey(const K &key) {
            return key_hash(key);
        }
    };
    
}

#endif /* hashtable_hpp */
