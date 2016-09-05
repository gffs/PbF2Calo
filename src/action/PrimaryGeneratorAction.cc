#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "GeometryUtility.h"
#include "JsonUtility.h"
#include "PrimaryGeneratorAction.h"
#include "Randomize.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(const json11::Json& cfg):
    G4VUserPrimaryGeneratorAction(),
    fParticleGun(new G4ParticleGun(1)),
    cfg_(cfg)
{
    fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->
        FindParticle(cfg_["particle"].string_value()));

    fParticleGun->SetParticleEnergy(cfg_["energy"].number_value() * MeV);

    fParticleGun->SetParticleMomentumDirection(json11::Init<G4ThreeVector>::with(
        cfg_["momentum_direction"])
    );
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    fParticleGun->SetParticlePosition(json11::Init<G4ThreeVector>::with(
        cfg_["particle_position"], [&](double x) { return x * mm; })
    );

    fParticleGun->GeneratePrimaryVertex(anEvent);
}
