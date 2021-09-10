#include "component/myoRC.hpp"
#include "component/myogenic.hpp"

// Returns a full myogenic compartment
// not currently working


component * myocompartment()
{
  component * wrapper = {};


      vector<double> P_myo, P_flow;
  //
  //    string organName = "organ";
  //    string organSymbol = "o";

      vector<string> myoShared, myoInputStates, myoAlgebraic, myoStates;
      vector<string> flowInputStates, flowAlgebraic, flowStates;


      // All parameters are listed

      double C_symp, C_tone_primeprime, C_ATP_0, C_rbc_meta, tau_d, tau_a, E;
      double C_pass[] = {1154, 834.9, 636.0, 419.8, 275.6};
      double C_pass_prime[] = {7.846, 9.138, 9.944, 10.82, 11.40};
      double D_0[] = {173.1, 125.3, 95.41, 62.98, 41.35};
      double C_act[] = {2671, 1654, 1106, 598.1, 320.8};
      double C_act_prime[] = {0.6709, 0.6991, 0.7167, 0.7358, 0.7486};
      double C_act_primeprime[] = {0.2765, 0.3148, 0.3387, 0.3646, 0.3819};
      double C_myo[] = {0.0079, 0.0109, 0.0143, 0.0217, 0.0331};
      double C_shear[] = {0.0258, 0.0258, 0.0258, 0.0258, 0.0258};
      double mu[] = {2.09, 2.12, 2.43, 3.41, 5.16};
      double nParallel[] = {1, 4.33, 14.62, 93.06, 585.3};
      double length[] = {0.5453, 0.2242, 0.1563, 0.0945, 0.0588};
      C_symp = 3.0;
      C_tone_primeprime = 0;
      C_ATP_0 = 0.5;
      C_rbc_meta = 30.0;
      tau_d = 1.0;
      tau_a = 20.0;
      E = 1500000;


      // Assign P for myogenic module
      for (double * arr : {C_pass, C_pass_prime, D_0, C_act, C_act_prime, C_act_primeprime, C_myo, C_shear, mu, nParallel}) {
          for (int i = 0; i < nSegs; i++) {
              P_myo.push_back(arr[i]);
          }
      }

      for (double val : {C_symp, C_tone_primeprime, C_ATP_0, C_rbc_meta, tau_d, tau_a}){
          P_myo.push_back(val);
      }


      for (string s : {"Q"})
          myoShared.push_back(s + "_" + organSymbol);
      myoShared = expand(myoShared, nSegs);

      for (string s : {"D", "A", "P"})
          myoInputStates.push_back(s + "_" + organSymbol);
      myoInputStates = expand(myoInputStates, nSegs);

      for (string s : {"T_total", "A_total", "T"})
          myoAlgebraic.push_back(s + "_" + organSymbol);
      myoAlgebraic = expand(myoAlgebraic, nSegs);

      for (string s : {"D", "A"})
          myoStates.push_back(s + "_" + organSymbol);
      myoStates = expand(myoStates, nSegs);


     myogenic * myoOrgan = new myogenic(organName,myoInputStates, myoAlgebraic, myoStates, P_myo);
     myoOrgan->setSegNum(nSegs);


      for (string s : {"P", "D"})
          flowInputStates.push_back(s + "_" + organSymbol);
      flowInputStates = expand(flowInputStates, nSegs);

      flowInputStates.insert(flowInputStates.begin(), inletPressure);

      flowAlgebraic = expand({"Q_" + organSymbol}, nSegs);

      flowStates = expand({"P_" + organSymbol}, nSegs);

      for (double * arr : {length, nParallel, mu}) {
          for (int i = 0; i < nSegs; i++) {
              P_flow.push_back(arr[i]);
          }
      }
      P_flow.push_back(E);

     myoRC * myoFlow = new myoRC(organName+"Flow", flowInputStates, flowAlgebraic, flowStates, P_flow);
     myoFlow->setSegNum(nSegs);



  //    wrapper * mdl = new wrapper(std::vector<model *> {myoOrgan, myoFlow});
      wrapper * mdl = new wrapper();

  return wrapper;
}
