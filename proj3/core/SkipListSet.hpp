// SkipListSet.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// A SkipListSet is an implementation of a Set that is a skip list, implemented
// as we discussed in lecture.  A skip list is a sequence of levels
//
// You are not permitted to use the containers in the C++ Standard Library
// (such as std::set, std::map, or std::vector) to store the keys and their
// values.  Instead, you'll need to implement your own dynamically-allocated
// nodes, with pointers connecting them.  You can, however, use other parts of
// the C++ Standard Library -- including <random>, notably.
//
// Each node should contain only two pointers: one to the node that follows it
// on the same level and another to the equivalent node on the level below it.
// Additional pointers use more memory but don't enable any techniques not
// enabled by the other two.
//
// A couple of utilities are included here: SkipListKind and SkipListKey.
// You can feel free to use these as-is and probably will not need to
// modify them, though you can make changes to them, if you'd like.

#ifndef SKIPLISTSET_HPP
#define SKIPLISTSET_HPP

#include <memory>
#include <random>
#include "Set.hpp"
#include <iostream>
#include <chrono>


// SkipListKind indicates a kind of key: a normal one, the special key
// -INF, or the special key +INF.  It's necessary for us to implement
// the notion of -INF and +INF separately, since we're building a class
// template and not all types of keys would have a reasonable notion of
// -INF and +INF.

enum class SkipListKind
{
    Normal,
    NegInf,
    PosInf
};




// A SkipListKey represents a single key in a skip list.  It is possible
// to compare these keys using < or == operators (which are overloaded here)
// and those comparisons respect the notion of whether each key is normal,
// -INF, or +INF.

template <typename ElementType>
class SkipListKey
{
public:
    SkipListKey(SkipListKind kind, const ElementType& element);

    bool operator==(const SkipListKey& other) const;
    bool operator<(const SkipListKey& other) const;
    
private:
    SkipListKind kind;
    ElementType element;
   
};


template <typename ElementType>
SkipListKey<ElementType>::SkipListKey(SkipListKind kind, const ElementType& element)
    : kind{kind}, element{element}
{
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator==(const SkipListKey& other) const
{
    return kind == other.kind
        && (kind != SkipListKind::Normal || element == other.element);
}


template <typename ElementType>
bool SkipListKey<ElementType>::operator<(const SkipListKey& other) const
{
    switch (kind)
    {
    case SkipListKind::NegInf:
        return other.kind != SkipListKind::NegInf;

    case SkipListKind::PosInf:
        return false;

    default: // SkipListKind::Normal
        return other.kind == SkipListKind::PosInf
            || (other.kind == SkipListKind::Normal && element < other.element);
    }
}



// The SkipListLevelTester class represents the ability to decide whether
// a key placed on one level of the skip list should also occupy the next
// level.  This is the "coin flip," so to speak.  Note that this is an
// abstract base class with one implementation, RandomSkipListLevelTester,
// just below it.  RandomSkipListLevelTester is what it sounds like: It
// makes the decision at random (with a 50/50 chance of deciding whether
// a key should occupy the next level).  However, by setting things up
// this way, we have a way to control things more carefully in our
// testing (as you can, as well).
//
// DO NOT MAKE CHANGES TO THE SIGNATURES OF THE MEMBER FUNCTIONS OF
// THE "level tester" CLASSES.  You can add new member functions or even
// whole new level tester classes, but the ones declared below are part
// of how we test your implementation, so they need to maintain the
// parts of the interface that are declared already.

template <typename ElementType>
class SkipListLevelTester
{
public:
    virtual ~SkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) = 0;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() = 0;
};


template <typename ElementType>
class RandomSkipListLevelTester : public SkipListLevelTester<ElementType>
{
public:
    RandomSkipListLevelTester();
    virtual ~RandomSkipListLevelTester() = default;

    virtual bool shouldOccupyNextLevel(const ElementType& element) override;
    virtual std::unique_ptr<SkipListLevelTester<ElementType>> clone() override;

private:
    std::default_random_engine engine;
    std::bernoulli_distribution distribution;
};


