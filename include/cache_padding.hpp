#pragma once

#include <cstddef>

// Most CPUs use 64B cache line
constexpr size_t CACHE_LINE_SIZE = 64;

// Helper wrapper to pad any variable to cache line
template <typename T>
struct alignas(CACHE_LINE_SIZE) Padded
{
    T value;
};
