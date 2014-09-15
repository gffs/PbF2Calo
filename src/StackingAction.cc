#include "StackingAction.h"
#include "RunAction.h"

#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4RunManager.hh"
#include "G4UserRunAction.hh"

StackingAction::StackingAction() : 
    G4UserStackingAction()
{
        const G4UserRunAction* const ura =
            G4RunManager::GetRunManager()->GetUserRunAction();
        ra = dynamic_cast<const RunAction*>(ura);
}

StackingAction::~StackingAction()
{
}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
    if ((aTrack->GetParticleDefinition()->GetParticleName() == "e-"
            || aTrack->GetParticleDefinition()->GetParticleName() == "e+")
            && aTrack->GetTrackID() > 1)
    {
        G4double kin = aTrack->GetKineticEnergy();
        const G4ThreeVector pos = aTrack->GetPosition();
        G4double gtime = aTrack->GetGlobalTime();
        G4int evID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
        auto particleName = aTrack->GetParticleDefinition()->GetParticleName();
        auto physVol = aTrack->GetVolume()->GetName();
        auto procName = aTrack->GetCreatorProcess()->GetProcessName();

        ra->FillSecondariesDeposit(kin, pos, gtime, evID, physVol, particleName, procName);
    }

    G4ClassificationOfNewTrack classification = fUrgent;
    //if(aTrack->GetParentID() != 0) classification = fKill;
  
    return classification;
}

