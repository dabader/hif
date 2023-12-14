#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "hif_write.hpp"
#include "hif_read.hpp"


namespace py = pybind11;

PYBIND11_MODULE(libhif_pybind, m) {
    py::class_<Hif_write, std::shared_ptr<Hif_write>>(m, "Hif_write")
        .def_static("create", 
                    (std::shared_ptr<Hif_write> (*)(std::string_view, std::string_view, std::string_view)) &Hif_write::create,
                    "Create a new Hif_write object")
        .def("add", &Hif_write::add, "Add a statement")
        .def_static("create_node", &Hif_write::create_node, "Create a new node statement")
        .def_static("create_assign", &Hif_write::create_assign, "Create a new assign statement")
        .def_static("create_attr", &Hif_write::create_attr, "Create a new attr statement")
        .def_static("create_open_call", &Hif_write::create_open_call, "Create a new open_call statement")
        .def_static("create_closed_call", &Hif_write::create_closed_call, "Create a new closed_call statement")
        .def_static("create_open_def", &Hif_write::create_open_def, "Create a new open_def statement")
        .def_static("create_closed_def", &Hif_write::create_closed_def, "Create a new closed_def statement")
        .def_static("create_end", &Hif_write::create_end, "Create a new end statement")
        .def_static("create_use", &Hif_write::create_use, "Create a new use statement");

        // Exposing protected method 'is_ok' as a public method in Python
    py::class_<Hif_base> base(m, "Hif_base");

    py::class_<Hif_base::Tuple_entry> te(m, "Tuple_entry");
    te.def_readonly("input", &Hif_base::Tuple_entry::input)
      .def_readonly("lhs", &Hif_base::Tuple_entry::lhs)
      .def_readonly("rhs", &Hif_base::Tuple_entry::rhs)
      .def_readonly("lhs_cat", &Hif_base::Tuple_entry::lhs_cat)
      .def_readonly("rhs_cat", &Hif_base::Tuple_entry::rhs_cat)
      .def("is_lhs_string", &Hif_base::Tuple_entry::is_lhs_string)
      .def("is_lhs_base2", &Hif_base::Tuple_entry::is_lhs_base2)
      .def("is_lhs_int64", &Hif_base::Tuple_entry::is_lhs_int64)
      .def("get_lhs_string", &Hif_base::Tuple_entry::get_lhs_string)
      .def("get_lhs_int64", &Hif_base::Tuple_entry::get_lhs_int64)
      .def("is_rhs_string", &Hif_base::Tuple_entry::is_rhs_string)
      .def("is_rhs_base2", &Hif_base::Tuple_entry::is_rhs_base2)
      .def("is_rhs_int64", &Hif_base::Tuple_entry::is_rhs_int64)
      .def("get_rhs_string", &Hif_base::Tuple_entry::get_rhs_string)
      .def("get_rhs_int64", &Hif_base::Tuple_entry::get_rhs_int64)

      .def("__repr__",
          [](const Hif_base::Tuple_entry &entry) {
            return "Tuple_entry {  Input: " + std::string(entry.input ? "true": "false") + ", lhs=" + entry.lhs + ", rhs=" + entry.rhs + ", lhs_cat= " + ToString(entry.lhs_cat) + ", rhs_cat= " + ToString(entry.rhs_cat) + " }"; 
            }
          )

      .def("__eq__", [](const Hif_base::Tuple_entry &self, const Hif_base::Tuple_entry &other) {
            return self == other;
       });


    py::class_<Hif_base::Statement> stmt(m, "Statement");
    stmt.def_readwrite("instance", &Hif_base::Statement::instance)
        .def_readonly("sclass", &Hif_base::Statement::sclass)
        .def_readonly("io", &Hif_base::Statement::io)
        .def_readonly("attr", &Hif_base::Statement::attr)
        .def(py::init<>())
        .def(py::init<Hif_base::Statement_class>())
        .def("add", (void (Hif_base::Statement::*)(bool inp, std::string_view)) &Hif_base::Statement::add)
        .def("add", (void (Hif_base::Statement::*)(bool inp, std::string_view, std::string_view)) &Hif_base::Statement::add)
        .def("add", (void (Hif_base::Statement::*)(bool inp, std::string_view, const int64_t &)) &Hif_base::Statement::add)
        .def("add", (void (Hif_base::Statement::*)(bool inp, Hif_base::String, Hif_base::String)) &Hif_base::Statement::add)
        .def("add", (void (Hif_base::Statement::*)(bool inp, Hif_base::String, Hif_base::Base2)) &Hif_base::Statement::add)
        .def("add", (void (Hif_base::Statement::*)(bool inp, Hif_base::String, Hif_base::Base4)) &Hif_base::Statement::add)
        .def("add", (void (Hif_base::Statement::*)(bool inp, Hif_base::String, Hif_base::Custom)) &Hif_base::Statement::add)
        .def("add", (void (Hif_base::Statement::*)(bool inp, Hif_base::Base2, Hif_base::String)) &Hif_base::Statement::add)
        .def("add", (void (Hif_base::Statement::*)(bool inp, Hif_base::Base2, Hif_base::Base2)) &Hif_base::Statement::add)
        .def("add", (void (Hif_base::Statement::*)(bool inp, Hif_base::Base2, Hif_base::Base4)) &Hif_base::Statement::add)
        .def("add", (void (Hif_base::Statement::*)(bool inp, Hif_base::Base2, Hif_base::Custom)) &Hif_base::Statement::add)

         .def("add_input", (void (Hif_base::Statement::*)(std::string_view)) &Hif_base::Statement::add_input) 
         .def("add_input", (void (Hif_base::Statement::*)(std::string_view, std::string_view)) &Hif_base::Statement::add_input) 
         .def("add_input", (void (Hif_base::Statement::*)(std::string_view, const int64_t &)) &Hif_base::Statement::add_input) 
         .def("add_input", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::String)) &Hif_base::Statement::add_input) 
         .def("add_input", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::Base2)) &Hif_base::Statement::add_input) 
         .def("add_input", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::Base4)) &Hif_base::Statement::add_input) 
         .def("add_input", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::Custom)) &Hif_base::Statement::add_input) 
         .def("add_input", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::String)) &Hif_base::Statement::add_input) 
         .def("add_input", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::Base2)) &Hif_base::Statement::add_input) 
         .def("add_input", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::Base4)) &Hif_base::Statement::add_input) 
         .def("add_input", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::Custom)) &Hif_base::Statement::add_input)

         .def("add_output", (void (Hif_base::Statement::*)(std::string_view)) &Hif_base::Statement::add_output) 
         .def("add_output", (void (Hif_base::Statement::*)(std::string_view, std::string_view)) &Hif_base::Statement::add_output) 
         .def("add_output", (void (Hif_base::Statement::*)(std::string_view, const int64_t &)) &Hif_base::Statement::add_output) 
         .def("add_output", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::String)) &Hif_base::Statement::add_output) 
         .def("add_output", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::Base2)) &Hif_base::Statement::add_output) 
         .def("add_output", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::Base4)) &Hif_base::Statement::add_output) 
         .def("add_output", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::Custom)) &Hif_base::Statement::add_output) 
         .def("add_output", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::String)) &Hif_base::Statement::add_output) 
         .def("add_output", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::Base2)) &Hif_base::Statement::add_output) 
         .def("add_output", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::Base4)) &Hif_base::Statement::add_output) 
         .def("add_output", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::Custom)) &Hif_base::Statement::add_output)

         .def("add_attr", (void (Hif_base::Statement::*)(std::string_view)) &Hif_base::Statement::add_attr) 
         .def("add_attr", (void (Hif_base::Statement::*)(std::string_view, std::string_view)) &Hif_base::Statement::add_attr) 
         .def("add_attr", (void (Hif_base::Statement::*)(std::string_view, const int64_t &)) &Hif_base::Statement::add_attr) 
         .def("add_attr", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::String)) &Hif_base::Statement::add_attr) 
         .def("add_attr", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::Base2)) &Hif_base::Statement::add_attr) 
         .def("add_attr", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::Base4)) &Hif_base::Statement::add_attr) 
         .def("add_attr", (void (Hif_base::Statement::*)(Hif_base::String, Hif_base::Custom)) &Hif_base::Statement::add_attr) 
         .def("add_attr", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::String)) &Hif_base::Statement::add_attr) 
         .def("add_attr", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::Base2)) &Hif_base::Statement::add_attr) 
         .def("add_attr", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::Base4)) &Hif_base::Statement::add_attr) 
         .def("add_attr", (void (Hif_base::Statement::*)(Hif_base::Base2, Hif_base::Custom)) &Hif_base::Statement::add_attr)

         .def("is_node", &Hif_base::Statement::is_node)
         .def("is_assign", &Hif_base::Statement::is_assign)
         .def("is_attr", &Hif_base::Statement::is_attr)
         .def("is_open_call", &Hif_base::Statement::is_open_call)
         .def("is_closed_call", &Hif_base::Statement::is_closed_call)
         .def("is_open_def", &Hif_base::Statement::is_open_def)
         .def("is_closed_def", &Hif_base::Statement::is_closed_def)
         .def("is_end", &Hif_base::Statement::is_end)
         .def("is_use", &Hif_base::Statement::is_use)
         .def("__repr__",
             [](const Hif_base::Statement &stmt) {
                 std::string result;
                 result += "<Statement class=" + ToString(stmt.sclass)
                        + ", Type=" + std::to_string(stmt.type)
                        + ", Instance=" + stmt.instance
                        + ", IO=[";
     
                 for (const auto& io : stmt.io) {
                     result += py::str(py::cast(io)).cast<std::string>() + ", ";
                 }
     
                 result += "], Attr=[";
                 for (const auto& attr : stmt.attr) {
                     result += py::str(py::cast(attr)).cast<std::string>() + ", ";
                 }
     
                 result += "]>";
                 return result;
             })



         .def("__eq__", [](const Hif_base::Statement &self, const Hif_base::Statement &other) {
             return self == other;
         });

  



       py::enum_<Hif_base::Statement_class>(base, "Statement_class")
         .value("Node", Hif_base::Statement_class::Node) 
         .value("Assign", Hif_base::Statement_class::Assign) 
         .value("Attr", Hif_base::Statement_class::Attr) 
         .value("Open_call", Hif_base::Statement_class::Open_call) 
         .value("Closed_call", Hif_base::Statement_class::Closed_call) 
         .value("Open_def", Hif_base::Statement_class::Open_def) 
         .value("Closed_def", Hif_base::Statement_class::Closed_def) 
         .value("End", Hif_base::Statement_class::End) 
         .value("Use", Hif_base::Statement_class::Use) 
         .export_values();

    py::enum_<Hif_base::ID_cat>(base, "ID_cat")
         .value("String_cat", Hif_base::ID_cat::String_cat)
         .value("Base2_cat", Hif_base::ID_cat::Base2_cat)
         .value("Base3_cat", Hif_base::ID_cat::Base3_cat)
         .value("Base4_cat", Hif_base::ID_cat::Base4_cat)
         .value("Custom_cat", Hif_base::ID_cat::Custom_cat)
         .export_values();


    py::class_<Hif_read, std::shared_ptr<Hif_read>>(m, "Hif_read")
         .def_static("open",
                     (std::shared_ptr<Hif_read> (*)(std::string_view)) &Hif_read::open, "Open a Hif file")
         .def("next_stmt", &Hif_read::next_stmt)
         .def("get_current_stmt", &Hif_read::get_current_stmt)
         .def("each", [](Hif_read &self, py::function py_callback) {
             self.each([&py_callback](const Hif_base::Statement &stmt) {
               py_callback(stmt);
             });
         })
         .def("get_tool", &Hif_read::get_tool)
         .def("get_version", &Hif_read::get_version);
}

