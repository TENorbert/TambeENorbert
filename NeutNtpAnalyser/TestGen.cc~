#include "TestGen.h"

TestGen::TestGen( string datacardfile ){

  Input  = new AnaInput( datacardfile );
  
  Input->GetParameters("PlotType",      &plotType ) ; 
  Input->GetParameters("Path",          &hfolder ) ; 
  Input->GetParameters("RootFiles",     &rfolder ) ; 
  Input->GetParameters("ProcessEvents", &ProcessEvents ) ; 

}

TestGen::~TestGen(){

  delete Input ;
  cout<<" done ! "<<endl ;

}

// analysis template
void TestGen::ReadTree( string dataName ) { 


   TTree* tr = Input->TreeMap( dataName );

    tr->SetBranchAddress("nPhotons",    &nPhotons);
    tr->SetBranchAddress("nJets",    &nJets);

   tr->SetBranchAddress("phoPx",       phoPx );
   tr->SetBranchAddress("phoPy",       phoPy );
   tr->SetBranchAddress("phoPz",       phoPz );
   tr->SetBranchAddress("phoE",        phoE );

   tr->SetBranchAddress("phoVx",         phoVx );
   tr->SetBranchAddress("phoVy",         phoVy );
   tr->SetBranchAddress("phoVz",         phoVz );
   tr->SetBranchAddress("phoEta",        phoEta );
   tr->SetBranchAddress("phoPhi",        phoPhi );
   tr->SetBranchAddress("sMinPho",       sMinPho );
   tr->SetBranchAddress("sMajPho",       sMajPho );
   tr->SetBranchAddress("jetE",           jetE );
   tr->SetBranchAddress("jetPx",       jetPx );
   tr->SetBranchAddress("jetPy",       jetPy );
   tr->SetBranchAddress("jetPz",       jetPz );
   tr->SetBranchAddress("met",            &met);
   tr->SetBranchAddress("phoTime",     phoTime );


   int totalN = tr->GetEntries();
   cout<<" from  "<< dataName <<" total entries = "<< totalN <<" Process "<< ProcessEvents <<endl;


   TH1D* h_Time  = new TH1D("h_Time", "Photon Time", 120,  -4.5, 25.5);
   TH1D* h_g1Pt  = new TH1D("h_g1Pt", "Leading Photon Pt ", 50,  0, 500);

   TH1D* evt_met = new TH1D("evt_met", "Missing Energy(GeV)", 50, 0, 500);

   TH1D* ph_smin = new TH1D("ph_smin", "Shower Width in dxn of Minor Axis of Ecal Cluster", 100, 0,5);
   TH1D* ph_smaj = new TH1D("ph_smaj", "Shower Width in dxn of Major Axis of Ecal Cluster", 100,0, 5);
   TH1D* ph_e = new TH1D("ph_e","Leading Photon Energy(GeV)", 50, 0, 500);
   TH1D* ph_z = new TH1D("ph_z", "Z Vextex Position of Photon(cm)", 100, 0, 100);
   TH1D* ph_eta = new TH1D("ph_eta","Leading Photon #eta", 100, -3.0, 3.0);
   TH1D* ph_phi = new TH1D("ph_phi", "Leading Photon #phi", 100, -3.142, 3.142);
   TH1D* jet_eta = new TH1D("jet_eta", "Leading Jet #eta", 100, -3.0, 3.0);
   TH1D* jet_phi = new TH1D("jet_phi", "Leading Jet #phi", 100, -3.142, 3.142);
   TH1D* jp_deta = new TH1D("jp_deta", " Leading Jet-Photo #Delta#eta", 100, -3.0, 3.0);
   TH1D* jp_dphi = new TH1D("jp_dphi", "Leading Jet-Photon #Delta#phi", 100, -10, 10);
   TH1D* njets = new TH1D("njets", "Number of Jets in Event", 20, 0, 20);
   TH1D* npho = new TH1D("npho", "Number of #gamma s in Event", 10, 0, 10);
   TH1D* j_pt = new TH1D("j_pt", " Leading Jet Pt(GeV)", 50, 0, 500);

   int nEvt = 0 ;
   for ( int i=0; i< totalN ; i++ ) {
       if ( ProcessEvents > 0 && i > ( ProcessEvents - 1 ) ) break;
       tr->GetEntry( i );

       nEvt++; 

       TLorentzVector g1P4(0,0,0,0),j1p4(0,0,0,0)  ;
       double max_gPt  = 0 ;
       for ( int k=0; k< nPhotons; k++) {
           TLorentzVector gP4_ = TLorentzVector( phoPx[k], phoPy[k], phoPz[k], phoE[k] ) ;
           //if ( nPhotons > 0 ) cout<<" photon"<<k <<" pt:"<<gP4_.Pt() <<endl;
           if ( gP4_.Pt() > max_gPt ) {
              max_gPt = gP4_.Pt() ;
              g1P4 = gP4_ ;
           } 

           h_Time->Fill( phoTime[k] );
           ph_smin->Fill(sMinPho[k] );
           ph_smaj ->Fill(sMajPho[k] );
  
       }
       h_g1Pt->Fill( max_gPt );
       ph_e ->Fill( g1P4.E() );
       ph_z ->Fill( g1P4.Z());
       ph_eta->Fill(g1P4.Eta());
       ph_phi->Fill(g1P4.Phi());
     

     double max_jPt = 0;
    for ( int j = 0; j < nJets; j++){
        TLorentzVector jp4_ = TLorentzVector(jetPx[j], jetPy[j], jetPz[j], jetE[j] ); 
       if (jp4_.Pt() > max_jPt) {
           max_jPt = jp4_.Pt();
           j1p4     = jp4_;
          }       

        }   
         j_pt ->Fill(max_jPt); // Jet With Largest Pt
         jet_eta->Fill(j1p4.Eta() );
         jet_phi->Fill(j1p4.Phi() );

 evt_met->Fill(met);
 njets ->Fill(nJets);
 npho ->Fill(nPhotons);

 jp_deta->Fill( j1p4.Eta() - g1P4.Eta() );
 jp_dphi->Fill ( j1p4.Phi() - g1P4.Phi() ) ;

   } // end of event looping


// Now Make Plots
   TCanvas*  c1 = new TCanvas("c1","", 800, 600);
   c1->SetFillColor(10);
   c1->SetFillColor(10);
   gStyle->SetOptStat("emriou");
   //c1->SetLogy();

   // Photon Pt distribution
   c1->cd();
   c1->SetLogy();
   gStyle->SetStatY(0.95);
   gStyle->SetStatTextColor(1);
   h_g1Pt->SetLineColor(1) ;
   h_g1Pt->Draw() ;
   c1->Update();

   TString plotname1 = hfolder + "PhotonPt." +plotType ;
   c1->Print( plotname1 );
   c1->SetLogy(1);

   // Photon Time
   c1->cd() ;
   gStyle->SetStatY(0.95);
   h_Time->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhoTime." + plotType ;
   c1->Print( plotname1 );

// Event Met
   c1->cd() ;
   gStyle->SetStatY(0.95);
   c1->SetLogy(1);
   evt_met->Draw() ;
   c1->Update();
   plotname1 = hfolder + "EventMet." + plotType ;
 //  c1->SetLogy(1)
   c1->Print( plotname1 );

// Photon sMinor
   c1->cd() ;
   gStyle->SetStatY(0.95);
   ph_smin->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhotonsMinor." + plotType ;
   c1->Print( plotname1 );

// Photon sMajor
   c1->cd() ;
   gStyle->SetStatY(0.95);
   ph_smaj->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhotonsMajor." + plotType ;
   c1->Print( plotname1 );

// Photon Energy dist
   c1->cd() ;
   gStyle->SetStatY(0.95);
   ph_e->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhotonEnergy." + plotType ;
   c1->Print( plotname1 );

// Photon Z Position
   c1->cd() ;
   gStyle->SetStatY(0.95);
   ph_z->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhotonZ." + plotType ;
   c1->Print( plotname1 );

// Photon Eta
   c1->cd() ;
   gStyle->SetStatY(0.95);
   ph_eta->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhotonEta." + plotType ;
   c1->Print( plotname1 );

// Photon Phi
   c1->cd() ;
   gStyle->SetStatY(0.95);
   ph_phi->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhotonPhi." + plotType ;
   c1->Print( plotname1 );

// Leading Jet Eta
   c1->cd() ;
   gStyle->SetStatY(0.95);
   jet_eta->Draw() ;
   c1->Update();
   plotname1 = hfolder + "JetEta." + plotType ;
   c1->Print( plotname1 );

// Leading Jet Phi
   c1->cd() ;
   gStyle->SetStatY(0.95);
   jet_phi->Draw() ;
   c1->Update();
   plotname1 = hfolder + "JetPhi." + plotType ;
   c1->Print( plotname1 );

// Photon - Jet Eta
   c1->cd() ;
   gStyle->SetStatY(0.95);
   jp_deta->Draw() ;
   c1->Update();
   plotname1 = hfolder + "JetPhtonDEta." + plotType ;
   c1->Print( plotname1 );

// Photon -Jet Phi
   c1->cd() ;
   gStyle->SetStatY(0.95);
   jp_dphi->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhotonJetDPhi." + plotType ;
   c1->Print( plotname1 );

// Number of Jet Dist
   c1->cd() ;
   gStyle->SetStatY(0.95);
   njets->Draw() ;
   c1->Update();
   plotname1 = hfolder + "NumberOfJet." + plotType ;
   c1->Print( plotname1 );

// Number of Photon Dist
   c1->cd() ;
   gStyle->SetStatY(0.95);
   npho->Draw() ;
   c1->Update();
   plotname1 = hfolder + "NumberOfPho" + plotType ;
   c1->Print( plotname1 );

// Leading Jet Pt dist
   c1->cd() ;
   gStyle->SetStatY(0.95);
   j_pt->Draw() ;
   c1->Update();
   plotname1 = hfolder + "JetPt." + plotType ;
   c1->Print( plotname1 );

}  

