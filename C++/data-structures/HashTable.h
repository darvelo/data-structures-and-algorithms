#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <list>
#include <cstddef>

static
float abs(float num) {
    return (num < 0) ? -num : num;
}

static
float sqrt(size_t num, float guess = 1) {
    // check if guess is close enough
    float difference = abs(guess * guess - num);
    if (difference < .001) {
        return guess;
    }

    // guess was not close enough.
    // compute a number closer to the sqrt
    // using an average of numbers on either side
    guess = (guess + num/guess) / 2;

    return sqrt(num, guess);
}

static
size_t nextPrime(size_t after = 2) {
    while (1) {
        size_t squareRoot = sqrt(++after);

        bool found = true;

        for (size_t i = 2; i <= squareRoot; ++i) {
            if (after % i == 0) {
                found = false;
                break;
            };
        }

        if (found) return after;
    }
}

template <typename HashedObj>
class HashTable {
public:
    explicit HashTable(size_t size = 101) : theLists(size) { };

    bool contains(const HashedObj& x) const;
    bool contains(const HashedObj&& x) const;
    HashedObj& access(const HashedObj& x) const;
    HashedObj& access(const HashedObj&& x) const;

    void MakeEmpty();
    bool insert(const HashedObj& x);
    bool insert(const HashedObj&& x);
    bool remove(const HashedObj& x);
    bool remove(const HashedObj&& x);

private:
    std::vector<std::list<HashedObj>> theLists;
    size_t currentSize = 0;

    void rehash();
    size_t myhash(const HashedObj& x) const;
};

template <typename HashedObj>
void HashTable<HashedObj>::rehash() {
    std::vector<std::list<HashedObj>> copy( nextPrime(theLists.size()) );

    currentSize = 0;
    std::swap(theLists, copy);

    // insert items into new hashtable
    for (auto& thisList : copy) {
        for (auto& obj : thisList) {
            insert(std::move(obj));
        }

        thisList.clear();
    }
}

// helper hash function template
template <typename Key>
class hash {
public:
    size_t operator()(const Key& k) const;
};

// actual hashing method for the hashed object type
template <typename HashedObj>
inline
size_t HashTable<HashedObj>::myhash(const HashedObj& x) const {
    static hash<HashedObj> hf;
    return hf(x) % theLists.size();
}

template <typename HashedObj>
inline
void HashTable<HashedObj>::MakeEmpty() {
    currentSize = 0;

    for (auto& thisList : theLists) {
        thisList.clear();
    }
}

template <typename HashedObj>
inline
bool HashTable<HashedObj>::contains(const HashedObj& x) const {
    auto& whichList = theLists[myhash(x)];
    return find(begin(whichList), end(whichList), x) != end(whichList);
}

template <typename HashedObj>
inline
bool HashTable<HashedObj>::contains(const HashedObj&& x) const {
    return contains(x);
}

template <typename HashedObj>
inline
HashedObj& HashTable<HashedObj>::access(const HashedObj& x) const {
    auto& whichList = theLists[myhash(x)];
    auto itr = find(begin(whichList), end(whichList), x);
    return const_cast<HashedObj&>(*itr);
}

template <typename HashedObj>
inline
HashedObj& HashTable<HashedObj>::access(const HashedObj&& x) const {
    return access(x);
}

template <typename HashedObj>
bool HashTable<HashedObj>::insert(const HashedObj& x) {
    auto& whichList = theLists[myhash(x)];

    if (find(begin(whichList), end(whichList), x) != end(whichList)) {
        return false;
    }

    whichList.push_back(x);

    // rehash if the ratio of items to table size gets too large
    if (++currentSize > theLists.size()) {
        rehash();
    }

    return true;
}

template <typename HashedObj>
bool HashTable<HashedObj>::insert(const HashedObj&& x) {
    auto& whichList = theLists[myhash(x)];

    if (find(begin(whichList), end(whichList), x) != end(whichList)) {
        return false;
    }

    whichList.push_back(std::move(x));

    // rehash if the ratio of items to table size gets too large
    if (++currentSize > theLists.size()) {
        rehash();
    }

    return true;
}

template <typename HashedObj>
bool HashTable<HashedObj>::remove(const HashedObj& x) {
    auto& whichList = theLists[myhash(x)];
    auto itr = find(begin(whichList), end(whichList), x);

    if (itr == end(whichList)) {
        return false;
    }

    whichList.erase(itr);
    --currentSize;
    return true;
}

template <typename HashedObj>
bool HashTable<HashedObj>::remove(const HashedObj&& x) {
    return remove(x);
}

#endif


#ifdef HASH_TABLE_TEST
#include <string>

// function object to hash strings
template <>
class hash<std::string> {
public:
    size_t operator()(const std::string& key) const {
        size_t hashVal = 0;

        for (char ch : key) {
            hashVal = 37 * hashVal + ch;
        }

        return hashVal;
    }
};

class Employee {
public:
    Employee(std::string name, int salary = 50'000)
        : name(name), salary(salary) { };
    const std::string& getName() const
        { return name; }
    int getSalary() const
        { return salary; }
    void setSalary(int newSalary) {
        salary = newSalary;
    }
    // must provide operator== and/or operator!= to be hashable
    bool operator==(const Employee& rhs) const
        { return getName() == rhs.getName(); }
    bool operator!=(const Employee& rhs) const
        { return !(*this == rhs); }

private:
    std::string name;
    int salary;
};

// must provide a hash function object for the class to be hashable
template<>
class hash<Employee> {
public:
    size_t operator()(const Employee& emp) const {
        static std::hash<std::string> hf;
        return hf(emp.getName());
    }
};

#endif
