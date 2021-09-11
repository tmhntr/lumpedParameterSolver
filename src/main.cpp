//
//  main.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-16.
//

#include <iostream>
#include <model/solver.hpp>
#include <model/printer.hpp>
#include <model/component.hpp>

#include "solver/sdsolver.hpp"
#include "printer/betterprinter.hpp"
#include "component/component_wrapper.hpp"
#include "component/component_model.hpp"

#include "rat.cpp"
#include "dialyzer.cpp"

#include <string>


using namespace std;


int main(int argc, const char * argv[]) {
    // insert code here...
    double deltat = 0.01;

    double reltol = 1e-8;
//    double abstol[] = {1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4, 1e-4};
    double abstol[] = {1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6, 1e-6};
//    double abstol[] = {1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8, 1e-8};
// //    double abstol[] = {1e-6, 1e-6};

//  Assign initial conditions:

    // vector<double> y0 = {15.20531, 1.0, 0.2042, 0.232, 0.149, 0, 1.6697, 0.0, 1.2921, 0.5*1.0567, 0.44066, 0.38685, 1.8*5.8495, 0.41389, 1.8*2.3484, 1.249, 0, 0, 0};
    // vector<double> y0 = {100.0, 250.0, 3535.0, 84.0, 10.0, 100.0, 0.0, 55.0, 2130.0, 75.0, 1470.0, 100.0, 100.0, 0.0, 25.0, 11.0, 3.25};


    vector<double> y0 = {15.20531, 1.0, 0.2042, 0.232, 0.149, 0, 1.6697, 0.0, 1.2921, 0.5*1.0567, 0.44066, 0.38685, 1.8*5.8495, 0.41389, 1.8*2.3484, 1.249, 0, 0, 0, 100.0, 250.0, 3535.0, 84.0, 10.0, 100.0, 0.0, 55.0, 2130.0, 75.0, 1470.0, 100.0, 100.0, 0.0, 25.0, 11.0, 3.25};

    component_model * r = (component_model *) rat();
    component_model * d = (component_model *) dialyzer();
    component_wrapper * mdl = new component_wrapper({r, d});
    // component * d = dialyzer();
    mdl->init(mdl);
    mdl->setY(y0);
    // std::vector<double> raty = {15.20531, 1.0, 0.2042, 0.232, 0.149, 0, 1.6697, 0.0, 1.2921, 0.5*1.0567, 0.44066, 0.38685, 1.8*5.8495, 0.41389, 1.8*2.3484, 1.249, 0, 0, 0};

    // cout << r->getDerived(0) << endl;

    // cout<< y0.size() - mdl->getNEQ()<< endl;


    // mdl = d;

    sdsolver * slvr = new sdsolver(mdl, y0.data(), reltol, abstol, &deltat);


    // cout<< r->getNEQ() << endl;
    // cout<< d->getNEQ() << endl;


//    slvr->setPrinter(new betterprinter((void *) slvr, mdl));
    slvr->setPrinter(new betterprinter(slvr));
   // slvr->print();
    double tstop = 2.0;
    if (argc > 1)
        tstop = atof(argv[1]);
    for (double t = slvr->getDeltaT(); t <= tstop; t+=slvr->getDeltaT())
    {
        try
        {
            // cout << "Solving step to t: " << t << endl;
            slvr->solveStep(t);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            cout<<"Solver failed at timestep " << t << endl;
            return 1;
        }

        slvr->print();
    }
    slvr->print();
//

    std::cout << "ran through!!" << '\n';
    return 0;
}
