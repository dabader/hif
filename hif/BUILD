load("@rules_cc//cc:defs.bzl", "cc_binary", "cc_test")
load("@pybind11_bazel//:build_defs.bzl", "pybind_extension")

cc_library(
    name = "hif",
    srcs = glob(
        ["*.cpp"],
        exclude = ["*test*.cpp", "*bench*.cpp", "hif_pybind.cpp"],
    ),
    hdrs = glob(["*.hpp"]),
    visibility = ["//visibility:public"],
    deps = [
      "@com_google_absl//absl/container:flat_hash_map",
    ],
)

cc_library(
    name = "hif_py",
    srcs = ["hif_pybind.cpp"],
    deps = [
        ":hif",
        "@pybind11//:pybind11_embed",
     ],
    visibility = ["//visibility:public"],
)         

cc_shared_library(
    name = "hif_py_shared",
    deps = [
        ":hif_py",
    ],
    shared_lib_name = "hif_pybind.so",
    visibility = ["//visibility:public"],
)

