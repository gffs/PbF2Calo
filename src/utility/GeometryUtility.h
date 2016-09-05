#pragma once

#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "unordered_set"

class G4LogicalVolume;
class G4PVPlacement;


static const double in = 25.4 * mm;

constexpr long double operator"" _in(long double v)
{
    return v * 25.4;
}


std::unordered_set<G4PVPlacement*>
PlaceAroundRing(
    const G4RotationMatrix& rot,
    const G4ThreeVector& tran,
    const std::unordered_set<unsigned int> arc_set,
    G4LogicalVolume* currentLV,
    G4LogicalVolume* parentLV
);

std::unordered_set<G4PVPlacement*>
PlaceIntoArcs(
    const G4RotationMatrix& rot,
    const G4ThreeVector& tran,
    const std::unordered_set<unsigned int> arc_set,
    G4LogicalVolume* currentLV
);
