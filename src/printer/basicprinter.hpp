//
//  basicprinter.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-26.
//

#ifndef basicprinter_hpp
#define basicprinter_hpp

#include <component/component_model.hpp>
#include <model/component.hpp>
#include <model/printer.hpp>
#include <model/solver.hpp>
// #include <iomanip>

class basicprinter : public printer {
public:
    // Constructors
    basicprinter();
    basicprinter(solver* slvr);

    void setPrecision(int precision);
    int getPrecision();
    void setSolver(void* slvr);
    void print();

private:
    solver* _slvr;
    bool titlesPrinted = false;
    int _precision = 2;

    void printTitles();
};

#endif /* basicprinter_hpp */
