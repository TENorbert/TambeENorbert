/***************************************************************************** 
 * Project: RooFit                                                           * 
 *                                                                           * 
 * This code was autogenerated by RooClassFactory                            * 
 *****************************************************************************/ 

// Your description goes here... 

#include "Riostream.h" 

#include "RooCFAuto000Func.h" 
#include "RooAbsReal.h" 
#include "RooAbsCategory.h" 
#include <math.h> 
#include "TMath.h" 

ClassImp(RooCFAuto000Func) 

 RooCFAuto000Func::RooCFAuto000Func(const char *name, const char *title, 
                        RooAbsReal& _lumi_kappa,
                        RooAbsReal& _beta_lumi) :
   RooAbsReal(name,title), 
   lumi_kappa("lumi_kappa","lumi_kappa",this,_lumi_kappa),
   beta_lumi("beta_lumi","beta_lumi",this,_beta_lumi)
 { 
 } 


 RooCFAuto000Func::RooCFAuto000Func(const RooCFAuto000Func& other, const char* name) :  
   RooAbsReal(other,name), 
   lumi_kappa("lumi_kappa",this,other.lumi_kappa),
   beta_lumi("beta_lumi",this,other.beta_lumi)
 { 
 } 



 Double_t RooCFAuto000Func::evaluate() const 
 { 
   // ENTER EXPRESSION IN TERMS OF VARIABLE ARGUMENTS HERE 
   return pow(lumi_kappa,beta_lumi) ; 
 } 



