// -*- C++ -*-
//
// Package:    DPntumakerAlyzer
// Class:      DPntumakerAlyzer
// 
/**\class DPntumakerAlyzer DPntumakerAlyzer.cc EXO/DPntumakerAlyzer/src/DPntumakerAlyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Shih-Chuan Kao
//         Created:  Sat Oct  8 06:50:16 CDT 2011
// $Id: DPntumakerAlyzer.cc,v 1.1 2012/04/26 22:54:15 tambe Exp $
//    Editted  by TEN
//


// system include files
#include "DPntumakerAlyzer.h"
#include "Ntuple.h"

using namespace cms ;
using namespace edm ;
using namespace std ;

// constants, enums and typedefs
// static data member definitions

// constructors and destructor
DPntumakerAlyzer::DPntumakerAlyzer(const edm::ParameterSet& iConfig){

   //now do what ever initialization is needed
   rootFileName         = iConfig.getUntrackedParameter<string> ("rootFileName");
   l1GTSource           = iConfig.getParameter<string> ("L1GTSource");
   trigSource           = iConfig.getParameter<edm::InputTag> ("trigSource");
   pvSource             = iConfig.getParameter<edm::InputTag> ("pvSource");
   beamSpotSource       = iConfig.getParameter<edm::InputTag> ("beamSpotSource");
   muonSource           = iConfig.getParameter<edm::InputTag> ("muonSource");
   electronSource       = iConfig.getParameter<edm::InputTag> ("electronSource");
   photonSource         = iConfig.getParameter<edm::InputTag> ("photonSource");
   metSource            = iConfig.getParameter<edm::InputTag> ("metSource");
   jetSource            = iConfig.getParameter<edm::InputTag> ("jetSource");
   trackSource          = iConfig.getParameter<edm::InputTag> ("trackSource");

   EBRecHitCollection   = iConfig.getParameter<edm::InputTag> ("EBRecHitCollection") ;
   EERecHitCollection   = iConfig.getParameter<edm::InputTag> ("EERecHitCollection") ;
 
   //pileupSource         = iConfig.getParameter<edm::InputTag>("addPileupInfo");

   vtxCuts              = iConfig.getParameter<std::vector<double> >("vtxCuts");
   jetCuts              = iConfig.getParameter<std::vector<double> >("jetCuts");
   metCuts              = iConfig.getParameter<std::vector<double> >("metCuts");
   photonCuts           = iConfig.getParameter<std::vector<double> >("photonCuts");
   photonIso            = iConfig.getParameter<std::vector<double> >("photonIso");
   electronCuts         = iConfig.getParameter<std::vector<double> >("electronCuts");
   muonCuts             = iConfig.getParameter<std::vector<double> >("muonCuts");  
// triggerName          = iConfig.getUntrackedParameter<string> ("triggerName");
   triggerPatent        = iConfig.getParameter< std::vector<string> >("triggerName");
   isData               = iConfig.getUntrackedParameter<bool> ("isData");
   ctau                 = iConfig.getParameter<double>("ctau");
   seedGenNum           = iConfig.getParameter<double>("seedGenNum");
   L1Select             = iConfig.getParameter<bool>("L1Select");

   gen = new GenStudy( iConfig );

   theFile  = new TFile( rootFileName.c_str(), "RECREATE") ;
   theFile->cd () ;
   theTree  = new TTree ( "DPntumakerAlyzer","DPntumakerAlyzer" ) ;
   setBranches( theTree, leaves ) ;

 // why put in -1 to all firedTriggers container?
   firedTrig.clear() ;
   for( size_t i = 0 ; i < triggerPatent.size(); i++) firedTrig.push_back(-1) ;

   // reset the counter
   for ( int i=0; i< 10 ; i++) counter[i] = 0 ;
  // initialize the time corrector
   theTimeCorrector_.initEB("EB");
   theTimeCorrector_.initEE("EElow");
       runID_ = 0;
}


DPntumakerAlyzer::~DPntumakerAlyzer()
{
   // do anything here that needs to be done at desctruction time

   delete gen ;
   cout<<"All:"<< counter[0]<<" dumper:"<<counter[1]<<" Vertex:"<< counter[2] <<" photon:"<<counter[3] ;
   cout<<" sMinor:"<< counter[4] <<" BeamHalo:"<< counter[5] <<" Iso:"<<counter[6] <<" GJet:"<<counter[7] ;
   cout<<" Jets:"<< counter[8] << " G100:"<< counter[9] <<endl;
   theFile->cd () ;
   theTree->Write() ; 
   theFile->Close() ;

}

//
// member functions
//

// ------------ method called for each event  ------------
void DPntumakerAlyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   // get calibration service
  // IC's
  iSetup.get<EcalIntercalibConstantsRcd>().get(ical);
  // ADCtoGeV
  iSetup.get<EcalADCToGeVConstantRcd>().get(agc);
  // transp corrections
  iSetup.get<EcalLaserDbRecord>().get(laser);
  // Geometry
  iSetup.get<CaloGeometryRecord> ().get (pGeometry) ;
  theGeometry = pGeometry.product() ; 

    // Evt time
   eventTime = iEvent.time()   ;

   initializeBranches( theTree, leaves );

   leaves.bx          = iEvent.bunchCrossing();
   leaves.lumiSection = iEvent.id().luminosityBlock();
   leaves.orbit       = iEvent.orbitNumber();
   leaves.runId       = iEvent.id().run() ;
   leaves.eventId     = iEvent.id().event() ;



  /* 
   Handle<std::vector< PileupSummaryInfo > >  PupInfo;
   iEvent.getByLabel(pileupSource, PupInfo);

   for( std::vector<PileupSummaryInfo>::const_iterator PVI = PupInfo->begin(); PVI != PupInfo->end(); ++PVI) {
       std::cout << " Pileup Information: bunchXing, nvtx: " << PVI->getBunchCrossing() << " " << PVI->getPU_NumInteractions() << std::endl;
   }
   */

   if( (counter[0] = 0) ) { PrintTriggers( iEvent ) ;}
 
   int run_id    = iEvent.id().run() ;

   counter[0]++ ;  

 // L1 Trigger Selection
   bool passL1  =  L1TriggerSelection(iEvent, iSetup ) ;
 
 // HLT Trigger begins
   Handle<edm::TriggerResults> triggers;
   iEvent.getByLabel( trigSource, triggers );
   const edm::TriggerNames& trgNameList = iEvent.triggerNames( *triggers ) ;

   // Tag triggers 
   TriggerTagging( triggers, trgNameList, run_id, firedTrig ) ;
   bool passHLT = TriggerSelection( triggers, firedTrig ) ;

// Using L1 or HLT to select events
   bool passTrigger = (L1Select ) ? passL1 : passHLT ;  

   if( passTrigger) counter[1]++ ;
//   bool passTrg = TriggerSelection( iEvent ) ;

// Check if Data or MC?
   if ( !isData ){ gen->GetGenEvent( iEvent, leaves );
   
                 }
   // Previous way of getting MC infos
 //  if ( !isData ) gen->GetGen( iEvent, leaves );

   // Did it pass Evt Selection?
   bool pass = EventSelection( iEvent ) ;
   
   if ( pass && !isData ) theTree->Fill();        // MC evts which pass Event selection!
   if ( pass && isData &&  passTrigger ) theTree->Fill();  // Data Evts which pass trigger and TriggerSel !:w!

}

