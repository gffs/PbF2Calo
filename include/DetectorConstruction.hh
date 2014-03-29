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
    const G4VPhysicalVolume* GetWorld() { return fPBox; };

  private:
    G4VPhysicalVolume* fPBox;
    G4LogicalVolume* fLBox;
    G4Material* mPbF2;
    G4double fBoxSize;
};


