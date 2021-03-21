//PrefixTrie.cpp
#include "PrefixTrie.hpp"

PrefixTrie::PrefixTrie(){
        nexthop_key = -1;
        child_0 = nullptr;
        child_1 = nullptr;
    }

PrefixTrie::PrefixTrie(int key){
        nexthop_key = key;
        child_0 = nullptr;
        child_1 = nullptr;
    }

PrefixTrie::~PrefixTrie() {
        delete this->child_0;
        delete this->child_1;
    }

bool PrefixTrie::isLeaf(){
        if ((this->child_0 == nullptr) && (this->child_1 == nullptr)){
            return true;
        }
        return false;
    }

bool PrefixTrie::justOneLeaf(){
        if (((this->child_0 != nullptr) && (this->child_1 == nullptr)) || ((this->child_0 == nullptr) && (this->child_1 != nullptr))){
            return true;
        }
        return false;
    }

void PrefixTrie::createMissingLeaf(){
        if (this->child_0 == nullptr){
            this->child_0 = new PrefixTrie();
        }else{
            this->child_1 = new PrefixTrie();
        }
    }

bool PrefixTrie::isEmpty(){
        if (this->nexthop_key == -1){
            return false;
        }
        return true;
    }

PrefixTrie* PrefixTrie::addNewIP(PrefixTrie* root, auto ip, unsigned mask, int index){
        PrefixTrie* temp = root;
        if (root == nullptr) {                                                          //ha ures a fa
            root = new PrefixTrie(index);
            temp = root;
        }
        else{
            //cout<<"IP: "<<ip<<" MASK: "<<mask<<" INDEX: "<<index<<endl;
            for (int i=31; i>(31-(int)mask); i--){                                      //vegigmegyunk a hasznos reszen
                //cout<<"I: "<<ip[i]<<endl;
                if (ip[i] == 0) {                                                       //ha a kovetkezo elem 0:
                    if (temp->child_0 == nullptr){                                      //ha nincs meg ilyen ag, letrehozzuk
                        temp->child_0 = new PrefixTrie();
                        temp = temp->child_0;
                    }
                    else{
                        temp = temp->child_0;                                           //ha van mar ilyen ag, ralepunk
                    }

                    if ((i-1) == (31-(int)mask)) {                                      //ha ez az utolso eleme az ip-nek, akkor beleirjuk az indexet
                        temp->nexthop_key=index;
                    }
                }
                else {
                    if (temp->child_1 == nullptr){
                        temp->child_1 = new PrefixTrie();
                        temp = temp->child_1;
                    }
                    else{
                        temp = temp->child_1;
                    }

                    if ((i-1) == (31-(int)mask)) {
                        temp->nexthop_key=index;
                    }
                }
            }
        }
        return root;
    }
