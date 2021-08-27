//
//  basicprinter.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-26.
//

#ifndef basicprinter_hpp
#define basicprinter_hpp

#include "interface.hpp"
#include <iomanip>

class basicprinter: public printer {
    solver * _slvr;
    bool titlesPrinted = false;
    
    void printTitles()
    {
        std::cout << "Time\t";
        for (int i = 0; i < _slvr->getModel()->getNEQ(); i++)
        {
            std::cout << _slvr->getModel()->getStateName(i) << "\t";
        }
        std::cout << std::endl;
        titlesPrinted = true;
    }
public:
    // Constructors
    basicprinter() : printer() {  }
    basicprinter(void * slvr) : printer() { setSolver(slvr); }
    
    void setSolver(void * slvr) { _slvr = (solver *) slvr; }
    void print()
    {
        std::cout << std::fixed << std::setprecision(3);
        if (!titlesPrinted) printTitles();
        double t = _slvr->getT();
        std::cout << t << "\t";
        for (int i = 0; i < _slvr->getModel()->getNEQ(); i++)
        {
            std::cout << _slvr->getY(i) << "\t";
        }
        std::cout << std::endl;
    }
    
    
};

#endif /* basicprinter_hpp */
