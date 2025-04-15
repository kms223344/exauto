cmake_minimum_required(VERSION 3.11)

project(
  exauto
  VERSION 0.1
  DESCRIPTION "Automaton Executor, purposed on general, commercial purpose."
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

set(SOURCES
  src/AutomataBase.cpp
  src/DFA.cpp
)

add_library(exauto STATIC ${SOURCES})

add_executable(test01 tests/test01.cpp)
add_executable(test02 tests/test02.cpp)
add_executable(genabc tests/gen/genabc.c)

target_link_libraries(test01 exauto)
target_link_libraries(test02 exauto)