bool DPntumakerAlyzer::EventSelection(const edm::Event& iEvent ) {

   Handle<reco::BeamHaloSummary>       beamHaloSummary ;
   Handle<edm::TriggerResults>         triggers;
   Handle<reco::VertexCollection>      recVtxs;
   Handle<reco::PhotonCollection>      photons; 
   Handle<reco::GsfElectronCollection> electrons; 
   Handle<reco::MuonCollection>        muons; 
   Handle<reco::PFJetCollection>       jets; 
   Handle<reco::PFMETCollection>       met; 
   Handle<EcalRecHitCollection>        recHitsEB ;
   Handle<EcalRecHitCollection>        recHitsEE ;
   Handle<reco::TrackCollection>       tracks    ;


   iEvent.getByLabel( trigSource,     triggers );
   iEvent.getByLabel( pvSource,       recVtxs  );
   iEvent.getByLabel( photonSource,   photons  );
   iEvent.getByLabel( electronSource, electrons);
   iEvent.getByLabel( muonSource,     muons );
   iEvent.getByLabel( jetSource,      jets  );
   iEvent.getByLabel( metSource,      met  );
   iEvent.getByLabel( EBRecHitCollection,     recHitsEB );
   iEvent.getByLabel( EERecHitCollection,     recHitsEE );
   iEvent.getByLabel("BeamHaloSummary", beamHaloSummary) ;
   iEvent.getByLabel( trackSource,    tracks  )          ;

   bool passEvent = true ;


// Select events based on how good their Primary Vertex is!
   bool hasGoodVtx = VertexSelection( recVtxs );
   if (! hasGoodVtx ) passEvent = false ;
   if ( passEvent )   counter[2]++ ; 

// For Photons
   selectedPhotons.clear() ;
   PhotonSelection( photons, recHitsEB, recHitsEE, tracks, selectedPhotons ) ;
   if ( selectedPhotons.size() < (size_t)photonCuts[6] )  passEvent = false ;
   if ( passEvent )   counter[3]++ ;  

     // Avoid sMinor Selection for Now!
//   sMinorSelection( selectedPhotons, recHitsEB, recHitsEE ) ;
//   if ( selectedPhotons.size() < (size_t)photonCuts[5] )  passEvent = false ;
   /*
   if ( passEvent ) {  counter[4]++ ;  
                       printf("id:%d, nPho:%d, Pt:%.3f sMin:%.6f \n", 
                iEvent.id().event(), (int)selectedPhotons.size(), selectedPhotons[0]->pt(), sMin_ ) ;
   }   
   */
   if( beamHaloSummary.isValid() ) {
     const reco::BeamHaloSummary TheSummary = (*beamHaloSummary.product() ); 
     if( !TheSummary.CSCTightHaloId() && passEvent ) { 
       counter[4]++ ;  
     } else {
       passEvent = false ;
     }
   } else {
       counter[4]++ ;
   }

   // Neglect Isphoton Selection for now! 
//   IsoPhotonSelection( selectedPhotons ) ;
//   if( selectedPhotons.size() < photonCuts[6] ) passEvent = false ;
   if ( passEvent )   counter[5]++ ;  



// For   Jets
   selectedJets.clear() ;
   JetSelection( jets, selectedPhotons, selectedJets );

    // Neglect GammaJetSelection for now!
//   bool isGammaJets = GammaJetVeto( selectedPhotons, selectedJets ) ;
//   if ( isGammaJets ) passEvent = false ;

   if ( selectedJets.size() < jetCuts[3]  )   passEvent = false ;
   if ( passEvent )   counter[6]++ ;  


// Ask if pass required Jet multiplicity Selection
 //  selectedJets.clear();
//   bool passJet = JetSelection( jets, selectedPhotons, selectedJets );  // Select from 0 to 99 Jets Do event Selection @@ Ntuple Analyzer level.
   
//   if ( passEvent ) counter[7]++; 
//   if (! passJet )   passEvent = false ;
//   if ( passEvent)  counter[8]++ ;
 

 // For Electrons
   selectedElectrons.clear() ;
   ElectronSelection( electrons, selectedElectrons ) ;

// For Muons
   selectedMuons.clear() ;
   MuonSelection ( muons, selectedMuons ) ;

   //if ( selectedPhotons.size() > 0 ) {
   //   if ( selectedPhotons[0]->pt() < 100. ) passEvent = false ;
   //}
//   if ( passEvent ) counter[9]++ ; 

   /*
   selectedElectrons.clear() ;
   if ( electronCuts[0] == 1 ) ElectronSelection( electrons, selectedElectrons ) ;

   selectedMuons.clear() ;
   if ( muonCuts[0] == 1 )MuonSelection( muons, selectedMuons );
   */

  // Now Select Evt Based on PFMET   so brief? why?

   const reco::PFMET pfMet = (*met)[0] ;
   leaves.met   = pfMet.et() ;
   leaves.metPx = pfMet.px() ;
   leaves.metPy = pfMet.py() ;
   leaves.metPhi = pfMet.phi()/PI*180.0;
   if ( pfMet.pt() < metCuts[0]  ) passEvent = false;

   return passEvent ;
}

 // Now Lets Define  what all those fxns Above means!

//***********L1TriggerSelection fxn **************************************************//
bool DPntumakerAlyzer::L1TriggerSelection( const edm::Event& iEvent, const edm::EventSetup& iSetup ) {

    // Get L1 Trigger menu
    edm::ESHandle<L1GtTriggerMenu> menuRcd;
    iSetup.get<L1GtTriggerMenuRcd>().get(menuRcd) ;
    const L1GtTriggerMenu* menu = menuRcd.product();
    // Get L1 Trigger record  
    edm::Handle< L1GlobalTriggerReadoutRecord > gtRecord;
    iEvent.getByLabel( edm::InputTag("gtDigis"), gtRecord);
    // Get dWord after masking disabled bits
    const DecisionWord dWord = gtRecord->decisionWord();

   bool l1_accepted = menu->gtAlgorithmResult( l1GTSource , dWord);
   //int passL1 =  ( l1SingleEG22 ) ? 1 : 0 ; 

   //cout<<" pass L1 EG22 ? "<<  passL1 <<endl ;
   if ( l1_accepted ) leaves.L1a = 1 ;

   return l1_accepted ;
}


//**************************** TriggerTaggging ******************************************//
void DPntumakerAlyzer::TriggerTagging( Handle<edm::TriggerResults> triggers, const edm::TriggerNames& trgNameList, int RunId, vector<int>& firedTrig ) {

   if ( runID_ != RunId )  {
      for (size_t j=0; j< triggerPatent.size(); j++ ){ firedTrig[j] = -1;}

      // loop through trigger menu
      for ( size_t i =0 ; i < trgNameList.size(); i++ ) {
          string tName  = trgNameList.triggerName( i );
          // loop through desired triggers
          for ( size_t j=0; j< triggerPatent.size(); j++ )  {
              if ( strncmp( tName.c_str(), triggerPatent[j].c_str(), triggerPatent[j].size() ) ==0 ) {
                  firedTrig[j] = i;
                 cout<<" Trigger Found ("<<j <<"):  "<<tName ;
                 cout<<" Idx: "<< i <<" triggers "<<endl;
              }
          }
      }
      runID_ = RunId ;
   }

}

//**************************TriggerSelection fxn*****New Version **********************************************////
bool DPntumakerAlyzer::TriggerSelection( Handle<edm::TriggerResults> triggers, vector<int> firedTrigID ) {

   bool pass =false ;
   uint32_t trgbits = 0 ;
   for ( size_t i=0; i< firedTrigID.size(); i++ ) {
       if ( firedTrigID[i] == -1 ) continue ;
       if ( triggers->accept( firedTrigID[i] ) == 1  ) trgbits |= ( 1 << i ) ;
       //`cout<<" ("<< i <<") Trigger Found : "<< firedTrigID[i] <<" pass ? "<< triggers->accept( firedTrigID[i] ) <<" trigbit = "<< trgbits << endl; 
   }

   if ( trgbits != 0 ) {
      leaves.triggered = (int)(trgbits) ;
      pass = true ;
   }

   return pass ;
}

