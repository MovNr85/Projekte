cmake_minimum_required(VERSION 3.0.0)
project(Springerproblem VERSION 0.1.0)

include(CTest)
enable_testing()


find_package(Boost REQUIRED)




add_executable(Springerproblem main.cpp)


target_include_directories(Springerproblem PRIVATE ${Boost_INCLUDE_DIRS})



target_link_libraries(Springerproblem PRIVATE ${Boost_LIBRARIES})

target_compile_options(Springerproblem PUBLIC "-march=native" "-O3")

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
