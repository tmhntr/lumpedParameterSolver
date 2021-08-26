//
//  sdsolver.hpp
//  model
//
//  Created by Tim  Hunter on 2021-08-26.
//

#ifndef sdsolver_hpp
#define sdsolver_hpp

#include <stdio.h>
#include "interface.hpp"

#include <stdlib.h>
//#include <json-c/json.h>
#include <string.h>
#include <math.h>
// #include <malloc.h>
#include <time.h>
#include <cvode/cvode.h>               /* prototypes for CVODE fcts., consts.  */
#include <nvector/nvector_serial.h>    /* access to serial N_Vector            */
#include <sunmatrix/sunmatrix_dense.h> /* access to dense SUNMatrix            */
#include <sunlinsol/sunlinsol_dense.h> /* access to dense SUNLinearSolver      */
#include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype      */

#if defined(SUNDIALS_EXTENDED_PRECISION)
#define GSYM "Lg"
#define ESYM "Le"
#define FSYM "Lf"
#else
#define GSYM "g"
#define ESYM "e"
#define FSYM "f"
#endif

/* These macros are defined in order to write code which exactly matches
   the mathematical problem description given above.
   Ith(v,i) references the ith component of the vector v, where i is in
   the range [1..NEQ] and NEQ is defined below. The Ith macro is defined
   using the N_VIth macro in nvector.h. N_VIth numbers the components of
   a vector starting from 0.
   IJth(A,i,j) references the (i,j)th element of the dense matrix A, where
   i and j are in the range [1..NEQ]. The IJth macro is defined using the
   SM_ELEMENT_D macro in dense.h. SM_ELEMENT_D numbers rows and columns of
   a dense matrix starting from 0. */

#define Ith(v,i)    NV_Ith_S(v,i-1)         /* Ith numbers components 1..NEQ */
#define IJth(A,i,j) SM_ELEMENT_D(A,i-1,j-1) /* IJth numbers rows,cols 1..NEQ */


/* Problem Constants */

// #define NEQ   3                /* number of equations  */
#define Y1    RCONST(1.0)      /* initial y components */
#define Y2    RCONST(0.0)
#define Y3    RCONST(0.0)
// #define RTOL  RCONST(1.0e-4)   /* scalar relative tolerance            */
// #define ATOL1 RCONST(1.0e-8)   /* vector absolute tolerance components */
// #define ATOL2 RCONST(1.0e-14)
// #define ATOL3 RCONST(1.0e-6)
#define T0    RCONST(0.0)      /* initial time           */
#define T1    RCONST(0.4)      /* first output time      */
#define TMULT RCONST(1.0)     /* output time factor     */
#define NOUT  20               /* number of output times */

#define ZERO  RCONST(0.0)

class sdsolver: public solver {
private:
//    double * DY;
//    double * y;
    double _deltat = 0.01;
    
    realtype reltol, t;
    N_Vector y, abstol;
    SUNMatrix A;
    SUNLinearSolver LS;
    void *cvode_mem;
    int retval, retvalr, iout;
    int rootsfound[2];
    
    static int check_retval(void *returnvalue, const char *funcname, int opt)
    {
      int *retval;

      /* Check if SUNDIALS function returned NULL pointer - no memory allocated */
      if (opt == 0 && returnvalue == NULL) {
        fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed - returned NULL pointer\n\n",
            funcname);
        return(1); }

      /* Check if retval < 0 */
      else if (opt == 1) {
        retval = (int *) returnvalue;
        if (*retval < 0) {
          fprintf(stderr, "\nSUNDIALS_ERROR: %s() failed with retval = %d\n\n",
              funcname, *retval);
          return(1); }}

      /* Check if function returned NULL pointer - no memory allocated */
      else if (opt == 2 && returnvalue == NULL) {
        fprintf(stderr, "\nMEMORY_ERROR: %s() failed - returned NULL pointer\n\n",
            funcname);
        return(1); }

      return(0);
    }
