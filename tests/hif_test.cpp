//  This file is distributed under the BSD 3-Clause License. See LICENSE for
//  details.

#include <string>
#include <dirent.h>


#include "hif/hif_glaze.hpp"
#include "hif/hif_read.hpp"
#include "hif/hif_write.hpp"
#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include <chrono>
#define BITS 21  //tested limit
#define LARGE_STMT_BITS 19 //doesnt change
class Hif_test : public ::testing::Test {
protected:
  void SetUp() override {}

  void TearDown() override {
    // Graph_library::sync_all();
  }
};


std::string get_random_string() {
  std::string str;

  static int counter = 0;

  if (rand() & 1) {
    str = std::string("$");
  }
  str += std::to_string(counter);
  if (rand() & 1) {
    str += "something_quite_large_and_not_reusable";
  }
  if (rand() & 1) {
    str += std::to_string(1020303 + counter);
  }

  ++counter;

  return str;
}

TEST_F(Hif_test, Trivial_test1) {
  std::string fname("hif_test_data1");

  auto wr = Hif_write::create(fname, "testtool", "0.2.1");
  EXPECT_NE(wr, nullptr);

  auto stmt = Hif_write::create_assign();

  stmt.instance = "jojojo";
  stmt.add_input("A", "0");
  stmt.add_input("4", "jojojo");
  stmt.add_input("A", "1");
  stmt.add_input("A", "2");
  stmt.add_input("A", "3");

  stmt.add_output("Z");

  stmt.add_attr("loc", "3");

  wr->add(stmt);

  wr = nullptr; // close

  auto rd = Hif_read::open(fname);
  EXPECT_NE(rd, nullptr);

  EXPECT_EQ(rd->get_tool(), "testtool");
  EXPECT_EQ(rd->get_version(), "0.2.1");

  int conta = 0;
  rd->each([&conta, &stmt](const Hif_base::Statement &stmt2) {
    EXPECT_EQ(stmt, stmt2);
    ++conta;
  });

  EXPECT_EQ(conta, 1);
}
TEST_F(Hif_test, Large_stmt_write) {
  std::string fname("hif_test_data2");

  auto wr = Hif_write::create(fname, "testtool", "0.0.3");
  EXPECT_NE(wr, nullptr);

  auto stmt = Hif_write::create_assign();

  stmt.instance = "jojojo";

  for (auto i = 0u; i < (1ULL << LARGE_STMT_BITS); ++i) {
    stmt.add_input(std::to_string(i),
                   std::string("a_longer_string_") + std::to_string(i));
  }
  for (auto i = 0u; i < (1ULL << LARGE_STMT_BITS); ++i) {
    stmt.add_output(std::to_string(i) + "_out",
                    std::string("a_longer_string_") + std::to_string(i));
  }

  auto start = std::chrono::high_resolution_clock::now();

  wr->add(stmt);

  wr = nullptr; // close
  auto end = std::chrono::high_resolution_clock::now();

  auto rd = Hif_read::open(fname);
  EXPECT_NE(rd, nullptr);
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Duration: " << duration.count() << " ms" << std::endl;

  int conta = 0;
  rd->each([&conta, &stmt](const Hif_base::Statement &stmt2) {
    EXPECT_EQ(stmt, stmt2);
    ++conta;
  });

  EXPECT_EQ(conta, 1);

}
TEST_F(Hif_test, HIF_too_many_stmts_w_test) {
  std::string fname("hif_w_test_too_many_stmts");

  auto wr = Hif_write::create(fname, "testtool", "0.0.3");
  std::vector<Hif_base::Statement> directory;
    for (auto i = 0u; i < (1<<BITS); ++i) {
      Hif_base::Statement stmt;

      if (i & 1) {
        stmt = Hif_write::create_assign();
      } else {
        stmt = Hif_write::create_node();
      }

      std::string lhs = std::to_string(i);
      std::string rhs = std::to_string(i);

      stmt.add_input(lhs, i);
      stmt.add_output(rhs, i);
      directory.push_back(stmt);
      wr->add(stmt);
    }

  wr = nullptr; // close
  auto rd = Hif_read::open(fname);
  EXPECT_NE(rd, nullptr);

  int conta = 0;
  rd->each([&conta, &directory](const Hif_base::Statement &stmt2) {
    EXPECT_EQ(directory[conta], stmt2);
    ++conta;
  });
  
}
TEST_F(Hif_test, glaze_write_test) {
  std::vector<Hif_base::Statement> directory;
  {
    auto stmt = Hif_write::create_assign();
    stmt.instance = "jojojo";
    for (auto i = 0u; i < (1ULL << BITS); ++i) {
      stmt.add_input(std::to_string(i),
                     std::string("a_longer_string_") + std::to_string(i));
    }
    for (auto i = 0u; i < (1ULL << BITS); ++i) {
      stmt.add_output(std::to_string(i) + "_out",
                      std::string("a_longer_string_") + std::to_string(i));
    }

    directory.push_back(stmt);
  }

  auto start = std::chrono::high_resolution_clock::now();
  auto glaze_json_write =
      glz::write_file(directory, "./glaze_stmt_w_test.json", std::string{});
  auto end = std::chrono::high_resolution_clock::now();

  EXPECT_EQ(glaze_json_write, glz::error_code::none);

  std::vector<Hif_base::Statement> obj;
  auto glaze_json_read =
      glz::read_file(obj, "./glaze_stmt_w_test.json", std::string{});
  EXPECT_EQ(glaze_json_read, glz::error_code::none);
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Duration: " << duration.count() << " ms" << std::endl;

  EXPECT_EQ(directory, obj);
}