template <typename ElementType>
RandomSkipListLevelTester<ElementType>::RandomSkipListLevelTester()
    : engine{std::random_device{}()}, distribution{0.5}
{
}


template <typename ElementType>
bool RandomSkipListLevelTester<ElementType>::shouldOccupyNextLevel(const ElementType& element)
{
    return distribution(engine);
}


template <typename ElementType>
std::unique_ptr<SkipListLevelTester<ElementType>> RandomSkipListLevelTester<ElementType>::clone()
{
    return std::unique_ptr<SkipListLevelTester<ElementType>>{
        new RandomSkipListLevelTester<ElementType>};
}




template <typename ElementType>
class SkipListSet : public Set<ElementType>
{
public:
    // Initializes an SkipListSet to be empty, with or without a
    // "level tester" object that will decide, whenever a "coin flip"
    // is needed, whether a key should occupy the next level above.
    SkipListSet();
    explicit SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester);

    // Cleans up the SkipListSet so that it leaks no memory.
    virtual ~SkipListSet() noexcept;

    // Initializes a new SkipListSet to be a copy of an existing one.
    SkipListSet(const SkipListSet& s);

    // Initializes a new SkipListSet whose contents are moved from an
    // expiring one.
    SkipListSet(SkipListSet&& s) noexcept;

    // Assigns an existing SkipListSet into another.
    SkipListSet& operator=(const SkipListSet& s);

    // Assigns an expiring SkipListSet into another.
    SkipListSet& operator=(SkipListSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement a SkipListSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function runs in an expected time
    // of O(log n) (i.e., over the long run, we expect the average to be
    // O(log n)) with very high probability.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function runs in an expected time of O(log n)
    // (i.e., over the long run, we expect the average to be O(log n))
    // with very high probability.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // levelCount() returns the number of levels in the skip list.
    unsigned int levelCount() const noexcept;


    // elementsOnLevel() returns the number of elements that are stored
    // on the given level of the skip list.  Level 0 is the bottom level;
    // level 1 is the one above level 0; and so on.  If the given level
    // doesn't exist, this function returns 0.  (Note that the -INF
    // and +INF shouldn't be counted.)
    unsigned int elementsOnLevel(unsigned int level) const noexcept;


    // isElementOnLevel() returns true if the given element is on the
    // given level, false otherwise.  Level 0 is the bottom level; level 1
    // is the one above level 0; and so on.  If the given level doesn't
    // exist, this function returns false.
    bool isElementOnLevel(const ElementType& element, unsigned int level) const;
    
private:
    struct SkipListNode
    {
        SkipListKey<ElementType> key;
        SkipListNode* sameLevelNext;
        SkipListNode* nextLevel;
    };
    class LevelList
    {
    public:
        LevelList();

        ~LevelList();

        LevelList(const LevelList& s);

        LevelList(LevelList&& s);

        LevelList& operator=(const LevelList& s);

        LevelList& operator=(LevelList&& s);

        bool contains(const SkipListKey<ElementType>& key) const;

        SkipListNode* getHead() const { return head; }

        int size() const;
    private:
        SkipListNode* copy(const LevelList& s);
        void destroy();
        SkipListNode* head;

    };

private:
    void insert(const SkipListKey<ElementType>& element, SkipListNode* node, SkipListNode* above, int targetLevel, int currentLevel);
    LevelList* copy(const SkipListSet& s);
    void linkNodes(int depth);
    std::unique_ptr<SkipListLevelTester<ElementType>> levelTester;
    LevelList* list;
    int listSize;
};

// LevelList implementation
template<typename ElementType>
SkipListSet<ElementType>::LevelList::LevelList() 
    : head{new SkipListNode{SkipListKey<ElementType>{SkipListKind::NegInf, ElementType{}}, 
            new SkipListNode{SkipListKey<ElementType>{SkipListKind::PosInf, ElementType{}}, nullptr, nullptr}, nullptr}}
{
     // std::cout << "CONS" << std::endl;
}

