#include "GDMLWriter.h"
#include "G4Event.hh"
#include "G4Threading.hh"
#include "G4Track.hh"
#include "G4TransportationManager.hh"

#include "iostream"

GdmlWriter::GdmlWriter(const json11::Json& cfg, TFile&):
    ProductBase(cfg),
    parser()
{

    if (G4Threading::G4GetThreadId()) return;

    parser.SetRegionExport(true);
    parser.Write("ring_geometry.gdml", G4TransportationManager::GetTransportationManager()
        ->GetNavigatorForTracking()->GetWorldVolume()->GetLogicalVolume());
}

GdmlWriter::~GdmlWriter() {}
