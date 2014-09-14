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
    crystalLength(140 * mm)
{
}

DetectorConstruction::~DetectorConstruction()
{
    delete physWorld;
    delete logicWorld;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    G4NistManager* nistManager = G4NistManager::Instance();

    G4Material* mAir = nistManager->FindOrBuildMaterial("G4_AIR");
    G4Material* mPb = nistManager->FindOrBuildMaterial("G4_Pb");
    G4Material* mF = nistManager->FindOrBuildMaterial("G4_F");
    G4Material* mAl = nistManager->FindOrBuildMaterial("G4_Al");
    G4Material* mSi = nistManager->FindOrBuildMaterial("G4_Si");

    G4Material* mPbF2 = new G4Material("PbF2", 7.77*g/cm3, 2);
    mPbF2->AddMaterial(mPb, 0.84504);
    mPbF2->AddMaterial(mF, 0.15496);

    //stolen from artg4
    G4double wavelength[] = {1240, 800, 600, 400, 350, 300, 250, 200};
    G4double refractiveIndex[] = {1.74, 1.75, 1.76, 1.82, 1.85, 1.94, 2.02, 2.57};

    for(auto& w : wavelength) {
       w = 0.001240 * MeV / w;
    }

    G4MaterialPropertiesTable* table = new G4MaterialPropertiesTable();
    table->AddProperty("RINDEX", wavelength, refractiveIndex,
            sizeof(wavelength) / sizeof(wavelength[0]));
    mPbF2->SetMaterialPropertiesTable(table);

    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    const float caloHalfWidth = 250 * mm;
    //world has to be centered at origin
    UBox* solidWorld = new UBox("World", crystalLength + 3*mm,
            caloHalfWidth, caloHalfWidth);
    logicWorld = new G4LogicalVolume(
            new G4USolid(solidWorld->GetName(), solidWorld),
            mAir, solidWorld->GetName());
    physWorld = new G4PVPlacement(0, //no rotation
            G4ThreeVector(0, 0, 0), //world has to be centered at origin
            logicWorld, logicWorld->GetName(), 0, false, 0, true);

    UBox* solidAlPlate = new UBox("AlPlate", 1.5 * mm,
            caloHalfWidth, caloHalfWidth);
    G4LogicalVolume* logicAlPlate = new G4LogicalVolume(
            new G4USolid(solidAlPlate->GetName(), solidAlPlate),
            mAl, solidAlPlate->GetName());
    new G4PVPlacement(0, G4ThreeVector(-1.5 * mm, 0, 0),
            logicAlPlate, logicAlPlate->GetName(), logicWorld, false, 0, true);
    G4Region* regionAlPlate = new G4Region(logicAlPlate->GetName());
    regionAlPlate->AddRootLogicalVolume(logicAlPlate);

    UBox* solidSiPlate = new UBox("SiPlate", 1.5 * mm,
            caloHalfWidth, caloHalfWidth);
    G4LogicalVolume* logicSiPlate = new G4LogicalVolume(
            new G4USolid(solidSiPlate->GetName(), solidSiPlate),
            mSi, solidSiPlate->GetName());
    new G4PVPlacement(0, G4ThreeVector(crystalLength + 1.5 * mm, 0, 0),
            logicSiPlate, logicSiPlate->GetName(), logicWorld, false, 0, true);
    G4Region* regionSiPlate = new G4Region(logicSiPlate->GetName());
    regionSiPlate->AddRootLogicalVolume(logicSiPlate);

    UBox* solidPbF2 = new UBox("PbF2", crystalLength / 2.0,
            caloHalfWidth, caloHalfWidth);
    G4LogicalVolume* logicPbF2 = new G4LogicalVolume(
            new G4USolid(solidPbF2->GetName(), solidPbF2),
            mPbF2, solidPbF2->GetName());
    new G4PVPlacement(0, G4ThreeVector(crystalLength / 2.0, 0, 0),
            logicPbF2, logicPbF2->GetName(), logicWorld, false, 0, true);
    G4Region* regionPbF2 = new G4Region(logicPbF2->GetName());
    regionPbF2->AddRootLogicalVolume(logicPbF2);

    return physWorld;
}

