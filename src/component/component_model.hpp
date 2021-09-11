//
//  component_model.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-21.
//

#ifndef component_model_hpp
#define component_model_hpp

#include <stdio.h>
#include <model/component.hpp>
//#include "wrapper.hpp"
#include <math.h>
#include <string>
#include <vector>

class linker
{
public:
    virtual double get() = 0;
};


class component_model: public component {
public:
    // Constructors and destructors
    component_model(std::string n, int neq, int np, int nIn, int nDerived);

    component_model(std::string n, std::vector<std::string> inputNames, std::vector<std::string> derivedNames, std::vector<std::string> outputNames, std::vector<double> parameters);
    virtual ~component_model(){}

    std::vector<component *> components();

    int init(component * parent);


    // getters and setters; arrays will have getters and setters for each index as well as the array pointer.

//  Parameter Array
    void setP(int index, double value);
    void setP(std::vector<double> P_vec);
    double getP(int index);
    std::vector<double> getPVec();

//  Linker array
//    For referencing algebraic variables from other submodules
//    shared(int) is also included to call the get function of the linker.
    void setLink(int index, linker * l);
    void setLink(std::vector<linker *> Link_vec);
    linker * getLink(int index);
    std::vector<linker *> getLinkVec();

    double input(int index);

    void setInputName(int index, std::string inputName);
    void setInputName(std::vector<std::string> Name_vec);
    std::string getInputName(int index);
    std::vector<std::string> getInputNameVec();


//  Derived quantity array
//    This stores all calculated values that are used in the function DYDT
    void setDerived(int index, double value);
    void setDerived(std::vector<double> A_vec);
    double getDerived(int index);
    std::vector<double> getDerivedVec();

    void setDerivedName(int index, std::string name);
    void setDerivedName(std::vector<std::string> Name_vec);
    std::string getDerivedName(int index);
    std::vector<std::string> getDerivedNameVec();
private:
    int _nP;
    int _nInputs;
    int _nLinks;
    int _neq;

    double * P;
    linker ** links;
    double * derived;

    /*
     Names.
     The following are used so set up the model, properly associating submodules to the required shared and input values
     */
    std::string * _inputNames;
    std::string * _derivedNames;

    void check(int index, int size);
};

class stateLinker : public linker
{
private:
    component * _source;
    int _index;
public:
    stateLinker(component * src, int index) : linker()
    {
        _source = src;
        _index = index;
    }
    double get()
    {
        // std::cout << "getting" << _index << std::endl;
        return _source->getY(_index);
    }
};

class derivedLinker : public linker
{
private:
    int _index;
    component_model * _source;
public:
    derivedLinker(component_model * src, int index) : linker()
    {
        _source = src;
        _index = index;
    }
    double get() { return _source->getDerived(_index); }
};


//class linker
//{
//private:
//    int _index;
//    submodule * _source;
//
//public:
////    linker() {}
//    linker(std::string query, std::vector<submodule *> modlist)
//    {
//        int index;
//        for (int i = 0; i < modlist.size(); i++)
//        {
//            try {
//                index = modlist[i]->getAlgebraicIndex(query);
//                _index = index;
//                _source = modlist[i];
//                return;
//            } catch (int e) { }
//        }
//        std::cout << "Link for " << query << " failed." << std::endl;
//        throw 1;
//    }
//    double get() { return _source->getAlgebraic(_index); }
//    void setIndex(int index) { _index = index; }
//    int getIndex() { return _index; }
//    void setSource(submodule * source) { _source = source; }
//    submodule * getSource() { return _source; }
//};




#endif /* component_model_hpp */
