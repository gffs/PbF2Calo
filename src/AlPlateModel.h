#pragma once

#include "G4FastStep.hh"
#include "G4FastTrack.hh"
#include "G4Region.hh"
#include "G4LogicalVolume.hh"
#include "G4ParticleDefinition.hh"
#include "G4VFastSimulationModel.hh"

class AlPlateModel: public G4VFastSimulationModel
{
  public:
    AlPlateModel(const G4String& name, G4Region* const regionAlPlate);
    ~AlPlateModel() {}

    virtual G4bool IsApplicable(const G4ParticleDefinition& aParticle);
    virtual G4bool ModelTrigger(const G4FastTrack& aTrack);
    virtual void DoIt(const G4FastTrack& aTrack, G4FastStep& aStep);

  private:
    G4Region* const regionAlPlate_;
};

