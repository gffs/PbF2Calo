#pragma once

#include "json11.hpp"

namespace json11 {
    template <typename T>
    struct InitImpl {
        public:
        template <size_t n, typename arg>
        T unpack(const Json& j);

        private:
        template <typename arg, size_t... I>
        T unpack_helper(std::vector<arg>& args, std::index_sequence<I...>);
    };

    template <typename T>
    T Init(const Json&) { return T(); }
}

#include "JsonUtility.icc"

