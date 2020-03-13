#pragma once

// Copyright 2020 Ilya Chukanov

#include <iostream>
#include <algorithm>
#include <cmath>
#include <vector>
#include <list>
#include <stdexcept>      

using namespace std;

template <class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
 public:
    using const_iterator =
        typename list<pair<const KeyType, ValueType>>::const_iterator;
    using iterator = typename list<pair<const KeyType, ValueType>>::iterator;

    HashMap(Hash hasherObj = Hash());

    HashMap(initializer_list<pair<KeyType, ValueType>> list,
            Hash hasherObj = Hash());

    template <typename IteratorType>
    HashMap(IteratorType begin, IteratorType end, Hash hasherObj = Hash());

    Hash hash_function() const;

    bool empty() const;

    void insert(pair<KeyType, ValueType> promoteElement);

    iterator begin();

    const_iterator end() const;

    iterator end();

    HashMap& operator=(HashMap const& element_position_in_object);

    iterator find(KeyType sample_key_for_iterating);

    void erase(KeyType eraseElem);

    const_iterator begin() const;

    size_t size() const;

    ValueType& operator[](KeyType sample_key_for_iterating);

    const ValueType& at(KeyType sample_key_for_iterating) const;

    const_iterator find(KeyType sample_key_for_iterating) const;

    void clear();

 private:
    const size_t initialTableSize_ = 20;

    size_t hash_table_size_quantity_elements_;

    size_t num_hash_table_elements_ = 0;

    vector<list<typename list<pair<const KeyType, ValueType>>::iterator>>
        hash_table_;

    list<pair<const KeyType, ValueType>> all_elements_;

    Hash function_hash_number_;
};

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::insert(
    pair<KeyType, ValueType> promoteElement) {
    size_t hash_table_cell_id = function_hash_number_(promoteElement.first) %
                                hash_table_size_quantity_elements_;
    for (auto& element : hash_table_[hash_table_cell_id]) {
        KeyType def_prom = element->first;
        if (def_prom == promoteElement.first) {
            return;
        }
    }
    all_elements_.push_back(promoteElement);
    auto Myiterator = all_elements_.end();
    --Myiterator;
    hash_table_[hash_table_cell_id].push_back(Myiterator);

    ++num_hash_table_elements_;

    if (hash_table_size_quantity_elements_ > num_hash_table_elements_ * 2) {
        return;
    }
    list<pair<KeyType, ValueType>> position_of_scale;
    for (auto& Iterator_for_find_pos : all_elements_) {
        position_of_scale.push_back(make_pair(Iterator_for_find_pos.first,
                                              Iterator_for_find_pos.second));
    }
    hash_table_.clear();
    all_elements_.clear();
    hash_table_size_quantity_elements_ = hash_table_size_quantity_elements_ * 2;
    hash_table_.resize(hash_table_size_quantity_elements_);
    num_hash_table_elements_ = 0;
    for (auto& system : position_of_scale) {
        insert(system);
    }
    position_of_scale.clear();
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::clear() {
    for (auto& finderIter : all_elements_) {
        size_t hashFunction = function_hash_number_(finderIter.first) %
                              hash_table_size_quantity_elements_;
        hash_table_[hashFunction].clear();
    }
    all_elements_.clear();
    num_hash_table_elements_ = 0;
}

template <class KeyType, class ValueType, class Hash>
typename list<pair<const KeyType, ValueType>>::const_iterator
HashMap<KeyType, ValueType, Hash>::find(
    KeyType sample_key_for_iterating) const {
    size_t hashFunc = function_hash_number_(sample_key_for_iterating) %
                      hash_table_size_quantity_elements_;
    for (auto& fonderInter : hash_table_[hashFunc]) {
        KeyType def_prom = fonderInter->first;
        if (sample_key_for_iterating == def_prom) {
            return fonderInter;
        }
    }
    return all_elements_.end();
}

template <class KeyType, class ValueType, class Hash>
const ValueType& HashMap<KeyType, ValueType, Hash>::at(
    KeyType sample_key_for_iterating) const {
    auto iterator_for_find_key = find(sample_key_for_iterating);
    if (all_elements_.end() == iterator_for_find_key) {
        throw out_of_range("");
    }
    return iterator_for_find_key->second;
}

template <class KeyType, class ValueType, class Hash>
ValueType& HashMap<KeyType, ValueType, Hash>::operator[](
    KeyType sample_key_for_iterating) {
    auto iterator_for_find_key = find(sample_key_for_iterating);
    if (iterator(all_elements_.end()) == iterator_for_find_key) {
        insert({sample_key_for_iterating, ValueType()});
        iterator_for_find_key = find(sample_key_for_iterating);
    }
    return iterator_for_find_key->second;
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::erase(KeyType eraseElem) {
    size_t Polynomial_hash =
        function_hash_number_(eraseElem) % hash_table_size_quantity_elements_;
    for (auto Myit = hash_table_[Polynomial_hash].begin();
         Myit != hash_table_[Polynomial_hash].end(); ++Myit) {
        KeyType def_prom = (*Myit)->first;
        if (eraseElem == def_prom) {
            all_elements_.erase(*Myit);
            hash_table_[Polynomial_hash].erase(Myit);
            num_hash_table_elements_--;
            return;
        }
    }
}

template <class KeyType, class ValueType, class Hash>
typename list<pair<const KeyType, ValueType>>::iterator
HashMap<KeyType, ValueType, Hash>::find(KeyType sample_key_for_iterating) {
    size_t hashFunc = function_hash_number_(sample_key_for_iterating) %
                      hash_table_size_quantity_elements_;
    for (auto& position_in_quant : hash_table_[hashFunc]) {
        KeyType def_prom = position_in_quant->first;
        if (sample_key_for_iterating == def_prom) {
            return iterator(position_in_quant);
        }
    }
    return iterator(all_elements_.end());
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>& HashMap<KeyType, ValueType, Hash>::operator=(
    HashMap<KeyType, ValueType, Hash> const& element_position_in_object) {
    auto iterator_for_iterating = this;
    if (&element_position_in_object == iterator_for_iterating) {
        return *iterator_for_iterating;
    }
    for (auto& finderIter : all_elements_) {
        size_t hashFunction = function_hash_number_(finderIter.first) %
                              hash_table_size_quantity_elements_;
        hash_table_[hashFunction].clear();
    }
    all_elements_.clear();
    num_hash_table_elements_ = 0;
    function_hash_number_ = element_position_in_object.hash_function();
    hash_table_.resize(element_position_in_object.hash_table_size_quantity_elements_);
    for (auto& foundIter : element_position_in_object) {
        insert(foundIter);
    }
    return *iterator_for_iterating;
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(Hash hasherObj)
    : function_hash_number_(hasherObj) {
    hash_table_size_quantity_elements_ = initialTableSize_;
    hash_table_.resize(hash_table_size_quantity_elements_);
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(
    initializer_list<pair<KeyType, ValueType>> list, Hash hasherObj)
    : function_hash_number_(hasherObj) {
    hash_table_size_quantity_elements_ = initialTableSize_;
    hash_table_.resize(hash_table_size_quantity_elements_);
    for (auto& pos_in_list : list) {
        insert(pos_in_list);
    }
}

template <class KeyType, class ValueType, class Hash>
template <typename IteratorType>
HashMap<KeyType, ValueType, Hash>::HashMap(IteratorType begin, IteratorType end,
                                           Hash hasherObj)
    : function_hash_number_(hasherObj) {
    hash_table_size_quantity_elements_ = initialTableSize_;
    hash_table_.resize(hash_table_size_quantity_elements_);
    for (; begin != end; ++begin) {
        insert(*begin);
    }
}

template <class KeyType, class ValueType, class Hash>
Hash HashMap<KeyType, ValueType, Hash>::hash_function() const {
    return function_hash_number_;
}

template <class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::empty() const {
    return !num_hash_table_elements_;
}

template <class KeyType, class ValueType, class Hash>
typename list<pair<const KeyType, ValueType>>::iterator
HashMap<KeyType, ValueType, Hash>::begin() {
    return iterator(all_elements_.begin());
}

template <class KeyType, class ValueType, class Hash>
typename list<pair<const KeyType, ValueType>>::const_iterator
HashMap<KeyType, ValueType, Hash>::end() const {
    return all_elements_.end();
}

template <class KeyType, class ValueType, class Hash>
typename list<pair<const KeyType, ValueType>>::iterator
HashMap<KeyType, ValueType, Hash>::end() {
    return iterator(all_elements_.end());
}

template <class KeyType, class ValueType, class Hash>
typename list<pair<const KeyType, ValueType>>::const_iterator
HashMap<KeyType, ValueType, Hash>::begin() const {
    return all_elements_.begin();
}

template <class KeyType, class ValueType, class Hash>
size_t HashMap<KeyType, ValueType, Hash>::size() const {
    return num_hash_table_elements_;
}
