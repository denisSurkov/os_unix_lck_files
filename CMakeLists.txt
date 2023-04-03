cmake_minimum_required(VERSION 3.25)
project(os_unix_lck_files C)

set(CMAKE_C_STANDARD 23)

add_executable(os_unix_lck_files src/main.c src/files_locking.c src/files_locking.h)
