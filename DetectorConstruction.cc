#include "DetectorConstruction.hh"
#include "CalorimeterSD.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4Text.hh"
#include "G4VisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Transform3D.hh"

namespace B4c
{

G4ThreadLocal G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = nullptr;

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    DefineMaterials();
    return DefineVolumes();
}

void DetectorConstruction::DefineMaterials()
{
    auto nistManager = G4NistManager::Instance();
    nistManager->FindOrBuildMaterial("G4_Si");
    nistManager->FindOrBuildMaterial("G4_Galactic");
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
    auto silicon = G4NistManager::Instance()->FindOrBuildMaterial("G4_Si");
    auto vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");

    G4Box* worldBox = new G4Box("World", 1 * m, 1 * m, 1 * m);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(worldBox, vacuum, "WorldLV");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, true);

    G4Box* siliconBox = new G4Box("SiliconBox", 20 * mm, 20 * mm, 4.0 * mm);

    G4RotationMatrix* rotY = new G4RotationMatrix();
    rotY->rotateY(90 * deg);

    G4RotationMatrix* rotX = new G4RotationMatrix();
    rotX->rotateX(90 * deg);

    std::vector<G4LogicalVolume*> siliconLogVols;

    for (int i = 1; i <= 6; i++)
    {
        std::string lvName = "SiliconLV" + std::to_string(i);
        auto logicSilicon = new G4LogicalVolume(siliconBox, silicon, lvName);
        siliconLogVols.push_back(logicSilicon);
    }

    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 21 * mm), siliconLogVols[0], "SiliconDetector1", logicWorld, false, 0, true);
    new G4PVPlacement(nullptr, G4ThreeVector(0, 0, -21 * mm), siliconLogVols[1], "SiliconDetector2", logicWorld, false, 1, true);
    new G4PVPlacement(rotY, G4ThreeVector(21 * mm, 0, 0), siliconLogVols[2], "SiliconDetector3", logicWorld, false, 2, true);
    new G4PVPlacement(rotY, G4ThreeVector(-21 * mm, 0, 0), siliconLogVols[3], "SiliconDetector4", logicWorld, false, 3, true);
    new G4PVPlacement(rotX, G4ThreeVector(0, 21 * mm, 0), siliconLogVols[4], "SiliconDetector5", logicWorld, false, 4, true);
    new G4PVPlacement(rotX, G4ThreeVector(0, -21 * mm, 0), siliconLogVols[5], "SiliconDetector6", logicWorld, false, 5, true);

    return physWorld;
}

void DetectorConstruction::ConstructSDandField()
{
    G4SDManager* sdManager = G4SDManager::GetSDMpointer();

    for (int i = 1; i <= 6; i++)
    {
        std::string sdName = "CalorimeterSD" + std::to_string(i);
        std::string colName = "SiliconHitsCollection" + std::to_string(i);
        auto siliconSD = new CalorimeterSD(sdName, colName, 1);
        sdManager->AddNewDetector(siliconSD);
        std::string lvName = "SiliconLV" + std::to_string(i);
        SetSensitiveDetector(lvName, siliconSD);
    }
}

} // namespace B4c
