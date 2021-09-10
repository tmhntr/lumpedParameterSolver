#include "component/dialysis.hpp"
#include "component/dialysis.hpp"
#include <iostream>


component * dialyzer()
{
    component * d = new dialysis(
      "dialysis",
      {"P_ao", "P_ao", "P_ao", "P_vc", "P_vc"},
      {"c_ic", "c_is", "F_a", "R_v", "phi_u", "phi_na", "phi_k", "phi_cl", "phi_hco3", "phi_h", "phi_p", "J_u", "J_na", "J_k", "J_cl", "J_hco3", "J_h", "J_p", "R_hco3_ic", "R_h_ic", "R_p_ic", "R_hco3_ex", "R_h_ex", "R_p_ex"},
      {"M_u_ic", "M_na_ic", "M_k_ic", "M_cl_ic", "M_hco3_ic", "M_h_ic", "M_p_ic", "M_u_ex", "M_na_ex", "M_k_ex", "M_cl_ex", "M_hco3_ex", "M_h_ex", "M_p_ex", "V_ic", "V_is", "V_pl"},
      { 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0});
    std::vector<double> y0 = {15.20531, 1.0, 0.2042, 0.232, 0.149, 0, 1.6697, 0.0, 1.2921, 0.5*1.0567, 0.44066, 0.38685, 1.8*5.8495, 0.41389, 1.8*2.3484, 1.249, 0, 0, 0};
    return d;


}
