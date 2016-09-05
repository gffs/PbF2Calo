#include "Calorimeter.h"
#include "G4Box.hh"
#include "G4GeometryTolerance.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4Material.hh"
#include "G4MultiUnion.hh"
#include "G4NistManager.hh"
#include "G4OpticalSurface.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4VisAttributes.hh"
#include "GeometryUtility.h"

#include "array"
#include "math.h" // M_PI, order matters: first math.h then cmath
#include "cmath"
#include "cassert"
#include "chrono"
#include "iostream"
#include "unordered_map"


Calorimeter::Calorimeter(const json11::Json& cfg):
    GeometryBase(cfg),
    parentName(cfg_["parent_logical"].string_value()),
    innerClearance(cfg_["inner_clearance"].number_value() * in)
{
    part_list = {
        {"enclosure", std::bind(&Calorimeter::Enclosure, this)},
        {"frontPlate", std::bind(&Calorimeter::FrontPlate, this)},
        {"sipmPlate", std::bind(&Calorimeter::SipmPlate, this)},
        {"crystalArray", std::bind(&Calorimeter::CrystalArray, this)}
    };

    //parentLV = G4LogicalVolumeStore::GetInstance()->GetVolume(parentName);
    //assert(parentLV != nullptr);

    for (auto& part: part_list) {
        auto start_time = std::chrono::system_clock::now();
        part.second();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - start_time
        );
        std::cout << part.first << " built in: " << duration.count() << " msec." << std::endl;
    }
}

void Calorimeter::Enclosure()
{

    return;
/*
    G4Tubs& outerWallSV = *new G4Tubs(
        "VacuumChamberOuterWall",
        outerWallRmin,
        outerWallRmin + outerWallThickness,
        innerClearance * 0.5,
        outerWallLength / outerWallRmin * (-0.5) * rad,
        outerWallLength / outerWallRmin * rad
    );

    G4Material* mAl = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

    G4LogicalVolume* outerWallLV = new G4LogicalVolume(
        &outerWallSV, mAl, outerWallSV.GetName()
    );

    G4VisAttributes vis(true, {0.1,0,0.1,1});
    //vis.SetForceLineSegmentsPerCircle(720);

    outerWallLV->SetVisAttributes(vis);
*/
}

//Plastic front plate with prisms
void Calorimeter::FrontPlate()
{
    //Aluminum plate from 2015, for now
    G4Box& frontPlateSV = *new G4Box(
        "CalorimeterFrontPlate",
        2.0 * mm,
        500 * mm, 500 * mm
    );

    G4Material* mAl = G4NistManager::Instance()->FindOrBuildMaterial("G4_Al");

    G4LogicalVolume* frontPlateLV = new G4LogicalVolume(
        &frontPlateSV, mAl, frontPlateSV.GetName()
    );

    G4LogicalVolume* parentLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    if (parentLV == 0) { return; }

    new G4PVPlacement(0, G4ThreeVector(-2.0 * mm, 0, 0),
        frontPlateLV, frontPlateLV->GetName(), parentLV,
        false, 0, true
    );

    G4Region* regionFrontPlate = new G4Region(frontPlateLV->GetName());
    regionFrontPlate->AddRootLogicalVolume(frontPlateLV);
}


