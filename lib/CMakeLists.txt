set(SRCS
    ./YoloVM/Value.cpp
    ./YoloVM/VirtualMachine.cpp
    ./YoloVM/Memory.cpp
    ./Interpreter/Interpreter.cpp
    ./Interpreter/Parser.cpp
  )


add_library(YoloVM ${SRCS})

target_compile_features(YoloVM PRIVATE cxx_std_20)

target_compile_options(YoloVM
                       PRIVATE
                       $<$<CXX_COMPILER_ID:MSVC>:/W3 /permissive- /TP>
                       $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wextra -Wall -Wsign-conversion -Wfloat-equal -pedantic -Wredundant-decls -Wshadow -Wpointer-arith -O3>)

target_include_directories(YoloVM PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(YoloVM PUBLIC brigand aggreget)