template<typename ElementType>
SkipListSet<ElementType>::LevelList::LevelList(const LevelList& s) : head{nullptr}
{
    head = copy(s);
}

template<typename ElementType>
SkipListSet<ElementType>::LevelList::LevelList(LevelList&& s)
    : head{nullptr}
{
    std::swap(head, s.head);
}

template<typename ElementType>
typename SkipListSet<ElementType>::LevelList& SkipListSet<ElementType>::LevelList::operator=(const LevelList& s)
{
    if(this != &s)
    {
        destroy();
        head = copy(s);
        // head = nullptr;
    }
    return *this;
}

template<typename ElementType>
typename SkipListSet<ElementType>::LevelList& SkipListSet<ElementType>::LevelList::operator=(LevelList&& s)
{
    // std::cout << "A" << std::endl;
    if(this != &s)
    {
        std::swap(head, s.head);
    }
    return *this;
}

template<typename ElementType>
SkipListSet<ElementType>::LevelList::~LevelList()
{
    destroy();
}

template<typename ElementType>
bool SkipListSet<ElementType>::LevelList::contains(const SkipListKey<ElementType>& key) const
{
    if(head == nullptr)
        return false;
    SkipListNode* it = head;
    while(it != nullptr)
    {
        if(key < it->key)
            return false;
        if(it->key == key)
            return true;
        it = it->sameLevelNext;
    }
    return false;
}

template<typename ElementType>
typename SkipListSet<ElementType>::SkipListNode* SkipListSet<ElementType>::LevelList::copy(const LevelList& s)
{
    SkipListNode* other = s.head;
    SkipListNode* newHead = nullptr;
    SkipListNode* it = nullptr;
    try
    {
        if(other != nullptr)
        {
            newHead = new SkipListNode{other->key, nullptr, nullptr};
            it = newHead;
        }
        
        while(other != nullptr)
        {
            it->sameLevelNext = new SkipListNode{other->key, nullptr, nullptr};
            it = it->sameLevelNext;
            other = other->sameLevelNext;
        }
    }
    catch(...)
    {
        destroy();
        throw;
    }
    

    return newHead;
}

template<typename ElementType>
void SkipListSet<ElementType>::LevelList::destroy()
{
    
    SkipListNode* it = head;

    while (it != nullptr)
    {
        SkipListNode* temp = it;
        it = it->sameLevelNext;
        delete temp;
    }
    

}

template<typename ElementType>
int SkipListSet<ElementType>::LevelList::size() const
{
    int count = 0;
    SkipListNode* it = head;
    while(it != nullptr)
    {
        it = it->sameLevelNext;
        count++;
    }
    return count - 2;
}

template <typename ElementType>
SkipListSet<ElementType>::SkipListSet()
    : SkipListSet{std::make_unique<RandomSkipListLevelTester<ElementType>>()}
{

}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(std::unique_ptr<SkipListLevelTester<ElementType>> levelTester)
    : levelTester{std::move(levelTester)}, list{new LevelList[1]}, listSize{1}
{
}


