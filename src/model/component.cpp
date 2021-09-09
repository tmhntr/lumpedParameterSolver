//
//  submodule.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-27.
//

#include <stdio.h>
#include "component.hpp"
#include <vector>
#include <type_traits>

component::component(std::string n, int neq, int np, int nIn, int nDerived) : model(n)
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

component::component(std::string n, std::vector<std::string> inputNames, std::vector<std::string> derivedNames, std::vector<std::string> outputNames, std::vector<double> parameters) : component(n, (int) outputNames.size(), (int) parameters.size(), (int) inputNames.size(), (int) derivedNames.size())
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

int component::init(model * parent)
{
    bool linked;
    std::vector<model *> modlist = parent->components();
    int failed = 0;
    for (int i = 0; i < _nInputs; i++)
    {
        linked = false;
        // search through statevars to find the input first
        for (int j = 0; j < parent->getNEQ(); j++)
        {
            if (getInputName(i) == parent->getStateName(j))
            {
                links[i] = new stateLinker(this, j);
                linked = true;
                break;
            }
        }

        std::vector<model *>::iterator m = modlist.begin();
        component * cmp;
        std::vector<std::string> derivedList;

        while (!linked && m != modlist.end())
        {
            cmp = (component *) (*m);
            derivedList = cmp->getDerivedNameVec();
            for (int j = 0; j < derivedList.size(); j++)
            {
                if (getInputName(i) == derivedList[j])
                {
                    links[i] = new derivedLinker(cmp, j);
                    linked = true;
                    break;
                }
            }
            m++;
        }

        if (!linked)
        {
            std::cout << "Failed to link input " << getInputName(i) << " for component " << this->getName() << std::endl;
            failed++;
        }
    }
    return failed;
}

void component::check(int index, int size)
{
  if (index >= size)
  {
      std::cout << index << " outside of array range." << std::endl;
      throw (1);
  }
}

std::vector<model *> component::components() { return {this}; }

// getters and setters; arrays will have getters and setters for each index as well as the array pointer.

//  Parameter Array
void component::setP(int index, double value)
{
    check(index, _nP);
    P[index] = value;
}
void component::setP(std::vector<double> P_vec)
{
    if (P_vec.size() == _nP)
        for (int i = 0; i < P_vec.size(); i++) P[i] = P_vec[i];
    else
    {
        std::cout << "Input vector is " << P_vec.size() << ", nParameters is " << _nP <<". Wrong size." << std::endl;
        throw (1);
    }
}
double component::getP(int index)
{
    check(index, _nP);
    return P[index];
}

std::vector<double> component::getPVec()
{
    return std::vector<double> (P, P+_nP);
}

//  Linker array
//    For referencing algebraic variables from other submodules
//    shared(int) is also included to call the get function of the linker.
void component::setLink(int index, linker * l)
{
    check(index, _nInputs);
    links[index] = l;
}
void component::setLink(std::vector<linker *> Link_vec)
{
    if (Link_vec.size() == _nInputs)
        for (int i = 0; i < Link_vec.size(); i++) links[i] = Link_vec[i];
    else
    {
        std::cout << "Input vector is " << Link_vec.size() << ", nInputs is " << _nInputs <<". Wrong size." << std::endl;
        throw (1);
    }
}
linker * component::getLink(int index)
{
    check(index, _nInputs);
    return links[index];
}
std::vector<linker *> component::getLinkVec()
{
    return std::vector<linker *> (links, links+_nInputs);
}

double component::input(int index)
{
    check(index, _nInputs);
    return links[index]->get();
}

//  Derived quantity array
//    This stores all calculated values that are used in the function DYDT
void component::setDerived(int index, double value)
{
    check(index, _nDerived);
    derived[index] = value;
}
void component::setDerived(std::vector<double> A_vec)
{
    if (A_vec.size() == _nDerived)
        for (int i = 0; i < A_vec.size(); i++) derived[i] = A_vec[i];
    else
    {
        std::cout << "Input vector is " << A_vec.size() << ", nDerived is " << _nDerived <<". Wrong size." << std::endl;
        throw (1);
    }
}
double component::getDerived(int index)
{
    check(index, _nDerived);
    return derived[index];
}
std::vector<double> component::getDerivedVec()
{
    return std::vector<double> (derived, derived+_nDerived);
}

void component::setInputName(int index, std::string inputName)
{
    check(index, _nInputs);
    inputNames[index] = inputName;
}
void component::setInputName(std::vector<std::string> Name_vec)
{
    if (Name_vec.size() == _nInputs)
        for (int i = 0; i < Name_vec.size(); i++) inputNames[i] = Name_vec[i];
    else
    {
        std::cout << "Input vector is " << Name_vec.size() << ", nInputs is " << _nInputs <<". Wrong size." << std::endl;
        throw (1);
    }
}
std::string component::getInputName(int index)
{
    check(index, _nInputs);
    return inputNames[index];
}
std::vector<std::string> component::getInputNameVec()
{
    return std::vector<std::string> (inputNames, inputNames+_nInputs);
}

void component::setDerivedName(int index, std::string name)
{
    check(index, _nDerived);
    derivedNames[index] = name;
}
void component::setDerivedName(std::vector<std::string> Name_vec)
{
    if (Name_vec.size() == _nDerived)
        for (int i = 0; i < Name_vec.size(); i++) derivedNames[i] = Name_vec[i];
    else
    {
        std::cout << "Input vector is " << Name_vec.size() << ", nParameters is " << _nDerived <<". Wrong size." << std::endl;
        throw (1);
    }
}
std::string component::getDerivedName(int index)
{
    check(index, _nDerived);
    return derivedNames[index];
}
std::vector<std::string> component::getDerivedNameVec()
{
    return std::vector<std::string> (derivedNames, derivedNames+_nDerived);
}
