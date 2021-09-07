//
//  submodule.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-27.
//

#include <stdio.h>
#include "submodule.hpp"
#include <type_traits>

double submodule::shared(int index) { return links[index]->get(); }

int submodule::link(std::vector<model *> modlist)
{
    std::vector<submodule *> flatModList = flattenModList(modlist);
    int failed = 0;
    for (int i = 0; i < _nLinks; i++)
    {
        
        try {
            links[i] = new linker(sharedNames[i], flatModList);
        } catch (int e) {
            failed ++;
        }
    }
    if (failed != 0)
        std::cout << failed << " links failed." << std::endl;
    return failed;
}



std::vector<submodule *> submodule::flattenModList(std::vector<model *> modlist)
{
    std::vector<submodule *> newModList;
    wrapper * wrap;
    for (int i = 0; i < modlist.size(); i++)
    {
        
        if (typeid(wrapper) == typeid(*(modlist[i])))
        {
            wrap = (wrapper *) modlist[i];
            std::vector<submodule *> newNewModList = flattenModList(wrap->getModelVec());
            for (int j = 0; j  < newNewModList.size(); j++)
            {
                newModList.push_back((submodule *) newNewModList[j]);
            }
        } else
        {
            newModList.push_back((submodule *) modlist[i]);

        }
    }
    return newModList;
}



