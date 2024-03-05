#pragma once

#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>

template<typename T> void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp;
}

template<typename T> void reverseArray(T* array, size_t n) {
    size_t i = 0, j = n - 1;
    while (i < j) {
        swap<T>(array[i++], array[j--]);
    }
}

template<typename T> T readbytes(std::ifstream& is) {
    size_t size = sizeof(T);
    char* buf = new char[size];
    is.read(buf, size);
    reverseArray(buf, size);
    T ret = *(T*) buf;
    delete[] buf;
    return ret;
}