public:
//    sdsolver() : solver() {}
    sdsolver(model * m) : solver(m) { }
    
    void setDeltaT(double deltat) { _deltat = deltat; }
    double getDeltaT() { return _deltat; }
    
    double getY(int index) { return Ith(y,index+1); }
    double getT() { return (double) t;}
    
    sdsolver(model * m, double INITIAL_CONDITIONS[], double reltol1, double abstol1[]) : solver(m)
    {
//        realtype reltol, t, tout;
//        N_Vector y, abstol;
//        SUNMatrix A;
//        SUNLinearSolver LS;
//        void *cvode_mem;
//        int retval, retvalr, iout;
//        int rootsfound[2];

//        realtype DELTAT = mdl.cfg->deltat;

        y = abstol = NULL;
        A = NULL;
        LS = NULL;
        cvode_mem = NULL;

        // #define NEQ   temp
        // printf("NEQ: %d\n", NEQ);
        int NEQ = getModel()->getNEQ();

        /* Create serial vector of length NEQ for I.C. and abstol */
        y = N_VNew_Serial(NEQ);
        if (check_retval((void *)y, "N_VNew_Serial", 0)) throw(1);
        abstol = N_VNew_Serial(NEQ);
        if (check_retval((void *)abstol, "N_VNew_Serial", 0)) throw(2);

        /* Initialize y */
        for(int i=0;i<NEQ;i++) Ith(y,i+1) = 1.0;

        for(int i=0;i<NEQ;i++)
        {
          Ith(y,i+1) = (realtype) INITIAL_CONDITIONS[i];
        }

        /* Set the scalar relative tolerance */
        reltol = (realtype) reltol1;

        
        /* Set the vector absolute tolerance */
        for(int i=0;i<NEQ;i++) Ith(abstol,i+1) = (realtype) abstol1[0];

        /* Call CVodeCreate to create the solver memory and specify the
         * Backward Differentiation Formula */
        cvode_mem = CVodeCreate(CV_BDF);
        if (check_retval((void *)cvode_mem, "CVodeCreate", 0)) throw(3);

        /* Call CVodeInit to initialize the integrator memory and specify the
         * user's right hand side function in y'=f(t,y), the inital time T0, and
         * the initial dependent variable vector y. */
        retval = CVodeInit(cvode_mem, RHS, T0, y);
        if (check_retval(&retval, "CVodeInit", 1)) throw(4);

        /* Call CVodeSVtolerances to specify the scalar relative tolerance
         * and vector absolute tolerances */
        retval = CVodeSVtolerances(cvode_mem, reltol, abstol);
        if (check_retval(&retval, "CVodeSVtolerances", 1)) throw(5);

        /* Call CVodeRootInit to specify the root function g with 2 components */
        // retval = CVodeRootInit(cvode_mem, 2, g);
        // if (check_retval(&retval, "CVodeRootInit", 1)) return(1);

        /* Create dense SUNMatrix for use in linear solves */
        A = SUNDenseMatrix(NEQ, NEQ);
        if(check_retval((void *)A, "SUNDenseMatrix", 0)) throw(6);

        /* Create dense SUNLinearSolver object for use by CVode */
        LS = SUNLinSol_Dense(y, A);
        if(check_retval((void *)LS, "SUNLinSol_Dense", 0)) throw(7);

        /* Call CVodeSetLinearSolver to attach the matrix and linear solver to CVode */
        retval = CVodeSetLinearSolver(cvode_mem, LS, A);
        if(check_retval(&retval, "CVodeSetLinearSolver", 1)) throw(8);
        
    }
    
    static int RHS(realtype t, N_Vector y, N_Vector ydot, void *user_data)
    {
        model *mdl;
        mdl = (model *) user_data;
        int NEQ = mdl->getNEQ();
        double DYDT[NEQ];
        double Y[NEQ];
        
        for(int i=0;i<NEQ;i++) Y[i] = Ith(y,i+1);
        
        mdl->updateAlgebraic(t, Y);
        
        mdl->getDY((double) t, Y, DYDT);
        
        for(int i=0;i<NEQ;i++) Ith(ydot,i+1) = DYDT[i];

        return(0);

    }
    
    void solveStep(double tout)
    {
        CVodeSetUserData(cvode_mem, getModel());
        
        retval = CVode(cvode_mem, (realtype) tout, y, &t, CV_NORMAL);
        if (retval == CV_ROOT_RETURN)
        {
            retvalr = CVodeGetRootInfo(cvode_mem, rootsfound);
            if (check_retval(&retvalr, "CVodeGetRootInfo", 1)) throw(1);
//            PrintRootInfo(rootsfound[0],rootsfound[1]);
        }

        if (check_retval(&retval, "CVode", 1)) throw(1);

    }
    
};

#endif /* sdsolver_hpp */
