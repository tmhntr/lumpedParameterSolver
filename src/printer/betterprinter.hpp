//
//  betterprinter.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-31.
//

#ifndef betterprinter_h
#define betterprinter_h

#include <model/printer.hpp>
#include <model/component.hpp>
#include <component/component_model.hpp>
#include <model/solver.hpp>
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>


class betterprinter: public printer {
    solver * _slvr;
    component * _model;

    bool titlesPrinted = false;
    bool _split = true;
    bool _printAlg = true;
    int precision = 3;

    void printStateTitles(component * model);

    void printStates(component * model);

    std::string makeFileName(component * model, char type);

    void printAlgebraicTitles(component * model);
    void printAlgebraic(component * model);

    void printTitles();
    void printValues();

public:
    // Constructors
    betterprinter();
    betterprinter(solver * slvr);

    void setSolver(void * slvr);
    void setModel(component * mdl);
    void print();


};

#endif /* betterprinter_h */
