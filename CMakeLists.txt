cmake_minimum_required(VERSION 3.14)
project(Tosa2Linalg VERSION 0.1.0 LANGUAGES CXX C)

# ============================================================================
# C++ Standard & Compiler Settings
# ============================================================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ============================================================================
# Options
# ============================================================================
option(TOSA2LINALG_BUILD_TESTS "Build test suite" ON)
option(TOSA2LINALG_BUILD_EXAMPLES "Build example applications" ON)
option(TOSA2LINALG_ENABLE_SANITIZERS "Enable AddressSanitizer/UBSan for debug builds" ON)

# ============================================================================
# Find Dependencies
# ============================================================================
find_package(Protobuf REQUIRED)
find_package(MLIR REQUIRED CONFIG)
list(APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}")
include(TableGen)
include(AddMLIR)


include(FetchContent)
if(TOSA2LINALG_BUILD_TESTS)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)
    enable_testing()
endif()

# ============================================================================


set(TOSA2LINALG_BASE_FLAGS
    -Wall -Wextra
    -Wc++14-compat
    -Wmissing-declarations
    -Wcast-align
    -Wcast-qual
    -Wchar-subscripts
    -Wconversion
    -Wctor-dtor-privacy
    -Wempty-body
    -Wfloat-equal
    -Wformat-security
    -Winline
    -Wnon-virtual-dtor
    -Woverloaded-virtual
    -Wpacked
    -Wpointer-arith
    -Winit-self
    -Wredundant-decls
    -Wshadow
    -Wsign-conversion
    -Wsign-promo
    -Wstrict-overflow=2
    -Wsuggest-override
    -Wswitch-default
    -Wundef
    -Wunreachable-code
    -Wunused
    -Wvariadic-macros
    -Wno-missing-field-initializers
    -Wno-narrowing
    -Wno-old-style-cast
    -Wno-varargs
    -Wstack-protector
    -fcheck-new
    -fsized-deallocation
    -fstack-protector
    -fstrict-overflow
    -fno-omit-frame-pointer
    -fPIE
    -fdiagnostics-all-candidates
    -Werror=vla
)


if(TOSA2LINALG_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        set(SANITIZER_FLAGS "-fsanitize=address,undefined")
        list(APPEND TOSA2LINALG_BASE_FLAGS ${SANITIZER_FLAGS})
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${SANITIZER_FLAGS}")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${SANITIZER_FLAGS}")
    endif()
endif()

# ============================================================================
# Core Library: Tosa2Linalg
# ============================================================================
add_library(tosa2linalg_core STATIC)

target_compile_definitions(tosa2linalg_core PUBLIC
    PROJECT_ROOT="${CMAKE_CURRENT_SOURCE_DIR}"
)

