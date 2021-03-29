cmake_minimum_required(VERSION 3.17)
project(DSA_zad_2 C)

set(CMAKE_C_STANDARD 99)

add_executable(DSA_zad_2 main.c AVL_tree.c AVL_tree.h LP_hash.c LP_hash.h SPLAY_tree.c SPLAY_tree.h CHAINING_hash.c CHAINING_hash.h)