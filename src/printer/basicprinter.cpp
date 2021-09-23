//
//  basicprinter.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-26.
//

#include "basicprinter.hpp"

#include <iomanip>

// public:
// CONSTRUCTORS
basicprinter::basicprinter()
    : printer()
{
}
basicprinter::basicprinter(solver* slvr)
    : printer()
{
    setSolver((void*)slvr);
}

// ACCESSORS
int basicprinter::getPrecision() { return _precision; }

// MANIPULATORS
void basicprinter::setSolver(void* slvr) { _slvr = (solver*)slvr; }
void basicprinter::setPrecision(int precision) { _precision = precision; }

// PUBLIC METHODS
void basicprinter::print()
{
    std::cout << std::fixed << std::setprecision(getPrecision());
    if (!titlesPrinted)
        printTitles();
    double t = _slvr->getT();
    std::cout << t << "\t";
    for (int i = 0; i < _slvr->getModel()->getNEQ(); i++) {
        std::cout << _slvr->getY(i) << "\t";
    }
    std::cout << std::endl;
}

// private:
// PRIVATE METHODS
void basicprinter::printTitles()
{
    std::cout << "Time\t";
    for (int i = 0; i < _slvr->getModel()->getNEQ(); i++) {
        std::cout << _slvr->getModel()->getStateName(i) << "\t";
    }
    std::cout << std::endl;
    titlesPrinted = true;
}
