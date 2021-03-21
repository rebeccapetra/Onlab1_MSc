//EntropyPasses.cpp
#include "EntropyPasses.hpp"


void EntropyPasses::pass_one_recursive(PrefixTrie* node, int key){
        if(node->isLeaf()){
            if (node->nexthop_key == -1){
                node->nexthop_key = key;
            }
            return;
        }

        if(node->justOneLeaf()){
            node->createMissingLeaf();
        }

        int ret_value;
        if (node->nexthop_key != -1){
            ret_value = node->nexthop_key;
            node->nexthop_key = -1;
        }else {
            ret_value = key;
        }

        pass_one_recursive(node->child_0, ret_value);
        pass_one_recursive(node->child_1, ret_value);
    }

void EntropyPasses::pass_two_recursive(PrefixTrie* node){
        if(node->isLeaf()){
            return;
        }

        pass_two_recursive(node->child_0);
        pass_two_recursive(node->child_1);

        if (node->child_0->isLeaf() && node->child_1->isLeaf()){
            if ((node->child_0->nexthop_key) == (node->child_1->nexthop_key)){
                node->nexthop_key = node->child_0->nexthop_key;
                delete node->child_0;
                delete node->child_1;
                node->child_0 = nullptr;
                node->child_1 = nullptr;
            }
        }
    }

pass3_return_type EntropyPasses::pass_three_bfs(PrefixTrie* root, int hoptablesize, int leafs_nr, int nodes_nr){
        std::queue<PrefixTrie *> q;
        q.push(root);

        std::vector<int> CounterTable ;
        for (int i = 0; i < hoptablesize+1; i++){
            CounterTable.push_back(0);
        }

        sdsl::bit_vector S_i = sdsl::bit_vector(nodes_nr, 0);
        sdsl::int_vector<> S_alfa(leafs_nr, 0);

        int n = 0;
        int l = 0;

        while (q.empty() == false) {
            PrefixTrie *node = q.front();
            if (node->isLeaf()){
                S_alfa[l] = node->nexthop_key;
                S_i[n] = 1;
                CounterTable[node->nexthop_key] += 1;
                n++;
                l++;
            }
            else{
                S_i[n] = 0;
                n++;
            }

            q.pop();
            if (node->child_0 != NULL)
               q.push(node->child_0);
            if (node->child_1 != NULL)
               q.push(node->child_1);
       }

    //    cout<<"TABLE"<<endl;
    //    for(int i = 0; i<CounterTable.size();i++){
    //        cout<<"i: "<<i<<" "<<CounterTable[i]<<" ";
    //    }
    //    cout<<endl;

       return {S_i, S_alfa, CounterTable};
    }

double EntropyPasses::returnEntropy(std::vector<int> CounterTable,int leafnumber){
        double H_S_alfa = 2;

        for(int i=0; i < (int)CounterTable.size(); i++){
            double p_i = 0.0;
            if (CounterTable[i] != 0){
                p_i = (double)CounterTable[i]/(double)leafnumber;
                H_S_alfa += p_i * (log2(1/p_i));
                //cout<<"i: "<<i<<" p_i: "<<p_i<<" p_i * (log2(1/p_i)) "<<p_i * (log2(1/p_i))<<endl;
            }
        }

        return H_S_alfa;
    }

int EntropyPasses::getLeafsCount(PrefixTrie* node){
        if (node == nullptr) {
            return 0;
        }
        else if (node->isLeaf()){
                return 1;
            } else return getLeafsCount(node->child_0) + getLeafsCount(node->child_1);
    }

int EntropyPasses::getNodesCount(PrefixTrie* node){
        if (node == nullptr) {
            return 0;
        }
        return 1 + getNodesCount(node->child_0) + getNodesCount(node->child_1);
    }

int EntropyPasses::getNonEmptyNodesCount(PrefixTrie* node){
        if (node == nullptr) {
            return 0;
        }
        if (node->isEmpty()){
            return 0 + getNodesCount(node->child_0) + getNodesCount(node->child_1);
        } else {
            return 1 + getNodesCount(node->child_0) + getNodesCount(node->child_1);
        }
    }
