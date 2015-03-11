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
// make Output Root file
TFile *f = new TFile("MCgmsbctau250lambda100.root","recreate");
// Initialize Histograms:

   TH1D* h_Time;
   TH1D* h_g1Pt ;

   TH1D* evt_met ;
   TH1D* ph_smin ;
   TH1D* ph_smaj; 
   TH1D* ph_e ;
   TH1D* ph_z ;
   TH1D* ph_eta ;
   TH1D* ph_phi;
   TH1D* jet_eta;
   TH1D* jet_phi;
   TH1D* jp_deta;
   TH1D* jp_dphi;
   TH1D* njets ;
   TH1D* npho ;
   TH1D* j_pt ;
   TH1D* ph_HoverE ;
   TH1D* mettheta ;
   TH1D* phomet_thetadiff;
   TH1D* MassNt;  
   TH1D* pho_angle;






// Fxn to write hist!
void Writehists()

{

f->cd();
TDirectory* neut = f->mkdir("NeutKin");
neut->cd();

h_g1Pt->GetXaxis()->SetTitle("Photon Pt(GeV)");
h_g1Pt->GetYaxis()->SetTitle("Event Number");
h_g1Pt->Draw();
h_g1Pt->Write();


h_Time->GetXaxis()->SetTitle("Photon Time(ns)");
h_Time->GetYaxis()->SetTitle("Event Number");
h_Time->Draw();
h_Time->Write();

evt_met->GetXaxis()->SetTitle("Event MET(GeV)");
evt_met->GetYaxis()->SetTitle("Event Number");
evt_met->Draw();
evt_met->Write();

ph_smin->Draw();
ph_smin->Write();

ph_smaj->Draw();
ph_smaj->Write();

ph_e->GetXaxis()->SetTitle("Photon Energy(GeV)");
ph_e->GetYaxis()->SetTitle("Event Number");
ph_e->Draw();
ph_e->Write();

njets->GetXaxis()->SetTitle("Number of Jets");
njets->GetYaxis()->SetTitle("Event Number");
njets->Draw() ;
njets->Write();

npho->GetXaxis()->SetTitle("Number of #gammas");
npho->GetYaxis()->SetTitle("Event Number");
npho->Draw();
npho->Write();

j_pt->GetXaxis()->SetTitle("JetPt(GeV)");
j_pt->GetYaxis()->SetTitle("Event Number");
j_pt->Draw();
j_pt->Write();

}


