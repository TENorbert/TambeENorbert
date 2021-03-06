#ifndef Selection_H
#define Selection_H

#include "TObject.h"
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <TMath.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TFile.h>
#include <TTree.h>
#include <TBranch.h>
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
//#include "MathFunctions.h"

#define MAXSC 50
#define MAXC 200
#define MAXXTALINC 25 
#define MAXOBJ 10
#define MAXPHO 40
#define MAXVTX 40

typedef pair<int, TLorentzVector> objID ;

//class Selection : public TObject {
class Selection {

public:

   Selection( string datacardfile = "DataCard.txt");     
   ~Selection();     
   
   void Init( TTree* tr ) ;

   bool HLTFilter();
   bool PhotonFilter( bool doIso = true );
   bool JetMETFilter();
   bool VertexFilter();
   bool ElectronFilter();
   bool MuonFilter();

   bool GammaJetsBackground() ; 

   void ResetCuts( string cutName, vector<int>& cutId, vector<double>& newValue ) ;
   void ResetCuts( string cutName, int cutId, double newValue ) ;
   void ResetCuts( string cutName = "All" ) ; // set cuts to default values from datacard 

   void GetCollection( string collName, vector<objID>& coll ) ;
   void ResetCollection( string cutName = "All" ) ; // clean the storage containers 

private:

   AnaInput*       Input;

   vector<objID> phoV ;
   vector<objID> jetV ;
   vector<objID> eleV ;
   vector<objID> muV ;

   vector<double> photonCuts ;
   vector<double> photonIso ;
   vector<double> vtxCuts ;
   vector<double> jetCuts ;
   vector<double> electronCuts ;
   vector<double> muonCuts ;

   
   float vtxX[MAXVTX],    vtxY[MAXVTX],  vtxZ[MAXVTX],   vtxChi2[MAXVTX], vtxNdof[MAXVTX];
   float jetPx[MAXOBJ],   jetPy[MAXOBJ], jetPz[MAXOBJ],  jetE[MAXOBJ] ;
   float jetNDau[MAXOBJ], jetCM[MAXOBJ], jetCEF[MAXOBJ], jetNHF[MAXOBJ], jetNEF[MAXOBJ];
   float phoPx[MAXPHO],      phoPy[MAXPHO],      phoPz[MAXPHO],     phoE[MAXPHO], phoTime[MAXPHO] ;
   float phoEcalIso[MAXPHO], phoHcalIso[MAXPHO], phoTrkIso[MAXPHO], phoHovE[MAXPHO], phoSmin[MAXPHO], phoSmaj[MAXPHO] ;
   float elePx[MAXOBJ], elePy[MAXOBJ], elePz[MAXOBJ], eleE[MAXOBJ] ;
   float eleEcalIso[MAXOBJ], eleHcalIso[MAXOBJ], eleTrkIso[MAXOBJ], eleNLostHits[MAXOBJ] ;
   float muPx[MAXOBJ], muPy[MAXOBJ], muPz[MAXOBJ], muE[MAXOBJ] ;
   float muEcalIso[MAXOBJ], muHcalIso[MAXOBJ], muTrkIso[MAXOBJ] ;
   float metPx, metPy, metE ;
   int   nJets, nPhotons, nElectrons, nVertices, nMuons, eventId, trgCut ;

   //ClassDef(Selection, 1);
};

//#if !defined(__CINT__)
//    ClassImp(Selection);
#endif

