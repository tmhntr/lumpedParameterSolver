#include "sdsolver.hpp"

int sdsolver::check_retval(void *returnvalue, const char *funcname, int opt)
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

sdsolver::sdsolver(component * m, double INITIAL_CONDITIONS[], double reltol1, double abstol1[], double * deltat) : solver(m)
{
    setDeltaT(deltat);
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
    for(int i=0;i<NEQ;i++) Ith(abstol,i+1) = (realtype) abstol1[i];

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

//         Create dense SUNLinearSolver object for use by CVode
    LS = SUNLinSol_Dense(y, A);
    if(check_retval((void *)LS, "SUNLinSol_Dense", 0)) throw(7);

//        SUNMatrix A = SUNSparseMatrix(NEQ, NEQ, 19, 0);
//        if(check_retval((void *)A, "SUNSparseMatrix", 0)) throw(6);
////
//        LS = SUNLinSol_KLU(y, A);
//        if(check_retval((void *)LS, "SUNLinSol_KLU", 0)) throw(7);
//

    /* Call CVodeSetLinearSolver to attach the matrix and linear solver to CVode */
    retval = CVodeSetLinearSolver(cvode_mem, LS, A);
    if(check_retval(&retval, "CVodeSetLinearSolver", 1)) throw(8);

    if(m->init(getModel()) > 0) throw(9);

}

int sdsolver::RHS(realtype t, N_Vector y, N_Vector ydot, void *user_data)
{
    component *mdl;
    mdl = (component *) user_data;
    int NEQ = mdl->getNEQ();
    double DYDT[NEQ];
    double Y[NEQ];

    for(int i=0;i<NEQ;i++) Y[i] = Ith(y,i+1);

    mdl->setY(Y);

    mdl->updateDerived(t, Y);

    mdl->getDY((double) t, Y, DYDT);

    for(int i=0;i<NEQ;i++) Ith(ydot,i+1) = DYDT[i];

    return(0);

}

void sdsolver::solveStep(double tout)
{
    if (!started){
        getModel()->init(getModel());
        started = true;
    }

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
