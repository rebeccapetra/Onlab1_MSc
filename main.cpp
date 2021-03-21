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

#include "PrefixTrie.cpp"
#include "HopTable.cpp"
#include "EntropyPasses.cpp"
#include "Lookup.cpp"

using namespace std;
using namespace sdsl;


struct Trunk{
    Trunk *prev;
    std::string str;

    Trunk(Trunk *prev, std::string str){
        this->prev = prev;
        this->str = str;
    }
};

void showTrunks(Trunk *p){
    if (p == nullptr)
        return;

    showTrunks(p->prev);

    cout << p->str;
}

void printTree(PrefixTrie *root, Trunk *prev, bool isLeft){
    if (root == nullptr)
        return;

    string prev_str = "    ";
    Trunk *trunk = new Trunk(prev, prev_str);

    printTree(root->child_0, trunk, true);

    if (!prev)
        trunk->str = "---(";
    else if (isLeft){
        trunk->str = ".---(";
        prev_str = "   |";
    }
    else{
        trunk->str = "`---(";
        prev->str = prev_str;
    }

    showTrunks(trunk);
    cout << root->nexthop_key << ")" << endl;

    if (prev)
        prev->str = prev_str;
    trunk->str = "   |";

    printTree(root->child_1, trunk, false);
}


char usage[] = "./main.cpp <fib_dump> <csv>";
char *fib_file, *csv_file;

int main(int argc, char** argv){

    if (argc != 3) {
    printf("%s\n", usage);
    }

    fib_file = argv[1];
    csv_file = argv[2];

    PrefixTrie *root = nullptr;
    HopTable table;
    EntropyPasses pass;
    Lookup lookup;

    ifstream file(fib_file);
    string line = "";

    root = root->addNewIP(root, bitset<32>((0<<24U) + (0<<16U) + (0<<8U) + 0), 0, table.returnKeyValue(0));        //black hole - default

    while (getline(file,line)) {
        unsigned a, b, c, d, mask, n, e, x, t;
        const char *cstr = line.c_str();                                            //sor kiolvasas

        sscanf(cstr, "%d.%d.%d.%d/%d %d.%d.%d.%d", &a, &b, &c, &d, &mask, &n, &e, &x, &t);

        auto ip = bitset<32>((a<<24U) + (b<<16U) + (c<<8U) + d);                    //ip cim
        uint32_t ip_nexthop = (n<<24) + (e<<16) + (x<<8) +t;                        //nexthop ertek

        table.insertNewHopIP(ip_nexthop);                                           //hozzadja a nexthopot a tablahoz
        root = root->addNewIP(root, ip, mask, table.returnKeyValue(ip_nexthop));       //beleteszi az ip-t a prefix faba
    }

    pass.pass_one_recursive(root, root->nexthop_key);                                //1. lepes: leaf pushing
    pass.pass_two_recursive(root);                                                  //2. lepes: egyszerusiti a leafpushed fat

    int leafs_nr = pass.getLeafsCount(root);                                        //3.1 megszamolja a leafeket es nodeokaat
    int nodes_nr = pass.getNodesCount(root);
    int nonempty_nr = pass.getNonEmptyNodesCount(root);

    pass3_return_type ret = pass.pass_three_bfs(root,(int)table.hopTable.size(),leafs_nr,nodes_nr);     //3.2 meghatarozza az adott ertekeket

    double H_S_alfa = pass.returnEntropy(ret.CounterTable,leafs_nr);        //entropia ertek szamitas

    //file name and date

    string delimiter = "/";
    string token;
    string fib_name = "";
    string fib_date = "";
    int i = 0;
    size_t pos = 0;
    std::string s(fib_file);

    pos = s.find(delimiter);
    s.erase(0, pos+delimiter.length());

    delimiter = "_";

    while (i <= 4){
        pos = s.find(delimiter);
        token = s.substr(0,pos);
        if (i < 2) {fib_name += token;} else {fib_date += token;}
        s.erase(0, pos+delimiter.length());
        i++;
    }

    //compressing

    rrr_vector<63> rrrb(ret.S_I);                   //S_I tomoritese
    wt_huff_int<rrr_vector<63>> wt;                 //wavelet tree letrehozasa
    construct_im(wt, ret.S_A,0);                    //S_alfa tomoritese


    uint32_t ip;
    int eredmeny=0;
    rrr_vector<>::rank_1_type Si_rank1(&rrrb);
    rrr_vector<>::rank_0_type Si_rank0(&rrrb);

    typedef std::chrono::high_resolution_clock Clock;
    auto start_time = Clock::now();

    for (int i = 0; i<100000; i++){
        ip = rand() %4294967295;
        eredmeny = lookup.lookup_forIP(ip, rrrb, wt, Si_rank0, Si_rank1);
    }

    auto end_time = Clock::now();
    auto nanosec100k = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();

    //fajlba iras

    std::ofstream myfile;
    myfile.open(csv_file);
    myfile << "fib_name, date, prefix_nr, next_hop_nr, H_0, node_nr, leaf_nr, H_0+2, nxH_02, S_i_size_byte, S_i_size_Mb, S_alfa_size_byte, S_alfa_size_Mb, comp_S_i_byte, comp_S_i_Mb, comp_S_alfa_byte, comp_S_alfa_Mb, comp_SiSa_byte, comp_SiSa_Mb, nanosec100k\n";
    myfile << fib_name << "," << fib_date << "," << nonempty_nr << "," << table.getCount() << "," ;
    myfile << H_S_alfa-2 << "," << nodes_nr << "," << leafs_nr << "," ;
    myfile << H_S_alfa << "," << H_S_alfa*leafs_nr << "," ;
    myfile << size_in_bytes(ret.S_I) << "," << size_in_mega_bytes(ret.S_I) << "," ;
    myfile << size_in_bytes(ret.S_A) << "," << size_in_mega_bytes(ret.S_A) << "," ;
    myfile << size_in_bytes(rrrb) << "," << size_in_mega_bytes(rrrb) << "," ;
    myfile << size_in_bytes(wt) << "," << size_in_mega_bytes(wt) << "," ;
    myfile << size_in_bytes(rrrb)+size_in_bytes(wt) << "," << size_in_mega_bytes(wt)+size_in_mega_bytes(rrrb) << "," ;
    myfile << nanosec100k << "," ;
    myfile.close();

    cout << "1 " << endl;

    delete root;
    return 0;
}
