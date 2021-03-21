//HopTable.cpp
#include "HopTable.hpp"

HopTable::HopTable(){
        insertNewHopIP(0);
    }

int HopTable::returnKeyValue(uint32_t ip){
        std::vector<uint32_t>::iterator itr = std::find(hopTable.begin(), hopTable.end(), ip);
        int test;
        if (itr != hopTable.cend()) {
            test = std::distance(hopTable.begin(), itr);
        }
        return test;
    }

void HopTable::insertNewHopIP(uint32_t ip){
        if (std::find(hopTable.begin(), hopTable.end(), ip) == hopTable.end()) {
            hopTable.push_back(ip);
        }
    }

void HopTable::printHopTable(){
        std::cout << "HopTable: "<< std::endl;
        for (int i = 0; i < (int)hopTable.size(); i++) {
            std::cout << hopTable.at(i) << ' ' ;
        }
    }

int HopTable::getCount(){
        return hopTable.size();
    }

