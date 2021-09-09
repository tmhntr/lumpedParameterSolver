#include "wrapper.hpp"

wrapper::wrapper() : model("wrapper") {}
wrapper::wrapper(std::vector<model *> mdls) : wrapper()
{
    for (std::vector<model *>::iterator it = mdls.begin() ; it != mdls.end(); ++it)
    {
        addModel(*it);
    }
}

double wrapper::getDerived(int index)
{
  std::vector<model *> models = components();
  std::vector<model *>::iterator itModel = models.begin();
  try
  {
    while (index >= (*itModel)->getNEQ())
    {
      index = index - (*itModel)->getNEQ();
      itModel++;
    }
    return (*itModel)->getDerived(index);
  } catch (...) {
    std::cout << "Something went wrong in wrapper::getDerived" << std::endl;
    return NULL;
  }
}

std::string wrapper::getDerivedName(int index)
{
  std::vector<model *> models = components();
  std::vector<model *>::iterator itModel = models.begin();
  try
  {
    while (index >= (*itModel)->getNEQ())
    {
      index = index - (*itModel)->getNEQ();
      itModel++;
    }
    return (*itModel)->getDerivedName(index);
  } catch (...) {
    std::cout << "Something went wrong in wrapper::getDerivedName" << std::endl;
    return "";
  }

}

void wrapper::addModel(model * mdl)
{
    if(getY()!=NULL)
    {
      mdl->setY(getY());
    }
    models.push_back(mdl);
    int oldNeq = getNEQ();
    setNEQ(oldNeq + mdl->getNEQ());
    stateNames.resize(getNEQ());
    for (int i = 0; i < mdl->getNEQ(); i++)
    {
        setStateName(i+oldNeq, mdl->getStateName(i));
    }
}

model * wrapper::getModel(int index) { return models[index]; }
model * wrapper::getModel(std::string modelName)
{
    std::vector<model *>::iterator it = models.begin();
    while (it != models.end())
    {
        if (modelName == (*it)->getName())
            return (*it);
    }
    return NULL;
}
std::vector<model *> wrapper::getModelVec() { return models; }

std::string * wrapper::getStateNames()
{
    return stateNames.data();
}

void wrapper::updateDerived(double t, double y[])
{
    for (int i = 0; i <  models.size(); i++)
    {
        models[i]->updateDerived(t, y);
    }
}

void wrapper::getDY(double t, double y[], double * DY)
{
    model * mdlPtr;
    double * DYPtr = DY;
    for (std::vector<model *>::iterator it = models.begin() ; it != models.end(); ++it)
    {
        mdlPtr = *it;
//            std::cout << mdlPtr->getName() << std::endl;
        mdlPtr->getDY(t, y, DYPtr);
        DYPtr+=mdlPtr->getNEQ();
    }
}

int init(model * parent)
{
    int retval = 0;
//        std::vector<component *> flatmodlist = flattenModList(modlist);
    std::vector<model *> modlist = components();
    for (std::vector<model *>::iterator it = modlist.begin() ; it != modlist.end(); it++)
    {
        try {
            if ((*it)->init(parent) != 0)
                throw 1;
        } catch (int e) {
            std::cout << "Initialization for model (" << (*it)->getName() << ") failed." << std::endl;
            retval++;
        }
    }
    return retval;
}

std::vector<component *> flattenModList(std::vector<model *> modlist)
{
    std::vector<component *> newModList;
    wrapper * wrap;
    for (int i = 0; i < modlist.size(); i++)
    {

        if (typeid(wrapper) == typeid(*(modlist[i])))
        {
            wrap = (wrapper *) modlist[i];
            std::vector<component *> newNewModList = flattenModList(wrap->getModelVec());
            for (int j = 0; j  < newNewModList.size(); j++)
            {
                newModList.push_back(newNewModList[j]);
            }
        } else
        {
            newModList.push_back((component *) modlist[i]);
        }
    }
    return newModList;
}

std::vector<component *> flattenModList() {return flattenModList(models); }

std::vector<model *> components()
{
    std::vector<model *> mlist;
    std::vector<component *> clist = flattenModList(models);
    for (int i = 0; i < clist.size(); i++) {
        mlist.push_back((model *) clist[i]);
    }
    return mlist;
}
