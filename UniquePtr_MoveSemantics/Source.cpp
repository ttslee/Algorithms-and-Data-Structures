#include <memory>
#include <iostream>
#include <string>
#include "UniquePtr.h"
// lvalue  vs rvalue
//
class MyVector
{
public:
	MyVector(int i) 
        {
		arr = new double[i];
		size = i;
	}

	MyVector(const MyVector & rhs){
		size = rhs.size;
		arr = new double[size];
		for (int i = 0; i < size; i++)
		{
			arr[i] = rhs.arr[i];
		}
	}
	
	MyVector(MyVector&& rhs) noexcept{
		size = rhs.size;
		arr = rhs.arr;
		rhs.arr = nullptr;
	}

	~MyVector() { delete[] arr; }
	
	//MyVector createMyvector();
private:
	int size;
	double *arr;
};
std::string makeMeat(){ return "Pork"; }

MyVector createMyvector() {
	return MyVector(50);
}
int main() {
	/*std::unique_ptr<int> up(new int(1));
	std::unique_ptr<int> up2;
	up2 = std::move(up);

	std::string s("Beef");
	std::string s2(s);
	std::string &&s3(makeMeat());
	int &&x = 3;
	int a = 3, b = 4;
	int &&z = a + b;*/

	MyVector my(createMyvector());
	MyVector my2(std::move(my));

	UniquePtr<std::string> a(new std::string("HELLO")), b;
	b = std::move(a);
	std::cout << b.get() << std::endl;
}

