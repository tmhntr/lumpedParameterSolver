//
//  submodule.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-21.
//

#ifndef submodule_hpp
#define submodule_hpp

#include <stdio.h>
#include "interface.hpp"
#include <math.h>
#include <string>
#include <vector>

class linker;


class submodule: public model {
private:
    int _nP;
    int _nInputs;
    int _nLinks;
    int _nAlgebraic;
    int _neq;
    
    int updateInputIndex(std::vector<std::string> statevars)
    {
        bool foundStateVar = false;
        int missing = 0;
        int c = 0;
        
        for (int i = 0; i < _nInputs; i++)
        {
            c = 0;
            foundStateVar = false;
            while (!foundStateVar && c < statevars.size())
            {
                
                if (inputNames[i] == statevars[c])
                {
                    foundStateVar = true;
                    setInputIndex(i, c);
                }
                c++;
            }
            if (!foundStateVar)
            {
                std::cout << "Error: " << inputNames[i] << " not found in statevars. " << std::endl;
//                throw (1);
                missing++;
            }
        }
        return missing;
    }
    
    int link(std::vector<submodule *> modlist);
    
    double * P;
    int * inputIndex;
    linker ** links;
    double * algebraic;
    
    /*
     Names.
     The following are used so set up the model, properly associating submodules to the required shared and input values
     */
    std::string * inputNames;
    std::string * sharedNames;
    std::string * algebraicNames;
   
public:
    // Constructors and destructors
    submodule(std::string n, int neq, int np, int nIn, int nAlgebraic, int nLinks) : model(n)
    {
        setNEQ(neq);
        _nP = np;
        _nInputs = nIn;
        _nAlgebraic = nAlgebraic;
        _nLinks = nLinks;
        
        
        P = (double *) calloc(_nP, sizeof(double));
        inputIndex = (int *) calloc(_nInputs, sizeof(int));
        algebraic = (double *) calloc(_nAlgebraic, sizeof(double));
//        shared = (double **) calloc(_nShared, sizeof(double *));
        links = (linker **) calloc(_nLinks, sizeof(linker *));
        
        stateNames.resize(getNEQ());
        algebraicNames = (std::string *) calloc(_nAlgebraic, sizeof(std::string));
        
        inputNames = (std::string *) calloc(_nInputs, sizeof(std::string));
        sharedNames = (std::string *) calloc(_nLinks, sizeof(std::string));
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
            setP(i, parameters[i]);
    }
    virtual ~submodule(){}
    
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
    
//  Input Index Array
//      for referencing the y state vector
    void setInputIndex(int index, int value)
    {
        if (index < _nInputs)
            inputIndex[index] = value;
        else
        {
            std::cout << index << " outside of input array range." << std::endl;
            throw (1);
        }
    }
    void setInputIndex(std::vector<int> II_vec)
    {
        if (II_vec.size() == _nInputs)
            for (int i = 0; i < II_vec.size(); i++) inputIndex[i] = II_vec[i];
        else
        {
            std::cout << "Input vector is " << II_vec.size() << ", nInputs is " << _nInputs <<". Wrong size." << std::endl;
            throw (1);
        }
    }
    int getInputIndex(int index)
    {
        if (index >= _nInputs)
        {
            std::cout << index << " outside of input array range." << std::endl;
            throw (1);
        } else
            return inputIndex[index];
        
    }
    std::vector<int> getInputIndexVec() { return std::vector<int> (inputIndex, inputIndex+_nInputs); }

    
//  Linker array
//    For referencing algebraic variables from other submodules
//    shared(int) is also included to call the get function of the linker.
    void setLink(int index, linker * l)
    {
        if (index < _nLinks)
            links[index] = l;
        else
        {
            std::cout << index << " outside of link array range." << std::endl;
            throw (1);
        }
    }
    void setLink(std::vector<linker *> Link_vec)
    {
        if (Link_vec.size() == _nLinks)
            for (int i = 0; i < Link_vec.size(); i++) links[i] = Link_vec[i];
        else
        {
            std::cout << "Input vector is " << Link_vec.size() << ", nLinks is " << _nLinks <<". Wrong size." << std::endl;
            throw (1);
        }
    }
    linker * getLink(int index)
    {
        if (index >= _nLinks)
        {
            std::cout << index << " outside of links array range." << std::endl;
            throw (1);
        } else
            return links[index];
        
    }
    std::vector<linker *> getLinkVec() { return std::vector<linker *> (links, links+_nLinks); }
    
