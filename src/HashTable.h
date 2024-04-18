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

#define key_ first
#define value_ second

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
            backingStore[findArraySlot(key, array_slots)].push_back(make_pair(key, value));
        }

        bool keyExists(const K key) {
            if (locate(key) == nullptr)
                return false;

            return true;
        }

        pair<K, V> *locate(const K &key) {
            for (pair<K, V>& element : backingStore[findArraySlot(key, array_slots)]) {
                if (element.key_ == key) {
                    return &element;
                }
            }
            // return nullptr if the item is not found
            return nullptr;
        }

        void updateValue(const K key, const V value) {
            locate(key)->value_ = value;
        }

        optional<V> getValue(const K &key) {
            if (locate(key) == nullptr)
                return nullopt;
            return locate(key)->value_;
        }
        
        void remove(const K &key) {
            backingStore[findArraySlot(key, array_slots)].remove(*locate(key));
            total_elements--;
        }

        float getLoadFactor() { return ((float) total_elements) / ((float) array_slots); }
        
        int getTotalElements() { return total_elements; }
        
        int getArraySlots() { return array_slots; }

        void setArraySlots(size_t newSize) { array_slots = newSize; }

        bool atMAX_LOAD_FACTOR() { return getLoadFactor() >= MAX_LOAD_FACTOR; }

        bool isInvalidCapacity(int capacity) const { return capacity < 1; }

        bool isElementsToMove() const { return total_elements > 0; }

        size_t findArraySlot(const K key, const size_t capacity) { return (getHashKey(key) % capacity); }

        void printHashTable() {
            for (int i = 0; i < array_slots; i++) {
                cout << i << ":";
                for (pair<K, V>& element : backingStore[i]) {
                    cout << " -> (" << element.key_ << ", " << element.value_ << ")";
                }
                cout << endl;
            }
        }
        
    private:
        int array_slots = 0;
        int total_elements = 0;
        hash<K> key_hash;
        list<pair<K, V> > *backingStore = nullptr;
        
        void resizeHashTable(int capacity) {
            list<pair<K, V> > *newBackingStore = createNewBackingStore(capacity);

            if (isElementsToMove())
                moveOver(capacity, newBackingStore);

            delete[] backingStore;
            updateBackingStore(newBackingStore);

            setArraySlots(capacity);
        }

        void updateBackingStore(list<pair<K, V> > *newBackingStore) { backingStore = newBackingStore; }

        void moveOver(int capacity, list<pair<K, V> > *newBackingStore) {
            for (int currentIndex = 0; currentIndex < array_slots; currentIndex++) {
                for (pair<K, V> element : backingStore[currentIndex]) {
                    newBackingStore[findArraySlot(element.key_, capacity)].push_back(element);
                }
            }
        }

        list<pair<K, V> > *createNewBackingStore(int capacity) const {
            list<pair<K, V> > *newBackingStore = new list<pair<K, V> >[capacity];
            for (int currentIndex = 0; currentIndex < capacity; currentIndex++) {
                newBackingStore[currentIndex] = list<pair<K, V> >();
            }
            return newBackingStore;
        }

        // hash anything into an integer appropriate for
        // the current array_slots
        // TIP: use the std::hash key_hash defined as a private variable
        size_t getHashKey(const K &key) {
            return key_hash(key);
        }
    };
}

#endif /* hashtable_hpp */
