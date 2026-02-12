#pragma once
#include <vector>
#include <iostream>
using namespace std;

template <typename T>

class RingBuffer
{
private:
    vector<T> buffer;
    size_t capacity;
    size_t head = 0; // where produce writes
    size_t tail = 0; // where consumer reads

public:
    RingBuffer(size_t size)
        : buffer(size), capacity(size)
    {
    }
    // push (producer)
    bool push(const T &item)
    {
        size_t next_head = (head + 1) % capacity;

        // check full
        if (next_head == tail)
        {
            return false;
        }

        buffer[head] = item;

        head = next_head;

        return true;
    }

    // pop (consume)

    // check empty
    bool pop(T &item)
    {
        // check empty
        if (head == tail)
        {
            return false;
        }

        item = buffer[tail];

        tail = (tail + 1) % capacity;

        return true;
    }
};