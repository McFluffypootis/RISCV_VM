cmake_minimum_required(VERSION 3.15...3.31) 

project(RISCV_VM VERSION 1.0 DESCRIPTION "RISCV_VM" LANGUAGES C) 

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#file(GLOB SOURCES ${PROJECT_SOURCE_DIR}/${PROJECT_NAME}/*.?pp)
#list(FILTER SOURCES EXCLUDE REGEX ".test.cpp")
#list(FILTER SOURCES EXCLUDE REGEX "main.cpp")

#add_library(libLC3_VM STATIC ${SOURCES})

#set_target_properties(libLC3_VM PROPERTIES LINKER_LANGUAGE CXX)
add_executable(demo RISCV_VM/main.c)
#target_link_libraries(LC3_VM PUBLIC libLC3_VM)
