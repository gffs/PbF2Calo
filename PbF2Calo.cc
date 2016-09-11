#include "ActionInitialization.h"
#include "DetectorConstruction.h"
#include "G4MTRunManager.hh"
#include "G4UImanager.hh"
#include "json11.hpp"
#include "PhysicsListBase.h"
#include "Randomize.hh"
#include "TROOT.h"
#include "TThread.h"

#include <ctime>
#include <iostream>
#include <sstream>

#include "SteppingVerbose.h"

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "usage: " << argv[0] <<
        " example.json [geant.mac]" << std::endl;
        return -1;
    }

    std::ifstream conf_f(argv[1], std::ios::in);
    std::stringstream sstr;
    sstr << conf_f.rdbuf();
    std::string json_err;
    auto conf = json11::Json::parse(sstr.str(), json_err);
    if (!json_err.empty()) {
        std::cout << "\nFailed to parse config file \"" << argv[1] <<
            "\" with error: \"" << json_err << "\"." << std::endl;
        return -1;
    }

    //initialize ROOT before we spawn threads
    ROOT::GetROOT();
    TThread::Initialize();

    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    long seeds[2];
    seeds[0] = (long) time(NULL);
    seeds[1] = (long) (seeds[0] * G4UniformRand());
    CLHEP::HepRandom::setTheSeeds(seeds);

    //my Verbose output class
    G4VSteppingVerbose::SetInstance(new SteppingVerbose);

    G4MTRunManager* runManager = new G4MTRunManager;
    runManager->SetNumberOfThreads(2);

    runManager->SetUserInitialization(new DetectorConstruction(conf["detector_construction"]));
    runManager->SetUserInitialization(PhysicsListBase::init(conf["physics_list"]));
    runManager->SetUserInitialization(new ActionInitialization(conf["action"]));

  // Start execution
  if (argc > 2) {        // execute an argument macro file if exist
    G4String command = "/control/execute ";
    G4String fileName = argv[2];
    G4UImanager::GetUIpointer()->ApplyCommand(command+fileName);

  } else {                // start interactive session
      G4UIExecutive * ui = new G4UIExecutive(argc, argv);
      ui->SessionStart();
      delete ui;
  }

    delete runManager;

    return 0;
}
