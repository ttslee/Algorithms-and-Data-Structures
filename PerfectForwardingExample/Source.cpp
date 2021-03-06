#include <iostream>
#include <string>

//template<typename T>
//T minimum(T a, T b)
//{
//	if (a < b)
//		return a;
//	return b;
//}
//int main()
//{
//	int k;
//	std::cin >> k;
//	std::cout << minimum(k, 10) / 2;
//	double x = 2.0;
//
//	double y = 3 * minimum(x*x, x + 10);
//
//	int z = minimum(0, 3 * k - k + 4);
//}
class MyVector {
	int size;
	double * arr;
public:
	MyVector(int i) { arr = new double[i]; size = i; }
	// regular copy constructor
	MyVector(const MyVector &rhs) {
		//cout << "MOVE" << endl;
		size = rhs.size;
		arr = new double[size];
		for (int i = 0; i < size; i++) { arr[i] = rhs.arr[i]; }
	}

	// move copy constructor
	MyVector(MyVector &&rhs) {
		size = rhs.size;
		arr = rhs.arr;
		rhs.arr = nullptr;  // have to set this to nullptr
	}

	// assignment operator
	MyVector& operator=(const MyVector &rhs) {
		delete[] arr;
		size = rhs.size;
		for (int i = 0; i < size; i++) { arr[i] = rhs.arr[i]; }
		return *this;
	}

	// move assignment operator
	MyVector& operator=(MyVector &&rhs) {
		delete[] arr;
		size = rhs.size;
		arr = rhs.arr;
		rhs.arr = nullptr;  // have to set this to nullptr
		return *this;
	}

	~MyVector() { delete arr; }
};


void foo(MyVector arg)
{

}


MyVector createMyVector() { return MyVector(5); }

// Reference Collapsing Rules (c++11)
// 1) T& &   => T&
// 2) T& &&  => T&
// 3) T&& &  => T&
// 4) T&& && => T&&

template<typename T>
void relay(T&& arg) {   // Universal Type
	foo(std::forward<T>(arg)); // forward it as lvalue or rvalue
}

//void relay(MyVector & arg)
//{
//	foo(arg);
//}
//void relay(MyVector && arg)
//{
//	foo(std::move(arg));
//}
int main() { 
	MyVector reusable = createMyVector();
	relay(reusable); // relay(lvalue)
	// T = MyVector& => MyVector& && => MyVector &
	// ...
	relay(createMyVector()); // relay(rvalue)
	// T = MyVector&& => MyVector&& &&= MyVector &&
}