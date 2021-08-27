//
//  main.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-16.
//

#include <iostream>
#include "RCR.hpp"
#include "heart.hpp"
#include "wrapper.hpp"
#include "sdsolver.hpp"
#include "basicprinter.hpp"


using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
//    std::cout << "Hello, World!\n";
    
//    std::vector<std::string> sharedNames {"Q_v"};
//    std::vector<std::string> inputNames {"P_lv", "P_a"};
//    std::vector<std::string> algebraicNames {"Q_a"};
//    std::vector<std::string> outputNames {"P_a"};
//    std::vector<double> parameters {0.05, 0.9};
//
//    std::vector<submodule *> modules;
//    modules.push_back(new RCR("Arteries", {"Q_v"}, {"P_lv", "P_a"}, {"Q_a"}, {"P_a"}, {0.05, 0.9}));
//    submodule * artery = new RCR("Arteries", {"Q_v"}, {"P_lv", "P_a"}, {"Q_a"}, {"P_a"}, {0.05, 0.9});
    
//    sharedNames = {"Q_ri"};
//    inputNames = {"P_a", "P_v"};
//    algebraicNames = {"Q_v"};
//    outputNames = {"P_v"};
//    parameters = {3.0, 30.0};
    
//    submodule * vein = new RCR("Veins", sharedNames, inputNames, algebraicNames, outputNames, parameters);
    
//    sharedNames = {"Q_li"};
//    inputNames = {"P_rv", "P_pulm"};
//    algebraicNames = {"Q_pulm"};
//    outputNames = {"P_pulm"};
//    parameters = {0.083, 3.0};
    
//    submodule * pulm = new RCR("Pulmonary", sharedNames, inputNames, algebraicNames, outputNames, parameters);
//    
//    sharedNames = {"Q_pulm", "Q_a"};
//    inputNames = {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv", "P_v", "P_pulm"};
//    algebraicNames = {"Q_ri", "Q_rav", "Q_li", "Q_lav"};
//    outputNames = {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv"};
//    parameters = {0.8, 2.5, 0.13, 1.3, 0.07, 0.61, 0.5, 0.74, 0.3, 0.01, 0.005, 0.01, 0.01};
//    
//    submodule * hrt = new heart(sharedNames, inputNames, algebraicNames, outputNames, parameters);
    
//    modules.push_back(hrt);
//    modules.push_back(new RCR("Arteries", {"Q_v"}, {"P_lv", "P_a"}, {"Q_a"}, {"P_a"}, {0.05, 0.9}));
//    modules.push_back(vein);
//    modules.push_back(pulm);
    
    RCR * body[10];
    body[0] = new RCR("Arteries", {"Q_up", "Q_kid", "Q_spl", "Q_ll"}, {"P_lv", "P_a"}, {"Q_lo"}, {"P_a"}, {0.05, 2.0});
    body[1] = new RCR("Upper Body", {"Q_sup"}, {"P_a", "P_up"}, {"Q_up"}, {"P_up"}, {3.9, 8.0});
    body[2] = new RCR("Kidneys", {"Q_kido"}, {"P_a", "P_kid"}, {"Q_kid"}, {"P_kid"}, {4.1, 15.0});
    body[3] = new RCR("Splanchnic", {"Q_splo"}, {"P_a", "P_spl"}, {"Q_spl"}, {"P_spl"}, {3.0, 55.0});
    body[4] = new RCR("Lower Legs", {"Q_llo"}, {"P_a", "P_ll"}, {"Q_ll"}, {"P_ll"}, {3.6, 19.0});
    body[5] = new RCR("Sup. Vena Cava", {"Q_supo"}, {"P_up", "P_sup"}, {"Q_sup"}, {"P_sup"}, {0.23, 15.0});
    body[6] = new RCR("Abd. Vena Cava", {"Q_inf"}, {"P_kid", "P_spl", "P_ll", "P_abd"}, {"Q_kido", "Q_splo", "Q_llo"}, {"P_abd"}, {0.3, 0.18, 0.3, 25.0});
    body[7] = new RCR("Inf. Vena Cava", {"Q_info"}, {"P_abd", "P_inf"}, {"Q_inf"}, {"P_inf"}, {0.015, 2.0});
    body[8] = new RCR("Pulm. Arteries", {"Q_pv"}, {"P_rv", "P_pa"}, {"Q_pa"}, {"P_pa"}, {0.003, 4.3});
    body[9] = new RCR("Pulm. Veins", {"Q_li"}, {"P_pa", "P_pv"}, {"Q_pv"}, {"P_pv"}, {0.08, 8.4});
    
    heart * hrt = new heart({"Q_pv", "Q_lo"}, {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv", "P_sup", "P_inf", "P_pv"}, {"C_ra", "C_rv", "C_la", "C_lv", "Q_supo", "Q_info", "Q_rav", "Q_li", "Q_lav"}, {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv"}, {0.8, 2.5, 0.13, 1.3, 0.07, 0.61, 0.5, 0.74, 0.3, 0.06, 0.015, 0.005, 0.01, 0.01});
    
    wrapper * mdl = new wrapper(std::vector<submodule *> {hrt});
    for (int i = 0; i < 10; i++)
        mdl->addModel(body[i]);
