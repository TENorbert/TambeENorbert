#include <iostream> 
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>
#include <stdio.h>
#include <TString.h>
#include <TSystem.h>
#include <TFile.h>
#include <TTree.h>
#include <TMinuit.h>

#include "AnaInput.h"
#include "TestGen.h"

using namespace std; 

int main( int argc, const char* argv[] ) { 

  string datacardfile = ( argc > 1 ) ? argv[1] : "DataCard.txt";
  AnaInput  *Input = new AnaInput( datacardfile );
  TestGen   *tgen  = new TestGen( datacardfile );

  // method to read root files
  Input->LinkForests("DPntumakerAlyzer");
  string dataFileNames ;
  Input->GetParameters( "TheData", &dataFileNames );

  tgen->ReadTree( dataFileNames);
  //for ( size_t i =0 ; i< dataFileNames.size(); i++ ) {
  //    tgen->ReadTree( dataFileNames[i] );
  //} 

  delete tgen ;

  return 0;

}

