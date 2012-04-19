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

   tr->SetBranchAddress("nNeu",        &nNeu);
   tr->SetBranchAddress("nPhotons",    &nPhotons);

   tr->SetBranchAddress("phoPx",       phoPx );
   tr->SetBranchAddress("phoPy",       phoPy );
   tr->SetBranchAddress("phoPz",       phoPz );
   tr->SetBranchAddress("phoE",        phoE );
   tr->SetBranchAddress("phoVx",       phoVx );
   tr->SetBranchAddress("phoVy",       phoVy );
   tr->SetBranchAddress("phoVz",       phoVz );
   tr->SetBranchAddress("phoMother",   phoMother );
   tr->SetBranchAddress("sTime",        sTime );

   tr->SetBranchAddress("neuPx",       neuPx );
   tr->SetBranchAddress("neuPy",       neuPy );
   tr->SetBranchAddress("neuPz",       neuPz );
   tr->SetBranchAddress("neuE",        neuE );
   tr->SetBranchAddress("neuM",        neuM );
   tr->SetBranchAddress("neuGamma",    neuGamma);
   tr->SetBranchAddress("neuctau",     neuctau);


   int totalN = tr->GetEntries();
   cout<<" from  "<< dataName <<" total entries = "<< totalN <<" Process "<< ProcessEvents <<endl;


   TH1D* h_Time  = new TH1D("h_Time", "Photon Time", 120,  -4.5, 25.5);
   TH1D* h_g1Pt  = new TH1D("h_g1Pt", "Leading Photon Pt ", 50,  0, 500);

   int nEvt = 0 ;
   for ( int i=0; i< totalN ; i++ ) {
       if ( ProcessEvents > 0 && i > ( ProcessEvents - 1 ) ) break;
       tr->GetEntry( i );

       nEvt++; 

       TLorentzVector g1P4(0,0,0,0)  ;
       double max_gPt  = 0 ;
       for ( int k=0; k< nPhotons; k++) {
           TLorentzVector gP4_ = TLorentzVector( phoPx[k], phoPy[k], phoPz[k], phoE[k] ) ;
           //if ( nPhotons > 0 ) cout<<" photon"<<k <<" pt:"<<gP4_.Pt() <<endl;
           if ( gP4_.Pt() > max_gPt ) {
              max_gPt = gP4_.Pt() ;
              g1P4 = gP4_ ;
           } 

           h_Time->Fill( sTime[k] );
       }
       h_g1Pt->Fill( max_gPt );

   } // end of event looping

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
   c1->SetLogy(0);

   // Photon Time
   c1->cd() ;
   gStyle->SetStatY(0.95);
   h_Time->Draw() ;
   c1->Update();
   plotname1 = hfolder + "PhoTime." + plotType ;
   c1->Print( plotname1 );

}  

