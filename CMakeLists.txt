set(CMAKE_CXX_VERSION 20)

add_library(c3lt INTERFACE)
target_include_directories(c3lt INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
