//
//  betterprinter.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-31.
//

#ifndef betterprinter_h
#define betterprinter_h

#include "printer/printer.hpp"
#include "model/model.hpp"
#include "model/component.hpp"
#include "solver/solver.hpp"
#include <iomanip>
#include <vector>
#include <string>
#include <algorithm>


class betterprinter: public printer {
    solver * _slvr;
    model * _model;

    bool titlesPrinted = false;
    bool _split = true;
    bool _printAlg = true;
    int precision = 3;

    void printStateTitles(component * model);

    void printStates(component * model);

    std::string fname(model * model, char type);

    void printAlgebraicTitles(component * model);
    void printAlgebraic(component * model);

    void printTitles();
    void printValues();

public:
    // Constructors
    betterprinter();
    betterprinter(void * slvr);

    void setSolver(void * slvr);
    void setModel(model * mdl);
    void print();


};

#endif /* betterprinter_h */
