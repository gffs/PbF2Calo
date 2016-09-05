#include "Calorimeter.h"
#include "GeometryBase.h"
#include "G4GeometryTolerance.hh"

#include "fstream"
#include "iostream"
#include "sstream"

std::unordered_map<std::string, GeometryBase*(*)(const json11::Json&)> GeometryBase::geo_map = {
    {"Calorimeter", &GeometryBase::create<Calorimeter>}
};

GeometryBase::GeometryBase(const json11::Json& cfg):
    cfg_(cfg),
    carTolerance(10 * G4GeometryTolerance::GetInstance()->GetSurfaceTolerance())
{ }

GeometryBase::~GeometryBase() {}

GeometryBase* GeometryBase::build(std::string name, const std::string& conf_file_name)
{
    std::ifstream conf_f("conf/" + conf_file_name + ".json", std::ios::in);
    std::stringstream sstr;
    sstr << conf_f.rdbuf();
    std::string json_err;

    json11::Json cfg = json11::Json::parse(sstr.str(), json_err);
    if (!json_err.empty()) {
        std::cout << "\nFailed to parse config file \"" << conf_file_name <<
            "\" with error: \"" << json_err << "\"." << std::endl;
        return nullptr;
    }

    auto it = geo_map.find(name);
    if (it == geo_map.end()) { return nullptr; }
    return it->second(cfg);
}
