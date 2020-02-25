// Copyright 2020 Ilya Chukanov

#include <algorithm>
#include <cmath>
#include <list>
#include <map>
#include <set>
#include <vector>
#include <utility>

#define BOOST_TEST_MODULE <yourtestName>

template <class KeyType, class ValueType, class Hash = std::hash<KeyType>>
class HashMap {
 private:
    int quantityElements;

    int amountNumbers = 0;

    std::vector<std::list<
        typename std::list<std::pair<const KeyType, ValueType>>::iterator>>
        significantSaver;

    std::list<std::pair<const KeyType, ValueType>> secondListSaving;

    Hash functionHash;

 public:
    using const_iterator =
        typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;
    using iterator =
        typename std::list<std::pair<const KeyType, ValueType>>::iterator;

    int spesialChangeType = 20;

    HashMap(Hash hasherObj = Hash()) : functionHash(hasherObj) {
        quantityElements = spesialChangeType;
        significantSaver.resize(quantityElements);
    }

    HashMap(std::initializer_list<std::pair<KeyType, ValueType>> list,
            Hash hasherObj = Hash())
        : functionHash(hasherObj) {
        quantityElements = spesialChangeType;
        significantSaver.resize(quantityElements);
        for (auto &pos_in_list : list) {
            insert(pos_in_list);
        }
    }

    template <typename NewSpecialType>
    HashMap(NewSpecialType begin, NewSpecialType end, Hash hasherObj = Hash())
        : functionHash(hasherObj) {
        quantityElements = spesialChangeType;
        significantSaver.resize(quantityElements);
        for (; begin != end; ++begin) {
            insert(*begin);
        }
    }

    Hash hash_function() const {
        auto ans = functionHash;
        return ans;
    }
    bool empty() const { return !amountNumbers; }

    void insert(std::pair<KeyType, ValueType> promoteElement) {
        int qhashPar = functionHash(promoteElement.first) % quantityElements;
        for (auto &autoIt : significantSaver[qhashPar]) {
            KeyType def_prom = autoIt->first;
            if (def_prom == promoteElement.first) {
                return;
            }
        }
        secondListSaving.push_back(promoteElement);
        auto Myiterator = secondListSaving.end();
        --Myiterator;
        significantSaver[qhashPar].push_back(Myiterator);

        ++amountNumbers;

        if (quantityElements > amountNumbers * 2) {
            return;
        }
        std::list<std::pair<KeyType, ValueType>> paceType;
        for (auto &myIter : secondListSaving) {
            paceType.push_back(std::make_pair(myIter.first, myIter.second));
        }
        significantSaver.clear();
        secondListSaving.clear();
        quantityElements = quantityElements * 2;
        significantSaver.resize(quantityElements);
        amountNumbers = 0;
        for (auto &system : paceType) {
            insert(system);
        }
        paceType.clear();
    }

    iterator begin() {
        auto ans = iterator(secondListSaving.begin());
        return ans;
    }
    const_iterator end() const {
        auto ans = secondListSaving.end();
        return ans;
    }
    iterator end() {
        auto ans = iterator(secondListSaving.end());
        return ans;
    }

    HashMap &operator=(HashMap const &finderOne) {
        auto finderOperator = this;
        if (&finderOne == finderOperator) {
            return *finderOperator;
        }
        for (auto &finderIter : secondListSaving) {
            int hashFunction =
                functionHash(finderIter.first) % quantityElements;
            significantSaver[hashFunction].clear();
        }
        secondListSaving.clear();
        amountNumbers = 0;
        functionHash = finderOne.hash_function();
        significantSaver.resize(finderOne.quantityElements);
        for (auto &foundIter : finderOne) {
            insert(foundIter);
        }
        return *finderOperator;
    }

    iterator find(KeyType keyExample) {
        int hashFunc = functionHash(keyExample) % quantityElements;
        for (auto &MyPos : significantSaver[hashFunc]) {
            KeyType def_prom = MyPos->first;
            if (keyExample == def_prom) {
                return iterator(MyPos);
            }
        }
        return iterator(secondListSaving.end());
    }

    void erase(KeyType eraseElem) {
        int PolHash = functionHash(eraseElem) % quantityElements;
        for (auto Myit = significantSaver[PolHash].begin();
             Myit != significantSaver[PolHash].end(); ++Myit) {
            KeyType def_prom = (*Myit)->first;
            if (eraseElem == def_prom) {
                secondListSaving.erase(*Myit);
                significantSaver[PolHash].erase(Myit);
                amountNumbers--;
                return;
            }
        }
    }

    const_iterator begin() const { return secondListSaving.begin(); }

    int size() const {
        int ans = amountNumbers;
        return ans;
    }

    ValueType &operator[](KeyType keyExample) {
        auto Myit = find(keyExample);
        if (iterator(secondListSaving.end()) == Myit) {
            insert({keyExample, ValueType()});
            Myit = find(keyExample);
        }
        return Myit->second;
    }

    const ValueType &at(KeyType keyexample) const {
        auto Myit = find(keyexample);
        if (secondListSaving.end() == Myit) {
            throw std::out_of_range("");
        }
        return Myit->second;
    }

    const_iterator find(KeyType keyexample) const {
        int hashFunc = functionHash(keyexample) % quantityElements;
        for (auto &fonderInter : significantSaver[hashFunc]) {
            KeyType def_prom = fonderInter->first;
            if (keyexample == def_prom) {
                return fonderInter;
            }
        }
        auto ans = secondListSaving.end();
        return ans;
    }

    void clear() {
        for (auto &finderIter : secondListSaving) {
            int hashFunction =
                functionHash(finderIter.first) % quantityElements;
            significantSaver[hashFunction].clear();
        }
        secondListSaving.clear();
        amountNumbers = 0;
    }
};
