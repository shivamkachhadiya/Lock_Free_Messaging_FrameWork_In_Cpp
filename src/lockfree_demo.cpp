#include <iostream>
#include <thread>
#include "../include/spsc_queue.hpp"
using namespace std;

SPSCQueue<int> q(1024);

void producer()
{
    for (int i = 1; i <= 20; ++i)
    {
        while (!q.push(i))
        {
        } // spin

        cout << "produced: " << i << endl;
    }
}
void consumer()
{
    int item;

    for (int i = 1; i <= 20; ++i)
    {
        while (!q.pop(item))
        {
        }
        cout << "Consumed: " << item << endl;
    }
}

int main()
{
    thread p(producer);
    thread c(consumer);

    p.join();
    c.join();
}