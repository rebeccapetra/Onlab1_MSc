// PrefixTrie.hpp
#ifndef PrefixTrie_HPP
#define PrefixTrie_HPP

class PrefixTrie {

public:
    int nexthop_key;
    PrefixTrie *child_0;
    PrefixTrie *child_1;


    PrefixTrie();
    PrefixTrie(int key);
    ~PrefixTrie();

    bool isLeaf();
    bool justOneLeaf();
    void createMissingLeaf();
    bool isEmpty();
    PrefixTrie* addNewIP(PrefixTrie* root, auto ip, unsigned mask, int index);

};

#endif // PrefixTrie