TEST_F(Hif_test, beve_write_test) {
  std::vector<Hif_base::Statement> directory;
  {
    auto stmt = Hif_write::create_assign();
    stmt.instance = "jojojo";
    for (auto i = 0u; i < (1ULL << BITS); ++i) {
      stmt.add_input(std::to_string(i),
                     std::string("a_longer_string_") + std::to_string(i));
    }
    for (auto i = 0u; i < (1ULL << BITS); ++i) {
      stmt.add_output(std::to_string(i) + "_out",
                      std::string("a_longer_string_") + std::to_string(i));
    }

    directory.push_back(stmt);
  }

  auto start = std::chrono::high_resolution_clock::now();
  auto beve_write_code =
      glz::write_file_binary(directory, "output0.eve", std::string{});
  auto end = std::chrono::high_resolution_clock::now();

  EXPECT_EQ(beve_write_code, glz::error_code::none);

  std::vector<Hif_base::Statement> obj;
  auto beve_read_code =
      glz::read_file_binary(obj, "output0.eve", std::string{});
  EXPECT_EQ(beve_read_code, glz::error_code::none);

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Duration: " << duration.count() << " ms" << std::endl;

  EXPECT_EQ(directory, obj);
}
TEST_F(Hif_test, Beve_too_many_stmts_w_test) {
  std::string fname("beve_w_test_too_many_stmts");

  std::vector<Hif_base::Statement> directory;

    for (auto i = 0u; i < (1<<BITS); ++i) {
      Hif_base::Statement stmt;

      if (i & 1) {
        stmt = Hif_write::create_assign();
      } else {
        stmt = Hif_write::create_node();
      }

//      std::string lhs = get_random_string();
//      std::string rhs = get_random_string();
      std::string lhs = std::to_string(i);
      std::string rhs = std::to_string(i);


      stmt.add_input(lhs, i);
      stmt.add_output(rhs, i);
      directory.push_back(stmt);
    }
  auto start = std::chrono::high_resolution_clock::now();
  auto beve_write_code =
      glz::write_file_binary(directory, "output1.eve", std::string{});
  auto end = std::chrono::high_resolution_clock::now();

  EXPECT_EQ(beve_write_code, glz::error_code::none);

  std::vector<Hif_base::Statement> obj;
  auto beve_read_code =
      glz::read_file_binary(obj, "output1.eve", std::string{});
  EXPECT_EQ(beve_read_code, glz::error_code::none);
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Duration: " << duration.count() << " ms" << std::endl;

  EXPECT_EQ(directory, obj);
  
}
TEST_F(Hif_test, Large_stmt_read) {
  std::string fname("hif_test_data3");

  auto wr = Hif_write::create(fname, "testtool", "0.0.3");
  EXPECT_NE(wr, nullptr);

  auto stmt = Hif_write::create_assign();

  stmt.instance = "jojojo";

  for (auto i = 0u; i < (1ULL << BITS); ++i) {
    stmt.add_input(std::to_string(i),
                   std::string("a_longer_string_") + std::to_string(i));
  }
  for (auto i = 0u; i < (1ULL << BITS); ++i) {
    stmt.add_output(std::to_string(i) + "_out",
                    std::string("a_longer_string_") + std::to_string(i));
  }

  wr->add(stmt);

  wr = nullptr; // close

  auto start = std::chrono::high_resolution_clock::now();
  auto rd = Hif_read::open(fname);
//  EXPECT_NE(rd, nullptr);
  int conta = 0;
  rd->each([&conta, &stmt](const Hif_base::Statement &stmt2) {
//    EXPECT_EQ(stmt, stmt2);
//    ++conta;
  });

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Duration: " << duration.count() << " ms" << std::endl;


//  EXPECT_EQ(conta, 1);
}
TEST_F(Hif_test, glaze_read_test) {
  std::vector<Hif_base::Statement> directory;
  {
    auto stmt = Hif_write::create_assign();
    stmt.instance = "jojojo";
    for (auto i = 0u; i < (1ULL << BITS); ++i) {
      stmt.add_input(std::to_string(i),
                     std::string("a_longer_string_") + std::to_string(i));
    }
    for (auto i = 0u; i < (1ULL << BITS); ++i) {
      stmt.add_output(std::to_string(i) + "_out",
                      std::string("a_longer_string_") + std::to_string(i));
    }

    directory.push_back(stmt);
  }

  auto glaze_json_write =
      glz::write_file(directory, "./glaze_stmt_r_test.json", std::string{});
  EXPECT_EQ(glaze_json_write, glz::error_code::none);

  std::vector<Hif_base::Statement> another_directory;

  auto start = std::chrono::high_resolution_clock::now();
  auto glaze_json_read = glz::read_file(
      another_directory, "./glaze_stmt_r_test.json", std::string{});
  auto end = std::chrono::high_resolution_clock::now();

  EXPECT_EQ(glaze_json_read, glz::error_code::none);

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Duration: " << duration.count() << " ms" << std::endl;

  EXPECT_EQ(directory, another_directory);
}
TEST_F(Hif_test, beve_read_test) {
  std::vector<Hif_base::Statement> directory;
  {
    auto stmt = Hif_write::create_assign();
    stmt.instance = "jojojo";
    for (auto i = 0u; i < (1ULL << BITS); ++i) {
      stmt.add_input(std::to_string(i),
                     std::string("a_longer_string_") + std::to_string(i));
    }
    for (auto i = 0u; i < (1ULL << BITS); ++i) {
      stmt.add_output(std::to_string(i) + "_out",
                      std::string("a_longer_string_") + std::to_string(i));
    }

    directory.push_back(stmt);
  }

  auto write_ec =
      glz::write_file_binary(directory, "output2.eve", std::string{});
  EXPECT_EQ(write_ec, glz::error_code::none);

  std::vector<Hif_base::Statement> obj;

  auto start = std::chrono::high_resolution_clock::now();
  auto read_ec = glz::read_file_binary(obj, "output2.eve", std::string{});
  auto end = std::chrono::high_resolution_clock::now();

  EXPECT_EQ(read_ec, glz::error_code::none);

  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Duration: " << duration.count() << " ms" << std::endl;

  EXPECT_EQ(directory, obj);
}

