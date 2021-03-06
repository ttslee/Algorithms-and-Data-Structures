// HashSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// A HashSet is an implementation of a Set that is a separately-chained
// hash table, implemented as a dynamically-allocated array of linked
// lists.  At any given time, the HashSet has a "size" indicating
// how many elements are stored within it, along with a "capacity"
// indicating the size of the array.
//
// As elements are added to the HashSet and the proportion of the HashSet's
// size to its capacity exceeds 0.8 (i.e., there are more than 80% as many
// elements as there are array cells), the HashSet should be resized so
// that it is twice as large as it was before.
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the information
// in your data structure.  Instead, you'll need to use a dynamically-
// allocated array and your own linked list implemenation; the linked list
// doesn't have to be its own class, though you can do that, if you'd like.

#ifndef HASHSET_HPP
#define HASHSET_HPP

#include <functional>
#include "Set.hpp"
#include "MyList.hpp"
#include <chrono>
#include <iostream>

template <typename ElementType>
class HashSet : public Set<ElementType>
{
public:
    // The default capacity of the HashSet before anything has been
    // added to it.
    static constexpr unsigned int DEFAULT_CAPACITY = 10;

    // A HashFunction is a function that takes a reference to a const
    // ElementType and returns an unsigned int.
    using HashFunction = std::function<unsigned int(const ElementType&)>;

public:
    // Initializes a HashSet to be empty, so that it will use the given
    // hash function whenever it needs to hash an element.
    explicit HashSet(HashFunction hashFunction);

    // Cleans up the HashSet so that it leaks no memory.
    virtual ~HashSet() noexcept;

    // Initializes a new HashSet to be a copy of an existing one.
    HashSet(const HashSet& s);

    // Initializes a new HashSet whose contents are moved from an
    // expiring one.
    HashSet(HashSet&& s) noexcept;

    // Assigns an existing HashSet into another.
    HashSet& operator=(const HashSet& s);

    // Assigns an expiring HashSet into another.
    HashSet& operator=(HashSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a HashSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function triggers a resizing of the
    // array when the ratio of size to capacity would exceed 0.8.  In the case
    // where the array is resized, this function runs in linear time (with
    // respect to the number of elements, assuming a good hash function);
    // otherwise, it runs in constant time (again, assuming a good hash
    // function).
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in constant time (with respect
    // to the number of elements, assuming a good hash function).
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // elementsAtIndex() returns the number of elements that hashed to a
    // particular index in the array.  If the index is out of the boundaries
    // of the array, this function returns 0.
    unsigned int elementsAtIndex(unsigned int index) const;


    // isElementAtIndex() returns true if the given element hashed to a
    // particular index in the array, false otherwise.  If the index is
    // out of the boundaries of the array, this functions returns 0.
    bool isElementAtIndex(const ElementType& element, unsigned int index) const;


private:
    int numElements;
    int capacity;
    MyList<ElementType>* list;
    HashFunction hashFunction;
};



namespace impl_
{
    template <typename ElementType>
    unsigned int HashSet__undefinedHashFunction(const ElementType& element)
    {
        return 0;
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashFunction hashFunction)
    : numElements{0}, capacity{DEFAULT_CAPACITY}, list{new MyList<ElementType>[DEFAULT_CAPACITY]}, hashFunction{hashFunction}  
{
}


template <typename ElementType>
HashSet<ElementType>::~HashSet() noexcept
{  
    numElements = 0;
    capacity = DEFAULT_CAPACITY;
    if(list != nullptr)
        delete[] list;
}


template <typename ElementType>
HashSet<ElementType>::HashSet(const HashSet& s)
    : numElements{s.numElements}, capacity{s.capacity}, list{new MyList<ElementType>[s.capacity]}, hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>} 
{
    for (int i = 0; i < capacity; ++i)
    {
        list[i] = s.list[i];
    }
}


template <typename ElementType>
HashSet<ElementType>::HashSet(HashSet&& s) noexcept
    : numElements{s.numElements}, capacity{s.capacity}, list{new MyList<ElementType>[s.capacity]}, hashFunction{impl_::HashSet__undefinedHashFunction<ElementType>} 
{
    for (int i = 0; i < capacity; ++i)
    {
        list[i] = std::move(s.list[i]);
    }
    s.numElements = 0;
    s.capacity = DEFAULT_CAPACITY;
    delete[] s.list;
    s.list = new MyList<ElementType>[DEFAULT_CAPACITY];
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(const HashSet& s)
{
    if(this != &s)
    {
        delete[] list;
        numElements = s.numElements;
        capacity = s.capacity;
        list = new MyList<ElementType>[capacity];
        for (int i = 0; i < capacity; ++i)
        {
            list[i] = s.list[i];
        }
    }
    return *this;
}


template <typename ElementType>
HashSet<ElementType>& HashSet<ElementType>::operator=(HashSet&& s) noexcept
{
    if(this != &s)
    {
        delete[] list;
        numElements = s.numElements;
        capacity = s.capacity;
        list = new MyList<ElementType>[capacity];
        for (int i = 0; i < capacity; ++i)
        {
            list[i] = std::move(s.list[i]);
        }
        s.numElements = 0;
        s.capacity = DEFAULT_CAPACITY;
        delete[] s.list;
        s.list = new MyList<ElementType>[DEFAULT_CAPACITY];
    }
    return *this;
}


template <typename ElementType>
bool HashSet<ElementType>::isImplemented() const noexcept
{
    return true;
}


template <typename ElementType>
void HashSet<ElementType>::add(const ElementType& element)
{
    if(contains(element))
        return;
    if(((1.0 * size() + 1) / capacity) > 0.8)
    {
        int newCap = capacity * 2;
        MyList<ElementType>* temp = new MyList<ElementType>[newCap];
        for (int i = 0; i < capacity; ++i)
        {
            Node<ElementType>* it = list[i].getFront();
            while(it != nullptr)
            {
                temp[hashFunction(it->data)%newCap].insert(it->data);
                it = it->next;
            }
        }
        delete[] list;
        list = temp;
        capacity = newCap;
        list[hashFunction(element)%capacity].insert(element);
        numElements++;
    }
    else
    {
        list[hashFunction(element)%capacity].insert(element);
        numElements++;
    }
}


template <typename ElementType>
bool HashSet<ElementType>::contains(const ElementType& element) const
{
    return list[hashFunction(element)%capacity].contains(element);
}


template <typename ElementType>
unsigned int HashSet<ElementType>::size() const noexcept
{
    return numElements;
}


template <typename ElementType>
unsigned int HashSet<ElementType>::elementsAtIndex(unsigned int index) const
{
    return list[index].size();
}


template <typename ElementType>
bool HashSet<ElementType>::isElementAtIndex(const ElementType& element, unsigned int index) const
{
    if(index >= numElements)
        return 0;
    else
        return list[index].contains(element);
}



#endif // HASHSET_HPP

