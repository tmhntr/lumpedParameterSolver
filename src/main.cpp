//
//  main.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-16.
//

#include <iostream>
#include "interface.hpp"
#include "RCR.hpp"
#include "heart.hpp"
#include "wrapper.hpp"
// #include "sdsolver.hpp"
#include "basicprinter.hpp"
#include "betterprinter.hpp"
#include "myoRC.hpp"
#include "myogenic.hpp"
#include "vpr_circulation.hpp"
#include <string>


using namespace std;

model * myogenicOrgan(string inletPressure, string outletFlow, int nSegs, string organName, string organSymbol);
model * heldt();
vector<string> expand(vector<string> vec, int nSegs);



int main(int argc, const char * argv[]) {
    // insert code here...

    model * rat = new vpr_circulation("Rat", {"A", "C_ach", "eps_1", "eps_2", "eps_3", "Q_av", "Q_mt", "Q_pv", "Q_tc", "V_ao", "V_lv", "V_pa", "V_pu", "V_rv", "V_vc", "c_nor", "delta_HR_pslow", "delta_HR_s", "Psi", "P_ao", "Tp", "C1", "C2", "C3", "C4", "C5", "C6", "Ts", "delta_HR_ps", "delta_HR_ss", "HR", "e_t", "V_spt", "E_es_spt", "E_es_lvf", "E_es_rvf", "P_lv", "P_pu", "P_rv", "P_pa", "P_vc", "Q_sys", "Q_pul"}, {"P_ao", "Tp", "C1", "C2", "C3", "C4", "C5", "C6", "Ts", "delta_HR_ps", "delta_HR_ss", "HR", "e_t", "V_spt", "E_es_spt", "E_es_lvf", "E_es_rvf", "P_lv", "P_pu", "P_rv", "P_pa", "P_vc", "Q_sys", "Q_pul"}, {"A", "C_ach", "eps_1", "eps_2", "eps_3", "Q_av", "Q_mt", "Q_pv", "Q_tc", "V_ao", "V_lv", "V_pa", "V_pu", "V_rv", "V_vc", "c_nor", "delta_HR_pslow", "delta_HR_s", "Psi"}, {});


    vector<double> y0 = {15.20531, 1.0, 0.2042, 0.232, 0.149, 0, 1.6697, 0.0, 1.2921, 0.5*1.0567, 0.44066, 0.38685, 1.8*5.8495, 0.41389, 1.8*2.3484, 1.249, 0, 0, 0};

    wrapper * mdl = new wrapper();
    mdl->addModel(rat);




//    mdl->init();

    // vector<double> y0 = {90, 40, 40, 40, 15, 7, 7, 50.0, 20.0, 2.0, 20.0, 1.0, 10.0, 1.0, 3.0, 8.0, 13.0, 80,  173.1, 125.3, 95.41, 62.98, 41.35, 0.0, 0.0, 0.0, 0.0, 0.0, 50.0, 50.0, 50.0, 50.0, 50.0, 30.0, };
//    double y0[] = {2.0, 20.0};
    double reltol = 1e-8;
//    double abstol[] = {1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4};
    double abstol[] = {1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6};
//    double abstol[] = {1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8};
// //    double abstol[] = {1e-6, 1e-6};


//     sdsolver * slvr = new sdsolver(mdl, y0.data(), reltol, abstol);
//
// //    slvr->setPrinter(new betterprinter((void *) slvr, mdl));
//     slvr->setPrinter(new basicprinter((void *) slvr));
//     slvr->setDeltaT(0.001);
// //    slvr->print();
//     double tstop = 60.0;
//     if (argc > 1)
//         tstop = atof(argv[1]);
//     for (double t = slvr->getDeltaT(); t <= tstop; t+=slvr->getDeltaT())
//     {
//         slvr->print();
//         try {
//             slvr->solveStep(t);
//         } catch (int e) {
//             cout<<"Solver failed at timestep " << t << endl;
//             return 1;
//         }
//     }
//     slvr->print();
//

    std::cout << "ran through" << '\n';
    return 0;
}

