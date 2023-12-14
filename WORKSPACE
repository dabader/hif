load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "new_git_repository")
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

#http_archive(
#    name = "rules_foreign_cc",
#    sha256 = "2a4d07cd64b0719b39a7c12218a3e507672b82a97b98c6a89d38565894cf7c51",
#    strip_prefix = "rules_foreign_cc-0.9.0",
#    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/refs/tags/0.9.0.tar.gz",
#)
#
#load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

# This sets up some common toolchains for building targets. For more details, please see
# https://bazelbuild.github.io/rules_foreign_cc/0.9.0/flatten.html#rules_foreign_cc_dependencies
#rules_foreign_cc_dependencies()

git_repository(
    name = "com_google_googletest",
    remote = "https://github.com/google/googletest",
    tag = "release-1.11.0",
)

#git_repository(
#    name = "glog",
#    remote = "https://github.com/google/glog.git",
#    tag = "v0.4.0",
#)

git_repository(
    name = "com_google_benchmark",
    remote = "https://github.com/google/benchmark.git",
    tag = "v1.7.1",
)

#git_repository(
#    name = "com_github_gflags_gflags",
#    remote = "https://github.com/gflags/gflags.git",
#    tag = "v2.2.2",
#)

# optional library. Used only when -DUSE_ABSL_MAP set (faster)
git_repository(
    name = "platforms",
    tag = "0.0.6",
    remote = "https://github.com/bazelbuild/platforms.git",
)

git_repository(
    name = "rules_cc",
    tag = "0.0.6",
    remote = "https://github.com/bazelbuild/rules_cc.git",
)

git_repository(
    name = "rules_python",
    tag = "0.23.1",
    remote = "https://github.com/bazelbuild/rules_python.git",
)

# Dependencies
## Python
git_repository(
    name = "pybind11_bazel",
    commit = "fc56ce8a8b51e3dd941139d329b63ccfea1d304b",
    remote = "https://github.com/pybind/pybind11_bazel.git",
)

new_git_repository(
    name = "pybind11",
    build_file = "@pybind11_bazel//:pybind11.BUILD",
    tag = "v2.10.3",
    remote = "https://github.com/pybind/pybind11.git",
)

load("@pybind11_bazel//:python_configure.bzl", "python_configure")
python_configure(name = "local_config_python", python_version = "3")


http_archive(
  name = "com_google_absl",
  urls = ["https://github.com/abseil/abseil-cpp/archive/04610889a913d29037205ca72e9d7fd7acc925fe.zip"],
  strip_prefix = "abseil-cpp-04610889a913d29037205ca72e9d7fd7acc925fe",
)