// ************************Old TriggerSelection fxn****************************///
/* bool DPntumakerAlyzer::TriggerSelection( const edm::Event& iEvent ) {

   Handle<edm::TriggerResults> triggers;
   iEvent.getByLabel( trigSource, triggers );

   const edm::TriggerNames& trgNames = iEvent.triggerNames( *triggers );

   int trgIndex  = trgNames.triggerIndex(triggerName);
   int trgResult = 0;
   if ( trgIndex == (int)(trgNames.size()) ) {
          cout<<" NO Matched Trigger -- Turn TriggerSelection Off "<<endl;
          cout<<"" <<endl;
          trgResult = 1 ;
   } else {
          trgResult = triggers->accept(trgIndex);
          leaves.triggered = trgResult ;
   }

   bool pass =  ( trgResult == 1 ) ? true : false ;
   return pass ;
}

int DPntumakerAlyzer::TriggerSelection( const edm::Event& iEvent, int cutVal, string str_head, string str_body ) {

   Handle<edm::TriggerResults> triggers;
   iEvent.getByLabel( trigSource, triggers );

   cout<<" ** Trigger size = "<< triggers->size() <<endl;
   const edm::TriggerNames& trgNames = iEvent.triggerNames( *triggers );

   int cutSize = ( cutVal > 99 ) ? 3 : 2 ;
   int trgWordSize = str_head.size() + str_body.size() + cutSize + 3 ; //  3 -> _vX

   int trgResult = -1 ;
   int trgResultSum = 0 ;
   for ( size_t i =0 ; i < trgNames.size(); i++ ) {
       string tName  = trgNames.triggerName( i );
       if  ( (int) tName.size() < trgWordSize ) continue ;
       string strhead = tName.substr(0, str_head.size() ) ;
       string strPt   = tName.substr( str_head.size() , cutSize ) ;
       int cutPt      =  atoi ( strPt.c_str() ) ;
       string strbody = tName.substr( str_head.size() + cutSize , str_body.size() ) ;
       string strend = tName.substr( trgWordSize-3 ,3) ;
       if ( strhead == str_head && strbody == str_body && cutPt >= cutVal) {
          int trgIndex  = trgNames.triggerIndex(tName);
          int accept    = triggers->accept(trgIndex);
          if ( accept == 1 ) trgResult = cutPt ; 
          if ( accept == 0 ) trgResult = 0 ; 
          
          if ( accept == 1 ) trgResultSum += cutPt ; 
          cout<<" -> "<< strhead << cutPt << strbody << strend <<" = "<< trgResult <<"  sum = "<<trgResultSum <<endl;
       }
   } 
   return trgResult ;
}
*/

//**********************Print Triggers fxn ******************************/////
void DPntumakerAlyzer::PrintTriggers( const edm::Event& iEvent ) {

   Handle<edm::TriggerResults> triggers;
   iEvent.getByLabel( trigSource, triggers );

   cout<<" ** Trigger size = "<< triggers->size() <<endl;
   const edm::TriggerNames& trgNames = iEvent.triggerNames( *triggers );

   for ( size_t i =0 ; i < trgNames.size(); i++ ) {
       string tName  = trgNames.triggerName( i );
       int trgIndex  = trgNames.triggerIndex(tName);
       int trgResult = triggers->accept(trgIndex);
       cout<<" name: "<< tName <<"  idx:"<< trgIndex <<"  accept:"<< trgResult <<endl;
 
        for ( size_t j=0; j< triggerPatent.size(); j++) {
           if ( strncmp( tName.c_str(), triggerPatent[j].c_str(), triggerPatent[j].size() ) ==0 ) {
              //TriggerName = tName ;
              cout<<" Trigger Found : "<< tName <<" accepted ? "<< triggers->accept(i) <<endl;
           }
       }


       //string triggered = triggers->accept(i) ? "Yes" : "No" ;
       //cout<<" path("<<i<<") accepted ? "<< triggered ;
   }
}

//********************* VertexSelction fxn **********************///
bool DPntumakerAlyzer::VertexSelection( Handle<reco::VertexCollection> vtx ) {

    int thisVertex=0;
    bool hasGoodVertex = true ;
    int totalN_vtx = 0 ;

    for(reco::VertexCollection::const_iterator v=vtx->begin();  v!=vtx->end() ; v++){

       if ( v->isFake() ) continue ;   // Remove Evts with no reconstructed Vertex ie chiSq ==0 and ndof == 0
       if ( !v->isValid() ) continue ;  // Remove Evts with Non Valid tracks

       if ( fabs(v->z()) >= vtxCuts[0] ) continue ; 
       if (   v->ndof()   < vtxCuts[1] ) continue ;
       double d0 = sqrt( ( v->x()*v->x() ) + ( v->y()*v->y() ) );
       if ( d0 >= vtxCuts[2] ) continue ;
       totalN_vtx++ ;       

       leaves.vtxNTracks[thisVertex]= v->tracksSize();
       leaves.vtxChi2[thisVertex] =   v->chi2();
       leaves.vtxNdof[thisVertex] =   v->ndof();
       leaves.vtxX[thisVertex] =      v->x();
       leaves.vtxY[thisVertex] =      v->y();
       leaves.vtxZ[thisVertex] =      v->z();
       leaves.vtxDx[thisVertex] =     v->xError();
       leaves.vtxDy[thisVertex] =     v->yError();
       leaves.vtxDz[thisVertex] =     v->zError();
     
       thisVertex++ ;
     }
     leaves.nVertices = thisVertex ;
     leaves.totalNVtx = totalN_vtx ;

     if ( thisVertex < 1 )   hasGoodVertex = false ;
 
     return hasGoodVertex ;
}

//****** Useful Fxns *****///

float DPntumakerAlyzer::recHitE( const DetId& id, const EcalRecHitCollection& recHits){
if ( id == DetId(0) ) {
    return 0;
  } else {
    EcalRecHitCollection::const_iterator it = recHits.find( id );
    if ( it != recHits.end() ) return (*it).energy();
  }
  return 0;
}  // End of recHit Energy

const std::vector<DetId> DPntumakerAlyzer::neighbours( const DetId& id ) {

  std::vector<DetId> ret;

  if ( id.subdetId() == EcalBarrel) {

    ret.push_back( EBDetId::offsetBy( id,  1, 0 ));
    ret.push_back( EBDetId::offsetBy( id, -1, 0 ));
    ret.push_back( EBDetId::offsetBy( id,  0, 1 ));
    ret.push_back( EBDetId::offsetBy( id,  0,-1 ));
  }
  // nobody understands what polymorphism is for, sgrunt !
  else  if (id.subdetId() == EcalEndcap) {
    ret.push_back( EEDetId::offsetBy( id,  1, 0 ));
    ret.push_back( EEDetId::offsetBy( id, -1, 0 ));
    ret.push_back( EEDetId::offsetBy( id,  0, 1 ));
    ret.push_back( EEDetId::offsetBy( id,  0,-1 ));

  }


  return ret;

}  // End of Crystal neighbors


// e4e1
float DPntumakerAlyzer::e4e1( const DetId& id, const EcalRecHitCollection& rhs){

  float s4 = 0;
  float e1 =recHitE( id, rhs );
  
  
  if ( e1 == 0 ) return 0;
  const std::vector<DetId>& neighs =  neighbours(id);
  for (size_t i=0; i<neighs.size(); ++i)
    // avoid hits out of time when making s4
    s4+=recHitE(neighs[i],rhs);   // Does not use Oot timeinfo
  
  return s4 / e1;
  
} // end of e4e1


//e6e2
float DPntumakerAlyzer::e6e2( const DetId& id, const EcalRecHitCollection& rhs){

    float s4_1 = 0;
    float s4_2 = 0;
    float e1 = recHitE( id, rhs );

    float maxene=0;
    DetId maxid;

    if ( e1 == 0 ) return 0;

    const std::vector<DetId>& neighs =  neighbours(id);

    // find the most energetic neighbour ignoring time info
    for (size_t i=0; i<neighs.size(); ++i){
      float ene = recHitE(neighs[i],rhs);
      if (ene>maxene)  {
	maxene=ene;
	maxid = neighs[i];
      }
    }

    float e2=maxene;

    s4_1 = e4e1(id,rhs)* e1;
    s4_2 = e4e1(maxid,rhs)* e2;

    return (s4_1 + s4_2) / (e1+e2) -1. ;

}  // End of e6e2


////***********//AnomalousSignalSuperCluster Rejecter********************************//////

