#include <iostream>
#include <thread>
#include <chrono>
#include "../include/spsc_queue.hpp"

using namespace std;
using namespace std::chrono;

constexpr size_t N = 5000000;

SPSCQueue<int> q(1024);

// producer

void producer()
{
    for (size_t i = 0; i < N; ++i)
    {
        while (!q.push(i))
        {
            // spin wait
        }
    }
}

void consumer()
{
    int item;

    for (size_t i = 0; i < N; ++i)
    {
        while (!q.pop(item))
        {
            // spin wait
        }
    }
}

// main

int main()
{
    auto start = high_resolution_clock::now();

    thread p(producer);
    thread c(consumer);

    p.join();
    c.join();

    auto end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start).count();

    double seconds = duration / 1000.0;
    double throughput = N / seconds;

    cout << "Time : " << seconds << "sec\n";
    cout << "Throughput : " << throughput / 1e6 << " million msgs/sec\n";
}