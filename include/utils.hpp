#ifndef VORTEX_UTILS_H
#define VORTEX_UTILS_H

#include <cstddef>

/// A C++-safe alternative to `memcpy()`. The idea is to have a `memcpy()`
/// alternative, which explicitly calls the `operator=` of the generic type `T`,
/// ensuring consistend behaviour.
template <typename T>
void memtransfer(T *dst, const T *src, size_t size) {
        for (size_t i = 0; i < size; ++i) {
                dst[i] = src[i];
        }
}

#endif
