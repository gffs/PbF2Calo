#pragma once

#include "cassert"
#include "json11.hpp"
#include "unordered_set"

namespace json11 {
    template <typename T>
    struct InitImpl {
        public:
        template <size_t n, typename arg, class UnaryOperation>
        T unpack(const Json& j, UnaryOperation op);

        private:
        template <typename arg, size_t... I>
        T unpack_helper(std::vector<arg>& args, std::index_sequence<I...>);
    };


    template <typename T>
    struct Init
    {
        template <class UnaryOperation>
        static T with(const Json&, UnaryOperation)
        {
            assert (false);
            return T();
        };

        static T with(const Json&)
        {
            assert (false);
            return T();
        };
    };


template <typename T>
template <typename arg, size_t... I>
T InitImpl<T>::unpack_helper(std::vector<arg>& args, std::index_sequence<I...>) {
    return T(args[I]...);
}

template <typename T>
template <size_t n, typename arg, class UnaryOperation>
T InitImpl<T>::unpack(const Json& j, UnaryOperation op)
{
    auto ma = j.array_items();
    std::vector<arg> v;
    std::transform(ma.begin(), ma.end(), std::back_inserter(v),
        [&](const Json& x) { return op(x.number_value()); });

    assert(v.size() == n);

    return unpack_helper(v, std::make_index_sequence<n>{});
}


//specializations

template<typename T>
struct Identity
{
    T operator()(const T& t) { return t; }
};

template <>
template <class UnaryOperation>
G4ThreeVector Init<G4ThreeVector>::with(const Json& j, UnaryOperation op)
{
    return InitImpl<G4ThreeVector>().unpack<3, double>(j, op);
}

template <>
inline G4ThreeVector Init<G4ThreeVector>::with(const Json& j)
{
    return InitImpl<G4ThreeVector>().unpack<3, double>(j, Identity<double>());
}


template <>
inline std::unordered_set<unsigned int> Init<std::unordered_set<unsigned int>>::with(const Json& j)
{
    auto ma = j.array_items();
    std::unordered_set<unsigned int> res;
    std::transform(ma.begin(), ma.end(), std::inserter(res, res.end()),
        [&](const Json& x) { return x.int_value(); });

    return res;
}


} //::json11