//model * heldt()
//{
//    RCR * body[10];
//    body[0] = new RCR("Arteries", {"Q_up", "Q_kid1", "Q_spl1", "Q_ll1"}, {"P_lv", "P_a"}, {"Q_lo"}, {"P_a"}, {0.006, 2.0});
//    body[0]->setHasValve(true);
//    body[1] = new RCR("Upper Body", {"P_a", "P_up", "Q_sup"}, {"Q_up"}, {"P_up"}, {3.9, 8.0});
//    body[2] = new RCR("Kidneys", {"P_a", "P_kid", "Q_kid2"}, {"Q_kid1"}, {"P_kid"}, {4.1, 15.0});
//    body[3] = new RCR("Splanchnic", {"Q_spl2"}, {"P_a", "P_spl"}, {"Q_spl1"}, {"P_spl"}, {3.0, 55.0});
//    body[4] = new RCR("Lower Legs", {"Q_ll2"}, {"P_a", "P_ll"}, {"Q_ll1"}, {"P_ll"}, {3.6, 19.0});
//    body[5] = new RCR("Sup. Vena Cava", {"Q_ri1"}, {"P_up", "P_sup"}, {"Q_sup"}, {"P_sup"}, {0.23, 15.0});
//    body[5]->setHasValve(true);
//    body[6] = new RCR("Abd. Vena Cava", {"Q_inf"}, {"P_kid", "P_spl", "P_ll", "P_abd"}, {"Q_kid2", "Q_spl2", "Q_ll2"}, {"P_abd"}, {0.3, 0.18, 0.3, 25.0});
//    body[6]->setHasValve(true);
//    body[7] = new RCR("Inf. Vena Cava", {"Q_ri2"}, {"P_abd", "P_inf"}, {"Q_inf"}, {"P_inf"}, {0.015, 2.0});
//    body[7]->setHasValve(true);
//    body[8] = new RCR("Pulm. Arteries", {"Q_pv"}, {"P_rv", "P_pa"}, {"Q_pa"}, {"P_pa"}, {0.003, 4.3});
//    body[8]->setHasValve(true);
//    body[9] = new RCR("Pulm. Veins", {"Q_li"}, {"P_pa", "P_pv"}, {"Q_pv"}, {"P_pv"}, {0.08, 8.4});
//
//    heart * hrt = new heart({"Q_pv", "Q_lo"}, {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv", "P_sup", "P_inf", "P_pv"}, {"C_ra", "C_rv", "C_la", "C_lv", "Q_ri1", "Q_ri2", "Q_rav", "Q_li", "Q_lav"}, {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv"}, {0.8, 2.5, 0.13, 1.3, 0.07, 0.61, 0.5, 0.74, 0.3, 0.06, 0.015, 0.005, 0.01, 0.01});
//
//    wrapper * mdl = new wrapper(std::vector<model *> {hrt});
//    for (int i = 0; i < 10; i++)
//        mdl->addModel(body[i]);
//
//    mdl->init();
//    return mdl;
//}



model * myogenicOrgan(string inletPressure, string outletFlow, int nSegs, string organName, string organSymbol)
{
//    int nSegs = 5;
    vector<double> P_myo, P_flow;
//
//    string organName = "organ";
//    string organSymbol = "o";

    vector<string> myoShared, myoInputStates, myoAlgebraic, myoStates;
    vector<string> flowInputStates, flowAlgebraic, flowStates;


    // All parameters are listed

    double C_symp, C_tone_primeprime, C_ATP_0, C_rbc_meta, tau_d, tau_a, E;
    double C_pass[] = {1154, 834.9, 636.0, 419.8, 275.6};
    double C_pass_prime[] = {7.846, 9.138, 9.944, 10.82, 11.40};
    double D_0[] = {173.1, 125.3, 95.41, 62.98, 41.35};
    double C_act[] = {2671, 1654, 1106, 598.1, 320.8};
    double C_act_prime[] = {0.6709, 0.6991, 0.7167, 0.7358, 0.7486};
    double C_act_primeprime[] = {0.2765, 0.3148, 0.3387, 0.3646, 0.3819};
    double C_myo[] = {0.0079, 0.0109, 0.0143, 0.0217, 0.0331};
    double C_shear[] = {0.0258, 0.0258, 0.0258, 0.0258, 0.0258};
    double mu[] = {2.09, 2.12, 2.43, 3.41, 5.16};
    double nParallel[] = {1, 4.33, 14.62, 93.06, 585.3};
    double length[] = {0.5453, 0.2242, 0.1563, 0.0945, 0.0588};
    C_symp = 3.0;
    C_tone_primeprime = 0;
    C_ATP_0 = 0.5;
    C_rbc_meta = 30.0;
    tau_d = 1.0;
    tau_a = 20.0;
    E = 1500000;


    // Assign P for myogenic module
    for (double * arr : {C_pass, C_pass_prime, D_0, C_act, C_act_prime, C_act_primeprime, C_myo, C_shear, mu, nParallel}) {
        for (int i = 0; i < nSegs; i++) {
            P_myo.push_back(arr[i]);
        }
    }

    for (double val : {C_symp, C_tone_primeprime, C_ATP_0, C_rbc_meta, tau_d, tau_a}){
        P_myo.push_back(val);
    }


    for (string s : {"Q"})
        myoShared.push_back(s + "_" + organSymbol);
    myoShared = expand(myoShared, nSegs);

    for (string s : {"D", "A", "P"})
        myoInputStates.push_back(s + "_" + organSymbol);
    myoInputStates = expand(myoInputStates, nSegs);

    for (string s : {"T_total", "A_total", "T"})
        myoAlgebraic.push_back(s + "_" + organSymbol);
    myoAlgebraic = expand(myoAlgebraic, nSegs);

    for (string s : {"D", "A"})
        myoStates.push_back(s + "_" + organSymbol);
    myoStates = expand(myoStates, nSegs);


//    myogenic * myoOrgan = new myogenic(organName, myoShared, myoInputStates, myoAlgebraic, myoStates, P_myo);
//    myoOrgan->setSegNum(nSegs);


    for (string s : {"P", "D"})
        flowInputStates.push_back(s + "_" + organSymbol);
    flowInputStates = expand(flowInputStates, nSegs);

    flowInputStates.insert(flowInputStates.begin(), inletPressure);

    flowAlgebraic = expand({"Q_" + organSymbol}, nSegs);

    flowStates = expand({"P_" + organSymbol}, nSegs);

    for (double * arr : {length, nParallel, mu}) {
        for (int i = 0; i < nSegs; i++) {
            P_flow.push_back(arr[i]);
        }
    }
    P_flow.push_back(E);

//    myoRC * myoFlow = new myoRC(organName+"Flow", {outletFlow}, flowInputStates, flowAlgebraic, flowStates, P_flow);
//    myoFlow->setSegNum(nSegs);



//    wrapper * mdl = new wrapper(std::vector<model *> {myoOrgan, myoFlow});
    wrapper * mdl = new wrapper();
    return mdl;

}