// analysis template
void TestGen::ReadTree( string dataName ) { 


   TTree* tr = Input->TreeMap( dataName );

    tr->SetBranchAddress("nPhotons",    &nPhotons);
    tr->SetBranchAddress("nJets",       &nJets);

   tr->SetBranchAddress("phoPx",       phoPx );
   tr->SetBranchAddress("phoPy",       phoPy );
   tr->SetBranchAddress("phoPz",       phoPz );
   tr->SetBranchAddress("phoE",        phoE );
   tr->SetBranchAddress("phoHoverE",   phoHoverE);
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
   tr->SetBranchAddress("metPx",        &metPx);
   tr->SetBranchAddress("metPy",        &metPy);

   tr->SetBranchAddress("phoTime",     phoTime );


   int totalN = tr->GetEntries();
   cout<<" from  "<< dataName <<" total entries = "<< totalN <<" Process "<< ProcessEvents <<endl;


    h_Time  = new TH1D("h_Time", "Photon Time(ns)", 500,  -25.0, 25.0);
    h_g1Pt  = new TH1D("h_g1Pt", "Leading Photon Pt(GeV) ", 200,  0, 2000);

     evt_met = new TH1D("evt_met", "Missing Energy(GeV)", 200, 0, 2500);

    ph_smin = new TH1D("ph_smin", "Shower Width in dxn of Minor Axis of Ecal Cluster", 100, 0,5);
    ph_smaj = new TH1D("ph_smaj", "Shower Width in dxn of Major Axis of Ecal Cluster", 100,0, 5);
    ph_e = new TH1D("ph_e","Leading Photon Energy(GeV)", 200, 0, 2000);
    ph_z = new TH1D("ph_z", "Z Vextex Position of Photon(mm)", 1000, 0, 1000);
    ph_eta = new TH1D("ph_eta","Leading Photon #eta", 100, -3.0, 3.0);
    ph_phi = new TH1D("ph_phi", "Leading Photon #phi", 100, -3.142, 3.142);
    jet_eta = new TH1D("jet_eta", "Leading Jet #eta", 100, -3.0, 3.0);
    jet_phi = new TH1D("jet_phi", "Leading Jet #phi", 100, -3.142, 3.142);
    jp_deta = new TH1D("jp_deta", " Leading Jet-Photo #Delta#eta", 100, -3.0, 3.0);
    jp_dphi = new TH1D("jp_dphi", "Leading Jet-Photon #Delta#phi", 100, -10, 10);
    njets = new TH1D("njets", "Number of Jets in Event", 20, 0, 20);
    npho = new TH1D("npho", "Number of #gamma s in Event", 10, 0, 10);
    j_pt = new TH1D("j_pt", " Leading Jet Pt(GeV)", 200, 0, 2000);
    ph_HoverE = new TH1D("ph_HoverE", "Photon Hadronic Energy Over EM Energy Ratio", 5, 0.0, 5.0);
    mettheta  =  new TH1D("mettheta", " Agular Distribution of supposed Gravitino", 200, -10.0, 10.0);
    phomet_thetadiff = new TH1D("phomet_thetadiff", "Photon-Met Angular Distrubution", 200, -10.0, 10.0);
    MassNt  = new TH1D("MassNt", "Neutralino Transverse Mass Distribution(GeV)", 250, 0.0, 2500.0);
    pho_angle = new TH1D("pho_angle","Photon Angular Distribution(Radians)", 200, -10.0, 10.0);


   int nEvt = 0 ;
   for ( int i=0; i< totalN ; i++ ) {
       if ( ProcessEvents > 0 && i > ( ProcessEvents - 1 ) ) break;
       tr->GetEntry( i );

       nEvt++;
     double metratio = 0;
     if(metPx == 0){ metratio = 0;} else { metratio = metPy ? metPx/metPy : 0 ;} 
      double thetamet = TMath::ATan(metratio); // Gravitino direction for MET Vector!
      double thetapho = 0;  // Photon angle
      double phoeta = 0;    // Photon eta
      double pho_gravtheta = 0; // dxn of Grav wrt to photon
      float  massNt = 0;  // Neutralino  transverse Mass 

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
           ph_HoverE ->Fill(phoHoverE[k]);
  
       } // End of Loop over Photons in Event

       h_g1Pt->Fill( max_gPt );
       ph_e ->Fill( g1P4.E() );
       ph_z ->Fill( g1P4.Z());
       phoeta = g1P4.Eta();
       ph_eta->Fill(g1P4.Eta());
       ph_phi->Fill(g1P4.Phi());
       thetapho = 2*TMath::ATan(TMath::Exp(-phoeta));  // Photon Angle
       pho_gravtheta = thetapho - thetamet;
   //    phomet_thetadiff->Fill(g1P4.Eta() - (-TMath::Log(TMath::Tan(thetamet/2)))); 
       massNt = TMath::Sqrt(2*max_gPt*met*(1 - TMath::Cos(pho_gravtheta)) );
       phomet_thetadiff->Fill(pho_gravtheta);
    
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
 mettheta->Fill(thetamet);
 MassNt->Fill(massNt);
 pho_angle ->Fill(thetapho);
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
h_g1Pt->GetXaxis()->SetTitle("Photon Pt(GeV)");
h_g1Pt->GetYaxis()->SetTitle("Event Number");

   h_g1Pt->Draw() ;
   c1->Update();

   TString plotname1 = hfolder + "PhotonPt." +plotType ;
   c1->Print( plotname1 );
   c1->SetLogy(1);

   // Photon Time
   c1->cd() ;
   gStyle->SetStatY(0.95);
h_Time->GetXaxis()->SetTitle("Photon Time(ns)");
h_Time->GetYaxis()->SetTitle("Event Number");
   h_Time->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhotonTime." + plotType ;
   c1->Print( plotname1 );

// Event Met
   c1->cd() ;
   gStyle->SetStatY(0.95);
   c1->SetLogy(1);
evt_met->GetXaxis()->SetTitle("Event MET(GeV)");
evt_met->GetYaxis()->SetTitle("Event Number");

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
   ph_e->GetXaxis()->SetTitle("Photon Energy(GeV)");
   ph_e->GetYaxis()->SetTitle("Event Number");

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
 njets->GetXaxis()->SetTitle("Number of Jets");
 njets->GetYaxis()->SetTitle("Event Number");

   njets->Draw() ;
   c1->Update();
   plotname1 = hfolder + "NumberOfJets." + plotType ;
   c1->Print( plotname1 );

// Number of Photon Dist
   c1->cd() ;
   gStyle->SetStatY(0.95);
   npho->GetXaxis()->SetTitle("Number of #gammas");
   npho->GetYaxis()->SetTitle("Event Number");

   npho->Draw() ;
   c1->Update();
   plotname1 = hfolder + "NumberOfPhotons." + plotType ;
   c1->Print( plotname1 );

// Leading Jet Pt dist
   c1->cd() ;
   gStyle->SetStatY(0.95);
  j_pt->GetXaxis()->SetTitle("JetPt(GeV)");
  j_pt->GetYaxis()->SetTitle("Event Number");
   j_pt->Draw() ;
   c1->Update();
   plotname1 = hfolder + "JetPt." + plotType ;
   c1->Print( plotname1 );

	// Met Direction	
	c1->cd() ;
	gStyle->SetStatY(0.95);
	mettheta->Draw();
	c1->Update();
	plotname1 = hfolder + "MetTheta." + plotType ;
	c1->Print(plotname1);

	//Met-Photon Direction
   c1->cd() ;
   gStyle->SetStatY(0.95);

   phomet_thetadiff->GetXaxis()->SetTitle("Photon-Gravitino Angle Difference(radians)");
   phomet_thetadiff->GetYaxis()->SetTitle("Event Number");
   phomet_thetadiff->Draw() ;
   c1->Update();
   plotname1 = hfolder + "Met_PhotonThetaDiff." + plotType ;
   c1->Print( plotname1 );

    //Photon Hadronic Energy over Total Energy Ratio
   c1->cd() ;
   gStyle->SetStatY(0.95);
   ph_HoverE->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhotonHadOverE." + plotType ;
   c1->Print( plotname1 );

   // Transverse Mass of Neutralino
   c1->cd() ;
   gStyle->SetStatY(0.95);
   MassNt->GetXaxis()->SetTitle("M_{NLSP}(GeV)");
   MassNt->GetYaxis()->SetTitle("Event Number");
   MassNt->Draw() ;
   c1->Update();
   plotname1 = hfolder + "NeutTMass." + plotType ;
   c1->Print( plotname1 );

   // Photon Angular Dist
   c1->cd() ;
   gStyle->SetStatY(0.95);
   pho_angle->GetXaxis()->SetTitle("Photon Angle(radians)");
   pho_angle->GetYaxis()->SetTitle("Event Number");
   pho_angle->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhotonAngle." + plotType ;
   c1->Print( plotname1 );


Writehists();
f->Close();



}
