
cmake_minimum_required (VERSION 3.8)
include("${THIRD_PARTY}/vcpkg/scripts/buildsystems/vcpkg.cmake")

project(openGL)
file(GLOB_RECURSE DIR_SRCS "${CMAKE_SOURCE_DIR}/openGL/*.*")
add_executable (openGL ${DIR_SRCS})

find_package(glad REQUIRED)
find_package(assimp REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(stb REQUIRED)
add_definitions(-DSTB_IMAGE_IMPLEMENTATION)

target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)
target_link_libraries(${PROJECT_NAME} PRIVATE assimp::assimp)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)
target_include_directories(${PROJECT_NAME} PRIVATE stb)

