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


//int component::link(std::vector<component *> modlist)
//{
//
////    std::vector<submodule *> flatModList = flattenModList(modlist);
//    int failed = 0;
//    for (int i = 0; i < _nLinks; i++)
//    {
//
//        try {
//            links[i] = new linker(sharedNames[i], modlist);
//        } catch (int e) {
//            failed ++;
//        }
//    }
//    if (failed != 0)
//        std::cout << failed << " links failed." << std::endl;
//    return failed;
//}

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



//std::vector<submodule *> submodule::flattenModList(std::vector<model *> modlist)
//{
//    std::vector<submodule *> newModList;
//    wrapper * wrap;
//    for (int i = 0; i < modlist.size(); i++)
//    {
//
//        if (typeid(wrapper) == typeid(*(modlist[i])))
//        {
//            wrap = (wrapper *) modlist[i];
//            std::vector<submodule *> newNewModList = flattenModList(wrap->getModelVec());
//            for (int j = 0; j  < newNewModList.size(); j++)
//            {
//                newModList.push_back((submodule *) newNewModList[j]);
//            }
//        } else
//        {
//            newModList.push_back((submodule *) modlist[i]);
//
//        }
//    }
//    return newModList;
//}
