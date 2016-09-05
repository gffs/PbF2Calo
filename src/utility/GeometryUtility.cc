#include "GeometryUtility.h"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4PVPlacement.hh"


std::unordered_set<G4PVPlacement*>
PlaceAroundRing(
    const G4RotationMatrix& rot,
    const G4ThreeVector& tran,
    const std::unordered_set<unsigned int> arc_set,
    G4LogicalVolume* currentLV,
    G4LogicalVolume* parentLV
)
{
    std::unordered_set<G4PVPlacement*> res;

    for (auto arc: arc_set) {
        //preserve the rotation with respect to the radial direction
        G4RotationMatrix r(rot);
        r.rotateZ(arc * 30 * deg);
        G4ThreeVector t(tran);
        t.rotateZ((360 - arc * 30) * deg);

        res.emplace(new G4PVPlacement(
            new G4RotationMatrix(r), t,
            currentLV, currentLV->GetName(),
            parentLV, false, 0, true
        ));
    }

    return res;
}

std::unordered_set<G4PVPlacement*>
PlaceIntoArcs(
    const G4RotationMatrix& rot,
    const G4ThreeVector& tran,
    const std::unordered_set<unsigned int> arc_set,
    G4LogicalVolume* currentLV
)
{
    std::unordered_set<G4PVPlacement*> res;

    for (auto arc: arc_set) {

        G4LogicalVolume* parentLV;
        parentLV = G4LogicalVolumeStore::GetInstance()->
                    GetVolume("Arc" + std::to_string(arc));
        if (parentLV == 0) { continue; }

        res.emplace(new G4PVPlacement(
            new G4RotationMatrix(rot), tran,
            currentLV, currentLV->GetName(),
            parentLV, false, 0, true
        ));
    }

    return res;
}