bool DPntumakerAlyzer::ASignalSCRejecter( reco::SuperClusterRef scref, Handle<EcalRecHitCollection> recHitsEB, Handle<EcalRecHitCollection> recHitsEE, Handle<reco::TrackCollection> tracks, BCCrys& spiky, BCInfo& sidBC ) {

  using namespace std ;
  const EcalIntercalibConstantMap& icalMap = ical->getMap();
  float adcToGeV = float(agc->getEBValue());
  // Flag to reject SC
  bool rejectSC = false ;
  std::vector<BCCrys> SpikeBCInf ; 
  int  nSCrej = 0 ; 
//  Bad xtals
//  int nkpoor = 0 ;
//  int nL1flag = 0 ;
  int nswissX = 0 ;
  int nbadHits = 0 ;
  int nxtal  = 0 ;  // number of BC in SC
  int Nbadchi2Xtal = 0;
  int ndispike  = 0;   // number of dispike

  float swissX = 0 ;
  float e6_e2  = 0 ;
  float e4_e1  = 0 ;
  float hittime = 0 ;
  float xtalChi2 = 0 ;
  float ootChi2  = 0 ;
  float  xtaltimeErr   = 0 ;
  double  xtalEr       = 0 ;

  // crys time
  float xtime = 0 ;
  float xtimeErr = 0 ;

// Loop Over all SCref
for ( reco::CaloCluster_iterator clusref = scref->clustersBegin(); clusref  != scref->clustersEnd(); ++clusref) {


// Get only seed BC in SC
if ( *clusref != scref->seed()  )  continue ;
reco::CaloClusterPtr seedBC = scref->seed() ;  
// get nxtals in SeedBC
nxtal = seedBC->size() ;
       
// Now Loop seed BC Rechits for bad ones
std::vector<std::pair<DetId, float> > clusterDetIds = (*clusref)->hitsAndFractions() ; //get rechit and DetId from the cluster


for (std::vector<std::pair<DetId, float> >::const_iterator detitr = clusterDetIds.begin () ; detitr != clusterDetIds.end () ; ++detitr ) { 
	      // Here I use the "find" on a recHit collection... I have been warned...   (GFdoc: ??)
   	      // GFdoc: check if DetId belongs to ECAL; if so, find it among those if this basic cluster
    	     if ( (detitr -> first).det () != DetId::Ecal)  { 
   	          cout << " det is " << (detitr -> first).det () << " (and not DetId::Ecal)" << endl ;
	          continue ;
	     }
             bool isEB = ( (detitr -> first).subdetId () == EcalBarrel)  ? true : false ;
	   
	     // GFdoc now find it!
	     EcalRecHitCollection::const_iterator thishit = (isEB) ? recHitsEB->find( (detitr->first) ) : recHitsEE->find( (detitr->first) );
	     if (thishit == recHitsEB->end () &&  isEB )  continue ;
	     if (thishit == recHitsEE->end () && !isEB )  continue ;

              // Now I have the Hit I can do what I want with it!
              EcalRecHit hit = (*thishit) ;
       
              // Make Checks on Hit!
              if (! (hit.checkFlag(EcalRecHit::kGood) || hit.checkFlag(EcalRecHit::kPoorReco) || hit.checkFlag(EcalRecHit::kWeird)
		     || hit.checkFlag(EcalRecHit::kDiWeird) ) )  nbadHits++ ;   // count hits flagged as bad
              if (! (hit.checkFlag(EcalRecHit::kGood) || hit.checkFlag(EcalRecHit::kPoorReco) || hit.checkFlag(EcalRecHit::kWeird)
		     || hit.checkFlag(EcalRecHit::kDiWeird) ) ) continue ;  // skip them
       
		// Get SwissX value   //Does not use any timing                      
	     swissX = EcalTools::swissCross( detitr->first, *recHitsEB, 0.5 );
                // Get e6/e2 Value
             e6_e2  = e6e2( detitr->first, *recHitsEB );
             e4_e1  = e4e1( detitr->first, *recHitsEB );                    
		  // reject single spikes
		if ( swissX > swissXmaxCut) nswissX++;
//                if ( swissX > swissXmax ) continue ;      // do Not skip spikelike but count them
		// reject di-spikes
		if ( e6_e2 < e6e2Cut ) ndispike++ ;       
//		if ( e6_e2 < e6e2Cut ) continue ;       // Do Not Skip di-spike rechits
         
	      hittime     = hit.time()  ; 	
		// Get Chi^2 of Time from Each Xtal
              xtalChi2   =  hit.chi2() ;
              ootChi2    = hit.outOfTimeChi2() ; 
      
                // Check if Rechit Time error is valid then get it
		if ( hit.isTimeErrorValid() )  xtaltimeErr = hit.timeError() ;
		  //count number Xtals with bad Chi2
		if ( xtalChi2  >  XtalChi2threshold )   Nbadchi2Xtal++ ;

                // Look at Energy of Xtal
                // thisamp is the EB amplitude of the current rechit
                double thisamp  = hit.energy () ;
   	                xtalEr  = thisamp  ; 	   

               //  Put Info on on Ntuple
               spiky.se4Oe1 = e4_e1 ; 
               spiky.se6Oe2 = e6_e2 ; 
               spiky.schi2  = xtalChi2 ;
               spiky.sootchi2 = ootChi2 ; 
               spiky.stime    = hittime ;
               spiky.stimeErr = xtaltimeErr ; 
               spiky.sCrysE      =  xtalEr ;  

// Keep Infor of Each Xtal In BC
SpikeBCInf.push_back(spiky) ;

EcalIntercalibConstantMap::const_iterator icalit = icalMap.find(detitr->first);
EcalIntercalibConstant icalconst = 1;

if( icalit!=icalMap.end() ) {
 icalconst = (*icalit);
 } else {
 std::cout << "No intercalib const found for xtal " << (detitr->first).rawId();
        }
  	     	   	     	     	     	       	     	          	     	   
  // get laser coefficient
  float lasercalib = laser->getLaserCorrection( detitr->first, eventTime );
 
 // discard rechits with A/sigma < 12
  if ( thisamp/(icalconst*lasercalib*adcToGeV) < (1.1*12) ) continue ;
 
  GlobalPoint pos = theGeometry->getPosition(( hit).detid()) ;
  
 // Seed BC time and error with time correction
  float  thistime = hit.time();
  thistime += theTimeCorrector_.getCorrection((float) thisamp/(icalconst*lasercalib*adcToGeV), pos.eta()  );  
 // get time error 
  float xtimeErr_ = ( hit.isTimeErrorValid() ) ?  hit.timeError() : 999999 ;
     	          	     	   	     	     	     	     	                  	     	                                            
 xtime     += thistime / pow( xtimeErr_ , 2 ) ;
  
 xtimeErr  += 1/ pow( xtimeErr_ , 2 ) ;
     
 } // end of Loop over seed BC Xtals

  // Calculate Weighted Seed BC average Time and error							 
   float  wAveBCTime = xtime / xtimeErr ;   	     	   	     	     	     	       	     	        	          
   float  wAveBCTimeErr = 1. / sqrt( xtimeErr) ;
       //Now   Reject SC if many Bad Xtals in Seed BC of SC
        if ( ( nswissX >  nswissXxtalsInSeedBCThreshold )  || ( Nbadchi2Xtal >  nbadchi2XtalInSeedBCthreshold ) || 
         ( ndispike >  ndispikeInSeedBCThreshold )  || ( nbadHits >  nbadHitsInSeedBCthreshold  ) )   rejectSC = true ;
       
    // Ave Info
     sidBC.WAvebctime = wAveBCTime ;
     sidBC.WAvebctimeErr = wAveBCTimeErr ; 
     sidBC.nsCry    = nswissX  ; 
     sidBC.nBadxtalInBC = nbadHits ; 
     sidBC.nCrysInBC   = nxtal ;
     sidBC.nCrysWithBadChi2  =  Nbadchi2Xtal; 
     sidBC.nDispike   = ndispike ; 
  // Now Print info of Seed BC info of Rejected SC

if ( rejectSC ) { nSCrej++ ; 
cout << "hmmm! I have rejected" << nSCrej << "Bad SC" << endl ;

for ( unsigned r = 0 ; r < SpikeBCInf.size(); r++ ) {
cout << "Rejected BC Infos for Xtals:  Spike Crys Chi2:\t " << SpikeBCInf[r].schi2 << " And Spike Crys OOTimeChi2:\t" << SpikeBCInf[r].sootchi2 << "\n";
cout <<" Spike Crys Time :\t" << SpikeBCInf[r].stime << "\n";
cout << "Spike Crys Time Err:\t" << SpikeBCInf[r].stimeErr << " Spike Crys Energy(GeV):\t " << SpikeBCInf[r].sCrysE << "\n";
cout <<" Spike Crys SwissCross in BC: \t" << SpikeBCInf[r].se4Oe1 << endl; 
cout << " Weighted AverageTime In BC:\t " << wAveBCTime << " And Weighted AverageTime Error In BC: \t" << wAveBCTimeErr << endl;
}
 
} else {
cout <<" Info of Good SC" << " This Are Passed BC" << endl ;
       }       
 

         }   // End of Loop Over all SC
   
       
return rejectSC ; 

 }  


