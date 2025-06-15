add_library(align align.c)

add_executable(test_align test_align.cpp)
target_link_libraries(test_align align gtest_main)

add_test(NAME AlignTest COMMAND test_align)