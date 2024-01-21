cmake_minimum_required(VERSION 3.0.0)
project(mandelbrot_opencl VERSION 0.1.0)


set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")

if(NOT USE_SYSTEM_LIBS)

  include(ExternalProject)


  ExternalProject_Add(OpenCL-SDK 
      GIT_REPOSITORY "https://github.com/KhronosGroup/OpenCL-SDK"
      GIT_TAG "main"
      CMAKE_ARGS 
      -DBUILD_TESTING=OFF
      -DBUILD_DOCS=OFF
      -DBUILD_EXAMPLES=OFF
      -DBUILD_TESTS=OFF
      -DOPENCL_SDK_BUILD_SAMPLES=OFF
      -DOPENCL_SDK_TEST_SAMPLES=OFF
    
  )

  ExternalProject_Get_Property(OpenCL-SDK SOURCE_DIR)

  set(OpenCL_INCLUDE_DIR ${SOURCE_DIR}install/include)
  set(OPENCL_HPP ${SOURCE_DIR}install/include)



  if (WIN32)
    set(OpenCL_LIBRARY ${SOURCE_DIR}/install/lib/OpenCL.lib)
  else()
    set(OpenCL_LIBRARY ${SOURCE_DIR}/install/lib/libOpenCL.so)
  endif()
endif()

add_executable(mandelbrot_opencl main.cpp)

add_dependencies(mandelbrot_opencl OpenCL-SDK)

add_subdirectory(${PROJECT_SOURCE_DIR}/third_party/vexcl)

target_include_directories(mandelbrot_opencl PUBLIC VexCL::Common)

target_link_libraries(mandelbrot_opencl PUBLIC VexCL::OpenCL)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
