//{
// #pragma once
// #include <vector>
// #include <atomic>
// using namespace std;

// template <typename T>
// class SPSCQueue
// {
// private:
//     vector<T> buffer;
//     const size_t capacity;

//     atomic<size_t> head{0};
//     atomic<size_t> tail{0};

// public:
//     explicit SPSCQueue(size_t size)
//         : buffer(size), capacity(size) {}

//     // prducer thread
//     bool push(const T &item)
//     {
//         size_t h = head.load(memory_order_relaxed);

//         size_t next = (h + 1) % capacity;

//         // read consumer position
//         if (next == tail.load(memory_order_acquire))
//             return false;

//         buffer[h] = item;

//         // publish write
//         head.store(next, memory_order_release);

//         return true;
//     }

//     // consumer thread only
//     bool pop(T &item)
//     {
//         size_t t = tail.load(memory_order_relaxed);

//         // read producer position
//         if (t == head.load(memory_order_acquire))
//             return false;

//         item = buffer[t];

//         // publish read
//         tail.store((t + 1) % capacity, memory_order_release);

//         return true;
//     }
// };
//}
#pragma once
#include <vector>
#include <atomic>
#include "cache_padding.hpp"
using namespace std;

template <typename T>
class SPSCQueue
{
private:
    vector<T> buffer;
    const size_t capacity;

    // Separate cache lines to avoid false sharing
    Padded<atomic<size_t>> head;
    Padded<atomic<size_t>> tail;

public:
    explicit SPSCQueue(size_t size)
        : buffer(size),
          capacity(size)
    {
        head.value.store(0);
        tail.value.store(0);
    }

    // Producer (ONLY 1 thread)
    bool push(const T &item)
    {
        size_t h = head.value.load(memory_order_relaxed);

        size_t next = (h + 1) % capacity;

        // check if full
        if (next == tail.value.load(memory_order_acquire))
            return false;

        buffer[h] = item;

        // publish write
        head.value.store(next, memory_order_release);

        return true;
    }

    // Consumer (ONLY 1 thread)
    bool pop(T &item)
    {
        size_t t = tail.value.load(memory_order_relaxed);

        // check if empty
        if (t == head.value.load(memory_order_acquire))
            return false;

        item = buffer[t];

        // free slot
        tail.value.store((t + 1) % capacity, memory_order_release);

        return true;
    }
};
