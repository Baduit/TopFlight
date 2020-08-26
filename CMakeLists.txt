cmake_minimum_required(VERSION 3.12)

project(TopFlightProject VERSION 0.1 DESCRIPTION "Topflight vm, intepreter, web components, etc")

find_package(Threads REQUIRED)

add_subdirectory(lib)

add_executable(topflight ./bin/TopFlight/main.cpp)
target_compile_features(topflight PRIVATE cxx_std_20)
target_compile_options(topflight
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)
target_link_libraries(topflight PRIVATE YoloVM)                       

add_executable(webTopflight ./bin/web/main.cpp ./bin/web/external/httplib.cc)
target_compile_features(webTopflight PRIVATE cxx_std_20)
target_compile_options(webTopflight
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wfloat-equal -pedantic -Wredundant-decls -Wpointer-arith -O3>)
target_link_libraries(webTopflight PRIVATE YoloVM Threads::Threads)                  