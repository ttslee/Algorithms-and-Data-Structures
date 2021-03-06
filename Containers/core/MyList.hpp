#ifndef MYLIST_HPP
#define MYLIST_HPP 

template<typename T>
struct Node
{
	T data;
	Node<T>* next;
};

template<typename T>
class MyList
{
public:
	MyList();

	MyList(const MyList& other);

	MyList(MyList&& other);

	MyList& operator=(const MyList& other);

	MyList& operator=(MyList&& other);

	Node<T>* copy(const MyList& other);

	void insert(const T& element);
	
	Node<T>* getFront() const;

	int size();

	bool empty() const;

	bool contains(const T& element) const;

	void clear(Node<T>* node);

	~MyList();
private:
	Node<T>* front;
};

template<typename T>
MyList<T>::MyList() : front{nullptr}
{ 
}

template<typename T>
MyList<T>::MyList(const MyList& other) : front{nullptr}
{
	front = copy(other);
}

template<typename T>
MyList<T>::MyList(MyList&& other) : front{nullptr}
{
	std::swap(front, other.front);
}

template<typename T>
MyList<T>& MyList<T>::operator=(const MyList& other)
{
	if(this != &other)
	{
		Node<T>* temp = copy(other);
		clear(front);
		front = temp;
	}
	return *this;
}

template<typename T>
MyList<T>& MyList<T>::operator=(MyList&& other)
{
	if(this != &other)
	{
		std::swap(front, other.front);
	}
	return *this;
}

template<typename T>
Node<T>* MyList<T>::copy(const MyList& other) 
{
	Node<T>* temp = other.front;
    Node<T>* newFront = nullptr;

    try
    {
        while (temp != nullptr)
        {
            newFront = new Node<T>{temp->data, newFront};
            temp = temp->next;
        }
        return newFront;
    }
    catch (...)
    {
        clear(newFront);
        throw;
    }
}

template<typename T>
void MyList<T>::insert(const T& element)
{
	Node<T>* it = front;
	while(it != nullptr)
	{
		if(it->data == element)
			return;
		it = it->next;
	}
	front = new Node<T>{element, front};
}

template<typename T>
Node<T>* MyList<T>::getFront() const
{
	return front;
}

template<typename T>
bool MyList<T>::empty() const
{ 
	return front == nullptr; 
}

template<typename T>
bool MyList<T>::contains(const T& element) const
{
	if(empty())
		return false;
	Node<T>* it = front;
	while(it != nullptr)
	{
		if(it->data == element)
			return true;
		it = it->next;
	}
	return false;
}

template<typename T>
void MyList<T>::clear(Node<T>* front)
{
	Node<T>* it = front;

    while (it != nullptr)
    {
        Node<T>* temp = it;
        it = it->next;
        delete temp;
    }
}

template<typename T>
int MyList<T>::size()
{
	Node<T>* temp = front;
    unsigned int count = 0;

    while (temp != nullptr)
    {
        ++count;
        temp = temp->next;
    }
    return count;
}

template<typename T>
MyList<T>::~MyList()
{
	clear(front);
}
#endif