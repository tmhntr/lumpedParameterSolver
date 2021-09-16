#include "betterprinter.hpp"

void betterprinter::printStateTitles(component * model)
{

    std::string fn =makeFileName(model, 's');
    FILE * fid = fopen(fn.data(), "w");

    fprintf(fid, "Time");
    for (int i = 0; i < model->getNEQ(); i++)
    {
        fprintf(fid, "\t%s", model->getStateName(i).data());
    }
    fprintf(fid, "\n");
    fclose(fid);
}

void betterprinter::printStates(component * model)
{
    std::string fn =makeFileName(model, 's');
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

std::string betterprinter::makeFileName(component * model, char type)
{
    std::string fn, modelname;
    modelname = model->getName();
    modelname.erase(std::remove_if(modelname.begin(), modelname.end(), isspace), modelname.end());
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

void betterprinter::printAlgebraicTitles(component * model)
{
    std::string fn =makeFileName(model, 'a');
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
void betterprinter::printAlgebraic(component * model)
{
    std::string fn =makeFileName(model, 'a');
    FILE * fid = fopen(fn.data(), "a");


    fprintf(fid, "%.4f", _slvr->getT());
    for (int i = 0; i < model->getDerivedNameVec().size(); i++)
    {
        fprintf(fid, "\t%.8f", model->getDerived(i));
    }
    fprintf(fid, "\n");
    fclose(fid);
}

void betterprinter::printTitles()
{
    if (_split)
    {
        std::vector<component *> list = _model->components();
        for (component * m : list) {
            printAlgebraicTitles((component *) m);
            printStateTitles((component *) m);
        }
    } else
        std::cout << "Printing together not yet implemented" << std::endl;

    titlesPrinted = true;
}
void betterprinter::printValues()
{
    if (_split)
    {
        std::vector<component *> list = _model->components();
        for (component * m : list) {
            printAlgebraic( m);
            printStates( m);
        }
    } else
        std::cout << "Printing together not yet implemented" << std::endl;
}

betterprinter::betterprinter() : printer() {  }
betterprinter::betterprinter(solver * slvr) : printer()
{
    setSolver((void *) slvr);
    setModel(_slvr->getModel());
}

void betterprinter::setSolver(void * slvr) { _slvr = (solver *) slvr; }
void betterprinter::setModel(component * mdl) { _model = mdl; }
void betterprinter::print()
{
    if (!titlesPrinted) printTitles();
    printValues();
}
