#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "json11.hpp"
#include <unordered_set>

class VUSolid;
class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction(const json11::Json cfg);
    DetectorConstruction(): DetectorConstruction(json11::Json()) {}
    ~DetectorConstruction();
  
    virtual G4VPhysicalVolume* Construct();
    const G4VPhysicalVolume* GetWorld() { return physWorld; }

  private:
    G4VPhysicalVolume* physWorld;
    G4LogicalVolume* logicWorld;
    G4double crystalLength;
    std::unordered_set<VUSolid*> usolidStore;

    json11::Json cfg_;
};