//NuSil + Epoxy + active Si, for now
void Calorimeter::SipmPlate()
{
    G4Box& sipmPlateSV = *new G4Box(
        "CalorimeterSipmPlate",
        1.5 * mm,
        500 * mm, 500 * mm
    );

    G4Material* mSi =  G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");

    G4LogicalVolume* sipmPlateLV = new G4LogicalVolume(
        &sipmPlateSV, mSi, sipmPlateSV.GetName()
    );

    G4LogicalVolume* parentLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    if (parentLV == 0) { return; }

    new G4PVPlacement(0, G4ThreeVector(140 * mm + 1.5 * mm, 0, 0),
        sipmPlateLV, sipmPlateLV->GetName(), parentLV,
        false, 0, true
    );

    G4Region* regionSipmPlate = new G4Region(sipmPlateLV->GetName());
    regionSipmPlate->AddRootLogicalVolume(sipmPlateLV);


    //grease between crystal and SiPM
    G4Box& nusilSV = *new G4Box(
        "CalorimeterGreaseNusil",
        0.1 * mm,
        500 * mm, 500 * mm
    );

    G4Material* mNusil =  G4NistManager::Instance()->FindOrBuildMaterial("Nusil");
    G4LogicalVolume* nusilLV = new G4LogicalVolume(
        &nusilSV, mNusil, nusilSV.GetName());

    new G4PVPlacement(
        0, G4ThreeVector(-1.4 * mm, 0, 0),
        nusilLV, nusilLV->GetName(), sipmPlateLV, false, 0, true);


    //SiPM epoxy window
    G4Box& epoxySV = *new G4Box(
        "CalorimeterEpoxyWindow",
        0.225 * mm,
        500 * mm, 500 * mm
    );

    G4Material* mEpoxy =  G4NistManager::Instance()->FindOrBuildMaterial("Epoxy");
    G4LogicalVolume* epoxyLV = new G4LogicalVolume(
        &epoxySV, mEpoxy, epoxySV.GetName());

    new G4PVPlacement(
        0, G4ThreeVector(-1.075 * mm, 0, 0),
        epoxyLV, epoxyLV->GetName(), sipmPlateLV, false, 0, true
    );

    //Active Si region of SiPM
    G4Box& siActiveSV = *new G4Box(
        "CalorimeterSiPMActive",
        0.075 * mm,
        500 * mm, 500 * mm
    );

    G4LogicalVolume* siActiveLV = new G4LogicalVolume(
        &siActiveSV, mSi, siActiveSV.GetName()
    );

    new G4PVPlacement(
        0, G4ThreeVector(-0.775 * mm, 0, 0),
        siActiveLV, siActiveLV->GetName(), sipmPlateLV,
        false, 0, true
    );
}


void Calorimeter::CrystalArray()
{
    G4Box& crystalArraySV = *new G4Box(
        "CalorimeterCrystalArray",
        140.0 / 2.0 * mm,
        500 * mm, 500 * mm
    );

    G4Material* mPbF2 = G4NistManager::Instance()->FindOrBuildMaterial("PbF2");
    G4LogicalVolume* crystalArrayLV = new G4LogicalVolume(
        &crystalArraySV, mPbF2, crystalArraySV.GetName()
    );

    G4LogicalVolume* parentLV = G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    if (parentLV == 0) { return; }

    auto crystalArrayPV = new G4PVPlacement(
        0, G4ThreeVector(70 * mm, 0, 0),
        crystalArrayLV, crystalArrayLV->GetName(), parentLV,
        false, 0, true
    );

    G4Region* regionPbF2 = new G4Region(crystalArrayLV->GetName());
    regionPbF2->AddRootLogicalVolume(crystalArrayLV);


    //if (cfg_["crystal_wrapping"]["is_present"].bool_value()) {
    if (false) {
        double crystalLength = 140 * mm;
        double caloHalfWidth = 500 * mm;
        G4Box& solidWrappingLayer = *new G4Box("Wrapping", crystalLength / 2.0,
                caloHalfWidth, 0.050);
        G4MultiUnion* solidWrapping = new G4MultiUnion("Wrapping");

        unsigned short num_layers = (int)((caloHalfWidth - 12.5) / 25.0);
        for (unsigned short i = 0; i < 2 * num_layers; i++) {
            float offset = (-num_layers + i - 0.5) * 25.0;
            G4Transform3D& aTv = *new HepGeom::Translate3D(0,0,offset);
            solidWrapping->AddNode(solidWrappingLayer, aTv);

            //angles in deg are euler in x convention: z, x, z
            G4Transform3D& aTh = *new G4Transform3D();
            aTh = HepGeom::Translate3D(0,0,offset) * HepGeom::RotateY3D(90 * deg);
            solidWrapping->AddNode(solidWrappingLayer, aTh);
        }

        solidWrapping->Voxelize();

        G4Material* mMylar = G4NistManager::Instance()->FindOrBuildMaterial("Mylar");
        G4LogicalVolume* logicWrapping = new G4LogicalVolume(
            solidWrapping, mMylar, solidWrapping->GetName()
        );
        auto physicalWrapping = new G4PVPlacement(
            0, G4ThreeVector(0, 0, 0),
            logicWrapping, logicWrapping->GetName(), crystalArrayLV,
            false, 0, true
        );

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
                G4double eng = CLHEP::h_Planck * CLHEP::c_light * MeV / (nm * pr[0].number_value());
                G4double val = pr[1].number_value();
                ompt->AddEntry(key, eng, val);
            }
        }
        //ompt->DumpTable();
        osWrapping->SetMaterialPropertiesTable(ompt);

        new G4LogicalBorderSurface("WrappingOpSurface", crystalArrayPV, physicalWrapping, osWrapping);
    }
}
