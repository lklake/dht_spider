cmake_minimum_required(VERSION 3.14)
project(dht C)

set(CMAKE_C_STANDARD 11)

add_executable(dht tree.c queue.c queue.h tree.h golbal_extern.c pool.c pool.h main.c str.c str.h hashtable.c hashtable.h bencode.c bencode.h test.c g_macros.h)
find_package(Threads REQUIRED)
target_link_libraries(dht Threads::Threads)