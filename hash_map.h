#pragma once

// Copyright 2020 Ilya Chukanov

#include <algorithm>
#include <cmath>
#include <vector>
#include <list>
#include <stdexcept>

template <class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
    using KeyValuePair = std::pair<const KeyType, ValueType>;

 public:
    using const_iterator = typename std::list<KeyValuePair>::const_iterator;
    using iterator = typename std::list<KeyValuePair>::iterator;

    HashMap(Hash hasher_obj = Hash());

    HashMap(std::initializer_list<KeyValuePair> list, Hash hasher_obj = Hash());

    template <typename IteratorType>
    HashMap(IteratorType begin, IteratorType end, Hash hasher_obj = Hash());

    Hash hash_function() const;

    bool empty() const;

    void insert(const KeyValuePair& key_value_pair);

    iterator begin();

    const_iterator end() const;

    iterator end();

    HashMap& operator=(HashMap const& other);

    iterator find(const KeyType& key);

    void erase(const KeyType& key);

    const_iterator begin() const;

    size_t size() const;

    ValueType& operator[](const KeyType& key);

    const ValueType& at(const KeyType& key) const;

    const_iterator find(const KeyType& key) const;

    void clear();

 private:
    const size_t initialTableSize_ = 20;

    size_t hash_table_size_;

    size_t num_hash_table_elements_ = 0;

    std::vector<std::list<iterator>> hash_table_;

    std::list<KeyValuePair> all_elements_;

    Hash hash_function_;

    void DoubleSize();
};

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::insert(const KeyValuePair& key_value_pair) {
    size_t hash_table_cell =
        hash_function_(key_value_pair.first) % hash_table_size_;
    for (auto& element : hash_table_[hash_table_cell]) {
        if (element->first == key_value_pair.first) {
            return;
        }
    }
    all_elements_.push_back(key_value_pair);
    hash_table_[hash_table_cell].push_back(std::prev(all_elements_.end()));
    ++num_hash_table_elements_;
    DoubleSize();
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::DoubleSize() {
    // на первый взгляд кажется, что ничего не поменялось, но это не так, я
    // после добавления элемента тут же его удаляю из 2 списка, так что тут идет
    // +1 и -1 одновременно, так что нет удвоения хранимой памяти
    if (hash_table_size_ > num_hash_table_elements_ * 2) {
        return;
    }
    std::list<KeyValuePair> position_of_scale;
    while (!all_elements_.empty()) {
        position_of_scale.push_back(*all_elements_.begin());
        all_elements_.erase(all_elements_.begin());
    }
    hash_table_.clear();
    all_elements_.clear();
    hash_table_size_ = hash_table_size_ * 2;
    hash_table_.resize(hash_table_size_);
    num_hash_table_elements_ = 0;
    while (!position_of_scale.empty()) {
        insert(*position_of_scale.begin());
        position_of_scale.erase(position_of_scale.begin());
    }
    position_of_scale.clear();
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::clear() {
    for (auto& element : all_elements_) {
        size_t hash_table_cell =
            hash_function_(element.first) % hash_table_size_;
        hash_table_[hash_table_cell].clear();
    }
    all_elements_.clear();
    num_hash_table_elements_ = 0;
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::find(const KeyType& key) const
        -> const_iterator {
    size_t hash_table_cell = hash_function_(key) % hash_table_size_;
    for (auto& it : hash_table_[hash_table_cell]) {
        if (key == it->first) {
            return it;
        }
    }
    return all_elements_.end();
}

template <class KeyType, class ValueType, class Hash>
const ValueType& HashMap<KeyType, ValueType, Hash>::at(const KeyType& key) const {
    auto iterator_for_find_key = find(key);
    if (all_elements_.end() == iterator_for_find_key) {
        throw std::out_of_range("");
    }
    return iterator_for_find_key->second;
}

template <class KeyType, class ValueType, class Hash>
ValueType& HashMap<KeyType, ValueType, Hash>::operator[](const KeyType& key) {
    auto iterator_for_find_key = find(key);
    if (all_elements_.end() == iterator_for_find_key) {
        insert({key, ValueType()});
        iterator_for_find_key = find(key);
    }
    return iterator_for_find_key->second;
}

template <class KeyType, class ValueType, class Hash>
void HashMap<KeyType, ValueType, Hash>::erase(const KeyType& key) {
    size_t hash_table_cell = hash_function_(key) % hash_table_size_;
    for (auto it = hash_table_[hash_table_cell].begin();
         it != hash_table_[hash_table_cell].end(); ++it) {
        if (key == (*it)->first) {
            all_elements_.erase(*it);
            hash_table_[hash_table_cell].erase(it);
            num_hash_table_elements_--;
            return;
        }
    }
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::find(const KeyType& key) -> iterator {
    size_t hash_table_cell = hash_function_(key) % hash_table_size_;
    for (auto& it : hash_table_[hash_table_cell]) {
        if (key == it->first) {
            return it;
        }
    }
    return all_elements_.end();
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>& HashMap<KeyType, ValueType, Hash>::operator=(
        HashMap<KeyType, ValueType, Hash> const& other) {
    if (&other == this) {
        return *this;
    }
    for (auto& element : all_elements_) {
        size_t hash_table_cell =
            hash_function_(element.first) % hash_table_size_;
        hash_table_[hash_table_cell].clear();
    }
    all_elements_.clear();
    num_hash_table_elements_ = 0;
    hash_function_ = other.hash_function();
    hash_table_.resize(other.hash_table_size_);
    for (auto& element_in_other : other) {
        insert(element_in_other);
    }
    return *this;
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(Hash hasher_obj)
        : hash_function_(hasher_obj) {
    hash_table_size_ = initialTableSize_;
    hash_table_.resize(hash_table_size_);
}

template <class KeyType, class ValueType, class Hash>
HashMap<KeyType, ValueType, Hash>::HashMap(
        std::initializer_list<KeyValuePair> list, 
        Hash hasher_obj) : hash_function_(hasher_obj) {
    hash_table_size_ = initialTableSize_;
    hash_table_.resize(hash_table_size_);
    for (auto& pos_in_list : list) {
        insert(pos_in_list);
    }
}

template <class KeyType, class ValueType, class Hash>
template <typename IteratorType>
HashMap<KeyType, ValueType, Hash>::HashMap(IteratorType begin, IteratorType end,                              
       Hash hasher_obj) : hash_function_(hasher_obj) {
    hash_table_size_ = initialTableSize_;
    hash_table_.resize(hash_table_size_);
    for (; begin != end; ++begin) {
        insert(*begin);
    }
}

template <class KeyType, class ValueType, class Hash>
Hash HashMap<KeyType, ValueType, Hash>::hash_function() const {
    return hash_function_;
}

template <class KeyType, class ValueType, class Hash>
bool HashMap<KeyType, ValueType, Hash>::empty() const {
    return !num_hash_table_elements_;
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::begin() -> iterator {
    return all_elements_.begin();
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::end() const -> const_iterator {
    return all_elements_.cend();
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::end() -> iterator {
    return all_elements_.end();
}

template <class KeyType, class ValueType, class Hash>
auto HashMap<KeyType, ValueType, Hash>::begin() const -> const_iterator {
    return all_elements_.cbegin();
}

template <class KeyType, class ValueType, class Hash>
size_t HashMap<KeyType, ValueType, Hash>::size() const {
    return num_hash_table_elements_;
}
