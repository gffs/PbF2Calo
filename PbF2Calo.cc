#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "Randomize.hh"

#include "DetectorConstruction.h"
#include "PhysicsList.h"
#include "ActionInitialization.h"

#include "SteppingVerbose.h"

#ifdef G4VIS_USE
 #include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "TROOT.h"
#include "TThread.h"
#include <ctime>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
int main(int argc,char** argv) {

    //initialize ROOT before we spawn threads
    ROOT::GetROOT();
    TThread::Initialize();

    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    long seeds[2];
    seeds[0] =  (long) time(NULL);
    seeds[1] = (long) (seeds[0] * G4UniformRand());
    CLHEP::HepRandom::setTheSeeds(seeds);

  //my Verbose output class
  G4VSteppingVerbose::SetInstance(new SteppingVerbose);
    
    G4MTRunManager* runManager = new G4MTRunManager;

    DetectorConstruction* det;
    runManager->SetUserInitialization(det = new DetectorConstruction);
    runManager->SetUserInitialization(new PhysicsList);
    runManager->SetUserInitialization(new ActionInitialization);

 
  // Start execution
  //      
  if (argc > 1) {        // execute an argument macro file if exist
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    G4UImanager::GetUIpointer()->ApplyCommand(command+fileName);
    
  } else {                // start interactive session
#ifdef G4VIS_USE
   G4VisManager* visManager = new G4VisExecutive;
   visManager->Initialize();
#endif    
     
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);      
      ui->SessionStart();
      delete ui;
#endif
          
#ifdef G4VIS_USE
     delete visManager;
#endif     
  }

    delete runManager;

    return 0;
}

