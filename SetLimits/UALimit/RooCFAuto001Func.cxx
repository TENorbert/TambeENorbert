/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooCFAuto001Func.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(RooCFAuto001Func) 

 RooCFAuto001Func::RooCFAuto001Func(const char *name, const char *title, 
                        RooAbsReal& _efficiency_kappa,
                        RooAbsReal& _beta_efficiency) :
   RooAbsReal(name,title), 
   efficiency_kappa("efficiency_kappa","efficiency_kappa",this,_efficiency_kappa),
   beta_efficiency("beta_efficiency","beta_efficiency",this,_beta_efficiency)
 { 
 } 


 RooCFAuto001Func::RooCFAuto001Func(const RooCFAuto001Func& other, const char* name) :  
   RooAbsReal(other,name), 
   efficiency_kappa("efficiency_kappa",this,other.efficiency_kappa),
   beta_efficiency("beta_efficiency",this,other.beta_efficiency)
 { 
 } 



 Double_t RooCFAuto001Func::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return pow(efficiency_kappa,beta_efficiency) ; 
 } 



