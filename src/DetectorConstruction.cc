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

DetectorConstruction::DetectorConstruction(const json11::Json cfg):
    G4VUserDetectorConstruction(),
    crystalLength(140 * mm),
    cfg_(cfg)
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
    G4Material* mAl = nistManager->FindOrBuildMaterial("G4_Al");
    G4Material* mC = nistManager->FindOrBuildMaterial("G4_C");
    G4Material* mF = nistManager->FindOrBuildMaterial("G4_F");
    G4Material* mH = nistManager->FindOrBuildMaterial("G4_H");
    G4Material* mO = nistManager->FindOrBuildMaterial("G4_O");
    G4Material* mPb = nistManager->FindOrBuildMaterial("G4_Pb");
    G4Material* mSi = nistManager->FindOrBuildMaterial("G4_Si");

    G4Material* mPbF2 = new G4Material("PbF2", 7.77*g/cm3, 2);
    mPbF2->AddMaterial(mPb, 0.84504);
    mPbF2->AddMaterial(mF, 0.15496);

    G4Material* mNusil = new G4Material("Nusil", 1.02*g/cm3, 2);
    mNusil->AddMaterial(mC, 0.799);
    mNusil->AddMaterial(mH, 0.201);

    G4Material* mEpoxy = new G4Material("Epoxy", 1.02*g/cm3, 2);
    mEpoxy->AddMaterial(mSi, 0.46743);
    mEpoxy->AddMaterial(mO, 0.53257);

    const G4MaterialTable* theMaterialTable = G4Material::GetMaterialTable();
    for (auto mat: *theMaterialTable) {
        auto mat_cfg = cfg_["material_properties"][mat->GetName()];
        if (mat_cfg.is_null()) { continue; }

        G4MaterialPropertiesTable* mpt = new G4MaterialPropertiesTable();
        for (auto& mat_prop: mat_cfg.object_items()) {
            const char* key = mat_prop.first.c_str();
            mpt->AddProperty(key, new G4MaterialPropertyVector());

            for (auto& pr: mat_prop.second.array_items()) {
                G4double eng = h_Planck * c_light * MeV / (nm * pr[0].number_value());
                G4double val = pr[1].number_value();
                mpt->AddEntry(key, eng, val);
            }
        }
        mat->SetMaterialPropertiesTable(mpt);
    }

    G4GeometryManager::GetInstance()->OpenGeometry();
    G4PhysicalVolumeStore::GetInstance()->Clean();
    G4LogicalVolumeStore::GetInstance()->Clean();
    G4SolidStore::GetInstance()->Clean();

    const float caloHalfWidth = 500 * mm;
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

    UBox* solidNusil = new UBox("Nusil", 0.1 * mm,
            caloHalfWidth, caloHalfWidth);
    G4LogicalVolume* logicNusil = new G4LogicalVolume(
            new G4USolid(solidNusil->GetName(), solidNusil),
            mNusil, solidNusil->GetName());
    new G4PVPlacement(0, G4ThreeVector(-1.4 * mm, 0, 0),
            logicNusil, logicNusil->GetName(), logicSiPlate, false, 0, true);

    UBox* solidEpoxy = new UBox("Epoxy", 0.1 * mm,
            caloHalfWidth, caloHalfWidth);
    G4LogicalVolume* logicEpoxy = new G4LogicalVolume(
            new G4USolid(solidEpoxy->GetName(), solidEpoxy),
            mEpoxy, solidEpoxy->GetName());
    new G4PVPlacement(0, G4ThreeVector(-1.2 * mm, 0, 0),
            logicEpoxy, logicEpoxy->GetName(), logicSiPlate, false, 0, true);

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

