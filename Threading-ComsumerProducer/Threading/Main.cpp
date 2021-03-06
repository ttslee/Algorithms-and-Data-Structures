#include "Consumer.h"
#include "Producer.h"
using namespace std;
#include <random>


void shared_print(string msg, int i){
	std::lock_guard<std::mutex> locker(mu);
	std::cout << msg << i << endl;
	mu.unlock();
}
void sleep()
{

}
void produce_thread(Producer & p)
{
	while (true)
	{
		std::unique_lock<std::mutex> locker(mu);
		cv.wait(locker, [&]() { return p.getQueue()->size() < 15; });
		int i = rand() % 5;
		Item it(i);
		p.produce(it);
		locker.unlock();
		cv.notify_all();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}
void consume_thread(Consumer & c)
{
	while (true)
	{
		std::unique_lock<std::mutex> locker(mu);
		cv.wait(locker, [&]() { return !c.getQueue()->empty(); });
		c.consume();
		locker.unlock();
		cv.notify_all();
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}
}
int main(){
	srand(static_cast<unsigned int>(time(0)));
	std::queue<Item> q;
	Producer p(q);
	Consumer c(q);
	std::thread t1(produce_thread, std::ref(p));
	std::thread t2(consume_thread, std::ref(c));

	t1.join();
	t2.join();
	system("pause");
	return 0;
}
//#include <queue>
//#include <mutex>
//#include <chrono>
//#include <condition_variable>
//#include <iostream>
//std::mutex mu, cout_mu;
//std::condition_variable cond;
//
//class Buffer
//{
//public:
//	void add(int num) {
//		while (true) {
//			std::unique_lock<std::mutex> locker(mu);
//			cond.wait(locker, [&]() { return buffer_.size() < size_; });
//			buffer_.push(num);
//			locker.unlock();
//			cond.notify_all();
//			return;
//		}
//	}
//	int remove() {
//		while (true)
//		{
//			std::unique_lock<std::mutex> locker(mu);
//			cond.wait(locker, [&]() { return buffer_.size() > 0; });
//			int back = buffer_.size();
//			buffer_.pop();
//			locker.unlock();
//			cond.notify_all();
//			return back;
//		}
//	}
//	Buffer() {}
//private:
//	std::queue<int> buffer_;
//	const unsigned int size_ = 10;
//};
//
//class Producer
//{
//public:
//	Producer(Buffer* buffer)
//	{
//		this->buffer_ = buffer;
//	}
//	void run() {
//		while (true) {
//			int num = std::rand() % 100;
//			buffer_->add(num);
//			std::lock_guard<std::mutex> locker(cout_mu);
//			std::cout << "Produced: " << num << std::endl;
//			std::this_thread::sleep_for(std::chrono::milliseconds(50));
//		}
//	}
//private:
//	Buffer *buffer_;
//};
//
//class Consumer
//{
//public:
//	Consumer(Buffer* buffer)
//	{
//		this->buffer_ = buffer;
//	}
//	void run() {
//		while (true) {
//			int num = buffer_->remove();
//			std::lock_guard<std::mutex> locker(cout_mu);
//			std::cout << "Consumed: " << num << std::endl;
//			std::this_thread::sleep_for(std::chrono::milliseconds(50));
//		}
//	}
//private:
//	Buffer *buffer_;
//};
//
//int main() {
//	Buffer b;
//	Producer p(&b);
//	Consumer c(&b);
//
//	std::thread producer_thread(&Producer::run, &p);
//	std::thread consumer_thread(&Consumer::run, &c);
//
//	producer_thread.join();
//	consumer_thread.join();
//	getchar();
//	return 0;
//}