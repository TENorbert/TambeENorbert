#ifndef TestGen_H
#define TestGen_H

#include "TObject.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <TMath.h>
#include <TF1.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
#include <TRandom.h>
#include <TLeaf.h>
#include <TString.h>
#include <TSystem.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TFrame.h>
#include <TLegend.h>
#include <TGraph.h>
#include <TGraphErrors.h>
#include "TLorentzVector.h"

#include "AnaInput.h"

#define MAXPHO 5
#define MAXOBJ 5
#define MAXJET 15

class TestGen : public TObject {

public:

   TestGen( string datacardfile = "DataCard.txt");     
   ~TestGen();     
   
   void ReadTree( string dataName );

private:

   AnaInput*     Input;

   string hfolder  ;
   string plotType ;
   string rfolder  ;
   int ProcessEvents ;

   float phoPx[MAXPHO], phoPy[MAXPHO], phoPz[MAXPHO], phoE[MAXPHO] ;
   float phoTime[MAXPHO], phoEta[MAXPHO],phoPhi[MAXPHO],sMinPho[MAXPHO],sMajPho[MAXPHO],jetPx[MAXJET],jetPy[MAXJET],jetPz[MAXJET], jetE[MAXJET], phoVx[MAXPHO],phoVy[MAXPHO],phoVz[MAXPHO];
   int   nPhotons, nJets ;
   float met;

   //ClassDef(TestGen, 1);
};

//#if !defined(__CINT__)
//    ClassImp(TestGen);
#endif