//
//    RCR * art = new RCR("Arteries", {"Q_v"}, {"P_lv", "P_a"}, {"Q_a"}, {"P_a"}, {0.05, 0.9});
//    art->setHasValve(true);
//
//    heart * hrt = new heart({"Q_pulm", "Q_a"}, {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv", "P_v", "P_pulm"}, {"C_ra", "C_rv", "C_la", "C_lv", "Q_ri", "Q_rav", "Q_li", "Q_lav"}, {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv"}, {0.8, 2.5, 0.13, 1.3, 0.07, 0.61, 0.5, 0.74, 0.3, 0.01, 0.005, 0.01, 0.01});
//    RCR * ven = new RCR("Veins", {"Q_ri"}, {"P_a", "P_v"}, {"Q_v"}, {"P_v"}, {0.083, 3.0});
//    ven->setHasValve(true);
//    RCR * pul = new RCR("Pulmonary", {"Q_li"}, {"P_rv", "P_pulm"}, {"Q_pulm"}, {"P_pulm"}, {0.083, 3.0});
//    pul->setHasValve(true);
//    wrapper * mdl = new wrapper(std::vector<submodule *> {hrt, art, ven, pul});
    
//    RCR * m1 = new RCR("model1", {"Q_2"}, {"P_2", "P_1"}, {"Q_1"}, {"P_1"}, {0.1, 0.9});
//    RCR * m2 = new RCR("model2", {"Q_1"}, {"P_1", "P_2"}, {"Q_2"}, {"P_2"}, {0.8, 2});
//
//    wrapper * mdl = new wrapper(std::vector<submodule *> {m1, m2});
    mdl->init();
    
//    mdl->addModel(hrt);
//    mdl->addModel(artery);
//    mdl->addModel(vein);
//    mdl->addModel(pulm);


//    artery->assignInputIndices(mdl->stateNames, mdl->getNEQ());
//    vein->assignInputIndices(mdl->stateNames, mdl->getNEQ());
//    pulm->assignInputIndices(mdl->stateNames, mdl->getNEQ());
//    hrt->assignInputIndices(mdl->stateNames, mdl->getNEQ());
    
    // linker
//    bool success = false;
//    for (int i = 0; i < mdl->getModelList().size(); i++)
//    {
////        vector<string> list = mdl->getModel(i)->getSharedNameList();
//        for (int j = 0; j < mdl->getModel(i)->getSharedNameList().size(); j++)
//        {
//            try
//            {
//                mdl->link(mdl->getModel(i), j);
//            } catch (string e) {
//                cout << "Could not find algebraic parameter " << e << " needed for module " << mdl->getModel(i)->getName() << "." << endl;
//                return 1;
//            }
////            link(mdl->getModel(i), j);
//        }
//    }
    

    cout << mdl->getNEQ() << endl;
    double y0[] = {2.0, 20.0, 1.0, 10.0, 1.0, 3.0, 8.0, 13.0, 90.0, 50.0, 50.0, 50.0, 40.0, 7.0, 7.0, 7.0, 50.0, 15.0};
//    double y0[] = {2.0, 20.0};
    double reltol = 1e-6;
    double abstol[] = {1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6};
//    double abstol[] = {1e-6, 1e-6};
    sdsolver * slvr = new sdsolver(mdl, y0, reltol, abstol);
    
    slvr->setPrinter(new basicprinter((void *) slvr));
    
//    slvr->print();
    for (double t = 0.01; t <= 50.0; t+=slvr->getDeltaT())
    {
        slvr->print();
        slvr->solveStep(t);
    }
    slvr->print();
    
    
    return 0;
}
