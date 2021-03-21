// Lookup.cpp
#include "Lookup.hpp"

int Lookup::lookup_forIP(uint32_t address, sdsl::rrr_vector<63> Si, sdsl::wt_huff_int<sdsl::rrr_vector<63>> Salfa,sdsl::rrr_vector<>::rank_0_type Si_rank0,sdsl::rrr_vector<>::rank_1_type Si_rank1){
        auto ip = std::bitset<32>(address);                                                          //cout << "ip: " << ip << endl;
        int q = 0;                                                                              //cout << "q: " << q << endl;
        int i = 0;                                                                              //cout << "i: " << i << endl;
        while (q<32){
            if (Si[i] == 1) {
                    return Salfa[Si_rank1(i)];                                                  //cout << "Si[i]: " << Si[i] << " Si[i+1]: " << Si[i+1] <<endl << "Salfa[Si_rank1(i)]: " << Salfa[Si_rank1(i)] <<endl<< "Si_rank1(i): " << Si_rank1(i)<<endl;
            }
                                                                                                //cout << "Si[i]: " << Si[i] << " Si[i+1]: " << Si[i+1] <<" Si_rank0(i): "<<Si_rank0(i)<<endl;
            int r = Si_rank0(i)+1;                                                              //cout << "r = Si_rank0(i)+1: " <<  r << endl;
            int f = 2*r;                                                                        //cout << "f=2r: " << f <<endl;
            int j;
            if (ip[31-q]==0) { j = -1; } else { j = 0;}                                         //cout<<"j: "<<j<<endl;
            i = f+j;                                                                            //cout << "i = f+j: " << i << endl;
            q++;                                                                                //cout << "q: " << q <<endl;
        }
        return -1;
    }

