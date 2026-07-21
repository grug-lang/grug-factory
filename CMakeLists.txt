cmake_minimum_required(VERSION 3.15)
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(GrugFactory C)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

option(ENABLE_ASAN "Enable AddressSanitizer and UBSan" OFF)

set(CMAKE_C_STANDARD 11)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.0
)
FetchContent_MakeAvailable(raylib)

FetchContent_Declare(
    grug_rs
    GIT_REPOSITORY https://github.com/grug-lang/grug-rs.git
    GIT_TAG generics
)
FetchContent_MakeAvailable(grug_rs)

find_program(CARGO_COMMAND cargo REQUIRED)

set(GRUG_RS_SOURCE_DIR ${grug_rs_SOURCE_DIR})
set(GRUG_RS_LIB ${GRUG_RS_SOURCE_DIR}/target/release/libgruggers.a) 

add_custom_target(
    build_grug_rs ALL
    COMMAND ${CARGO_COMMAND} build --release
    WORKING_DIRECTORY ${GRUG_RS_SOURCE_DIR}
    BYPRODUCTS ${GRUG_RS_LIB}
    COMMENT "Compiling grug-rs via Cargo..."
)

add_executable(grugtorio main.c cJSON.c)

target_compile_options(grugtorio PRIVATE -Wall -Wextra -Werror -Wpedantic)

target_compile_options(grugtorio PRIVATE
    $<$<CONFIG:Debug>:-g>
)

if(ENABLE_ASAN)
    message(STATUS "AddressSanitizer and UBSan are ENABLED")
    target_compile_options(grugtorio PRIVATE -fsanitize=address,undefined)
    target_link_options(grugtorio PRIVATE -fsanitize=address,undefined)

    set(LSAN_SUPP_FILE "${CMAKE_CURRENT_BINARY_DIR}/lsan.supp")
    file(WRITE ${LSAN_SUPP_FILE} "leak:InitWindow\n")
endif()

add_dependencies(grugtorio build_grug_rs)

target_include_directories(grugtorio SYSTEM PRIVATE ${GRUG_RS_SOURCE_DIR}/include)
target_link_libraries(grugtorio PRIVATE raylib ${GRUG_RS_LIB} dl pthread m)
