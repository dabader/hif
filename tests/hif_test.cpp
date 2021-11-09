//  This file is distributed under the BSD 3-Clause License. See LICENSE for details.

#include <string>

#include "fmt/format.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "hif/hif_read.hpp"

class Hif_test : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {
    // Graph_library::sync_all();
  }
};

TEST_F(Hif_test, Trivial_test1) {

  // instantiate IMLI, some configuration and test the basic API so that it it
  // learns. It is also a way to showcase the API

  std::string test {
    "{conf:::tool=some_verilog_tool,version= 3.some_string.1\n"
    "=::: file=submodule.v,loc=2\n"
    "{module:inner:\\a,\\h:\\z,\\y,order=a;b;c;d\n"
    "+and::\a=Z:A=\\y,B=\\z,loc=3\n"
    "=:::loc=4\n"
    "+and::\\tmp=Z:A=\\y,B=\\z\n"
    "+not::\\h:\\tmp\n"
    "}loc=5\n"
    "{module:submodule:\\c,\\d:\\a,\\b,loc=7\n"
    "+inner:foo:\\d=h,\\y=a:z=\\b,y=\\b,loc=8\n"
    "}loc=9\n"
    "+comment:::col=0,txt= some comment\\, and another,loc=a\n"
    "}\n"
  };

  auto h = Hif_read::input(test);
  h.dump();

  h.each([](const Hif_read::Statement &stmt) {
    std::cout << "type:" << stmt.type << "\n";
    std::cout << "id:" << stmt.id << "\n";
    for(const auto &e:stmt.io) {
      std::cout << "  out.lhs:" << e.lhs << ":" << e.lhs_cat << "\n";
      std::cout << "  out.rhs:" << e.rhs << ":" << e.lhs_cat << "\n";
    }
  });

  EXPECT_NE(10,100);
}

