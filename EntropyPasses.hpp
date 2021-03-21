// EntropyPasses.hpp
#ifndef EntropyPasses_HPP
#define EntropyPasses_HPP

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


struct pass3_return_type{
        sdsl::bit_vector S_I;
        sdsl::int_vector<> S_A;
        std::vector<int> CounterTable;
    };

class EntropyPasses{

public:

    void pass_one_recursive(PrefixTrie* node, int key);
    void pass_two_recursive(PrefixTrie* node);
    pass3_return_type pass_three_bfs(PrefixTrie* root, int hoptablesize, int leafs_nr, int nodes_nr);
    double returnEntropy(std::vector<int> CounterTable,int leafnumber);
    int getLeafsCount(PrefixTrie* node);
    int getNodesCount(PrefixTrie* node);
    int getNonEmptyNodesCount(PrefixTrie* node);
};

#endif // EntropyPasses
