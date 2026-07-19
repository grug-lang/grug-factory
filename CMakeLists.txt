cmake_minimum_required(VERSION 3.15)
project(GrugFactory C)

set(CMAKE_C_STANDARD 11)

option(ENABLE_ASAN "Enable AddressSanitizer and UndefinedBehaviorSanitizer" ON)

find_package(raylib 5.0 REQUIRED)

include(FetchContent)
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

add_executable(grugtorio main.c)

target_compile_options(grugtorio PRIVATE -Wall -Wextra -Werror -Wpedantic -g)

if(ENABLE_ASAN)
    message(STATUS "AddressSanitizer and UBSan are ENABLED")
    target_compile_options(grugtorio PRIVATE -fsanitize=address,undefined)
    target_link_options(grugtorio PRIVATE -fsanitize=address,undefined)

    set(LSAN_SUPP_FILE "${CMAKE_CURRENT_BINARY_DIR}/lsan.supp")
    file(WRITE ${LSAN_SUPP_FILE} "leak:InitWindow\n")

    set(LSAN_ENV_CMD ${CMAKE_COMMAND} -E env LSAN_OPTIONS=suppressions=${LSAN_SUPP_FILE}:print_suppressions=0)
else()
    set(LSAN_ENV_CMD "")
endif()

add_dependencies(grugtorio build_grug_rs)

target_include_directories(grugtorio SYSTEM PRIVATE ${GRUG_RS_SOURCE_DIR}/include)
target_link_libraries(grugtorio PRIVATE raylib ${GRUG_RS_LIB} dl pthread m)

add_custom_target(run-grugtorio
    COMMAND ${LSAN_ENV_CMD} $<TARGET_FILE:grugtorio>
    DEPENDS grugtorio
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)
