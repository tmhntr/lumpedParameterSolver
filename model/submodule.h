//
//  submodule.h
//  model
//
//  Created by Tim  Hunter on 2021-08-21.
//

#ifndef submodule_h
#define submodule_h

#include <stdio.h>
#include "interface.h"
#include <math.h>
#include <string>
#include <map>
#include <vector>
#include "wrapper.h"

class submodule: public model {
private:
    int _nP;
    int _nInputs;
    int _nShared;
    int _nAlgebraic;
    int _neq;
        
protected:
    double * P;
    int * inputIndex;
    double ** shared; // algebraic parameters fetched from other functions
    double * algebraic;
            
public:
    // following are names of model inputs, outputs, and working variables. They can be used in model setup, or to reference specific values of the model as the correspond to the values stored.
    std::string * inputNames;
    std::string * sharedNames;
    std::string * algebraicNames;
//    std::string * stateNames;
    
    
    
    submodule(std::string n, int neq, int np, int nIn, int nAlgebraic, int nShared) : model(n)
    {
        setNEQ(neq);
        _nP = np;
        _nInputs = nIn;
        _nAlgebraic = nAlgebraic;
        _nShared = nShared;
        
        
        P = (double *) calloc(_nP, sizeof(double));
        inputIndex = (int *) calloc(_nInputs, sizeof(int));
        algebraic = (double *) calloc(_nAlgebraic, sizeof(double));
        shared = (double **) calloc(_nShared, sizeof(double *));
        
        stateNames = (std::string *) calloc(getNEQ(), sizeof(std::string));
        algebraicNames = (std::string *) calloc(_nAlgebraic, sizeof(std::string));
        
        inputNames = (std::string *) calloc(_nInputs, sizeof(std::string));
        sharedNames = (std::string *) calloc(_nShared, sizeof(std::string));
    }
    
    submodule(std::string n, std::vector<std::string> sharedNames, std::vector<std::string> inputNames, std::vector<std::string> algebraicNames, std::vector<std::string> outputNames, std::vector<double> parameters) : submodule(n, (int) outputNames.size(), (int) parameters.size(), (int) inputNames.size(), (int) algebraicNames.size(), (int) sharedNames.size())
    {
        for (int i = 0; i < sharedNames.size(); i++)
            setSharedName(i, sharedNames[i]);
        for (int i = 0; i < inputNames.size(); i++)
            setInputName(i, inputNames[i]);
        for (int i = 0; i < algebraicNames.size(); i++)
            setAlgebraicName(i, algebraicNames[i]);
        for (int i = 0; i < outputNames.size(); i++)
            setStateName(i, outputNames[i]);
        for (int i = 0; i < parameters.size(); i++)
            setParameter(i, parameters[i]);
    }
    
    
    
    void setParameter(int index, double value)
    {
        
        if (index < _nP)
            P[index] = value;
        else
        {
            std::cout << index << " outside of parameter array range." << std::endl;
            throw (index);
        }
    }
    
    void setII(int index, int value)
    {
        if (index < _nInputs)
            inputIndex[index] = value;
        else
        {
            std::cout << index << " outside of input array range." << std::endl;
            throw (index);
        }
    }
    
    void setAlgebraic(int index, double value)
    {
        if (index < _nInputs)
            algebraic[index] = value;
        else
        {
            std::cout << index << " outside of algebraic array range." << std::endl;
            throw (index);
        }
    }
    
    // this function sets the value of shared[index] to the double pointer sharedPtr. Used for linking
    void setShared(int index, double * sharedPtr)
    {
        if (index < _nShared)
            shared[index] = sharedPtr;
        else
        {
            std::cout << index << " outside of shared array range." << std::endl;
            throw (index);
        }
    }
    
    void setInputName(int index, std::string inputName)
    {
        if (index < _nInputs)
            inputNames[index] = inputName;
        else
        {
            std::cout << index << " outside of inputs array range." << std::endl;
            throw (index);
        }
    }
    
