cmake_minimum_required(VERSION 3.20)
project(TreroInternal VERSION 0.1.0)

add_library(TreroInternal SHARED)

target_sources(TreroInternal PRIVATE "main.cpp")

add_subdirectory(minhook)
target_include_directories(TreroInternal PRIVATE "${CMAKE_CURRENT_LIST_DIR}/minhook/include")

target_link_libraries(TreroInternal PRIVATE minhook)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)