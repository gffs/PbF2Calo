#pragma once

#include "json11.hpp"
#include "string"
#include "unordered_map"

class TFile;

class ProductBase
{
  public:
    ProductBase(const json11::Json&);
    virtual ~ProductBase() {}

    static ProductBase* init(std::string name, const std::string& conf_file_name, TFile& root_file);

    template<typename T>
    static ProductBase* create(const json11::Json& cfg, TFile& root_file) { return new T(cfg, root_file); }

    virtual int FillTree() const { return 0; }

  protected:
    json11::Json cfg_;

  private:
    static std::unordered_map<std::string, ProductBase*(*)(const json11::Json&, TFile&)> product_map;

};