///*******************PhotonSelection ********************************************/////
bool DPntumakerAlyzer::PhotonSelection( Handle<reco::PhotonCollection> photons, Handle<EcalRecHitCollection> recHitsEB, Handle<EcalRecHitCollection> recHitsEE, Handle<reco::TrackCollection> tracks, vector<const reco::Photon*>& selectedPhotons ) {

   int k= 0 ;
   double maxPt = 0 ;
   int  nSCreject  = 0 ;
  
   BCCrys spikcrys ;
   BCInfo siddBC ;

   for(reco::PhotonCollection::const_iterator it = photons->begin(); it != photons->end(); it++) {
       // fiducial cuts
       if ( k >= MAXPHO ) break ;
       if ( it->pt() < photonCuts[0] || fabs( it->eta() ) > photonCuts[1] ) continue ;
//       float hcalIsoRatio = it->hcalTowerSumEtConeDR04() / it->pt() ;
//       if  ( ( hcalIsoRatio + it->hadronicOverEm() )*it->energy() >= 6.0 ) continue ;

 
//Do NOT CHECK SPIKES NOW!
    // Check on Anomalous SC
    bool badSC = ASignalSCRejecter( it->superCluster(), recHitsEB, recHitsEE, tracks, spikcrys, siddBC ) ;
   
       if ( badSC  ) nSCreject++ ; 
//      if ( badSC  ) continue  ;    // skip Bad SCs


       // S_Minor Cuts from the seed cluster
       reco::CaloClusterPtr SCseed = it->superCluster()->seed() ;
       const EcalRecHitCollection* rechits = ( it->isEB()) ? recHitsEB.product() : recHitsEE.product() ;
       //const EBRecHitCollection* rechits = ( it->isEB()) ? recHitsEB.product() : recHitsEE.product() ;

       Cluster2ndMoments moments = EcalClusterTools::cluster2ndMoments(*SCseed, *rechits);
       float sMin =  moments.sMin  ;
       float sMaj =  moments.sMaj  ;

       // seed Time & Error
       pair<DetId, float> maxRH = EcalClusterTools::getMaximum( *SCseed, rechits );
       DetId seedCrystalId = maxRH.first;
       EcalRecHitCollection::const_iterator seedRH = rechits->find(seedCrystalId);
       float seedTime = (float)seedRH->time();
       float seedTimeErr = (float) seedRH->timeError() ;
    
       if ( sMaj > photonCuts[2] ) continue ;
       if ( sMin <= photonCuts[3] || sMin >= photonCuts[4] ) continue ;

       // Isolation Cuts 
       float ecalSumEt = it->ecalRecHitSumEtConeDR04();
       float hcalSumEt = it->hcalTowerSumEtConeDR04();
       float trkSumPt  = it->trkSumPtSolidConeDR04(); 

       // 2012 New HcalIso which is  PU independent   
       float hcalIsoConeDR04_2012 = it->hcalTowerSumEtConeDR04() + (it->hadronicOverEm() - it->hadTowOverEm())*it->superCluster()->energy()/cosh(it->superCluster()->etaWidth());

//       float HtrkSumPt = it->trkSumPtHollowConeDR04();      // for TightPhoton Selection. 

//       bool trkIso  = ( ( trkSumPt / it->pt())     < photonIso[0] ) ; 
//       bool ecalIso = ( (ecalSumEt / it->energy()) < photonIso[2] && ecalSumEt < photonIso[1] ) ; 
//       bool hcalIso = ( (hcalSumEt / it->energy()) < photonIso[4] && hcalSumEt < photonIso[3] ) ; 
//       if ( !trkIso || !ecalIso || !hcalIso ) continue ;

          
// VeryLoose(VL) IsoLation Cuts
       bool trkIso  = ( trkSumPt  < photonIso[3] +   0.002*it->pt() );
       bool ecalIso = ( ecalSumEt < photonIso[1] +   0.012*it->et() );
       bool hcalIso = ( hcalSumEt < photonIso[3] +   0.005*it->et() );
       if (! trkIso || !ecalIso || !hcalIso) continue ;


// Track Veto-ing
      int nTrk = 0 ;
       double minDR = 99. ;
  //     double trkpt = 0 ;
       for (reco::TrackCollection::const_iterator itrk = tracks->begin(); itrk != tracks->end(); itrk++ )  {
           if ( itrk->pt() < 3. ) continue ;
	   LorentzVector trkP4( itrk->px(), itrk->py(), itrk->pz(), itrk->p() ) ;
	   double dR =  ROOT::Math::VectorUtil::DeltaR( trkP4 , it->p4()  ) ;
           if ( dR < minDR ) {
              minDR = dR ;
    //          trkpt = itrk->pt() ;
           }
	   if ( dR < photonCuts[5] )  nTrk++ ;
       }
       if ( nTrk > 0 ) continue ;

 // Check Leading Photon pt
     maxPt = (it->pt() > maxPt ) ? it->pt() :maxPt ;

   
///   Electron Veto-ing
   if (it->hasPixelSeed()) continue ;   // Electron Veto

   if (it->isEBEEGap() )  continue ; // Reject Photons in EB-EE Gap 

 //      if ( it->sigmaIetaIeta() > 0.013) continue ;  // Calorimeter information Tight EB Photon 0.030 for EE
 //      if ( it->hasMatchedPromptElectron())  continue ;  // check for conversion.
 //
 //     if ( it->r9() )  continue ;                      // Seperates unconverted Gamma from Converted Gamma



       // Timing Calculation
       pair<double,double> AveXtalTE =  ClusterTime( it->superCluster(), recHitsEB , recHitsEE );

       PhoInfo phoTmp ;
       phoTmp.t      = AveXtalTE.first ;
       phoTmp.dt     = AveXtalTE.second ;
       phoTmp.nchi2  = 0 ;
       phoTmp.nxtals = 0 ;
       phoTmp.nBC    = 0 ;
       phoTmp.fSpike = -1 ;
       phoTmp.maxSX  = -1 ;
       //cout<<" 1st xT : "<< aveXtalTime <<"  xTE : "<< aveXtalTimeErr << endl;
       // Only use the seed cluster
       ClusterTime( it->superCluster(), recHitsEB , recHitsEE, phoTmp );
       //cout<<" 2nd xT : "<< aveXtalTime <<"  xTE : "<< aveXtalTimeErr << endl;
       leaves.aveTime1[k]     = phoTmp.t ;    // weighted ave. time of seed cluster
       leaves.aveTimeErr1[k]  = phoTmp.dt ;
       leaves.timeChi2[k]     = phoTmp.nchi2 ;
       leaves.nXtals[k]       = phoTmp.nxtals ;
       leaves.nBC[k]          = phoTmp.nBC ;
       leaves.fSpike[k]       = phoTmp.fSpike ;
       leaves.maxSwissX[k]    = phoTmp.maxSX ; 

       // refine the timing 
       ClusterTime( it->superCluster(), recHitsEB , recHitsEE, phoTmp );
       //cout<<" 3rd xT : "<< aveXtalTime <<"  xTE : "<< aveXtalTimeErr << endl;



       leaves.phoPx[k] = it->p4().Px() ;
       leaves.phoPy[k] = it->p4().Py() ;
       leaves.phoPz[k] = it->p4().Pz() ;
       leaves.phoVx[k] = it->p4().x();
       leaves.phoVy[k] = it->p4().y();
       leaves.phoVz[k] = it->p4().z();

       leaves.phoE[k]  = it->p4().E() ;
 //    leaves.phoHoverE[k]  = it->hadronicOverEm() ;
       leaves.phoHoverE[k]  = it->hadTowOverEm() ;   // @@ la new Def of H/E for ele/Pho ID in 2012 in CMSSW5XY
       leaves.phoEcalIso[k] = ecalSumEt ;
       leaves.phoHcalIso[k] = hcalSumEt ;
       leaves.phoTrkIso[k]  = trkSumPt ;
       
       leaves.phoEta[k] = it->eta();
       leaves.phoPhi[k] = it->phi();
       leaves.sMinPho[k] = sMin ;
       leaves.sMajPho[k] = sMaj ;
       leaves.phoTime[k] = seedTime ;

      // New 2012 HcalIso
       leaves.HcalIso[k] = hcalIsoConeDR04_2012 ; 
       
       leaves.seedTime[k]     = seedTime ;
       leaves.seedTimeErr[k]  = seedTimeErr ;
       leaves.aveTime[k]      = phoTmp.t ;       // weighted ave. time of all clusters
       leaves.aveTimeErr[k]   = phoTmp.dt ;
      

 
       selectedPhotons.push_back( &(*it) ) ;
       k++ ;
   }
   leaves.nPhotons = k ;
   //leaves.nPhotons = (int)( selectedPhotons.size() ) ;

   if ( selectedPhotons.size() > 0 && maxPt >= photonCuts[7] )  return true ; 
   else                               return false ;    

}

