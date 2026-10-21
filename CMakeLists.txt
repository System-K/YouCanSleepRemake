cmake_minimum_required(VERSION 3.21)
project(YouCanSleepRemake VERSION 1.0.2 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(SKSEAPI)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake/Plugin.h.in" "${CMAKE_CURRENT_BINARY_DIR}/include/Plugin.h" @ONLY)

set(ENABLE_SKYRIM_SE ON CACHE BOOL "" FORCE)
set(ENABLE_SKYRIM_AE ON CACHE BOOL "" FORCE)
set(ENABLE_SKYRIM_VR ON CACHE BOOL "" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory("$ENV{COMMONLIB_SSE_FOLDER}" "${CMAKE_CURRENT_BINARY_DIR}/_deps/clib-build")
get_target_property(COMMONLIB_SRC_DIR CommonLibSSE SOURCE_DIR)
include("${COMMONLIB_SRC_DIR}/cmake/CommonLibSSE.cmake")

find_path(SIMPLEINI_INCLUDE_DIRS "SimpleIni.h")
find_package(nlohmann_json CONFIG REQUIRED)

# DKUtil includes "Impl/pch.hpp" and lowercase paths in Impl/Config and Impl/Hook
# (fine on Windows NTFS, not on case-sensitive Linux builds).
# Mirror DKUtil include tree into build directory with lowercase case aliases.
set(DKUTIL_GEN "${CMAKE_CURRENT_BINARY_DIR}/dkutil-include/DKUtil")
file(MAKE_DIRECTORY "${DKUTIL_GEN}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/extern/DKUtil/include/" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/dkutil-include")

set(LOWERCASE_ALIASES
    "Impl/pch.hpp:Impl/PCH.hpp"
    "Impl/Config/shared.hpp:Impl/Config/Shared.hpp"
    "Impl/Config/data.hpp:Impl/Config/Data.hpp"
    "Impl/Config/ini.hpp:Impl/Config/Ini.hpp"
    "Impl/Config/json.hpp:Impl/Config/Json.hpp"
    "Impl/Config/schema.hpp:Impl/Config/Schema.hpp"
    "Impl/Config/toml.hpp:Impl/Config/Toml.hpp"
    "Impl/Config/proxy.hpp:Impl/Config/Proxy.hpp"
    "Impl/Hook/shared.hpp:Impl/Hook/Shared.hpp"
    "Impl/Hook/api.hpp:Impl/Hook/API.hpp"
    "Impl/Hook/assembly.hpp:Impl/Hook/Assembly.hpp"
    "Impl/Hook/internal.hpp:Impl/Hook/Internal.hpp"
    "Impl/Hook/jit.hpp:Impl/Hook/JIT.hpp"
    "Impl/Hook/trampoline.hpp:Impl/Hook/Trampoline.hpp"
)

foreach(ALIAS_PAIR ${LOWERCASE_ALIASES})
    string(REPLACE ":" ";" PAIR_LIST ${ALIAS_PAIR})
    list(GET PAIR_LIST 0 DST)
    list(GET PAIR_LIST 1 SRC)
    file(CREATE_LINK "${DKUTIL_GEN}/${SRC}" "${DKUTIL_GEN}/${DST}" SYMBOLIC)
endforeach()

file(GLOB sources CONFIGURE_DEPENDS src/*.cpp)
file(GLOB headers CONFIGURE_DEPENDS src/*.h)

add_commonlibsse_plugin(${PROJECT_NAME}
    SOURCES ${headers} ${sources}
    NAME "YouCanSleepRemake"
    AUTHOR "Dropkicker / gottyduke"
    VERSION 1.0.2)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_precompile_headers(${PROJECT_NAME} PRIVATE src/PCH.h)
target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}/include"
    src
    "${CMAKE_CURRENT_BINARY_DIR}/dkutil-include"
    ${SIMPLEINI_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE nlohmann_json::nlohmann_json)
