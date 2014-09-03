#pragma once

#include "G4VModularPhysicsList.hh"

#include "json11.hpp"
#include "unordered_map"

class PhysicsListBase : public G4VModularPhysicsList
{
  public:
    PhysicsListBase();
    static PhysicsListBase* init(const json11::Json& cfg);

    template<typename T>
    static PhysicsListBase* create() { return new T; }

    void SetCuts();

  protected:
    void AddParametrisation();

  private:
    static std::unordered_map<std::string, PhysicsListBase*(*)()> list_map;
    static json11::Json cfg_;

    G4double fCutForGamma;
    G4double fCutForElectron;
    G4double fCutForPositron;
};