//wrapper * heldtWithMyogenicKidney()
//{
//    RCR * body[9];
//    body[0] = new RCR("Arteries", {"Q_up", "Q_kid_1", "Q_spl1", "Q_ll1"}, {"P_lv", "P_a"}, {"Q_lo"}, {"P_a"}, {0.006, 2.0});
//    body[0]->setHasValve(true);
//    body[1] = new RCR("Upper Body", {"Q_sup"}, {"P_a", "P_up"}, {"Q_up"}, {"P_up"}, {3.9, 8.0});
////    body[2] = new RCR("Kidneys", {"Q_kid2"}, {"P_a", "P_kid"}, {"Q_kid1"}, {"P_kid"}, {4.1, 15.0});
//    body[2] = new RCR("Splanchnic", {"Q_spl2"}, {"P_a", "P_spl"}, {"Q_spl1"}, {"P_spl"}, {3.0, 55.0});
//    body[3] = new RCR("Lower Legs", {"Q_ll2"}, {"P_a", "P_ll"}, {"Q_ll1"}, {"P_ll"}, {3.6, 19.0});
//    body[4] = new RCR("Sup. Vena Cava", {"Q_ri1"}, {"P_up", "P_sup"}, {"Q_sup"}, {"P_sup"}, {0.23, 15.0});
//    body[4]->setHasValve(true);
//    body[5] = new RCR("Abd. Vena Cava", {"Q_inf"}, {"P_kid_5", "P_spl", "P_ll", "P_abd"}, {"Q_kid_out", "Q_spl2", "Q_ll2"}, {"P_abd"}, {0.3, 0.18, 0.3, 25.0});
//    body[5]->setHasValve(true);
//    body[6] = new RCR("Inf. Vena Cava", {"Q_ri2"}, {"P_abd", "P_inf"}, {"Q_inf"}, {"P_inf"}, {0.015, 2.0});
//    body[6]->setHasValve(true);
//    body[7] = new RCR("Pulm. Arteries", {"Q_pv"}, {"P_rv", "P_pa"}, {"Q_pa"}, {"P_pa"}, {0.003, 4.3});
//    body[7]->setHasValve(true);
//    body[8] = new RCR("Pulm. Veins", {"Q_li"}, {"P_pa", "P_pv"}, {"Q_pv"}, {"P_pv"}, {0.08, 8.4});
//
//    heart * hrt = new heart({"Q_pv", "Q_lo"}, {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv", "P_sup", "P_inf", "P_pv"}, {"C_ra", "C_rv", "C_la", "C_lv", "Q_ri1", "Q_ri2", "Q_rav", "Q_li", "Q_lav"}, {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv"}, {0.8, 2.5, 0.13, 1.3, 0.07, 0.61, 0.5, 0.74, 0.3, 0.06, 0.015, 0.005, 0.01, 0.01});
//
//    RCR * kidneyInlet = new RCR("Kidney Inlet", {"Q_kid_1", "Q_kid_1", "Q_kid_1"}, {"P_a", "P_kid_i"}, {"Q_kid_i"}, {"P_kid_i"}, {0.08, 2.});
//    model * kidney = myogenicOrgan("P_kid_i", "Q_kid_o" , 5, "kidney", "kid");
//    RCR * kidneyIOutlet = new RCR("Kidney Outlet", {"Q_kid_out"}, {"P_kid_5", "P_kid_5", "P_kid_5", "P_kid_o"}, {"Q_kid_o", "Q_kid_o2", "Q_kid_o3"}, {"P_kid_o"}, {0.3,0.3,0.3, 2.0});
//
//
//    wrapper * mdl = new wrapper();
//
//    for (model * m : body)
//        mdl->addModel(m);
//
//    for (model * m : {(model *) hrt, (model *) kidneyInlet, kidney, (model *) kidneyIOutlet})
//        mdl->addModel(m);
//
//
////    model * list = {hrt, PA, PV, art, myo, veins};
//
////    wrapper * mdl = new wrapper();
//
//    mdl->init();
//    return mdl;
//}

vector<string> expand(vector<string> vec, int nSegs)
{
    vector<string> expanded;
    for (string s : vec)
    {
        for (int i = 1; i <= nSegs; i++)
            expanded.push_back(s + "_" + to_string(i));
    }
    return expanded;
}
