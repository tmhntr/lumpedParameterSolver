//
//  main.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-16.
//

#include <iostream>
#include "RCR.h"
#include "heart.h"
#include "wrapper.h"


using namespace std;

class basicSolver: public solver {
private:
//    model * mdl;
//    int NEQ = 3;
    double * DY;
    double * y;
    double deltat = 0.01;
public:
    basicSolver() : solver() {}
    basicSolver(model * m) : solver(m)
    {
        DY = (double *) calloc(getModel()->getNEQ(), sizeof(double));
        y = (double *) calloc(getModel()->getNEQ(), sizeof(double));
    }
    basicSolver(model * m, double y1[]) : basicSolver(m)
    {
        for (int i = 0; i < getModel()->getNEQ(); i++)
        {
            y[i] = y1[i];
        }
    }
    
    void solveStep(double t) {
        getModel()->updateAlgebraic(t, y);
        getModel()->getDY(t, y, DY);
        for (int i = 0; i < getModel()->getNEQ(); i++)
        {
            y[i] = y[i] + deltat * DY[i];
        }
    }
    
    double getY(int index) { return y[index]; }
    double getDT() { return deltat; }
    
    void print(double t)
    {
        cout<<t;
        for(int i = 0; i<getModel()->getNEQ(); i++)
        {
            cout<<"\t\t";
            cout<<y[i];
        }
        cout<<endl;
    }
};



int main(int argc, const char * argv[]) {
    // insert code here...
//    std::cout << "Hello, World!\n";
    
    std::vector<std::string> sharedNames {"Q_v"};
    std::vector<std::string> inputNames {"P_lv", "P_a"};
    std::vector<std::string> algebraicNames {"Q_a"};
    std::vector<std::string> outputNames {"P_a"};
    std::vector<double> parameters {0.05, 0.9};

    
    submodule * artery = new RCR("Arteries", sharedNames, inputNames, algebraicNames, outputNames, parameters);
    
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
    

    wrapper * mdl = new wrapper(std::vector<submodule *> {hrt, artery, vein, pulm});
    
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
    bool success = false;
    for (int i = 0; i < mdl->getModelList().size(); i++)
    {
//        vector<string> list = mdl->getModel(i)->getSharedNameList();
        for (int j = 0; j < mdl->getModel(i)->getSharedNameList().size(); j++)
        {
            for (int k = 0; k < mdl->getModelList().size(); k++)
            {
                if(mdl->getModel(i)->link(j, mdl->getModel(k), mdl->getModel(i)->getSharedName(j)))
                    success = true;
            }
            if(!success)
            {
                cout << "Could not find shared input " << mdl->getModel(i)->getSharedName(j) << " for module " << mdl->getModel(i)->getName() << endl;
                throw (mdl->getModel(i)->getSharedName(j));
            }
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

    double y[] = {2.0, 20.0, 1.0, 10.0, 1.0, 3.0, 8.0, 13.0, 90.0, 7.0, 15.0};
    basicSolver * slvr = new basicSolver(mdl, y);
    
    for (double t = 0.0; t <= 10.0; t+=slvr->getDT())
    {
        slvr->print(t);
        slvr->solveStep(t);
    }
    
    return 0;
}
