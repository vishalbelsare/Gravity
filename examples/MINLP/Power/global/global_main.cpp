//  Decompose.cpp
//
//  Gravity
//
//  Created by Guanglei Wang on 6/9/17.
//
//
#include "Partition.hpp"
#include "global.hpp"
bool node_id_compare(const Node* n1, const Node* n2) {
    return n1->_id < n2->_id;
}

bool arc_id_compare(const Arc* n1, const Arc* n2) {
    return n1->_id < n2->_id;
}

bool bus_pair_compare(const index_pair* n1, const index_pair* n2) {
    return n1->_name < n2->_name;
}

struct net_param {
    param<Real> c0, c1, c2; /**< Generation costs */
    param<Real> tan_th_min, tan_th_max;
    param<Real> g_ff, g_ft, g_tt, g_tf, b_ff, b_ft, b_tf, b_tt;
    param<Real> S_max;
};

// Note that time based decomposition needs more constraints relaxed..
// for the ANU project, we neglect min-up and min-down constraints in the original ACUC formulation
// while choose to relax ramp up/down constraints and inter temporal constraints (in total 4 constraints).
int main (int argc, const char * argv[])
{
    // Decompose
    const char* fname;
    double l = 0;
    if (argc >= 2) {
        fname = argv[1];
        l = atof(argv[2]);
    }
    else {
        // fname = "../../data_sets/Power/nesta_case5_pjm.m";
        // fname = "../../data_sets/Power/nesta_case30_ieee.m";
        //fname = "../../data_sets/Power/nesta_case6_c.m";
        //fname = "../../data_sets/Power/nesta_case5_pjm.m";
        //fname = "../../data_sets/Power/nesta_case3_lmbd.m";
        //fname = "../../data_sets/Power/nesta_case300_ieee.m";
        //fname = "../../data_sets/Power/nesta_case1354_pegase.m";
        fname = "../../data_sets/Power/nesta_case14_ieee.m";
        //fname = "../../data_sets/Power/nesta_case57_ieee.m";
        l = 1;
    }
    auto grid = new PowerNet();
    grid->readgrid(fname);
    //GRAPH PARTITION
    double solver_time_end, total_time_end, solve_time, total_time;
    double total_time_start = get_cpu_time();
    // Schedule Parameters
    int T = 2;
    int Num_parts = 2;
    auto global = new Global(grid, Num_parts, T);
    double cst_t = global->getdual_relax_time_();
    double lr_t = global->LR_bound_time_();
    cout << "time lr lower bound " << to_string(lr_t) << endl;
    double cst_s =global->getdual_relax_spatial();
    double lr_s = global->LR_bound_spatial_();
    cout << "Spaital lr lower bound: " << to_string(lr_s) << endl;

//    // now we need to solve it faster
//    total_time_end = get_cpu_time();
//    total_time = total_time_end - total_time_start;
//    string out = "DATA_OPF, " + grid._name + ", " + to_string(nb_buses) + ", " + to_string(nb_lines)
//                 +", " + to_string(LB) + ", " + to_string(-numeric_limits<double>::infinity()) +", CPU time, " + to_string(total_time);
//    cout << out << endl;
    return 0;
}
