cmake_minimum_required(VERSION 3.10)
project(portable_attributes LANGUAGES C)

add_library(portable_attributes INTERFACE)


install(FILES portable_attributes.h DESTINATION include)
