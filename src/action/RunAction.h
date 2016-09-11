#pragma once

#include "G4UserRunAction.hh"
#include "G4ThreeVector.hh"
#include "ProductBase.h"
#include "json11.hpp"
#include "queue"
#include "unordered_map"

class G4Run;
class TFile;
class TTree;

class RunAction : public G4UserRunAction
{
  public:
    RunAction(const json11::Json& cfg);
    ~RunAction();

    void BeginOfRunAction(const G4Run*);
    void EndOfRunAction(const G4Run*);
    void FillRootTree() const;

    const ProductBase& operator[](const std::string& key);

  private:
    json11::Json cfg_;
    std::unordered_map<std::string, ProductBase*> product_run_map;

    TFile* root_file;
};