///********************************* Fxn To Calculate  ClusterTime i.e Weighted Ave Time ****************************////
// return time, timeError
pair<double,double> DPntumakerAlyzer::ClusterTime( reco::SuperClusterRef scRef, Handle<EcalRecHitCollection> recHitsEB, Handle<EcalRecHitCollection> recHitsEE ) {

  const EcalIntercalibConstantMap& icalMap = ical->getMap();
  float adcToGeV = float(agc->getEBValue());

  double xtime = 0 ;
  double xtimeErr = 0 ;

  // 1. loop all the basic clusters 
  for ( reco::CaloCluster_iterator  clus = scRef->clustersBegin() ;  clus != scRef->clustersEnd();  ++clus) {

      // only use seed basic cluster  
      if ( *clus != scRef->seed() ) continue ;
      // GFdoc clusterDetIds holds crystals that participate to this basic cluster 
      // 2. loop on xtals in cluster
      std::vector<std::pair<DetId, float> > clusterDetIds = (*clus)->hitsAndFractions() ; //get these from the cluster
      //cout<<" --------------- "<<endl ;
      int nXtl = 0 ;
      for (std::vector<std::pair<DetId, float> >::const_iterator detitr = clusterDetIds.begin () ; 
           detitr != clusterDetIds.end () ; ++detitr) { 

             // Here I use the "find" on a recHit collection... I have been warned...   (GFdoc: ??)
   	     // GFdoc: check if DetId belongs to ECAL; if so, find it among those if this basic cluster
    	     if ( (detitr -> first).det () != DetId::Ecal)  { 
   	          cout << " det is " << (detitr -> first).det () << " (and not DetId::Ecal)" << endl ;
	          continue ;
	     }
             bool isEB = ( (detitr -> first).subdetId () == EcalBarrel)  ? true : false ;
	   
	     // GFdoc now find it!
	     EcalRecHitCollection::const_iterator thishit = (isEB) ? recHitsEB->find( (detitr->first) ) : recHitsEE->find( (detitr->first) );
	     if (thishit == recHitsEB->end () &&  isEB )  continue ;
	     if (thishit == recHitsEE->end () && !isEB )  continue ;

	     // GFdoc this is one crystal in the basic cluster
	     EcalRecHit myhit = (*thishit) ;
	   
             // SIC Feb 14 2011 -- Add check on RecHit flags (takes care of spike cleaning in 42X)
             if ( !( myhit.checkFlag(EcalRecHit::kGood) || myhit.checkFlag(EcalRecHit::kOutOfTime) || 
                    myhit.checkFlag(EcalRecHit::kPoorCalib)  ) )  continue;

             // swiss cross cleaning 
             //float swissX = (isEB) ? EcalTools::swissCross(detitr->first, *recHitsEB , 0., true ) : 
             //                        EcalTools::swissCross(detitr->first, *recHitsEE , 0., true ) ;

             //if ( swissX > 0.95 ) { 
             //if ( myhit.checkFlag(EcalRecHit::kWeird) || myhit.checkFlag(EcalRecHit::kDiWeird) ) {
                //cout<<" swissX = "<< swissX <<" @ "<< nXtl <<endl ;
                //continue ;
             //}
             nXtl++ ;

             // thisamp is the EB amplitude of the current rechit
	     double thisamp  = myhit.energy () ;
	   
	     EcalIntercalibConstantMap::const_iterator icalit = icalMap.find(detitr->first);
	     EcalIntercalibConstant icalconst = 1;
	     if( icalit!=icalMap.end() ) {
	       icalconst = (*icalit);
	     } else {
	       edm::LogError("EcalTimePhyTreeMaker") << "No intercalib const found for xtal " << (detitr->first).rawId();
   	     }
	   
	     // get laser coefficient
	     float lasercalib = laser->getLaserCorrection( detitr->first, eventTime );

	     // discard rechits with A/sigma < 12
	     if ( thisamp/(icalconst*lasercalib*adcToGeV) < (1.1*12) ) continue;

	     GlobalPoint pos = theGeometry->getPosition((myhit).detid());

             // time and time correction
	     double thistime = myhit.time();
	     thistime += theTimeCorrector_.getCorrection((float) thisamp/(icalconst*lasercalib*adcToGeV), pos.eta()  );

             // get time error 
             double xtimeErr_ = ( myhit.isTimeErrorValid() ) ?  myhit.timeError() : 999999 ;
 
             xtime     += thistime / pow( xtimeErr_ , 2 ) ;
             xtimeErr  += 1/ pow( xtimeErr_ , 2 ) ;
      }
      //cout<<" total Xtl = " << nXtl << endl ;
  }
  double wAveTime = xtime / xtimeErr ;
  double wAveTimeErr = 1. / sqrt( xtimeErr) ;
  pair<double, double> wAveTE( wAveTime, wAveTimeErr ) ;
  return wAveTE ;  

}


//************************* GF Chi2 Time method: Fxn to Calculate this Time *************************///

// re-calculate time and timeError as well as normalized chi2
//void DPAnalysis::ClusterTime( reco::SuperClusterRef scRef, Handle<EcalRecHitCollection> recHitsEB, Handle<EcalRecHitCollection> recHitsEE, double& aveTime, double& aveTimeErr, double& nChi2, bool useAllClusters ) {

