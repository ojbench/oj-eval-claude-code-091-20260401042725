#ifndef ESET_HPP
#define ESET_HPP

#include <set>
#include <utility>
#include <algorithm>
#include <stdexcept>
#include <memory>

template<class Key, class Compare = std::less<Key>>
class ESet {
private:
    // Use shared_ptr for copy-on-write semantics
    std::shared_ptr<std::set<Key, Compare>> data;

    // Ensure we have exclusive ownership before modifying
    void ensure_unique() {
        if (!data) {
            data = std::make_shared<std::set<Key, Compare>>();
        } else if (data.use_count() > 1) {
            data = std::make_shared<std::set<Key, Compare>>(*data);
        }
    }

    const std::set<Key, Compare>* get_data() const {
        return data ? data.get() : nullptr;
    }

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
            if (!container || it == container->end()) {
                throw std::out_of_range("Dereferencing end iterator");
            }
            return *it;
        }

        iterator& operator++() {
            if (container && it != container->end()) {
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
            if (container && it != container->begin()) {
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
            if (container == other.container) {
                return it == other.it;
            }
            return false;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    ESet() : data(nullptr) {}

    ~ESet() = default;

    // Copy constructor - shallow copy (copy-on-write)
    ESet(const ESet& other) : data(other.data) {}

    // Copy assignment - shallow copy (copy-on-write)
    ESet& operator=(const ESet& other) {
        if (this != &other) {
            data = other.data;
        }
        return *this;
    }

    // Move constructor
    ESet(ESet&& other) noexcept : data(std::move(other.data)) {}

    // Move assignment
    ESet& operator=(ESet&& other) noexcept {
        if (this != &other) {
            data = std::move(other.data);
        }
        return *this;
    }

    template<class... Args>
    std::pair<iterator, bool> emplace(Args&&... args) {
        ensure_unique();
        auto result = data->emplace(std::forward<Args>(args)...);
        return {iterator(result.first, data.get()), result.second};
    }

    size_t erase(const Key& key) {
        if (!data || data->empty()) return 0;
        ensure_unique();
        return data->erase(key);
    }

    iterator find(const Key& key) const {
        if (!data) {
            static std::set<Key, Compare> empty_set;
            return iterator(empty_set.end(), &empty_set);
        }
        auto it = const_cast<std::set<Key, Compare>*>(data.get())->find(key);
        return iterator(it, data.get());
    }

    size_t range(const Key& l, const Key& r) const {
        if (!data || data->empty()) return 0;
        if (Compare()(r, l)) return 0;
        auto it_l = data->lower_bound(l);
        auto it_r = data->upper_bound(r);
        return std::distance(it_l, it_r);
    }

    size_t size() const noexcept {
        return data ? data->size() : 0;
    }

    iterator lower_bound(const Key& key) const {
        if (!data) {
            static std::set<Key, Compare> empty_set;
            return iterator(empty_set.end(), &empty_set);
        }
        auto it = const_cast<std::set<Key, Compare>*>(data.get())->lower_bound(key);
        return iterator(it, data.get());
    }

    iterator upper_bound(const Key& key) const {
        if (!data) {
            static std::set<Key, Compare> empty_set;
            return iterator(empty_set.end(), &empty_set);
        }
        auto it = const_cast<std::set<Key, Compare>*>(data.get())->upper_bound(key);
        return iterator(it, data.get());
    }

    iterator begin() const noexcept {
        if (!data) {
            static std::set<Key, Compare> empty_set;
            return iterator(empty_set.begin(), &empty_set);
        }
        return iterator(const_cast<std::set<Key, Compare>*>(data.get())->begin(), data.get());
    }

    iterator end() const noexcept {
        if (!data) {
            static std::set<Key, Compare> empty_set;
            return iterator(empty_set.end(), &empty_set);
        }
        return iterator(const_cast<std::set<Key, Compare>*>(data.get())->end(), data.get());
    }
};

#endif // ESET_HPP
