add_library(ring_buffer_lib ring_buffer.c)

add_executable(test_ring_buffer test_ring_buffer.cpp)
target_link_libraries(test_ring_buffer ring_buffer_lib gtest_main)

add_test(NAME RingBufferTest COMMAND test_ring_buffer)