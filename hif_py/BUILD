load("@rules_python//python:defs.bzl", "py_test", "py_library")
load("@rules_cc//cc:defs.bzl", "cc_binary")
py_test(
    name = "hif_py_test",
    srcs = ["hif_py_test.py"],
    deps = [
      ":hif_py_shared_tmp",
    ],
)

py_library(
  name = "hif_py_shared_tmp",
  data = [":hif_pybind"],
  imports = ["."],
  visibility = ["//visibility:private"],
)

cc_binary(
    name = "hif_pybind",
    srcs = ["//hif:hif_py"],
    deps = [
        "//hif:hif_py",
        "@pybind11//:pybind11_embed",
     ],
    linkshared = 1,
    copts = ["-fPIC"],
    visibility = ["//visibility:private"],
)

