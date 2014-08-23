#include "DetectorConstruction.h"

#include "G4NistManager.hh"

#include "G4Material.hh"
#include "UBox.hh"
#include "G4Usolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"

#include "G4GeometryManager.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SolidStore.hh"

#include "G4UnitsTable.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

DetectorConstruction::DetectorConstruction() :
    G4VUserDetectorConstruction(),
    fPBox(0),
    fLBox(0),
    mPbF2(0),
    fBoxSize(50*cm)
{
}

DetectorConstruction::~DetectorConstruction()
{
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Material* mPb = nistManager->FindOrBuildMaterial("G4_Pb");
    G4Material* mF = nistManager->FindOrBuildMaterial("G4_F");

    mPbF2 = new G4Material("PbF2", 7.77*g/cm3, 2);
    mPbF2->AddMaterial(mPb, 0.84504);
    mPbF2->AddMaterial(mF, 0.15496);

    //stolen from artg4
    G4double wavelength[] = {1240, 800, 600, 400, 350, 300, 250, 200};
    G4double refractiveIndex[] = {1.74, 1.75, 1.76, 1.82, 1.85, 1.94, 2.02, 2.57};

    for(auto& w : wavelength) {
       w = 0.001240 * MeV / w;
    }

    G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable() ;
    table->AddProperty("RINDEX", wavelength, refractiveIndex, sizeof(wavelength) / sizeof(wavelength[0]));
    mPbF2->SetMaterialPropertiesTable(table);

    G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;

    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    UBox* sBox = new UBox("crystal", fBoxSize, fBoxSize, fBoxSize);
    G4USolid* gsBox = new G4USolid("crystal", sBox);
    fLBox = new G4LogicalVolume(gsBox, mPbF2, mPbF2->GetName());
    fPBox = new G4PVPlacement(0, G4ThreeVector(), fLBox, mPbF2->GetName(), 0, false, 0);
    return fPBox;
}

