#include "DetectorConstruction.h"
#include "G4GeometryManager.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4OpticalSurface.hh"
#include "G4PhysicalConstants.hh"
#include "G4PhysicalVolumeStore.hh"
#include "G4PVPlacement.hh"
#include "G4SolidStore.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Usolid.hh"
#include "UBox.hh"
#include "UMultiUnion.hh"

#include <array>

DetectorConstruction::DetectorConstruction(const json11::Json cfg):
    G4VUserDetectorConstruction(),
    crystalLength(140 * mm),
    usolidStore(),
    cfg_(cfg)
{
}

DetectorConstruction::~DetectorConstruction()
{
    delete physWorld;
    delete logicWorld;
    usolidStore.clear();
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
    G4Material* mMylar = nistManager->FindOrBuildMaterial("G4_MYLAR");


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

    UBox* solidEpoxy = new UBox("Epoxy", 0.225 * mm,
            caloHalfWidth, caloHalfWidth);
    G4LogicalVolume* logicEpoxy = new G4LogicalVolume(
            new G4USolid(solidEpoxy->GetName(), solidEpoxy),
            mEpoxy, solidEpoxy->GetName());
    new G4PVPlacement(0, G4ThreeVector(-1.075 * mm, 0, 0),
            logicEpoxy, logicEpoxy->GetName(), logicSiPlate, false, 0, true);

    UBox* solidSiActive = new UBox("SiActive", 0.075 * mm,
            caloHalfWidth, caloHalfWidth);
    G4LogicalVolume* logicSiActive = new G4LogicalVolume(
            new G4USolid(solidSiActive->GetName(), solidSiActive),
            mSi, solidSiActive->GetName());
    new G4PVPlacement(0, G4ThreeVector(-0.775 * mm, 0, 0),
            logicSiActive, logicSiActive->GetName(), logicSiPlate, false, 0, true);

    UBox* solidPbF2 = new UBox("PbF2", crystalLength / 2.0,
            caloHalfWidth, caloHalfWidth);
    G4LogicalVolume* logicPbF2 = new G4LogicalVolume(
            new G4USolid(solidPbF2->GetName(), solidPbF2),
            mPbF2, solidPbF2->GetName());
    auto physicalPbF2 = new G4PVPlacement(0, G4ThreeVector(crystalLength / 2.0, 0, 0),
            logicPbF2, logicPbF2->GetName(), logicWorld, false, 0, true);
    G4Region* regionPbF2 = new G4Region(logicPbF2->GetName());
    regionPbF2->AddRootLogicalVolume(logicPbF2);

    if (cfg_["crystal_wrapping"]["is_present"].bool_value()) {
        UBox& solidWrappingLayer = *new UBox("Wrapping", crystalLength / 2.0, 
                caloHalfWidth, 0.050);
        usolidStore.insert(&solidWrappingLayer);
        UMultiUnion* solidWrapping = new UMultiUnion("Wrapping");

        unsigned short num_layers = (int)((caloHalfWidth - 12.5) / 25.0);
        for (unsigned short i = 0; i < 2 * num_layers; i++) {
            float offset = (-num_layers + i - 0.5) * 25.0;
            UTransform3D& aTv = *new UTransform3D(0,0,offset, 0,0,0);
            solidWrapping->AddNode(solidWrappingLayer, aTv);

            //angles in deg are euler in x convention: z, x, z
            UTransform3D& aTh = *new UTransform3D(0,offset,0, 0,90,0);
            solidWrapping->AddNode(solidWrappingLayer, aTh);
        }

        solidWrapping->Voxelize();
        solidWrapping->Capacity();

        G4LogicalVolume* logicWrapping = new G4LogicalVolume(
                new G4USolid(solidWrapping->GetName(), solidWrapping),
                mMylar, solidWrapping->GetName());
        auto physicalWrapping = new G4PVPlacement(0, G4ThreeVector(0, 0, 0),
                logicWrapping, logicWrapping->GetName(), logicPbF2, false, 0, true);

        G4OpticalSurface* osWrapping = new G4OpticalSurface("wrapping");
        osWrapping->SetType(dielectric_dielectric);
        osWrapping->SetModel(unified);

        std::string wrappingName = cfg_["crystal_wrapping"]["material"].string_value();
        auto opProp = cfg_["optical_properties"][wrappingName];

        std::map<std::string, int> opFinish = {
            {"polished", polished},
            {"polishedfrontpainted", polishedfrontpainted},
            {"polishedbackpainted", polishedbackpainted},
            {"ground", ground},
            {"groundfrontpainte", groundfrontpainted},
            {"groundbackpainted", groundbackpainted}
        };

        osWrapping->SetFinish(static_cast<G4OpticalSurfaceFinish>
                (opFinish[opProp["finish"].string_value()]));
        osWrapping->SetSigmaAlpha(opProp["sigma_alpha"].number_value());

        std::array<std::string, 9> opArray = {{
            "RINDEX", "SPECULARLOBECONSTANT", "SPECULARSPIKECONSTANT",
            "BACKSCATTERCONSTANT", "REFLECTIVITY", "TRANSMITTANCE",
            "EFFICIENCY", "REALRINDEX", "IMAGINARYRINDEX"
        }}; 

        G4MaterialPropertiesTable* ompt = new G4MaterialPropertiesTable();
        for (auto prop: opArray) {
            auto op_cfg = opProp[prop];
            if (op_cfg.is_null()) { continue; }

            const char* key = prop.c_str();
            ompt->AddProperty(key, new G4MaterialPropertyVector());

            for (auto& pr: op_cfg.array_items()) {
                G4double eng = h_Planck * c_light * MeV / (nm * pr[0].number_value());
                G4double val = pr[1].number_value();
                ompt->AddEntry(key, eng, val);
            }
        }
        //ompt->DumpTable();
        osWrapping->SetMaterialPropertiesTable(ompt);

        new G4LogicalBorderSurface("WrappingOpSurface", physicalPbF2, physicalWrapping, osWrapping);
    }

    return physWorld;
}

