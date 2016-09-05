#pragma once

#include "json11.hpp"
#include "string"
#include "unordered_map"
#include "unordered_set"

class VUSolid;

class GeometryBase
{
public:
    GeometryBase(const json11::Json&);
    virtual ~GeometryBase();

    static GeometryBase* build(std::string name, const std::string& conf_file_name);

    template<typename T>
    static GeometryBase* create(const json11::Json& cfg) { return new T(cfg); }

protected:
    json11::Json cfg_;
    std::unordered_map<std::string, std::function<void()>> part_list;
    const double carTolerance;

private:
    static std::unordered_map<std::string, GeometryBase*(*)(const json11::Json& )> geo_map;
};
