#pragma once

#include "G4GDMLParser.hh"
#include "ProductBase.h"
#include "json11.hpp"

class TFile;

class GdmlWriter: public ProductBase
{
public:
    GdmlWriter(const json11::Json& cfg, TFile& root_file);
    ~GdmlWriter();

private:
    G4GDMLParser parser;
};
