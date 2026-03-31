#ifndef ESET_HPP
#define ESET_HPP

#include <set>
#include <utility>
#include <algorithm>
#include <stdexcept>

template<class Key, class Compare = std::less<Key>>
class ESet {
private:
    std::set<Key, Compare> data;

public:
    class iterator {
    private:
        typename std::set<Key, Compare>::iterator it;
        const std::set<Key, Compare>* container;

    public:
        iterator() : container(nullptr) {}
        iterator(typename std::set<Key, Compare>::iterator i, const std::set<Key, Compare>* c)
            : it(i), container(c) {}

        const Key& operator*() const {
            if (it == container->end()) {
                throw std::out_of_range("Dereferencing end iterator");
            }
            return *it;
        }

        iterator& operator++() {
            if (it != container->end()) {
                ++it;
            }
            return *this;
        }

        iterator operator++(int) {
            iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        iterator& operator--() {
            if (it != container->begin()) {
                --it;
            }
            return *this;
        }

        iterator operator--(int) {
            iterator tmp = *this;
            --(*this);
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return it == other.it;
        }

        bool operator!=(const iterator& other) const {
            return it != other.it;
        }
    };

    ESet() = default;
    ~ESet() = default;

    ESet(const ESet& other) : data(other.data) {}

    ESet& operator=(const ESet& other) {
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }

    ESet(ESet&& other) noexcept : data(std::move(other.data)) {}

    ESet& operator=(ESet&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
        }
        return *this;
    }

    template<class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        auto result = data.emplace(std::forward<Args>(args)...);
        return {iterator(result.first, &data), result.second};
    }

    size_t erase(const Key& key) {
        return data.erase(key);
    }

    iterator find(const Key& key) const {
        return iterator(const_cast<std::set<Key, Compare>&>(data).find(key), &data);
    }

    size_t range(const Key& l, const Key& r) const {
        if (Compare()(r, l)) return 0;
        auto it_l = data.lower_bound(l);
        auto it_r = data.upper_bound(r);
        return std::distance(it_l, it_r);
    }

    size_t size() const noexcept {
        return data.size();
    }

    iterator lower_bound(const Key& key) const {
        return iterator(const_cast<std::set<Key, Compare>&>(data).lower_bound(key), &data);
    }

    iterator upper_bound(const Key& key) const {
        return iterator(const_cast<std::set<Key, Compare>&>(data).upper_bound(key), &data);
    }

    iterator begin() const noexcept {
        return iterator(const_cast<std::set<Key, Compare>&>(data).begin(), &data);
    }

    iterator end() const noexcept {
        return iterator(const_cast<std::set<Key, Compare>&>(data).end(), &data);
    }
};

#endif // ESET_HPP
