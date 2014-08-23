#pragma once

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    ~DetectorConstruction();
  
    virtual G4VPhysicalVolume* Construct();
    const G4VPhysicalVolume* GetWorld() { return physWorld; }

  private:

    G4VPhysicalVolume* physWorld;
    G4LogicalVolume* logicWorld;
    G4double crystalLength;
};


