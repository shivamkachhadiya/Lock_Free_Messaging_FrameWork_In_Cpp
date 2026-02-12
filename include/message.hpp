#pragma once
#include <cstdint>

struct Message
{
    uint64_t id;
    uint64_t timestamp;
    char payload[32]; // simulate real data
};
