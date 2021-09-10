#include "component_wrapper.hpp"

component_wrapper::component_wrapper() : component("wrapper") {}
component_wrapper::component_wrapper(std::vector<component *> mdls) : component_wrapper()
{
    for (std::vector<component *>::iterator it = mdls.begin() ; it != mdls.end(); ++it)
    {
        addModel(*it);
    }
}

double component_wrapper::getDerived(int index)
{
  std::vector<component *> models = components();
  std::vector<component *>::iterator itModel = models.begin();
  try
  {
    while (index >= (*itModel)->getNDerived())
    {
      index = index - (*itModel)->getNDerived();
      itModel++;
    }
    return (*itModel)->getDerived(index);
  } catch (...) {
    std::cout << "Something went wrong in component_wrapper::getDerived" << std::endl;
    throw;
  }
}

std::string component_wrapper::getDerivedName(int index)
{
  std::vector<component *> models = components();
  std::vector<component *>::iterator itModel = models.begin();
  try
  {
    while (index >= (*itModel)->getNDerived())
    {
      index = index - (*itModel)->getNDerived();
      itModel++;
    }
    return (*itModel)->getDerivedName(index);
  } catch (...) {
    std::cout << "Something went wrong in component_wrapper::getDerivedName" << std::endl;
    return "";
  }
}

std::vector<std::string> component_wrapper::getDerivedNameVec()
{
    std::vector<std::string> namevec;
    for (int i = 0; i < getNDerived(); i++)
    {
        namevec.push_back(getDerivedName(i));
    }
    return namevec;
}


void component_wrapper::addModel(component * mdl)
{
    if(getY()!=NULL)
    {
      mdl->setY(getY());
    }
    models.push_back(mdl);
    int oldNeq = getNEQ();
    setNEQ(oldNeq + mdl->getNEQ());
    _nDerived += mdl->getNDerived();
    _stateNames.resize(getNEQ());
    for (int i = 0; i < mdl->getNEQ(); i++)
    {
        setStateName(i+oldNeq, mdl->getStateName(i));
    }
}

component * component_wrapper::getModel(int index) { return models[index]; }
component * component_wrapper::getModel(std::string modelName)
{
    std::vector<component *>::iterator it = models.begin();
    while (it != models.end())
    {
        if (modelName == (*it)->getName())
            return (*it);
    }
    return NULL;
}
std::vector<component *> component_wrapper::getModelVec() { return models; }

std::string * component_wrapper::getStateNames()
{
    return _stateNames.data();
}

void component_wrapper::setY(double * y)
{
  // std::vector<model *> mdls = getModelVec();
  for (std::vector<component *>::iterator it = models.begin(); it != models.end(); it++)
  {
      (*it)->setY(y);
  }
}

void component_wrapper::updateDerived(double t, double y[])
{
    for (int i = 0; i <  models.size(); i++)
    {
        models[i]->updateDerived(t, y);
    }
}

void component_wrapper::getDY(double t, double y[], double * DY)
{
    component * mdlPtr;
    double * DYPtr = DY;
    for (std::vector<component *>::iterator it = models.begin() ; it != models.end(); ++it)
    {
        mdlPtr = *it;
//            std::cout << mdlPtr->getName() << std::endl;
        mdlPtr->getDY(t, y, DYPtr);
        DYPtr+=mdlPtr->getNEQ();
    }
}

int component_wrapper::init(component * parent)
{
    int retval = 0;
//        std::vector<component_model *> flatmodlist = flattenModList(modlist);
    std::vector<component *> modlist = components();
    for (std::vector<component *>::iterator it = modlist.begin() ; it != modlist.end(); it++)
    {
        try {
            if ((*it)->init(parent) != 0)
                throw 1;
        } catch (int e) {
            std::cout << "Initialization for component (" << (*it)->getName() << ") failed." << std::endl;
            retval++;
        }
    }
    return retval;
}

std::vector<component_model *> component_wrapper::flattenModList(std::vector<component *> modlist)
{
    std::vector<component_model *> newModList;
    component_wrapper * wrap;
    for (int i = 0; i < modlist.size(); i++)
    {

        if (typeid(component_wrapper) == typeid(*(modlist[i])))
        {
            wrap = (component_wrapper *) modlist[i];
            std::vector<component_model *> newNewModList = flattenModList(wrap->getModelVec());
            for (int j = 0; j  < newNewModList.size(); j++)
            {
                newModList.push_back(newNewModList[j]);
            }
        } else
        {
            newModList.push_back((component_model *) modlist[i]);
        }
    }
    return newModList;
}

std::vector<component_model *> component_wrapper::flattenModList() {return flattenModList(models); }

std::vector<component *> component_wrapper::components()
{
    std::vector<component *> mlist;
    std::vector<component_model *> clist = flattenModList(models);
    for (int i = 0; i < clist.size(); i++) {
        mlist.push_back((component *) clist[i]);
    }
    return mlist;
}