void DPntumakerAlyzer::ClusterTime( reco::SuperClusterRef scRef, Handle<EcalRecHitCollection> recHitsEB, Handle<EcalRecHitCollection> recHitsEE, PhoInfo& phoTmp, bool useAllClusters ) {

  const EcalIntercalibConstantMap& icalMap = ical->getMap();
  float adcToGeV = float(agc->getEBValue());

  double xtime    = 0 ;
  double xtimeErr = 0 ;
  double chi2_bc  = 0 ;
  double ndof     = 0 ;
  double maxSwissX = 0 ;
  int    nBC      = 0 ;
  int    nXtl     = 0 ;
  int    nSpike   = 0 ; 
  int    nSeedXtl = 0 ;
  for ( reco::CaloCluster_iterator  clus = scRef->clustersBegin() ;  clus != scRef->clustersEnd();  ++clus) {

      nBC++ ;
      // only use seed basic cluster  
      bool isSeed = ( *clus == scRef->seed() ) ;
      if ( *clus != scRef->seed() && !useAllClusters ) continue ;

      // GFdoc clusterDetIds holds crystals that participate to this basic cluster 
      //loop on xtals in cluster
      std::vector<std::pair<DetId, float> > clusterDetIds = (*clus)->hitsAndFractions() ; //get these from the cluster
      for (std::vector<std::pair<DetId, float> >::const_iterator detitr = clusterDetIds.begin () ; 
           detitr != clusterDetIds.end () ; ++detitr) { 
	      // Here I use the "find" on a recHit collection... I have been warned...   (GFdoc: ??)
   	      // GFdoc: check if DetId belongs to ECAL; if so, find it among those if this basic cluster
    	     if ( (detitr -> first).det () != DetId::Ecal)  { 
   	          cout << " det is " << (detitr -> first).det () << " (and not DetId::Ecal)" << endl ;
	          continue ;
	     }
             bool isEB = ( (detitr -> first).subdetId () == EcalBarrel)  ? true : false ;
	   
	     // GFdoc now find it!
	     EcalRecHitCollection::const_iterator thishit = (isEB) ? recHitsEB->find( (detitr->first) ) : recHitsEE->find( (detitr->first) );
	     if (thishit == recHitsEB->end () &&  isEB )  continue ;
	     if (thishit == recHitsEE->end () && !isEB )  continue ;

	     // GFdoc this is one crystal in the basic cluster
	     EcalRecHit myhit = (*thishit) ;
	   
             // SIC Feb 14 2011 -- Add check on RecHit flags (takes care of spike cleaning in 42X)
             if ( !( myhit.checkFlag(EcalRecHit::kGood) || myhit.checkFlag(EcalRecHit::kOutOfTime) || 
                    myhit.checkFlag(EcalRecHit::kPoorCalib)  ) )  continue;

             //if ( myhit.checkFlag(EcalRecHit::kWeird) || myhit.checkFlag(EcalRecHit::kDiWeird) ) continue ;
             bool gotSpike = ( myhit.checkFlag(EcalRecHit::kWeird) || myhit.checkFlag(EcalRecHit::kDiWeird) )  ;

             // swiss cross cleaning 
             float swissX = (isEB) ? EcalTools::swissCross(detitr->first, *recHitsEB , 0., true ) : 
                                     EcalTools::swissCross(detitr->first, *recHitsEE , 0., true ) ;
             maxSwissX = ( isSeed && swissX  > maxSwissX ) ? swissX : maxSwissX ;
             if ( gotSpike && isSeed ) nSpike++  ;
             if ( isSeed             ) nSeedXtl++  ;
             //if ( gotSpike ) continue ;

             // thisamp is the EB amplitude of the current rechit
	     double thisamp  = myhit.energy () ;
	   
	     EcalIntercalibConstantMap::const_iterator icalit = icalMap.find(detitr->first);
	     EcalIntercalibConstant icalconst = 1;
	     if( icalit!=icalMap.end() ) {
	       icalconst = (*icalit);
	     } else {
	       edm::LogError("DPntumakerAlyzer") << "No intercalib const found for xtal " << (detitr->first).rawId();
   	     }
	   
	     // get laser coefficient
	     float lasercalib = laser->getLaserCorrection( detitr->first, eventTime );

	     // discard rechits with A/sigma < 12
	     if ( thisamp/(icalconst*lasercalib*adcToGeV) < (1.1*12) ) continue;

	     GlobalPoint pos = theGeometry->getPosition((myhit).detid());

             // time and time correction
	     double thistime = myhit.time();
	     thistime += theTimeCorrector_.getCorrection((float) thisamp/(icalconst*lasercalib*adcToGeV), pos.eta()  );

             // get time error 
             double xtimeErr_ = ( myhit.isTimeErrorValid() ) ?  myhit.timeError() : 999999 ;

             // calculate chi2 for the BC of the seed
             double chi2_x = pow( ( (thistime - phoTmp.t) / xtimeErr_ ) , 2 ) ;  // bug reported by SC
             chi2_bc += chi2_x ;
             ndof += 1 ;
             // remove un-qualified hits 
             if ( fabs ( thistime - phoTmp.t ) > 3.*phoTmp.dt ) continue ;
 
             xtime     += thistime / pow( xtimeErr_ , 2 ) ;
             xtimeErr  += 1/ pow( xtimeErr_ , 2 ) ;
             nXtl++ ;
      }
  }

  //cout<<" nSpike = "<<  nSpike <<" nXtl = "<< nSeedXtl <<"  maxSwissX = "<< maxSwissX  << endl ;
  // update ave. time and error
  phoTmp.t     = xtime / xtimeErr ;
  phoTmp.dt    = 1. / sqrt( xtimeErr) ;
  phoTmp.nchi2 = ( ndof != 0 ) ? chi2_bc / ndof : 9999999 ;     
  phoTmp.fSpike = ( nSeedXtl > 0 ) ? (nSpike*1.) / (nSeedXtl*1.) : -1 ;
  phoTmp.nxtals = nXtl ;
  phoTmp.nBC    = nBC ;
  phoTmp.maxSX  = maxSwissX ;

}




//***************************** JetSelection fxn *******************************//////
bool DPntumakerAlyzer::JetSelection( Handle<reco::PFJetCollection> jets, vector<const reco::Photon*>& selectedPhotons, 
                               vector<const reco::PFJet*>& selectedJets) {

   int k = 0 ;
   for(reco::PFJetCollection::const_iterator it = jets->begin(); it != jets->end(); it++) {
       // fiducial cuts
       if ( it->pt() < jetCuts[0] || fabs( it->eta() ) > jetCuts[1] ) continue ;
// Remove Jets ID for Now!
       // Jet ID cuts      
 /*      if ( it->photonEnergyFraction() >= 0.95) continue; // Veto Jets EVent with High EM(photon) Energy Fraction in Jet Energy
       if ( it->photonMultiplicity() >= 2 ) continue ; // veto Pi-zero ->gammagamma Objects 
       if ( it->numberOfDaughters() <= 1 )               continue ;
       if ( it->chargedEmEnergyFraction() >=0.99 )     continue ;
       if ( it->neutralHadronEnergyFraction() >= 0.95 ) continue ;  // Leave it at Medium!
       if ( it->neutralEmEnergyFraction() >= 0.95 )     continue ;
       if ( fabs( it->eta() ) < 2.4 && it->chargedHadronEnergyFraction() <= 0 ) continue ;
       if ( fabs( it->eta() ) < 2.4 && it->chargedMultiplicity() <= 0 ) continue ;
       if ( fabs( it->eta() ) < 2.4 && it->chargedEmEnergyFraction() >= 0.99) continue ;
   */    // dR cuts 
       double dR = 999 ;
       for (size_t j=0; j < selectedPhotons.size(); j++ ) {
           double dR_ =  ROOT::Math::VectorUtil::DeltaR( it->p4(), selectedPhotons[j]->p4() ) ;
           if ( dR_ < dR ) dR = dR_ ;
       }
       if ( dR <= jetCuts[2] ) continue ;

       if ( k >= MAXJET ) break ;
       selectedJets.push_back( &(*it) ) ;
       leaves.jetPx[k] = it->p4().Px() ;
       leaves.jetPy[k] = it->p4().Py() ;
       leaves.jetPz[k] = it->p4().Pz() ;
       leaves.jetE[k]  = it->p4().E()  ;
       leaves.jetNDau[k] = it->numberOfDaughters() ;
       leaves.jetCM[k]   = it->chargedMultiplicity() ;
       leaves.jetCEF[k]  = it->chargedEmEnergyFraction() ;
       leaves.jetNHF[k]  = it->neutralHadronEnergyFraction() ;  
       leaves.jetNEF[k]  = it->neutralEmEnergyFraction() ;
       leaves.jetphoEF[k] = it->photonEnergyFraction();
       k++ ;
   }
   leaves.nJets = (int)( selectedJets.size() ) ;

//Cuts on the number of Jets in Each Event   
//   if ( selectedJets.size() > 0 )  return true ;
   if (selectedJets.size() > 0 )   return true;  // Select events with 0<=Njets < 99 
   else                            return false ;    

}


