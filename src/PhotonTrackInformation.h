#pragma once

#include "G4VUserTrackInformation.hh"
#include "G4ThreeVector.hh"

class PhotonTrackInformation: public G4VUserTrackInformation
{
  public:
    PhotonTrackInformation();
    PhotonTrackInformation(const G4ThreeVector& aPos, const G4ThreeVector& aMom);
    ~PhotonTrackInformation() {}

  public:
    G4ThreeVector pos_org;
    G4ThreeVector mom_org;
};

