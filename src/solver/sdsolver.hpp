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
// #include <sunmatrix/sunmatrix_sparse.h>
#include <sunlinsol/sunlinsol_dense.h> /* access to dense SUNLinearSolver      */
// #include <sunlinsol/sunlinsol_klu.h>
// #include <sundials/sundials_types.h>   /* defs. of realtype, sunindextype      */

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
    double _deltat = 0.01; // bring deltat out of solver // units: s

    bool started = false;

    realtype reltol, t;
    N_Vector y, abstol;
    SUNMatrix A;
    SUNLinearSolver LS;
    void *cvode_mem;
    int retval, retvalr, iout;
    int rootsfound[2];

    static int check_retval(void *returnvalue, const char *funcname, int opt);
public:
//    sdsolver() : solver() {}
    // sdsolver(model * m) : solver(m) { }

//    void setDeltaT(double deltat) { _deltat = deltat; }
//    double getDeltaT() { return _deltat; }

    double getY(int index) { return Ith(y,index+1); }
    double getT() { return (double) t;}

    sdsolver(model * m, double INITIAL_CONDITIONS[], double reltol1, double abstol1[]);

    static int RHS(realtype t, N_Vector y, N_Vector ydot, void *user_data);

    void solveStep(double tout);

};

#endif /* sdsolver_hpp */
