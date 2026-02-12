#include <iostream>
#include <thread>
#include <chrono>
#include "../include/spsc_queue.hpp"
#include "../include/message.hpp"

using namespace std;
using namespace std::chrono;

constexpr size_t N = 5'000'000;

SPSCQueue<Message> q(1024);

// only consumer uses → no atomic needed
uint64_t total_latency_ns = 0;

// ================= Producer =================
void producer()
{
    for (uint64_t i = 0; i < N; ++i)
    {
        Message m;
        m.id = i;

        // SAFE + PRECISE nanoseconds
        m.timestamp =
            duration_cast<nanoseconds>(
                steady_clock::now().time_since_epoch())
                .count();

        // spin but be polite to CPU
        while (!q.push(m))
            this_thread::yield();
    }
}

// ================= Consumer =================
void consumer()
{
    Message m;

    for (size_t i = 0; i < N; ++i)
    {
        while (!q.pop(m))
            this_thread::yield();

        uint64_t now =
            duration_cast<nanoseconds>(
                steady_clock::now().time_since_epoch())
                .count();

        total_latency_ns += (now - m.timestamp);
    }
}

// ================= Main =================
int main()
{
    auto start = steady_clock::now();

    thread p(producer);
    thread c(consumer);

    p.join();
    c.join();

    auto end = steady_clock::now();

    double seconds = duration<double>(end - start).count();

    double throughput = N / seconds;

    double avg_latency =
        static_cast<double>(total_latency_ns) / N;

    cout << "===== Lock-Free SPSC Benchmark =====\n";
    cout << "Time: " << seconds << " sec\n";
    cout << "Throughput: " << throughput / 1e6 << " million msgs/sec\n";
    cout << "Avg Latency: " << avg_latency << " ns\n";
}
