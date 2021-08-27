//
//  submodule.cpp
//  model
//
//  Created by Tim  Hunter on 2021-08-27.
//

#include <stdio.h>
#include "submodule.hpp"

double submodule::shared(int index) { return links[index]->get(); }

int submodule::link(std::vector<submodule *> modlist)
{
    int failed = 0;
    for (int i = 0; i < _nLinks; i++)
    {
        try {
            links[i] = new linker(sharedNames[i], modlist);
        } catch (int e) {
            failed ++;
        }
    }
    if (failed != 0)
        std::cout << failed << " links failed." << std::endl;
    return failed;
}
