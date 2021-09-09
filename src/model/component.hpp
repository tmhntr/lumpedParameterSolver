//
//  submodule.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-21.
//

#ifndef component_hpp
#define component_hpp

#include <stdio.h>
#include "model.hpp"
//#include "wrapper.hpp"
#include <math.h>
#include <string>
#include <vector>

class linker
{
public:
    virtual double get() = 0;
};


class component: public model {
private:
    int _nP;
    int _nInputs;
    int _nDerived;
    int _neq;
    double * _y;


    double * P;
    linker ** links;
    double * derived;

    /*
     Names.
     The following are used so set up the model, properly associating submodules to the required shared and input values
     */
    std::string * inputNames;
    std::string * derivedNames;

public:
    // Constructors and destructors
    component(std::string n, int neq, int np, int nIn, int nDerived) : model(n)
    {
        setNEQ(neq);
        _nP = np;
        _nInputs = nIn;
        _nDerived = nDerived;

        P = (double *) calloc(_nP, sizeof(double));
        derived = (double *) calloc(_nDerived, sizeof(double));
        links = (linker **) calloc(_nInputs, sizeof(linker *));

        _stateNames.resize(getNEQ());
        derivedNames = (std::string *) calloc(_nDerived, sizeof(std::string));

        inputNames = (std::string *) calloc(_nInputs, sizeof(std::string));
    }

    component(std::string n, std::vector<std::string> inputNames, std::vector<std::string> derivedNames, std::vector<std::string> outputNames, std::vector<double> parameters) : component(n, (int) outputNames.size(), (int) parameters.size(), (int) inputNames.size(), (int) derivedNames.size())
    {
        for (int i = 0; i < inputNames.size(); i++)
            setInputName(i, inputNames[i]);
        for (int i = 0; i < derivedNames.size(); i++)
            setDerivedName(i, derivedNames[i]);
        for (int i = 0; i < outputNames.size(); i++)
            setStateName(i, outputNames[i]);
        for (int i = 0; i < parameters.size(); i++)
            setP(i, parameters[i]);
    }
    virtual ~component(){}

    std::vector<model *> components() { return {this}; }

    int init(model * modlist);


    // getters and setters; arrays will have getters and setters for each index as well as the array pointer.

//  Parameter Array
    void setP(int index, double value)
    {
        if (index < _nP)
            P[index] = value;
        else
        {
            std::cout << index << " outside of parameter array range." << std::endl;
            throw (1);
        }
    }
    void setP(std::vector<double> P_vec)
    {
        if (P_vec.size() == _nP)
            for (int i = 0; i < P_vec.size(); i++) P[i] = P_vec[i];
        else
        {
            std::cout << "Input vector is " << P_vec.size() << ", nParameters is " << _nP <<". Wrong size." << std::endl;
            throw (1);
        }
    }
    double getP(int index)
    {
        if (index >= _nP)
        {
            std::cout << index << " outside of parameter array range." << std::endl;
            throw (1);
        }
        else
            return P[index];
    }
    std::vector<double> getPVec() { return std::vector<double> (P, P+_nP); }

//  Linker array
//    For referencing algebraic variables from other submodules
//    shared(int) is also included to call the get function of the linker.
    void setLink(int index, linker * l)
    {
        if (index < _nInputs)
            links[index] = l;
        else
        {
            std::cout << index << " outside of link array range." << std::endl;
            throw (1);
        }
    }
    void setLink(std::vector<linker *> Link_vec)
    {
        if (Link_vec.size() == _nInputs)
            for (int i = 0; i < Link_vec.size(); i++) links[i] = Link_vec[i];
        else
        {
            std::cout << "Input vector is " << Link_vec.size() << ", nInputs is " << _nInputs <<". Wrong size." << std::endl;
            throw (1);
        }
    }
    linker * getLink(int index)
    {
        if (index >= _nInputs)
        {
            std::cout << index << " outside of links array range." << std::endl;
            throw (1);
        } else
            return links[index];

    }
    std::vector<linker *> getLinkVec() { return std::vector<linker *> (links, links+_nInputs); }

    double input(int index)
    {
        return links[index]->get();
    }


//  Derived quantity array
//    This stores all calculated values that are used in the function DYDT
    void setDerived(int index, double value)
    {
        if (index < _nDerived)
            derived[index] = value;
        else
        {
            std::cout << index << " outside of derived array range." << std::endl;
            throw (1);
        }
    }
    void setDerived(std::vector<double> A_vec)
    {
        if (A_vec.size() == _nDerived)
            for (int i = 0; i < A_vec.size(); i++) derived[i] = A_vec[i];
        else
        {
            std::cout << "Input vector is " << A_vec.size() << ", nDerived is " << _nDerived <<". Wrong size." << std::endl;
            throw (1);
        }
    }
    double getDerived(int index)
    {
        if (index >= _nDerived)
        {
            std::cout << index << " outside of derived array range." << std::endl;
            throw (1);
        }
        else
            return derived[index];
    }
    std::vector<double> getDerivedVec() { return std::vector<double> (derived, derived+_nDerived); }



    void setInputName(int index, std::string inputName)
    {
        if (index < _nInputs)
            inputNames[index] = inputName;
        else
        {
            std::cout << index << " outside of inputs array range." << std::endl;
            throw (1);
        }
    }
    void setInputName(std::vector<std::string> Name_vec)
    {
        if (Name_vec.size() == _nInputs)
            for (int i = 0; i < Name_vec.size(); i++) inputNames[i] = Name_vec[i];
        else
        {
            std::cout << "Input vector is " << Name_vec.size() << ", nInputs is " << _nInputs <<". Wrong size." << std::endl;
            throw (1);
        }
    }
    std::string getInputName(int index)
    {
        if (index >= _nInputs)
        {
            std::cout << index << " outside of input array range." << std::endl;
            throw (1);
        }
        else
            return inputNames[index];
    }
    std::vector<std::string> getInputNameVec() { return std::vector<std::string> (inputNames, inputNames+_nInputs); }


    void setDerivedName(int index, std::string name)
    {
        if (index < _nDerived)
            derivedNames[index] = name;
        else
        {
            std::cout << index << " outside of derived array range." << std::endl;
            throw (1);
        }
    }
    void setDerivedName(std::vector<std::string> Name_vec)
    {
        if (Name_vec.size() == _nDerived)
            for (int i = 0; i < Name_vec.size(); i++) derivedNames[i] = Name_vec[i];
        else
        {
            std::cout << "Input vector is " << Name_vec.size() << ", nParameters is " << _nDerived <<". Wrong size." << std::endl;
            throw (1);
        }
    }
    std::string getDerivedName(int index)
    {
        if (index >= _nDerived)
        {
            std::cout << index << " outside of parameter array range." << std::endl;
            throw (1);
        }
        else
            return derivedNames[index];
    }
    std::vector<std::string> getDerivedNameVec() { return std::vector<std::string> (derivedNames, derivedNames+_nDerived); }

};

class stateLinker : public linker
{
private:
    model * _source;
    int _index;
public:
    stateLinker(model * src, int index) : linker()
    {
        _source = src;
        _index = index;
    }
    double get()
    {
        double * Y = _source->getY();
        return Y[_index];
    }
};

class derivedLinker : public linker
{
private:
    int _index;
    component * _source;
public:
    derivedLinker(component * src, int index) : linker()
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




#endif /* component_hpp */