TEST_F(Hif_test, Statement_class_check) {
  std::string fname("hif_test_statement_class_check");

  Hif_base::Statement stmt;

#define STMT_CLASS_CHECK(type)                                                 \
  stmt = Hif_write::create_##type();                                           \
  EXPECT_TRUE(stmt.is_##type());

  STMT_CLASS_CHECK(node);
  STMT_CLASS_CHECK(assign);
  STMT_CLASS_CHECK(attr);
  STMT_CLASS_CHECK(open_call);
  STMT_CLASS_CHECK(closed_call);
  STMT_CLASS_CHECK(open_def);
  STMT_CLASS_CHECK(closed_def);
  STMT_CLASS_CHECK(end);
  STMT_CLASS_CHECK(use);
}

TEST_F(Hif_test, empty_string) {
  std::string fname = "hif_empty_string";

  auto wr = Hif_write::create(fname, "lnast", "test");
  EXPECT_NE(wr, nullptr);
  auto stmt = Hif_write::create_attr();
  stmt.add_attr("name", "");
  wr->add(stmt);

  wr = nullptr;

  auto rd = Hif_read::open(fname);
  EXPECT_NE(rd, nullptr);
  rd->get_tool();
  rd->get_version();
  rd->next_stmt();
  stmt = rd->get_current_stmt();
  EXPECT_EQ(stmt.attr[0].lhs, "name");
  EXPECT_EQ(stmt.attr[0].rhs, "");

  rd = nullptr;
}
TEST_F(Hif_test, strings_match) {
  std::string fname("hif_test_strings_match");

  std::vector<std::string> inp_vector;
  std::vector<std::string> out_vector;

  {
    auto wr = Hif_write::create(fname, "testtool", "0.0.4");
    EXPECT_NE(wr, nullptr);

    for (int64_t i = 0; i < 1024; ++i) {
      Hif_base::Statement stmt;

      if (i & 1) {
        stmt = Hif_write::create_assign();
      } else {
        stmt = Hif_write::create_node();
      }

      std::string lhs = get_random_string();
      std::string rhs = get_random_string();

      inp_vector.emplace_back(lhs);
      out_vector.emplace_back(rhs);

      stmt.add_input(lhs, i);
      stmt.add_output(rhs, i);

      wr->add(stmt);
    }

    // wr out of scope, closes the Hif_write
  }

  {
    auto rd = Hif_read::open(fname);
    EXPECT_NE(rd, nullptr);

    int conta = 0;
    rd->each(
        [&conta, &inp_vector, &out_vector](const Hif_base::Statement &stmt2) {
          if (conta & 1) {
            EXPECT_TRUE(stmt2.is_assign());
          } else {
            EXPECT_TRUE(stmt2.is_node());
          }

          EXPECT_TRUE(stmt2.attr.empty()); // FIXME: add random attributes too
          EXPECT_EQ(stmt2.io.size(), 2);

          for (const auto &io : stmt2.io) {
            EXPECT_TRUE(io.is_lhs_string());
            EXPECT_TRUE(io.is_rhs_int64());

            auto v = io.get_rhs_int64();
            EXPECT_EQ(conta, v);

            if (io.input) {
              EXPECT_EQ(inp_vector[conta], io.lhs);
            } else {
              EXPECT_EQ(out_vector[conta], io.lhs);
            }
          }

          ++conta;
        });

    EXPECT_EQ(conta, inp_vector.size());
    EXPECT_EQ(conta, out_vector.size());
  }

  { // Same test but check different API
    auto rd = Hif_read::open(fname);
    EXPECT_NE(rd, nullptr);

    rd->next_stmt(); // Skip verstion/tool

    int conta = 0;
    do {
      auto stmt2 = rd->get_current_stmt();

      if (conta & 1) {
        EXPECT_TRUE(stmt2.is_assign());
      } else {
        EXPECT_TRUE(stmt2.is_node());
      }

      EXPECT_TRUE(stmt2.attr.empty()); // FIXME: add random attributes too
      EXPECT_EQ(stmt2.io.size(), 2);

      for (const auto &io : stmt2.io) {
        EXPECT_TRUE(io.is_lhs_string());
        EXPECT_TRUE(io.is_rhs_int64());

        auto v = io.get_rhs_int64();
        EXPECT_EQ(conta, v);

        if (io.input) {
          EXPECT_EQ(inp_vector[conta], io.lhs);
        } else {
          EXPECT_EQ(out_vector[conta], io.lhs);
        }
      }

      ++conta;
    } while (rd->next_stmt());

    EXPECT_EQ(conta, inp_vector.size());
    EXPECT_EQ(conta, out_vector.size());
  }
}

