#ifndef AVLSET_HPP
#define AVLSET_HPP

#include <functional>
#include "Set.hpp"
#include <iostream>

enum class LeftRight{
    left, right
};


template <typename ElementType>
class AVLSet : public Set<ElementType>
{
public:
    // A VisitFunction is a function that takes a reference to a const
    // ElementType and returns no value.
    using VisitFunction = std::function<void(const ElementType&)>;

public:
    // Initializes an AVLSet to be empty, with or without balancing.
    explicit AVLSet(bool shouldBalance = true);

    // Cleans up the AVLSet so that it leaks no memory.
    virtual ~AVLSet() noexcept;

    // Initializes a new AVLSet to be a copy of an existing one.
    AVLSet(const AVLSet& s);

    // Initializes a new AVLSet whose contents are moved from an
    // expiring one.
    AVLSet(AVLSet&& s) noexcept;

    // Assigns an existing AVLSet into another.
    AVLSet& operator=(const AVLSet& s);

    // Assigns an expiring AVLSet into another.
    AVLSet& operator=(AVLSet&& s) noexcept;


    // isImplemented() should be modified to return true if you've
    // decided to implement an AVLSet, false otherwise.
    virtual bool isImplemented() const noexcept override;


    // add() adds an element to the set.  If the element is already in the set,
    // this function has no effect.  This function always runs in O(log n) time
    // when there are n elements in the AVL tree.
    virtual void add(const ElementType& element) override;


    // contains() returns true if the given element is already in the set,
    // false otherwise.  This function always runs in O(log n) time when
    // there are n elements in the AVL tree.
    virtual bool contains(const ElementType& element) const override;


    // size() returns the number of elements in the set.
    virtual unsigned int size() const noexcept override;


    // height() returns the height of the AVL tree.  Note that, by definition,
    // the height of an empty tree is -1.
    int height() const;


    // preorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a preorder traversal of the AVL
    // tree.
    void preorder(VisitFunction visit) const;


    // inorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by an inorder traversal of the AVL
    // tree.
    void inorder(VisitFunction visit) const;


    // postorder() calls the given "visit" function for each of the elements
    // in the set, in the order determined by a postorder traversal of the AVL
    // tree.
    void postorder(VisitFunction visit) const;

private:
    struct AVLNode
    {
        ElementType data;
        AVLNode* left;
        AVLNode* right;
        int height;
    };
private:
    AVLNode* newNode(const ElementType& element);

    AVLNode* unbalancedInsert(AVLNode* node, const ElementType& element);

    AVLNode* balancedInsert(AVLNode* node, const ElementType& element);

    AVLNode* rotateRight(AVLNode* node);

    AVLNode* rotateLeft(AVLNode* node);

    void preOrderHelper(VisitFunction visit, AVLNode* node);

    void postOrderHelper(VisitFunction visit, AVLNode* node);

    void inOrderHelper(VisitFunction visit, AVLNode* node);

    void preOrderHelper(VisitFunction visit, AVLNode* node) const;

    void postOrderHelper(VisitFunction visit, AVLNode* node) const;

    void inOrderHelper(VisitFunction visit, AVLNode* node) const;

    bool lookUp(AVLNode* node, const ElementType& element) const;

    // bool unbalancedLookUp(AVLNode* node, const ElementType& element) const;
    void destroy(AVLNode* node);

    int height(AVLNode* node) const;

    int getBalance(AVLNode* node) const;

    bool balanceFlag;
    AVLNode* root;
    int numElements;
    // You'll no doubt want to add member variables and "helper" member
    // functions here.
};


template <typename ElementType>
AVLSet<ElementType>::AVLSet(bool shouldBalance) : balanceFlag{shouldBalance}, root{nullptr}, numElements{0}
{
}


