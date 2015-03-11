#ifndef DPntumakerAlyzer_H
#define DPntumakerAlyzer_H
// -*- C++ -*-
//
// Package:    DPntumakerAlyzer
// Class:      DPntumakerAlyzer
// 
/**\class DPntumakerAlyzer DPntumakerAlyzer.cc Exotica/DPntumakerAlyzer/src/DPntumakerAlyzer.h

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Shih-Chuan Kao
//         Created:  Thu Sep 29 05:26:22 CDT 2011
// $Id: DPntumakerAlyzer.h,v 1.1 2012/04/26 22:54:34 tambe Exp $
// Editing by : TEN
//


// system include files
#include <memory>

//CMSSW user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
//Trigger Selection by Names
#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
//  PF things
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"

// AOD Objects
// For Primary Vertex
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
// For PF MET
#include "DataFormats/METReco/interface/PFMET.h"
#include "DataFormats/METReco/interface/PFMETCollection.h"
// For Photon-like-Objects
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
// For Tracking leading to Electrons
#include "DataFormats/GsfTrackReco/interface/GsfTrack.h"
#include "DataFormats/EgammaCandidates/interface/ElectronFwd.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
// For Muon Objects
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
// Beam Halo
#include "DataFormats/METReco/interface/BeamHaloSummary.h"
// Track finding
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

// L1 Trigger 
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"
#include "CondFormats/DataRecord/interface/L1GtTriggerMenuRcd.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"

// for ECAL cluster
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/BasicClusterFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalCleaningAlgo.h"
#include <algorithm>

// Calibration and Geometry services
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include "Geometry/CaloTopology/interface/EcalTrigTowerConstituentsMap.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/CaloTopology/interface/CaloSubdetectorTopology.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"

// Geometry
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "Geometry/CaloTopology/interface/CaloTowerConstituentsMap.h"
#include "DataFormats/CaloTowers/interface/CaloTowerDetId.h"
// InterCalibration Studies
#include "CalibCalorimetry/EcalTPGTools/interface/EcalTPGScale.h"
#include "DataFormats/EcalDetId/interface/EcalScDetId.h"
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"
#include "CondFormats/DataRecord/interface/EcalADCToGeVConstantRcd.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalTools.h"
//#include "CalibCalorimetry/EcalTiming/interface/timeVsAmpliCorrector.h"
#include "ECALTime/EcalTimePi0/interface/timeVsAmpliCorrector.h"
// PU SummeryInfo
//#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h" 

#include <TMath.h>
#include "TFile.h"
#include "TTree.h"

#include "Ntuple.h"
#include "GenStudy.h"

#include <Math/VectorUtil.h>

#define PI 3.14159265


// Anomalous Signal Studies

// use offline Spike Rejection as http://indico.cern.ch/getFile.py/access?contribId=3&resId=0&materialId=slides&confId=87687
const float swissXmaxCut = 0.95 ; // 1-e4/e1
const float e9spikeCut   = 0.95 ; // e1/e9 @ https://indico.cern.ch/getFile.py/access?sessionId=1&resId=0&materialId=1&confId=88158
const float e6e2Cut      = 0.95 ;
const int   XtalChi2threshold  = 40.0 ; // XtalChiSquare Minnimum
const int   nbadHitsInSeedBCthreshold  = 2.0 ; // Chosen Number of Bad Hits in SeedBC of SC for for SC to be Rejected.
const int   nbadchi2XtalInSeedBCthreshold = 2.0 ; // Xtals/rechits with bad chi2 to determine SC rejection.
const int   nswissXxtalsInSeedBCThreshold  = 2.0 ; // Number of Xtals with bad SwissX to determine SC rejections.
const int   ndispikeInSeedBCThreshold      = 2.0 ; // Number of Dispike  to determine rejection of SC.


using namespace std ;

//
// class declaration
//
typedef std::pair<reco::SuperClusterRef, float> ParticleSC  ;

struct PhoInfo {

  double t    ;
  double dt   ;
  double nchi2  ;
  double fSpike ;
  double maxSX  ;
  int    nxtals ;  
  int    nBC    ;

} ;


// SC Properties
 struct  BCCrys {
 //  Spike-like BC Properties 
   float se4Oe1 ; 
   float se6Oe2 ; 
   float schi2 ;
   float  stime ; 
   float  stimeErr ; 
   float sootchi2 ;
   double sCrysE ;
 } ; 

 struct BCInfo {
    int  nBadxtalInBC ;  // Crys with Bad Flags
    int  nCrysInBC ; 
    int   nsCry  ;  // Number of Spikes in BC
    int   nCrysWithBadChi2 ;
    int   nDispike ; 
    float WAvebctime ;
    float WAvebctimeErr ;
   } ;




class DPntumakerAlyzer : public edm::EDAnalyzer {
   public:
      explicit DPntumakerAlyzer(const edm::ParameterSet&);
      ~DPntumakerAlyzer();

      virtual void analyze(const edm::Event&, const edm::EventSetup&);

      bool EventSelection( const edm::Event& iEvent );

       // SC's new Trigger Selection Scheme
       bool L1TriggerSelection( const edm::Event& iEvent, const edm::EventSetup& iSetup ) ;

       void TriggerTagging( edm::Handle<edm::TriggerResults> triggers, const edm::TriggerNames& trgNameList, int RunID, vector<int>& firedTrig ) ;
       bool TriggerSelection( edm::Handle<edm::TriggerResults> triggers, vector<int> firedTrig ) ;

//      int  TriggerSelection( const edm::Event& iEvent, int cutVal, string str_head = "HLT_Photon", string str_body = "_CaloIdVL_IsoL" ) ;
//      bool TriggerSelection( const edm::Event& iEvent ) ;
 
      bool VertexSelection( edm::Handle<reco::VertexCollection> vtx ) ;

      bool PhotonSelection(  edm::Handle<reco::PhotonCollection> photons, edm::Handle<EcalRecHitCollection> recHitsEB, edm::Handle<EcalRecHitCollection> recHitsEE, edm::Handle<reco::TrackCollection> tracks, vector<const reco::Photon*>& selectedPhotons ) ;


      pair<double,double> ClusterTime( reco::SuperClusterRef scRef, edm::Handle<EcalRecHitCollection> recHitsEB, edm::Handle<EcalRecHitCollection> recHitsEE ) ;
      void ClusterTime( reco::SuperClusterRef scRef, edm::Handle<EcalRecHitCollection> recHitsEB, edm::Handle<EcalRecHitCollection> recHitsEE, PhoInfo& phoTmp, bool useAllClusters = false ) ;

      bool JetSelection( edm::Handle<reco::PFJetCollection> jets, vector<const reco::Photon*>& selectedPhotons,
                                                                     vector<const reco::PFJet*>& selectedJets ) ;
      bool ElectronSelection( edm::Handle<reco::GsfElectronCollection> electrons, 
                              vector<const reco::GsfElectron*>& selectedElectrons ) ;
      bool MuonSelection( edm::Handle<reco::MuonCollection> muons, vector<const reco::Muon*>& selectedMuons ) ;
      void PrintTriggers( const edm::Event& iEvent ) ;

      bool sMinorSelection( vector<const reco::Photon*>& selectedPhotons,  edm::Handle<EcalRecHitCollection> recHitsEB,     
                              edm:: Handle<EcalRecHitCollection> recHitsEE ) ;

      bool IsoPhotonSelection( vector<const reco::Photon*>& selectedPhotons ) ; 

      bool GammaJetVeto( vector<const reco::Photon*>& selectedPhotons, vector<const reco::PFJet*>& selectedJets) ;
     // Anomalous signal SC rejecter
     bool ASignalSCRejecter( reco::SuperClusterRef scref, edm::Handle<EcalRecHitCollection> recHitsEB, edm::Handle<EcalRecHitCollection> recHitsEE, edm::Handle<reco::TrackCollection> tracks, BCCrys& spiky, BCInfo& sidBC ) ;
     // Define e41e1 and e2e6 fxns
     float e4e1( const DetId& id, const EcalRecHitCollection& rhs) ;
     float e6e2( const DetId& id, const EcalRecHitCollection& rhs) ;
     const std::vector<DetId> neighbours (const DetId& id ) ;
     float recHitE( const DetId& id, const EcalRecHitCollection& rhs) ;


   private:

      Ntuple leaves ;

      TTree *theTree;

      TFile *theFile;
      GenStudy *gen ; 

      // ----------member data ---------------------------
      string rootFileName;
      std::vector<string> triggerPatent ;
      string l1GTSource ;
//      string triggerName ;
      bool isData ;
      bool L1Select;
      double ctau;
      double seedGenNum;
   
      
      edm::InputTag trigSource;
      edm::InputTag pvSource;
      edm::InputTag beamSpotSource;
      edm::InputTag muonSource;
      edm::InputTag electronSource;
      edm::InputTag photonSource;
      edm::InputTag metSource;
      edm::InputTag jetSource;
      edm::InputTag trackSource;
      edm::InputTag EBRecHitCollection;
      edm::InputTag EERecHitCollection;
      //edm::InputTag pileupSource ;

      // Grab Calibration Stuffs!
      edm::ESHandle<EcalIntercalibConstants> ical;
      edm::ESHandle<EcalADCToGeVConstant> agc;
      edm::ESHandle<EcalLaserDbService> laser;
      edm::ESHandle<CaloGeometry> pGeometry ;
      const CaloGeometry * theGeometry ;


      std::vector<double> muonCuts ;
      std::vector<double> electronCuts ;
      std::vector<double> photonCuts ;
      std::vector<double> photonIso ;
      std::vector<double> metCuts ;
      std::vector<double> jetCuts ; 
      std::vector<double> vtxCuts ; 

      std::vector<const reco::PFJet*> selectedJets ;
      std::vector<const reco::GsfElectron*> selectedElectrons ;
      std::vector<const reco::Muon*> selectedMuons ;
      std::vector<const reco::Photon*> selectedPhotons ;

      bool passEvent ;
      int counter[10] ; 
      float sMin_ ;
      int runID_ ;

     timeCorrector theTimeCorrector_ ;
     edm::Timestamp eventTime ;
     std::vector<int> firedTrig ;
};

#endif
