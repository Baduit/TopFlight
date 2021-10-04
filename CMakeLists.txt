cmake_minimum_required(VERSION 3.12)

project(TopFlightProject VERSION 0.1 DESCRIPTION "Topflight vm, intepreter, web components, etc")

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(CPM)


# Depts
find_package(Threads REQUIRED)

CPMAddPackage(
    NAME cpp_httplib
    GITHUB_REPOSITORY  yhirose/cpp-httplib
    VERSION 0.8.7
)

CPMAddPackage(
  NAME brigand
  GITHUB_REPOSITORY baduit/brigand
  GIT_TAG 96cef03d112916c8cb90b2bd60de111a08ce069f
  OPTIONS "TESTS OFF"
)

CPMAddPackage(
  NAME aggreget
  GITHUB_REPOSITORY  Baduit/Aggreget
  #VERSION 0.2.4
  GIT_TAG 4f8a6de0ed36dcd9e306075920535fa14dc80785
  OPTIONS "TESTS OFF" "EXAMPLE OFF"
)

# Lib
add_subdirectory(lib)

# Bins

add_executable(topflight ./bin/TopFlight/main.cpp)
target_compile_features(topflight PRIVATE cxx_std_20)
target_compile_options(topflight
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
target_link_libraries(topflight PRIVATE YoloVM)

add_executable(webTopflight ./bin/web/main.cpp)
target_compile_features(webTopflight PRIVATE cxx_std_20)
target_compile_options(webTopflight
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wfloat-equal -pedantic -Wredundant-decls -Wpointer-arith -O3>)
target_link_libraries(webTopflight PRIVATE YoloVM Threads::Threads)
target_include_directories(webTopflight PUBLIC ${cpp_httplib_SOURCE_DIR})

if (PRETTY_INTERPERTER)
    add_executable(prettytopflight ./bin/PrettyTopFlight/main.cpp)
    target_compile_features(prettytopflight PRIVATE cxx_std_20)
    target_compile_options(prettytopflight
                        PRIVATE
                        $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
    target_link_libraries(prettytopflight PRIVATE YoloVM)
    target_link_libraries(prettytopflight PRIVATE replxx)
endif()