//********************* Electron Selection ******************************//////
bool DPntumakerAlyzer::ElectronSelection( Handle<reco::GsfElectronCollection> electrons, 
                                    vector<const reco::GsfElectron*>& selectedElectrons ) {

   // Electron Identification Based on Simple Cuts
   // https://twiki.cern.ch/twiki/bin/view/CMS/SimpleCutBasedEleID#Selections_and_How_to_use_them

   float eidx = 11. ;
   int k = 0 ;
   for(reco::GsfElectronCollection::const_iterator it = electrons->begin(); it != electrons->end(); it++) {
       if ( it->pt() < electronCuts[0] || fabs( it->eta() ) > electronCuts[1] ) continue ;
       // Isolation Cuts
       float ecalSumEt = ( it->isEB() ) ? max(0., it->dr03EcalRecHitSumEt() - 1. ) : it->dr03EcalRecHitSumEt();
       float hcalSumEt = it->dr03HcalTowerSumEt();
       float trkSumPt  = it->dr03TkSumPt();  
       double relIso   = (ecalSumEt + hcalSumEt + trkSumPt) / it->pt() ;

       if ( relIso > electronCuts[2] &&  it->isEB() ) continue ;
       if ( relIso > electronCuts[3] && !it->isEB() ) continue ;

       double nLost = it->gsfTrack()->trackerExpectedHitsInner().numberOfLostHits() ;
       if ( nLost >= electronCuts[4]  ) continue ;
       eidx += 0.1 ;
       //if ( !it->superCluster().isNull() ) sclist.push_back( make_pair(it->superCluster(), eidx ) );
       if ( k >= MAXELE ) break ;
       selectedElectrons.push_back( &(*it) ) ;
       leaves.elePx[k] = it->p4().Px() ;
       leaves.elePy[k] = it->p4().Py() ;
       leaves.elePz[k] = it->p4().Pz() ;
       leaves.eleE[k]  = it->p4().E() ;
       leaves.eleEcalIso[k] = ecalSumEt ;
       leaves.eleHcalIso[k] = hcalSumEt ;
       leaves.eleTrkIso[k]  = trkSumPt ;
       leaves.eleNLostHits[k]  = nLost ;
       k++;
   }
   leaves.nElectrons = (int)( selectedElectrons.size() ) ;

   if ( selectedElectrons.size() > 0 )  return true ; 
   else                                 return false ;    

}


//*****************Muon Selection fxn *******************************//
bool DPntumakerAlyzer::MuonSelection( Handle<reco::MuonCollection> muons, vector<const reco::Muon*>& selectedMuons ) {

//   float midx = 13.0 ;
   int k = 0;
   for(reco::MuonCollection::const_iterator it = muons->begin(); it != muons->end(); it++) {
       if ( it->pt() < muonCuts[0] || fabs( it->eta() ) > muonCuts[1] ) continue ;
       // Isolation for PAT muon
       //double relIso =  ( it->chargedHadronIso()+ it->neutralHadronIso() + it->photonIso () ) / it->pt();
       // Isolation for RECO muon
       double relIso =0. ;
       if ( it->isIsolationValid() ) {
	 relIso = ( it->isolationR05().emEt + it->isolationR05().hadEt + it->isolationR05().sumPt ) / it->pt();
       }
       if ( relIso > muonCuts[2] ) continue ;
 /*      double dR = 999. ;
       for (size_t j=0; j < selectedJets.size(); j++ ) {
           double dR_ =  ROOT::Math::VectorUtil::DeltaR( it->p4(), selectedJets[j]->p4() ) ; 
           if ( dR_ < dR ) dR = dR_ ;
       }
       if ( dR <= muonCuts[4] ) continue ;
       midx += 0.1 ;
 */      //if ( !it->superCluster().isNull() ) sclist.push_back( make_pair(it->superCluster(), midx ) );
    
       if ( k >= MAXMU ) break ;
       selectedMuons.push_back( &(*it) ) ;
       leaves.muPx[k] = it->p4().Px() ;
       leaves.muPy[k] = it->p4().Py() ;
       leaves.muPz[k] = it->p4().Pz() ;
       leaves.muE[k]  = it->p4().E() ;
       k++ ;
   }
   leaves.nMuons = (int)( selectedMuons.size() ) ;

   if ( selectedMuons.size() > 0 )  return true ; 
   else                             return false ;    

}

//**********************sMinorSelection fxn*********************************////
bool DPntumakerAlyzer::sMinorSelection( vector<const reco::Photon*>& selectedPhotons,  Handle<EcalRecHitCollection> recHitsEB, 
                                  Handle<EcalRecHitCollection> recHitsEE ) {

    // sMinor and sMajor are from 
    // CMSSW/JetMETCorrections/GammaJet/src/GammaJetAnalyzer.cc
    
    vector<float> sMinV ;
 
    size_t sz = selectedPhotons.size() ;
    for ( size_t i=0; i < selectedPhotons.size(); i++ ) {

        // S_Minor Cuts from the seed cluster
        reco::CaloClusterPtr SCseed = selectedPhotons[i]->superCluster()->seed() ;
        const EcalRecHitCollection* rechits = ( selectedPhotons[i]->isEB()) ? recHitsEB.product() : recHitsEE.product() ;
        Cluster2ndMoments moments = EcalClusterTools::cluster2ndMoments(*SCseed, *rechits);
        float sMin =  moments.sMin  ;
        //float sMaj =  moments.sMaj  ;

        // seed Time 
 //       pair<DetId, float> maxRH = EcalClusterTools::getMaximum( *SCseed, rechits );
 //       DetId seedCrystalId = maxRH.first;
 //       EcalRecHitCollection::const_iterator seedRH = rechits->find(seedCrystalId);
 //       float seedTime = (float)seedRH->time();

        //if ( sMin < 0.  ) selectedPhotons.erase( selectedPhotons.begin() + i ) ;
        if ( sMin <= photonCuts[3] || sMin >= photonCuts[4] ) selectedPhotons.erase( selectedPhotons.begin() + i ) ;
        sMinV.push_back( sMin );
    }
    if ( sMinV.size() > 0 ) sMin_ = sMinV[0] ; 
    
    if ( sz != selectedPhotons.size() ) return true ;
    else                                return false ;
}


//********************IsoPhotonSelection **************************///
bool DPntumakerAlyzer::IsoPhotonSelection( vector<const reco::Photon*>& selectedPhotons ) {

    // Another photon Isolation also can be done by using the EgammaIsolationAlogs
    // http://cmssw.cvs.cern.ch/cgi-bin/cmssw.cgi/CMSSW/RecoEgamma/EgammaIsolationAlgos/interface/

    size_t sz = selectedPhotons.size() ;
    for ( size_t i=0; i < selectedPhotons.size(); i++ ) {

        if ( fabs( selectedPhotons[i]->eta() ) > 1.3 ) {
           selectedPhotons.erase( selectedPhotons.begin() + i ) ;
           continue ;
        }
        // Isolation Cuts 
        float ecalSumEt = selectedPhotons[i]->ecalRecHitSumEtConeDR04();
	float hcalSumEt = selectedPhotons[i]->hcalTowerSumEtConeDR04();
	float trkSumPt  = selectedPhotons[i]->trkSumPtSolidConeDR04(); 

 
	bool trkIso  = ( ( trkSumPt / selectedPhotons[i]->pt())     < photonIso[0] ) ; 
	bool ecalIso = ( (ecalSumEt / selectedPhotons[i]->energy()) < photonIso[2] && ecalSumEt < photonIso[1] ) ; 
	bool hcalIso = ( (hcalSumEt / selectedPhotons[i]->energy()) < photonIso[4] && hcalSumEt < photonIso[3] ) ; 
	if ( !trkIso || !ecalIso || !hcalIso ) selectedPhotons.erase( selectedPhotons.begin() + i ) ;
    }

    if ( sz != selectedPhotons.size() ) return true ;
    else                                return false ;

}


//*********************GammaJetVeto fxn********************************//
bool DPntumakerAlyzer::GammaJetVeto( vector<const reco::Photon*>& selectedPhotons, vector<const reco::PFJet*>& selectedJets) {

     bool isGammaJets = false ;
     
     if (  selectedJets.size() > 0 && selectedPhotons.size() > 0  ) {
       double dR      = ROOT::Math::VectorUtil::DeltaR( selectedJets[0]->p4(), selectedPhotons[0]->p4() ) ;
       double PtRatio = selectedJets[0]->pt() / selectedPhotons[0]->pt() ;
       if ( dR > (2.*3.1416/3.) && PtRatio > 0.7 && PtRatio < 1.3 )  isGammaJets = true ;
     }
     return isGammaJets ;
}


//define this as a plug-in
DEFINE_FWK_MODULE(DPntumakerAlyzer);
