
#pragma once

#include "hif_base.hpp"

#include "glaze/glaze.hpp"
#include "glaze/glaze_exceptions.hpp"
#include <glaze/exceptions/binary_exceptions.hpp>

template <>
struct glz::meta<Hif_base::Tuple_entry>
{
   using T = Hif_base::Tuple_entry;
   static constexpr auto value = object("input", &T::input, "lhs", &T::lhs, "rhs", &T::rhs, "lhs_cat", &T::lhs_cat, "rhs_cat", &T::rhs_cat);
};


template <>
struct glz::meta<Hif_base::Statement>
{
   using T = Hif_base::Statement;
   static constexpr auto value = object("sclass", &T::sclass, "instance", &T::instance, "io", &T::io, "attr", &T::attr);
};
template <>
struct glz::meta<Hif_base::ID_cat> {
   using enum Hif_base::ID_cat;
   static constexpr auto value = enumerate("0", String_cat,
                                           "1", Base2_cat,
                                           "2", Base3_cat,
                                           "3", Base4_cat,
                                           "4", Custom_cat

   );
};
template <>
struct glz::meta<Hif_base::Statement_class> {
   using enum Hif_base::Statement_class;
   static constexpr auto value = enumerate("0", Node,
                                           "1", Assign,
                                           "2", Attr,
                                           "3", Open_call,
                                           "4", Closed_call,
                                           "5", Open_def,
                                           "6", Closed_def,
                                           "7", End,
                                           "8", Use
    );

};