set(TOSA2LINALG_SRC_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/
)
set(TOSA2LINALG_INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

set(TOSA2LINALG_SOURCES "")
foreach(dir ${TOSA2LINALG_SRC_DIRS})
    file(GLOB_RECURSE dir_sources "${dir}/*.cpp")
    list(APPEND TOSA2LINALG_SOURCES ${dir_sources})
endforeach()

set(TOSA2LINALG_INCLUDES "")
foreach(dir ${TOSA2LINALG_INCLUDE_DIRS})
    file(GLOB_RECURSE dir_sources "${dir}/*.hpp")
    list(APPEND TOSA2LINALG_INCLUDES ${dir_sources})
endforeach()


target_sources(tosa2linalg_core
    PRIVATE
        ${TOSA2LINALG_SOURCES}
        ${PROTO_SOURCES}
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS include
        FILES
            ${TOSA2LINALG_INCLUDES}
)

target_include_directories(tosa2linalg_core
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${Protobuf_INCLUDE_DIRS}
)

target_link_libraries(tosa2linalg_core
    PUBLIC
        protobuf::libprotobuf
)
target_link_libraries(tosa2linalg_core PRIVATE
    MLIRIR
    MLIRPass
    MLIRTransforms

    MLIRReconcileUnrealizedCasts
    MLIRTargetLLVMIRExport
    MLIRBuiltinToLLVMIRTranslation
    MLIRLLVMToLLVMIRTranslation
    MLIRToLLVMIRTranslationRegistration

    MLIRFuncDialect
    MLIRArithDialect
    MLIRMemRefDialect

    MLIRLinalgToStandard
    MLIRTensorToLinalg
    MLIRArithToLLVM
    MLIRMemRefToLLVM
    MLIRFuncToLLVM
    MLIRSCFToControlFlow


    MLIRTosaTransforms
    MLIRTosaToLinalg
    MLIRTosaToArith
    MLIRSCFToControlFlow
    MLIRFuncToLLVM
    MLIRArithToLLVM
    MLIRMemRefToLLVM
    MLIRLLVMCommonConversion

    MLIRLinalgDialect
    MLIRTensorDialect
    MLIRFuncDialect
    MLIRTosaDialect
    LLVMCore
    LLVMSupport
)

target_compile_options(tosa2linalg_core PRIVATE
    ${TOSA2LINALG_BASE_FLAGS}
    -Werror
    -Wno-error=unused-parameter
    -Wno-error=unused-variable
    -Wno-error=unused-but-set-variable
    -Wno-error=sign-conversion
)

set_source_files_properties(
    ${PROTO_SOURCES}
    PROPERTIES
        COMPILE_FLAGS "-w -Wno-error"
)


# ============================================================================
# Main Executable
# ============================================================================
add_executable(tosa2linalg main.cpp)

target_link_libraries(tosa2linalg PRIVATE tosa2linalg_core)

if(TOSA2LINALG_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(tosa2linalg PRIVATE ${SANITIZER_FLAGS})
    endif()
endif()

# ============================================================================
# Testing System
# ============================================================================
if(TOSA2LINALG_BUILD_TESTS)
    file(GLOB TEST_SOURCES tests/*.cpp tests/**/*.cpp)

    if(TEST_SOURCES)
        add_executable(tosa2linalg_tests ${TEST_SOURCES})

        target_link_libraries(tosa2linalg_tests
            PRIVATE
                tosa2linalg_core
                GTest::gtest_main
                GTest::gmock
        )

        target_compile_definitions(tosa2linalg_tests PRIVATE
            TEST_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
        )

        if(TOSA2LINALG_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
            if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
                target_compile_options(tosa2linalg_tests PRIVATE ${SANITIZER_FLAGS})
            endif()
        endif()

        include(GoogleTest)
        gtest_discover_tests(tosa2linalg_tests
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        )

        add_custom_target(test_verbose
            COMMAND ${CMAKE_CTEST_COMMAND} --verbose
            DEPENDS tosa2linalg_tests
        )
    else()
        message(WARNING "No test files found in tests/ directory")
    endif()

    set(TOSA2LINALG_TEST_DATA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    if(EXISTS ${TOSA2LINALG_TEST_DATA_DIR})
        target_compile_definitions(tosa2linalg_tests PRIVATE
            TOSA2LINALG_TEST_DATA_DIR="${TOSA2LINALG_TEST_DATA_DIR}"
        )
    endif()
endif()

# ============================================================================
# Installation
# ============================================================================
include(GNUInstallDirs)

install(TARGETS tosa2linalg_core tosa2linalg
    EXPORT Tosa2LinalgTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILE_SET HEADERS
)

install(EXPORT Tosa2LinalgTargets
    FILE Tosa2LinalgTargets.cmake
    NAMESPACE Tosa2Linalg::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Tosa2Linalg
)

include(CMakePackageConfigHelpers)


write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/Tosa2LinalgConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/Tosa2LinalgConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/Tosa2LinalgConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Tosa2Linalg
)


# ============================================================================
# Clang-format
# ============================================================================

find_program(CLANG_FORMAT_BIN clang-format)

if(CLANG_FORMAT_BIN)
    file(GLOB_RECURSE ALL_CXX_SOURCE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp
    )

    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_BIN} --dry-run --Werror ${ALL_CXX_SOURCE_FILES}
        COMMENT "Checking code formatting..."
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_BIN} -i ${ALL_CXX_SOURCE_FILES}
        COMMENT "Formatting code..."
    )
endif()

# ============================================================================
# Summary
# ============================================================================
message(STATUS "")
message(STATUS "Tosa2Linalg Configuration:")
message(STATUS "  Version:        ${PROJECT_VERSION}")
message(STATUS "  Build Type:     ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ Standard:   ${CMAKE_CXX_STANDARD}")
message(STATUS "  Tests:          ${TOSA2LINALG_BUILD_TESTS}")
message(STATUS "  Sanitizers:     ${TOSA2LINALG_ENABLE_SANITIZERS}")
if(TOSA2LINALG_ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "  Sanitizer Flags: ${SANITIZER_FLAGS}")
endif()
message(STATUS "")


