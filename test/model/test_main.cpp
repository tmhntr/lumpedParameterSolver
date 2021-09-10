#include <iostream>
// #include "interface.hpp"
#include "model/RCR.hpp"
#include "model/heart.hpp"
#include "model/wrapper.hpp"
#include "solver/sdsolver.hpp"
#include "printer/basicprinter.hpp"
#include "printer/betterprinter.hpp"
#include "model/myoRC.hpp"
#include "model/myogenic.hpp"
#include "model/vpr_circulation.hpp"
#include <string>

using namespace std;

int modeltest(int testnum, model * mdl)
{
  if (testnum == 1)
  {
      double Y_1[] = {1.0, 2.0, 3.0};
      mdl->setY(Y_1);

      double * Y_2 = mdl->getY();

      if (Y_2[1] == Y_1[1])
        return 0;
      else
        return 1;
  } else if(testnum == 2){
      mdl->setStateName(1, "C_ach-modified");

      string statename1 = mdl->getStateName(0);
      string statename2 = mdl->getStateName(1);

      if (statename1 == "A" && statename2 == "C_ach-modified")
        return 0;
      else
        return 1;
  }

  return 2;
}

int comptest(int testnum, component * mdl)
{
  if (testnum == 1)
  {
      return mdl->init(mdl);
  } else if(testnum == 2){
      mdl->setP(2, 4.4);
      if (mdl->getP(2) == 4.4)
          return 0;
      else
        return 1;
  } else if(testnum == 3) {
      std::vector<double> Param = {5.5, 5.5, 5.5};
      mdl->setP(Param);
      if (mdl->getPVec() == Param)
          return 0;
      else
        return 1;
  } else if(testnum == 4) {
      double y[] = {15.20531, 1.0, 0.2042, 0.232, 0.149, 0, 1.6697, 0.0, 1.2921, 0.5*1.0567, 0.44066, 0.38685, 1.8*5.8495, 0.41389, 1.8*2.3484, 1.249, 0, 0, 0};
      mdl->setY(y);
      if (y[0] == mdl->input(0))
          return 0;
      else
        return 1;
  }

  return 2;
}

int main(int argc, const char * argv[]) {
    int failed = 0;
    int nModelTests = 2;
    int nCompTests = 4;

    // tests will be run on a rat as usual
    model * rat = new vpr_circulation(
      "Rat",
      {"A", "C_ach", "eps_1", "eps_2", "eps_3", "Q_av", "Q_mt", "Q_pv", "Q_tc", "V_ao", "V_lv", "V_pa", "V_pu", "V_rv", "V_vc", "c_nor", "delta_HR_pslow", "delta_HR_s", "Psi", "P_ao", "Tp", "C1", "C2", "C3", "C4", "C5", "C6", "Ts", "delta_HR_ps", "delta_HR_ss", "HR", "e_t", "V_spt", "E_es_spt", "E_es_lvf", "E_es_rvf", "P_lv", "P_pu", "P_rv", "P_pa", "P_vc", "Q_sys", "Q_pul"},
      {"P_ao", "Tp", "C1", "C2", "C3", "C4", "C5", "C6", "Ts", "delta_HR_ps", "delta_HR_ss", "HR", "e_t", "V_spt", "E_es_spt", "E_es_lvf", "E_es_rvf", "P_lv", "P_pu", "P_rv", "P_pa", "P_vc", "Q_sys", "Q_pul"},
      {"A", "C_ach", "eps_1", "eps_2", "eps_3", "Q_av", "Q_mt", "Q_pv", "Q_tc", "V_ao", "V_lv", "V_pa", "V_pu", "V_rv", "V_vc", "c_nor", "delta_HR_pslow", "delta_HR_s", "Psi"},
      {}
      ); // NAME, INPUTS, DERIVED, STATES, PARAMETERS


    // first test all methods defined iin model
    for (int i = 1; i <= nModelTests; i++)
    {
        if (modeltest(i, rat) == 0) // tests set y
        {
            cout << "Model test " << i <<" passed" << endl;
        } else {
            cout << "Model test " << i <<" failed" << endl;
            failed++;
        }
    }

    component * rat2 = new vpr_circulation(
      "Rat2",
      {"A", "C_ach", "eps_1", "eps_2", "eps_3", "Q_av", "Q_mt", "Q_pv", "Q_tc", "V_ao", "V_lv", "V_pa", "V_pu", "V_rv", "V_vc", "c_nor", "delta_HR_pslow", "delta_HR_s", "Psi", "P_ao", "Tp", "C1", "C2", "C3", "C4", "C5", "C6", "Ts", "delta_HR_ps", "delta_HR_ss", "HR", "e_t", "V_spt", "E_es_spt", "E_es_lvf", "E_es_rvf", "P_lv", "P_pu", "P_rv", "P_pa", "P_vc", "Q_sys", "Q_pul"},
      {"P_ao", "Tp", "C1", "C2", "C3", "C4", "C5", "C6", "Ts", "delta_HR_ps", "delta_HR_ss", "HR", "e_t", "V_spt", "E_es_spt", "E_es_lvf", "E_es_rvf", "P_lv", "P_pu", "P_rv", "P_pa", "P_vc", "Q_sys", "Q_pul"},
      {"A", "C_ach", "eps_1", "eps_2", "eps_3", "Q_av", "Q_mt", "Q_pv", "Q_tc", "V_ao", "V_lv", "V_pa", "V_pu", "V_rv", "V_vc", "c_nor", "delta_HR_pslow", "delta_HR_s", "Psi"},
      {1.1, 2.2, 3.3}
      ); // NAME, INPUTS, DERIVED, STATES, PARAMETERS

      for (int i = 1; i <= nCompTests; i++)
      {
          if (comptest(i, rat2) == 0) // tests set y
          {
              cout << "Component test " << i <<" passed" << endl;
          } else {
              cout << "Component test " << i <<" failed" << endl;
              failed++;
          }
      }
    return failed;

}
