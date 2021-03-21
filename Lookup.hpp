// Lookup.hpp
#ifndef Lookup_HPP
#define Lookup_HPP


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

class Lookup {

public:

    int lookup_forIP(uint32_t address, sdsl::rrr_vector<63> Si, sdsl::wt_huff_int<sdsl::rrr_vector<63>> Salfa, sdsl::rrr_vector<>::rank_0_type Si_rank0, sdsl::rrr_vector<>::rank_1_type Si_rank1);
};

#endif // Lookup