template <typename ElementType>
SkipListSet<ElementType>::~SkipListSet() noexcept
{
    delete[] list;
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(const SkipListSet& s) : list{nullptr}, listSize{0}
{
    list = copy(s);
}


template <typename ElementType>
SkipListSet<ElementType>::SkipListSet(SkipListSet&& s) noexcept : list{nullptr}, listSize{0}
{
     std::swap(list, s.list);
     listSize = s.listSize;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(const SkipListSet& s)
{
    if(this != &s)
    {
        delete[] list;
        list = copy(s);
    }
    return *this;
}


template <typename ElementType>
SkipListSet<ElementType>& SkipListSet<ElementType>::operator=(SkipListSet&& s) noexcept
{
    if(this != &s)
    {
        std::swap(list, s.list);
        listSize = s.listSize;
    }
    return *this;
}


template <typename ElementType>
bool SkipListSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

template<typename ElementType>
void SkipListSet<ElementType>::insert(const SkipListKey<ElementType>& element, SkipListNode* node, SkipListNode* above, int targetLevel, int currentLevel)
{   
    if(node == nullptr)
        return;
    if(element < node->sameLevelNext->key)
    {
        if(targetLevel == currentLevel)
        {
            node->sameLevelNext = new SkipListNode{element, node->sameLevelNext, nullptr};
            if(above != nullptr)
                above->nextLevel = node->sameLevelNext;
            insert(element, node->nextLevel, node->sameLevelNext, targetLevel--, currentLevel--);
        }
        else
            insert(element, node->nextLevel, above, targetLevel, currentLevel--);

    }
    else
        insert(element, node->sameLevelNext, above, targetLevel, currentLevel);
}

template <typename ElementType>
void SkipListSet<ElementType>::add(const ElementType& element)
{
    
    if(!contains(element))
    {

        SkipListKey<ElementType> key{SkipListKind::Normal, element};
        int currentLevel = 0;
        while(levelTester->shouldOccupyNextLevel(element))
            currentLevel++;
        if(currentLevel >= listSize)
        {
            int prev = listSize;
            LevelList* temp = new LevelList[currentLevel + 1];
            for (int i = 0; i < listSize; ++i)  
            {
                temp[i] = std::move(list[i]);
            }
            delete[] list;
            listSize = currentLevel + 1;
            list = temp;
            linkNodes(listSize - prev);
        }
        insert(key, list[listSize - 1].getHead(), nullptr, currentLevel, listSize - 1);
    }
    
    
}


template <typename ElementType>
bool SkipListSet<ElementType>::contains(const ElementType& element) const
{

    SkipListKey<ElementType> temp{SkipListKind::Normal, element};
    int currentLevel = listSize - 1;
    SkipListNode* it = list[currentLevel].getHead();
    while(it != nullptr)
    {
        if(it->key == temp)
            return true;
        else if(temp < it->sameLevelNext->key)
        {
            if(currentLevel == 0)
                return false;
            currentLevel--;
            it = it->nextLevel;
        }
        else
            it = it->sameLevelNext;
    }
    
    return false;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::size() const noexcept
{
    return list[0].size();
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::levelCount() const noexcept
{
    return listSize;
}


template <typename ElementType>
unsigned int SkipListSet<ElementType>::elementsOnLevel(unsigned int level) const noexcept
{
    if(level >= listSize)
        return 0;
    return list[level].size();
}


template <typename ElementType>
bool SkipListSet<ElementType>::isElementOnLevel(const ElementType& element, unsigned int level) const
{
    SkipListKey<ElementType> el{SkipListKind::Normal, element};
    if(level >= listSize)
        return 0;
    return list[level].contains(el);
}

template <typename ElementType>
typename SkipListSet<ElementType>::LevelList* SkipListSet<ElementType>::copy(const SkipListSet& s)
{
    listSize = s.listSize;
    LevelList* newList = new LevelList[listSize]{};
    for (int i = 0; i < listSize; ++i)
    {
        newList[i] = s.list[i];
    }

    linkNodes(listSize - 1);
    return newList;
}



template <typename ElementType>
void SkipListSet<ElementType>::linkNodes(int depth)
{
    int currentLevel = listSize - 1;
    for (int i = 0; i < depth; ++i)
    {
        SkipListNode* it = list[currentLevel].getHead();
        SkipListNode* mover = list[currentLevel - 1].getHead();
        while(it != nullptr)
        {
            if(it->key == mover->key)
            {
                it->nextLevel = mover;
                it = it->sameLevelNext;
                mover = mover->sameLevelNext;
            }
            else
            {
                mover = mover->sameLevelNext;
            }
        }
        currentLevel--;
    }
}
#endif // SKIPLISTSET_HPP