    void setSharedName(int index, std::string sharedName)
    {
        if (index < _nShared)
            sharedNames[index] = sharedName;
        else
        {
            std::cout << index << " outside of shared array range." << std::endl;
            throw (index);
        }
    }
    
    void setAlgebraicName(int index, std::string algName)
    {
        if (index < _nAlgebraic)
            algebraicNames[index] = algName;
        else
        {
            std::cout << index << " outside of algebraic array range." << std::endl;
            throw (index);
        }
    }
    
//    void setStateName(int index, std::string stateName)
//    {
//        if (index < getNEQ())
//            stateNames[index] = stateName;
//        else
//        {
//            std::cout << index << " outside of stateName array range." << std::endl;
//            throw (index);
//        }
//    }
    
    
    
    double getParameter(int index)
    {
        if (index >= _nP)
        {
            std::cout << index << " outside of parameter array range." << std::endl;
            throw (index);
        }
        else
            return P[index];
        
    }
    
    std::string getSharedName(int index)
    {
        if (index >= _nShared)
        {
            std::cout << index << " outside of shared array range." << std::endl;
            throw (index);
        }
        else
            return sharedNames[index];
    }
    
    int getII(int index)
    {
        if (index >= _nInputs)
        {
            std::cout << index << " outside of input array range." << std::endl;
            throw (index);
        } else
            return inputIndex[index];
        
    }
    
    double getSharedVal(int i) { return *(shared[i]); }
    
    
    std::vector<std::string> getInputNameList()
    {
        std::vector<std::string> nameList (inputNames, inputNames + _nInputs);
        return nameList;
    }
    std::vector<std::string> getSharedNameList()
    {
        std::vector<std::string> nameList (sharedNames, sharedNames + _nShared);
        return nameList;
    }
    std::vector<std::string> getAlgebraicNameList()
    {
        std::vector<std::string> nameList (algebraicNames, algebraicNames + _nAlgebraic);
        return nameList;
    }
    std::vector<std::string> getStateNameList()
    {
        std::vector<std::string> nameList (stateNames, stateNames + _neq);
        return nameList;
    }
    
        
    
    double * getAlgebraicPtr(int index)
    {
        if (index >= _nAlgebraic)
        {
            std::cout << index << " outside of algebraic array range." << std::endl;
            throw (index);
        } else
            return (&algebraic[index]);
    }
    
    double * getAlgebraicPtr(std::string query)
    {
        for (int i = 0; i < _nAlgebraic; i++)
        {
            if (algebraicNames[i] == query)
                return getAlgebraicPtr(i);
        }
        return NULL;
    }
    
    bool link(int index, submodule * source, std::string sharedName)
    {
        double * ptr = source->getAlgebraicPtr(sharedName);
        
        if (ptr != NULL)
        {
            setShared(index, ptr);
            return true;
        }
        else
            return false;
    }
    
    bool link(int index, std::vector<submodule *> sources, std::string sharedName)
    {
        std::vector<submodule *>::iterator it = sources.begin() ;
        bool isLinked = link(index, *it, sharedName);
        while (! isLinked && it != sources.end())
        {
            it++;
            isLinked = link(index, *it, sharedName);
        }
        if (! isLinked)
            return false;
        else
            return true;
    }
    
    void assignInputIndices(std::string statevars[], int len)
    {
        bool foundStateVar = false;
        int c = 0;
        
        for (int i = 0; i < _nInputs; i++)
        {
            c = 0;
            foundStateVar = false;
            while (!foundStateVar && c < len)
            {
                
                if (inputNames[i] == statevars[c])
                {
                    foundStateVar = true;
                    setII(i, c);
                }
                c++;
            }
        }
    }
    
};

//class linker {
//private:
//    submodule _source;
//    std::string * parameters;
//public:
//    linker(submodule source, std::string parameter[])
//    {
//        _source = source;
//        int c = 0;
//        while (parameter[c].size())
//        {
//
//        }
//
//    }
//};

#endif /* submodule_h */
