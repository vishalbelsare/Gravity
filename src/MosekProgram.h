////
////  MosekProgram.hpp
////  Gravity
////
////
////   Note mosek fusion API is designed for CONIC Optimization. 
//
//#ifndef MosekProgram_h
//#define MosekProgram_h
//
//#include <stdio.h>
//#ifdef USE_MOSEK
//#include <fusion.h>
//#include <monty.h>
//#include <mosek.h>
//#endif
//#include <gravity/model.h>
//using namespace gravity;
//
//class MosekProgram: public Program{
//public:
//    Model* _model;
//    int _output;
//    MosekProgram();
//    MosekProgram(Model* m);
//    ~MosekProgram();
//    void reset_model();
//    
//    
//    bool solve(bool relax);
//    void prepare_model();
//    void update_model();
//    void relax_model();
//    
//    void fill_in_mosek_vars();
//    void create_mosek_constraints();
//    void set_mosek_objective();
//    void print_constraints();
//    
//private:
//    mosek::fusion::Model::t _mosek_model;
//    vector<mosek::fusion::Variable::t> _mosek_vars;
//
//    mosek::fusion::Expression::t form_Fx(map<string, qterm> &qterms, size_t qn);
//    mosek::fusion::Expression::t form_Fx(map<string, qterm> &qterms, size_t qn, size_t inst);
//    mosek::fusion::Expression::t create_lin_expr(map<string, lterm>& lt, constant_* cst, size_t inst);
//};
//
//#endif /* MosekProgram_h */
