#include "PhotonTrackInformation.h"


PhotonTrackInformation::PhotonTrackInformation():
    PhotonTrackInformation({0,0,0}, {0,0,0}) {}

PhotonTrackInformation::PhotonTrackInformation(
        const G4ThreeVector& aPos, const G4ThreeVector& aMom):
    G4VUserTrackInformation(),
    pos_org(aPos),
    mom_org(aMom)
{}

