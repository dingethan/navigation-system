#include<iostream>
#include<thread>
#include<string>
#include<mutex>
using namespace std;
int a = 0;
mutex mtx;
void func() {
	for (int i = 0; i < 1000; i++) {
		mtx.lock();
		a += 1;
		mtx.unlock();
	}
}
int main() {
	thread t1(func);
	thread t2(func);
	//这里因为两个线程同时访问a的值，拿到的a值是相同的
	t1.join();
	t2.join();
	cout << a << endl;
	return 0;

}