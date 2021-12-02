//  This file is distributed under the BSD 3-Clause License. See LICENSE for details.

#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

class Hif_base {
public:
  // ID categories (ttt field)
  enum ID_cat : uint8_t {
    Net_cat    = 0,
    String_cat = 1,
    Base2_cat  = 2,
    Base3_cat  = 3,
    Base4_cat  = 4,
    Custom_cat = 5
  };

  // statement class (cccc field)
  enum Statement_class : uint8_t {
    Node_class = 0,
    Assign_class,
    Forward_class,
    Attr_class,
    Begin_open_scope_class,
    Begin_close_scope_class,
    Begin_open_function_class,
    Begin_close_function_class,
    End_class,
    Use_class,
    Declare_class
  };

  struct Tuple_entry {
    // add input
    Tuple_entry(std::string_view l, std::string_view r, ID_cat lc, ID_cat rc) :input(true), lhs(l), rhs(r), lhs_cat(lc), rhs_cat(rc) {}

    // add output
    Tuple_entry(std::string_view l) :input(false), lhs(l), lhs_cat(ID_cat::Net_cat), rhs_cat(ID_cat::Net_cat)  {
    }

    bool             input;
    std::string_view lhs;
    std::string_view rhs;
    ID_cat           lhs_cat;  // Either String or Net (net in output, string in input)
    ID_cat           rhs_cat;
  };

  struct Statement {
    Statement_class sclass;

    uint16_t         type;  // 12 bit type
    std::string_view id;

    std::vector<Tuple_entry> io;
    std::vector<Tuple_entry> attr;

    Statement(Statement_class c) : sclass(c) {
    }

    void add_input_net   (std::string_view lhs, std::string_view rhs) {
      io.emplace_back(lhs, rhs, ID_cat::Net_cat, ID_cat::Net_cat);
    }
    void add_input_string(std::string_view lhs, std::string_view rhs) {
      io.emplace_back(lhs, rhs, ID_cat::Net_cat, ID_cat::String_cat);
    }
    void add_input_base2 (std::string_view lhs, std::string_view rhs) {
      io.emplace_back(lhs, rhs, ID_cat::Net_cat, ID_cat::Base2_cat);
    }
    void add_input_base4 (std::string_view lhs, std::string_view rhs) {
      io.emplace_back(lhs, rhs, ID_cat::Net_cat, ID_cat::Base4_cat);
    }
    void add_input_custom(std::string_view lhs, std::string_view rhs) {
      io.emplace_back(lhs, rhs, ID_cat::Net_cat, ID_cat::Custom_cat);
    }

    void add_output(std::string_view lhs) {
      io.emplace_back(lhs);
    }

    void add_attr_net   (std::string_view lhs, std::string_view rhs) {
      attr.emplace_back(lhs, rhs, ID_cat::Net_cat, ID_cat::Net_cat);
    }
    void add_attr_string(std::string_view lhs, std::string_view rhs) {
      attr.emplace_back(lhs, rhs, ID_cat::Net_cat, ID_cat::String_cat);
    }
    void add_attr_base2 (std::string_view lhs, std::string_view rhs) {
      attr.emplace_back(lhs, rhs, ID_cat::Net_cat, ID_cat::Base2_cat);
    }
    void add_attr_base4 (std::string_view lhs, std::string_view rhs) {
      attr.emplace_back(lhs, rhs, ID_cat::Net_cat, ID_cat::Base4_cat);
    }
    void add_attr_custom(std::string_view lhs, std::string_view rhs) {
      attr.emplace_back(lhs, rhs, ID_cat::Net_cat, ID_cat::Custom_cat);
    }

  };
};
