#include "GdmlWriter.h"
#include "ProductBase.h"

#include "cassert"
#include "fstream"
#include "iostream"
#include "sstream"

std::unordered_map<std::string, ProductBase*(*)(const json11::Json&, TFile&)> ProductBase::product_map = {
    {"GDMLWriter", &ProductBase::create<GdmlWriter>}
};

ProductBase::ProductBase(const json11::Json& cfg):
    cfg_(cfg)
{ }

ProductBase* ProductBase::init(std::string name, const std::string& conf_file_name, TFile& root_file)
{
    std::ifstream conf_f("conf/" + conf_file_name + ".json", std::ios::in);
    std::stringstream sstr;
    sstr << conf_f.rdbuf();
    std::string json_err;

    json11::Json cfg = json11::Json::parse(sstr.str(), json_err);
    if (!json_err.empty()) {
        std::cout << "\nFailed to parse config file \"" << conf_file_name <<
            "\" with error: \"" << json_err << "\"." << std::endl;
        assert(false);
    }

    auto it = product_map.find(name);
    assert (it != product_map.end());
    return it->second(cfg, root_file);
}
