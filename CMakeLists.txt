# CMake 프로그램의 최소 버전
cmake_minimum_required(VERSION 3.11)

# 프로젝트 정보
project(
  exauto
  VERSION 0.1
  DESCRIPTION "Automaton Executor, purposed on general, commercial purpose."
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_library(libexauto STATIC
            lib/AutomataBase.cpp lib/DFA.cpp)

add_executable(test01 tests/test01.cpp)

target_link_libraries(test01 libexauto)