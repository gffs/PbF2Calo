#include "PrimaryGeneratorAction.h"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction(const json11::Json& cfg):
    G4VUserPrimaryGeneratorAction(),
    fParticleGun(new G4ParticleGun(1)),
    cfg_(cfg)
{
    fParticleGun->SetParticleDefinition(G4ParticleTable::GetParticleTable()->
        FindParticle(cfg_["particle"].string_value()));

    fParticleGun->SetParticleEnergy(cfg_["energy"].number_value() * MeV);    

    auto ma = cfg_["momentum_direction"].array_items();
    fParticleGun->SetParticleMomentumDirection(
            { ma[0].number_value(), ma[1].number_value(), ma[2].number_value() });
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fParticleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    auto pp = cfg_["particle_position"].array_items();
    fParticleGun->SetParticlePosition(
            { pp[0].number_value(), pp[1].number_value(), pp[2].number_value() });

    fParticleGun->GeneratePrimaryVertex(anEvent);
}

