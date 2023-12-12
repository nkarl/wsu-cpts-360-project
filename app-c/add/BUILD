cc_library(
  name = "add",
  srcs = ["add.cc",],
  hdrs = ["add.h"], 
  visibility = ["//visibility:public"]
)

cc_test(
  name = "add_test",
  size = "small",
  srcs =["add_test.cc"], 
  deps = [
    "@com_google_googletest//:gtest_main",
    ":add"
  ],
)
