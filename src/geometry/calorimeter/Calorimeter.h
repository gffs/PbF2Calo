#pragma once

#include "functional"
#include "GeometryBase.h"
#include "string"
#include "json11.hpp"

class G4LogicalVolume;

class Calorimeter: public GeometryBase
{
public:
    Calorimeter(const json11::Json& cfg);

private:
    void Enclosure();
    void FrontPlate();
    void SipmPlate();
    void CrystalArray();

    const std::string parentName;
    const double innerClearance;

    //G4LogicalVolume* parentLV;
};
