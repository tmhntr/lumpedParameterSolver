#include "component/dialysis.hpp"
#include "component/dialysis.hpp"
#include <iostream>


component * dialyzer()
{
    dialysis * d = new dialysis(
      "dialysis",
      {"P_ao", "P_ao", "P_ao", "P_ao", "P_ao"},
      {"c_ic", "c_is", "F_a", "R_v", "phi_u", "phi_na", "phi_k", "phi_cl", "phi_hco3", "phi_h", "phi_p", "J_u", "J_na", "J_k", "J_cl", "J_hco3", "J_h", "J_p", "R_hco3_ic", "R_h_ic", "R_p_ic", "R_hco3_ex", "R_h_ex", "R_p_ex"},
      {"M_u_ic", "M_na_ic", "M_k_ic", "M_cl_ic", "M_hco3_ic", "M_h_ic", "M_p_ic", "M_u_ex", "M_na_ex", "M_k_ex", "M_cl_ex", "M_hco3_ex", "M_h_ex", "M_p_ex", "V_ic", "V_is", "V_pl"},
      {   150.0, 150.0, 5.0, 0.2, 3.0,
          0.0, 142.0, 2.0, 0.0, 35.0,
          0.0, 0.0, 0.0, 0.0, 2.67,
          2.67,  0.13/60.0, 0.94, 0.72, 1.0,
          1.0, 0.0, 0.0, 0.0, 0.0,
          25.0, 0.0704, 0.0667, 28.2, 13.0,
          1.0, 0.004, 2.45, 11.0, 3.25,
          7.4, 1.37, 1.05, 0.95, 0.2/60.0,
          0.03/60.0, 0.4, 3.5, 6.0/60.0, pow(10.0,(-6.1)),
          6.0/60.0, pow(10.0,(-7.4)), 1.2, 1.2, 4.0,
          -5.97, 0.01, 0.062});

    std::vector<double> y0 = {100.0, 250.0, 3535.0, 84.0, 10.0, 100.0, 0.0, 55.0, 2130.0, 75.0, 1470.0, 100.0, 100.0, 0.0};
    return d;


}
