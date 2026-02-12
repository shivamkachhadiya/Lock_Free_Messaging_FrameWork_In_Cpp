#include "../include/ring_buffer.hpp"
#include <thread>
#include <mutex>
#include <condition_variable>
#include <iostream>
using namespace std;

RingBuffer<int> q(1024);

// sync tools

mutex mtx;
condition_variable cv;

// producer
void producer()
{
    for (int i = 1; i <= 20; ++i)
    {
        unique_lock<mutex> lock(mtx);

        // wait untill push success
        cv.wait(lock, [&]
                { return q.push(i); });
        cout << "produced: " << i << "\n";

        lock.unlock();
        cv.notify_one();
    }
}

// consumer
void consumer()
{
    int item;

    for (int i = 1; i <= 20; ++i)
    {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, [&]
                { return q.pop(item); });

        cout << "consumed: " << item << "\n";

        lock.unlock();
        cv.notify_one();
    }
}

int main()
{
    thread p(producer);
    thread c(consumer);

    p.join();
    c.join();

    return 0;
}


