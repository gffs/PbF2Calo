#include "StackingAction.h"
#include "RunAction.h"

#include "G4Track.hh"
#include "G4TrackStatus.hh"
#include "G4RunManager.hh"
#include "G4UserRunAction.hh"

StackingAction::StackingAction() : 
    G4UserStackingAction()
{
        const G4UserRunAction* const ura = G4RunManager::GetRunManager()->GetUserRunAction();
        ra = dynamic_cast<const RunAction*>(ura);
}

StackingAction::~StackingAction()
{
}

G4ClassificationOfNewTrack StackingAction::ClassifyNewTrack(const G4Track* aTrack)
{
    G4ClassificationOfNewTrack classification = fUrgent;

    if (aTrack->GetParticleDefinition()->GetParticleName() == "opticalphoton") {
        const G4ThreeVector pos = aTrack->GetPosition();
        const G4ThreeVector mom = aTrack->GetMomentum();
        G4double gtime = aTrack->GetGlobalTime();

        ra->FillPhotonDeposit(pos, gtime, mom);
    }

    //if(aTrack->GetParentID() != 0) classification = fKill;
  
    return classification;
}

