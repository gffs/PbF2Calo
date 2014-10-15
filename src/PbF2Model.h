#pragma once

#include "G4FastStep.hh"
#include "G4FastTrack.hh"
#include "G4ParticleDefinition.hh"
#include "G4VFastSimulationModel.hh"

class G4Region;

class PbF2Model: public G4VFastSimulationModel
{
  public:
    PbF2Model(const G4String& name, G4Region* const regionPbF2);
    ~PbF2Model() {}

    G4bool IsApplicable(const G4ParticleDefinition& aParticle);
    G4bool ModelTrigger(const G4FastTrack& aTrack);
    void DoIt(const G4FastTrack& aTrack, G4FastStep& aStep);
};

