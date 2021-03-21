// HopTable.hpp
#ifndef HopTable_HPP
#define HopTable_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <queue>
#include <string>
#include <math.h>
#include <filesystem>
#include <chrono>
#include <sdsl/bit_vectors.hpp>
#include <sdsl/vectors.hpp>
#include <sdsl/wavelet_trees.hpp>
#include <stdlib.h>

class HopTable {

public:
    std::vector<uint32_t> hopTable = {};

    HopTable();

    int returnKeyValue(uint32_t ip);
    void insertNewHopIP(uint32_t ip);
    void printHopTable();
    int getCount();
};

#endif // HopTable
