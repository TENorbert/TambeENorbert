

// This is an example of a RooStats interval calculation
// using Bayesian approach via numeric integration
//

/*
RooStats exercise at CMSDAS-2012:
  Fermilab, January 10-14, 2012
  inspired by official RooStats tutorials,
  see http://root.cern.ch/root/html/tutorials/roostats/
*/

#include "TFile.h"
#include "TCanvas.h"
#include "RooWorkspace.h"
#include "RooAbsData.h"
#include "RooAbsReal.h"
#include "RooRealVar.h"
#include "RooArgSet.h"
#include "RooArgList.h"
#include "RooDataSet.h"
#include "RooPlot.h"
#include "RooStats/ModelConfig.h"
#include "RooStats/SimpleInterval.h"
#include "RooStats/BayesianCalculator.h"
#include "RooStats/SequentialProposal.h"
#include "RooStats/MCMCCalculator.h"
#include "RooStats/MCMCInterval.h"
#include "RooStats/MCMCIntervalPlot.h"


using namespace RooFit;
using namespace RooStats;
using namespace std; 


int GetBayesianInterval( std::string filename = "workspace.root",  std::string wsname = "myWS" ){
  
   //
  // this function loads a workspace and computes
  // a Bayesian upper limit
  //
// open file with workspace for reading
  TFile * pInFile = new TFile(filename.c_str(), "read");

  // load workspace
  RooWorkspace * pWs = (RooWorkspace *)pInFile->Get(wsname.c_str());
  if (!pWs){
    std::cout << "workspace " << wsname 
	      << " not found" << std::endl;
   return -1;
  }

  // printout workspace content
  pWs->Print();

  // load and print data from workspace
  RooAbsData * data = pWs->data("data");
  data->Print();
  
  // load and print S+B Model Config
  RooStats::ModelConfig * pSbHypo = (RooStats::ModelConfig *)pWs->obj("SbHypo");
  pSbHypo->Print();

  return 0;
}

//void bayesian_num ( void) {

// This is the main fxn
//cout << "Now Runnning fxn GetBayesianInterval" << endl;
// GetBayesianInterval( std::string filename = "workspace.root", std::string wsname = "myWS" );
 

//return ;
//}


