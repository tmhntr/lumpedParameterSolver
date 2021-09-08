//
//  basicprinter.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-26.
//

#include "basicprinter.hpp"
#include <iomanip>

// public:
basicprinter::basicprinter() : printer() {  }
basicprinter::basicprinter(void * slvr) : printer() { setSolver(slvr); }

void basicprinter::setPrecision(int precision) { _precision = precision; }
int basicprinter::getPrecision() { return _precision; }

void basicprinter::setSolver(void * slvr) { _slvr = (solver *) slvr; }
void basicprinter::print()
{
    std::cout << std::fixed << std::setprecision(getPrecision());
    if (!titlesPrinted) printTitles();
    double t = _slvr->getT();
    std::cout << t << "\t";
    for (int i = 0; i < _slvr->getModel()->getNEQ(); i++)
    {
        std::cout << _slvr->getY(i) << "\t";
    }
    std::cout << std::endl;
}

// private:
void basicprinter::printTitles()
{
    std::cout << "Time\t";
    for (int i = 0; i < _slvr->getModel()->getNEQ(); i++)
    {
        std::cout << _slvr->getModel()->getStateName(i) << "\t";
    }
    std::cout << std::endl;
    titlesPrinted = true;
}
