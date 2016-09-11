#pragma once

#include "G4FastStep.hh"
#include "G4FastTrack.hh"
#include "G4ParticleDefinition.hh"
#include "G4VFastSimulationModel.hh"
#include "MuonTrackSniffer.h"

class G4Region;
class RunAction;

class MuonTrackSnifferModel: public G4VFastSimulationModel
{
  public:
    MuonTrackSnifferModel(const G4String& name, G4Region* const regionSniff);
    ~MuonTrackSnifferModel() {}

    G4bool IsApplicable(const G4ParticleDefinition& aParticle);
    G4bool ModelTrigger(const G4FastTrack& aTrack);
    void DoIt(const G4FastTrack& aTrack, G4FastStep& aStep);

  private:
    RunAction* ra_;
    //const MuonTrackSniffer& sniffer;
};
