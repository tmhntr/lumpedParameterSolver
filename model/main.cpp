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
    
    std::vector<std::string> sharedNames {"Q_v"};
    std::vector<std::string> inputNames {"P_lv", "P_a"};
    std::vector<std::string> algebraicNames {"Q_a"};
    std::vector<std::string> outputNames {"P_a"};
    std::vector<double> parameters {0.05, 0.9};

    std::vector<submodule *> modules;
//    modules.push_back(new RCR("Arteries", {"Q_v"}, {"P_lv", "P_a"}, {"Q_a"}, {"P_a"}, {0.05, 0.9}));
//    submodule * artery = new RCR("Arteries", {"Q_v"}, {"P_lv", "P_a"}, {"Q_a"}, {"P_a"}, {0.05, 0.9});
    
    sharedNames = {"Q_ri"};
    inputNames = {"P_a", "P_v"};
    algebraicNames = {"Q_v"};
    outputNames = {"P_v"};
    parameters = {3.0, 30.0};
    
    submodule * vein = new RCR("Veins", sharedNames, inputNames, algebraicNames, outputNames, parameters);
    
    sharedNames = {"Q_li"};
    inputNames = {"P_rv", "P_pulm"};
    algebraicNames = {"Q_pulm"};
    outputNames = {"P_pulm"};
    parameters = {0.083, 3.0};
    
    submodule * pulm = new RCR("Pulmonary", sharedNames, inputNames, algebraicNames, outputNames, parameters);
    
    sharedNames = {"Q_pulm", "Q_a"};
    inputNames = {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv", "P_v", "P_pulm"};
    algebraicNames = {"Q_ri", "Q_rav", "Q_li", "Q_lav"};
    outputNames = {"C_ra", "C_rv", "C_la", "C_lv", "P_ra", "P_rv", "P_la", "P_lv"};
    parameters = {0.8, 2.5, 0.13, 1.3, 0.07, 0.61, 0.5, 0.74, 0.3, 0.01, 0.005, 0.01, 0.01};
    
    submodule * hrt = new heart(sharedNames, inputNames, algebraicNames, outputNames, parameters);
    
    modules.push_back(hrt);
    modules.push_back(new RCR("Arteries", {"Q_v"}, {"P_lv", "P_a"}, {"Q_a"}, {"P_a"}, {0.05, 0.9}));
    modules.push_back(vein);
    modules.push_back(pulm);
    wrapper * mdl = new wrapper(modules);
    
//    mdl->addModel(hrt);
//    mdl->addModel(artery);
//    mdl->addModel(vein);
//    mdl->addModel(pulm);

    mdl->updateStateNames();
    for (int i = 0; i < 4; i++)
    {
        try {
            mdl->getModel(i)->assignInputIndices(mdl->getStateNames(), mdl->getNEQ());
        } catch (...) {
            cout << "Input assignment for " << mdl->getModel(i)->getName() << " module failed... Exiting..." << endl;
            return 1;
        }
    }
//    artery->assignInputIndices(mdl->stateNames, mdl->getNEQ());
//    vein->assignInputIndices(mdl->stateNames, mdl->getNEQ());
//    pulm->assignInputIndices(mdl->stateNames, mdl->getNEQ());
//    hrt->assignInputIndices(mdl->stateNames, mdl->getNEQ());
    
    // linker
//    bool success = false;
    for (int i = 0; i < mdl->getModelList().size(); i++)
    {
//        vector<string> list = mdl->getModel(i)->getSharedNameList();
        for (int j = 0; j < mdl->getModel(i)->getSharedNameList().size(); j++)
        {
            try
            {
                mdl->link(mdl->getModel(i), j);
            } catch (string e) {
                cout << "Could not find algebraic parameter " << e << " needed for module " << mdl->getModel(i)->getName() << "." << endl;
                return 1;
            }
//            link(mdl->getModel(i), j);
        }
    }
    
//    artery->link(0, vein, "Q_v");
//    vein->link(0, hrt, "Q_ri");
//
//    hrt->link(0, pulm, "Q_pulm");
//    pulm->link(0, hrt, "Q_li");
//
//    hrt->link(1, artery, "Q_a");

    
//    cout << m1->getName() << "input indices: " << m1->getII(0) << ", " << m1->getII(1) << endl;

    double y0[] = {2.0, 20.0, 1.0, 10.0, 1.0, 3.0, 8.0, 13.0, 90.0, 7.0, 15.0};
    double reltol = 1e-6;
    double abstol[] = {1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6};
    sdsolver * slvr = new sdsolver(mdl, y0, reltol, abstol);
    
    slvr->setPrinter(new basicprinter((void *) slvr));
    
//    slvr->print();
    for (double t = 0.01; t <= 10.0; t+=slvr->getDeltaT())
    {
        slvr->print();
        slvr->solveStep(t);
    }
    
    return 0;
}
