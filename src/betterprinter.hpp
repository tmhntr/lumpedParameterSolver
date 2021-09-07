//
//  betterprinter.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-31.
//

#ifndef betterprinter_h
#define betterprinter_h

#include "interface.hpp"
#include "wrapper.hpp"
#include "component.hpp"
#include <iomanip>
#include <vector>
#include <string>


class betterprinter: public printer {
    solver * _slvr;
    wrapper * _model;
    
    bool titlesPrinted = false;
    bool _split = true;
    bool _printAlg = true;
    int precision = 3;
    
    void printStateTitles(component * model)
    {

        std::string fn =fname(model, 's');
        FILE * fid = fopen(fn.data(), "w");
        
        fprintf(fid, "Time");
        for (int i = 0; i < model->getNEQ(); i++)
        {
            fprintf(fid, "\t%s", model->getStateName(i).data());
        }
        fprintf(fid, "\n");
        fclose(fid);
    }
    
    void printStates(component * model)
    {
        std::string fn =fname(model, 's');
        FILE * fid = fopen(fn.data(), "a");
        
        
        fprintf(fid, "%.4f", _slvr->getT());
        for (int i = 0; i < model->getNEQ(); i++)
        {
            for (int j = 0; j < _model->getNEQ(); j++)
            {
                if (model->getStateName(i) == _model->getStateName(j))
                {
                    fprintf(fid, "\t%.8f", _slvr->getY(j));
                    break;
                }
            }
        }
        fprintf(fid, "\n");
        fclose(fid);
    }
    
    std::string fname(model * model, char type)
    {
        std::string fn, modelname;
        modelname = model->getName();
        modelname.erase(remove_if(modelname.begin(), modelname.end(), isspace), modelname.end());
        switch (type) {
            case 'a':
                fn = modelname + "_Derived.dat";
                break;
            case 's':
                fn = modelname + "_States.dat";
                break;
            default:
                break;
        }
        return fn;
    }
    
    void printAlgebraicTitles(component * model)
    {
        std::string fn =fname(model, 'a');
        FILE * fid = fopen(fn.data(), "w");
        
        
        fprintf(fid, "Time");
        for (int i = 0; i < model->getDerivedNameVec().size(); i++)
        {
            fprintf(fid, "\t%s", model->getDerivedName(i).data());
        }
        fprintf(fid, "\n");
        fclose(fid);
//        titlesPrinted = true;
    }
    void printAlgebraic(component * model)
    {
        std::string fn =fname(model, 'a');
        FILE * fid = fopen(fn.data(), "a");
        
        
        fprintf(fid, "%.4f", _slvr->getT());
        for (int i = 0; i < model->getDerivedNameVec().size(); i++)
        {
            fprintf(fid, "\t%.8f", model->getDerived(i));
        }
        fprintf(fid, "\n");
        fclose(fid);
    }

    void printTitles()
    {
        if (_split)
        {
            std::vector<component *> list = _model->flattenModList();
            for (component * m : list) {
                printAlgebraicTitles(m);
                printStateTitles(m);
            }
        } else
            std::cout << "Printing together not yet implemented" << std::endl;
        
        titlesPrinted = true;
    }
    void printValues()
    {
        if (_split)
        {
            std::vector<component *> list = _model->flattenModList();
            for (component * m : list) {
                printAlgebraic(m);
                printStates(m);
            }
        } else
            std::cout << "Printing together not yet implemented" << std::endl;
    }
    
public:
    // Constructors
    betterprinter() : printer() {  }
    betterprinter(void * slvr, wrapper * mdl) : printer()
    {
        setSolver(slvr);
        setModel(mdl);
    }
    
    void setSolver(void * slvr) { _slvr = (solver *) slvr; }
    void setModel(wrapper * mdl) { _model = mdl; }
    void print()
    {
        if (!titlesPrinted) printTitles();
        printValues();
    }
    
    
};

#endif /* betterprinter_h */