template <typename ElementType>
AVLSet<ElementType>::~AVLSet() noexcept
{
    destroy(root);
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(const AVLSet& s) : balanceFlag{s.balanceFlag}, root{nullptr}, numElements{0}
{
    if(balanceFlag)
        preOrderHelper([&](const ElementType& element){ root = balancedInsert(root, element); }, s.root);
    else
        preOrderHelper([&](const ElementType& element){ root = unbalancedInsert(root, element); }, s.root);
    numElements = s.numElements;
}


template <typename ElementType>
AVLSet<ElementType>::AVLSet(AVLSet&& s) noexcept : balanceFlag{s.balanceFlag},root{nullptr}, numElements{0} 
{
    std::swap(root, s.root);
    std::swap(numElements, s.numElements);
}


template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(const AVLSet& s)
{
    if(this != &s)
    {
        destroy(root);
        if(balanceFlag)
            preOrderHelper([&](const ElementType& element){ root = balancedInsert(root, element); }, s.root);
        else
            preOrderHelper([&](const ElementType& element){ root = unbalancedInsert(root, element); }, s.root);
        numElements = s.numElements;
    }
    return *this;
}

template <typename ElementType>
AVLSet<ElementType>& AVLSet<ElementType>::operator=(AVLSet&& s) noexcept
{
    if(this != &s)
    {
        std::swap(root, s.root);
        std::swap(numElements, s.numElements);
    }
    return *this;
}


template <typename ElementType>
bool AVLSet<ElementType>::isImplemented() const noexcept
{
    return true;
}

template <typename ElementType>
void AVLSet<ElementType>::add(const ElementType& element)
{
	if(!contains(element))
	{
	    if(balanceFlag)
	        root = balancedInsert(root, element);
	    else
	        root = unbalancedInsert(root, element);
	    numElements++;
	}
}

template <typename ElementType>
typename AVLSet<ElementType>::AVLNode* AVLSet<ElementType>::newNode(const ElementType& element)
{
    AVLNode* node;
    try
    {
        node = new AVLNode{element, nullptr, nullptr, 0};
    }
    catch(...)
    {
        destroy(node);
        throw;
    }

    return node;
}

template <typename ElementType>
typename AVLSet<ElementType>::AVLNode* AVLSet<ElementType>::unbalancedInsert(AVLNode* node, const ElementType& element)
{
    // ITERATIVE SOLUTION
    // if(node == nullptr)
    //     return newNode(element);
    // AVLNode** tempArr = new AVLNode*[node->height + 2];
    // AVLNode* curr = node;
    // int index = 0;
    // LeftRight lr;
    // while(curr != nullptr)
    // {
    //     if(element < curr->data)
    //     {
    //         lr = LeftRight::left;
    //         tempArr[index] = curr;
    //         index++;
    //         curr = curr->left;
    //     }
    //     else if(element > curr->data)
    //     {
    //         lr = LeftRight::right;
    //         tempArr[index] = curr;
    //         index++;
    //         curr = curr->right;
    //     }
    // }
    // if(lr == LeftRight::left)
    // {
    //     tempArr[index - 1]->left = newNode(element);
    // }
    // if(lr == LeftRight::right)
    // {
    //     tempArr[index - 1]->right = newNode(element);
    // }
    // for (int i = 0; i < index - 1; ++i)
    // {
    //     tempArr[i]->height++;
    // }
    // delete[] tempArr;
    if(node == nullptr)
        return newNode(element);
    if(element < node->data)
        node->left = unbalancedInsert(node->left, element);
    if(element > node->data)
        node->right = unbalancedInsert(node->right, element);
    node->height = 1 + std::max(height(node->left), height(node->right));
    return node;
}

template <typename ElementType>
typename AVLSet<ElementType>::AVLNode* AVLSet<ElementType>::balancedInsert(AVLNode* node, const ElementType& element)
{
    if(node == nullptr)
        return newNode(element);
    if(element == node->data)
        return node;
    if(element < node->data)
        node->left = balancedInsert(node->left, element);
    if(element > node->data)
        node->right = balancedInsert(node->right, element);

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // left left
    if (balance > 1 && element < node->left->data) 
        return rotateRight(node); 
    // right right 
    if (balance < -1 && element > node->right->data) 
        return rotateLeft(node); 

    // left right
    if(balance > 1 && element > node->left->data)
    {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // right left
    if(balance < -1 && element < node->right->data)
    {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

template <typename ElementType>
bool AVLSet<ElementType>::contains(const ElementType& element) const
{
    return lookUp(root, element);
}


template <typename ElementType>
unsigned int AVLSet<ElementType>::size() const noexcept
{
    return numElements;
}

template <typename ElementType>
int AVLSet<ElementType>::height() const
{
    return height(root);
}


template <typename ElementType>
void AVLSet<ElementType>::preorder(VisitFunction visit) const
{
    preOrderHelper(visit, root);
}


template <typename ElementType>
void AVLSet<ElementType>::inorder(VisitFunction visit) const
{
    inOrderHelper(visit, root);
}


template <typename ElementType>
void AVLSet<ElementType>::postorder(VisitFunction visit) const
{
    postOrderHelper(visit, root);
}
// rotate right with node as root.
template <typename ElementType>
typename AVLSet<ElementType>::AVLNode* AVLSet<ElementType>::rotateRight(AVLNode* node)
{
    AVLNode* pivot = node->left;
    AVLNode* pivotChild = pivot->right;

    pivot->right = node;
    node->left = pivotChild;

    node->height = 1 + std::max(height(node->left), height(node->right));
    pivot->height = 1 + std::max(height(pivot->left), height(pivot->right));

    return pivot;
}

// rotate left with node as root.
template <typename ElementType>
typename AVLSet<ElementType>::AVLNode* AVLSet<ElementType>::rotateLeft(AVLNode* node)
{
    AVLNode* pivot = node->right;
    AVLNode* pivotChild = pivot->left;

    pivot->left = node;
    node->right = pivotChild;

    node->height = 1 + std::max(height(node->left), height(node->right));
    pivot->height = 1 + std::max(height(pivot->left), height(pivot->right));

    return pivot;
}

template <typename ElementType>
void AVLSet<ElementType>::preOrderHelper(VisitFunction visit, AVLNode* node) const
{
    if(node != nullptr)
    {
        visit(node->data);
        preOrderHelper(visit, node->left);
        preOrderHelper(visit, node->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::preOrderHelper(VisitFunction visit, AVLNode* node) 
{
    if(node != nullptr)
    {
        visit(node->data);
        preOrderHelper(visit, node->left);
        preOrderHelper(visit, node->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postOrderHelper(VisitFunction visit, AVLNode* node) 
{
    if(node != nullptr)
    {
        postOrderHelper(visit, node->left);
        postOrderHelper(visit, node->right);
        visit(node->data);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inOrderHelper(VisitFunction visit, AVLNode* node) 
{
    if(node != nullptr)
    {
        inOrderHelper(visit, node->left);
        visit(node->data);
        inOrderHelper(visit, node->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::postOrderHelper(VisitFunction visit, AVLNode* node) const
{
    if(node != nullptr)
    {
        postOrderHelper(visit, node->left);
        postOrderHelper(visit, node->right);
        visit(node->data);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::inOrderHelper(VisitFunction visit, AVLNode* node) const
{
    if(node != nullptr)
    {
        inOrderHelper(visit, node->left);
        visit(node->data);
        inOrderHelper(visit, node->right);
    }
}

template <typename ElementType>
void AVLSet<ElementType>::destroy(AVLNode* node)
{
    if(node != nullptr)
    {
        destroy(node->left);
        destroy(node->right);
        delete node;
    }
}

template <typename ElementType>
int AVLSet<ElementType>::height(AVLNode* node) const
{
    if(node == nullptr)
        return -1;
    return node->height;
}

template <typename ElementType>
int AVLSet<ElementType>::getBalance(AVLNode* node) const
{
    if(node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

template <typename ElementType>
bool AVLSet<ElementType>::lookUp(AVLNode* node, const ElementType& element) const
{
    AVLNode* curr = node;
    while(curr != nullptr)
    {
        if(element == curr->data)
            return true;
        if(element < curr->data)
        {
            curr = curr->left;
        }
        else if(element > curr->data)
        {
            curr = curr->right;
        }
    }
    // if(node != nullptr)
    // {
    //     if(node->data == element)
    //         return true;
    //     if(element < node->data)
    //         return lookUp(node->left, element);
    //     if(element > node->data)
    //         return lookUp(node->right, element);
    // }
    return false;
}

#endif // AVLSET_HPP

