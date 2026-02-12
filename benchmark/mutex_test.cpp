#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;
using namespace std::chrono;

constexpr size_t N = 5'000'000;

queue<int> q;
mutex mtx;
condition_variable cv;

// ==========================
void producer()
{
    for (size_t i = 0; i < N; ++i)
    {
        unique_lock<mutex> lock(mtx);
        q.push(i);
        lock.unlock();
        cv.notify_one();
    }
}

// ==========================
void consumer()
{
    int item;

    for (size_t i = 0; i < N; ++i)
    {
        unique_lock<mutex> lock(mtx);

        cv.wait(lock, []
                { return !q.empty(); });

        item = q.front();
        q.pop();
    }
}

// ==========================
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

    cout << "Time: " << seconds << " sec\n";
    cout << "Throughput: " << throughput / 1e6 << " million msgs/sec\n";
}