    double shared(int index); // this is implemented after the definition of the linker class
    

//  Algebraic variable array
//    This stores all calculated values that are used in the function DYDT
    void setAlgebraic(int index, double value)
    {
        if (index < _nAlgebraic)
            algebraic[index] = value;
        else
        {
            std::cout << index << " outside of algebraic array range." << std::endl;
            throw (1);
        }
    }
    void setAlgebraic(std::vector<double> A_vec)
    {
        if (A_vec.size() == _nAlgebraic)
            for (int i = 0; i < A_vec.size(); i++) algebraic[i] = A_vec[i];
        else
        {
            std::cout << "Input vector is " << A_vec.size() << ", nAlgebraic is " << _nAlgebraic <<". Wrong size." << std::endl;
            throw (1);
        }
    }
    double getAlgebraic(int index)
    {
        if (index >= _nAlgebraic)
        {
            std::cout << index << " outside of algebraic array range." << std::endl;
            throw (1);
        }
        else
            return algebraic[index];
    }
    std::vector<double> getAlgebraicVec() { return std::vector<double> (algebraic, algebraic+_nAlgebraic); }
    
    
    
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
    
    
    void setSharedName(int index, std::string name)
    {
        if (index < _nLinks)
            sharedNames[index] = name;
        else
        {
            std::cout << index << " outside of links array range." << std::endl;
            throw (1);
        }
    }
    void setSharedName(std::vector<std::string> Name_vec)
    {
        if (Name_vec.size() == _nLinks)
            for (int i = 0; i < Name_vec.size(); i++) sharedNames[i] = Name_vec[i];
        else
        {
            std::cout << "Input vector is " << Name_vec.size() << ", nLinks is " << _nLinks <<". Wrong size." << std::endl;
            throw (1);
        }
    }
    std::string getSharedName(int index)
    {
        if (index >= _nLinks)
        {
            std::cout << index << " outside of link array range." << std::endl;
            throw (1);
        }
        else
            return sharedNames[index];
    }
    std::vector<std::string> getSharedNameVec() { return std::vector<std::string> (sharedNames, sharedNames+_nLinks); }
    
    void setAlgebraicName(int index, std::string name)
    {
        if (index < _nAlgebraic)
            algebraicNames[index] = name;
        else
        {
            std::cout << index << " outside of algebraic array range." << std::endl;
            throw (1);
        }
    }
    void setAlgebraicName(std::vector<std::string> Name_vec)
    {
        if (Name_vec.size() == _nAlgebraic)
            for (int i = 0; i < Name_vec.size(); i++) algebraicNames[i] = Name_vec[i];
        else
        {
            std::cout << "Input vector is " << Name_vec.size() << ", nParameters is " << _nAlgebraic <<". Wrong size." << std::endl;
            throw (1);
        }
    }
    std::string getAlgebraicName(int index)
    {
        if (index >= _nAlgebraic)
        {
            std::cout << index << " outside of parameter array range." << std::endl;
            throw (1);
        }
        else
            return algebraicNames[index];
    }
    std::vector<std::string> getAlgebraicNameVec() { return std::vector<std::string> (algebraicNames, algebraicNames+_nAlgebraic); }
    
    
    int getAlgebraicIndex(std::string query)
    {
        int index = -1;
        for (int i = 0; i < _nAlgebraic; i++)
        {
            if (algebraicNames[i] == query)
                index = i;
        }
        if (index != -1)
        {
            return index;
        } else
            throw 1;
    }
    
    int init(std::vector<submodule *> modlist, std::vector<std::string> statevars)
    {
        int retval, inputVal, linkerVal;
        
        inputVal = updateInputIndex(statevars);
        linkerVal = link(modlist);
        
        retval = inputVal + linkerVal;
        
        if (inputVal != 0)
        {
            std::cout << "Input configuration failed for " << inputVal << " value(s)." << std::endl;
        }
        if (linkerVal != 0)
        {
            std::cout << "Linker configuration failed for " << linkerVal << " value(s)." << std::endl;
        }
        
        return retval;
    }
    
};

class linker
{
private:
    int _index;
    submodule * _source;
    
public:
//    linker() {}
    linker(std::string query, std::vector<submodule *> modlist)
    {
        int index;
        for (int i = 0; i < modlist.size(); i++)
        {
            try {
                index = modlist[i]->getAlgebraicIndex(query);
                _index = index;
                _source = modlist[i];
                return;
            } catch (int e) { }
        }
        std::cout << "Link for " << query << " failed." << std::endl;
        throw 1;
    }
    double get() { return _source->getAlgebraic(_index); }
    void setIndex(int index) { _index = index; }
    int getIndex() { return _index; }
    void setSource(submodule * source) { _source = source; }
    submodule * getSource() { return _source; }
};




#endif /* submodule_hpp */
