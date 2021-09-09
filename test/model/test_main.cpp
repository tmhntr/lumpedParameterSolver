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

int componenttest(int testnum, model * mdl)
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

int main(int argc, const char * argv[]) {
    int failed = 0;
    int nModelTests = 2;

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
            cout << "Test " << i <<" passed" << endl;
        } else {
            cout << "Test " << i <<" failed" << endl;
            failed++;
        }
    }

    component * rat2 = new vpr_circulation(
      "Rat2",
      {"A", "C_ach", "eps_1", "eps_2", "eps_3", "Q_av", "Q_mt", "Q_pv", "Q_tc", "V_ao", "V_lv", "V_pa", "V_pu", "V_rv", "V_vc", "c_nor", "delta_HR_pslow", "delta_HR_s", "Psi", "P_ao", "Tp", "C1", "C2", "C3", "C4", "C5", "C6", "Ts", "delta_HR_ps", "delta_HR_ss", "HR", "e_t", "V_spt", "E_es_spt", "E_es_lvf", "E_es_rvf", "P_lv", "P_pu", "P_rv", "P_pa", "P_vc", "Q_sys", "Q_pul"},
      {"P_ao", "Tp", "C1", "C2", "C3", "C4", "C5", "C6", "Ts", "delta_HR_ps", "delta_HR_ss", "HR", "e_t", "V_spt", "E_es_spt", "E_es_lvf", "E_es_rvf", "P_lv", "P_pu", "P_rv", "P_pa", "P_vc", "Q_sys", "Q_pul"},
      {"A", "C_ach", "eps_1", "eps_2", "eps_3", "Q_av", "Q_mt", "Q_pv", "Q_tc", "V_ao", "V_lv", "V_pa", "V_pu", "V_rv", "V_vc", "c_nor", "delta_HR_pslow", "delta_HR_s", "Psi"},
      {}
      ); // NAME, INPUTS, DERIVED, STATES, PARAMETERS

    return failed;

}
