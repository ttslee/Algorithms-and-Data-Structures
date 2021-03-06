#include <iostream>

template<typename T>
class UniquePtr {
public:
	UniquePtr() : p(nullptr){}
	UniquePtr(T* point) :p(point) {}
	UniquePtr(const UniquePtr & rhs) = delete;
	UniquePtr(UniquePtr&& rhs) {
		p = rhs.p;
		rhs.p = nullptr;
	}

	UniquePtr& operator=(const UniquePtr&) = delete;
	UniquePtr& operator=(UniquePtr&& rhs) noexcept{
		delete p;
		p = rhs.p;
		rhs.p = nullptr;
		return *this;
	}
	~UniquePtr() { delete p; }
	T get(){ return *p; }
private:
	T *p;
};