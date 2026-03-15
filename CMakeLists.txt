cmake_minimum_required(VERSION 3.20)
project(CPP_LSP_API VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR     "${CMAKE_CURRENT_LIST_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include")

add_library(CPP_LSP_API "${SRC_DIR}/LSP.cpp")
target_include_directories(CPP_LSP_API PUBLIC "${INCLUDE_DIR}")

# ------------------------------------------------------------------------------
# DEPENDENCY MANAGER
# ------------------------------------------------------------------------------

file(DOWNLOAD
        "https://raw.githubusercontent.com/stort0/cmake-utils/refs/heads/main/GetCMakeUtils.cmake"
        "${CMAKE_BINARY_DIR}/GetCMakeUtils.cmake")
include("${CMAKE_BINARY_DIR}/GetCMakeUtils.cmake")

# ------------------------------------------------------------------------------
# DEPENDENCIES
# ------------------------------------------------------------------------------

include(GetProject)

get_project(
        GIT_REPOSITORY "https://github.com/stephenberry/glaze.git"
        VERSION LATEST)

target_link_libraries(CPP_LSP_API PRIVATE
        glaze::glaze)

target_include_directories(CPP_LSP_API PRIVATE
        "${glaze_SOURCE}/include")
