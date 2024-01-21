cmake_minimum_required(VERSION 3.5.0)
project(VulkanGraphicsTest VERSION 0.1.0)

include(CTest)
enable_testing()

#Useful Variabls
set (DEPS ${PROJECT_SOURCE_DIR}/third_party)


#Disable stuff i don't need
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

#Required Packages
#find_package(Vulkan REQUIRED)


#Dependencies
add_subdirectory(${DEPS}/glfw)
add_subdirectory(${DEPS}/volk)

#Main Executable
add_executable(VulkanGraphicsTest ${PROJECT_SOURCE_DIR}/src/main.cpp)

#Include Directorys
#target_include_directories(VulkanGraphicsTest PUBLIC ${Vulkan_INCLUDE_DIR})
target_include_directories(VulkanGraphicsTest PUBLIC ${DEPS}/volk)
target_include_directories(VulkanGraphicsTest PUBLIC ${DEPS}/glfw/include)


#Libaries to Link
#target_link_libraries(VulkanGraphicsTest PRIVATE ${Vulkan_LIBRARY})
target_link_libraries(VulkanGraphicsTest PRIVATE volk)
target_link_libraries(VulkanGraphicsTest PRIVATE glfw)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
