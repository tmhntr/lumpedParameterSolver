//
//  main.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-16.
//

#include <iostream>
#include "interface.h"
#include "RCR.h"
#include "heart.h"
#include "wrapper.h"


using namespace std;

class basicSolver: public solver {
private:
    model * mdl;
    int NEQ = 2;
    double DY[2] = {0, 0};
    double y[2] = {0.0, 90.0};
    double deltat = 0.01;
public:
    void solveStep(double t) {
        mdl->updateAlgebraic(t, y);
        mdl->getDY(t, y, DY);
        for (int i = 0; i < NEQ; i++)
        {
            y[i] = y[i] + deltat * DY[i];
        }
    }
    void setModel(model * m) {
        mdl = m;
    }
    
    double getY(int index) { return y[index]; }
    
    void print(double t)
    {
        cout<<t;
        for(int i = 0; i<NEQ; i++)
        {
            cout<<"\t";
            cout<<y[i];
        }
        cout<<endl;
    }
};



int main(int argc, const char * argv[]) {
    // insert code here...
//    std::cout << "Hello, World!\n";
    
    int II[] = {0, 1, 2, 3, 4, 5, 6, 7, 9, 10};
    double Pheart[] = {0.02, 2.3, 0.02, 2.3, 0.02, 2.3, 0.02, 2.3, 0.02, 2.3, 0.02, 2.3, 0.02, 2.3};
    cout<<"loading heart"<<endl;
    modelBlock *h = new heart("heart", 8, II, Pheart); // DY 0-7
    cout<<"loaded"<<endl;
    II[0] = 7; II[1] = 8;
    double Prcr[] = {1.2, 0.2};
//    cout<<"loading rcr1"<<endl;
    modelBlock *arteries = new RCR("arteries", 1, II, Prcr); // DY 8
//    cout<<"loaded"<<endl;
    II[0] = 8; II[1] = 9;
    modelBlock *veins = new RCR("veins", 1, II, Prcr); // DY 9
    II[0] = 5; II[1] = 10;
    modelBlock *pulm = new RCR("pulm", 1, II, Prcr); // DY 10
    
    arteries->addShared(veins->getAlgebraicPtr(0)); // adding venous inlet as arterial outlet
    
    veins->addShared(h->getAlgebraicPtr(4)); // adding q_ri as venous outlet
    
    h->addShared(pulm->getAlgebraicPtr(0)); // adding pulmonary flow as q_ro
    h->addShared(arteries->getAlgebraicPtr(0)); // adding arterial flow as q_lo


    wrapper * test = new wrapper();
    cout<<"adding models into wrapper"<<endl;
    test->addModel(h); test->addModel(arteries); test->addModel(veins); test->addModel(pulm);
    cout<<"added"<<endl;
    
    double deltat = 0.01;
    
    cout<<"initializing solver"<<endl;
    basicSolver * slvr = new basicSolver();
    slvr->setModel(test);
    cout<<"initialized"<<endl;
    
    for(double t = 0.0; t < 2; t+=deltat)
    {
        slvr->print(t);
        slvr->solveStep(t);
    }
    
    
    
    
    
    return 0;
}
